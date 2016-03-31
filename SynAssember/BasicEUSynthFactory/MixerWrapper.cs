using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Controls;
using System.Xml;

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

		public override UserControl getUserControlPanel()
		{
			return m_MixerPanel;
		}

		public override void write(XmlTextWriter writer)
		{
			base.write(writer);
		}

		public override void read(XmlTextReader reader)
		{
			base.read(reader);
		}

		public override bool hasChanged()
		{
			return false;
		}

		public override void readParametersFromEngine()

		{
			m_MixerPanel.readParametersFromEngine();
		}


		static String XML_OUTPUT_LVL = "OutputLevel";
		static String XML_INPUT_LVL_ = "InputLevel";
		internal static int NUM_INPUTS = 4;
	}
}
