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
        void init(SynthPanelManager.WriteEUDoubleProperty writeEUProp);
        ISynthPanel createSynthPanel(String name, Int32 id);
    }
}
