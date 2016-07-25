using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Shapes;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows;
using ClayAudioEngine;

namespace EUShelves
{
    public class EUShelf
    {
        private List<EULedge> m_Ledges;
        private double m_LateralWidth = 15;
        private Rectangle m_LeftRect = new Rectangle();
        private Rectangle m_RightRect = new Rectangle();
        private Rectangle m_TopLedge = new Rectangle();
        private IList<ElaborationUnitFactory> m_Factories;

		public EUShelf(double ledgeHeight, double width, double room, double x, double y, int numEmptyLedges)
		{
			m_Ledges = new List<EULedge>();
			double pos = room + EULedge.Height;
			for (int i = 0; i < numEmptyLedges; i++)
			{
				EULedge ledge = new EULedge(width, room, x + m_LateralWidth, pos, GetShelfColor());
				pos += room + EULedge.Height;
				m_Ledges.Add(ledge);
			}

			m_LeftRect.Width = m_LateralWidth;
			m_LeftRect.Height = m_Ledges[0].TotalHeight * numEmptyLedges + EULedge.Height;
			m_LeftRect.Fill = GetShelfColor();
			Canvas.SetTop(m_LeftRect, 0);
			Canvas.SetLeft(m_LeftRect, 0);

			m_RightRect.Width = m_LateralWidth;
			m_RightRect.Height = m_Ledges[0].TotalHeight * numEmptyLedges + EULedge.Height;
			m_RightRect.Fill = GetShelfColor();
			Canvas.SetTop(m_RightRect, 0);
			Canvas.SetLeft(m_RightRect, m_LateralWidth + width);

			m_TopLedge.Width = width;
			m_TopLedge.Height = EULedge.Height;
			m_TopLedge.Fill = GetShelfColor();
			Canvas.SetTop(m_TopLedge, 0);
			Canvas.SetLeft(m_TopLedge, m_LateralWidth);

			size = new Size(width + m_LateralWidth * 2, numEmptyLedges * (room + EULedge.Height) + EULedge.Height);
		}

		public EUShelf(double ledgeHeight, double width, double room, double x, double y, IList<ElaborationUnitFactory> factories)
        {
            m_Factories = factories;
            m_Ledges = new List<EULedge>();
            double pos = room + EULedge.Height;
            for (int i = 0; i < m_Factories.Count; i++)
            {
                EULedge ledge = new EULedge(width, room, x + m_LateralWidth, pos, m_Factories.ElementAt(i), GetShelfColor() );
                pos += room + EULedge.Height;
                m_Ledges.Add(ledge);
            }

            m_LeftRect.Width = m_LateralWidth;
            m_LeftRect.Height = m_Ledges[0].TotalHeight * m_Factories.Count + EULedge.Height;
            m_LeftRect.Fill = GetShelfColor();
			Canvas.SetTop(m_LeftRect, 0);
            Canvas.SetLeft(m_LeftRect, 0);

            m_RightRect.Width = m_LateralWidth;
            m_RightRect.Height = m_Ledges[0].TotalHeight * m_Factories.Count + EULedge.Height;
			m_RightRect.Fill = GetShelfColor();
            Canvas.SetTop(m_RightRect, 0);
            Canvas.SetLeft(m_RightRect, m_LateralWidth + width);

            m_TopLedge.Width = width;
            m_TopLedge.Height = EULedge.Height;
            m_TopLedge.Fill = GetShelfColor();
			Canvas.SetTop(m_TopLedge, 0);
            Canvas.SetLeft(m_TopLedge, m_LateralWidth);

			size = new Size(width + m_LateralWidth * 2, m_Factories.Count * (room + EULedge.Height) + EULedge.Height);
        }

		Brush GetShelfColor()
		{
			Color color = new Color();
			color.R = 46;
			color.G = 90;
			color.B = 90;
			color.A = 255;
			Brush result = new SolidColorBrush(color);
			return result;
		}


		Size size;

		internal Size GetSize()
		{
			return size;
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

		public void DrawLedgePlaceHolders()
		{
			foreach(EULedge ledge in m_Ledges)
			{
				ledge.DrawEUPlaceholders();
			}
		}

		internal Rectangle GetNearestRect(Point p)
		{
			List<Rectangle> rects = new List<Rectangle>();
			foreach (EULedge ledge in m_Ledges)
			{
				Rectangle rect = ledge.GetNearesRect(p);
				if (rect != null)
					rects.Add(rect);
			}
			if (rects.Count == 0)
				return null;
			else
			{
				Rectangle nearest = rects[0];
				for(int i=1;i<rects.Count;i++)
				foreach(Rectangle rect in rects)
				{
					nearest = EULedge.GetNearestRect(nearest, rect, p);
				}
				return nearest;
			}
		}


	}
}
