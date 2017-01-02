using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SynthPanels;

namespace ClayAudioEngine
{
    public class EUPropertyPlumbing
    {
        public static int writeEUProperty(int euId, int propertyIndex, String value)
        {
			return AudioEngineWrapper.getDefault().writeEUProperty(euId, propertyIndex, value);
        }

		public static String readEUProperty(int euId, int propertyIndex)
		{
			return AudioEngineWrapper.getDefault().readEUProperty(euId, propertyIndex);
		}

        public static int writeEUDoubleProperty(int euId, int propertyIndex, Double value)
        {
			return AudioEngineWrapper.getDefault().writeEUDProperty(euId, propertyIndex, value);
        }

		public static int writeEUIntegerProperty(int euId, int propertyIndex, int value)
		{
			return AudioEngineWrapper.getDefault().writeEUIProperty(euId, propertyIndex, value);
		}

		public static int writeEUBoolProperty(int euId, int propertyIndex, bool value)
		{
			return AudioEngineWrapper.getDefault().writeEUBProperty(euId, propertyIndex, value);
		}

		public static double readEUDoubleProperty(int euId, int propertyIndex)
		{
			return AudioEngineWrapper.getDefault().readEUDProperty(euId, propertyIndex);
		}

		public static int readEUIntegerProperty(int euId, int propertyIndex)
		{
			return AudioEngineWrapper.getDefault().readEUIProperty(euId, propertyIndex);
		}

		public static bool readEUBoolProperty(int euId, int propertyIndex)
		{
			return AudioEngineWrapper.getDefault().readEUBProperty(euId, propertyIndex);
		}
    }
}
