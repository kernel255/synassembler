using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SynthPanels;
using System.Windows.Controls;
using System.Drawing;
using System.Xml;
using System.Globalization;

using GeneralUtils;

namespace BasicEUSynthFactory
{
    class OscillatorPanelWrapper : AbstractSynthPanel
    {

        OscillatorPanel m_OscillatorPanel = null;
		Facilities facilities;

		public OscillatorPanelWrapper(int id, SynthDelegateHolder deleHolder, Facilities facilities) : base(new OscillatorPanel(id, deleHolder, facilities), id, deleHolder, facilities)
		{
            m_OscillatorPanel = (OscillatorPanel)m_UserControl;
			this.facilities = facilities;
        }

        public UserControl getUserControlPanel()
        {
            return m_OscillatorPanel;
        }

        public override Rectangle getRect()
        {
            return m_OscillatorPanel.getRectangle();
        }

		static string XML_WAVE_ID = "WaveId";
		static string XML_OUT_LEVEL = "OutputLevel";

        public override void write(XmlTextWriter writer)
        {
			base.write(writer);
			// Write wave
			int waveId = delegateHolder.readEUIProp(m_EUId, OscillatorPanel.WAVE_PROPERTY_INDEX);
			writer.WriteAttributeString(XML_WAVE_ID, waveId.ToString());
			// Write output
			double level = delegateHolder.readEUDprop(m_EUId, OscillatorPanel.LEVEL_PROPERTY_INDEX);
			writer.WriteAttributeString(XML_OUT_LEVEL, level.ToString());
        }

        public override void read(XmlTextReader reader)
        {
			// Read and write inside model (AudioEngine)
			base.read(reader);
			String str;
			// Read wave
			str = reader.GetAttribute(XML_WAVE_ID);
			int waveId = Int32.Parse(str);
			delegateHolder.writeEUIProp(m_EUId, OscillatorPanel.WAVE_PROPERTY_INDEX, waveId);
			// Read level
			str = reader.GetAttribute(XML_OUT_LEVEL);
			CultureInfo cInfo = new CultureInfo("en-US");
			double level = Double.Parse(str, cInfo);
			delegateHolder.writeEUDProp(m_EUId, OscillatorPanel.LEVEL_PROPERTY_INDEX, level);
			// Now update the view to the model
			m_OscillatorPanel.updateToModel();
        }

        public bool hasChanged()
        {
            return false;
        }

    }
}
