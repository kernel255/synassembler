using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Diagnostics;
using System.Windows.Threading;

namespace BasicAudioControls
{
    /// <summary>
    /// Interaction logic for VuMeter.xaml
    /// </summary>
    public partial class VuMeter : UserControl
    {

        private Rectangle[] leds;

        private Color[] getRedLedColors()
        {
            Color[] colors = new Color[2];
            Color high = new Color();
            high.A = 0xFF;
            high.R = 0xA0;
            high.G = 0x00;
            high.B = 0x00;
            colors[0] = high;
            Color low = new Color();
            low.A = 0xFF;
            low.R = 0xE0;
            low.G = 0x00;
            low.B = 0x00;
            colors[1] = low;

            return colors;
        }

        private Color[] getYellowColors()
        {
            Color[] colors = new Color[2];
            Color high = new Color();
            high.A = 0xFF;
            high.R = 0xA0;
            high.G = 0xA0;
            high.B = 0x00;
            colors[0] = high;
            Color low = new Color();
            low.A = 0xFF;
            low.R = 0xE0;
            low.G = 0xE0;
            low.B = 0x00;
            colors[1] = low;
            return colors;
        }

        private Color[] getGreenColors()
        {
            Color[] colors = new Color[2];
            Color high = new Color();
            high.A = 0xFF;
            high.R = 0x00;
            high.G = 0xA0;
            high.B = 0x00;
            colors[0] = high;
            Color low = new Color();
            low.A = 0xFF;
            low.G = 0xE0;
            colors[1] = low;
            return colors;
        }

        private static readonly double[] OFFSETS = new double[] { 0.0, 0.5, 1.0 };

        GradientStopCollection getRedLedColl()
        {
            return getGradient(getRedLedColors());
        }

        GradientStopCollection getGreenColl()
        {
            return getGradient(getGreenColors());
        }

        GradientStopCollection getYellowColl()
        {
            return getGradient(getYellowColors());
        }

        GradientStopCollection getGradient(Color[] colors)
        {
            GradientStopCollection coll = new GradientStopCollection();
            GradientStop stop1 = new GradientStop(colors[0], OFFSETS[0]);
            GradientStop stop2 = new GradientStop(colors[1], OFFSETS[1]);
            GradientStop stop3 = new GradientStop(colors[0], OFFSETS[2]);
            coll.Add(stop1);
            coll.Add(stop2);
            coll.Add(stop3);
            return coll;
        }





        LinearGradientBrush[] gradients;
        int numLed;

        public VuMeter()
        {
            InitializeComponent();

            leds = new Rectangle[] { Led1, Led2, Led3, Led4, Led5, Led6, Led7, Led8 };
            numLed = leds.Length;
            gradients = new LinearGradientBrush[leds.Length];
            GradientStopCollection coll;
            for (int i = 0; i < leds.Length; i++)
            {
                Rectangle led = leds[i];
                if (i >= 0 && i <= 1)
                    coll = getRedLedColl();
                else if (i > 1 && i <= 3)
                    coll = getYellowColl();
                else
                    coll = getGreenColl();
                LinearGradientBrush lin = new LinearGradientBrush(coll, 0);
                lin.Opacity = 0.0;
                gradients[i] = lin;
                led.Fill = lin;
            }
        }

        DispatcherTimer timer = new DispatcherTimer();

        private Rectangle getEnlightLed(double level)
        {
            int index = getHiIndex(level);

            return leds[index];
        }

        private int getHiIndex(double level)
        {
            double d = level * leds.Length;
            //d += 0.5;
            int index = (int)d;
            if (index == leds.Length)
                return index - 1;

            return index;
        }

        private int NUM_OPACITY_LEVELS = 10;

        private int getLoIndex(double lev, int hiIndex)
        {
            double d = lev * leds.Length;
            int index = (int)d;
            double div = d - index;
            div *= NUM_OPACITY_LEVELS;
            int loIndex = (int)div;
            return loIndex;
        }

        private double getOpacity(int index)
        {
            double opacity = index;
            double numLevel = NUM_OPACITY_LEVELS + 1;
            opacity = 1.0 / numLevel + opacity / numLevel;
            return opacity;
        }

        public double Level
        {
            set
            {
                if (value > 1)
                {
                    Trace.Write("Error: level=" + level);
                    return;
                }

                Rectangle led = getEnlightLed(value);
                int hiIndex = getHiIndex(value);
                int loIndex = getLoIndex(value, hiIndex);
                Trace.Write("Level=" + value + "LO=" + loIndex + "HI=" + hiIndex + "\n");
                for (int i = 0; i < leds.Length; i++)
                {
                    if (i < hiIndex)
                        gradients[numLed - i - 1].Opacity = 1.0;
                    else if (i == hiIndex)
                        gradients[numLed - i - 1].Opacity = getOpacity(loIndex);
                    else
                        gradients[numLed - i - 1].Opacity = getOpacity(0);
                }
            }
            get { return 0.0; }
        }

        double level = 0.0;

        private void Timer_Tick(object sender, EventArgs e)
        {
            Level = level;
            level += 0.05;
            if (level >= 1.0)
                level = 0;
        }

        private void StackPanel_Loaded(object sender, RoutedEventArgs e)
        {
            timer.Tick += Timer_Tick;
            timer.Interval = TimeSpan.FromMilliseconds(50);
            //timer.Start();
        }






    }
}
