using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SynthPanels;
using System.Drawing;
using GeneralUtils;
using System.IO;
using System.Xml;

namespace BasicEUSynthFactory
{
	class PCMWaveformWrapper : AbstractSynthPanel
	{
		internal static List<String> s_WaveNames = new List<string>();
		const String WAV_SUFFIX = ".wav";

		PCMWaveformPanel m_WaveformPanel;

		public PCMWaveformWrapper(int id, SynthDelegateHolder deleHolder, Facilities facilities)
			: base(new PCMWaveformPanel(), id, deleHolder, facilities)
		{
			m_WaveformPanel = (PCMWaveformPanel)m_UserControl;
		}

		internal static void InitWaveNames(String resourcesFolder)
		{
			DirectoryInfo info = new DirectoryInfo(resourcesFolder);
			foreach (FileInfo file in info.GetFiles())
			{
				if (file.Name.EndsWith(WAV_SUFFIX))
					s_WaveNames.Add(file.Name);
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
			double dOutLvl = delegateHolder.readEUDprop(m_EUId, PCMWaveformPanel.OUTPUT_LEVEL_INDEX);
			writer.WriteAttributeString(XML_OUTPUT_LEVEL, dOutLvl.ToString());
		}

	}
}
