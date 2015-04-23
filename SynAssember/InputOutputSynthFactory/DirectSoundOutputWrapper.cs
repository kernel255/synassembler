using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SynthPanels;
using System.Drawing;

namespace InputOutputSynthFactory
{
    class DirectSoundOutputWrapper : AbstractSynthPanel
    {
        public DirectSoundOutputWrapper(int id, SynthDelegateHolder deleHolder) : base(new DirectSoundOutputPanel(), id, deleHolder)
        {
			m_DSPanel = (DirectSoundOutputPanel)m_UserControl;
        }

		public override System.Drawing.Rectangle getRect()
		{
			return m_DSPanel.getRectangle();
		}

		DirectSoundOutputPanel m_DSPanel;
    }
}
