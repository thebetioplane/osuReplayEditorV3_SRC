using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace osuReplayEditor.MetadataEditor
{
    class Int32TextBox : ValidateTextBox
    {
        private int val;
        public int Value
        {
            get { return val; }
            set
            {
                val = value;
                Text = value.ToString();
            }
        }

        protected override bool Validate(string text)
        {
            if (int.TryParse(text, out int res))
            {
                Value = res;
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}
