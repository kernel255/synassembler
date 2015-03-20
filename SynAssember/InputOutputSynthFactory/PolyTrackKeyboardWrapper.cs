using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SynthPanels;

namespace InputOutputSynthFactory
{
    class PolyTrackKeyboardWrapper : AbstractSynthPanel
    {
        public PolyTrackKeyboardWrapper(int id) : base(new PolyTrackKeyboardPanel(), id)
        {
        }

        public string getName()
        {
            return "PolyTrack Keyboard";
        }


    }
}
