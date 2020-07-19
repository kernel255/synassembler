using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WpfApplicationStudySpline
{
	class DeltaRounder
	{
		public DeltaRounder(double _initialLevel, double _finalLevel, double _initialX, double _range)
		{
			m_InitialLevel = _initialLevel;
			m_FinalLevel = _finalLevel;
			m_InitialX = _initialX;
			m_Range = _range;
			a = _range / 2;
			b = (_finalLevel - _initialLevel) / 2;
		}

		private double getY(double a, double b, double x)
		{
			double result = (b / a) * Math.Sqrt(Math.Pow(a, 2.0) - Math.Pow(x, 2.0));
			return result;
		}

		public double getYValue(double x)
		{
			double y;
			double relX = x - m_InitialX;
			if (relX < m_Range / 2)
			{
				y = b - getY(a, b, relX);
				y += m_InitialLevel;
			}
			else
			{
				y = b + getY(a, b, m_Range - relX);
				y += m_InitialLevel;
			}
			return y;
		}

		double m_InitialLevel;
		double m_FinalLevel;
		double m_InitialX;
		double m_Range;
		double a;
		double b;
	}
}
