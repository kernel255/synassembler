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

namespace BasicEUFact
{
    /// <summary>
    /// Interaction logic for MIDISequencePanel.xaml
    /// </summary>
    public partial class MIDISequencePanel : UserControl
    {

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
				MIDISequenceName.Items.Add(item);
			}
		}

        public MIDISequencePanel()
        {
            InitializeComponent();
			AddAllMIDIFiles(MIDISequencePlayerWrapper.s_MIDIFilenames);
        }

		private void MIDISequenceName_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{

		}
	}
}
