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

namespace BasicAudioControls
{
	/// <summary>
	/// Interaction logic for OnOffLedButton.xaml
	/// </summary>
	public partial class OnOffLedButton : UserControl
	{
		public OnOffLedButton()
		{
			InitializeComponent();
		}

		static OnOffLedButton()
		{
			FrameworkPropertyMetadata metadata1 = new FrameworkPropertyMetadata(new Double(), FrameworkPropertyMetadataOptions.AffectsMeasure);
			WidthProperty = DependencyProperty.Register("Width", typeof(Double), typeof(OnOffLedButton), metadata1);
			FrameworkPropertyMetadata metadata2 = new FrameworkPropertyMetadata(new Double(), FrameworkPropertyMetadataOptions.AffectsMeasure);
			HeightProperty = DependencyProperty.Register("Height", typeof(Double), typeof(OnOffLedButton), metadata2);
		}

		public delegate void SwitchChanged(Object sender, bool value);

		public event SwitchChanged SwitchChangeEvent;

		object owner;
		public void SetOwner(object _owner)
		{
			owner = _owner;
		}

		public Double Width
		{
			set { SetValue(WidthProperty, value); }
			get { return (Double)GetValue(WidthProperty); }
		}

		public Double Height
		{
			set { SetValue(HeightProperty, value); }
			get { return (Double)GetValue(HeightProperty); }
		}

		public static readonly DependencyProperty WidthProperty;
		public static readonly DependencyProperty HeightProperty;

		bool bLedOn = false;

		public bool SwitchOn
		{
			set 
			{ 
				bLedOn = value;
				UpdateButtonColor();
			}
			get { return bLedOn; }
		}

		private void UpdateButtonColor()
		{
			if (bLedOn)
			{
				Led.Fill = Brushes.Green;
			}
			else
			{
				
				Led.Fill = Brushes.Red;
			}
		}

		private void ToggleButton()
		{
			bLedOn = !bLedOn;
			UpdateButtonColor();
			SwitchChangeEvent(owner, bLedOn);
		}

		private void Led_MouseDown(object sender, MouseButtonEventArgs e)
		{
			ToggleButton();
		}

		const double WHRatio = 0.40;

		private void UserControl_Loaded(object sender, RoutedEventArgs e)
		{
			ButtonSpace.Width = Width;
			ButtonSpace.Height = Height;
			double ledWidth = Width * WHRatio;
			double ledHeight = Height * WHRatio;
			Led.Width = ledWidth;
			Led.Height = ledHeight;
			Canvas.SetTop(Led, (Height - ledHeight) / 2.0);
			Canvas.SetLeft(Led, (Width - ledWidth) / 2.0);

		}


	}
}
