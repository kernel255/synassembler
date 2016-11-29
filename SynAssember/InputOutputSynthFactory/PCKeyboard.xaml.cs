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


namespace InputOutputSynthFactory
{
	/// <summary>
	/// Interaction logic for PCKeyboard.xaml
	/// </summary>
	public partial class PCKeyboard : UserControl
	{
		public PCKeyboard()
		{
			InitializeComponent();
			DrawKeyboard(23);
		}

		static int BLACKKEY_HEIGHT = 90;
		const int WHITEKEY_HEIGHT = 140;
		const int BASE_WIDTH = 18;
		const int KEY_DISTANCE = 3;
		const int BORDER_WIDTH = 5;

		internal System.Drawing.Rectangle getRectangle()
		{
			return new System.Drawing.Rectangle(0, 0, 150, 200);
		}

		void DrawKeyboard(int numKeys)
		{
			Rectangle backgroundRect = new Rectangle();
			backgroundRect.Width = (BASE_WIDTH + KEY_DISTANCE * 2) * numKeys;
			backgroundRect.Height = WHITEKEY_HEIGHT + BORDER_WIDTH * 2;
			backgroundRect.Fill = new SolidColorBrush(Colors.BurlyWood);
			MainCanvas.Children.Add(backgroundRect);

			Rectangle rect = new Rectangle();
			double currX = BORDER_WIDTH, currY = 0.0;
			for (int i = 0; i <= numKeys; i++)
			{
				rect = new Rectangle();
				rect.Width = BASE_WIDTH;

				if (IsWhite(i))
				{
					rect.Height = WHITEKEY_HEIGHT;
					rect.Fill = new SolidColorBrush(Colors.White);
					if (IsFullAhead(i))
					{
						Rectangle r2 = new Rectangle();
						r2.Width = BASE_WIDTH / 2;
						r2.Height = WHITEKEY_HEIGHT - BLACKKEY_HEIGHT;
						Canvas.SetTop(r2, BLACKKEY_HEIGHT + BORDER_WIDTH);
						Canvas.SetLeft(r2, currX + BASE_WIDTH);
						r2.Fill = new SolidColorBrush(Colors.White);
						MainCanvas.Children.Add(r2);
					}
					if (IsFullBack(i))
					{
						Rectangle r3 = new Rectangle();
						r3.Width = BASE_WIDTH / 2;
						r3.Height = WHITEKEY_HEIGHT - BLACKKEY_HEIGHT;
						Canvas.SetTop(r3, BLACKKEY_HEIGHT + BORDER_WIDTH);
						Canvas.SetLeft(r3, currX - BASE_WIDTH / 2);
						r3.Fill = new SolidColorBrush(Colors.White);
						MainCanvas.Children.Add(r3);
					}
				}
				else
				{
					rect.Height = BLACKKEY_HEIGHT;
					rect.Fill = new SolidColorBrush(Colors.Black);
				}
				Canvas.SetLeft(rect, currX);
				Canvas.SetTop(rect, currY + BORDER_WIDTH);
				currX += rect.Width + KEY_DISTANCE;
				MainCanvas.Children.Add(rect);
			}
			backgroundRect.Width = currX + KEY_DISTANCE/2;
		}

		bool IsWhite(int i)
		{
			int ofs = i % 12;
			switch (ofs)
			{
				case 0:
				case 2:
				case 4:
				case 5:
				case 7:
				case 9:
				case 11:
					return true;
				default:
					return false;
			}
		}

		bool IsFullAhead(int i)
		{
			int ofs = i % 12;
			switch (ofs)
			{
				case 0:
				case 2:
				case 3:
				case 5:
				case 7:
				case 9:
					return true;
				default:
					return false;
			}
		}
		bool IsFullBack(int i)
		{
			int ofs = i % 12;
			switch (ofs)
			{
				case 2:
				case 4:
				case 7:
				case 9:
				case 11:
					return true;
				default:
					return false;
			}
		}


	}
}
