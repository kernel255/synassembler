using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SynthPanels;

namespace InputOutputSynthFactory
{
    class PolyTrackKeyboardWrapper : AbstractSynthPanel
    {
		public PolyTrackKeyboardWrapper(int id, SynthDelegateHolder deleHolder)
			: base(new PolyTrackKeyboardPanel(), id, deleHolder)
        {
        }

        public string getName()
        {
            return "PolyTrack Keyboard";
        }


    }
}
