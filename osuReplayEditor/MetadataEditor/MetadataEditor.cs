using System;
using System.Windows.Forms;

namespace osuReplayEditor.MetadataEditor
{
    public partial class MetadataEditorForm : Form
    {
        private const int ModNoFail = 1;
        private const int ModEasy = 2;
        private const int ModTouchDevice = 4;
        private const int ModHidden = 8;
        private const int ModHardRock = 16;
        private const int ModSuddenDeath = 32;
        private const int ModDoubleTime = 64;
        private const int ModRelax = 128;
        private const int ModHalfTime = 256;
        private const int ModNightcore = 512;
        private const int ModFlashlight = 1024;
        private const int ModAutoplay = 2048;
        private const int ModSpunOut = 4096;
        private const int ModAutopilot = 8192;
        private const int ModPerfect = 16384;
        private const int ModKey4 = 32768;
        private const int ModKey5 = 65536;
        private const int ModKey6 = 131072;
        private const int ModKey7 = 262144;
        private const int ModKey8 = 524288;
        private const int ModFadeIn = 1048576;
        private const int ModRandom = 2097152;
        private const int ModCinema = 4194304;
        private const int ModKey9 = 16777216;
        private const int ModCoop = 33554432;
        private const int ModKey1 = 67108864;
        private const int ModKey3 = 134217728;
        private const int ModKey2 = 268435456;

        public MetadataEditorForm()
        {
            InitializeComponent();
            dateTimePicker.CustomFormat = "yyyy-MM-dd HH:mm:ss";
        }

        public bool IsHardRock { get; private set; }

        public bool IsEasy { get; private set; }


        public DateTime Timestamp
        {
            get
            {
                return dateTimePicker.Value;
            }
            set
            {
                try
                {
                    dateTimePicker.Value = value;
                }
                catch
                {
                }
            }
        }

        public void FromAPI()
        {
            byte gamemode = API.Replay_GetGamemode();
            switch (gamemode)
            {
                case 1:
                    radioButtonTaiko.Checked = true;
                    break;
                case 2:
                    radioButtonCatchTheBeat.Checked = true;
                    break;
                case 3:
                    radioButtonMania.Checked = true;
                    break;
                default:
                    radioButtonStandard.Checked = true;
                    break;
            }
            versionTextBox.Value = API.Replay_GetVersion();
            Timestamp = TimeZoneInfo.ConvertTimeFromUtc(DateTime.FromBinary(API.Replay_GetTimestamp()), TimeZoneInfo.Local);
            playerNameTextBox.Text = getPlayerName();
            num300TextBox.Value = API.Replay_GetNum300();
            num100TextBox.Value = API.Replay_GetNum100();
            num50TextBox.Value = API.Replay_GetNum50();
            scoreTextBox.Value = API.Replay_GetTotalScore();
            numGekiTextBox.Value = API.Replay_GetNumGeki();
            numKatuTextBox.Value = API.Replay_GetNumKatu();
            numMissTextBox.Value = API.Replay_GetNumMiss();
            maxComboTextBox.Value = API.Replay_GetMaxCombo();
            if (API.Replay_GetFullCombo())
                radioButtonFCYes.Checked = true;
            else
                radioButtonFCNo.Checked = true;
            int mods = API.Replay_GetMods();
            checkBoxNoFail.Checked = getMod(mods, ModNoFail);
            IsEasy = getMod(mods, ModEasy);
            checkBoxEasy.Checked = IsEasy;
            checkBoxTouchDevice.Checked = getMod(mods, ModTouchDevice);
            checkBoxHidden.Checked = getMod(mods, ModHidden);
            IsHardRock = getMod(mods, ModHardRock);
            checkBoxHardRock.Checked = IsHardRock;
            checkBoxSuddenDeath.Checked = getMod(mods, ModSuddenDeath);
            checkBoxDoubleTime.Checked = getMod(mods, ModDoubleTime);
            checkBoxRelax.Checked = getMod(mods, ModRelax);
            checkBoxHalfTime.Checked = getMod(mods, ModHalfTime);
            checkBoxNightcore.Checked = getMod(mods, ModNightcore);
            checkBoxFlashlight.Checked = getMod(mods, ModFlashlight);
            checkBoxAutoplay.Checked = getMod(mods, ModAutoplay);
            checkBoxSpunOut.Checked = getMod(mods, ModSpunOut);
            checkBoxAutopilot.Checked = getMod(mods, ModAutopilot);
            checkBoxPerfectMod.Checked = getMod(mods, ModPerfect);
            checkBoxKey1.Checked = getMod(mods, ModKey1);
            checkBoxKey2.Checked = getMod(mods, ModKey2);
            checkBoxKey3.Checked = getMod(mods, ModKey3);
            checkBoxKey4.Checked = getMod(mods, ModKey4);
            checkBoxKey5.Checked = getMod(mods, ModKey5);
            checkBoxKey6.Checked = getMod(mods, ModKey6);
            checkBoxKey7.Checked = getMod(mods, ModKey7);
            checkBoxKey8.Checked = getMod(mods, ModKey8);
            checkBoxKey9.Checked = getMod(mods, ModKey9);
            checkBoxFadeIn.Checked = getMod(mods, ModFadeIn);
            checkBoxRandom.Checked = getMod(mods, ModRandom);
            checkBoxCinema.Checked = getMod(mods, ModCinema);
            checkBoxCoop.Checked = getMod(mods, ModCoop);
        }

