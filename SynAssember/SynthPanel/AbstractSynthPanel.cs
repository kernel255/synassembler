﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Controls;
using System.Drawing;
using System.Xml;
using GeneralUtils;

namespace SynthPanels
{
    public abstract class AbstractSynthPanel : ISynthPanel
    {
		protected SynthDelegateHolder delegateHolder;
        protected int m_EUId;
		protected Facilities facilities;

		public AbstractSynthPanel(UserControl userControl, int id, SynthDelegateHolder deleHolder, Facilities facilities)
        {
            m_UserControl = userControl;
            m_EUId = id;
			delegateHolder = deleHolder;
			this.facilities = facilities;
        }

		public virtual Rectangle getRect()
		{
            
			return new Rectangle(0, 0, (int)m_UserControl.ActualWidth, (int)m_UserControl.ActualHeight);
		}

		public virtual UserControl getUserControlPanel()
		{
			return m_UserControl;
		}

		protected UserControl m_UserControl = null;

		public virtual void write(XmlTextWriter writer)
		{
			// Write Panel position
			x = (int)Canvas.GetLeft(m_UserControl);
			y = (int)Canvas.GetTop(m_UserControl);
			writer.WriteAttributeString(XML_PANEL_X, x.ToString());
			writer.WriteAttributeString(XML_PANEL_Y, y.ToString());
		}

        protected static string XML_PANEL_X = "PanelX";
        protected static string XML_PANEL_Y = "PanelY";

        int x;
        public int GetX()
        {
            return x;
        }
        int y;
        public int GetY()
        {
            return y;
        }
        public virtual void read(XmlTextReader reader)
        {
            string str;
            str = reader.GetAttribute(XML_PANEL_X);
            x = Int32.Parse(str);
            str = reader.GetAttribute(XML_PANEL_Y);
            y = Int32.Parse(str);
        }

        public virtual bool hasChanged()
        {
            return false;
        }

		public virtual void readParametersFromEngine()
		{
			// By default does nothing
		}

		int GetHorizontalSpan()
		{
			return 1;
		}
		int GetVerticalSpan()
		{
			return 1;
		}
	}
}
