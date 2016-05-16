using SynthPanels;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Xml;

namespace ClayAudioEngine
	{
	public class ElaborationUnitGlyphInstance : ElaborationUnitGlyph , IXMLAble
	{

		public int AudioEngineId
		{
			get { return m_AudioEngineId; }
		}

		ISynthPanel synthPanel;
		public ISynthPanel m_SynthPanel
		{
			get { return synthPanel; }
			set { synthPanel = value; }
		}

		private void init(double x, double y, int audioEngineId)
		{
			m_x = x;
			m_y = y;
			m_AudioEngineId = audioEngineId;
			//Assign names to belonging IOs
			foreach (InputOutputGlyph inGlyph in base.inputGlyphs)
			{
				inGlyph.setCombinedName(audioEngineId);
			}
			foreach (InputOutputGlyph outGlyph in base.outputGlyphs)
			{
				outGlyph.setCombinedName(audioEngineId);
			}
		}

		public void setInputConnected(int inIndex)
		{
			if(inIndex>=base.inputGlyphs.Count<InputOutputGlyph>())
			{
				throw new Exception("Input index bigger than available EU entries");
			}
			else
			{
				base.inputGlyphs[inIndex].setInputConnected();
			}
		}

		public Point GetInputCenter(int index)
		{
			if (index >= base.inputGlyphs.Count<InputOutputGlyph>())
			{
				throw new Exception("Input index bigger than available EU entries");
			}
			else
			{
				return base.inputGlyphs[index].GetCenter();
			}
		}

		public ElaborationUnitGlyphInstance(double x, double y, ElaborationUnitDescription euDescr, ElaborationUnitFactory factory, int audioEngineId) : base(x,y,euDescr,factory)
		{
			init(x, y, audioEngineId);
		}

		public ElaborationUnitGlyphInstance(double x, double y, ElaborationUnitDescription euDescr, ElaborationUnitFactory factory, int audioEngineId, string physInstName, int physInstId)
			: base(x, y, euDescr, factory, physInstName, physInstId)
		{
			init(x, y, audioEngineId);
		}

		public void write(XmlTextWriter writer)
		{
			writer.WriteStartElement(XML_ELEMENT_NAME);
			writer.WriteAttributeString(XML_EUNAME_ATTR, m_EUDescr.Name);
			writer.WriteAttributeString(XML_FACTNAME_ATTR, m_EUFact.getName());
			writer.WriteAttributeString(XML_PHYSINST_ATTR, m_PhysicalInstanceIndex.ToString());
			writer.WriteAttributeString("X", m_x.ToString());
			writer.WriteAttributeString("Y", m_y.ToString());
			writer.WriteAttributeString(XML_AUDIOENGINEID_ATTR, this.m_AudioEngineId.ToString());

			if (m_SynthPanel != null)
			{
				SynthPanelManager.getDefault().WritePanel(writer, m_SynthPanel);
			}
            
			writer.WriteEndElement();

			m_Changed = false;
		}

		public void read(XmlTextReader reader)
		{
            
		}

		bool IXMLAble.hasChanged()
		{
			return m_Changed;
		}

		public static string getElementName()
		{
			return XML_ELEMENT_NAME;
		}

		// The id assigned by the AudioEngine
		private int m_AudioEngineId;
		private double m_x;
		private double m_y;
        

		public static string XML_EUNAME_ATTR = "Name";
		public static string XML_FACTNAME_ATTR = "Factory";
		public static string XML_PHYSINST_ATTR = "PhysicalInstanceId";
		public static string XML_AUDIOENGINEID_ATTR = "Id";

		private bool m_Changed = false;

		public const String XML_ELEMENT_NAME = "ElaborationUnit";

	}
	}
