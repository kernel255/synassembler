using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Controls;
using System.Xml;

using SynthPanels;
using GeneralUtils;
using System.Globalization;
using System.Drawing;

namespace BasicEUSynthFactory
{
	class MixerPanelWrapper : AbstractSynthPanel
	{
		MixerPanel m_MixerPanel;

		public MixerPanelWrapper(int id, SynthDelegateHolder deleHolder, Facilities facilities) : base(new MixerPanel(id, deleHolder, facilities), id, deleHolder, facilities)
		{
			m_MixerPanel = (MixerPanel)m_UserControl;
			this.facilities = facilities;
		}

		public override UserControl getUserControlPanel()
		{
			return m_MixerPanel;
		}


		string getXMLInName(int port)
		{
			return XML_IN_LVL + port;
		}

		string getXMLInMuteName(int port)
		{
			return XML_IN_MUTE + port;
		}

		public override void write(XmlTextWriter writer)
		{
			base.write(writer);
			CultureInfo cInfo = new CultureInfo("en-US");
			bool b;
			double d;
			string val;
			d = delegateHolder.readEUDprop(m_EUId, MixerPanel.OUT_LEVEL_PROPERTY_INDEX);
			val = d.ToString(cInfo);
			writer.WriteAttributeString(XML_OUTPUT_LVL, val);

			for(int port=0;port<MixerPanel.NUM_INPUTS;port++)
			{
				string xmlName = getXMLInName(port);
				d = delegateHolder.readEUDprop(m_EUId, MixerPanel.IN0_LEVEL_PROPERTY_INDEX + port);
				val = d.ToString(cInfo);
				writer.WriteAttributeString(xmlName, val);
			}
			b = delegateHolder.readEUBProp(m_EUId, MixerPanel.OUT_MUTE_PROPERTY_INDEX);
			val = b.ToString(cInfo);
			writer.WriteAttributeString(XML_OUTPUT_MUTE, val);
			for (int port = 0; port < MixerPanel.NUM_INPUTS; port++)
			{
				string xmlName = getXMLInMuteName(port);
				b = delegateHolder.readEUBProp(m_EUId, MixerPanel.IN0_MUTE_PROPERTY_INDEX + port);
				val = b.ToString(cInfo);
				writer.WriteAttributeString(xmlName, val);
			}

		}

		public override void read(XmlTextReader reader)
		{
			base.read(reader);
			string str;
			double d;
			bool b;
			CultureInfo cInfo = new CultureInfo("en-US");
			str = reader.GetAttribute(XML_OUTPUT_LVL);
			d = Double.Parse(str);
			delegateHolder.writeEUDProp(m_EUId, MixerPanel.OUT_LEVEL_PROPERTY_INDEX, d);
			for(int port =0;port<MixerPanel.NUM_INPUTS;port++)
			{
				str = reader.GetAttribute(getXMLInName(port));
				d = Double.Parse(str);
				delegateHolder.writeEUDProp(m_EUId, MixerPanel.IN0_LEVEL_PROPERTY_INDEX + port, d);
			}
			str = reader.GetAttribute(XML_OUTPUT_MUTE);
			b = Boolean.Parse(str);
			delegateHolder.writeEUBProp(m_EUId, MixerPanel.OUT_MUTE_PROPERTY_INDEX, b);
			for (int port = 0; port < MixerPanel.NUM_INPUTS; port++)
			{
				str = reader.GetAttribute(getXMLInMuteName(port));
				b = Boolean.Parse(str);
				delegateHolder.writeEUBProp(m_EUId, MixerPanel.IN0_MUTE_PROPERTY_INDEX + port, b);
			}
		}

		public override bool hasChanged()
		{
			return false;
		}

		public override void readParametersFromEngine()

		{
			m_MixerPanel.readParametersFromEngine();
		}

		public override Rectangle getRect()
		{
			return m_MixerPanel.getRectangle();
		}

		static string XML_OUTPUT_LVL = "OutputLevel";
		static string XML_IN_LVL = "InLevel";
		static string XML_OUTPUT_MUTE = "OutputMute";
		static string XML_IN_MUTE = "InMute";

		internal static int NUM_INPUTS = 4;
	}
}
