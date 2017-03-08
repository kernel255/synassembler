using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SynthPanels;
using GeneralUtils;
using System.Drawing;

namespace BasicEUFact
{
    class MIDISequencePlayerWrapper : AbstractSynthPanel
    {
		MIDISequencePanel m_MIDIPanel;

		public MIDISequencePlayerWrapper(int id, SynthDelegateHolder deleHolder, Facilities facilities)
			: base(new MIDISequencePanel(), id, deleHolder, facilities)
        {
			m_MIDIPanel = (MIDISequencePanel)m_UserControl;

		}

		public override System.Windows.Controls.UserControl getUserControlPanel()
		{
			return m_MIDIPanel;
		}


		public override Rectangle getRect()
		{
			return new Rectangle(0, 0, 300, 100);
		}

		override public int GetHorizontalSpan()
		{
			return 2;
		}
		override public int GetVerticalSpan()
		{
			return 1;
		}


	}
}
