using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;
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

namespace BasicEUSynthFactory
{
    /// <summary>
    /// Interaction logic for PCMWaveformPanel.xaml
    /// </summary>
    public partial class PCMWaveformPanel : UserControl
    {
        public PCMWaveformPanel()
        {
            InitializeComponent();
        }

		private void BrowseButton_Click(object sender, RoutedEventArgs e)
		{
			OpenFileDialog dlg = new OpenFileDialog();

			dlg.CheckFileExists = true;
			dlg.Multiselect = false;
			dlg.Title = "Open a waveform file";
			dlg.ShowDialog();
			if(dlg.FileName.Length!=0)
			{
				
			}

		}
	}
}
