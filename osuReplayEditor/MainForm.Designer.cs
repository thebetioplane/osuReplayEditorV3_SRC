
namespace osuReplayEditor
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exportAsosrToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.changeReplaysBeatmapToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.replayMetadataToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.undoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.redoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.quickLoadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.configToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.githubToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.githubToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.keybindReferenceToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.analyzeAccTraceToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.volumeBar = new System.Windows.Forms.TrackBar();
            this.volumeLabel = new System.Windows.Forms.Label();
            this.playbackSpeedLabel = new System.Windows.Forms.Label();
            this.playbackRadio025x = new System.Windows.Forms.RadioButton();
            this.playbackRadio050x = new System.Windows.Forms.RadioButton();
            this.playbackRadio075x = new System.Windows.Forms.RadioButton();
            this.playbackRadio100x = new System.Windows.Forms.RadioButton();
            this.playbackRadio150x = new System.Windows.Forms.RadioButton();
            this.playbackRadio005x = new System.Windows.Forms.RadioButton();
            this.playbackRadio400x = new System.Windows.Forms.RadioButton();
            this.playbackSpeedPanel = new System.Windows.Forms.Panel();
            this.playbackRadio010x = new System.Windows.Forms.RadioButton();
            this.muteBtn = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.cursorRadioPresses = new System.Windows.Forms.RadioButton();
            this.cursorRadioNormal = new System.Windows.Forms.RadioButton();
            this.cursorRadioKeys = new System.Windows.Forms.RadioButton();
            this.cursorTrailLabel = new System.Windows.Forms.Label();
            this.togglePauseBtn = new System.Windows.Forms.Button();
            this.songTimeLabelLabel = new System.Windows.Forms.Label();
            this.songTimeLabel = new System.Windows.Forms.Label();
            this.markInBtn = new System.Windows.Forms.Button();
            this.markOutBtn = new System.Windows.Forms.Button();
            this.markMidBtn = new System.Windows.Forms.Button();
            this.clearMarksBtn = new System.Windows.Forms.Button();
            this.markerInfoLabel = new System.Windows.Forms.Label();
            this.keypressInfoLabel = new System.Windows.Forms.Label();
            this.markAllBtn = new System.Windows.Forms.Button();
            this.keyPress1Btn = new System.Windows.Forms.Button();
            this.keyPress2Btn = new System.Windows.Forms.Button();
            this.keyPress12Btn = new System.Windows.Forms.Button();
            this.keyPressNoneBtn = new System.Windows.Forms.Button();
            this.trailLengthBar = new System.Windows.Forms.TrackBar();
            this.trailLengthInfoLabel = new System.Windows.Forms.Label();
            this.cursorTrailValueLabel = new System.Windows.Forms.Label();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.flipBeatmapBtn = new System.Windows.Forms.Button();
            this.flipCursorDataBtn = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.zoomPanResetBtn = new System.Windows.Forms.Button();
            this.zoomInBtn = new System.Windows.Forms.Button();
            this.zoomOutBtn = new System.Windows.Forms.Button();
            this.updateTimestampCB = new System.Windows.Forms.CheckBox();
            this.label3 = new System.Windows.Forms.Label();
            this.analyzeAccBtn = new System.Windows.Forms.Button();
            this.label6 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.nextMissBtn = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.acc_ur_tb = new System.Windows.Forms.Label();
            this.acc_avg_late_tb = new System.Windows.Forms.Label();
            this.acc_avg_early_tb = new System.Windows.Forms.Label();
            this.acc_acc_tb = new System.Windows.Forms.Label();
            this.acc_misses_tb = new System.Windows.Forms.Label();
            this.acc_50s_tb = new System.Windows.Forms.Label();
            this.acc_100s_tb = new System.Windows.Forms.Label();
            this.acc_300s_tb = new System.Windows.Forms.Label();
            this.next50btn = new System.Windows.Forms.Button();
            this.next100btn = new System.Windows.Forms.Button();
            this.panel2 = new System.Windows.Forms.Panel();
            this.toolSelGrabRadioButton = new System.Windows.Forms.RadioButton();
            this.toolBrushRadioButton = new System.Windows.Forms.RadioButton();
            this.label12 = new System.Windows.Forms.Label();
            this.brushRadiusTrackBar = new System.Windows.Forms.TrackBar();
            this.label13 = new System.Windows.Forms.Label();
            this.brushRadiusLabel = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.currentHitObjectIdLabel = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.hitObjectErrorLabel = new System.Windows.Forms.Label();
            this.hitObjectPointsLabel = new System.Windows.Forms.Label();
            this.label17 = new System.Windows.Forms.Label();
            this.nextObjectBtn = new System.Windows.Forms.Button();
            this.currentHitObjectLabel = new System.Windows.Forms.Label();
            this.isKeyboardCB = new System.Windows.Forms.CheckBox();
            this.drawFramesOnTimelineCB = new System.Windows.Forms.CheckBox();
            this.moveFrameBackBtn = new System.Windows.Forms.Button();
            this.moveFrameForwardBtn = new System.Windows.Forms.Button();
            this.centerCurrentFrameBtn = new System.Windows.Forms.Button();
            this.addFrameBtn = new System.Windows.Forms.Button();
            this.deleteFrameBtn = new System.Windows.Forms.Button();
            this.toKeyboardBtn = new System.Windows.Forms.Button();
            this.openFileDialog2 = new System.Windows.Forms.OpenFileDialog();
            this.scaleFramesBtn = new System.Windows.Forms.Button();
            this.scaleFramesUd = new System.Windows.Forms.NumericUpDown();
            this.drawHitWindowsCB = new System.Windows.Forms.CheckBox();
            this.drawSliderendRangeCB = new System.Windows.Forms.CheckBox();
            this.timelineControl = new osuReplayEditor.TimelineControl();
            this.canvas = new osuReplayEditor.Canvas();
            this.menuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.volumeBar)).BeginInit();
            this.playbackSpeedPanel.SuspendLayout();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trailLengthBar)).BeginInit();
            this.panel2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.brushRadiusTrackBar)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.scaleFramesUd)).BeginInit();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.replayMetadataToolStripMenuItem,
            this.editToolStripMenuItem,
            this.quickLoadToolStripMenuItem,
            this.configToolStripMenuItem,
            this.githubToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1579, 30);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openToolStripMenuItem,
            this.saveToolStripMenuItem,
            this.exportAsosrToolStripMenuItem,
            this.changeReplaysBeatmapToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(46, 26);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.Size = new System.Drawing.Size(257, 26);
            this.openToolStripMenuItem.Text = "Open";
            this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Enabled = false;
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(257, 26);
            this.saveToolStripMenuItem.Text = "Save";
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
            // 
            // exportAsosrToolStripMenuItem
            // 
            this.exportAsosrToolStripMenuItem.Name = "exportAsosrToolStripMenuItem";
            this.exportAsosrToolStripMenuItem.Size = new System.Drawing.Size(257, 26);
            this.exportAsosrToolStripMenuItem.Text = "Export as .osr";
            this.exportAsosrToolStripMenuItem.Click += new System.EventHandler(this.exportAsosrToolStripMenuItem_Click);
            // 
            // changeReplaysBeatmapToolStripMenuItem
            // 
            this.changeReplaysBeatmapToolStripMenuItem.Name = "changeReplaysBeatmapToolStripMenuItem";
            this.changeReplaysBeatmapToolStripMenuItem.Size = new System.Drawing.Size(257, 26);
            this.changeReplaysBeatmapToolStripMenuItem.Text = "Change replays beatmap";
            this.changeReplaysBeatmapToolStripMenuItem.Click += new System.EventHandler(this.changeReplaysBeatmapToolStripMenuItem_Click);
            // 
            // replayMetadataToolStripMenuItem
            // 
            this.replayMetadataToolStripMenuItem.Name = "replayMetadataToolStripMenuItem";
            this.replayMetadataToolStripMenuItem.Size = new System.Drawing.Size(136, 26);
            this.replayMetadataToolStripMenuItem.Text = "Replay Metadata";
            this.replayMetadataToolStripMenuItem.Click += new System.EventHandler(this.replayMetadataToolStripMenuItem_Click);
            // 
            // editToolStripMenuItem
            // 
            this.editToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.undoToolStripMenuItem,
            this.redoToolStripMenuItem});
            this.editToolStripMenuItem.Name = "editToolStripMenuItem";
            this.editToolStripMenuItem.Size = new System.Drawing.Size(49, 26);
            this.editToolStripMenuItem.Text = "Edit";
            // 
            // undoToolStripMenuItem
            // 
            this.undoToolStripMenuItem.Name = "undoToolStripMenuItem";
            this.undoToolStripMenuItem.Size = new System.Drawing.Size(128, 26);
            this.undoToolStripMenuItem.Text = "Undo";
            this.undoToolStripMenuItem.Click += new System.EventHandler(this.undoToolStripMenuItem_Click);
            // 
            // redoToolStripMenuItem
            // 
            this.redoToolStripMenuItem.Name = "redoToolStripMenuItem";
            this.redoToolStripMenuItem.Size = new System.Drawing.Size(128, 26);
            this.redoToolStripMenuItem.Text = "Redo";
            this.redoToolStripMenuItem.Click += new System.EventHandler(this.redoToolStripMenuItem_Click);
            // 
            // quickLoadToolStripMenuItem
            // 
            this.quickLoadToolStripMenuItem.Name = "quickLoadToolStripMenuItem";
            this.quickLoadToolStripMenuItem.Size = new System.Drawing.Size(97, 26);
            this.quickLoadToolStripMenuItem.Text = "Quick Load";
            this.quickLoadToolStripMenuItem.DropDownOpening += new System.EventHandler(this.quickLoadToolStripMenuItem_DropDownOpening);
            // 
            // configToolStripMenuItem
            // 
            this.configToolStripMenuItem.Name = "configToolStripMenuItem";
            this.configToolStripMenuItem.Size = new System.Drawing.Size(67, 26);
            this.configToolStripMenuItem.Text = "Config";
            this.configToolStripMenuItem.Click += new System.EventHandler(this.configToolStripMenuItem_Click);
            // 
            // githubToolStripMenuItem
            // 
            this.githubToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.githubToolStripMenuItem1,
            this.keybindReferenceToolStripMenuItem,
            this.analyzeAccTraceToolStripMenuItem});
            this.githubToolStripMenuItem.Name = "githubToolStripMenuItem";
            this.githubToolStripMenuItem.Size = new System.Drawing.Size(55, 26);
            this.githubToolStripMenuItem.Text = "Help";
            // 
            // githubToolStripMenuItem1
            // 
            this.githubToolStripMenuItem1.Name = "githubToolStripMenuItem1";
            this.githubToolStripMenuItem1.Size = new System.Drawing.Size(216, 26);
            this.githubToolStripMenuItem1.Text = "Github";
            this.githubToolStripMenuItem1.Click += new System.EventHandler(this.githubToolStripMenuItem1_Click);
            // 
            // keybindReferenceToolStripMenuItem
            // 
            this.keybindReferenceToolStripMenuItem.Name = "keybindReferenceToolStripMenuItem";
            this.keybindReferenceToolStripMenuItem.Size = new System.Drawing.Size(216, 26);
            this.keybindReferenceToolStripMenuItem.Text = "Keybind Reference";
            this.keybindReferenceToolStripMenuItem.Click += new System.EventHandler(this.keybindReferenceToolStripMenuItem_Click);
            // 
            // analyzeAccTraceToolStripMenuItem
            // 
            this.analyzeAccTraceToolStripMenuItem.Name = "analyzeAccTraceToolStripMenuItem";
            this.analyzeAccTraceToolStripMenuItem.Size = new System.Drawing.Size(216, 26);
            this.analyzeAccTraceToolStripMenuItem.Text = "Analyze Acc Trace";
            this.analyzeAccTraceToolStripMenuItem.Click += new System.EventHandler(this.analyzeAccTraceToolStripMenuItem_Click);
            // 
            // volumeBar
            // 
            this.volumeBar.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.volumeBar.Location = new System.Drawing.Point(1497, 64);
            this.volumeBar.Margin = new System.Windows.Forms.Padding(4);
            this.volumeBar.Name = "volumeBar";
            this.volumeBar.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.volumeBar.Size = new System.Drawing.Size(56, 530);
            this.volumeBar.TabIndex = 2;
            this.volumeBar.TickStyle = System.Windows.Forms.TickStyle.Both;
            this.volumeBar.Value = 2;
            this.volumeBar.ValueChanged += new System.EventHandler(this.volumeBar_ValueChanged);
            // 
            // volumeLabel
            // 
            this.volumeLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.volumeLabel.AutoSize = true;
            this.volumeLabel.Location = new System.Drawing.Point(1483, 44);
            this.volumeLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.volumeLabel.Name = "volumeLabel";
            this.volumeLabel.Size = new System.Drawing.Size(89, 16);
            this.volumeLabel.TabIndex = 3;
            this.volumeLabel.Text = "Volume 100%";
            // 
            // playbackSpeedLabel
            // 
            this.playbackSpeedLabel.AutoSize = true;
            this.playbackSpeedLabel.Location = new System.Drawing.Point(9, 44);
            this.playbackSpeedLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.playbackSpeedLabel.Name = "playbackSpeedLabel";
            this.playbackSpeedLabel.Size = new System.Drawing.Size(108, 16);
            this.playbackSpeedLabel.TabIndex = 4;
            this.playbackSpeedLabel.Text = "Playback Speed";
            // 
            // playbackRadio025x
            // 
            this.playbackRadio025x.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.playbackRadio025x.AutoSize = true;
            this.playbackRadio025x.Location = new System.Drawing.Point(12, 72);
            this.playbackRadio025x.Margin = new System.Windows.Forms.Padding(4);
            this.playbackRadio025x.Name = "playbackRadio025x";
            this.playbackRadio025x.Size = new System.Drawing.Size(61, 20);
            this.playbackRadio025x.TabIndex = 5;
            this.playbackRadio025x.Text = "0.25 x";
            this.playbackRadio025x.UseVisualStyleBackColor = true;
            this.playbackRadio025x.CheckedChanged += new System.EventHandler(this.playbackRadio025x_CheckedChanged);
            // 
            // playbackRadio050x
            // 
            this.playbackRadio050x.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.playbackRadio050x.AutoSize = true;
            this.playbackRadio050x.Location = new System.Drawing.Point(12, 100);
            this.playbackRadio050x.Margin = new System.Windows.Forms.Padding(4);
            this.playbackRadio050x.Name = "playbackRadio050x";
            this.playbackRadio050x.Size = new System.Drawing.Size(61, 20);
            this.playbackRadio050x.TabIndex = 6;
            this.playbackRadio050x.Text = "0.50 x";
            this.playbackRadio050x.UseVisualStyleBackColor = true;
            this.playbackRadio050x.CheckedChanged += new System.EventHandler(this.playbackRadio050x_CheckedChanged);
            // 
            // playbackRadio075x
            // 
            this.playbackRadio075x.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.playbackRadio075x.AutoSize = true;
            this.playbackRadio075x.Location = new System.Drawing.Point(12, 128);
            this.playbackRadio075x.Margin = new System.Windows.Forms.Padding(4);
            this.playbackRadio075x.Name = "playbackRadio075x";
            this.playbackRadio075x.Size = new System.Drawing.Size(91, 20);
            this.playbackRadio075x.TabIndex = 7;
            this.playbackRadio075x.Text = "0.75 x (HT)";
            this.playbackRadio075x.UseVisualStyleBackColor = true;
            this.playbackRadio075x.CheckedChanged += new System.EventHandler(this.playbackRadio075x_CheckedChanged);
            // 
            // playbackRadio100x
            // 
            this.playbackRadio100x.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.playbackRadio100x.AutoSize = true;
            this.playbackRadio100x.Checked = true;
            this.playbackRadio100x.Location = new System.Drawing.Point(12, 157);
            this.playbackRadio100x.Margin = new System.Windows.Forms.Padding(4);
            this.playbackRadio100x.Name = "playbackRadio100x";
            this.playbackRadio100x.Size = new System.Drawing.Size(61, 20);
            this.playbackRadio100x.TabIndex = 8;
            this.playbackRadio100x.TabStop = true;
            this.playbackRadio100x.Text = "1.00 x";
            this.playbackRadio100x.UseVisualStyleBackColor = true;
            this.playbackRadio100x.CheckedChanged += new System.EventHandler(this.playbackRadio100x_CheckedChanged);
            // 
            // playbackRadio150x
            // 
            this.playbackRadio150x.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.playbackRadio150x.AutoSize = true;
            this.playbackRadio150x.Location = new System.Drawing.Point(12, 185);
            this.playbackRadio150x.Margin = new System.Windows.Forms.Padding(4);
            this.playbackRadio150x.Name = "playbackRadio150x";
            this.playbackRadio150x.Size = new System.Drawing.Size(91, 20);
            this.playbackRadio150x.TabIndex = 9;
            this.playbackRadio150x.Text = "1.50 x (DT)";
            this.playbackRadio150x.UseVisualStyleBackColor = true;
            this.playbackRadio150x.CheckedChanged += new System.EventHandler(this.playbackRadio150x_CheckedChanged);
            // 
            // playbackRadio005x
            // 
            this.playbackRadio005x.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.playbackRadio005x.AutoSize = true;
            this.playbackRadio005x.Location = new System.Drawing.Point(12, 16);
            this.playbackRadio005x.Margin = new System.Windows.Forms.Padding(4);
            this.playbackRadio005x.Name = "playbackRadio005x";
            this.playbackRadio005x.Size = new System.Drawing.Size(61, 20);
            this.playbackRadio005x.TabIndex = 10;
            this.playbackRadio005x.Text = "0.05 x";
            this.playbackRadio005x.UseVisualStyleBackColor = true;
            this.playbackRadio005x.CheckedChanged += new System.EventHandler(this.playbackRadio005x_CheckedChanged);
            // 
            // playbackRadio400x
            // 
            this.playbackRadio400x.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.playbackRadio400x.AutoSize = true;
            this.playbackRadio400x.Location = new System.Drawing.Point(13, 212);
            this.playbackRadio400x.Margin = new System.Windows.Forms.Padding(4);
            this.playbackRadio400x.Name = "playbackRadio400x";
            this.playbackRadio400x.Size = new System.Drawing.Size(61, 20);
            this.playbackRadio400x.TabIndex = 11;
            this.playbackRadio400x.Text = "4.00 x";
            this.playbackRadio400x.UseVisualStyleBackColor = true;
            this.playbackRadio400x.CheckedChanged += new System.EventHandler(this.playbackRadio400x_CheckedChanged);
            // 
            // playbackSpeedPanel
            // 
            this.playbackSpeedPanel.Controls.Add(this.playbackRadio075x);
            this.playbackSpeedPanel.Controls.Add(this.playbackRadio400x);
            this.playbackSpeedPanel.Controls.Add(this.playbackRadio010x);
            this.playbackSpeedPanel.Controls.Add(this.playbackRadio025x);
            this.playbackSpeedPanel.Controls.Add(this.playbackRadio005x);
            this.playbackSpeedPanel.Controls.Add(this.playbackRadio050x);
            this.playbackSpeedPanel.Controls.Add(this.playbackRadio150x);
            this.playbackSpeedPanel.Controls.Add(this.playbackRadio100x);
            this.playbackSpeedPanel.Location = new System.Drawing.Point(0, 64);
            this.playbackSpeedPanel.Margin = new System.Windows.Forms.Padding(4);
            this.playbackSpeedPanel.Name = "playbackSpeedPanel";
            this.playbackSpeedPanel.Size = new System.Drawing.Size(125, 240);
            this.playbackSpeedPanel.TabIndex = 12;
            // 
            // playbackRadio010x
            // 
            this.playbackRadio010x.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.playbackRadio010x.AutoSize = true;
            this.playbackRadio010x.Location = new System.Drawing.Point(12, 44);
            this.playbackRadio010x.Margin = new System.Windows.Forms.Padding(4);
            this.playbackRadio010x.Name = "playbackRadio010x";
            this.playbackRadio010x.Size = new System.Drawing.Size(61, 20);
            this.playbackRadio010x.TabIndex = 5;
            this.playbackRadio010x.Text = "0.10 x";
            this.playbackRadio010x.UseVisualStyleBackColor = true;
            this.playbackRadio010x.CheckedChanged += new System.EventHandler(this.playbackRadio010x_CheckedChanged);
            // 
            // muteBtn
            // 
            this.muteBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.muteBtn.Location = new System.Drawing.Point(1475, 602);
            this.muteBtn.Margin = new System.Windows.Forms.Padding(4);
            this.muteBtn.Name = "muteBtn";
            this.muteBtn.Size = new System.Drawing.Size(100, 28);
            this.muteBtn.TabIndex = 13;
            this.muteBtn.Text = "Mute";
            this.muteBtn.UseVisualStyleBackColor = true;
            this.muteBtn.Click += new System.EventHandler(this.muteBtn_Click);
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.cursorRadioPresses);
            this.panel1.Controls.Add(this.cursorRadioNormal);
            this.panel1.Controls.Add(this.cursorRadioKeys);
            this.panel1.Location = new System.Drawing.Point(0, 336);
            this.panel1.Margin = new System.Windows.Forms.Padding(4);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(125, 106);
            this.panel1.TabIndex = 15;
            // 
            // cursorRadioPresses
            // 
            this.cursorRadioPresses.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cursorRadioPresses.AutoSize = true;
            this.cursorRadioPresses.Checked = true;
            this.cursorRadioPresses.Location = new System.Drawing.Point(13, 73);
            this.cursorRadioPresses.Margin = new System.Windows.Forms.Padding(4);
            this.cursorRadioPresses.Name = "cursorRadioPresses";
            this.cursorRadioPresses.Size = new System.Drawing.Size(78, 20);
            this.cursorRadioPresses.TabIndex = 7;
            this.cursorRadioPresses.TabStop = true;
            this.cursorRadioPresses.Text = "Presses";
            this.cursorRadioPresses.UseVisualStyleBackColor = true;
            this.cursorRadioPresses.CheckedChanged += new System.EventHandler(this.cursorRadioPresses_CheckedChanged);
            // 
            // cursorRadioNormal
            // 
            this.cursorRadioNormal.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cursorRadioNormal.AutoSize = true;
            this.cursorRadioNormal.Location = new System.Drawing.Point(13, 16);
            this.cursorRadioNormal.Margin = new System.Windows.Forms.Padding(4);
            this.cursorRadioNormal.Name = "cursorRadioNormal";
            this.cursorRadioNormal.Size = new System.Drawing.Size(72, 20);
            this.cursorRadioNormal.TabIndex = 5;
            this.cursorRadioNormal.Text = "Normal";
            this.cursorRadioNormal.UseVisualStyleBackColor = true;
            this.cursorRadioNormal.CheckedChanged += new System.EventHandler(this.cursorRadioNormal_CheckedChanged);
            // 
            // cursorRadioKeys
            // 
            this.cursorRadioKeys.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cursorRadioKeys.AutoSize = true;
            this.cursorRadioKeys.Location = new System.Drawing.Point(13, 44);
            this.cursorRadioKeys.Margin = new System.Windows.Forms.Padding(4);
            this.cursorRadioKeys.Name = "cursorRadioKeys";
            this.cursorRadioKeys.Size = new System.Drawing.Size(58, 20);
            this.cursorRadioKeys.TabIndex = 6;
            this.cursorRadioKeys.Text = "Keys";
            this.cursorRadioKeys.UseVisualStyleBackColor = true;
            this.cursorRadioKeys.CheckedChanged += new System.EventHandler(this.cursorRadioKeys_CheckedChanged);
            // 
            // cursorTrailLabel
            // 
            this.cursorTrailLabel.AutoSize = true;
            this.cursorTrailLabel.Location = new System.Drawing.Point(9, 316);
            this.cursorTrailLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.cursorTrailLabel.Name = "cursorTrailLabel";
            this.cursorTrailLabel.Size = new System.Drawing.Size(46, 16);
            this.cursorTrailLabel.TabIndex = 14;
            this.cursorTrailLabel.Text = "Cursor";
            // 
            // togglePauseBtn
            // 
            this.togglePauseBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.togglePauseBtn.Location = new System.Drawing.Point(133, 575);
            this.togglePauseBtn.Margin = new System.Windows.Forms.Padding(4);
            this.togglePauseBtn.Name = "togglePauseBtn";
            this.togglePauseBtn.Size = new System.Drawing.Size(100, 28);
            this.togglePauseBtn.TabIndex = 17;
            this.togglePauseBtn.Text = "Play";
            this.togglePauseBtn.UseVisualStyleBackColor = true;
            this.togglePauseBtn.Click += new System.EventHandler(this.togglePauseBtn_Click);
            // 
            // songTimeLabelLabel
            // 
            this.songTimeLabelLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.songTimeLabelLabel.AutoSize = true;
            this.songTimeLabelLabel.Location = new System.Drawing.Point(241, 581);
            this.songTimeLabelLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.songTimeLabelLabel.Name = "songTimeLabelLabel";
            this.songTimeLabelLabel.Size = new System.Drawing.Size(79, 16);
            this.songTimeLabelLabel.TabIndex = 18;
            this.songTimeLabelLabel.Text = "Song Time: ";
            // 
            // songTimeLabel
            // 
            this.songTimeLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.songTimeLabel.AutoSize = true;
            this.songTimeLabel.Location = new System.Drawing.Point(319, 581);
            this.songTimeLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.songTimeLabel.Name = "songTimeLabel";
            this.songTimeLabel.Size = new System.Drawing.Size(15, 16);
            this.songTimeLabel.TabIndex = 19;
            this.songTimeLabel.Text = "--";
            // 
            // markInBtn
            // 
            this.markInBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.markInBtn.Location = new System.Drawing.Point(264, 617);
            this.markInBtn.Margin = new System.Windows.Forms.Padding(4);
            this.markInBtn.Name = "markInBtn";
            this.markInBtn.Size = new System.Drawing.Size(100, 28);
            this.markInBtn.TabIndex = 22;
            this.markInBtn.Text = "Mark In";
            this.markInBtn.UseVisualStyleBackColor = true;
            this.markInBtn.Click += new System.EventHandler(this.markInBtn_Click);
            // 
            // markOutBtn
            // 
            this.markOutBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.markOutBtn.Location = new System.Drawing.Point(372, 617);
            this.markOutBtn.Margin = new System.Windows.Forms.Padding(4);
            this.markOutBtn.Name = "markOutBtn";
            this.markOutBtn.Size = new System.Drawing.Size(100, 28);
            this.markOutBtn.TabIndex = 23;
            this.markOutBtn.Text = "Mark Out";
            this.markOutBtn.UseVisualStyleBackColor = true;
            this.markOutBtn.Click += new System.EventHandler(this.markOutBtn_Click);
            // 
            // markMidBtn
            // 
            this.markMidBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.markMidBtn.Location = new System.Drawing.Point(480, 617);
            this.markMidBtn.Margin = new System.Windows.Forms.Padding(4);
            this.markMidBtn.Name = "markMidBtn";
            this.markMidBtn.Size = new System.Drawing.Size(100, 28);
            this.markMidBtn.TabIndex = 24;
            this.markMidBtn.Text = "Mark Mid";
            this.markMidBtn.UseVisualStyleBackColor = true;
            this.markMidBtn.Click += new System.EventHandler(this.markMidBtn_Click);
            // 
            // clearMarksBtn
            // 
            this.clearMarksBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.clearMarksBtn.Location = new System.Drawing.Point(696, 617);
            this.clearMarksBtn.Margin = new System.Windows.Forms.Padding(4);
            this.clearMarksBtn.Name = "clearMarksBtn";
            this.clearMarksBtn.Size = new System.Drawing.Size(100, 28);
            this.clearMarksBtn.TabIndex = 25;
            this.clearMarksBtn.Text = "Clear Marks";
            this.clearMarksBtn.UseVisualStyleBackColor = true;
            this.clearMarksBtn.Click += new System.EventHandler(this.clearMarksBtn_Click);
            // 
            // markerInfoLabel
            // 
            this.markerInfoLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.markerInfoLabel.AutoSize = true;
            this.markerInfoLabel.Location = new System.Drawing.Point(129, 623);
            this.markerInfoLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.markerInfoLabel.Name = "markerInfoLabel";
            this.markerInfoLabel.Size = new System.Drawing.Size(115, 16);
            this.markerInfoLabel.TabIndex = 26;
            this.markerInfoLabel.Text = "Markers / Anchors";
            // 
            // keypressInfoLabel
            // 
            this.keypressInfoLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.keypressInfoLabel.AutoSize = true;
            this.keypressInfoLabel.Location = new System.Drawing.Point(169, 654);
            this.keypressInfoLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.keypressInfoLabel.Name = "keypressInfoLabel";
            this.keypressInfoLabel.Size = new System.Drawing.Size(83, 16);
            this.keypressInfoLabel.TabIndex = 28;
            this.keypressInfoLabel.Text = "Key Presses";
            // 
            // markAllBtn
            // 
            this.markAllBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.markAllBtn.Location = new System.Drawing.Point(588, 617);
            this.markAllBtn.Margin = new System.Windows.Forms.Padding(4);
            this.markAllBtn.Name = "markAllBtn";
            this.markAllBtn.Size = new System.Drawing.Size(100, 28);
            this.markAllBtn.TabIndex = 29;
            this.markAllBtn.Text = "Mark All";
            this.markAllBtn.UseVisualStyleBackColor = true;
            this.markAllBtn.Click += new System.EventHandler(this.markAllBtn_Click);
            // 
            // keyPress1Btn
            // 
            this.keyPress1Btn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.keyPress1Btn.Location = new System.Drawing.Point(372, 654);
            this.keyPress1Btn.Margin = new System.Windows.Forms.Padding(4);
            this.keyPress1Btn.Name = "keyPress1Btn";
            this.keyPress1Btn.Size = new System.Drawing.Size(100, 28);
            this.keyPress1Btn.TabIndex = 34;
            this.keyPress1Btn.Text = "Key 1";
            this.keyPress1Btn.UseVisualStyleBackColor = true;
            this.keyPress1Btn.Click += new System.EventHandler(this.keyPress1Btn_Click);
            // 
            // keyPress2Btn
            // 
            this.keyPress2Btn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.keyPress2Btn.Location = new System.Drawing.Point(480, 654);
            this.keyPress2Btn.Margin = new System.Windows.Forms.Padding(4);
            this.keyPress2Btn.Name = "keyPress2Btn";
            this.keyPress2Btn.Size = new System.Drawing.Size(100, 28);
            this.keyPress2Btn.TabIndex = 33;
            this.keyPress2Btn.Text = "Key 2";
            this.keyPress2Btn.UseVisualStyleBackColor = true;
            this.keyPress2Btn.Click += new System.EventHandler(this.keyPress2Btn_Click);
            // 
            // keyPress12Btn
            // 
            this.keyPress12Btn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.keyPress12Btn.Location = new System.Drawing.Point(588, 654);
            this.keyPress12Btn.Margin = new System.Windows.Forms.Padding(4);
            this.keyPress12Btn.Name = "keyPress12Btn";
            this.keyPress12Btn.Size = new System.Drawing.Size(100, 28);
            this.keyPress12Btn.TabIndex = 31;
            this.keyPress12Btn.Text = "Key 1 + 2";
            this.keyPress12Btn.UseVisualStyleBackColor = true;
            this.keyPress12Btn.Click += new System.EventHandler(this.keyPress12Btn_Click);
            // 
            // keyPressNoneBtn
            // 
            this.keyPressNoneBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.keyPressNoneBtn.Location = new System.Drawing.Point(264, 654);
            this.keyPressNoneBtn.Margin = new System.Windows.Forms.Padding(4);
            this.keyPressNoneBtn.Name = "keyPressNoneBtn";
            this.keyPressNoneBtn.Size = new System.Drawing.Size(100, 28);
            this.keyPressNoneBtn.TabIndex = 30;
            this.keyPressNoneBtn.Text = "None";
            this.keyPressNoneBtn.UseVisualStyleBackColor = true;
            this.keyPressNoneBtn.Click += new System.EventHandler(this.keyPressNoneBtn_Click);
            // 
            // trailLengthBar
            // 
            this.trailLengthBar.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.trailLengthBar.Location = new System.Drawing.Point(264, 688);
            this.trailLengthBar.Margin = new System.Windows.Forms.Padding(4);
            this.trailLengthBar.Maximum = 30;
            this.trailLengthBar.Minimum = 1;
            this.trailLengthBar.Name = "trailLengthBar";
            this.trailLengthBar.Size = new System.Drawing.Size(532, 56);
            this.trailLengthBar.TabIndex = 35;
            this.trailLengthBar.Value = 10;
            this.trailLengthBar.Scroll += new System.EventHandler(this.trailLengthBar_Scroll);
            // 
            // trailLengthInfoLabel
            // 
            this.trailLengthInfoLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.trailLengthInfoLabel.AutoSize = true;
            this.trailLengthInfoLabel.Location = new System.Drawing.Point(128, 688);
            this.trailLengthInfoLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.trailLengthInfoLabel.Name = "trailLengthInfoLabel";
            this.trailLengthInfoLabel.Size = new System.Drawing.Size(119, 16);
            this.trailLengthInfoLabel.TabIndex = 36;
            this.trailLengthInfoLabel.Text = "Cursor Trail Length";
            // 
            // cursorTrailValueLabel
            // 
            this.cursorTrailValueLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.cursorTrailValueLabel.AutoSize = true;
            this.cursorTrailValueLabel.Location = new System.Drawing.Point(804, 700);
            this.cursorTrailValueLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.cursorTrailValueLabel.Name = "cursorTrailValueLabel";
            this.cursorTrailValueLabel.Size = new System.Drawing.Size(39, 16);
            this.cursorTrailValueLabel.TabIndex = 37;
            this.cursorTrailValueLabel.Text = "1 sec";
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "replay.osr";
            this.openFileDialog1.Filter = "osu! Replay Files (*.osr)|*.osr|All files (*.*)|*.*";
            this.openFileDialog1.RestoreDirectory = true;
            this.openFileDialog1.FileOk += new System.ComponentModel.CancelEventHandler(this.openFileDialog1_FileOk);
            // 
            // saveFileDialog1
            // 
            this.saveFileDialog1.Filter = "osu! Replay Files (*.osr)|*.osr|All files (*.*)|*.*";
            this.saveFileDialog1.FileOk += new System.ComponentModel.CancelEventHandler(this.saveFileDialog1_FileOk);
            // 
            // flipBeatmapBtn
            // 
            this.flipBeatmapBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.flipBeatmapBtn.Location = new System.Drawing.Point(871, 595);
            this.flipBeatmapBtn.Margin = new System.Windows.Forms.Padding(4);
            this.flipBeatmapBtn.Name = "flipBeatmapBtn";
            this.flipBeatmapBtn.Size = new System.Drawing.Size(132, 28);
            this.flipBeatmapBtn.TabIndex = 38;
            this.flipBeatmapBtn.Text = "Flip Beatmap";
            this.flipBeatmapBtn.UseVisualStyleBackColor = true;
            this.flipBeatmapBtn.Click += new System.EventHandler(this.flipBeatmapBtn_Click);
            // 
            // flipCursorDataBtn
            // 
            this.flipCursorDataBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.flipCursorDataBtn.Location = new System.Drawing.Point(871, 632);
            this.flipCursorDataBtn.Margin = new System.Windows.Forms.Padding(4);
            this.flipCursorDataBtn.Name = "flipCursorDataBtn";
            this.flipCursorDataBtn.Size = new System.Drawing.Size(132, 28);
            this.flipCursorDataBtn.TabIndex = 39;
            this.flipCursorDataBtn.Text = "Flip Cursor Data";
            this.flipCursorDataBtn.UseVisualStyleBackColor = true;
            this.flipCursorDataBtn.Click += new System.EventHandler(this.flipCursorDataBtn_Click);
            // 
            // label1
            // 
            this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(867, 575);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(143, 16);
            this.label1.TabIndex = 26;
            this.label1.Text = "Hardrock Compatibility";
            // 
            // label2
            // 
            this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(128, 757);
            this.label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(190, 16);
            this.label2.TabIndex = 40;
            this.label2.Text = "Pan / Zoom (Right click / scroll)";
            // 
            // zoomPanResetBtn
            // 
            this.zoomPanResetBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.zoomPanResetBtn.Location = new System.Drawing.Point(355, 751);
            this.zoomPanResetBtn.Margin = new System.Windows.Forms.Padding(4);
            this.zoomPanResetBtn.Name = "zoomPanResetBtn";
            this.zoomPanResetBtn.Size = new System.Drawing.Size(100, 28);
            this.zoomPanResetBtn.TabIndex = 22;
            this.zoomPanResetBtn.Text = "Reset";
            this.zoomPanResetBtn.UseVisualStyleBackColor = true;
            this.zoomPanResetBtn.Click += new System.EventHandler(this.zoomPanResetBtn_Click);
            // 
            // zoomInBtn
            // 
            this.zoomInBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.zoomInBtn.Location = new System.Drawing.Point(463, 751);
            this.zoomInBtn.Margin = new System.Windows.Forms.Padding(4);
            this.zoomInBtn.Name = "zoomInBtn";
            this.zoomInBtn.Size = new System.Drawing.Size(100, 28);
            this.zoomInBtn.TabIndex = 22;
            this.zoomInBtn.Text = "Zoom In";
            this.zoomInBtn.UseVisualStyleBackColor = true;
            this.zoomInBtn.Click += new System.EventHandler(this.zoomInBtn_Click);
            // 
            // zoomOutBtn
            // 
            this.zoomOutBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.zoomOutBtn.Location = new System.Drawing.Point(571, 751);
            this.zoomOutBtn.Margin = new System.Windows.Forms.Padding(4);
            this.zoomOutBtn.Name = "zoomOutBtn";
            this.zoomOutBtn.Size = new System.Drawing.Size(100, 28);
            this.zoomOutBtn.TabIndex = 22;
            this.zoomOutBtn.Text = "Zoom Out";
            this.zoomOutBtn.UseVisualStyleBackColor = true;
            this.zoomOutBtn.Click += new System.EventHandler(this.zoomOutBtn_Click);
            // 
            // updateTimestampCB
            // 
            this.updateTimestampCB.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.updateTimestampCB.AutoSize = true;
            this.updateTimestampCB.Location = new System.Drawing.Point(871, 694);
            this.updateTimestampCB.Margin = new System.Windows.Forms.Padding(4);
            this.updateTimestampCB.Name = "updateTimestampCB";
            this.updateTimestampCB.Size = new System.Drawing.Size(204, 20);
            this.updateTimestampCB.TabIndex = 41;
            this.updateTimestampCB.Text = "Update Timestamp on Export";
            this.updateTimestampCB.UseVisualStyleBackColor = true;
            this.updateTimestampCB.CheckedChanged += new System.EventHandler(this.updateTimestampCB_CheckedChanged);
            // 
            // label3
            // 
            this.label3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(867, 673);
            this.label3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(90, 16);
            this.label3.TabIndex = 26;
            this.label3.Text = "Other Settings";
            // 
            // analyzeAccBtn
            // 
            this.analyzeAccBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.analyzeAccBtn.Location = new System.Drawing.Point(1329, 38);
            this.analyzeAccBtn.Margin = new System.Windows.Forms.Padding(4);
            this.analyzeAccBtn.Name = "analyzeAccBtn";
            this.analyzeAccBtn.Size = new System.Drawing.Size(100, 28);
            this.analyzeAccBtn.TabIndex = 44;
            this.analyzeAccBtn.Text = "Analyze Acc";
            this.analyzeAccBtn.UseVisualStyleBackColor = true;
            this.analyzeAccBtn.Click += new System.EventHandler(this.analyzeAccBtn_Click);
            // 
            // label6
            // 
            this.label6.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(1327, 112);
            this.label6.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(31, 16);
            this.label6.TabIndex = 47;
            this.label6.Text = "50s:";
            // 
            // label8
            // 
            this.label8.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(1327, 149);
            this.label8.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(32, 16);
            this.label8.TabIndex = 49;
            this.label8.Text = "acc:";
            // 
            // nextMissBtn
            // 
            this.nextMissBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.nextMissBtn.Enabled = false;
            this.nextMissBtn.Location = new System.Drawing.Point(1329, 260);
            this.nextMissBtn.Margin = new System.Windows.Forms.Padding(4);
            this.nextMissBtn.Name = "nextMissBtn";
            this.nextMissBtn.Size = new System.Drawing.Size(100, 28);
            this.nextMissBtn.TabIndex = 43;
            this.nextMissBtn.Text = "Next Miss";
            this.nextMissBtn.UseVisualStyleBackColor = true;
            this.nextMissBtn.Click += new System.EventHandler(this.nextMissBtn_Click);
            // 
            // label4
            // 
            this.label4.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(1327, 75);
            this.label4.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(38, 16);
            this.label4.TabIndex = 45;
            this.label4.Text = "300s:";
            // 
            // label5
            // 
            this.label5.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(1327, 94);
            this.label5.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(38, 16);
            this.label5.TabIndex = 46;
            this.label5.Text = "100s:";
            // 
            // label7
            // 
            this.label7.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(1327, 130);
            this.label7.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(53, 16);
            this.label7.TabIndex = 48;
            this.label7.Text = "misses:";
            // 
            // label9
            // 
            this.label9.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(1327, 167);
            this.label9.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(66, 16);
            this.label9.TabIndex = 50;
            this.label9.Text = "avg early:";
            // 
            // label10
            // 
            this.label10.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(1327, 186);
            this.label10.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(58, 16);
            this.label10.TabIndex = 51;
            this.label10.Text = "avg late:";
            // 
            // label11
            // 
            this.label11.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(1327, 204);
            this.label11.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(87, 16);
            this.label11.TabIndex = 52;
            this.label11.Text = "unstable rate:";
            // 
            // acc_ur_tb
            // 
            this.acc_ur_tb.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.acc_ur_tb.AutoSize = true;
            this.acc_ur_tb.Location = new System.Drawing.Point(1415, 204);
            this.acc_ur_tb.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.acc_ur_tb.Name = "acc_ur_tb";
            this.acc_ur_tb.Size = new System.Drawing.Size(14, 16);
            this.acc_ur_tb.TabIndex = 53;
            this.acc_ur_tb.Text = "?";
            // 
            // acc_avg_late_tb
            // 
            this.acc_avg_late_tb.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.acc_avg_late_tb.AutoSize = true;
            this.acc_avg_late_tb.Location = new System.Drawing.Point(1415, 186);
            this.acc_avg_late_tb.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.acc_avg_late_tb.Name = "acc_avg_late_tb";
            this.acc_avg_late_tb.Size = new System.Drawing.Size(14, 16);
            this.acc_avg_late_tb.TabIndex = 54;
            this.acc_avg_late_tb.Text = "?";
            // 
            // acc_avg_early_tb
            // 
            this.acc_avg_early_tb.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.acc_avg_early_tb.AutoSize = true;
            this.acc_avg_early_tb.Location = new System.Drawing.Point(1415, 167);
            this.acc_avg_early_tb.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.acc_avg_early_tb.Name = "acc_avg_early_tb";
            this.acc_avg_early_tb.Size = new System.Drawing.Size(14, 16);
            this.acc_avg_early_tb.TabIndex = 55;
            this.acc_avg_early_tb.Text = "?";
            // 
            // acc_acc_tb
            // 
            this.acc_acc_tb.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.acc_acc_tb.AutoSize = true;
            this.acc_acc_tb.Location = new System.Drawing.Point(1415, 149);
            this.acc_acc_tb.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.acc_acc_tb.Name = "acc_acc_tb";
            this.acc_acc_tb.Size = new System.Drawing.Size(14, 16);
            this.acc_acc_tb.TabIndex = 56;
            this.acc_acc_tb.Text = "?";
            // 
            // acc_misses_tb
            // 
            this.acc_misses_tb.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.acc_misses_tb.AutoSize = true;
            this.acc_misses_tb.Location = new System.Drawing.Point(1415, 130);
            this.acc_misses_tb.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.acc_misses_tb.Name = "acc_misses_tb";
            this.acc_misses_tb.Size = new System.Drawing.Size(14, 16);
            this.acc_misses_tb.TabIndex = 57;
            this.acc_misses_tb.Text = "?";
            // 
            // acc_50s_tb
            // 
            this.acc_50s_tb.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.acc_50s_tb.AutoSize = true;
            this.acc_50s_tb.Location = new System.Drawing.Point(1415, 112);
            this.acc_50s_tb.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.acc_50s_tb.Name = "acc_50s_tb";
            this.acc_50s_tb.Size = new System.Drawing.Size(14, 16);
            this.acc_50s_tb.TabIndex = 58;
            this.acc_50s_tb.Text = "?";
            // 
            // acc_100s_tb
            // 
            this.acc_100s_tb.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.acc_100s_tb.AutoSize = true;
            this.acc_100s_tb.Location = new System.Drawing.Point(1415, 94);
            this.acc_100s_tb.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.acc_100s_tb.Name = "acc_100s_tb";
            this.acc_100s_tb.Size = new System.Drawing.Size(14, 16);
            this.acc_100s_tb.TabIndex = 59;
            this.acc_100s_tb.Text = "?";
            // 
            // acc_300s_tb
            // 
            this.acc_300s_tb.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.acc_300s_tb.AutoSize = true;
            this.acc_300s_tb.Location = new System.Drawing.Point(1415, 75);
            this.acc_300s_tb.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.acc_300s_tb.Name = "acc_300s_tb";
            this.acc_300s_tb.Size = new System.Drawing.Size(14, 16);
            this.acc_300s_tb.TabIndex = 60;
            this.acc_300s_tb.Text = "?";
            // 
            // next50btn
            // 
            this.next50btn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.next50btn.Enabled = false;
            this.next50btn.Location = new System.Drawing.Point(1329, 295);
            this.next50btn.Margin = new System.Windows.Forms.Padding(4);
            this.next50btn.Name = "next50btn";
            this.next50btn.Size = new System.Drawing.Size(100, 28);
            this.next50btn.TabIndex = 61;
            this.next50btn.Text = "Next 50";
            this.next50btn.UseVisualStyleBackColor = true;
            this.next50btn.Click += new System.EventHandler(this.next50btn_Click);
            // 
            // next100btn
            // 
            this.next100btn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.next100btn.Enabled = false;
            this.next100btn.Location = new System.Drawing.Point(1329, 331);
            this.next100btn.Margin = new System.Windows.Forms.Padding(4);
            this.next100btn.Name = "next100btn";
            this.next100btn.Size = new System.Drawing.Size(100, 28);
            this.next100btn.TabIndex = 62;
            this.next100btn.Text = "Next 100";
            this.next100btn.UseVisualStyleBackColor = true;
            this.next100btn.Click += new System.EventHandler(this.next100btn_Click);
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.toolSelGrabRadioButton);
            this.panel2.Controls.Add(this.toolBrushRadioButton);
            this.panel2.Location = new System.Drawing.Point(0, 465);
            this.panel2.Margin = new System.Windows.Forms.Padding(4);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(125, 68);
            this.panel2.TabIndex = 64;
            // 
            // toolSelGrabRadioButton
            // 
            this.toolSelGrabRadioButton.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.toolSelGrabRadioButton.AutoSize = true;
            this.toolSelGrabRadioButton.Checked = true;
            this.toolSelGrabRadioButton.Location = new System.Drawing.Point(13, 16);
            this.toolSelGrabRadioButton.Margin = new System.Windows.Forms.Padding(4);
            this.toolSelGrabRadioButton.Name = "toolSelGrabRadioButton";
            this.toolSelGrabRadioButton.Size = new System.Drawing.Size(109, 20);
            this.toolSelGrabRadioButton.TabIndex = 5;
            this.toolSelGrabRadioButton.TabStop = true;
            this.toolSelGrabRadioButton.Text = "Select + Grab";
            this.toolSelGrabRadioButton.UseVisualStyleBackColor = true;
            this.toolSelGrabRadioButton.CheckedChanged += new System.EventHandler(this.toolSelGrabRadioButton_CheckedChanged);
            // 
            // toolBrushRadioButton
            // 
            this.toolBrushRadioButton.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.toolBrushRadioButton.AutoSize = true;
            this.toolBrushRadioButton.Location = new System.Drawing.Point(13, 44);
            this.toolBrushRadioButton.Margin = new System.Windows.Forms.Padding(4);
            this.toolBrushRadioButton.Name = "toolBrushRadioButton";
            this.toolBrushRadioButton.Size = new System.Drawing.Size(62, 20);
            this.toolBrushRadioButton.TabIndex = 6;
            this.toolBrushRadioButton.Text = "Brush";
            this.toolBrushRadioButton.UseVisualStyleBackColor = true;
            this.toolBrushRadioButton.CheckedChanged += new System.EventHandler(this.toolBrushRadioButton_CheckedChanged);
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(9, 446);
            this.label12.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(35, 16);
            this.label12.TabIndex = 63;
            this.label12.Text = "Tool";
            // 
            // brushRadiusTrackBar
            // 
            this.brushRadiusTrackBar.Location = new System.Drawing.Point(0, 558);
            this.brushRadiusTrackBar.Margin = new System.Windows.Forms.Padding(4);
            this.brushRadiusTrackBar.Maximum = 200;
            this.brushRadiusTrackBar.Minimum = 10;
            this.brushRadiusTrackBar.Name = "brushRadiusTrackBar";
            this.brushRadiusTrackBar.Size = new System.Drawing.Size(123, 56);
            this.brushRadiusTrackBar.TabIndex = 65;
            this.brushRadiusTrackBar.TickStyle = System.Windows.Forms.TickStyle.None;
            this.brushRadiusTrackBar.Value = 60;
            this.brushRadiusTrackBar.ValueChanged += new System.EventHandler(this.brushRadiusTrackBar_ValueChanged);
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(9, 539);
            this.label13.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(87, 16);
            this.label13.TabIndex = 66;
            this.label13.Text = "Brush Radius";
            // 
            // brushRadiusLabel
            // 
            this.brushRadiusLabel.AutoSize = true;
            this.brushRadiusLabel.Location = new System.Drawing.Point(13, 587);
            this.brushRadiusLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.brushRadiusLabel.Name = "brushRadiusLabel";
            this.brushRadiusLabel.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.brushRadiusLabel.Size = new System.Drawing.Size(21, 16);
            this.brushRadiusLabel.TabIndex = 67;
            this.brushRadiusLabel.Text = "60";
            // 
            // label14
            // 
            this.label14.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(1327, 372);
            this.label14.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(113, 16);
            this.label14.TabIndex = 68;
            this.label14.Text = "Current Hit Object:";
            // 
            // currentHitObjectIdLabel
            // 
            this.currentHitObjectIdLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.currentHitObjectIdLabel.AutoSize = true;
            this.currentHitObjectIdLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.currentHitObjectIdLabel.Location = new System.Drawing.Point(1327, 390);
            this.currentHitObjectIdLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.currentHitObjectIdLabel.Name = "currentHitObjectIdLabel";
            this.currentHitObjectIdLabel.Size = new System.Drawing.Size(23, 25);
            this.currentHitObjectIdLabel.TabIndex = 68;
            this.currentHitObjectIdLabel.Text = "?";
            // 
            // label15
            // 
            this.label15.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(1327, 441);
            this.label15.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(39, 16);
            this.label15.TabIndex = 69;
            this.label15.Text = "Error:";
            // 
            // hitObjectErrorLabel
            // 
            this.hitObjectErrorLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.hitObjectErrorLabel.AutoSize = true;
            this.hitObjectErrorLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.hitObjectErrorLabel.Location = new System.Drawing.Point(1327, 459);
            this.hitObjectErrorLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.hitObjectErrorLabel.Name = "hitObjectErrorLabel";
            this.hitObjectErrorLabel.Size = new System.Drawing.Size(23, 25);
            this.hitObjectErrorLabel.TabIndex = 70;
            this.hitObjectErrorLabel.Text = "?";
            // 
            // hitObjectPointsLabel
            // 
            this.hitObjectPointsLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.hitObjectPointsLabel.AutoSize = true;
            this.hitObjectPointsLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.hitObjectPointsLabel.Location = new System.Drawing.Point(1327, 508);
            this.hitObjectPointsLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.hitObjectPointsLabel.Name = "hitObjectPointsLabel";
            this.hitObjectPointsLabel.Size = new System.Drawing.Size(23, 25);
            this.hitObjectPointsLabel.TabIndex = 72;
            this.hitObjectPointsLabel.Text = "?";
            // 
            // label17
            // 
            this.label17.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(1327, 490);
            this.label17.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(47, 16);
            this.label17.TabIndex = 71;
            this.label17.Text = "Points:";
            // 
            // nextObjectBtn
            // 
            this.nextObjectBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.nextObjectBtn.Enabled = false;
            this.nextObjectBtn.Location = new System.Drawing.Point(1329, 224);
            this.nextObjectBtn.Margin = new System.Windows.Forms.Padding(4);
            this.nextObjectBtn.Name = "nextObjectBtn";
            this.nextObjectBtn.Size = new System.Drawing.Size(100, 28);
            this.nextObjectBtn.TabIndex = 73;
            this.nextObjectBtn.Text = "Next Object";
            this.nextObjectBtn.UseVisualStyleBackColor = true;
            this.nextObjectBtn.Click += new System.EventHandler(this.nextObjectBtn_Click);
            // 
            // currentHitObjectLabel
            // 
            this.currentHitObjectLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.currentHitObjectLabel.AutoSize = true;
            this.currentHitObjectLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.currentHitObjectLabel.Location = new System.Drawing.Point(1327, 412);
            this.currentHitObjectLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.currentHitObjectLabel.Name = "currentHitObjectLabel";
            this.currentHitObjectLabel.Size = new System.Drawing.Size(23, 25);
            this.currentHitObjectLabel.TabIndex = 74;
            this.currentHitObjectLabel.Text = "?";
            // 
            // isKeyboardCB
            // 
            this.isKeyboardCB.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.isKeyboardCB.AutoSize = true;
            this.isKeyboardCB.Checked = true;
            this.isKeyboardCB.CheckState = System.Windows.Forms.CheckState.Checked;
            this.isKeyboardCB.Location = new System.Drawing.Point(696, 658);
            this.isKeyboardCB.Margin = new System.Windows.Forms.Padding(4);
            this.isKeyboardCB.Name = "isKeyboardCB";
            this.isKeyboardCB.Size = new System.Drawing.Size(101, 20);
            this.isKeyboardCB.TabIndex = 80;
            this.isKeyboardCB.Text = "Is Keyboard";
            this.isKeyboardCB.UseVisualStyleBackColor = true;
            // 
            // drawFramesOnTimelineCB
            // 
            this.drawFramesOnTimelineCB.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.drawFramesOnTimelineCB.AutoSize = true;
            this.drawFramesOnTimelineCB.Checked = true;
            this.drawFramesOnTimelineCB.CheckState = System.Windows.Forms.CheckState.Checked;
            this.drawFramesOnTimelineCB.Location = new System.Drawing.Point(871, 721);
            this.drawFramesOnTimelineCB.Name = "drawFramesOnTimelineCB";
            this.drawFramesOnTimelineCB.Size = new System.Drawing.Size(200, 20);
            this.drawFramesOnTimelineCB.TabIndex = 81;
            this.drawFramesOnTimelineCB.Text = "Draw All Frames on Timeline";
            this.drawFramesOnTimelineCB.UseVisualStyleBackColor = true;
            this.drawFramesOnTimelineCB.CheckedChanged += new System.EventHandler(this.drawFramesOnTimelineCB_CheckedChanged);
            // 
            // moveFrameBackBtn
            // 
            this.moveFrameBackBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.moveFrameBackBtn.Location = new System.Drawing.Point(1116, 642);
            this.moveFrameBackBtn.Name = "moveFrameBackBtn";
            this.moveFrameBackBtn.Size = new System.Drawing.Size(120, 28);
            this.moveFrameBackBtn.TabIndex = 82;
            this.moveFrameBackBtn.Text = "Move Back";
            this.moveFrameBackBtn.UseVisualStyleBackColor = true;
            this.moveFrameBackBtn.Click += new System.EventHandler(this.moveFrameBackBtn_Click);
            // 
            // moveFrameForwardBtn
            // 
            this.moveFrameForwardBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.moveFrameForwardBtn.Location = new System.Drawing.Point(1116, 608);
            this.moveFrameForwardBtn.Name = "moveFrameForwardBtn";
            this.moveFrameForwardBtn.Size = new System.Drawing.Size(120, 28);
            this.moveFrameForwardBtn.TabIndex = 83;
            this.moveFrameForwardBtn.Text = "Move Forward";
            this.moveFrameForwardBtn.UseVisualStyleBackColor = true;
            this.moveFrameForwardBtn.Click += new System.EventHandler(this.moveFrameForwardBtn_Click);
            // 
            // centerCurrentFrameBtn
            // 
            this.centerCurrentFrameBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.centerCurrentFrameBtn.Location = new System.Drawing.Point(1116, 676);
            this.centerCurrentFrameBtn.Name = "centerCurrentFrameBtn";
            this.centerCurrentFrameBtn.Size = new System.Drawing.Size(120, 28);
            this.centerCurrentFrameBtn.TabIndex = 84;
            this.centerCurrentFrameBtn.Text = "Center";
            this.centerCurrentFrameBtn.UseVisualStyleBackColor = true;
            this.centerCurrentFrameBtn.Click += new System.EventHandler(this.centerCurrentFrameBtn_Click);
            // 
            // addFrameBtn
            // 
            this.addFrameBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.addFrameBtn.Location = new System.Drawing.Point(1242, 608);
            this.addFrameBtn.Name = "addFrameBtn";
            this.addFrameBtn.Size = new System.Drawing.Size(120, 28);
            this.addFrameBtn.TabIndex = 85;
            this.addFrameBtn.Text = "Add";
            this.addFrameBtn.UseVisualStyleBackColor = true;
            this.addFrameBtn.Click += new System.EventHandler(this.addFrameBtn_Click);
            // 
            // deleteFrameBtn
            // 
            this.deleteFrameBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.deleteFrameBtn.Location = new System.Drawing.Point(1242, 642);
            this.deleteFrameBtn.Name = "deleteFrameBtn";
            this.deleteFrameBtn.Size = new System.Drawing.Size(120, 28);
            this.deleteFrameBtn.TabIndex = 86;
            this.deleteFrameBtn.Text = "Delete";
            this.deleteFrameBtn.UseVisualStyleBackColor = true;
            this.deleteFrameBtn.Click += new System.EventHandler(this.deleteFrameBtn_Click);
            // 
            // toKeyboardBtn
            // 
            this.toKeyboardBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.toKeyboardBtn.Location = new System.Drawing.Point(1116, 751);
            this.toKeyboardBtn.Name = "toKeyboardBtn";
            this.toKeyboardBtn.Size = new System.Drawing.Size(243, 28);
            this.toKeyboardBtn.TabIndex = 87;
            this.toKeyboardBtn.Text = "Change All Inputs to Keyboard";
            this.toKeyboardBtn.UseVisualStyleBackColor = true;
            this.toKeyboardBtn.Click += new System.EventHandler(this.toKeyboardBtn_Click);
            // 
            // openFileDialog2
            // 
            this.openFileDialog2.FileName = "beatmap.osu";
            this.openFileDialog2.Filter = "osu! Beatmap Files (*.osu)|*.osu|All files (*.*)|*.*";
            this.openFileDialog2.RestoreDirectory = true;
            this.openFileDialog2.FileOk += new System.ComponentModel.CancelEventHandler(this.openFileDialog2_FileOk);
            // 
            // scaleFramesBtn
            // 
            this.scaleFramesBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.scaleFramesBtn.Location = new System.Drawing.Point(1242, 676);
            this.scaleFramesBtn.Name = "scaleFramesBtn";
            this.scaleFramesBtn.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.scaleFramesBtn.Size = new System.Drawing.Size(120, 28);
            this.scaleFramesBtn.TabIndex = 88;
            this.scaleFramesBtn.Text = "Scale";
            this.scaleFramesBtn.UseVisualStyleBackColor = true;
            this.scaleFramesBtn.Click += new System.EventHandler(this.scaleFramesBtn_Click);
            // 
            // scaleFramesUd
            // 
            this.scaleFramesUd.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.scaleFramesUd.DecimalPlaces = 3;
            this.scaleFramesUd.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.scaleFramesUd.Location = new System.Drawing.Point(1277, 716);
            this.scaleFramesUd.Maximum = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.scaleFramesUd.Minimum = new decimal(new int[] {
            5,
            0,
            0,
            65536});
            this.scaleFramesUd.Name = "scaleFramesUd";
            this.scaleFramesUd.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.scaleFramesUd.Size = new System.Drawing.Size(73, 22);
            this.scaleFramesUd.TabIndex = 91;
            this.scaleFramesUd.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // drawHitWindowsCB
            // 
            this.drawHitWindowsCB.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.drawHitWindowsCB.AutoSize = true;
            this.drawHitWindowsCB.Checked = true;
            this.drawHitWindowsCB.CheckState = System.Windows.Forms.CheckState.Checked;
            this.drawHitWindowsCB.Location = new System.Drawing.Point(870, 747);
            this.drawHitWindowsCB.Name = "drawHitWindowsCB";
            this.drawHitWindowsCB.Size = new System.Drawing.Size(203, 20);
            this.drawHitWindowsCB.TabIndex = 92;
            this.drawHitWindowsCB.Text = "Draw Hitwindows on Timeline";
            this.drawHitWindowsCB.UseVisualStyleBackColor = true;
            this.drawHitWindowsCB.CheckedChanged += new System.EventHandler(this.drawHitWindowsCB_CheckedChanged);
            // 
            // drawSliderendRangeCB
            // 
            this.drawSliderendRangeCB.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.drawSliderendRangeCB.AutoSize = true;
            this.drawSliderendRangeCB.Location = new System.Drawing.Point(870, 773);
            this.drawSliderendRangeCB.Name = "drawSliderendRangeCB";
            this.drawSliderendRangeCB.Size = new System.Drawing.Size(184, 20);
            this.drawSliderendRangeCB.TabIndex = 93;
            this.drawSliderendRangeCB.Text = "Draw Sliderend Hit Range";
            this.drawSliderendRangeCB.UseVisualStyleBackColor = true;
            this.drawSliderendRangeCB.CheckedChanged += new System.EventHandler(this.drawSliderendRangeCB_CheckedChanged);
            // 
            // timelineControl
            // 
            this.timelineControl.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.timelineControl.Location = new System.Drawing.Point(133, 539);
            this.timelineControl.Margin = new System.Windows.Forms.Padding(4);
            this.timelineControl.Name = "timelineControl";
            this.timelineControl.Size = new System.Drawing.Size(1188, 28);
            this.timelineControl.TabIndex = 16;
            this.timelineControl.Text = "timelineControl";
            this.timelineControl.Value = 0D;
            this.timelineControl.MouseClick += new System.Windows.Forms.MouseEventHandler(this.timelineControl_MouseClick);
            this.timelineControl.MouseMove += new System.Windows.Forms.MouseEventHandler(this.timelineControl_MouseMove);
            // 
            // canvas
            // 
            this.canvas.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.canvas.BackColor = System.Drawing.Color.Black;
            this.canvas.Location = new System.Drawing.Point(133, 41);
            this.canvas.Margin = new System.Windows.Forms.Padding(4);
            this.canvas.Name = "canvas";
            this.canvas.Size = new System.Drawing.Size(1188, 492);
            this.canvas.TabIndex = 0;
            this.canvas.TabStop = false;
            this.canvas.MouseDown += new System.Windows.Forms.MouseEventHandler(this.canvas_MouseDown);
            this.canvas.MouseMove += new System.Windows.Forms.MouseEventHandler(this.canvas_MouseMove);
            this.canvas.MouseUp += new System.Windows.Forms.MouseEventHandler(this.canvas_MouseUp);
            // 
            // MainForm
            // 
            this.AllowDrop = true;
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1579, 814);
            this.Controls.Add(this.drawSliderendRangeCB);
            this.Controls.Add(this.drawHitWindowsCB);
            this.Controls.Add(this.scaleFramesUd);
            this.Controls.Add(this.scaleFramesBtn);
            this.Controls.Add(this.toKeyboardBtn);
            this.Controls.Add(this.deleteFrameBtn);
            this.Controls.Add(this.addFrameBtn);
            this.Controls.Add(this.centerCurrentFrameBtn);
            this.Controls.Add(this.moveFrameForwardBtn);
            this.Controls.Add(this.moveFrameBackBtn);
            this.Controls.Add(this.drawFramesOnTimelineCB);
            this.Controls.Add(this.isKeyboardCB);
            this.Controls.Add(this.currentHitObjectLabel);
            this.Controls.Add(this.nextObjectBtn);
            this.Controls.Add(this.hitObjectPointsLabel);
            this.Controls.Add(this.label17);
            this.Controls.Add(this.hitObjectErrorLabel);
            this.Controls.Add(this.label15);
            this.Controls.Add(this.currentHitObjectIdLabel);
            this.Controls.Add(this.label14);
            this.Controls.Add(this.brushRadiusLabel);
            this.Controls.Add(this.label13);
            this.Controls.Add(this.brushRadiusTrackBar);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.label12);
            this.Controls.Add(this.next100btn);
            this.Controls.Add(this.next50btn);
            this.Controls.Add(this.acc_300s_tb);
            this.Controls.Add(this.acc_100s_tb);
            this.Controls.Add(this.acc_50s_tb);
            this.Controls.Add(this.acc_misses_tb);
            this.Controls.Add(this.acc_acc_tb);
            this.Controls.Add(this.acc_avg_early_tb);
            this.Controls.Add(this.acc_avg_late_tb);
            this.Controls.Add(this.acc_ur_tb);
            this.Controls.Add(this.label11);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.analyzeAccBtn);
            this.Controls.Add(this.nextMissBtn);
            this.Controls.Add(this.updateTimestampCB);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.flipCursorDataBtn);
            this.Controls.Add(this.flipBeatmapBtn);
            this.Controls.Add(this.cursorTrailValueLabel);
            this.Controls.Add(this.trailLengthInfoLabel);
            this.Controls.Add(this.trailLengthBar);
            this.Controls.Add(this.keyPress1Btn);
            this.Controls.Add(this.keyPress2Btn);
            this.Controls.Add(this.keyPress12Btn);
            this.Controls.Add(this.keyPressNoneBtn);
            this.Controls.Add(this.markAllBtn);
            this.Controls.Add(this.keypressInfoLabel);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.markerInfoLabel);
            this.Controls.Add(this.clearMarksBtn);
            this.Controls.Add(this.markMidBtn);
            this.Controls.Add(this.markOutBtn);
            this.Controls.Add(this.zoomOutBtn);
            this.Controls.Add(this.zoomInBtn);
            this.Controls.Add(this.zoomPanResetBtn);
            this.Controls.Add(this.markInBtn);
            this.Controls.Add(this.songTimeLabel);
            this.Controls.Add(this.songTimeLabelLabel);
            this.Controls.Add(this.togglePauseBtn);
            this.Controls.Add(this.timelineControl);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.cursorTrailLabel);
            this.Controls.Add(this.muteBtn);
            this.Controls.Add(this.playbackSpeedPanel);
            this.Controls.Add(this.playbackSpeedLabel);
            this.Controls.Add(this.volumeLabel);
            this.Controls.Add(this.volumeBar);
            this.Controls.Add(this.canvas);
            this.Controls.Add(this.menuStrip1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.Name = "MainForm";
            this.Text = "osu!ReplayEditor v3";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.DragDrop += new System.Windows.Forms.DragEventHandler(this.MainForm_DragDrop);
            this.DragEnter += new System.Windows.Forms.DragEventHandler(this.MainForm_DragEnter);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.MainForm_MouseDown);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.volumeBar)).EndInit();
            this.playbackSpeedPanel.ResumeLayout(false);
            this.playbackSpeedPanel.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trailLengthBar)).EndInit();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.brushRadiusTrackBar)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.scaleFramesUd)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private Canvas canvas;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.TrackBar volumeBar;
        private System.Windows.Forms.Label volumeLabel;
        private System.Windows.Forms.Label playbackSpeedLabel;
        private System.Windows.Forms.RadioButton playbackRadio025x;
        private System.Windows.Forms.RadioButton playbackRadio050x;
        private System.Windows.Forms.RadioButton playbackRadio075x;
        private System.Windows.Forms.RadioButton playbackRadio100x;
        private System.Windows.Forms.RadioButton playbackRadio150x;
        private System.Windows.Forms.RadioButton playbackRadio005x;
        private System.Windows.Forms.RadioButton playbackRadio400x;
        private System.Windows.Forms.Panel playbackSpeedPanel;
        private System.Windows.Forms.Button muteBtn;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.RadioButton cursorRadioPresses;
        private System.Windows.Forms.RadioButton cursorRadioNormal;
        private System.Windows.Forms.RadioButton cursorRadioKeys;
        private System.Windows.Forms.Label cursorTrailLabel;
        private TimelineControl timelineControl;
        private System.Windows.Forms.Button togglePauseBtn;
        private System.Windows.Forms.Label songTimeLabelLabel;
        private System.Windows.Forms.Label songTimeLabel;
        private System.Windows.Forms.ToolStripMenuItem replayMetadataToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exportAsosrToolStripMenuItem;
        private System.Windows.Forms.Button markInBtn;
        private System.Windows.Forms.Button markOutBtn;
        private System.Windows.Forms.Button markMidBtn;
        private System.Windows.Forms.Button clearMarksBtn;
        private System.Windows.Forms.Label markerInfoLabel;
        private System.Windows.Forms.Label keypressInfoLabel;
        private System.Windows.Forms.Button markAllBtn;
        private System.Windows.Forms.Button keyPress1Btn;
        private System.Windows.Forms.Button keyPress2Btn;
        private System.Windows.Forms.Button keyPress12Btn;
        private System.Windows.Forms.Button keyPressNoneBtn;
        private System.Windows.Forms.TrackBar trailLengthBar;
        private System.Windows.Forms.Label trailLengthInfoLabel;
        private System.Windows.Forms.Label cursorTrailValueLabel;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.ToolStripMenuItem githubToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem githubToolStripMenuItem1;
        private System.Windows.Forms.Button flipBeatmapBtn;
        private System.Windows.Forms.Button flipCursorDataBtn;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button zoomPanResetBtn;
        private System.Windows.Forms.Button zoomInBtn;
        private System.Windows.Forms.Button zoomOutBtn;
        private System.Windows.Forms.RadioButton playbackRadio010x;
        private System.Windows.Forms.CheckBox updateTimestampCB;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ToolStripMenuItem editToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem undoToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem redoToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem keybindReferenceToolStripMenuItem;
        private System.Windows.Forms.Button analyzeAccBtn;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Button nextMissBtn;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label acc_ur_tb;
        private System.Windows.Forms.Label acc_avg_late_tb;
        private System.Windows.Forms.Label acc_avg_early_tb;
        private System.Windows.Forms.Label acc_acc_tb;
        private System.Windows.Forms.Label acc_misses_tb;
        private System.Windows.Forms.Label acc_50s_tb;
        private System.Windows.Forms.Label acc_100s_tb;
        private System.Windows.Forms.Label acc_300s_tb;
        private System.Windows.Forms.Button next50btn;
        private System.Windows.Forms.Button next100btn;
        private System.Windows.Forms.ToolStripMenuItem quickLoadToolStripMenuItem;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.RadioButton toolSelGrabRadioButton;
        private System.Windows.Forms.RadioButton toolBrushRadioButton;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.TrackBar brushRadiusTrackBar;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label brushRadiusLabel;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.Label currentHitObjectIdLabel;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Label hitObjectErrorLabel;
        private System.Windows.Forms.Label hitObjectPointsLabel;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.Button nextObjectBtn;
        private System.Windows.Forms.ToolStripMenuItem analyzeAccTraceToolStripMenuItem;
        private System.Windows.Forms.Label currentHitObjectLabel;
        private System.Windows.Forms.ToolStripMenuItem configToolStripMenuItem;
        private System.Windows.Forms.CheckBox isKeyboardCB;
        private System.Windows.Forms.CheckBox drawFramesOnTimelineCB;
        private System.Windows.Forms.Button moveFrameBackBtn;
        private System.Windows.Forms.Button moveFrameForwardBtn;
        private System.Windows.Forms.Button centerCurrentFrameBtn;
        private System.Windows.Forms.Button addFrameBtn;
        private System.Windows.Forms.Button deleteFrameBtn;
        private System.Windows.Forms.Button toKeyboardBtn;
        private System.Windows.Forms.ToolStripMenuItem changeReplaysBeatmapToolStripMenuItem;
        private System.Windows.Forms.OpenFileDialog openFileDialog2;
        private System.Windows.Forms.Button scaleFramesBtn;
        private System.Windows.Forms.NumericUpDown scaleFramesUd;
        private System.Windows.Forms.CheckBox drawHitWindowsCB;
        private System.Windows.Forms.CheckBox drawSliderendRangeCB;
    }
}

