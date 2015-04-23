using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SynthPanels;

namespace InputOutputSynthFactory
{
    class InputOutputSynthFactory : AbstractSynthPanelFactory, ISynthPanelFactory
    {
        private const String DIRECTSOUND_NAME = "DirectSound Output";
        private const String MIDIIN_NAME = "MIDI Input";
        private const String POLYTRACKKEY_NAME = "Polytrack Keyboard";
		private const String PCKEYBOARD_NAME = "PC Keyboard";

		public override void init(DelegateFactoryInterface delegInterf)
        {
            base.init(delegInterf);
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
                    return new DirectSoundOutputWrapper(id, base.deleHolder);
                case MIDIIN_NAME:
                    return new MIDIInputWrapper(id, base.deleHolder);
                case POLYTRACKKEY_NAME:
                    return new PolyTrackKeyboardWrapper(id, base.deleHolder);
				case PCKEYBOARD_NAME:
					return new PCKeyboardWrapper(id, deleHolder);
                default:
                    return null;
            }
        }

    }
}
