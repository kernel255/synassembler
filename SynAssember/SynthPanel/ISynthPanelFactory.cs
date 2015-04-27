using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SynthPanels;
using GeneralUtils;

namespace SynthPanels
{
    public interface ISynthPanelFactory
    {
        String getEUFactoryName();
        String[] getEUNames();
        ISynthPanel createSynthPanel(String name, Int32 id);
		void init(SynthDelegateHolder deleHolder, Facilities facilities);
    }
}
