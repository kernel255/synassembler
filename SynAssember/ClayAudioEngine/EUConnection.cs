using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows;
using System.Xml;

namespace ClayAudioEngine
{
    public class EUConnection : IXMLAble
    {
        private Point startPoint;
        private Point endPoint;
        private Path connectionPath;
        private Panel panel;

        private ElaborationUnitGlyphInstance euSource;
        private ElaborationUnitGlyphInstance euDestination;
        internal int euSourceId;
        internal int euDestinationId;
        internal int sourcePortIndex;
        internal int destinationPortIndex;

        public EUConnection(EUConnection src) : this(src.StartPoint, src.endPoint, src.panel)
        {
            euSource = src.euSource;
            euSourceId = src.euSourceId;
            sourcePortIndex = src.sourcePortIndex;
            m_PortType = src.m_PortType;
        }

        public void setSourceEU(ElaborationUnitGlyphInstance euInst, int portIndex)
        {
            euSource = euInst;
            euSourceId = euInst.AudioEngineId;
            sourcePortIndex = portIndex;
            PortType = euInst.m_EUDescr.getNthOutputPort(portIndex).Type;
        }

        public void setDestinationEU(ElaborationUnitGlyphInstance euInst, int portIndex)
        {
            euDestination = euInst;
            euDestinationId = euInst.AudioEngineId;
            destinationPortIndex = portIndex;
        }
        
        public EUConnection(Point _startPoint, Point _endPoint, Panel _panel)
        {
            panel = _panel;
            PathGeometry pathGeometry = new PathGeometry();
            pathGeometry.FillRule = FillRule.Nonzero;
            Point[] cable = getCablePoints(_startPoint, _endPoint);
            startPoint = _startPoint;
            endPoint = _endPoint;
            PathFigure pathFigure = new PathFigure();
            pathFigure.StartPoint = cable[0];
            pathFigure.IsClosed = false;
            pathGeometry.Figures.Add(pathFigure);
            BezierSegment bezierSegment1 = new BezierSegment();
            bezierSegment1.Point1 = cable[0];
            bezierSegment1.Point2 = cable[1];
            bezierSegment1.Point3 = cable[2];
            pathFigure.Segments.Add(bezierSegment1);
            connectionPath = new Path();
            connectionPath.StrokeLineJoin = PenLineJoin.Round;
            connectionPath.Stroke = new SolidColorBrush(Color.FromRgb(0, 0, 0));
            connectionPath.StrokeThickness = 4;
            connectionPath.Data = pathGeometry;
        }

        private void update()
        {
            if (connectionPath.Data is PathGeometry)
            {
                PathGeometry pathGeometry = (PathGeometry)connectionPath.Data;

                pathGeometry.Figures.RemoveAt(0);
                Point[] cable = getCablePoints();    
                BezierSegment bezierSegment1 = new BezierSegment();
                bezierSegment1.Point1 = cable[0];
                bezierSegment1.Point2 = cable[1];
                bezierSegment1.Point3 = cable[2];
                PathFigure pathFigure = new PathFigure();
                pathFigure.Segments.Add(bezierSegment1);
                pathFigure.IsClosed = false;
                pathFigure.StartPoint = cable[0];
                pathGeometry.Figures.Add(pathFigure);
                //connectionPath = new Path();
                connectionPath.Data = pathGeometry;
            }
        }

        public void place()
        {
            Canvas.SetLeft(connectionPath, 0);
            Canvas.SetTop(connectionPath, 0);
        }

        public void setTransparent(bool enable)
        {
            if (enable)
            {
                connectionPath.Stroke = Brushes.Transparent;
                connectionPath.StrokeThickness = 0;
                panel.Children.Remove(connectionPath);
            }
            else
            {
                setConnectionColor();
                connectionPath.StrokeThickness = 4;
                panel.Children.Add(connectionPath);
            }

        }

        private Color[] getMIDIColors()
        {
            Color colorA = Color.FromArgb(255, 0, 0, 100);
            Color colorB = Color.FromArgb(255, 0, 80, 80);
            return new Color[] { colorA, colorB };
        }

        private Color[] getAudioColors()
        {
            Color colorA = Color.FromArgb(255, 100, 30, 30);
            Color colorB = Color.FromArgb(255, 200, 40, 0);
            return new Color[] { colorA, colorB };
        }

        private LinearGradientBrush getConnectionGradient(Color colorA, Color colorB)
        {
            LinearGradientBrush myBrush = new LinearGradientBrush();
            GradientStopCollection coll = new GradientStopCollection { };
            coll.Add(new GradientStop(colorA, 0));
            coll.Add(new GradientStop(colorB, 0.5));
            coll.Add(new GradientStop(colorA, 1));
            myBrush.StartPoint = new Point(0, 0);
            myBrush.EndPoint = new Point(1, 0);
            myBrush.GradientStops = coll;
            return myBrush;
        }

