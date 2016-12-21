using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SynthPanels;
using GeneralUtils;

namespace BasicEUSynthFactory
{
    class BasicEUSynthFactory : AbstractSynthPanelFactory, ISynthPanelFactory
    {
        private const String OSC_NAME = "Oscillator";
        private const String PCMWAVE_NAME = "PCM Waveform";
        private const String MIDISEQ_NAME = "MIDI Sequence Player";
		private const String MIXER_NAME = "Mixer";

		public override void init(SynthDelegateHolder deleHold, Facilities facilities)
        {
			base.init(deleHold, facilities);
			PCMWaveformWrapper.InitWaveNames(facilities.ResourcesFolder);
        }

		public override string getEUFactoryName()
		{
			return "BasicEU";
		}

		public override string[] getEUNames()
		{
			return new String[] { OSC_NAME };
		}

		public override ISynthPanel createSynthPanel(string name, int id)
		{
			switch (name)
			{
				case OSC_NAME:
					return new OscillatorPanelWrapper(id, base.deleHolder, facilities);
				case PCMWAVE_NAME:
					return new PCMWaveformWrapper(id, base.deleHolder, facilities);
				case MIDISEQ_NAME:
					return new MIDISequencePlayerWrapper(id, base.deleHolder, facilities);
				case MIXER_NAME:
					return new MixerPanelWrapper(id, base.deleHolder, facilities);
				default:
					return null;
			}
		}
	}
}
