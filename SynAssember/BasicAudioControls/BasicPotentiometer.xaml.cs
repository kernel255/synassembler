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

namespace BasicAudioControls
{
    /// <summary>
    /// Interaction logic for BasicPotentiometer.xaml
    /// </summary>
    public partial class BasicPotentiometer : UserControl
    {
		public static DependencyProperty LabelProperty;
		public static DependencyProperty MaxLevelProperty;

		static BasicPotentiometer()
		{
			String dummy = "Dummy";
			FrameworkPropertyMetadata metadata = new FrameworkPropertyMetadata(dummy, FrameworkPropertyMetadataOptions.AffectsMeasure, new PropertyChangedCallback(OnLabelChanged));
			LabelProperty = DependencyProperty.Register("LabelString", typeof(String), typeof(BasicPotentiometer),
				metadata);

			MaxLevelProperty = DependencyProperty.Register("MaxLevel", typeof(Double), typeof(BasicPotentiometer));

		}

		public String LabelString
		{
			set { SetValue(LabelProperty, value); }
			get { return (String)GetValue(LabelProperty); }
		}

		public Double MaxLevel
		{
			set { SetValue(MaxLevelProperty, value); }
			get { return (Double)GetValue(MaxLevelProperty); }
		}

		/*
		public Double CurrentLevel
		{

		}
		*/

		private static void OnLabelChanged(DependencyObject sender, DependencyPropertyChangedEventArgs e)
		{
			BasicPotentiometer me = (BasicPotentiometer)sender;
			String name = (String)e.NewValue;
			me.Label.Content = name;
		}

        public BasicPotentiometer()
        {
            InitializeComponent();
            centerX = -1;
            centerY = -1;
            //centerX = 0 + potentiometerSpace.ActualWidth / 2;
            //centerY = 0 + potentiometerSpace.ActualHeight / 2;
            //Point screenPos = potentiometerSpace.PointToScreen(new Point(0, 0));

            rotate = new RotateTransform(0, 0 + potentiometerSpace.ActualWidth / 2, 0 + potentiometerSpace.ActualHeight / 2);
            potentiometerPointer.RenderTransform = rotate;

        }

        private void Ellipse_MouseLeave(object sender, MouseEventArgs e)
        {
            if (dragging)
            {
                Mouse.Capture(potentiometer);
                Trace.Write("Capturing\n");
            }
        }

        private bool dragging = false;
        RotateTransform rotate;

        private void potentiometer_MouseDown(object sender, MouseButtonEventArgs e)
        {
            dragging = true;
            //rotate.Angle = 90;
            rotate.Angle = getDegreeByPoint(e.GetPosition(potentiometerSpace));
            Trace.Write("Started dragging\n");
        }

        private double getDegreeByPoint(Point p)
        {
            if (centerX == -1 && centerX == -1)
            {
                centerX = potentiometerSpace.ActualWidth / 2;
                centerY = potentiometerSpace.ActualHeight / 2;
                rotate = new RotateTransform(0, centerX, centerY);
                potentiometerPointer.RenderTransform = rotate;
            }
            double px = p.X - centerX;
            double py = p.Y - centerY;
            double angle;
            if (px != 0.0)
            {
                angle = Math.Atan(py / px);
                if (px < 0.0)
                    angle += Math.PI;
            }
            else
            {
                if (py > 0.0)
                    angle = 90.0;
                else if (py == 0.0)
                    angle = lastAngle;
                else
                    angle = -90.0;
            }
            lastAngle = angle;
            angle = 90.0 + angle * 180 / Math.PI;
            return angle;
        }

        double lastAngle, centerX, centerY;

        private void potentiometer_MouseUp(object sender, MouseButtonEventArgs e)
        {
            dragging = false;
            Mouse.Capture(null);
            Trace.Write("Stopped Dragging\n");

        }

        private void potentiometer_MouseMove(object sender, MouseEventArgs e)
        {
            if (dragging)
            {
                Point p = e.GetPosition(potentiometerSpace);
                double angle = getDegreeByPoint(p);
                rotate.Angle = angle;
                //Trace.Write("Angle=" + angle+"\n");
            }
        }

    }
}
