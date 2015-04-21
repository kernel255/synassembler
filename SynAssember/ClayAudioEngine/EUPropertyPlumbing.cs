using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SynthPanels;

namespace ClayAudioEngine
{
    public class EUPropertyPlumbing
    {
        public static bool writeEUProperty(Int32 euId, String propertyName, String value)
        {

            return false;
        }

        public static int writeEUDoubleProperty(int euId, int propertyIndex, Double value)
        {
			return AudioEngineWrapper.getDefault().writeEUDProperty(euId, 0, value);
        }

		public static int writeEUIntegerProperty(int euId, int propertyIndex, int value)
		{
			return AudioEngineWrapper.getDefault().writeEUIProperty(euId, propertyIndex, value);
		}
    }
}
