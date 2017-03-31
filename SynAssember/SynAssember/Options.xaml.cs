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
using System.Windows.Shapes;

namespace SynAssember
{
    /// <summary>
    /// Interaction logic for Options.xaml
    /// </summary>
    public partial class Options : Window
    {
        public Options()
        {
            InitializeComponent();
        }

        private bool loadAtStartup;
        public bool LoadAtStartup
        {
            get
            {
                return loadAtStartup;
            }
            set
            {
                loadAtStartup = value;
                LoadAtStartupCheckbox.IsChecked = loadAtStartup;
            }
        }

		private bool hardcodedMode;
		public bool HardcodedMode
		{
			get
			{
				return hardcodedMode;
			}
			set
			{
				hardcodedMode = value;
				HardcodedSynth.IsChecked = hardcodedMode;
			}
		}

        private void OkButton_Click(object sender, RoutedEventArgs e)
        {
			/*
            if (LoadAtStartupCheckbox.IsChecked == true)
                loadAtStartup = true;
            else
                loadAtStartup = false;
			*/
			loadAtStartup = (LoadAtStartupCheckbox.IsChecked==true);
			hardcodedMode = (HardcodedSynth.IsChecked==true);
            Close();
        }

        private void CancelButton_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }
    }
}
