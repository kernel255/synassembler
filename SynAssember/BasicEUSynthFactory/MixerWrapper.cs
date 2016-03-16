using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Controls;

using SynthPanels;
using GeneralUtils;

namespace BasicEUSynthFactory
{
	class MixerWrapper : AbstractSynthPanel
	{
		MixerPanel m_MixerPanel;
		Facilities facilities;

		public MixerWrapper(int id, SynthDelegateHolder deleHolder, Facilities facilities) : base(new MixerPanel(id, deleHolder, facilities), id, deleHolder, facilities)
		{
			m_MixerPanel = (MixerPanel)m_UserControl;
			this.facilities = facilities;
		}

		public UserControl getUserControlPanel()
		{
			return m_MixerPanel;
		}
	}
}
