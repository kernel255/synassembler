using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SynthPanels;

namespace BasicEUSynthFactory
{
    class MIDISequencePlayerWrapper : AbstractSynthPanel
    {
        public MIDISequencePlayerWrapper(int id, SynthDelegateHolder deleHolder) : base(new MIDISequencePanel(), id, deleHolder)
        {

        }
        
    }
}
