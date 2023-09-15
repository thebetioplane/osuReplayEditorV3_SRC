using System;
using System.Windows.Forms;
using System.IO;
using System.Runtime.InteropServices;
using System.Linq;

namespace osuReplayEditor
{
    public partial class MainForm : Form
    {
#if DEBUG
        private const string OSR_ON_LOAD = "hhh.osr";
#endif
        private int songMin = 0;
        private int songMax = 1;
        private Timer FrameTimer = new Timer();
        private Timer ScrollBarTimer = new Timer();
        private MetadataEditor.MetadataEditorForm MetadataEditorForm;
        private ConfigEditor.ConfigEditorForm ConfigEditorForm;
        private bool scrubRight = false;
        private bool scrubLeft = false;
        private float scrubMult = 1.0f;
        private bool ctrlKeyDown = false;
        private int selectedHitObject = -1;

        private bool visualMapInvert = false;
        private float originalAR = 0.0f;
        private float originalOD = 0.0f;
        private float originalCS = 0.0f;

        private bool VisualMapInvert
        {
            get => visualMapInvert;
            set
            {
                visualMapInvert = value;
                flipBeatmapBtn.Text = value ? "Un-Flip Beatmap" : "Flip Beatmap";
                API.VisualMapInvert(value);
            }
        }


        public MainForm()
        {
            InitializeComponent();
            this.MetadataEditorForm = new MetadataEditor.MetadataEditorForm();
            this.ConfigEditorForm = new ConfigEditor.ConfigEditorForm();
            this.ConfigEditorForm.Standalone = false;
            this.canvas.Begin();
            this.volumeBar.Value = API.CfgGetVolume();
            this.volumeBar_ValueChanged(null, null);
            this.ChangeCursorModeNoSave(API.CfgGetCursorMode());
            this.canvas.MouseWheel += canvas_MouseWheel;
            this.MetadataEditorForm.FormClosed += MetadataEditorForm_FormClosed;
        }

        private void MetadataEditorForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            UpdateModEffects();
        }

        private void UpdateModEffects()
        {
            // ---------------------------
            // DoubleTime and halftime settings:
            // ---------------------------
            if (MetadataEditorForm.IsHalfTime && MetadataEditorForm.IsDoubleTime)
            {
                ErrorMessage("This replay has incompatible mods HalfTime and DoubleTime applied.");
            }
            // Either both enabled (error state) or both disabled should be NoMod
            if (MetadataEditorForm.IsHalfTime == MetadataEditorForm.IsDoubleTime)
            {
                this.playbackRadio100x.Checked = true;
                this.ChangePlaybackSpeed();
            }
            else if (MetadataEditorForm.IsDoubleTime)
            {
                this.playbackRadio150x.Checked = true;
                this.ChangePlaybackSpeed();
            }
            else if (MetadataEditorForm.IsHalfTime)
            {
                this.playbackRadio075x.Checked = true;
                this.ChangePlaybackSpeed();
            }

            // ---------------------------
            // Easy and hardrock settings:
            // ---------------------------
            if (MetadataEditorForm.IsEasy && MetadataEditorForm.IsHardRock)
            {
                ErrorMessage("This replay has incompatible mods Easy and HardRock applied.");
            }
            // Either both enabled (error state) or both disabled should be NoMod
            if (MetadataEditorForm.IsEasy == MetadataEditorForm.IsHardRock)
            {
                VisualMapInvert = false;
                API.SetBeatmapAR(originalAR);
                API.SetBeatmapOD(originalOD);
                API.SetBeatmapCS(originalCS);
            }
            else if (MetadataEditorForm.IsHardRock)
            {
                VisualMapInvert = true;
                API.SetBeatmapAR(Math.Min(originalAR * 1.4f, 10.0f));
                API.SetBeatmapOD(Math.Min(originalOD * 1.4f, 10.0f));
                API.SetBeatmapCS(Math.Min(originalCS * 1.3f, 10.0f));
            }
            else if (MetadataEditorForm.IsEasy)
            {
                VisualMapInvert = false;
                API.SetBeatmapAR(originalAR * 0.5f);
                API.SetBeatmapOD(originalOD * 0.5f);
                API.SetBeatmapCS(originalCS * 0.5f);
            }
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            this.FrameTimer.Interval = 15;
            this.FrameTimer.Tick += FrameTimer_Tick;
            this.FrameTimer.Start();
            this.ScrollBarTimer.Interval = 333;
            this.ScrollBarTimer.Tick += ScrollBarTimer_Tick;
            this.ScrollBarTimer.Start();
            this.updateTimestampCB.Checked = API.CfgGetUpdateTimeStampOnExit() == 1;
            this.Text += $" [{GetDllBuildLabel()}]";
#if DEBUG
            LoadReplay(OSR_ON_LOAD);
            this.Text += " (C# DEBUG)";
#endif
        }

