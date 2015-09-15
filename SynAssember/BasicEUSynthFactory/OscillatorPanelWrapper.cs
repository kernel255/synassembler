﻿using System;
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
		static string XML_AT = "AttackTime";
		static string XML_DT = "DecayTime";
		static string XML_RT = "ReleaseTime";
		static string XML_AL = "AttackLevel";
		static string XML_SL = "SustainLevel";
		static string XML_LFOAMP_ENA = "LFOAmpEnable";
		static string XML_LFOAMP_AMP = "LFOAmp_Amp";
		static string XML_LFOAMP_FREQ = "LFOAmp_Freq";
		static string XML_LFOFREQ_ENA = "LFOFreqEnable";
		static string XML_LFOFREQ_AMP = "LFOFreq_Amp";
		static string XML_LFOFREQ_FREQ = "LFOFreq_Freq";

        public override void write(XmlTextWriter writer)
        {
			base.write(writer);
			// Write wave
			int waveId = delegateHolder.readEUIProp(m_EUId, OscillatorPanel.WAVE_PROPERTY_INDEX);
			writer.WriteAttributeString(XML_WAVE_ID, waveId.ToString());
			// Write output
			double level = delegateHolder.readEUDprop(m_EUId, OscillatorPanel.LEVEL_PROPERTY_INDEX);
			CultureInfo cInfo = new CultureInfo("en-US");
			string strLevel = level.ToString(cInfo);
			writer.WriteAttributeString(XML_OUT_LEVEL, strLevel);
			// Write ADSR
			double d;
			String str;
			d = delegateHolder.readEUDprop(m_EUId, OscillatorPanel.AT_PROPERTY_INDEX);
			str = level.ToString(cInfo);
			writer.WriteAttributeString(XML_AT, str);
			d = delegateHolder.readEUDprop(m_EUId, OscillatorPanel.DT_PROPERTY_INDEX);
			str = d.ToString(cInfo);
			writer.WriteAttributeString(XML_DT, str);
			d = delegateHolder.readEUDprop(m_EUId, OscillatorPanel.RT_PROPERTY_INDEX);
			str = d.ToString(cInfo);
			writer.WriteAttributeString(XML_RT, str);
			d = delegateHolder.readEUDprop(m_EUId, OscillatorPanel.AL_PROPERTY_INDEX);
			str = d.ToString(cInfo);
			writer.WriteAttributeString(XML_AL, str);
			d = delegateHolder.readEUDprop(m_EUId, OscillatorPanel.SL_PROPERTY_INDEX);
			str = d.ToString(cInfo);
			writer.WriteAttributeString(XML_SL, str);
			// Write LFOs
			bool enaLFOAmp = delegateHolder.readEUBProp(m_EUId, OscillatorPanel.AMP_LFO_ENABLE_INDEX);
			writer.WriteAttributeString(XML_LFOAMP_ENA, enaLFOAmp.ToString());
			double lfoAmpAmp = delegateHolder.readEUDprop(m_EUId, OscillatorPanel.AMP_LFO_LEVEL_INDEX);
			writer.WriteAttributeString(XML_LFOAMP_AMP, lfoAmpAmp.ToString());
			double lfoAmpFreq = delegateHolder.readEUDprop(m_EUId, OscillatorPanel.AMP_LFO_FREQ_INDEX);
			writer.WriteAttributeString(XML_LFOAMP_FREQ, lfoAmpFreq.ToString());
			bool enaLFOFreq = delegateHolder.readEUBProp(m_EUId, OscillatorPanel.FREQ_LFO_ENABLE_INDEX);
			writer.WriteAttributeString(XML_LFOFREQ_ENA, enaLFOFreq.ToString());
			double lfoFreqAmp = delegateHolder.readEUDprop(m_EUId, OscillatorPanel.FREQ_LFO_LEVEL_INDEX);
			writer.WriteAttributeString(XML_LFOAMP_AMP, lfoFreqAmp.ToString());
			double lfoFreqFreq = delegateHolder.readEUDprop(m_EUId, OscillatorPanel.FREQ_LFO_FREQ_INDEX);
			writer.WriteAttributeString(XML_LFOFREQ_FREQ, lfoFreqFreq.ToString());
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

			CultureInfo cInfo = new CultureInfo("en-US");
			// Read level
			str = reader.GetAttribute(XML_OUT_LEVEL);
			double level = Double.Parse(str, cInfo);
			delegateHolder.writeEUDProp(m_EUId, OscillatorPanel.LEVEL_PROPERTY_INDEX, level);
			// Read ADSR
			double d;
			str = reader.GetAttribute(XML_AT);
			d = Double.Parse(str, cInfo);
			delegateHolder.writeEUDProp(m_EUId, OscillatorPanel.AT_PROPERTY_INDEX, d);
			str = reader.GetAttribute(XML_DT);
			d = Double.Parse(str, cInfo);
			delegateHolder.writeEUDProp(m_EUId, OscillatorPanel.DT_PROPERTY_INDEX, d);
			str = reader.GetAttribute(XML_RT);
			d = Double.Parse(str, cInfo);
			delegateHolder.writeEUDProp(m_EUId, OscillatorPanel.RT_PROPERTY_INDEX, d);
			str = reader.GetAttribute(XML_AL);
			d = Double.Parse(str, cInfo);
			delegateHolder.writeEUDProp(m_EUId, OscillatorPanel.AL_PROPERTY_INDEX, d);
			str = reader.GetAttribute(XML_SL);
			d = Double.Parse(str, cInfo);
			delegateHolder.writeEUDProp(m_EUId, OscillatorPanel.SL_PROPERTY_INDEX, d);
			// Write LFOs
			str = reader.GetAttribute(XML_LFOAMP_ENA);
			bool b = Boolean.Parse(str);
			delegateHolder.writeEUBProp(m_EUId, OscillatorPanel.AMP_LFO_ENABLE_INDEX, b);
			str = reader.GetAttribute(XML_LFOAMP_AMP);
			d = Double.Parse(str, cInfo);
			delegateHolder.writeEUDProp(m_EUId, OscillatorPanel.AMP_LFO_LEVEL_INDEX, d);
			str = reader.GetAttribute(XML_LFOAMP_FREQ);
			d = Double.Parse(str, cInfo);
			delegateHolder.writeEUDProp(m_EUId, OscillatorPanel.AMP_LFO_FREQ_INDEX, d);

			str = reader.GetAttribute(XML_LFOFREQ_ENA);
			b = Boolean.Parse(str);
			delegateHolder.writeEUBProp(m_EUId, OscillatorPanel.FREQ_LFO_ENABLE_INDEX, b);
			str = reader.GetAttribute(XML_LFOFREQ_AMP);
			d = Double.Parse(str, cInfo);
			delegateHolder.writeEUDProp(m_EUId, OscillatorPanel.FREQ_LFO_LEVEL_INDEX, d);
			str = reader.GetAttribute(XML_LFOFREQ_FREQ);
			d = Double.Parse(str, cInfo);
			delegateHolder.writeEUDProp(m_EUId, OscillatorPanel.FREQ_LFO_FREQ_INDEX, d);

			// Now update the view to the model
			//m_OscillatorPanel.readParametersFromEngine();
        }

        public bool hasChanged()
        {
            return false;
        }

		public override void readParametersFromEngine()

		{
			m_OscillatorPanel.readParametersFromEngine();
		}

    }
}
