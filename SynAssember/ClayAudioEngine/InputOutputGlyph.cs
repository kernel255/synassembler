using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Windows.Shapes;
using System.Windows.Controls;
using System.Windows.Media;
using System.Text.RegularExpressions;

using EUShelves;
using BasicAudioControls;
using System.Windows;

namespace ClayAudioEngine
{
    public class InputOutputGlyph : CanavasableGroup, IXMLAble
    {
        private static char CHAR_SEPARATOR_IO = 'X';
        private static char INPUT_SYMBOL = 'I';
        private static char OUTPUT_SYMBOL = 'O';

        /*
         * PORT FORMAT: <I|O><eu_index>X<port_index>X<MIDI|AUDIO>
         */ 

        // Retrieve a string which represent the port characteristics
        public static String getCombinedIOName(bool isInput, String audioEngineId, String portIndex, String portType)
        {
            String result;
            if(isInput)
                result = INPUT_SYMBOL.ToString();
            else
                result = OUTPUT_SYMBOL.ToString();
            result += audioEngineId;
            result += CHAR_SEPARATOR_IO;
            result += portIndex;
            result += CHAR_SEPARATOR_IO;
            result += portType;

            return result;
        }

        public static bool IsShapeNameIO(String name)
        {
            Regex regex = new Regex(@"I|O");
            Match match = regex.Match(name);
            if (match.Success)
                return true;
            else
                return false;
        }

        public void setCombinedName(int id)
        {
            String name = getCombinedIOName(m_IsInput, id.ToString(), m_Port.m_Id.ToString(), m_Port.Type.ToString());
            //m_MainEllipse.Name = name;
            //m_IOShape.Name = name;
			foreach(Shape shape in m_Shapes)
			{
				shape.Name = name;
			}
        }

        public bool isInputByCombined(String str)
        {
            // TODO implement some check in order to compare the two values and if nor throw exception
            if (str.ElementAt(0).Equals(INPUT_SYMBOL))
                return true;
            else
                return false;
                     
        }

        private static String[] getEUPortByCombined(String str)
        {
            String combined = str.Substring(1, str.Length - 1);
            String[] elems = combined.Split(CHAR_SEPARATOR_IO);

            return elems;
        }

        public static int getPortIndexByCombined(String str)
        {
            int result = 0;
            String[] elems = getEUPortByCombined(str);
            String portStr = elems[1];
            result = Convert.ToInt32(portStr);

            return result;
        }

        public static int getEUAudioEngineIdByCombined(String str)
        {
            int result;
            String[] elems = getEUPortByCombined(str);
            String euStr = elems[0];

            result = Convert.ToInt32(euStr);

            return result;

        }

        public static Port.PortType getTypeByCombined(String str)
        {
            String[] elems = str.Split(CHAR_SEPARATOR_IO);

            return Port.getTypeByString(elems[2]);
        }

		public static bool IsOutputByCombibed(String str)
		{
			return str.StartsWith("O");
		}

        public static bool areIOCompatible(String src, String dst)
        {
            if ( (src[0].Equals(OUTPUT_SYMBOL) && dst[0].Equals(INPUT_SYMBOL)) || (src[0].Equals(INPUT_SYMBOL) && dst[0].Equals(OUTPUT_SYMBOL)) )
            {
                Port.PortType srcType = getTypeByCombined(src);
                Port.PortType dstType = getTypeByCombined(dst);
                if (srcType == Port.PortType.UNKNOWN || dstType == Port.PortType.UNKNOWN)
                    return false;
                if (srcType == dstType)
                    return true;
                else
                    return false;
            }
            else
                return false;
        }

        private Port m_Port;

        public Port LinkedPort
        {
            get { return m_Port; }
        }

        public static double Diameter
        {
            get { return DIAMETER; }
        }

        public static double Width
        {
            get { return DIAMETER + TEXT_OFFSET + TEXT_WIDTH; }
        }

		internal static List<Shape> getMIDIGlyph(double x, double y, Port port)
		{
			List<Shape> result = new List<Shape>();
			Brush brush = Brushes.LightGray;
			if (port.Direction.Equals(Port.PortDirection.IN))
				brush = Brushes.Black;
			Polygon hex = DrawUtils.createHexagon(x, y, DIAMETER / 2, brush, Brushes.DarkGray, 3);
			result.Add(hex);
			return result;
		}

