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
		WriteEUBoolProperty writeBProp;
		ReadEUBoolProperty readBProp;

		Facilities facilities;

		int id;

		public MixerPanel(int id, SynthDelegateHolder deleHolder, Facilities facilities)
		{
			InitializeComponent();
			this.facilities = facilities;

			OutLevel.setOwner(this);
			In0Level.setOwner(this);
			In1Level.setOwner(this);
			In2Level.setOwner(this);
			In3Level.setOwner(this);

			MuteI0.SetOwner(this);
			MuteI1.SetOwner(this);
			MuteI2.SetOwner(this);
			MuteI3.SetOwner(this);
			MuteO0.SetOwner(this);

			this.facilities = facilities;

			OutLevel.SliderChangedEvent += new BasicSlider.SliderChanged(OutSlider_SliderChangeEvent);
			In0Level.SliderChangedEvent += new BasicSlider.SliderChanged(Input0Slider_SliderChangeEvent);
			In1Level.SliderChangedEvent += new BasicSlider.SliderChanged(Input1Slider_SliderChangeEvent);
			In2Level.SliderChangedEvent += new BasicSlider.SliderChanged(Input2Slider_SliderChangeEvent);
			In3Level.SliderChangedEvent += new BasicSlider.SliderChanged(Input3Slider_SliderChangeEvent);

			MuteO0.SwitchChangeEvent += new OnOffLedButton.SwitchChanged(OutMute_ChangedEvent);
			MuteI0.SwitchChangeEvent += new OnOffLedButton.SwitchChanged(In0Mute_ChangedEvent);
			MuteI1.SwitchChangeEvent += new OnOffLedButton.SwitchChanged(In1Mute_ChangedEvent);
			MuteI2.SwitchChangeEvent += new OnOffLedButton.SwitchChanged(In2Mute_ChangedEvent);
			MuteI3.SwitchChangeEvent += new OnOffLedButton.SwitchChanged(In3Mute_ChangedEvent);

			writeDProp = deleHolder.writeEUDProp;
			readDProp = deleHolder.readEUDprop;
			writeBProp = deleHolder.writeEUBProp;
			readBProp = deleHolder.readEUBProp;

			this.id = id;
		}

		public static void OutSlider_SliderChangeEvent(Object o, Double level)
		{
			MixerPanel mixerPanel = (MixerPanel)o;
			mixerPanel.writeDProp(mixerPanel.id, OUT_LEVEL_PROPERTY_INDEX, level);
			mixerPanel.facilities.ChangedAlgorithm.algorithmChanged();
		}

		public static void Input0Slider_SliderChangeEvent(Object o, Double level)
		{
			InputSlider_SliderChangeEvent(o, level, IN0_LEVEL_PROPERTY_INDEX);
		}
		public static void Input1Slider_SliderChangeEvent(Object o, Double level)
		{
			InputSlider_SliderChangeEvent(o, level, IN1_LEVEL_PROPERTY_INDEX);
		}
		public static void Input2Slider_SliderChangeEvent(Object o, Double level)
		{
			InputSlider_SliderChangeEvent(o, level, IN2_LEVEL_PROPERTY_INDEX);
		}
		public static void Input3Slider_SliderChangeEvent(Object o, Double level)
		{
			InputSlider_SliderChangeEvent(o, level, IN3_LEVEL_PROPERTY_INDEX);
		}

		public static void InputSlider_SliderChangeEvent(Object o, Double level, int index)
		{
			MixerPanel mixerPanel = (MixerPanel)o;
			mixerPanel.writeDProp(mixerPanel.id, index, level);
			mixerPanel.facilities.ChangedAlgorithm.algorithmChanged();
		}

		public static void OutMute_ChangedEvent(Object o, bool on)
		{
			MixerPanel mixerPanel = (MixerPanel)o;
			mixerPanel.writeBProp(mixerPanel.id, OUT_MUTE_PROPERTY_INDEX, on);
			mixerPanel.facilities.ChangedAlgorithm.algorithmChanged();
		}
		public static void In0Mute_ChangedEvent(Object o, bool on)
		{
			MixerPanel mixerPanel = (MixerPanel)o;
			mixerPanel.writeBProp(mixerPanel.id, IN0_MUTE_PROPERTY_INDEX, on);
			mixerPanel.facilities.ChangedAlgorithm.algorithmChanged();
		}
		public static void In1Mute_ChangedEvent(Object o, bool on)
		{
			MixerPanel mixerPanel = (MixerPanel)o;
			mixerPanel.writeBProp(mixerPanel.id, IN1_MUTE_PROPERTY_INDEX, on);
			mixerPanel.facilities.ChangedAlgorithm.algorithmChanged();
		}
		public static void In2Mute_ChangedEvent(Object o, bool on)
		{
			MixerPanel mixerPanel = (MixerPanel)o;
			mixerPanel.writeBProp(mixerPanel.id, IN2_MUTE_PROPERTY_INDEX, on);
			mixerPanel.facilities.ChangedAlgorithm.algorithmChanged();
		}
		public static void In3Mute_ChangedEvent(Object o, bool on)
		{
			MixerPanel mixerPanel = (MixerPanel)o;
			mixerPanel.writeBProp(mixerPanel.id, IN3_MUTE_PROPERTY_INDEX, on);
			mixerPanel.facilities.ChangedAlgorithm.algorithmChanged();
		}


		public MixerPanel()
		{
			InitializeComponent();
		}

		internal void readParametersFromEngine()
		{
			double dOutLevel = readDProp(id, OUT_LEVEL_PROPERTY_INDEX);
			OutLevel.NormalizedLevelValue = dOutLevel;
			double dInLevel;
			dInLevel = readDProp(id, IN0_LEVEL_PROPERTY_INDEX);
			In0Level.LevelValue = dInLevel;
			dInLevel = readDProp(id, IN1_LEVEL_PROPERTY_INDEX);
			In1Level.LevelValue = dInLevel;
			dInLevel = readDProp(id, IN2_LEVEL_PROPERTY_INDEX);
			In2Level.LevelValue = dInLevel;
			dInLevel = readDProp(id, IN3_LEVEL_PROPERTY_INDEX);
			In3Level.LevelValue = dInLevel;

			bool bMute = readBProp(id, OUT_MUTE_PROPERTY_INDEX);
			MuteO0.SwitchOn = bMute;
			bMute = readBProp(id, IN0_MUTE_PROPERTY_INDEX);
			MuteI0.SwitchOn = bMute;
			bMute = readBProp(id, IN1_MUTE_PROPERTY_INDEX);
			MuteI1.SwitchOn = bMute;
			bMute = readBProp(id, IN2_MUTE_PROPERTY_INDEX);
			MuteI2.SwitchOn = bMute;
			bMute = readBProp(id, IN3_MUTE_PROPERTY_INDEX);
			MuteI3.SwitchOn = bMute;
		}

		internal System.Drawing.Rectangle getRectangle()
		{
			return new System.Drawing.Rectangle(0, 0, 280, 160);
		}

		internal static int OUT_LEVEL_PROPERTY_INDEX = 0;
		internal static int IN0_LEVEL_PROPERTY_INDEX = 1;
		internal static int IN1_LEVEL_PROPERTY_INDEX = 2;
		internal static int IN2_LEVEL_PROPERTY_INDEX = 3;
		internal static int IN3_LEVEL_PROPERTY_INDEX = 4;

		internal static int OUT_MUTE_PROPERTY_INDEX = 5;
		internal static int IN0_MUTE_PROPERTY_INDEX = 6;
		internal static int IN1_MUTE_PROPERTY_INDEX = 7;
		internal static int IN2_MUTE_PROPERTY_INDEX = 8;
		internal static int IN3_MUTE_PROPERTY_INDEX = 9;

		internal static int NUM_INPUTS = 4;
	}
}