        public void ErrorMessage(string msg)
        {
            MessageBox.Show(this, msg, "Error");
        }
        public static void ErrorMessageOwnerless(string msg)
        {
            MessageBox.Show(msg, "Error");
        }

        private string GetDllBuildLabel()
        {
            int len = 0;
            API.GetDllBuildLabel(null, ref len);
            byte[] buf = new byte[len];
            API.GetDllBuildLabel(buf, ref len);
            return System.Text.Encoding.ASCII.GetString(buf, 0, len);
        }

        private void LoadReplay(string fname)
        {
            if (API.LoadReplay(fname))
            {
                API.Pause();
                this.songMin = API.GetReplayStartMs();
                this.songMax = API.GetReplayEndMs();
                API.JumpTo(this.songMin);
                this.SetPauseBtnText(false);
                this.saveFileDialog1.FileName = Path.GetFileName(fname);
                MetadataEditorForm.FromAPI();
                originalAR = API.GetBeatmapAR();
                originalOD = API.GetBeatmapOD();
                originalCS = API.GetBeatmapCS();
                UpdateModEffects();
                acc_300s_tb.Text = "?";
                acc_100s_tb.Text = "?";
                acc_50s_tb.Text = "?";
                acc_misses_tb.Text = "?";
                acc_acc_tb.Text = "?";
                acc_avg_early_tb.Text = "?";
                acc_avg_late_tb.Text = "?";
                acc_ur_tb.Text = "?";
                nextObjectBtn.Enabled = false;
                nextMissBtn.Enabled = false;
                next50btn.Enabled = false;
                next100btn.Enabled = false;
                setHitObjectInfoBlank();
            }
            else
            {
                ErrorMessage("Replay and/or song failed to load");
            }
            API.SetVolume(this.volumeBar.Value / 10.0f);
        }

        private void FrameTimer_Tick(object sender, EventArgs e)
        {
            //int dt = (int)(15 * scrubMult);
            double dt = 15.0 * scrubMult;
            if (scrubRight)
            {
                API.RelJump(dt);
            }
            else if (scrubLeft)
            {
                API.RelJump(-dt);
            }
            this.canvas.Invalidate();
        }

        private void ScrollBarTimer_Tick(object sender, EventArgs e)
        {
            int ms = (int)API.GetTime();
            this.timelineControl.Value = ms / (double)this.songMax;
            int ms2 = ms % 1000;
            int sec = (ms / 1000) % 60;
            int min = (ms / 1000) / 60;
            this.songTimeLabel.Text = $"{min:D2}:{sec:D2}:{ms2:D3}";
        }

        private void muteBtn_Click(object sender, EventArgs e)
        {
            this.volumeBar.Value = 0;
        }

        private void volumeBar_ValueChanged(object sender, EventArgs e)
        {
            API.SetVolume(this.volumeBar.Value / 10.0f);
            this.volumeLabel.Text = $"Volume {this.volumeBar.Value * 10}%";
            API.CfgSetVolume(this.volumeBar.Value);
        }

        private void playbackRadio005x_CheckedChanged(object sender, EventArgs e)
        {
            this.ChangePlaybackSpeed();
        }

        private void playbackRadio010x_CheckedChanged(object sender, EventArgs e)
        {
            this.ChangePlaybackSpeed();
        }

        private void playbackRadio025x_CheckedChanged(object sender, EventArgs e)
        {
            this.ChangePlaybackSpeed();
        }

        private void playbackRadio050x_CheckedChanged(object sender, EventArgs e)
        {
            this.ChangePlaybackSpeed();
        }

        private void playbackRadio075x_CheckedChanged(object sender, EventArgs e)
        {
            this.ChangePlaybackSpeed();
        }

