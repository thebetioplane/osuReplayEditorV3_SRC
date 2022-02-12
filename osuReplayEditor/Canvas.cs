using System.Windows.Forms;

namespace osuReplayEditor
{
    public class Canvas : UserControl
    {
#if DEBUG
        private bool doPaint = false;
#endif
        public Canvas()
        {
        }

        public void Begin()
        {
            const int SUCCESS = 0;
            const int NULL_HWND = 1;
            const int AUDIO_FAILURE = 2;
            const int OSUDB_FAILURE = 3;
            const int REPLAYENGINE_FAILURE = 4;
            const int BEATMAPENGINE_FAILURE = 5;
            const int TEXTURE_FAILURE = 6;
            int status = API.Init(this.Handle, Config.mainConfig.OsuDbPath, Config.mainConfig.SongDirPath);
            if (status == TEXTURE_FAILURE)
            {
                System.Diagnostics.Process.Start(Application.ExecutablePath, "-repair_textures");
                System.Environment.Exit(0);
            }
            else if (status != SUCCESS)
            {
                string detail;
                switch (status)
                {
                    case SUCCESS:
                        detail = "no error";
                        break;
                    case NULL_HWND:
                        detail = "null hWnd";
                        break;
                    case AUDIO_FAILURE:
                        detail = "audio engine failure";
                        break;
                    case OSUDB_FAILURE:
                        detail = "osu!.db failure";
                        break;
                    case REPLAYENGINE_FAILURE:
                        detail = "replay engine failure";
                        break;
                    case BEATMAPENGINE_FAILURE:
                        detail = "beatmap engine failure";
                        break;
                    case TEXTURE_FAILURE:
                        detail = "failed to load some textures";
                        break;
                    default:
                        detail = $"error code {status}";
                        break;
                }
                throw new System.Exception($"Failure during initialization... {detail}");
            }
#if DEBUG
            doPaint = true;
#endif
            this.Resize += Canvas_Resize;
            this.Canvas_Resize(null, null);
        }

        protected override void OnPaint(PaintEventArgs e)
        {
#if DEBUG
            if (!doPaint)
            {
                e.Graphics.FillRectangle(System.Drawing.Brushes.CornflowerBlue, new System.Drawing.Rectangle(0, 0, this.Width, this.Height));
                return;
            }
#endif

            API.Render();
        }

        protected override void OnPaintBackground(PaintEventArgs e)
        {
        }

        private void Canvas_Resize(object sender, System.EventArgs e)
        {
           API.OnResize(this.Width, this.Height);
        }
    }
}
