using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace WpfApplicationStudySpline
{
	/// <summary>
	/// Interaction logic for MainWindow.xaml
	/// </summary>
	public partial class MainWindow : Window
	{
		public MainWindow()
		{
			InitializeComponent();

			/*
			Line left = new Line();
			left.X1 = 0.0;
			left.Y1 = 20.0;
			left.X2 = 100;
			left.Y2 = 20;
			left.Stroke = Brushes.Black;
			Canvas.SetTop(left, 0.0);
			Canvas.SetLeft(left, 0.0);
			*/


		}


		double getY(double a, double b, double x, bool top)
		{
			double result = (b / a) * Math.Sqrt(Math.Pow(a, 2.0) - Math.Pow(x, 2.0));
			if (top)
				return -result;
			else
				return result;
		}

		List<Point> getTransitionPoints(int numPoints, double initialLevel, double finalLevel, double range)
		{
			double a = range / 2;
			double b = (finalLevel - initialLevel) / 2;

			List<Point> result = new List<Point>();
			double accX = 0.0;
			double step = range / numPoints;
			double x, y;
			for (int i = 0; i < numPoints; i++)
			{
				if (accX < range / 2)
				{
					y = b -getY(a, b, accX, false);
					y += initialLevel;
				}
				else
				{
					y = b + getY(a, b, range - accX, false);
					y += initialLevel;
				}
				Point p = new Point(accX, y);
				result.Add(p);
				accX += step;
			}

			return result;
		}

		double startEllipseX = 200;
		double range = 600;
		double initialLevel = 200;
		double finalLevel = 100;

		private void MyCanvas_Loaded(object sender, RoutedEventArgs evt)
		{
			Line left = new Line();
			left.X1 = 0;
			left.Y1 = initialLevel;
			left.X2 = startEllipseX;
			left.Y2 = initialLevel;
			left.Stroke = Brushes.Black;
			Canvas.SetTop(left, 0.0);
			Canvas.SetLeft(left, 0.0);
			MyCanvas.Children.Add(left);
			Line right = new Line();
			right.X1 = startEllipseX + range;
			right.Y1 = finalLevel;
			right.X2 = startEllipseX + range + 200;
			right.Y2 = finalLevel;
			right.Stroke = Brushes.Black;
			Canvas.SetTop(right, 0);
			Canvas.SetLeft(right, 0);
			MyCanvas.Children.Add(right);

			List<Point> points = getTransitionPoints(10000, initialLevel, finalLevel, range);
			foreach(Point p in points)
			{
				Ellipse e = new Ellipse();
				e.Height = 1;
				e.Width = 1;
				e.Stroke = Brushes.Black;
				Canvas.SetTop(e, p.Y);
				Canvas.SetLeft(e, p.X+startEllipseX);
				MyCanvas.Children.Add(e);
			}
		}
	}
}
