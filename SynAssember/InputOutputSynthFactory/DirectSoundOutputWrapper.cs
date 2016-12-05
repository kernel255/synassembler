using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SynthPanels;
using System.Drawing;
using GeneralUtils;

namespace InputOutputSynthFactory
{
    class DirectSoundOutputWrapper : AbstractSynthPanel
    {
		public DirectSoundOutputWrapper(int id, SynthDelegateHolder deleHolder, Facilities facilities)
			: base(new DirectSoundOutputPanel(), id, deleHolder, facilities)
        {
			m_DSPanel = (DirectSoundOutputPanel)m_UserControl;
        }

		public override System.Drawing.Rectangle getRect()
		{
			return m_DSPanel.getRectangle();
		}

		override public int GetHorizontalSpan()
		{
			return 1;
		}
		override public int GetVerticalSpan()
		{
			return 1;
		}

		DirectSoundOutputPanel m_DSPanel;
    }
}
