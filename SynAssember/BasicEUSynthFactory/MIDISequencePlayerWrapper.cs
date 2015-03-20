using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SynthPanels;

namespace BasicEUSynthFactory
{
    class MIDISequencePlayerWrapper : AbstractSynthPanel
    {
        public MIDISequencePlayerWrapper(int id) : base(new MIDISequencePanel(), id)
        {

        }
        
    }
}
