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
		ReadEUDoubleProperty readDProp;
		ReadEUIntegerProperty readIProp;
		Facilities facilities;

        int id;

		public OscillatorPanel(int id, SynthDelegateHolder deleHolder, Facilities facilities)
		{
			InitializeComponent();
			OutputLevel.setOwner(this);
			WaveButton.setOwner(this);
			AttackRate.setOwner(this);
			DecayRate.setOwner(this);
			ReleaseRate.setOwner(this);
			AttackLevel.setOwner(this);
			SustainLevel.setOwner(this);
			this.facilities = facilities;
			AttackRate.MaxLevel = 100;
			DecayRate.MaxLevel = 100;
			ReleaseRate.MaxLevel = 600;
			writeDProp = deleHolder.writeEUDProp;
			writeIProp = deleHolder.writeEUIProp;
			readDProp = deleHolder.readEUDprop;
			readIProp = deleHolder.readEUIProp;
            OutputLevel.SliderChangedEvent += new BasicSlider.SliderChanged(OutputLevel_SliderChangedEvent);

			AttackRate.SliderChangedEvent += new BasicSlider.SliderChanged(AttackTime_SliderChangedEvent);
			DecayRate.SliderChangedEvent += new BasicSlider.SliderChanged(DecayTime_SliderChangedEvent);
			ReleaseRate.SliderChangedEvent += new BasicSlider.SliderChanged(ReleaseTime_SliderChangedEvent);
			AttackLevel.SliderChangedEvent += new BasicSlider.SliderChanged(AttackLevel_SliderChangedEvent);
			SustainLevel.SliderChangedEvent += new BasicSlider.SliderChanged(SustainLevel_SliderChangedEvent);

			WaveButton.WaveChangedEvent += new WaveButton.WaveChanged(WaveButton_ChangedEvent);
            this.id = id;
        }

		static void SetChangedProperty(Object o, Double level, int index)
		{
			OscillatorPanel oscPanel = (OscillatorPanel)o;
			oscPanel.writeDProp(oscPanel.id, index, level);
			oscPanel.facilities.ChangedAlgorithm.algorithmChanged();
		}

		public static void OutputLevel_SliderChangedEvent(Object o, Double level)
		{
			OscillatorPanel oscPanel = (OscillatorPanel)o;
			oscPanel.writeDProp(oscPanel.id, LEVEL_PROPERTY_INDEX, level);
			oscPanel.facilities.ChangedAlgorithm.algorithmChanged();
		}

		public static void AttackTime_SliderChangedEvent(Object o, Double level)	{	SetChangedProperty(o, level, AT_PROPERTY_INDEX);	}
		public static void DecayTime_SliderChangedEvent(Object o, Double level) { SetChangedProperty(o, level, DT_PROPERTY_INDEX); }
		public static void ReleaseTime_SliderChangedEvent(Object o, Double level) { SetChangedProperty(o, level, RT_PROPERTY_INDEX); }
		public static void AttackLevel_SliderChangedEvent(Object o, Double level) { SetChangedProperty(o, level, AL_PROPERTY_INDEX); }
		public static void SustainLevel_SliderChangedEvent(Object o, Double level) { SetChangedProperty(o, level, SL_PROPERTY_INDEX); }

		public static void WaveButton_ChangedEvent(Object o, Int32 wave)
		{
			OscillatorPanel oscPanel = (OscillatorPanel)o;
			oscPanel.writeIProp(oscPanel.id, WAVE_PROPERTY_INDEX, wave);
			oscPanel.facilities.ChangedAlgorithm.algorithmChanged();
		}

        internal System.Drawing.Rectangle getRectangle()
        {
            return new System.Drawing.Rectangle(0, 0, 470, 220);
        }

		internal void readParametersFromEngine()
		{
			int waveId = readIProp(id, WAVE_PROPERTY_INDEX);
			WaveButton.SelectedWave = waveId;
			double outLevel = readDProp(id, LEVEL_PROPERTY_INDEX);
			OutputLevel.NormalizedLevelValue = outLevel;
			// ADSR
			double at = readDProp(id, AT_PROPERTY_INDEX);
			AttackRate.LevelValue = at;
			double dt = readDProp(id, DT_PROPERTY_INDEX);
			DecayRate.LevelValue = dt;
			double rt = readDProp(id, RT_PROPERTY_INDEX);
			ReleaseRate.LevelValue = rt;
			double al = readDProp(id, AL_PROPERTY_INDEX);
			AttackLevel.LevelValue = al;
			double sl = readDProp(id, SL_PROPERTY_INDEX);
			SustainLevel.LevelValue = sl;
		}

		internal static int LEVEL_PROPERTY_INDEX = 0;

		internal static int WAVE_PROPERTY_INDEX = 1;

		internal static int AT_PROPERTY_INDEX = 2;
		internal static int DT_PROPERTY_INDEX = 3;
		internal static int RT_PROPERTY_INDEX = 4;
		internal static int AL_PROPERTY_INDEX = 5;
		internal static int SL_PROPERTY_INDEX = 6;
    }
}
