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
 
        public override System.Windows.Controls.UserControl  getUserControlPanel()
        {
 	        return new PCMWaveformPanel();
        }

        public override Rectangle getRect()
        {
            return new Rectangle(0, 0, 280, 80);
        }
    }
}
