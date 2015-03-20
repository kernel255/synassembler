using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SynthPanels
{
    public abstract partial class AbstractSynthPanelFactory : ISynthPanelFactory
    {
        protected SynthPanelManager.WriteEUDoubleProperty writeEUDProp;
        virtual public void init(SynthPanelManager.WriteEUDoubleProperty writeEUDProp)
        {
            this.writeEUDProp = writeEUDProp;
        }

        abstract public string getEUFactoryName();
        abstract public ISynthPanel createSynthPanel(string name, int id);
        abstract public String[] getEUNames();
    }
}
