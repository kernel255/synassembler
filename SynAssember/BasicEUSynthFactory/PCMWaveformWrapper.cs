using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SynthPanels;
using System.Drawing;
using GeneralUtils;

namespace BasicEUSynthFactory
{
    class PCMWaveformWrapper : AbstractSynthPanel
    {
		PCMWaveformPanel m_WaveformPanel;

		public PCMWaveformWrapper(int id, SynthDelegateHolder deleHolder, Facilities facilities)
			: base(new PCMWaveformPanel(), id, deleHolder, facilities)
        {
			m_WaveformPanel = (PCMWaveformPanel)m_UserControl;
        }
 
        public override System.Windows.Controls.UserControl getUserControlPanel()
        {
			return m_WaveformPanel;
        }

        public override Rectangle getRect()
        {
            return new Rectangle(0, 0, 300, 200);
        }

		override public int GetHorizontalSpan()
		{
			return 2;
		}
		override public int GetVerticalSpan()
		{
			return 2;
		}

	}
}
