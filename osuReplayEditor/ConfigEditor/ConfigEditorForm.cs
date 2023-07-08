using System;
using System.Diagnostics;
using System.Windows.Forms;
using System.IO;

namespace osuReplayEditor.ConfigEditor
{
    public partial class ConfigEditorForm : Form
    {
        public bool Standalone { get; set; }

        public ConfigEditorForm()
        {
            Standalone = true;
            InitializeComponent();
        }

        private void ErrorMessage(string message)
        {
            MessageBox.Show(this, message, "Error");
        }

        private void ExitAndRestart()
        {
            Process.Start(Application.ExecutablePath);
            Environment.Exit(0);
        }

        private void UpdateConfigFromForm()
        {
            osuDbPathTextBox.Text = osuDbPathTextBox.Text.Trim();
            songFolderPathTextbox.Text = songFolderPathTextbox.Text.Trim();
            replayFolderPathTextbox.Text = replayFolderPathTextbox.Text.Trim();
            Config.mainConfig.OsuDbPath = (osuDbPathTextBox.Text.Length > 0) ? osuDbPathTextBox.Text : null;
            Config.mainConfig.SongDirPath = (songFolderPathTextbox.Text.Length > 0) ? songFolderPathTextbox.Text : null;
            Config.mainConfig.ReplayDirPath = (replayFolderPathTextbox.Text.Length > 0) ? replayFolderPathTextbox.Text : null;
        }

        private void WriteConfigFile()
        {
            Config.mainConfig.CreateNewConfigFile();
        }

        private bool ValidateConfigFile()
        {
            try
            {
                Config.mainConfig.Validate();
                return true;
            }
            catch (Exception e)
            {
                ErrorMessage($"Your config has the following error:\n\n{e.Message}");
                return false;
            }
        }

        private void accept_btn_Click(object sender, EventArgs e)
        {
            UpdateConfigFromForm();
            if (ValidateConfigFile())
            {
                WriteConfigFile();
                Close();
                if (Standalone)
                {
                    ExitAndRestart();
                }
                else
                {
                    var result = MessageBox.Show(this, "Some config changes require a restart to take effect. Would you like to restart now?", "Restart Required", MessageBoxButtons.YesNo);
                    if (result == DialogResult.Yes)
                    {
                        ExitAndRestart();
                    }
                }
            }
        }

        private void edit_in_notepad_btn_Click(object sender, EventArgs e)
        {
            UpdateConfigFromForm();
            WriteConfigFile();
            Process.Start("notepad.exe", Config.mainConfig.FileName);
            Close();
        }

        private void cancel_btn_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void ConfigEditorForm_Shown(object sender, EventArgs e)
        {
            osuDbPathTextBox.Text = Config.mainConfig.OsuDbPath ?? "";
            songFolderPathTextbox.Text = Config.mainConfig.SongDirPath ?? "";
            replayFolderPathTextbox.Text = Config.mainConfig.ReplayDirPath ?? "";
        }

        private void selectOsuFolderBtn_Click(object sender, EventArgs e)
        {
            openFileDialog.ShowDialog(this);
        }

        private bool SetPathsFromOsuInstallation(string path)
        {
            if (!Directory.Exists(path))
                return false;
            string maybeOsuDbPath = Path.Combine(path, "osu!.db");
            if (!File.Exists(maybeOsuDbPath))
                return false;
            osuDbPathTextBox.Text = maybeOsuDbPath;
            songFolderPathTextbox.Text = Path.Combine(path, "Songs");
            string maybeReplayDirPath = Path.Combine(path, "Replays");
            replayFolderPathTextbox.Text = Directory.Exists(maybeReplayDirPath) ? maybeReplayDirPath : "";
            return true;
        }

        private bool AutoLocate(string fileName)
        {
            for (int i = 0; i < 5; ++i)
            {
                if (fileName == null || fileName.Length == 0)
                    return false;
                if (SetPathsFromOsuInstallation(fileName))
                    return true;
                fileName = Path.GetDirectoryName(fileName);
            }
            return false;
        }

        private void openFileDialog_FileOk(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if (!AutoLocate(openFileDialog.FileName))
            {
                ErrorMessage("The selected folder does not appear to contain a valid osu! installation. Please select a different folder or try manually setting the paths.");
            }
        }
    }
}
