using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SynthPanels;

namespace SynthPanels
{
    public interface ISynthPanelFactory
    {
        String getEUFactoryName();
        String[] getEUNames();
        ISynthPanel createSynthPanel(String name, Int32 id);
		void init(SynthDelegateHolder deleHolder);
    }
}