        private void playbackRadio100x_CheckedChanged(object sender, EventArgs e)
        {
            this.ChangePlaybackSpeed();
        }

        private void playbackRadio150x_CheckedChanged(object sender, EventArgs e)
        {
            this.ChangePlaybackSpeed();
        }

        

        private void playbackRadio400x_CheckedChanged(object sender, EventArgs e)
        {
            this.ChangePlaybackSpeed();
        }

        private void ChangePlaybackSpeed()
        {
            if (this.playbackRadio005x.Checked)
                scrubMult = 0.05f;
            else if(this.playbackRadio010x.Checked)
                scrubMult = 0.10f;
            else if (this.playbackRadio025x.Checked)
                scrubMult = 0.25f;
            else if (this.playbackRadio050x.Checked)
                scrubMult = 0.5f;
            else if (this.playbackRadio075x.Checked)
                scrubMult = 0.75f;
            else if (this.playbackRadio150x.Checked)
                scrubMult = 1.5f;
            else if (this.playbackRadio400x.Checked)
                scrubMult = 4.0f;
            else
                scrubMult = 1.0f;
            API.SetPlaybackSpeed(scrubMult);
        }

        private void ChangePlaybackSpeed(int percent)
        {
            throw new NotImplementedException();
        }

        private void cursorRadioNormal_CheckedChanged(object sender, EventArgs e)
        {
            this.ChangeCursorMode();
        }

        private void cursorRadioKeys_CheckedChanged(object sender, EventArgs e)
        {
            this.ChangeCursorMode();
        }

        private void cursorRadioPresses_CheckedChanged(object sender, EventArgs e)
        {
            this.ChangeCursorMode();
        }

        private void ChangeCursorMode()
        {
            int kind;
            if (this.cursorRadioNormal.Checked)
                kind = 0;
            else if (this.cursorRadioKeys.Checked)
                kind = 1;
            else
                kind = 2;
            API.SetCursorTrail(kind);
            API.CfgSetCursorMode(kind);
        }

        private void ChangeCursorModeNoSave(int kind)
        {
            switch (kind)
            {
                case 0:
                    this.cursorRadioNormal.Checked = true;
                    break;
                case 1:
                    this.cursorRadioKeys.Checked = true;
                    break;
                default:
                    this.cursorRadioPresses.Checked = true;
                    break;
            }
            API.SetCursorTrail(kind);
        }

        private void timelineControl_MouseClick(object sender, MouseEventArgs e)
        {
            this.timelineControl.Value = e.X / (double)this.timelineControl.Width;
            API.JumpTo((int)(this.timelineControl.Value * this.songMax));
        }

