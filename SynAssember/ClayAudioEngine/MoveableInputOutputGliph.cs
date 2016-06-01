using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Shapes;
using System.Windows.Controls;
using System.Windows;

namespace ClayAudioEngine
{
	public class MoveableInputOutputGliph
	{

		private List<Shape> m_Shapes;
		private MoveableInputOutputGliph audioGliph = null;
		private MoveableInputOutputGliph midiGliph = null;
		bool m_ShapesInCanvas = false;

		MoveableInputOutputGliph getGlyph(MoveableInputOutputGliph glyph, Port.PortType type)
		{
			if(glyph == null)
			{
				glyph = new MoveableInputOutputGliph();
				Port port = new Port("moveable", -1, type, Port.PortDirection.OUT);
				switch(type)
				{
					case Port.PortType.AUDIO:
						glyph.m_Shapes = InputOutputGlyph.getAudioGlyph(0, 0, port);
						break;
					case Port.PortType.MIDI:
						glyph.m_Shapes = InputOutputGlyph.getMIDIGlyph(0, 0, port);
						break;
					default:
						return null;
				}
				if(m_Canvas!=null)
				{
					if (!m_ShapesInCanvas)
					{
						foreach (Shape shape in glyph.m_Shapes)
						{
							m_Canvas.Children.Add(shape);
						}
						m_ShapesInCanvas = true;
					}
				}
			}
			else
			{
				if(!m_ShapesInCanvas)
				{
					foreach (Shape shape in glyph.m_Shapes)
					{
						m_Canvas.Children.Add(shape);
					}
					m_ShapesInCanvas = true;
				}
			}

			return glyph;
		}

		Port.PortType m_Type;

		public MoveableInputOutputGliph getGlyph(Port.PortType type)
		{
			switch(m_Type)
			{
				case Port.PortType.AUDIO:
					audioGliph = getGlyph(audioGliph, type);
					return audioGliph;
				case Port.PortType.MIDI:
					midiGliph = getGlyph(midiGliph, type);
					return midiGliph;
				default:
					return null;
			}
		}

		
		public void setPortType(Port.PortType type)
		{
			m_Type = type;
		}

		public void MoveTo(System.Windows.Point p)
		{
			Point space = getCenter();
			foreach(Shape shape in m_Shapes)
			{
				Canvas.SetTop(shape, p.Y-space.Y/2);
				Canvas.SetLeft(shape, p.X-space.X/2);
			}
		}

		Canvas m_Canvas = null;
		public void setCanvas(Canvas canvas)
		{
			m_Canvas = canvas;
		}
		Point getCenter()
		{
			if (m_Shapes != null && m_Shapes.Count != 0)
			{
				double maxX = 0, maxY = 0;
				foreach(Shape shape in m_Shapes)
				{
					if (shape.ActualWidth > maxX)
						maxX = shape.ActualWidth;
					if (shape.ActualHeight > maxY)
						maxY = shape.ActualWidth;
				}
				return new Point(maxX, maxY);
			}
			else
				return new Point(0, 0);
		}
		public void Hide()
		{
			if(audioGliph!=null)
			{
				foreach(Shape shape in audioGliph.m_Shapes)
				{
					m_Canvas.Children.Remove(shape);
				}
				m_ShapesInCanvas = false;
			}
			if (midiGliph != null)
			{
				foreach (Shape shape in midiGliph.m_Shapes)
				{
					m_Canvas.Children.Remove(shape);
				}
				m_ShapesInCanvas = false;
			}
		}
	}
}
