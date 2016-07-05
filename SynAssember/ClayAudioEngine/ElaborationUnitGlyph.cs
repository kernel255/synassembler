using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Shapes;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Resources;

using ClayAudioEngine;
using System.Windows;

namespace ClayAudioEngine
{
    public class ElaborationUnitGlyph : CanavasableGroup
    {
        private Rectangle createMainRect(double x, double y, string name)
        {
            Rectangle mainRect = new Rectangle();
            mainRect.Width = EU_WIDTH;
            mainRect.Height = EU_HEIGHT;
            mainRect.Fill = Brushes.Blue;
            mainRect.RadiusX = 10;
            mainRect.RadiusY = 10;
            mainRect.Name = name;
            Canvas.SetTop(mainRect, y);
            Canvas.SetLeft(mainRect, x);

            return mainRect;
        }

        private TextBlock createLabel(double x, double y, string text)
        {
            TextBlock label = new TextBlock();
            label.Text = text;
            label.FontSize = 10;
            label.Foreground = Brushes.White;
            Canvas.SetTop(label, y + EU_HEIGHT / 2);
            Canvas.SetLeft(label, x + EU_WIDTH / 2);

            return label;
        }

        private TextBlock createPhysInstLabel(double x, double y, string text)
        {
            TextBlock physInstLabel = new TextBlock();
            physInstLabel.Text = text;
            physInstLabel.FontSize = 10;
            physInstLabel.Foreground = Brushes.Gray;
            Canvas.SetTop(physInstLabel, getYLabelByMainAchor(y));
            Canvas.SetLeft(physInstLabel, getXLabelByMainAchor(x));

            return physInstLabel;
        }

		const int NUM_SHADED_RECT = 10;

		void drawShadedRects(double x, double y, ElaborationUnitDescription euDescr)
		{
			for(int i=0;i<NUM_SHADED_RECT;i++)
			{
				Rectangle rect = new Rectangle();
				rect.Width = EU_WIDTH;
				rect.Height = EU_HEIGHT;
				rect.RadiusX = 10;
				rect.RadiusY = 10;
				Canvas.SetTop(rect, y - i * 5);
				Canvas.SetLeft(rect, x);
				Color color = new Color();
				color.R = 0;
				color.G = 60;
				byte b = (byte)(255 - i * 20);
				color.B = 180;
				color.A = b;
				SolidColorBrush brush = new SolidColorBrush(color);
				rect.Fill = brush;
				this.addShape(rect);

			}
		}

