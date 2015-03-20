using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Shapes;
using System.Windows.Controls;
using System.Windows.Media;
using ClayAudioEngine;

namespace EUShelves
{
    public class EUShelf
    {
        private List<EULedge> m_Ledges;
        private double m_LateralWidth = 30;
        private Rectangle m_LeftRect = new Rectangle();
        private Rectangle m_RightRect = new Rectangle();
        private Rectangle m_TopLedge = new Rectangle();
        private IList<ElaborationUnitFactory> m_Factories;
        public EUShelf(double ledgeHeight, double width, double room, double x, double y, IList<ElaborationUnitFactory> factories)
        {
            m_Factories = factories;
            m_Ledges = new List<EULedge>();
            double pos = room + EULedge.Height;
            for (int i = 0; i < m_Factories.Count; i++)
            {
                EULedge ledge = new EULedge(width, room, x + m_LateralWidth, pos, m_Factories.ElementAt(i) );
                pos += room + EULedge.Height;
                m_Ledges.Add(ledge);
            }

            m_LeftRect.Width = m_LateralWidth;
            m_LeftRect.Height = m_Ledges[0].TotalHeight * m_Factories.Count + EULedge.Height;
            m_LeftRect.Fill = Brushes.Green;
            Canvas.SetTop(m_LeftRect, 0);
            Canvas.SetLeft(m_LeftRect, 0);

            m_RightRect.Width = m_LateralWidth;
            m_RightRect.Height = m_Ledges[0].TotalHeight * m_Factories.Count + EULedge.Height;
            m_RightRect.Fill = Brushes.Green;
            Canvas.SetTop(m_RightRect, 0);
            Canvas.SetLeft(m_RightRect, m_LateralWidth + width);

            m_TopLedge.Width = width;
            m_TopLedge.Height = EULedge.Height;
            m_TopLedge.Fill = Brushes.Green;
            Canvas.SetTop(m_TopLedge, 0);
            Canvas.SetLeft(m_TopLedge, m_LateralWidth);
        }

        public void addToPanel(Canvas panel)
        {
            int numLedges = m_Ledges.Count;
            for (int i = 0; i < numLedges; i++)
            {
                m_Ledges.ElementAt(i).addToPanel(panel);
            }
            panel.Children.Add(m_LeftRect);
            panel.Children.Add(m_RightRect);
            panel.Children.Add(m_TopLedge);
        }

        public void removePhysicalEUInstance(int factoryIndex, int euIndex, int physicalEUIndex)
        {
            EULedge ledge = m_Ledges[factoryIndex];
            ledge.removePhysicalEUGlyph(euIndex, physicalEUIndex);
        }
    }
}
