using System;
using GeneralUtils;


namespace SynthPanels
{
	public abstract partial class AbstractSynthPanelFactory : ISynthPanelFactory
	{
		protected SynthDelegateHolder deleHolder;
		protected Facilities facilities;
		virtual public void init(SynthDelegateHolder delHold, Facilities facilities)
		{
			deleHolder = delHold;
			this.facilities = facilities;
		}

		abstract public string getEUFactoryName();
		abstract public ISynthPanel createSynthPanel(string name, int id);
		abstract public String[] getEUNames();
	}
}
