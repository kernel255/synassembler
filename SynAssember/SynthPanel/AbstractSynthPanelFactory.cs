using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SynthPanels
{
    public abstract partial class AbstractSynthPanelFactory : ISynthPanelFactory
    {
        protected WriteEUDoubleProperty writeEUDProp;
        virtual public void init(WriteEUDoubleProperty writeEUDProp)
        {
            this.writeEUDProp = writeEUDProp;
        }

        abstract public string getEUFactoryName();
        abstract public ISynthPanel createSynthPanel(string name, int id);
        abstract public String[] getEUNames();
    }
}
