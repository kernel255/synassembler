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
using SynthPanels;
using BasicAudioControls;
using GeneralUtils;

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

		private class WaveFilenameItem
		{
			public String name;
			public String filename;
			public WaveFilenameItem(String name, String filename)
			{
				this.name = name;
				this.filename = filename;
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

		void AddAllWaveforms(List<String> names)
		{
			foreach(String n in names)
			{
				WaveFilenameItem item = new WaveFilenameItem(n, n);
				WaveSelectionComboBox.Items.Add(item);
			}
		}

		int id;
		Facilities facilities;
		SynthDelegateHolder synDeleHolder;

        public PCMWaveformPanel(int id, SynthDelegateHolder deleHolder, Facilities facilities)
        {
            InitializeComponent();
			this.id = id;
			this.facilities = facilities;
			this.synDeleHolder = deleHolder;
			AddAllPitchItems();
			AddAllWaveforms(PCMWaveformWrapper.s_WaveNames);
			PitchComboBox.SelectedItem = Zero;
			WaveSelectionComboBox.SelectedIndex = 0;
        }

		String _wavesFolder;
		public String WavesFolder
		{
			get { return _wavesFolder; }
		}

		internal static int OUTPUT_LEVEL_INDEX = 0;
		internal static int WAVE_NAME_INDEX = 1;
		internal static int PITCH_INDEX = 2;

		private void WaveSelectionComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			WaveFilenameItem selectedItem = (WaveFilenameItem) WaveSelectionComboBox.SelectedItem;
			if (selectedItem != null)
			{
				String waveName = selectedItem.name;
				synDeleHolder.writeEUProp(id, WAVE_NAME_INDEX, waveName);
			}
		}

		internal void readParametersFromEngine()
		{
			double lvl = synDeleHolder.readEUDprop(id, OUTPUT_LEVEL_INDEX);
			OutLevel.NormalizedLevelValue = lvl;
			String waveName = synDeleHolder.readEUProp(id, WAVE_NAME_INDEX);
			ItemCollection waves = WaveSelectionComboBox.Items;
			foreach(ItemsControl item in waves)
			{
				if(waveName.Equals(item.ToString()))
				{
					WaveSelectionComboBox.SelectedItem = item;
				}
			}
			int iPitch = synDeleHolder.readEUIProp(id, PITCH_INDEX);
			PitchItem pitch = new PitchItem(iPitch.ToString(), iPitch);
			PitchComboBox.SelectedItem = pitch;
		}
	}
}
