using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Shapes;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows;
using System.Xml;

using ClayAudioEngine;
using EUShelves;
using SynthPanels;

namespace ClayAudioEngine
{
    // This class holds the list of the Elaboration units dropped inside the graph
    public class AlgorithmGraph
    {

        public AlgorithmGraph(Canvas canvas)
        {
            m_Canvas = canvas;
            m_AudioEngineId = AudioEngineWrapper.createNewAlgorithm();
            m_EUGlyphs = new List<ElaborationUnitGlyphInstance>();
            m_EUConnections = new List<EUConnection>();
        }

        private ElaborationUnitGlyphInstance createElaborationUnitGlyph(double x, double y, ElaborationUnitDescription euDesc,
            ElaborationUnitFactory euFactory, string physInstName, int physicalInstanceId = -1)
        {
            int id;
			if (euDesc.Physical)
			{
				id = AudioEngineWrapper.createNewPhysicalElaborationUnit(euFactory.Id, euDesc.Id, 0, physicalInstanceId);
				if(euDesc.Name.Equals("PC Keyboard"))
				{
					AudioEngineWrapper.keybProcessor.EUId = id;
				}
			}
			else
				id = AudioEngineWrapper.createNewVirtualElaborationUnit(euFactory.Id, euDesc.Id);
            if (id < 0)
            {
                return null;
            }
            ElaborationUnitGlyphInstance euInst;
            if (euDesc.Physical)
                euInst = new ElaborationUnitGlyphInstance(x, y, euDesc, euFactory, id, physInstName, physicalInstanceId);
            else
                euInst = new ElaborationUnitGlyphInstance(x, y, euDesc, euFactory, id);

            return euInst;
        }

        public void addElaborationUnitGlyph(double x, double y, ElaborationUnitDescription euDesc, ElaborationUnitFactory euFactory, int physicalInstanceId = -1)
        {
            ElaborationUnitGlyphInstance euInst;
            String euPhysInstName = "";
            if (physicalInstanceId != -1)
                euPhysInstName = euDesc.getNthInstance(physicalInstanceId).m_Name;
                
            euInst = createElaborationUnitGlyph(x, y, euDesc, euFactory, euPhysInstName, physicalInstanceId);

            if (euInst == null)
            {
                throw new Exception("Unable to create: " + euDesc.Name);
            }
            m_EUGlyphs.Add(euInst);
            euInst.addToCanvas(m_Canvas);
            //euInst.addToPanel(m_Canvas);

			AudioEngineWrapper.addElaborationUnit(algoId, euDesc.Id);

            // Create synth panel
            ISynthPanel synPanel = AudioEngineWrapper.createNewPanel(euFactory.getName(), euDesc.Name, euInst.AudioEngineId);
			
            euInst.m_SynthPanel = synPanel;
            SynthPanelManager.getDefault().AddSynthPanel(synPanel, true);

            m_Changed = true;
        }

        public void addConnection(EUConnection euConn)
        {
            euConn.place();
            euConn.setTransparent(false);
            m_EUConnections.Add(euConn);

            AudioEngineWrapper.connectElaborationUnits(m_AudioEngineId, euConn.euSourceId,euConn.sourcePortIndex,euConn.euDestinationId,euConn.destinationPortIndex);

            m_Changed = true;
        }

		static string BUSYX_ATTR = "CurrentBusyX";
		static string BUSYY_ATTR = "CurrentBusyY";

		static string EUS = "ElaborationUnits";
		static string EUCONNS = "EUConnections";

        public void write(string filename)
        {
            XmlTextWriter writer = new XmlTextWriter(filename, Encoding.UTF8);

            writer.WriteStartDocument();
            writer.WriteStartElement("Algorithm");
			Point p = SynthPanelManager.getDefault().GetLastBusyPoint();
			writer.WriteAttributeString(BUSYX_ATTR, p.X.ToString());
			writer.WriteAttributeString(BUSYY_ATTR, p.Y.ToString());

			writer.WriteWhitespace("\\n\\t");

			writer.WriteStartElement(EUS);
            int numEU = m_EUGlyphs.Count();
            // Write all the EUs
            for (int euIndex = 0; euIndex < numEU; euIndex++)
            {
                ElaborationUnitGlyphInstance euInst = m_EUGlyphs.ElementAt(euIndex);
                euInst.write(writer);
            }
			writer.WriteEndElement();
            // Write all the connections
			writer.WriteStartElement(EUCONNS);
            int numConn = m_EUConnections.Count();
            for (int connIndex = 0; connIndex < numConn; connIndex++)
            {
                EUConnection euConn = m_EUConnections.ElementAt(connIndex);
                euConn.write(writer);
            }
			writer.WriteEndElement();
            writer.WriteEndElement();

            m_Changed = false;

            //m_CurrentAlgorithGraph.write(writer);

            writer.WriteEndDocument();
            writer.Close();

        }

        private Dictionary<string, ElaborationUnitGlyphInstance> aeIdMap = new Dictionary<string,ElaborationUnitGlyphInstance>();

        private void cleanUpMap()
        {
            aeIdMap.Clear();
        }

        private void storeTempAEId(ElaborationUnitGlyphInstance euInst, string id)
        {
            aeIdMap.Add(id, euInst);
        }

        private ElaborationUnitGlyphInstance getUEInstById(string id)
        {
            return aeIdMap[id];
        }