        private Brush getMIDIBrush()
        {
            Color[] colors = getMIDIColors();
            return getConnectionGradient(colors[0], colors[1]);
        }

        private Brush getAudioBrush()
        {
            Color[] colors = getAudioColors();
            return getConnectionGradient(colors[0], colors[1]);
        }


        private void setConnectionColor()
        {
            if (PortType == Port.PortType.AUDIO)
                connectionPath.Stroke = getAudioBrush();
            else if (PortType == Port.PortType.MIDI)
                connectionPath.Stroke = getMIDIBrush();
            else
                connectionPath.Stroke = Brushes.Black;
        }

        private Port.PortType m_PortType;

        public Port.PortType PortType
        {
            get { return m_PortType; }
            set
            {
                m_PortType = value;
                setConnectionColor();
            }
        }

        public Point StartPoint
        {
            get { return startPoint; }
            set { 
                startPoint.X = value.X; 
                startPoint.Y = value.Y;
                update();
            }
        }

        public Point EndPoint
        {
            get { return endPoint; }
            set { 
                endPoint.X = value.X; 
                endPoint.Y = value.Y;
                update();
            }
        }

        private Point[] getCablePoints(Point start, Point end)
        {
            Point[] result = new Point[3];

            result[0] = start;
            result[2] = end;
            double x = start.X + (end.X - start.X) / 2;
            double gravityCoeff = 1.4;
            double y = (start.Y > end.Y) ? start.Y * gravityCoeff : end.Y * gravityCoeff;
            result[1] = new Point(x, y);

            return result;
        }

        private Point[] getCablePoints()
        {
            Point[] result = new Point[3];

            result[0] = startPoint;
            result[2] = endPoint;
            double x = startPoint.X + (endPoint.X - startPoint.X) / 2;
            double gravityCoeff = 1.4;
            double y = (startPoint.Y > endPoint.Y) ? startPoint.Y * gravityCoeff : endPoint.Y * gravityCoeff;
            result[1] = new Point(x, y);

            return result;

        }

        private const string SOURCEID_ATTR = "SourceId";
        private const string DESTINATIONID_ATTR = "DestinationId";
        private const string SOURCEPORTINDEX_ATTR = "SourcePortIndex";
        private const string DESTINATIONINDEX_ATTR = "DestinationPortIndex";

        public void write(System.Xml.XmlTextWriter writer)
        {
            writer.WriteStartElement(XML_ELEMENT_NAME);

            Utilities.writePoint(writer, StartPoint, 0);
            //writer.WriteAttributeString("SX", StartPoint.X.ToString());
            //writer.WriteAttributeString("SY", StartPoint.Y.ToString());
            Utilities.writePoint(writer, EndPoint, 1);
            //writer.WriteAttributeString("EX", EndPoint.X.ToString());
            //writer.WriteAttributeString("EY", EndPoint.Y.ToString());
            writer.WriteAttributeString(SOURCEID_ATTR, euSource.AudioEngineId.ToString());
            writer.WriteAttributeString(DESTINATIONID_ATTR, euDestination.AudioEngineId.ToString());
            writer.WriteAttributeString(SOURCEPORTINDEX_ATTR, sourcePortIndex.ToString());
            writer.WriteAttributeString(DESTINATIONINDEX_ATTR, destinationPortIndex.ToString());

            writer.WriteEndElement();
        }

        public void read(System.Xml.XmlTextReader reader)
        {
            string strSourceId = reader.GetAttribute(SOURCEID_ATTR);
            string strDestinationId = reader.GetAttribute(DESTINATIONID_ATTR);
            string strSourcePortIndex = reader.GetAttribute(SOURCEPORTINDEX_ATTR);
            string strDestinationPortIndex = reader.GetAttribute(DESTINATIONINDEX_ATTR);

            euSourceId = int.Parse(strSourceId);
            euDestinationId = int.Parse(strDestinationId);
            sourcePortIndex = int.Parse(strSourcePortIndex);
            destinationPortIndex = int.Parse(strDestinationPortIndex);
        }

        public static EUConnection getConnectionFromXML(XmlTextReader reader, Panel panel)
        {
            Point start = Utilities.readPoint(reader, 0);
            Point end = Utilities.readPoint(reader, 1);

            return new EUConnection(start, end, panel);
        }

        bool IXMLAble.hasChanged()
        {
            return m_Changed;
        }


        public const String XML_ELEMENT_NAME = "EUConnection";

        private bool m_Changed = false;
    }
}
