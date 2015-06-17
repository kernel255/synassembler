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
			return AudioEngineWrapper.getDefault().writeEUDProperty(euId, propertyIndex, value);
        }

		public static int writeEUIntegerProperty(int euId, int propertyIndex, int value)
		{
			return AudioEngineWrapper.getDefault().writeEUIProperty(euId, propertyIndex, value);
		}

		public static double readEUDoubleProperty(int euId, int propertyIndex)
		{
			return AudioEngineWrapper.getDefault().readEUDProperty(euId, propertyIndex);
		}

		public static int readEUIntegerProperty(int euId, int propertyIndex)
		{
			return AudioEngineWrapper.getDefault().readEUIProperty(euId, propertyIndex);
		}
    }
}
