
using System;

namespace ClayAudioEngine
{

    public class ElaborationUnitProperty
    {
        public enum PropertyType
        {
            C_Integer = 0,
            C_Double = 1,
            C_String = 2,
            C_StringArray = 3,
            C_MIDINote = 4,
            C_Gain = 5,
            C_None = 100
        }

        public static PropertyType mapIntToPropertyType(int type)
        {
            switch (type)
            {
                case 0:
                    return PropertyType.C_Integer;
                case 1:
                    return PropertyType.C_Double;
                case 2:
                    return PropertyType.C_String;
                case 3:
                    return PropertyType.C_StringArray;
                case 4:
                    return PropertyType.C_MIDINote;
                case 5:
                    return PropertyType.C_Gain;
                default:
                    return PropertyType.C_None;
            }
        }

        public int m_Id;
        public String m_Name;
        public String m_LowRange;
        public String m_HighRange;
        public PropertyType m_Type;
        public ElaborationUnitProperty(String name, int id)
        {
            m_Name = name;
            m_Id = id;
        }
    }

}