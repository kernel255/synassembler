using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SynthPanels;
using GeneralUtils;

namespace InputOutputSynthFactory
{
    class MIDIInputWrapper : AbstractSynthPanel
    {
		MIDIInputPanel m_MIDIInPanel;

		public MIDIInputWrapper(int id, SynthDelegateHolder deleHolder, Facilities facilities)
			: base(new MIDIInputPanel(), id, deleHolder, facilities)
        {
			m_MIDIInPanel = (MIDIInputPanel)m_UserControl;
		}

		public System.Drawing.Rectangle getRectangle()
		{
			return new System.Drawing.Rectangle(0, 0, 150, 100);
		}

		override public int GetHorizontalSpan()
		{
			return 1;
		}
		override public int GetVerticalSpan()
		{
			return 1;
		}

	}
}
