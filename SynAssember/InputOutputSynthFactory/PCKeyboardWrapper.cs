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

		public PCKeyboardWrapper(int id) : base(new PCKeyboard(), id)
		{
			m_PCKeyboard = (PCKeyboard)m_UserControl;
		}

		public override Rectangle getRect()
		{
			return m_PCKeyboard.getRectangle();
		}

	}
}
