using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Shapes;
using System.Windows;
using System.Windows.Media;

namespace BasicAudioControls
{
    public class DrawUtils
    {
        private static Point createCirclePoint(double x, double y, double radius, double angle)
        {
            double px = x + radius * Math.Cos(angle);
            double py = y + radius * Math.Sin(angle);
            Point p = new Point(px, py);
            return p;
        }

        public static IList<Line> createTicks(double x, double y, double innerRadius, double outerRadius, int num, int longerModulo = -1)
        {
            List<Line> ticks = new List<Line>();
            double step = 2 * Math.PI / num;
            double mediumRadius = innerRadius + (outerRadius - innerRadius) / 2;
            for (int i = 0; i < num; i++)
            {
                Point start = createCirclePoint(x, y, outerRadius, step * i);
                Point end;
                if (longerModulo == -1)
                {
                    end = createCirclePoint(x, y, innerRadius, step * i);
                }
                else
                {
                    if (i % longerModulo != 0)
                    {
                        end = createCirclePoint(x, y, mediumRadius, step * i);
                    }
                    else
                        end = createCirclePoint(x, y, innerRadius, step * i);
                }
                Line tick = new Line();
                tick.X1 = start.X;
                tick.Y1 = start.Y;
                tick.X2 = end.X;
                tick.Y2 = end.Y;
                tick.Stroke = Brushes.Black;
                ticks.Add(tick);
            }

            return ticks;
        }

        public static Polyline createCrown(double x, double y, double innerRadius, double outerRadius, int numTheets)
        {
            Polyline extLine = new Polyline();
            PointCollection extPoints = new PointCollection();

            double step = 2 * Math.PI / numTheets;
            for (int i = 0; i < numTheets; i++)
            {
                Point p0 = createCirclePoint(x, y, innerRadius, step * i);
                extPoints.Add(p0);
                Point p1 = createCirclePoint(x, y, outerRadius, step * i);
                extPoints.Add(p1);
                Point p2 = createCirclePoint(x, y, outerRadius, step * i + (0.5 * step));
                extPoints.Add(p2);
                Point p3 = createCirclePoint(x, y, innerRadius, step * i + (0.5 * step));
                extPoints.Add(p3);
                Point p4 = createCirclePoint(x, y, innerRadius, step * (i + 1));
                extPoints.Add(p4);
            }
            extLine.Points = extPoints;

            return extLine;
        }

        public static Polygon createHexagon(double x, double y, double radius, Brush fillBrush, Brush strokeBrush, double thickness)
        {
            Polygon hex = new Polygon();
            double angle = 0, centerX = x + radius, centerY = y + radius;
            double step = Math.PI * 2 / 6;
            for (int i = 0; i < 6; i++)
            {
                Point p = createCirclePoint(centerX, centerY, radius, angle);
                hex.Points.Add(p);
		
                angle += step;
            }
            hex.Fill = fillBrush;
			hex.Stroke = strokeBrush;
			hex.StrokeThickness = thickness;

            return hex;
        }

		public static Polyline createHexagonContour(double x, double y, double radius, Brush brush)
		{
			Polyline hex = new Polyline();
			PointCollection points = new PointCollection();
			double angle = 0, centerX = x + radius, centerY = y + radius;
			double step = Math.PI * 2 / 6;
			for (int i = 0; i < 6; i++)
			{
				Point p = createCirclePoint(centerX, centerY, radius, angle);
				points.Add(p);
				hex.Points = points;

				angle += step;
			}
			hex.Fill = Brushes.Transparent;


			return hex;
		}


	}
}
