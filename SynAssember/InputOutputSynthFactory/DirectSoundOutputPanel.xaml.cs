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
using SynthPanels;

namespace InputOutputSynthFactory
{
    /// <summary>
    /// Interaction logic for DirectSoundOutput.xaml
    /// </summary>
    public partial class DirectSoundOutputPanel : UserControl
    {
        public DirectSoundOutputPanel()
        {
            InitializeComponent();
        }

		internal System.Drawing.Rectangle getRectangle()
		{
			return new System.Drawing.Rectangle(0, 0, 150, 200);
		}
    }
}
