using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SynthPanels;

namespace InputOutputSynthFactory
{
    class MIDIInputWrapper : AbstractSynthPanel
    {
        public MIDIInputWrapper(int id) : base(new MIDIInputPanel(), id)
        {

        }

    }
}