        private void timelineControl_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                this.timelineControl_MouseClick(sender, e);
            }
        }

        private void MainForm_DragEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                e.Effect = DragDropEffects.Copy;
            }
        }

        private void MainForm_DragDrop(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                string[] files = (string[])e.Data.GetData(DataFormats.FileDrop);
                this.LoadReplay(files[0]);
            }
        }

        private void togglePauseBtn_Click(object sender, EventArgs e)
        {
            API.TogglePause();
            this.SetPauseBtnText(API.IsPlaying());
        }

        private void SetPauseBtnText(bool isPlaying)
        {
            this.togglePauseBtn.Text = isPlaying ? "Pause" : "Play";
        }

        private void replayMetadataToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.MetadataEditorForm.FromAPI();
            this.MetadataEditorForm.ShowDialog();
        }

        private void configToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.ConfigEditorForm.ShowDialog();
        }

        private void markInBtn_Click(object sender, EventArgs e)
        {
            API.PlaceMarkIn();
        }

        private void markOutBtn_Click(object sender, EventArgs e)
        {
            API.PlaceMarkOut();
        }

        private void markMidBtn_Click(object sender, EventArgs e)
        {
            API.PlaceMarkMid();
        }

        private void markAllBtn_Click(object sender, EventArgs e)
        {
            API.PlaceMarkAll();
        }

        private void clearMarksBtn_Click(object sender, EventArgs e)
        {
            API.ClearMarks();
        }

        private void keyPressNoneBtn_Click(object sender, EventArgs e)
        {
            this.SetFrameKeyPress(0);
        }

        private void keyPress1Btn_Click(object sender, EventArgs e)
        {
            
            this.SetFrameKeyPress(this.isKeyboardCB.Checked ? 5 : 1);
        }

        private void keyPress2Btn_Click(object sender, EventArgs e)
        {
            this.SetFrameKeyPress(this.isKeyboardCB.Checked ? 10 : 2);
        }

        private void keyPress12Btn_Click(object sender, EventArgs e)
        {
            this.SetFrameKeyPress(this.isKeyboardCB.Checked ? 15 : 3);
        }

        private void SetFrameKeyPress(int mask)
        {
            if (!API.SetFrameKeyPress(mask))
            {
                ErrorMessage("Could not key change press data, do you have a valid selection?");
            }
        }

        private void canvas_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
                API.MouseDownRight(e.X, e.Y);
            else
                API.MouseDown(e.X, e.Y);
        }

        private void canvas_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
                API.MouseUpRight(e.X, e.Y);
            else
                API.MouseUp(e.X, e.Y);
        }

        private void canvas_MouseMove(object sender, MouseEventArgs e)
        {
            API.MouseMove(e.X, e.Y);
        }

        private void canvas_MouseWheel(object sender, MouseEventArgs e)
        {
            API.MouseWheel(e.X, e.Y, e.Delta > 0);
        }

        private void trailLengthBar_Scroll(object sender, EventArgs e)
        {
            int ms = this.trailLengthBar.Value * 100;
            API.SetCursorTrailLength(ms);
            this.cursorTrailValueLabel.Text = ms.ToString() + " ms";
        }

        private void Hotkey(Keys key)
        {
            if (ctrlKeyDown)
            {
                switch (key)
                {
                    case Keys.Z:
                        API.Undo();
                        break;
                    case Keys.Y:
                        API.Redo();
                        break;
                }
            }
            else
            {
                switch (key)
                {
                    case Keys.S:
                        this.togglePauseBtn_Click(null, null);
                        break;
                    case Keys.Q:
                        this.markInBtn_Click(null, null);
                        break;
                    case Keys.E:
                        this.markOutBtn_Click(null, null);
                        break;
                    case Keys.W:
                        this.markMidBtn_Click(null, null);
                        break;
                    case Keys.D1:
                        this.toolSelGrabRadioButton.Checked = true;
                        this.ChangeTool();
                        break;
                    case Keys.D2:
                        this.toolBrushRadioButton.Checked = true;
                        this.ChangeTool();
                        break;
                    case Keys.R:
                        this.zoomPanResetBtn_Click(null, null);
                        break;
                    case Keys.OemCloseBrackets:
                        this.incBrushRadiusTrackBarValue(5);
                        break;
                    case Keys.OemOpenBrackets:
                        this.incBrushRadiusTrackBarValue(-5);
                        break;
                    case Keys.Z:
                        this.keyPressNoneBtn_Click(null, null);
                        break;
                    case Keys.X:
                        this.keyPress1Btn_Click(null, null);
                        break;
                    case Keys.C:
                        this.keyPress2Btn_Click(null, null);
                        break;
                    case Keys.V:
                        this.keyPress12Btn_Click(null, null);
                        break;
                }
            }
        }

        protected override bool ProcessKeyPreview(ref Message msg)
        {
            const int WM_KEYDOWN = 0x100;
            const int WM_KEYUP = 0x101;
            if (msg.Msg != WM_KEYDOWN && msg.Msg != WM_KEYUP)
                return base.ProcessKeyPreview(ref msg);
            Keys keyData = (Keys)msg.WParam;
            bool isRight = keyData == Keys.D;
            bool isLeft = keyData == Keys.A;
            bool isCtrl = keyData == Keys.ControlKey;
            if (msg.Msg == WM_KEYDOWN)
            {
                if (isRight)
                {
                    scrubRight = true;
                    API.Pause();
                    this.SetPauseBtnText(false);
                }
                else if (isLeft)
                {
                    scrubLeft = true;
                    API.Pause();
                    this.SetPauseBtnText(false);
                }
                else if (isCtrl)
                {
                    ctrlKeyDown = true;
                }
                this.Hotkey(keyData);
            }
            else if (msg.Msg == WM_KEYUP)
            {
                if (isRight)
                {
                    scrubRight = false;
                }
                else if (isLeft)
                {
                    scrubLeft = false;
                }
                else if (isCtrl)
                {
                    ctrlKeyDown = false;
                }
            }
            return base.ProcessKeyPreview(ref msg);
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string replayPath = Config.mainConfig.ReplayDirPath;
            this.openFileDialog1.InitialDirectory = replayPath;
            this.openFileDialog1.RestoreDirectory = true;
            this.openFileDialog1.ShowDialog();
        }

        private void openFileDialog1_FileOk(object sender, System.ComponentModel.CancelEventArgs e)
        {
            this.LoadReplay(this.openFileDialog1.FileName);

            IntPtr ptr = API.GetMapPath();
            if (ptr == null) return;

            string path = Marshal.PtrToStringUni(ptr);
            API.FreeBuffer(ptr);

            this.openFileDialog2.InitialDirectory = Path.GetDirectoryName(path);
            this.openFileDialog2.FileName = Path.GetFileName(path);
            this.openFileDialog2.RestoreDirectory = true;
        }

        private void saveFile()
        {
            string replayPath = Config.mainConfig.ReplayDirPath;
            this.saveFileDialog1.InitialDirectory = replayPath;
            this.saveFileDialog1.RestoreDirectory = true;
            this.saveFileDialog1.ShowDialog();
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.saveFile();
        }

        private void exportAsosrToolStripMenuItem_Click(object sender, EventArgs e)
        {

            this.saveFile();
        }

        private void saveFileDialog1_FileOk(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if (API.CfgGetUpdateTimeStampOnExit() == 1)
            {
                MetadataEditorForm.PlayTimestamp = DateTime.Now;
                MetadataEditorForm.ToAPI();
                MetadataEditorForm.FromAPI();
                UpdateModEffects();
            }
            API.ExportAsOsr(this.saveFileDialog1.FileName);
        }

        private void githubToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("https://github.com/thebetioplane/OsuReplayEditorV3");
        }

        private void flipBeatmapBtn_Click(object sender, EventArgs e)
        {
            VisualMapInvert = !VisualMapInvert;
        }

        private void flipCursorDataBtn_Click(object sender, EventArgs e)
        {
            API.InvertCursorData();
        }

        private void zoomPanResetBtn_Click(object sender, EventArgs e)
        {
            API.ResetPanZoom();
        }

        private void zoomInBtn_Click(object sender, EventArgs e)
        {
            API.ZoomIn();
        }

        private void zoomOutBtn_Click(object sender, EventArgs e)
        {
            API.ZoomOut();
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            API.Cleanup();
        }

        private void updateTimestampCB_CheckedChanged(object sender, EventArgs e)
        {
            int value = updateTimestampCB.Checked ? 1 : 0;
            API.CfgSetUpdateTimeStampOnExit(value);
        }

        private void undoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            API.Undo();
        }

        private void redoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            API.Redo();
        }

        private void keybindReferenceToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show("A - scrub backwards\nD - scrub forwards\n\n(scrub speed dependent on playback speed... for finer control lower the playback speed)\n\nS - toggle play / pause\nQ - mark in\nE - mark out\nW - mark mid\nZ - set no keys for selection\nX - set K1 for selection\nC - set K2 for selection\nV - set K1 and K2 for selection\nMouse wheel - zoom\nRight click - pan\nR - reset zoom pan\nCtrl+Z - undo\nCtrl+Y - redo\n[ - decrease brush size\n] - increase brush size", "Keybind Reference", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private string getKindString(int kind)
        {
            switch (kind)
            {
                case 1:
                    return "Slider";
                case 2:
                    return "Spinner";
                case 3:
                    return "Slider Tick";
                case 4:
                    return "Slider End";
                default:
                    return "Circle";
            }
        }

        private void analyzeAccuracy(bool doTrace)
        {
            int num300 = 0, num100 = 0, num50 = 0, numMiss = 0;
            float accuracy = 0.0f, avgEarly = 0.0f, avgLate = 0.0f, unstableRate = 0.0f;
            API.AnalyzeAccuracy(doTrace, ref num300, ref num100, ref num50, ref numMiss, ref accuracy, ref avgEarly, ref avgLate, ref unstableRate);
            acc_300s_tb.Text = num300.ToString();
            acc_100s_tb.Text = num100.ToString();
            acc_50s_tb.Text = num50.ToString();
            acc_misses_tb.Text = numMiss.ToString();
            acc_acc_tb.Text = accuracy.ToString();
            acc_avg_early_tb.Text = avgEarly.ToString();
            acc_avg_late_tb.Text = avgLate.ToString();
            acc_ur_tb.Text = unstableRate.ToString();
            nextObjectBtn.Enabled = true;
            nextMissBtn.Enabled = true;
            next50btn.Enabled = true;
            next100btn.Enabled = true;
            setHitObjectInfoIndex(selectedHitObject);
        }

        private void analyzeAccBtn_Click(object sender, EventArgs e)
        {
            analyzeAccuracy(false);
        }

        private void setHitObjectInfo(int index, int kind, bool isMiss, int hitError, int points)
        {
            if (index < 0 || kind == -1)
            {
                setHitObjectInfoBlank();
            }
            else
            {
                currentHitObjectIdLabel.Text = $"#{index}";
                currentHitObjectLabel.Text = getKindString(kind);
                if (isMiss)
                {
                    switch (kind)
                    {
                        case 3:
                            hitObjectErrorLabel.Text = "Sliderbreak (Miss)";
                            break;
                        case 4:
                            hitObjectErrorLabel.Text = "SliderEnd Skip";
                            break;
                        default:
                            hitObjectErrorLabel.Text = "Miss";
                            break;
                    }
                    
                }
                else
                {
                    string hitErrorQualifier = (hitError > 0) ? "Late" : (hitError < 0) ? "Early" : "Perfect";
                    hitObjectErrorLabel.Text = $"{hitError} ms ({hitErrorQualifier})";
                }
                hitObjectPointsLabel.Text = points.ToString();
            }
        }
        private void setHitObjectInfoBlank()
        {
            currentHitObjectLabel.Text = "?";
            hitObjectErrorLabel.Text = "?";
            hitObjectPointsLabel.Text = "?";
        }
        private bool setHitObjectInfoIndex(int index)
        {
            selectedHitObject = index;
            if (index < 0)
            {
                setHitObjectInfoBlank();
                return false;
            }
            int kind = -1;
            bool isMiss = false;
            int hitError = 0;
            int points = 0;
            if (!API.GetHitInfo(index, ref kind, ref isMiss, ref hitError, ref points))
            {
                setHitObjectInfoBlank();
                return false;
            }
            setHitObjectInfo(index, kind, isMiss, hitError, points);
            return true;
        }
        private void nextMissBtn_Click(object sender, EventArgs e)
        {
            if (!setHitObjectInfoIndex(API.NextMiss()))
                MessageBox.Show("No more misses");
        }
        private void next50btn_Click(object sender, EventArgs e)
        {
            if (!setHitObjectInfoIndex(API.Next50()))
                MessageBox.Show("No more 50s");
        }
        private void next100btn_Click(object sender, EventArgs e)
        {
            if (!setHitObjectInfoIndex(API.Next100()))
                MessageBox.Show("No more 100s");
        }

        private void nextObjectBtn_Click(object sender, EventArgs e)
        {
            if (!setHitObjectInfoIndex(API.NextHitObject()))
                MessageBox.Show("No more hit objects");
        }

        private void quickLoadToolStripMenuItem_DropDownOpening(object sender, EventArgs e)
        {
            string path = Config.mainConfig.ReplayDirPath;
            if (path == null)
            {
                ErrorMessage("You do not have a replay folder specified. You can change this in config.txt.");
                return;
            }
            if (!Directory.Exists(path))
            {
                ErrorMessage("Your osu! replay folder does not exist. Check the path in config.txt");
                return;
            }
            quickLoadToolStripMenuItem.DropDownItems.Clear();
            string[] files = Directory.GetFiles(path, "*.osr", SearchOption.TopDirectoryOnly);
            ToolStripMenuItem parent = quickLoadToolStripMenuItem;
            const int groupBy = 30;
            int maxItems = files.Length;
            int counter = 0;
            foreach (string fullpath in files)
            {
                if (maxItems > groupBy && counter % groupBy == 0)
                {
                    parent = new ToolStripMenuItem($"{counter + 1} to {Math.Min(counter + groupBy, maxItems)}");
                    quickLoadToolStripMenuItem.DropDownItems.Add(parent);
                }
                ToolStripMenuItem item = new ToolStripMenuItem(Path.GetFileName(fullpath));
                item.Click += quickLoadItem_Click;
                parent.DropDownItems.Add(item);
                ++counter;
            }
        }

        private void quickLoadItem_Click(object sender, EventArgs e)
        {
            ToolStripMenuItem item = sender as ToolStripMenuItem;
            string replayPath = Config.mainConfig.ReplayDirPath;
            if (item != null && replayPath != null)
            {
                LoadReplay(Path.Combine(replayPath, item.Text));
            }
        }

        private void toolSelGrabRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            this.ChangeTool();
        }

        private void toolBrushRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            this.ChangeTool();
        }

        private void ChangeTool()
        {
            int whichTool;
            if (toolSelGrabRadioButton.Checked)
                whichTool = 0;
            else
                whichTool = 2;
            API.SetTool(whichTool);
        }

        private void brushRadiusTrackBar_ValueChanged(object sender, EventArgs e)
        {
            this.brushRadiusLabel.Text = this.brushRadiusTrackBar.Value.ToString();
            API.SetBrushRadius(brushRadiusTrackBar.Value);
        }

        private void incBrushRadiusTrackBarValue(int amt)
        {
            int value = this.brushRadiusTrackBar.Value + amt;
            int low = this.brushRadiusTrackBar.Minimum;
            int high = this.brushRadiusTrackBar.Maximum;
            if (value < low)
                value = low;
            else if (value > high)
                value = high;
            this.brushRadiusTrackBar.Value = value;
        }

        private void analyzeAccTraceToolStripMenuItem_Click(object sender, EventArgs e)
        {
#if DEBUG
            analyzeAccuracy(true);
#else
            var res = MessageBox.Show("This feature produces a log file with details on the decisions that the accuracy analyzer made. This is useful for reporting issues.\n\nWould you like to continue?", "Analyze Accuracy Trace", MessageBoxButtons.YesNoCancel);                Console.WriteLine(res);
            if (res == DialogResult.Yes)
            {
                analyzeAccuracy(true);
            }
#endif
        }

        private void drawFramesOnTimelineCB_CheckedChanged(object sender, EventArgs e)
        {
            API.SetDrawFramesOnTimeline(drawFramesOnTimelineCB.Checked);
        }

        private void moveFrameForwardBtn_Click(object sender, EventArgs e)
        {
            API.MoveFrames(1);
        }

        private void moveFrameBackBtn_Click(object sender, EventArgs e)
        {
            API.MoveFrames(-1);
        }

        private void centerCurrentFrameBtn_Click(object sender, EventArgs e)
        {
            API.CenterFrames();
        }

        private void addFrameBtn_Click(object sender, EventArgs e)
        {
            API.AddFrame();
        }

        private void deleteFrameBtn_Click(object sender, EventArgs e)
        {
            API.DeleteFrames();
        }

        private void toKeyboardBtn_Click(object sender, EventArgs e)
        {
            API.DeviceMarkAllFrames(true);
        }

        private void openFileDialog2_FileOk(object sender, System.ComponentModel.CancelEventArgs e)
        {
            string fullPath = this.openFileDialog2.FileName;

            string directory = Path.GetFileName(Path.GetDirectoryName(fullPath));
            string fileName = Path.GetFileName(fullPath);

            string relativePath = directory + '/' + fileName;

            bool result = API.ChangeHashOfBeatmap(relativePath);
            if(result)
            {
                System.Threading.Thread.Sleep(500);
                saveFile();
            }
            else
            {
                MessageBox.Show("Can't find the hash of the map. Try closing osu!", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }

        private void changeReplaysBeatmapToolStripMenuItem_Click(object sender, EventArgs e)
        {
            openFileDialog2.ShowDialog();
        }

        private void scaleFramesBar_Scroll(object sender, EventArgs e)
        {

        }

        private void MainForm_MouseDown(object sender, MouseEventArgs e)
        {
            this.ActiveControl = null;
        }

        private void scaleFramesBtn_Click(object sender, EventArgs e)
        {
            API.ScaleFrames((float)this.scaleFramesUd.Value);
        }

        private void drawHitWindowsCB_CheckedChanged(object sender, EventArgs e)
        {
            API.SetDrawHitWindows(drawHitWindowsCB.Checked);
        }

        private void drawSliderendRangeCB_CheckedChanged(object sender, EventArgs e)
        {
            API.SetDrawSliderendRange(drawSliderendRangeCB.Checked);
        }
    }
}
