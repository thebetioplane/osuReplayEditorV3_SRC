using System;
using System.Windows.Forms;

namespace osuReplayEditor.MetadataEditor
{
    public partial class MetadataEditorForm : Form
    {
        private const uint ModNoFail = 1;
        private const uint ModEasy = 2;
        private const uint ModTouchDevice = 4;
        private const uint ModHidden = 8;
        private const uint ModHardRock = 16;
        private const uint ModSuddenDeath = 32;
        private const uint ModDoubleTime = 64;
        private const uint ModRelax = 128;
        private const uint ModHalfTime = 256;
        private const uint ModNightcore = 512;
        private const uint ModFlashlight = 1024;
        private const uint ModAutoplay = 2048;
        private const uint ModSpunOut = 4096;
        private const uint ModAutopilot = 8192;
        private const uint ModPerfect = 16384;
        private const uint ModKey4 = 32768;
        private const uint ModKey5 = 65536;
        private const uint ModKey6 = 131072;
        private const uint ModKey7 = 262144;
        private const uint ModKey8 = 524288;
        private const uint ModFadeIn = 1048576;
        private const uint ModRandom = 2097152;
        private const uint ModCinema = 4194304;
        private const uint ModTargetPractice = 8388608;
        private const uint ModKey9 = 16777216;
        private const uint ModCoop = 33554432;
        private const uint ModKey1 = 67108864;
        private const uint ModKey3 = 134217728;
        private const uint ModKey2 = 268435456;
        private const uint ModScoreV2 = 536870912;
        private const uint ModMirror = 1073741824;
        private const uint Mod31 = 2147483648;

        public MetadataEditorForm()
        {
            InitializeComponent();
            dateTimePicker.CustomFormat = "yyyy-MM-dd HH:mm:ss";
        }

        public bool IsHardRock { get; private set; }

        public bool IsEasy { get; private set; }

        public bool IsDoubleTime { get; private set; }

        public bool IsHalfTime { get; private set; }


        public DateTime PlayTimestamp
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
            PlayTimestamp = TimeZoneInfo.ConvertTimeFromUtc(DateTime.FromBinary(API.Replay_GetPlayTimestamp()), TimeZoneInfo.Local);
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
            uint mods = API.Replay_GetMods();
            checkBoxNoFail.Checked = getMod(mods, ModNoFail);
            // Set IsEasy.
            IsEasy = getMod(mods, ModEasy);
            checkBoxEasy.Checked = IsEasy;
            checkBoxTouchDevice.Checked = getMod(mods, ModTouchDevice);
            checkBoxHidden.Checked = getMod(mods, ModHidden);
            // Set IsHardRock.
            IsHardRock = getMod(mods, ModHardRock);
            checkBoxHardRock.Checked = IsHardRock;
            checkBoxSuddenDeath.Checked = getMod(mods, ModSuddenDeath);
            // Set IsDoubleTime.
            IsDoubleTime = getMod(mods, ModDoubleTime);
            checkBoxDoubleTime.Checked = IsDoubleTime;
            checkBoxRelax.Checked = getMod(mods, ModRelax);
            // Set IsHalfTime.
            IsHalfTime = getMod(mods, ModHalfTime);
            checkBoxHalfTime.Checked = IsHalfTime;
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
            checkBoxTargetPractice.Checked = getMod(mods, ModTargetPractice);
            checkBoxScoreV2.Checked = getMod(mods, ModScoreV2);
            checkBoxMirror.Checked = getMod(mods, ModMirror);
            checkBoxMod31.Checked = getMod(mods, Mod31);
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
            API.Replay_SetPlayTimestamp(TimeZoneInfo.ConvertTimeToUtc(PlayTimestamp, TimeZoneInfo.Local).Ticks);
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
            uint mods = 0;
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
            setMod(ref mods, ModTargetPractice, checkBoxTargetPractice.Checked);
            setMod(ref mods, ModScoreV2, checkBoxScoreV2.Checked);
            setMod(ref mods, ModMirror, checkBoxMirror.Checked);
            setMod(ref mods, Mod31, checkBoxMod31.Checked);
            API.Replay_SetMods(mods);
        }

        private string getPlayerName()
        {
            int len = 0;
            API.Replay_GetPlayerName(null, ref len);
            byte[] buf = new byte[len];
            API.Replay_GetPlayerName(buf, ref len);
            return System.Text.Encoding.ASCII.GetString(buf, 0, len);
        }

        private void setMod(ref uint mods, uint mod, bool value)
        {
            if (value)
                mods |= mod;
            else
                mods &= ~mod;
        }

        private static bool getMod(uint mods, uint mod)
        {
            return (mods & mod) != 0;
        }

        private void buttonRevert_Click(object sender, System.EventArgs e)
        {
            FromAPI();
        }

        private void buttonApply_Click(object sender, System.EventArgs e)
        {
            API.MakeUndoSnapshot();
            ToAPI();
            Close();
        }

        private void MetadataEditorForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            FromAPI();
        }
    }
}
