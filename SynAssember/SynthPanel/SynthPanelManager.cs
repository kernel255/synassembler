using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;
using System.IO;
using System.Diagnostics;
using System.Windows;
using System.Windows.Controls;
using System.Xml;
using GeneralUtils;

namespace SynthPanels
{
	public class SynthPanelManager
    {

        private static double SPACE_X_BETWEENPANELS = 10;
        private static double SPACE_Y_BETWEENPANELS = 7;

        private static SynthPanelManager m_Instance = new SynthPanelManager();

        private SynthPanelManager()
        {
            m_CurrentBusyY = SPACE_Y_BETWEENPANELS;
        }

        public static SynthPanelManager getDefault()
        {
            return m_Instance;
        }

        private List<ISynthPanelFactory> m_euFactoryList;

        private List<String> getFolderFactories(String folderName)
        {
            List<String> result = new List<string>();

            return result;
        }

        private String getFullFactoryName(String name)
        {
            name = name.Replace(" ", string.Empty);
            String result = name + "SynthFactory.dll";

            return result;
        }

		/**
			* Read a list of name of factories and try to load them 
			* @return A list of SynthPanelFactories corresponding to the requested factories
			*/

		private Dictionary<String, ISynthPanelFactory> m_FactoryDict = new Dictionary<string, ISynthPanelFactory>();

		private WriteEUDoubleProperty writeEUDProp;

		private DelegateFactoryInterface deleFactory;

		private Facilities _facilities;
		public Facilities facilities
		{
			set
			{
				_facilities = value;
			}
			get
			{
				return _facilities;
			}
		}

		SynthDelegateHolder deleHolder;
		public IList<ISynthPanelFactory> initSynthPanelsFactories(StringBuilder folderName, IList<String> requestedFactories, SynthDelegateHolder synDeleHolder)
		{
			Assembly synPanelAssembly;
			List<ISynthPanelFactory> factories = new List<ISynthPanelFactory>();
			m_FactoryDict.Clear();

			deleHolder = synDeleHolder;

            foreach (String requestedFactory in requestedFactories)
            {
                try
                {
                    String currentFolder = Directory.GetCurrentDirectory();
                    String factoryName = getFullFactoryName(requestedFactory);
                    String absolutePath = currentFolder + @"\" + factoryName;
                    synPanelAssembly = Assembly.LoadFrom(absolutePath);
                    if (synPanelAssembly != null)
                    {
                        Type[] synTypes = synPanelAssembly.GetTypes();
                        List<Type> synthFactories = new List<Type>();
                        foreach (Type type in synTypes)
                        {
                            if (type.GetInterface("SynthPanels.ISynthPanelFactory") != null)
                                synthFactories.Add(type);
                        }

                        foreach (Type synFactType in synthFactories)
                        {
                            ISynthPanelFactory factory = (ISynthPanelFactory)synPanelAssembly.CreateInstance(synFactType.FullName);
                            if (factory != null)
                            {
                                //factory.init(writedProp);
								factory.init(synDeleHolder, facilities);
                                m_FactoryDict.Add(requestedFactory, factory);
                            }
                        }
                    }
                    else
                        Trace.Write("Something went wrong while loading assembly: " + absolutePath + "\n");

                }
                catch(Exception)
                {
                    Trace.WriteLine("Unable to load factory: " + requestedFactory);
                }
            }
            
            m_euFactoryList = factories;

            return factories;
        }


        public ISynthPanel createSynthPanel(String factoryName, String panelName, int euId)
        {
            if(!m_FactoryDict.ContainsKey(factoryName))
                return null;
            ISynthPanelFactory factory = m_FactoryDict[factoryName];
            ISynthPanel synPanel = factory.createSynthPanel(panelName, euId);
            
            if (synPanel != null)
                return synPanel;
            else
                return null;
        }

        private double m_CurrentBusyX = 0.0;
        private double m_CurrentBusyY;
		private double m_VirtBusyX = 0.0;
		private double m_VirtBusyY = 0.0;

        private double m_MaxPanelWidth = 1500;
        //private double m_MaxPanelHeight = 300;


