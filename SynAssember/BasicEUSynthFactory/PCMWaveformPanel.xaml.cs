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
using SynthPanels;
using BasicAudioControls;
using GeneralUtils;

namespace BasicEUFact
{
    /// <summary>
    /// Interaction logic for PCMWaveformPanel.xaml
    /// </summary>
    public partial class PCMWaveformPanel : UserControl
    {
		const int NUM_NOTES = 128;

		static String GetNoteNameById(int noteId)
		{
			int note = noteId % 12;
			int octave = (noteId / 12) - 1;
			String strNote = "";
			switch(note)
			{
				case 0: strNote = "C "; break;
				case 1:	strNote = "C#"; break;
				case 2: strNote = "D "; break;
				case 3: strNote = "D#"; break;
				case 4: strNote = "E "; break;
				case 5: strNote = "F "; break;
				case 6: strNote = "F#"; break;
				case 7: strNote = "G "; break;
				case 8: strNote = "G#"; break;
				case 9: strNote = "A "; break;
				case 10: strNote = "A#"; break;
				case 11: strNote = "B "; break;
			}

			return strNote + " " + octave.ToString();
		}


		private class PitchItem
		{
			public String name;
			public int value;
			public PitchItem(String name, int value)
			{
				this.name = name;
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

		void AddAllPitchItems()
		{
			for(int i=0;i<NUM_NOTES;i++)
			{
				String noteName = GetNoteNameById(i);
				PitchItem item = new PitchItem(noteName, i);
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
			WaveSelectionComboBox.SelectedIndex = 0;

			//readParametersFromEngine();
        }
		/*
		String _wavesFolder;
		public String WavesFolder
		{
			get { return _wavesFolder; }
		}
		*/
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
			//PCMWaveformPanel wavPanel = (PCMWaveformPanel)sender;
			facilities.ChangedAlgorithm.algorithmChanged();
		}

		internal void readParametersFromEngine()
		{
			double lvl = synDeleHolder.readEUDprop(id, OUTPUT_LEVEL_INDEX);
			OutLevel.NormalizedLevelValue = lvl;
			String waveName = synDeleHolder.readEUProp(id, WAVE_NAME_INDEX);
			ItemCollection waves = WaveSelectionComboBox.Items;
			
			for(int i=0;i<waves.Count;i++)
			//foreach(ItemsControl item in waves)
			{
				WaveFilenameItem item = (WaveFilenameItem) waves.GetItemAt(i);
				if (waveName.Equals(item.ToString()))
				{
					WaveSelectionComboBox.SelectedItem = item;
					break;
				}
			}
			int iPitch = synDeleHolder.readEUIProp(id, PITCH_INDEX);
			PitchComboBox.SelectedIndex = iPitch;
		}

		private void PitchComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			facilities.ChangedAlgorithm.algorithmChanged();
		}

		private void UserControl_Loaded(object sender, RoutedEventArgs e)
		{
			readParametersFromEngine();
		}
	}
}
