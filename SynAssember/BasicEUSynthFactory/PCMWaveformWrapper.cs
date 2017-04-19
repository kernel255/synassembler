using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SynthPanels;
using System.Drawing;
using GeneralUtils;
using System.IO;
using System.Xml;
using System.Globalization;

namespace BasicEUFact
{
	class PCMWaveformWrapper : AbstractSynthPanel
	{
		internal static List<String> s_WaveNames = new List<string>();
		const String WAV_SUFFIX = ".wav";

		PCMWaveformPanel m_WaveformPanel;

		public PCMWaveformWrapper(int id, SynthDelegateHolder deleHolder, Facilities facilities)
			: base(new PCMWaveformPanel(id, deleHolder, facilities), id, deleHolder, facilities)
		{
			m_WaveformPanel = (PCMWaveformPanel)m_UserControl;
		}

		internal static void InitWaveNames(String resourcesFolder)
		{
			DirectoryInfo info = new DirectoryInfo(resourcesFolder);
			if (info.Exists)
			{
				foreach (FileInfo file in info.GetFiles())
				{
					if (file.Name.EndsWith(WAV_SUFFIX))
						s_WaveNames.Add(file.Name);
				}
			}
		}

		public override System.Windows.Controls.UserControl getUserControlPanel()
		{
			return m_WaveformPanel;
		}

		public override Rectangle getRect()
		{
			return new Rectangle(0, 0, 300, 200);
		}

		override public int GetHorizontalSpan()
		{
			return 2;
		}
		override public int GetVerticalSpan()
		{
			return 2;
		}

		const String XML_WAVEFILE_NAME = "WaveFilename";
		const String XML_OUTPUT_LEVEL = "OutputLevel";
		const String XML_PITCH = "Pitch";

		public override void write(XmlTextWriter writer)
		{
			base.write(writer);
			CultureInfo cInfo = new System.Globalization.CultureInfo("en-US");
			// Output Level [0]
			double dOutLvl = delegateHolder.readEUDprop(m_EUId, PCMWaveformPanel.OUTPUT_LEVEL_INDEX);
			writer.WriteAttributeString(XML_OUTPUT_LEVEL, dOutLvl.ToString(cInfo));
			// Waveform Name [1]
			String waveName = delegateHolder.readEUProp(m_EUId, PCMWaveformPanel.WAVE_NAME_INDEX);
			writer.WriteAttributeString(XML_WAVEFILE_NAME, waveName);
			// Pitch [2]
			int pitch = delegateHolder.readEUIProp(m_EUId, PCMWaveformPanel.PITCH_INDEX);
			writer.WriteAttributeString(XML_PITCH, pitch.ToString());
		}

		public override void read(XmlTextReader reader)
		{
			String str;
			// Read and write inside model (AudioEngine)
			base.read(reader);
			// Output Level [0]
			str = reader.GetAttribute(XML_OUTPUT_LEVEL);
			CultureInfo cInfo = new System.Globalization.CultureInfo("en-US");
			double lvl = Double.Parse(str, cInfo);
			delegateHolder.writeEUDProp(m_EUId, PCMWaveformPanel.OUTPUT_LEVEL_INDEX, lvl);
			// Waveform Name [1]
			String waveName = reader.GetAttribute(XML_WAVEFILE_NAME);
			delegateHolder.writeEUProp(m_EUId, PCMWaveformPanel.WAVE_NAME_INDEX, waveName);
			// Pitch [2]
			str = reader.GetAttribute(XML_PITCH);
			int pitch = Int32.Parse(str);
			delegateHolder.writeEUIProp(m_EUId, PCMWaveformPanel.PITCH_INDEX, pitch);
		}

		public override void readParametersFromEngine()
		{
			m_WaveformPanel.readParametersFromEngine();
		}
	}
}
