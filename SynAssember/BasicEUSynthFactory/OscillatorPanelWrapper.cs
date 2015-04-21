using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SynthPanels;
using System.Windows.Controls;
using System.Drawing;
using System.Xml;

namespace BasicEUSynthFactory
{
    class OscillatorPanelWrapper : AbstractSynthPanel
    {

        OscillatorPanel m_OscillatorPanel = null;

		public OscillatorPanelWrapper(int id, SynthDelegateHolder deleHolder) : base(new OscillatorPanel(id, deleHolder), id)
		{
            m_OscillatorPanel = (OscillatorPanel)m_UserControl;
        }

        public UserControl getUserControlPanel()
        {
            return m_OscillatorPanel;
        }

        public override Rectangle getRect()
        {
            return m_OscillatorPanel.getRectangle();
        }



        public void write(XmlTextWriter writer)
        {
            
        }

        public void read(XmlTextReader reader)
        {

        }

        public bool hasChanged()
        {
            return false;
        }

    }
}
