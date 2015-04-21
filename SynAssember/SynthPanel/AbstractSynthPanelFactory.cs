using System;

namespace SynthPanels
{
	public abstract partial class AbstractSynthPanelFactory : ISynthPanelFactory
	{
		protected SynthDelegateHolder deleHolder;
		virtual public void init(SynthDelegateHolder delHold)
		{
			deleHolder = delHold;
		}

		protected DelegateFactoryInterface delegateFactory;
		virtual public void init(DelegateFactoryInterface delegInterf)
		{
			delegateFactory = delegInterf;
		}

		abstract public string getEUFactoryName();
		abstract public ISynthPanel createSynthPanel(string name, int id);
		abstract public String[] getEUNames();
	}
}
