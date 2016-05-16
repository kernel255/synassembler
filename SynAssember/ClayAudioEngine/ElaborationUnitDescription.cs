
using System;
using System.Collections.Generic;

namespace ClayAudioEngine
{

    public class Port
    {
        public enum PortType { AUDIO, MIDI, UNKNOWN };
		public enum PortDirection { IN, OUT, INOUT, UNKNOWN };
        public Port(String name, int id, PortType type, PortDirection dir)
        {
            m_Name = name;
            m_Id = id;
            m_Type = type;
			m_Dir = dir;
        }
        public int m_Id;
        private String m_Name;
        public String Name
        {
            get { return m_Name; }
        }
        private PortType m_Type;
        public PortType Type
        {
            get { return m_Type; }
        }
		private PortDirection m_Dir;
		public PortDirection Direction
		{
			get { return m_Dir; }
		}
        public static PortType getTypeByString(String str)
        {
            switch (str)
            {
                case "AUDIO":
                    return Port.PortType.AUDIO;
                case "MIDI":
                    return Port.PortType.MIDI;
                default:
                    return Port.PortType.UNKNOWN;
            }
        }
        public override String ToString()
        {
            switch (m_Type)
            {
                case PortType.AUDIO:
                    return "AUDIO";
                case PortType.MIDI:
                    return "MIDI";
                default:
                    return "UNKNOWN";
            }
        }
    }

    

    public class ElaborationUnitPhysicalInstance
    {
        private String euInstName;
        public String m_Name
        {
            get { return euInstName; }
            protected set { euInstName = value; }
        }
        int m_Id;
        public ElaborationUnitPhysicalInstance(String name, int id)
        {
            euInstName = name;
            m_Id = id;
        }
    }

	/**
	 * Represent the description of the ElaborationUnit and contains static information about the EU (ports, name, etc...)
	 */

    public class ElaborationUnitDescription
    {
        public int Id
        {
            get { return m_Id; }
        }
        public int m_Id;
        private String euName;
        public String Name
        {
            get { return euName; }
            protected set { euName = value; }
        }

        public IList<Port> getInputPorts()
        {
            return m_InputPorts;
        }

        public Port geNthInputPort(int index)
        {
            return m_InputPorts.ToArray()[index];
        }

        public void addInputPort(Port port)
        {
            m_InputPorts.Add(port);
        }

        private List<Port> m_InputPorts;

        public IList<Port> getOuputPorts()
        {
            return m_OutputPorts;
        }

        public Port getNthOutputPort(int index)
        {
            return m_OutputPorts.ToArray()[index];
        }

        public void addOutputPort(Port port)
        {
            m_OutputPorts.Add(port);
        }

        private List<Port> m_OutputPorts;

        public IList<ElaborationUnitProperty> getProperties()
        {
            return m_Properties;
        }

        private List<ElaborationUnitProperty> m_Properties;

        public IList<ElaborationUnitPhysicalInstance> getInstances()
        {
            return m_Instances;
        }
        public ElaborationUnitPhysicalInstance getNthInstance(int index)
        {
            return m_Instances[index];
        }
        public void addInstance(ElaborationUnitPhysicalInstance instance)
        {
            m_Instances.Add(instance);
        }
        private List<ElaborationUnitPhysicalInstance> m_Instances;

        public int m_NumInstances;
        public ElaborationUnitDescription(String name, int id)
        {
            m_Id = id;
            Name = name;
            m_InputPorts = new List<Port>();
            m_OutputPorts = new List<Port>();
            m_Instances = new List<ElaborationUnitPhysicalInstance>();
            m_Properties = new List<ElaborationUnitProperty>();
        }

        public int InstancesCount
        {
            get
            { return m_NumInstances; }
            set
            { m_NumInstances = value; }
        }

        private bool m_IsPhysical;
        public bool Physical
        {
            get { return m_IsPhysical; }
            set { m_IsPhysical = value; }
        }

		Dictionary<String, Boolean> capabilities = new Dictionary<string, bool>();

		public void SetCapabilitySupport(string capName)
		{
			capabilities.Add(capName, true);
		}

		public bool GetCapabilitySupport(string capName)
		{
			if (capabilities.ContainsKey(capName))
			{
				return capabilities[capName];
			}
			else
				return false;
		}

		public static string PCKEYBOARD_LISTENER_CAPNAME = "PCKeyboardCapName";

    }
}