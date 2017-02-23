﻿using System;
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
using GeneralUtils;

namespace BasicAudioControls
{
    /// <summary>
    /// Interaction logic for MySlider.xaml
    /// </summary>
    public partial class BasicSlider : UserControl
    {
        private Point initialPos;
        private bool dragging = false;
        private double delta;

        public static DependencyProperty ValueProperty;
        public static DependencyProperty LabelProperty;

        public Double NormalizedLevelValue
        {
            get 
			{ 
				return (Double)GetValue(ValueProperty);
			}
            set { 
				SetValue(ValueProperty, value);
				if (!sliderChanging)
				{
					setPosition(value);
					// Change does not come from slider: update cursor position
					//double min = getCursorMin();
					//double max = getCursorMax();
					//double lvl = value * (max - min);
					//Canvas.SetTop(SliderCursor, min + lvl);
				}
			}
        }

		public Double LevelValue
		{
			set
			{
				double norm = getNormalizedLevel(value);
				NormalizedLevelValue = norm;
			}
		}

		double max = 1.0;
		public Double MaxLevel
		{
			set
			{
				max = value;
			}
		}

		double getNormalizedLevel(double d)
		{
			return d / max;
		}

		double getDenormalizedLevel(double d)
		{
			return d * max;
		}

		void setPosition(double val)
		{
			double min = getCursorMin();
			double max = getCursorMax();
			double ext = getCursorExtension();
			Facilities.Log("MySlider: val {0} min {1} max {2}", val, min, max);
			val = 1 - val;
			double pos = min + ext * val - SliderCursor.ActualHeight/2;

			Canvas.SetTop(SliderCursor, pos);
		}

        public String Label
        {
            get { return (String)GetValue(LabelProperty); }
            set { SetValue(LabelProperty, value); }
        }

        private const Double LEVEL_DEFAULT_VALUE = 0.0;

        private Double previousValue = LEVEL_DEFAULT_VALUE;

        static BasicSlider()
        {
            ValueProperty = DependencyProperty.Register("NormalizedLevelValue", typeof(Double), typeof(BasicSlider),
                new FrameworkPropertyMetadata(LEVEL_DEFAULT_VALUE, new PropertyChangedCallback(OnLevelChanged)));
            LabelProperty = DependencyProperty.Register("Label", typeof(String), typeof(BasicSlider),
                new FrameworkPropertyMetadata("", new PropertyChangedCallback(OnLabelChanged)));
        }

        public BasicSlider()
        {
            InitializeComponent();
        }

		private Object owner;
		public void setOwner(Object _owner)
		{
			this.owner = _owner;
		}

        private static void OnLevelChanged(DependencyObject sender, DependencyPropertyChangedEventArgs e)
        {

			BasicSlider me = (BasicSlider)sender;
			Double level = (Double)e.NewValue;
			level = 1.0 - level;
        }

        private static void OnLabelChanged(DependencyObject sender, DependencyPropertyChangedEventArgs e)
        {
            BasicSlider me = (BasicSlider)sender;
            String name = (String) e.NewValue;
            me.SliderLabel.Content = name;
        }

        private void showPressed(bool press)
        {
        }

        private void SliderCursor_MouseDown(object sender, MouseButtonEventArgs e)
        {
            initialPos = e.GetPosition(null);
            delta = initialPos.Y - Canvas.GetTop(SliderCursor);
            showPressed(true);
            dragging = true;
            //Trace.Write("Start Dragging\n");
        }

        private void SliderCursor_MouseUp(object sender, MouseButtonEventArgs e)
        {
            dragging = false;
            Mouse.Capture(null);
            showPressed(false);
            //Trace.Write("Stopped Dragging\n");
        }

        private double getCursorCenterByTop(double py)
        {
            double result = py + SliderCursor.ActualHeight / 2;
            return result;
        }

        private double CURSOR_EXTENSION_PERCENTAGE = 80;

        private double getCursorExtension()
        {
            return SliderSpace.ActualHeight * (CURSOR_EXTENSION_PERCENTAGE / 100.0);
        }


        private double getCursorMin()
        {
            return 0 + (SliderSpace.ActualHeight - getCursorExtension()) / 2;
        }

        private double getCursorMax()
        {
            return getCursorMin() + getCursorExtension();
        }

        private double normalizeLevelToUI(double val)
        {
            double range = getCursorMax() - getCursorMin() - 1;

            return 1 - (val / range);
        }

		private double unNormalizeLevelFromUI(double val)
		{
			double range = getCursorMax() - getCursorMin() - 1;

			return (val - 1) * range;
		}

        public delegate void SliderChanged(Object sender, Double value);

        public event SliderChanged SliderChangedEvent;

		bool sliderChanging = false;

        private void SliderCursor_MouseMove(object sender, MouseEventArgs e)
        {
            if (dragging)
            {
                Point p = e.GetPosition(null);
                double estimatedTop = p.Y - delta;
                double estimatedCursorCenter = getCursorCenterByTop(estimatedTop);
                //Trace.Write("estimatedCursorCenter=" + estimatedCursorCenter + " p.Y=" + p.Y + " CursorMax=" + getCursorMax() + 
                //    " CursorMin=" + getCursorMin() + " delta=" + delta + "\n");
                double cursorMax = getCursorMax();
                double cursorMin = getCursorMin();
                if (estimatedCursorCenter <= cursorMax && estimatedCursorCenter >= cursorMin)
                {
                    Canvas.SetTop(SliderCursor, p.Y - delta);
					sliderChanging = true;
                    NormalizedLevelValue = normalizeLevelToUI(estimatedCursorCenter - cursorMin);
					double d = -9999;
                    if (NormalizedLevelValue != previousValue)
                    {
                        // Raise an event
						if (SliderChangedEvent != null)
						{
							d = getDenormalizedLevel(NormalizedLevelValue);
							SliderChangedEvent(owner, d);
						}
                    }
					sliderChanging = false;
                    previousValue = NormalizedLevelValue;
					StringBuilder msg = new StringBuilder();
					msg.AppendFormat("MySlider.SliderCursor_MouseMove: Denormalized {0} Normalized {1}\n", d, NormalizedLevelValue);
					Facilities.Log(msg.ToString());
                }
            }
        }

        private void SliderCursor_MouseLeave(object sender, MouseEventArgs e)
        {
            if (dragging)
            {
                Trace.Write("Mouse leaved from control\n");
                Mouse.Capture(SliderCursor);
            }
        }

		private void UserControl_Loaded(object sender, RoutedEventArgs e)
		{
			string label = (string)GetValue(LabelProperty);
			Facilities.Log("Slider Loaded label={0}", label);
			double value = (Double)GetValue(ValueProperty);
			Facilities.Log("Level = {0}", value);
			double lvl = unNormalizeLevelFromUI(value);
			double min = getCursorMin();
			if(min!=0)
			{
				double offset = Canvas.GetTop(SliderPanel);
				//double max = getCursorMax();
				//double lvl = value * (max - min);
				Canvas.SetTop(SliderCursor, offset + lvl);
			}
		}
	}
}
