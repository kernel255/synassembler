using SynthPanels;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Controls;
using System.Windows.Media;
using System.Drawing;
using GeneralUtils;

namespace InputOutputSynthFactory
{
	class PCKeyboardWrapper : AbstractSynthPanel
	{
		PCKeyboard m_PCKeyboard = null;

		public PCKeyboardWrapper(int id, SynthDelegateHolder deleHolder, Facilities facilities)
			: base(new PCKeyboard(), id, deleHolder, facilities)
		{
			m_PCKeyboard = (PCKeyboard)m_UserControl;
		}

		public override Rectangle getRect()
		{
			return m_PCKeyboard.getRectangle();
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
