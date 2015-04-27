using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SynthPanels;
using GeneralUtils;

namespace InputOutputSynthFactory
{
    class InputOutputSynthFactory : AbstractSynthPanelFactory, ISynthPanelFactory
    {
        private const String DIRECTSOUND_NAME = "DirectSound Output";
        private const String MIDIIN_NAME = "MIDI Input";
        private const String POLYTRACKKEY_NAME = "Polytrack Keyboard";
		private const String PCKEYBOARD_NAME = "PC Keyboard";

		public override void init(SynthDelegateHolder delHold, Facilities facilities)
        {
            base.init(delHold, facilities);
        }

        public override string getEUFactoryName()
        {
            return "Input Output";
        }

        public override string[] getEUNames()
        {
            return new String[] { DIRECTSOUND_NAME };
        }

        public override ISynthPanel createSynthPanel(string name, int id)
        {
            switch (name)
            {
                case DIRECTSOUND_NAME:
                    return new DirectSoundOutputWrapper(id, base.deleHolder, facilities);
                case MIDIIN_NAME:
                    return new MIDIInputWrapper(id, base.deleHolder, facilities);
                case POLYTRACKKEY_NAME:
                    return new PolyTrackKeyboardWrapper(id, base.deleHolder, facilities);
				case PCKEYBOARD_NAME:
					return new PCKeyboardWrapper(id, deleHolder, facilities);
                default:
                    return null;
            }
        }

    }
}
