using System.Windows.Forms;
using System.Drawing;

namespace osuReplayEditor
{
    public class TimelineControl: Control
    {
        public double Value
        {
            get { return this.value; }
            set
            {
                if (value != this.value)
                {
                    this.Invalidate();
                    this.value = value;
                }
            }
        }
        private Pen pen;
        private Brush backgroundBrush;
        private Brush foregroundBrush;
        private double value;

        public TimelineControl() : base()
        {
            this.Value = 0;
            this.pen = new Pen(Color.Black);
            this.backgroundBrush = new SolidBrush(Color.LightGray);
            this.foregroundBrush = new SolidBrush(Color.Black);
            this.Paint += Timeline_Paint;
        }

        private void Timeline_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.FillRectangle(this.backgroundBrush, 0, this.Height * 2 / 5, this.Width, this.Height / 5);
            int q = (int)(this.Value * this.Width + 0.5);
            e.Graphics.FillRectangle(this.foregroundBrush, q, 0, 5, this.Height);
        }
    }
}
