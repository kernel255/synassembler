using SynthPanels;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Controls;
using System.Windows.Media;
using System.Drawing;

namespace InputOutputSynthFactory
{
	class PCKeyboardWrapper : AbstractSynthPanel
	{
		PCKeyboard m_PCKeyboard = null;

		public PCKeyboardWrapper(int id, SynthDelegateHolder deleHolder)
			: base(new PCKeyboard(), id, deleHolder)
		{
			m_PCKeyboard = (PCKeyboard)m_UserControl;
		}

		public override Rectangle getRect()
		{
			return m_PCKeyboard.getRectangle();
		}

	}
}
