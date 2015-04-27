using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SynthPanels;
using GeneralUtils;

namespace InputOutputSynthFactory
{
    class PolyTrackKeyboardWrapper : AbstractSynthPanel
    {
		public PolyTrackKeyboardWrapper(int id, SynthDelegateHolder deleHolder, Facilities facilities)
			: base(new PolyTrackKeyboardPanel(), id, deleHolder, facilities)
        {
        }

        public string getName()
        {
            return "PolyTrack Keyboard";
        }


    }
}
