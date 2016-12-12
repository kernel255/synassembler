﻿using Microsoft.Win32;
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
		private class PitchItem
		{
			public String name;
			public int value;
			public PitchItem(String name, int value)
			{
				this.name = (value > 0 ? "+" : "") + name;
				this.value = value;
			}

			public override string ToString()
			{
				return name;
			}
		}

		PitchItem Zero;

		const int MAX_PITCH_OFFSET = 5;

		void AddAllPitchItems()
		{
			int totalRange = MAX_PITCH_OFFSET*2 + 1;
			for (int i=0;i<totalRange;i++)
			{
				int index = i - MAX_PITCH_OFFSET;
				PitchItem item = new PitchItem(index.ToString(), index);
				if (item.value == 0)
					Zero = item;
				PitchComboBox.Items.Add(item);
			}
		}

        public PCMWaveformPanel()
        {
            InitializeComponent();
			AddAllPitchItems();
			PitchComboBox.SelectedItem = Zero;
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
