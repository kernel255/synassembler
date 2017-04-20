using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using SynthPanels;
using GeneralUtils;
using System.Drawing;
using System.Xml;

namespace BasicEUFact
{
    class MIDISequencePlayerWrapper : AbstractSynthPanel
    {
		MIDISequencePanel m_MIDIPanel;
		internal static List<String> s_MIDIFilenames = new List<string>();
		const String MIDI_SUFFIX = ".mid";

		const String XML_MIDIFILE_NAME = "MIDIFilename";

		public MIDISequencePlayerWrapper(int id, SynthDelegateHolder deleHolder, Facilities facilities)
			: base(new MIDISequencePanel(id, deleHolder, facilities), id, deleHolder, facilities)
        {
			m_MIDIPanel = (MIDISequencePanel)m_UserControl;

		}

		public override System.Windows.Controls.UserControl getUserControlPanel()
		{
			return m_MIDIPanel;
		}


		public override Rectangle getRect()
		{
			return new Rectangle(0, 0, 300, 100);
		}

		override public int GetHorizontalSpan()
		{
			return 2;
		}
		override public int GetVerticalSpan()
		{
			return 1;
		}

		internal static void InitMIDIFileNames(String resourcesFolder)
		{
			DirectoryInfo info = new DirectoryInfo(resourcesFolder);
			if (info.Exists)
			{
				foreach (FileInfo file in info.GetFiles())
				{
					if (file.Name.EndsWith(MIDI_SUFFIX))
						s_MIDIFilenames.Add(file.Name);
				}
			}
		}

		public override void write(XmlTextWriter writer)
		{
			base.write(writer);
			String midiFilename = delegateHolder.readEUProp(m_EUId, MIDISequencePanel.MIDI_NAME_INDEX);
			writer.WriteAttributeString(XML_MIDIFILE_NAME, midiFilename);
		}

		public override void read(XmlTextReader reader)
		{
			base.read(reader);
			String str = reader.GetAttribute(XML_MIDIFILE_NAME);
			delegateHolder.writeEUProp(m_EUId, MIDISequencePanel.MIDI_NAME_INDEX, str);
		}


		public override void readParametersFromEngine()
		{
			m_MIDIPanel.readParametersFromEngine();
		}
	}
	}
