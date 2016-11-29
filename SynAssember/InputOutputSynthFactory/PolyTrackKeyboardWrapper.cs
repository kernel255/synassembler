using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SynthPanels;
using GeneralUtils;
using System.Drawing;

namespace InputOutputSynthFactory
{
    class PolyTrackKeyboardWrapper : AbstractSynthPanel
    {
		public PolyTrackKeyboardWrapper(int id, SynthDelegateHolder deleHolder, Facilities facilities)
			: base(new PolyTrackKeyboardPanel(), id, deleHolder, facilities)
        {
			m_PTKPanel = (PolyTrackKeyboardPanel)m_UserControl;

		}

        public string getName()
        {
            return "PolyTrack Keyboard";
        }

		public override System.Drawing.Rectangle getRect()
		{
			return new Rectangle(0, 0, 150, 200);
		}

		PolyTrackKeyboardPanel m_PTKPanel;
	}
}