		internal static List<Shape> getAudioGlyph(double x, double y, Port port)
		{
			List<Shape> result = new List<Shape>();
			Ellipse ellipse = new Ellipse();
			ellipse.Width = DIAMETER;
			ellipse.Height = DIAMETER;
			ellipse.StrokeThickness = 3;
			if (port.Direction.Equals(Port.PortDirection.IN))
			{
				ellipse.Fill = Brushes.Black;
				ellipse.Stroke = Brushes.Gray;
			}
			if (port.Direction.Equals(Port.PortDirection.OUT))
			{
				ellipse.Fill = Brushes.DarkGray;
				ellipse.Stroke = Brushes.Gray;
			}
			Canvas.SetTop(ellipse, y);
			Canvas.SetLeft(ellipse, x);
			result.Add(ellipse);
			return result;
		}

		internal void setInputConnected()
		{
			foreach(Shape shape in m_Shapes)
			{
				shape.Fill = Brushes.DarkGray;
			}
		}

		private List<Shape> createIO(double x, double y, Port port)
        {
			List<Shape> result = new List<Shape>();
            switch(port.Type)
            {
                case Port.PortType.AUDIO:
					result.AddRange(getAudioGlyph(x, y, port));
					break;
                case Port.PortType.MIDI:
					result.AddRange(getMIDIGlyph(x, y, port));
					break;
                default:
                    return null;
            }
			return result;
        }

        private TextBlock createTextBlock(double x, double y, String name)
        {
            TextBlock label = new TextBlock();
            label = new TextBlock();
            label.FontSize = 10;
            label.Foreground = Brushes.White;
            Canvas.SetTop(label, y);
            Canvas.SetLeft(label, x + DIAMETER + TEXT_OFFSET);
            label.Text = m_Port.Name;
            return label;
        }

        //private Shape m_IOShape;

		private List<Shape> m_Shapes = new List<Shape>();

        public InputOutputGlyph(double x, double y, bool input, Port port)
        {
            m_IsInput = input;
            m_Port = port;
            m_x = x;
            m_y = y;
			//m_MainEllipse = createMainEllipse(x, y, port);

			//m_Label = createTextBlock(x, y, m_Port.Name);
			//this.addShape(m_Label);
			//m_IOShape = createIO(x, y, port);
			List<Shape> shapes = createIO(x, y, port);
			m_Shapes = shapes;
			foreach(Shape shape in shapes)
			{
				this.addShape(shape);
			}

			//m_IOShape.Name = "X";
			//this.addShape(m_IOShape);


		}

		public void removeFromPanel(Canvas panel)
        {
            //panel.Children.Remove(m_IOShape);
			foreach(Shape shape in m_Shapes)
			{
				panel.Children.Remove(shape);
			}


            //panel.Children.Remove(m_MainEllipse);
            //panel.Children.Remove(m_Label);
        }

        public void write(System.Xml.XmlTextWriter writer)
        {
            writer.WriteStartElement("IO");
            //writer.WriteAttributeString("Connection");
            writer.WriteEndElement();
        }

        public void read(System.Xml.XmlTextReader reader)
        {
            throw new NotImplementedException();
        }


        bool IXMLAble.hasChanged()
        {
            return m_Changed;
        }

        public String getElementName()
        {
            return XML_ELEMENT_NAME;
        }

		internal Point GetCenter()
		{
			double maxW = 0.0, maxH = 0.0;
			double centX = 0.0, centY = 0.0;
			Shape biggerShape = null;
			foreach(Shape shape in m_Shapes)
			{
				if (shape.Width > maxW)
				{
					maxW = shape.Width;
					biggerShape = shape;
				}
				if (shape.Height > maxH)
				{
					maxH = shape.Height;
					biggerShape = shape;
				}
			}
			if(biggerShape!=null)
			{
				centX = Canvas.GetLeft(biggerShape);
				centY = Canvas.GetTop(biggerShape);
				centX = centX + maxW / 2;
				centY = centY + maxH / 2;
			}
			return new Point(centX, centY); ;
		}

        private static readonly String XML_ELEMENT_NAME = "IO";

        private bool m_Changed = false;
		private static double DIAMETER = 12;
		private static double INTERNAL_DIAMETER = 10;
        private static double TEXT_OFFSET = 5;
        private static double TEXT_WIDTH = 30;
        private bool m_IsInput = false;
        //Ellipse m_MainEllipse;
        //TextBlock m_Label;
        double m_x;
        double m_y;

    }
}