        public void ToAPI()
        {
            if (radioButtonTaiko.Checked)
                API.Replay_SetGamemode(1);
            else if (radioButtonCatchTheBeat.Checked)
                API.Replay_SetGamemode(2);
            else if (radioButtonMania.Checked)
                API.Replay_SetGamemode(3);
            else
                API.Replay_SetGamemode(0);
            API.Replay_SetVersion(versionTextBox.Value);
            API.Replay_SetTimestamp(TimeZoneInfo.ConvertTimeToUtc(Timestamp, TimeZoneInfo.Local).Ticks);
            API.Replay_SetPlayerName(playerNameTextBox.Text, playerNameTextBox.Text.Length);
            API.Replay_SetNum300(num300TextBox.Value);
            API.Replay_SetNum100(num100TextBox.Value);
            API.Replay_SetNum50(num50TextBox.Value);
            API.Replay_SetTotalScore(scoreTextBox.Value);
            API.Replay_SetNumGeki(numGekiTextBox.Value);
            API.Replay_SetNumKatu(numKatuTextBox.Value);
            API.Replay_SetNumMiss(numMissTextBox.Value);
            API.Replay_SetMaxCombo(maxComboTextBox.Value);
            API.Replay_SetFullCombo(radioButtonFCYes.Checked);
            int mods = 0;
            setMod(ref mods, ModNoFail, checkBoxNoFail.Checked);
            setMod(ref mods, ModEasy, checkBoxEasy.Checked);
            setMod(ref mods, ModTouchDevice, checkBoxTouchDevice.Checked);
            setMod(ref mods, ModHidden, checkBoxHidden.Checked);
            setMod(ref mods, ModHardRock, checkBoxHardRock.Checked);
            setMod(ref mods, ModSuddenDeath, checkBoxSuddenDeath.Checked);
            setMod(ref mods, ModDoubleTime, checkBoxDoubleTime.Checked);
            setMod(ref mods, ModRelax, checkBoxRelax.Checked);
            setMod(ref mods, ModHalfTime, checkBoxHalfTime.Checked);
            setMod(ref mods, ModNightcore, checkBoxNightcore.Checked);
            setMod(ref mods, ModFlashlight, checkBoxFlashlight.Checked);
            setMod(ref mods, ModAutoplay, checkBoxAutoplay.Checked);
            setMod(ref mods, ModSpunOut, checkBoxSpunOut.Checked);
            setMod(ref mods, ModAutopilot, checkBoxAutopilot.Checked);
            setMod(ref mods, ModPerfect, checkBoxPerfectMod.Checked);
            setMod(ref mods, ModKey1, checkBoxKey1.Checked);
            setMod(ref mods, ModKey2, checkBoxKey2.Checked);
            setMod(ref mods, ModKey3, checkBoxKey3.Checked);
            setMod(ref mods, ModKey4, checkBoxKey4.Checked);
            setMod(ref mods, ModKey5, checkBoxKey5.Checked);
            setMod(ref mods, ModKey6, checkBoxKey6.Checked);
            setMod(ref mods, ModKey7, checkBoxKey7.Checked);
            setMod(ref mods, ModKey8, checkBoxKey8.Checked);
            setMod(ref mods, ModKey9, checkBoxKey9.Checked);
            setMod(ref mods, ModFadeIn, checkBoxFadeIn.Checked);
            setMod(ref mods, ModRandom, checkBoxRandom.Checked);
            setMod(ref mods, ModCinema, checkBoxCinema.Checked);
            setMod(ref mods, ModCoop, checkBoxCoop.Checked);
            API.Replay_SetMods(mods);
        }

        private string getPlayerName()
        {
            byte[] buf = new byte[255];
            int len = buf.Length;
            API.Replay_GetPlayerName(buf, ref len);
            return System.Text.Encoding.ASCII.GetString(buf, 0, len);
        }

        private void setMod(ref int mods, int mod, bool value)
        {
            if (value)
                mods |= mod;
            else
                mods &= ~mod;
        }

        private static bool getMod(int mods, int mod)
        {
            return (mods & mod) != 0;
        }

        private void buttonRevert_Click(object sender, System.EventArgs e)
        {
            FromAPI();
        }

        private void buttonApply_Click(object sender, System.EventArgs e)
        {
            ToAPI();
            Close();
        }

        private void MetadataEditorForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            FromAPI();
        }
    }
}