        public void read(System.Xml.XmlTextReader reader)
        {
			reader.WhitespaceHandling = WhitespaceHandling.None;
            bool foundAlgorithm = false;
			string lastEUFact = "", lastEUName = "", lastId = "";
            while (reader.Read())
            {
                switch (reader.NodeType)
                {
					case XmlNodeType.EndElement:
						{
							break;
						}


                    case XmlNodeType.Element:
                        {
                            String element = reader.Name;

                            switch(element)
                            {
                                case XML_ELEMENT_NAME:
                                {
                                    foundAlgorithm = true;
									SynthPanelManager.getDefault().reset();
									string strX = reader.GetAttribute(BUSYX_ATTR);
									string strY = reader.GetAttribute(BUSYY_ATTR);
									double x = Double.Parse(strX);
									double y = Double.Parse(strY);
									Point p = new Point(x, y);
									SynthPanelManager.getDefault().SetLastBusyPoint(p);

                                    break;
                                }
                                case ElaborationUnitGlyphInstance.XML_ELEMENT_NAME:
                                {
                                    if (foundAlgorithm)
                                    {
                                        string euName = reader.GetAttribute(ElaborationUnitGlyphInstance.XML_EUNAME_ATTR);
                                        string euFact = reader.GetAttribute(ElaborationUnitGlyphInstance.XML_FACTNAME_ATTR);
                                        string physInst = reader.GetAttribute(ElaborationUnitGlyphInstance.XML_PHYSINST_ATTR);
                                        string id = reader.GetAttribute(ElaborationUnitGlyphInstance.XML_AUDIOENGINEID_ATTR);
                                        int factIndex = AudioEngineWrapper.getFactoryIdByName(euFact);
                                        int euIndex = AudioEngineWrapper.getEUByName(euName, factIndex);
                                        int physInstIndex = Int32.Parse(physInst);
                                        ElaborationUnitFactory fact = AudioEngineWrapper.getFactoryById(factIndex);
                                        ElaborationUnitDescription euDescr = AudioEngineWrapper.getEUDescriptionById(fact, euIndex);
                                        String posX = reader.GetAttribute("X");
                                        String posY = reader.GetAttribute("Y");

                                        double x = Double.Parse(posX);
                                        double y = Double.Parse(posY);
                                        string physInstName = "";
                                        if (euDescr.Physical)
                                        {
                                            if (physInstIndex != -1)
                                                physInstName = euDescr.getNthInstance(physInstIndex).m_Name;
                                        }
                                        ElaborationUnitGlyphInstance euInst = createElaborationUnitGlyph(x, y, euDescr, fact, physInstName, physInstIndex);
                                        if (euInst == null)
                                        {
                                            throw new Exception("Unable to create Elaboration Unit");
                                        }
                                        euInst.read(reader);
                                        storeTempAEId(euInst, id);
										AudioEngineWrapper.addElaborationUnit(algoId, euInst.AudioEngineId);
                                        m_EUGlyphs.Add(euInst);
										lastEUFact = euFact;
										lastEUName = euName;
										lastId = id;

										//SynthPanelManager.getDefault().ReadPanel(reader,euFact,euName,id);
                                        
										euInst.addToCanvas(m_Canvas);

                                    }

                                    break;
                                }
                                case EUConnection.XML_ELEMENT_NAME:
                                {
                                    if (foundAlgorithm)
                                    {
                                        EUConnection euConn = EUConnection.getConnectionFromXML(reader, m_Canvas);
                                        euConn.read(reader);
										int sourceId = euConn.euSourceId;
										int destId = euConn.euDestinationId;
										ElaborationUnitGlyphInstance euSourceInst = getUEInstById(sourceId.ToString());
										ElaborationUnitGlyphInstance euDestinationInst = getUEInstById(destId.ToString());
										int sourcePortIndex = euConn.sourcePortIndex;
										int destionationPortIndex = euConn.destinationPortIndex;

										euConn.setSourceEU(euSourceInst, sourcePortIndex);
										euConn.setDestinationEU(euDestinationInst, destionationPortIndex);

                                        euConn.setTransparent(false);
										m_EUConnections.Add(euConn);
										int res = AudioEngineWrapper.connectElaborationUnits(m_AudioEngineId, euConn.euSourceId, euConn.sourcePortIndex, euConn.euDestinationId, euConn.destinationPortIndex);
										if (res != 0)
											throw new Exception("Error while connecting EUId:" + euConn.euSourceId + " to EUId:" + euConn.euDestinationId);
                                    }
                                    break;
                                }
                                case SynthPanelManager.XML_PANEL:
                                {
                                    if(foundAlgorithm)
                                    {

										SynthPanelManager.getDefault().ReadPanel(reader, lastEUFact, lastEUName, lastId);

									}
                                    break;
                                }
                            }


                            break;
                        }
                }
            }

			SynthPanelManager.getDefault().UpdateBusySpace();
        }

        public bool hasChanged()
        {
            return m_Changed;
        }

        public ElaborationUnitGlyphInstance getEUInstanceByAudioEngineId(int id)
        {
            foreach(ElaborationUnitGlyphInstance euInst in m_EUGlyphs)
            {
                if (euInst.AudioEngineId == id)
                    return euInst;
            }

            return null;
        }

		public void cleanUp()
		{
			m_Changed = false;
			m_EUConnections.Clear();
			m_EUGlyphs.Clear();
		}

        public const string XML_ELEMENT_NAME = "Algorithm";

        // Id assigned by the AudioEngine library
        private int m_AudioEngineId;

		public int algoId
		{
			get
			{
				return m_AudioEngineId;
			}
		}

        private List<ElaborationUnitGlyphInstance> m_EUGlyphs;
        private List<EUConnection> m_EUConnections;
        private Canvas m_Canvas;
        private bool m_Changed = false;
    }
}
