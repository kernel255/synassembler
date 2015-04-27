using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SynthPanels;
using GeneralUtils;

namespace BasicEUSynthFactory
{
    class MIDISequencePlayerWrapper : AbstractSynthPanel
    {
		public MIDISequencePlayerWrapper(int id, SynthDelegateHolder deleHolder, Facilities facilities)
			: base(new MIDISequencePanel(), id, deleHolder, facilities)
        {

        }
        
    }
}
