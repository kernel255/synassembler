using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SynthPanels;
using GeneralUtils;
using System.Drawing;

namespace BasicEUSynthFactory
{
    class MIDISequencePlayerWrapper : AbstractSynthPanel
    {
		public MIDISequencePlayerWrapper(int id, SynthDelegateHolder deleHolder, Facilities facilities)
			: base(new MIDISequencePanel(), id, deleHolder, facilities)
        {

        }

		public override System.Windows.Controls.UserControl getUserControlPanel()
		{
			return new MIDISequencePanel();
		}


		public override Rectangle getRect()
		{
			return new Rectangle(0, 0, 150, 100);
		}


	}
}
