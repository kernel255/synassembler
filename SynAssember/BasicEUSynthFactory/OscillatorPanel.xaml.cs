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
using System.Drawing;
using SynthPanels;
using BasicAudioControls;
using GeneralUtils;

namespace BasicEUSynthFactory
{
    /// <summary>
    /// Interaction logic for OscillatorPanel.xaml
    /// </summary>
    public partial class OscillatorPanel : UserControl
    {
        WriteEUDoubleProperty writeDProp;
		WriteEUIntegerProperty writeIProp;
		Facilities facilities;

        int id;

		public OscillatorPanel(int id, SynthDelegateHolder deleHolder, Facilities facilities)
		{
			InitializeComponent();
			OutputLevel.setOwner(this);
			WaveButton.setOwner(this);
			this.facilities = facilities;
			writeDProp = deleHolder.writeEUDProp;
			writeIProp = deleHolder.writeEUIProp;
            OutputLevel.SliderChangedEvent += new BasicSlider.SliderChanged(OutputLevel_SliderChangedEvent);
			WaveButton.WaveChangedEvent += new WaveButton.WaveChanged(WaveButton_ChangedEvent);
            this.id = id;
        }

        public static void OutputLevel_SliderChangedEvent(Object o, Double level)
        {
            OscillatorPanel oscPanel = (OscillatorPanel)o;
			oscPanel.writeDProp(oscPanel.id, LEVEL_PROPERTY_INDEX, level);
			oscPanel.facilities.ChangedAlgorithm.algorithmChanged();
        }

		public static void WaveButton_ChangedEvent(Object o, Int32 wave)
		{
			OscillatorPanel oscPanel = (OscillatorPanel)o;
			oscPanel.writeIProp(oscPanel.id, WAVE_PROPERTY_INDEX, wave);
			oscPanel.facilities.ChangedAlgorithm.algorithmChanged();
		}

        internal System.Drawing.Rectangle getRectangle()
        {
            return new System.Drawing.Rectangle(0, 0, 180, 220);
        }

		internal static int LEVEL_PROPERTY_INDEX = 0;
		internal static int WAVE_PROPERTY_INDEX = 1;
    }
}