        // Standard version for Virtual ElaborationUnits
        public ElaborationUnitGlyph(double x, double y, ElaborationUnitDescription euDescr, ElaborationUnitFactory factory, bool drawShades)
        {
            m_EUDescr = euDescr;
            m_EUFact = factory;

            string name = getCombinedFactoryEUName(factory, euDescr);
            m_MainRect = createMainRect(x, y, name);
			if(drawShades)
				drawShadedRects(x, y, euDescr);
            this.addShape(m_MainRect);

			// Label creation
			m_Label = createLabel(x, y, euDescr.Name);
			this.addShape(m_Label);

			// IO creation
			m_NumInputs = euDescr.getInputPorts().Count;
            m_NumOutputs = euDescr.getOuputPorts().Count;
            inputGlyphs = new InputOutputGlyph[m_NumInputs];
            outputGlyphs = new InputOutputGlyph[m_NumOutputs];
            double alpha = getAlpha(EU_WIDTH, InputOutputGlyph.Diameter, m_NumInputs);
            for (int i = 0; i < m_NumInputs; i++)
            {
                InputOutputGlyph inputGlyph = new InputOutputGlyph(getXIOByMainAchor(x, i, true, alpha),
                    getYIOByMainAnchor(y, i, true, alpha), true, euDescr.getInputPorts().ElementAt(i));
                inputGlyphs[i] = inputGlyph;
                this.addGroup(inputGlyph);
            }

            alpha = getAlpha(EU_HEIGHT, PIN_HEIGHT, m_NumOutputs);
            for (int i = 0; i < m_NumOutputs; i++)
            {
                InputOutputGlyph outputGlyph = new InputOutputGlyph(getXIOByMainAchor(x, i, false, alpha),
                    getYIOByMainAnchor(y,i,false,alpha), false, euDescr.getOuputPorts().ElementAt(i));

                /*
                InputOutputGlyph outputGlyph = new InputOutputGlyph(x + alpha * (i + 1) + PIN_HEIGHT * i,
                    y + EU_HEIGHT - OUTPUT_BOTTOM_DISTANCE - InputOutputGlyph.Diameter, false, euDescr.getOuputPorts().ElementAt(i));
                 */
                outputGlyphs[i] = outputGlyph;
                this.addGroup(outputGlyph);
            }
        }
        // Special version for PhysicalElaborationUnit
        public ElaborationUnitGlyph(double x, double y, ElaborationUnitDescription euDescr, ElaborationUnitFactory factory, String physicalInstanceName, int physicalInstanceIndex, bool drawShades)
            : this(x, y, euDescr, factory, drawShades)
        {
            m_PhysicalInstanceName = physicalInstanceName;
            m_PhysicalInstanceIndex = physicalInstanceIndex;
            // Rename the rectangle to add the instance
            m_MainRect.Name = getCombinedFactoryEUName(factory, euDescr, physicalInstanceIndex.ToString());

            m_PhysInstLabel = createPhysInstLabel(x, y, physicalInstanceName);
            this.addShape(m_PhysInstLabel);
        }

        public double getXIOByMainAchor(double x, int ioIndex, bool isInput, double alpha)
        {
            if (isInput)
                return FIRST_INPUT_SPACE + x + alpha * ioIndex + PIN_HEIGHT * ioIndex;
            else
                return x + alpha * (ioIndex + 1) + PIN_HEIGHT * ioIndex;
        }

        public double getYIOByMainAnchor(double y, int ioIndex, bool isInput, double alpha)
        {
            if (isInput)
                return y + EU_HEIGHT - INPUT_BOTTOM_DISTANCE - InputOutputGlyph.Diameter;
            else
                return y + OUTPUT_BOTTOM_DISTANCE + InputOutputGlyph.Diameter;
        }

        public double getXLabelByMainAchor(double x)
        {
            return x;
        }

        public double getYLabelByMainAchor(double y)
        {
            return y + EU_HEIGHT / 2 - 20;
        }

        /**
         * @param x New position x
         * @param y New position y
         */
        public void move(double x, double y)
        {
            moveX(x);
            moveY(y);
        }

        public void moveX(double x)
        {
            Canvas.SetLeft(m_MainRect, x);
            Canvas.SetLeft(m_PhysInstLabel, getXLabelByMainAchor(x));
        }

        public void moveY(double y)
        {
            Canvas.SetTop(m_MainRect, y);
            Canvas.SetLeft(m_PhysInstLabel, getYLabelByMainAchor(y));
        }

        public void moveXBy(double step)
        {
            FrameworkElement[] elems = new FrameworkElement[] { m_MainRect, m_PhysInstLabel };
            foreach (FrameworkElement elem in elems)
            {
                moveShapeByX(elem, step);
            }
        }

        public void moveYBy(double step)
        {
            FrameworkElement[] elems = new FrameworkElement[] { m_MainRect, m_PhysInstLabel };
            foreach(FrameworkElement elem in elems)
            {
                moveShapeByY(elem, step);
            }
        }

        public void moveShapeByX(FrameworkElement shape, double x)
        {
            double curr_x = Canvas.GetLeft(shape);
            curr_x += x;
            Canvas.SetLeft(shape, curr_x);
        }

        public void moveShapeByY(FrameworkElement shape, double y)
        {
            double curr_y = Canvas.GetTop(shape);
            curr_y += y;
            Canvas.SetTop(shape, curr_y);
        }

