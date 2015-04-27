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
		public PCMWaveformWrapper(int id, SynthDelegateHolder deleHolder, Facilities facilities)
			: base(new PCMWaveformPanel(), id, deleHolder, facilities)
        {

        }
 
        public System.Windows.Controls.UserControl  getUserControlPanel()
        {
 	        return new PCMWaveformPanel();
        }

        public Rectangle getRect()
        {
            return new Rectangle(0, 0, 300, 300);
        }
    }
}
