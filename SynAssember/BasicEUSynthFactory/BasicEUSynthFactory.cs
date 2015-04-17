using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SynthPanels;

namespace BasicEUSynthFactory
{
    class BasicEUSynthFactory : AbstractSynthPanelFactory, ISynthPanelFactory
    {
        private const String OSC_NAME = "Oscillator";
        private const String PCMWAVE_NAME = "PCM Waveform";
        private const String MIDISEQ_NAME = "MIDI Sequence Player";

        public override void init(WriteEUDoubleProperty writeEUDProp)
        {
            base.init(writeEUDProp);
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
                    return new OscillatorPanelWrapper(id, base.writeEUDProp);
                case PCMWAVE_NAME:
                    return new PCMWaveformWrapper(id);
                case MIDISEQ_NAME:
                    return new MIDISequencePlayerWrapper(id);
                default:
                    return null;
            }
        }
    }
}
