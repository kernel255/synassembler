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

namespace BasicEUSynthFactory
{
	/// <summary>
	/// Interaction logic for MixerPanel.xaml
	/// </summary>
	public partial class MixerPanel : UserControl
	{
		WriteEUDoubleProperty writeDProp;
		ReadEUDoubleProperty readDProp;

		Facilities facilities;

		int id;

		public MixerPanel(int id, SynthDelegateHolder deleHolder, Facilities facilities)
		{
			InitializeComponent();
			this.facilities = facilities;

			writeDProp = deleHolder.writeEUDProp;
			readDProp = deleHolder.readEUDprop;
			this.id = id;
		}

		public MixerPanel()
		{
			InitializeComponent();
		}

		internal System.Drawing.Rectangle getRectangle()
		{
			return new System.Drawing.Rectangle(0, 0, 280, 160);
		}

	}
}
