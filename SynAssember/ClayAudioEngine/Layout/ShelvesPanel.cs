using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;

namespace ClayAudioEngine.Layout
{
	public class ShelvesPanel
	{
		Canvas canvas;
		public ShelvesPanel(Canvas canvas)
		{
			this.canvas = canvas;
		}

		public void SizeChanged(object sender, SizeChangedEventArgs e)
		{
			try { canvas.Width = GetWidth(); } catch { }
			try { canvas.Height = GetHeight(); } catch { }
		}

		double GetWidth()
		{
			return 400;
		}

		double GetHeight()
		{
			return 400;
		}
	}
}
