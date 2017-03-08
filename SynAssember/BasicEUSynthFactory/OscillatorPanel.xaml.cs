﻿using System;
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


namespace BasicEUFact
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
		WriteEUBoolProperty writeBProp;
		ReadEUBoolProperty readBProp;
		
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
			LFOAmpOnOff.SetOwner(this);
			LFOFreqOnOff.SetOwner(this);
			LFOAmplitudeAmp.setOwner(this);
			LFOAmplitudeFreq.setOwner(this);
			LFOFrequencyAmp.setOwner(this);
			LFOFrequencyFreq.setOwner(this);
			this.facilities = facilities;
			AttackRate.MaxLevel = 200;
			DecayRate.MaxLevel = 200;
			ReleaseRate.MaxLevel = 500;
			// Other sliders not set while using default = 1.0

			writeDProp = deleHolder.writeEUDProp;
			writeIProp = deleHolder.writeEUIProp;
			writeBProp = deleHolder.writeEUBProp;
			readDProp = deleHolder.readEUDprop;
			readIProp = deleHolder.readEUIProp;
			readBProp = deleHolder.readEUBProp;
			
            OutputLevel.SliderChangedEvent += new BasicSlider.SliderChanged(OutputLevel_SliderChangedEvent);

			AttackRate.SliderChangedEvent += new BasicSlider.SliderChanged(AttackTime_SliderChangedEvent);
			DecayRate.SliderChangedEvent += new BasicSlider.SliderChanged(DecayTime_SliderChangedEvent);
			ReleaseRate.SliderChangedEvent += new BasicSlider.SliderChanged(ReleaseTime_SliderChangedEvent);
			AttackLevel.SliderChangedEvent += new BasicSlider.SliderChanged(AttackLevel_SliderChangedEvent);
			SustainLevel.SliderChangedEvent += new BasicSlider.SliderChanged(SustainLevel_SliderChangedEvent);

			WaveButton.WaveChangedEvent += new WaveButton.WaveChanged(WaveButton_ChangedEvent);

			LFOAmpOnOff.SwitchChangeEvent += new OnOffLedButton.SwitchChanged(LFOAmp_ChangedEvent);
			LFOFreqOnOff.SwitchChangeEvent += new OnOffLedButton.SwitchChanged(LFOFreq_ChangedEvent);

			LFOAmplitudeAmp.PotentiometerChangedEvent += new BasicPotentiometer.PotentiometerChanged(LFOAmpAmp_ChangedEvent);
			LFOAmplitudeFreq.PotentiometerChangedEvent += new BasicPotentiometer.PotentiometerChanged(LFOAmpFreq_ChangedEvent);
			LFOFrequencyAmp.PotentiometerChangedEvent += new BasicPotentiometer.PotentiometerChanged(LFOFreqAmp_ChangedEvent);
			LFOFrequencyFreq.PotentiometerChangedEvent += new BasicPotentiometer.PotentiometerChanged(LFOFreqFreq_ChangedEvent);

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

		public static void LFOAmp_ChangedEvent(Object o, bool on)
		{
			OscillatorPanel oscPanel = (OscillatorPanel)o;
			oscPanel  .writeBProp(oscPanel.id, AMP_LFO_ENABLE_INDEX, on);
			oscPanel.facilities.ChangedAlgorithm.algorithmChanged();
		}

		public static void LFOFreq_ChangedEvent(Object o, bool on)
		{
			OscillatorPanel oscPanel = (OscillatorPanel)o;
			oscPanel.writeBProp(oscPanel.id, FREQ_LFO_ENABLE_INDEX, on);
			oscPanel.facilities.ChangedAlgorithm.algorithmChanged();
		}

		public static void LFOAmpAmp_ChangedEvent(Object o, double val) { SetChangedProperty(o, val, AMP_LFO_LEVEL_INDEX); }
		public static void LFOAmpFreq_ChangedEvent(Object o, double val) { SetChangedProperty(o, val, AMP_LFO_FREQ_INDEX); }
		public static void LFOFreqAmp_ChangedEvent(Object o, double val) { SetChangedProperty(o, val, FREQ_LFO_LEVEL_INDEX); }
		public static void LFOFreqFreq_ChangedEvent(Object o, double val) { SetChangedProperty(o, val, FREQ_LFO_FREQ_INDEX); }

        internal System.Drawing.Rectangle getRectangle()
        {
            return new System.Drawing.Rectangle(0, 0, 450, 400);
        }

		internal void readParametersFromEngine()
		{
			int waveId = readIProp(id, WAVE_PROPERTY_INDEX);
			WaveButton.SelectedWave = waveId;
			double outLevel = readDProp(id, LEVEL_PROPERTY_INDEX);
			OutputLevel.NormalizedLevelValue = outLevel;
			// ADSR
			double at = readDProp(id, AT_PROPERTY_INDEX);
			Facilities.Log("OscillatorPanel readParameters AR {0}", at);
			AttackRate.LevelValue = at;
			double dt = readDProp(id, DT_PROPERTY_INDEX);
			Facilities.Log("OscillatorPanel readParameters DR {0}", dt);
			DecayRate.LevelValue = dt;
			double rt = readDProp(id, RT_PROPERTY_INDEX);
			Facilities.Log("OscillatorPanel readParameters RR {0}", rt);
			ReleaseRate.LevelValue = rt;
			double al = readDProp(id, AL_PROPERTY_INDEX);
			Facilities.Log("OscillatorPanel readParameters AL {0}", al);
			AttackLevel.LevelValue = al;
			double sl = readDProp(id, SL_PROPERTY_INDEX);
			Facilities.Log("OscillatorPanel readParameters SL {0}", sl);
			SustainLevel.LevelValue = sl;

			bool enaLFOAmp = readBProp(id, AMP_LFO_ENABLE_INDEX);
			LFOAmpOnOff.SwitchOn = enaLFOAmp;

			double lfoAmpAmp = readDProp(id, AMP_LFO_LEVEL_INDEX);
			Facilities.Log("OscillatorPanel readParameters LFOAMPAMP {0}", lfoAmpAmp);
			LFOAmplitudeAmp.CurrentLevel = lfoAmpAmp;

			double lfoAmpFreq = readDProp(id, AMP_LFO_FREQ_INDEX);
			Facilities.Log("OscillatorPanel readParameters LFOAMPFRE {0}", lfoAmpFreq);
			LFOAmplitudeFreq.CurrentLevel = lfoAmpFreq;
			
			bool enaLFOFreq = readBProp(id, FREQ_LFO_ENABLE_INDEX);
			LFOFreqOnOff.SwitchOn = enaLFOFreq;

			double lfoFreqAmp = readDProp(id, FREQ_LFO_LEVEL_INDEX);
			Facilities.Log("OscillatorPanel readParameters LFOFREAMP {0}", lfoFreqAmp);
			LFOFrequencyAmp.CurrentLevel = lfoFreqAmp;

			double lfoFreqFreq = readDProp(id, FREQ_LFO_FREQ_INDEX);
			Facilities.Log("OscillatorPanel readParameters LFOFREFRE {0}", lfoFreqFreq);
			LFOFrequencyFreq.CurrentLevel = lfoFreqFreq;
		}

		internal static int LEVEL_PROPERTY_INDEX = 0;

		internal static int WAVE_PROPERTY_INDEX = 1;
		internal static int AT_PROPERTY_INDEX = 2;
		internal static int DT_PROPERTY_INDEX = 3;
		internal static int RT_PROPERTY_INDEX = 4;
		internal static int AL_PROPERTY_INDEX = 5;
		internal static int SL_PROPERTY_INDEX = 6;

		internal static int AMP_LFO_ENABLE_INDEX = 7;
		internal static int FREQ_LFO_ENABLE_INDEX = 8;
		internal static int AMP_LFO_LEVEL_INDEX = 9;
		internal static int FREQ_LFO_LEVEL_INDEX = 10;
		internal static int AMP_LFO_FREQ_INDEX = 11;
		internal static int FREQ_LFO_FREQ_INDEX = 12;

		private void UserControl_Loaded(object sender, RoutedEventArgs e)
		{
			readParametersFromEngine();
		}
	}
}
