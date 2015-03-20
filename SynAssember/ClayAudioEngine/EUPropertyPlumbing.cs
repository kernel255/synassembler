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

        public static bool writeEUDoubleProperty(int euId, String propertyName, Double value)
        {
            AudioEngineWrapper.writeEUDProperty(euId, 0, value);
            /*
            // TODO: find a way to pass directly a double to C++
            String strValue = Convert.ToString(value);
            AudioEngineWrapper.writeEUProperty(euId,0,strValue);
             */
            return false;
        }
    }
}
