using System;
using System.Windows.Forms;

namespace osuReplayEditor.MetadataEditor
{
#if DEBUG
    class ValidateTextBox : TextBox
#else
    abstract class ValidateTextBox : TextBox 
#endif
    {
        private string lastValidatedText = "";

        public ValidateTextBox()
        {
            LostFocus += ValidateTextBox_LostFocus;
        }

        private void ValidateTextBox_LostFocus(object sender, EventArgs e)
        {
            if (Validate(Text))
            {
                lastValidatedText = Text;
            }
            else
            {
                Text = lastValidatedText;
            }
        }

#if DEBUG
        protected virtual bool Validate(string text)
        {
            return false;
        }
#else
        protected abstract bool Validate(string text);
#endif
    }
}
