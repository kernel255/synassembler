using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Xml;

namespace ClayAudioEngine
{
    class Utilities
    {
        public const string X_ATTR = "X";
        public const string Y_ATTR = "Y";

        public static void writePoint(XmlTextWriter writer, Point p, int index)
        {
            double x = p.X;
            double y = p.Y;

            string strValX = x.ToString();
            string strValY = y.ToString();

            string strX = X_ATTR + index.ToString();
            string strY = Y_ATTR + index.ToString();

            writer.WriteAttributeString(strX, strValX);
            writer.WriteAttributeString(strY, strValY);
        }

        public static Point readPoint(XmlTextReader reader, int index)
        {
            string strAttrX = X_ATTR + index.ToString();
            string strAttrY = Y_ATTR + index.ToString();

            string strValX = reader.GetAttribute(strAttrX);
            string strValY = reader.GetAttribute(strAttrY);

            double x = Double.Parse(strValX);
            double y = Double.Parse(strValY);

            Point result = new Point(x,y);

            return result;
        }

    }
}
