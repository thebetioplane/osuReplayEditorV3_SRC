using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace osuReplayEditor.MetadataEditor
{
    class Int16TextBox : ValidateTextBox
    {
        private short val;
        public short Value
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
            if (short.TryParse(text, out short res))
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
