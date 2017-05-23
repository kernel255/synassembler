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
using BasicAudioControls;
using GeneralUtils;


namespace BasicEUFact
{
    /// <summary>
    /// Interaction logic for MIDISequencePanel.xaml
    /// </summary>
    public partial class MIDISequencePanel : UserControl
    {
		public MIDISequencePanel(int id, SynthDelegateHolder deleHolder, Facilities facilities)
		{
			InitializeComponent();
			AddAllMIDIFiles(MIDISequencePlayerWrapper.s_MIDIFilenames);
			this.id = id;
			this.facilities = facilities;
			synDeleHolder = deleHolder;
		}

		int id;
		Facilities facilities;
		SynthDelegateHolder synDeleHolder;

		private class MIDIFilenameItem
		{
			public String name;
			public String filename;
			public MIDIFilenameItem(String _name, String _filename)
			{
				name = _name;
				filename = _filename;
			}
			public override string ToString()
			{
				return name;
			}
		}

		void AddAllMIDIFiles(List<String> filenames)
		{
			foreach(String name in filenames)
			{
				MIDIFilenameItem item = new MIDIFilenameItem(name, name);
				MIDISequenceNameComboBox.Items.Add(item);
			}
		}

        public MIDISequencePanel()
        {
            InitializeComponent();
			AddAllMIDIFiles(MIDISequencePlayerWrapper.s_MIDIFilenames);
        }

		private void MIDISequenceName_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			MIDIFilenameItem selItem = (MIDIFilenameItem)MIDISequenceNameComboBox.SelectedItem;
			if(selItem!=null)
			{
				String midiName = selItem.name;
				synDeleHolder.writeEUProp(id, MIDI_NAME_INDEX, midiName);
			}
			if(loadCompleted)
			{
				facilities.ChangedAlgorithm.algorithmChanged();
			}
		}

		internal static int MIDI_NAME_INDEX = 0;

		internal void readParametersFromEngine()
		{
			String midiFilename = synDeleHolder.readEUProp(id, MIDI_NAME_INDEX);
			ItemCollection midis = MIDISequenceNameComboBox.Items;
			for(int i=0;i<midis.Count;i++)
			{
				MIDIFilenameItem item = (MIDIFilenameItem)midis.GetItemAt(i);
				if(midiFilename.Equals(item.ToString()))
				{
					MIDISequenceNameComboBox.SelectedItem = item;
					break;
				}
			}
		}

		private void UserControl_Loaded(object sender, RoutedEventArgs e)
		{
			readParametersFromEngine();
			loadCompleted = true;
		}

		private bool loadCompleted = false;
	}
}