        private Canvas m_Canvas;

        public void SetCanvas(Canvas canvas)
        {
            m_Canvas = canvas;
        }

        public void AddSynthPanel(ISynthPanel panel, bool automaticPlace)
        {
            UserControl uiElem = panel.getUserControlPanel();
            m_Canvas.Children.Add(uiElem);
            if (automaticPlace)
            {
                double nextPosition = m_CurrentBusyX + panel.getRect().Width;
                if (nextPosition > m_MaxPanelWidth)
                {
                    // Go to next line
                    m_CurrentBusyX = SPACE_X_BETWEENPANELS;
                    m_CurrentBusyY += SPACE_Y_BETWEENPANELS;
                    Canvas.SetLeft(uiElem, m_CurrentBusyX);
                    m_CurrentBusyY += panel.getRect().Height;
                    m_CurrentBusyX += panel.getRect().Width;
                    Canvas.SetTop(uiElem, m_CurrentBusyY);
                }
                else
                {
                    // Continue on the current line
                    m_CurrentBusyX += SPACE_X_BETWEENPANELS;
                    Canvas.SetLeft(uiElem, m_CurrentBusyX);
                    Canvas.SetTop(uiElem, m_CurrentBusyY);
                    m_CurrentBusyX += panel.getRect().Width;
                }
            } 
            else
            {
                Canvas.SetLeft(uiElem, panel.GetX());
                Canvas.SetTop(uiElem, panel.GetY());
				m_VirtBusyX = panel.GetX() + panel.getRect().Width + SPACE_X_BETWEENPANELS;
				m_VirtBusyY = panel.GetY() + panel.getRect().Height + SPACE_Y_BETWEENPANELS;
            }

			m_PanelList.Add(panel);
        }

		public void UpdateBusySpace()
		{
			//m_CurrentBusyX = m_VirtBusyX;
			//m_CurrentBusyY = m_VirtBusyY;
		}

        public void WritePanel(XmlTextWriter writer, ISynthPanel panel)
        {
            writer.WriteStartElement(XML_PANEL);
            panel.write(writer);
            writer.WriteEndElement();
        }

        public ISynthPanel ReadPanel(XmlTextReader reader, string factory, string euName, string id)
        {
			switch(reader.NodeType)
			{
				case XmlNodeType.Element:
					{
						if(reader.Name.Equals(XML_PANEL))
						{
							ISynthPanel panel = createSynthPanel(factory, euName, int.Parse(id));
							
							panel.read(reader);
							AddSynthPanel(panel, false);
							return panel;
						}
						break;
					}
			}
            return null;
        }

		List<ISynthPanel> m_PanelList = new List<ISynthPanel>();

		public void reset()
		{
			m_PanelList.Clear();
			m_VirtBusyX = 0.0;
			m_VirtBusyY = 0.0;
		}

		public Point GetLastBusyPoint()
		{
			return new Point(m_CurrentBusyX, m_CurrentBusyY);
		}

		public void SetLastBusyPoint(Point p)
		{
			m_CurrentBusyX = p.X;
			m_CurrentBusyY = p.Y;
		}

		public void updateSynthPanels()
		{
			foreach(ISynthPanel synthPanel in m_PanelList)
			{
				synthPanel.readParametersFromEngine();
			}
		}

		public void CleanUp()
		{
			foreach(ISynthPanel synth in m_PanelList)
			{
				UserControl uiElem = synth.getUserControlPanel();
				m_Canvas.Children.Remove(uiElem);
			}
			m_PanelList.Clear();
			m_CurrentBusyX = 0.0;
			m_CurrentBusyY = 0.0;
		}

        public const string XML_PANEL = "SynthPanel";

		public double GetWidth()
		{
			return m_MaxPanelWidth;
		}

		public double GetHeight()
		{
			return 1800;
		}

		public void SizeChanged(object sender, SizeChangedEventArgs e)
		{
			try { m_Canvas.Width = SynthPanelManager.getDefault().GetWidth(); } catch {}
			try	{ m_Canvas.Height = SynthPanelManager.getDefault().GetHeight();	} catch {}
		}


	}
}
