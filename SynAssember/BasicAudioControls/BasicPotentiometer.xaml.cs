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
		public static DependencyProperty CurrentLevelProperty;
		public static DependencyProperty CurrentAngleProperty;

		static BasicPotentiometer()
		{
			String dummy = "Dummy";
			FrameworkPropertyMetadata metadata = new FrameworkPropertyMetadata(dummy, FrameworkPropertyMetadataOptions.AffectsMeasure, new PropertyChangedCallback(OnLabelChanged));
			LabelProperty = DependencyProperty.Register("LabelString", typeof(String), typeof(BasicPotentiometer),
				metadata);
			MaxLevelProperty = DependencyProperty.Register("MaxLevel", typeof(Double), typeof(BasicPotentiometer));
			CurrentLevelProperty = DependencyProperty.Register("CurrentLevel", typeof(Double), typeof(BasicPotentiometer));
			CurrentAngleProperty = DependencyProperty.Register("CurrentAngle", typeof(Double), typeof(BasicPotentiometer));
		}

		public delegate void PotentiometerChanged(Object sender, Double value);

		public event PotentiometerChanged PotentiometerChangedEvent;

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

		public Double CurrentLevel
		{
			set { SetValue(CurrentLevelProperty, value); }
			get { return (Double)GetValue(CurrentLevelProperty); }
		}

		public Double CurrentAngle
		{
			set { SetValue(CurrentAngleProperty, value); }
			get { return (Double)GetValue(CurrentAngleProperty); }
		}

		double GetNormalizedLevel(double angle)
		{
			double max = (Double)GetValue(MaxLevelProperty);
			if(max>0.0)
			{
				double level = (angle / MaxAngleInterval) * max;
				return level;
			}
			return 0.0;
		}

		double GetUnormalizedLevel(double level)
		{
			double max = (Double)GetValue(MaxLevelProperty);
			if(max>0.0)
			{
				double angle = level / max * MaxAngleInterval;
				return angle;
			}
			return 0.0;
		}

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
			if (!DebugTextBoxOn)
			{
				DebugTextBox.Visibility = Visibility.Hidden;
			}

        }

        private void Ellipse_MouseLeave(object sender, MouseEventArgs e)
        {
            if (dragging)
            {
                Mouse.Capture(potentiometer);
				//Console.WriteLine("Capturing\n");
            }
        }

        private bool dragging = false;
        RotateTransform rotate;

		private bool IsLeftAngle(double angle)
		{
			return angle >= 180.0;
		}

        private void potentiometer_MouseDown(object sender, MouseButtonEventArgs e)
        {
            dragging = true;
            double angle = getDegreeByPoint(e.GetPosition(potentiometerSpace));
			if (!IsAngleInRange(angle))
			{
				if (IsLeftAngle(angle))
					angle = MaxAngle;
				else
					angle = MinAngle;
			}
            rotate.Angle = angle;
			SetValue(CurrentAngleProperty, angle);
			Console.WriteLine("Started dragging Angle=" + angle);
			
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
				{
					Console.WriteLine("To be corrected angle=" + angle);
					angle += Math.PI;
					Console.WriteLine("Corrected angle=" + angle);
				}
            }
            else
            {
				Console.WriteLine("Zero");

                if (py > 0.0)
					angle = HALF_PI;
                else if (py == 0.0)
                    angle = lastAngle;
                else
					angle = -HALF_PI;
            }
            lastAngle = angle;
			// Convert to degrees
            angle = 90.0 + angle * 180 / Math.PI;
			Console.WriteLine("p.x="+p.X+" p.y="+p.Y+" Angle=" + angle);
            return angle;
        }

        double lastAngle, centerX, centerY;

        private void potentiometer_MouseUp(object sender, MouseButtonEventArgs e)
        {
            dragging = false;
            Mouse.Capture(null);
            //Console.WriteLine("Stopped Dragging\n");

        }

		private Object owner;
		public void setOwner(Object _owner)
		{
			this.owner = _owner;
		}

		private double RemapAngle(double angle)
		{
			if(angle>MaxAngle)
			{
				return angle - MaxAngle;
			}
			else
			{
				return angle + MinAngle;
			}
		}

		bool IsAngleInRange(double angle)
		{
			return (angle < MinAngle || angle > MaxAngle);
		}

        private void potentiometer_MouseMove(object sender, MouseEventArgs e)
        {
            if (dragging)
            {
                Point p = e.GetPosition(potentiometerSpace);
                double angle = getDegreeByPoint(p);
				
				Console.WriteLine("Angle=" + angle);
				if (IsAngleInRange(angle))
				{
					rotate.Angle = angle;
					double remappedAngle = RemapAngle(angle);
					double level = GetNormalizedLevel(remappedAngle);
					SetValue(CurrentLevelProperty, level);
					//SetValue(CurrentAngleProperty, level);
					SetValue(CurrentAngleProperty, angle);
					if (PotentiometerChangedEvent != null)
					{
						PotentiometerChangedEvent(owner, level);
					}
				}
            }
        }

		const double HALF_PI = Math.PI / 2.0;

		double MinAngle = 135;
		double MaxAngle = 225;
		double MaxAngleInterval = 270;

		const bool DebugTextBoxOn = true;
    }
}