        private Boolean m_Available = true;

        public void setUnavailable()
        {
            m_MainRect.Fill = Brushes.Gray;
            m_Available = false;
        }

        public bool isAvailable()
        {
            return m_Available;
        }
        
        private double getAlpha(double rectWidth, double objectSize, int numPin)
        {
            if (numPin <= 0)
                return 0.0;
            return (rectWidth - numPin * objectSize) / (numPin + 1);
        }

        private const int PIN_WIDTH = 5;
        private const int PIN_HEIGHT = 10;

        public static double Width
        {
            get { return EU_WIDTH; }
        }
        private const int EU_WIDTH = 220;
        private const int EU_HEIGHT = 70;

        private const int INPUT_BOTTOM_DISTANCE = 10;
        private const int OUTPUT_BOTTOM_DISTANCE = 10;

        private const int FIRST_INPUT_SPACE = 10;


        public static double Height
        {
            get { return EU_HEIGHT; }
        }

        internal ElaborationUnitDescription m_EUDescr;
        protected ElaborationUnitFactory m_EUFact;

        private static char COMBINED_NAME_SEPARATOR = 'X';

        public static String getCombinedFactoryEUName(ElaborationUnitFactory fact, ElaborationUnitDescription euDescr, String physicalInstanceId = null)
        {

            String res = new String(new char[] { COMBINED_NAME_SEPARATOR });
            res += fact.Id.ToString();
            res += COMBINED_NAME_SEPARATOR;
            res += euDescr.Id.ToString();
            res += COMBINED_NAME_SEPARATOR;
            if (physicalInstanceId != null)
                res += physicalInstanceId;

            return res;
        }

        public static String getFactoryIndexByCombined(String combined)
        {
            combined = combined.Substring(1);
            String[] elems = combined.Split(COMBINED_NAME_SEPARATOR);

            return elems[0];
        }

        public static String getEUIndexByCombined(String combined)
        {
            combined = combined.Substring(1);
            String[] elems = combined.Split(COMBINED_NAME_SEPARATOR);

            return elems[1];
        }

        public static String getPhysicalInstaceByCombined(String combined)
        {
            combined = combined.Substring(1);
            String[] elems = combined.Split(COMBINED_NAME_SEPARATOR);

            return elems[2];
        }

        /* OLD UNFACTORED FUNCTION
        public void addToPanel(Canvas panel)
        {

            panel.Children.Add(mainRect);
            for (int i = 0; i < m_NumInputs; i++)
                inputGlyphs[i].addToPanel(panel);
            for (int i = 0; i < m_NumOutputs; i++)
                outputGlyphs[i].addToPanel(panel);
            panel.Children.Add(m_Label);
            if (m_EUDescr.Physical)
            {
                if(m_PhysInstLabel!=null)
                    panel.Children.Add(m_PhysInstLabel);
            }
        }
        */

        public void removeFromPanel(Canvas panel)
        {
            panel.Children.Remove(m_MainRect);
            for (int i = 0; i < m_NumInputs; i++)
                inputGlyphs[i].removeFromPanel(panel);
            for (int i = 0; i < m_NumOutputs; i++)
                outputGlyphs[i].removeFromPanel(panel);
            panel.Children.Remove(m_Label);
            if (m_EUDescr.Physical)
            {
                if (m_PhysInstLabel != null)
                    panel.Children.Remove(m_PhysInstLabel);
            }
        }

        private Rectangle m_MainRect;
        protected InputOutputGlyph[] inputGlyphs;
        protected InputOutputGlyph[] outputGlyphs;

        protected int m_NumInputs;
        protected int m_NumOutputs;

        public String m_PhysicalInstanceName = "";
        protected int m_PhysicalInstanceIndex;

        private TextBlock m_Label;
        private TextBlock m_PhysInstLabel;

    }
}
