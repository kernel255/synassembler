
using System;
using System.Runtime.InteropServices;
using System.Text;
using System.Collections.Generic;

namespace ClayAudioEngine
{
    public partial class AudioEngineWrapper
    {
        public const int PHYSICAL_EU = 0;
        public const int VIRTUAL_EU = 1;
        public const int NUM_CATEGORIES = 2;

        public IList<ElaborationUnitFactory> getEUDescription()
        {
            if (m_Factories == null)
                queryEUFactories();
            return m_Factories;
        }

        public ElaborationUnitFactory getFactoryById(int id)
        {
            ElaborationUnitFactory[] facts = m_Factories.ToArray();
            for (int i = 0; i < m_Factories.Count; i++)
            {
                if (facts[i].Id == id)
                    return facts[i];

            }
            return null;
        }

        public static ElaborationUnitDescription getEUDescriptionById(ElaborationUnitFactory factory, int id)
        {
            foreach (var eu in factory.getEUList())
            {
                if (eu.Id == id)
                    return eu;
            }
            return null;
        }

        enum Category
        {
            PHYSICAL = 0,
            VIRTUAL = 1
        }

        public bool queryEUFactories()
        {
            m_Factories = new List<ElaborationUnitFactory>();
            m_NumFactories = getFactoryNumber();
            if (m_NumFactories == 0)
                return false;
            for (int factoryIndex = 0; factoryIndex < m_NumFactories; factoryIndex++)
            {
                StringBuilder factoryName = new StringBuilder(2000);
                getNthFactoryNameEx(factoryIndex, factoryName, 2000);
                ElaborationUnitFactory factory = new ElaborationUnitFactory(factoryName.ToString(), factoryIndex);
                m_Factories.Add(factory);
                for (int categoryIndex = 0; categoryIndex < NUM_CATEGORIES; categoryIndex++)
                {
                    int numEU = getElaborationUnitNumber(factoryIndex, categoryIndex);
                    for (int euIndex = 0; euIndex < numEU; euIndex++)
                    {
                        StringBuilder euName = new StringBuilder(2000);
                        getNthElaborationUnitNameEx(factoryIndex, categoryIndex, euIndex, euName, 2000);
                        //name = getNthElaborationUnitName(factoryIndex, categoryIndex, euIndex);
                        //getNthElaborationUnitName(factoryIndex, categoryIndex, euIndex);
                        ElaborationUnitDescription eu = new ElaborationUnitDescription(euName.ToString(), euIndex);
						/*DEBUG*/
						if (eu.Name.Equals("PC Keyboard"))
							eu.SetCapabilitySupport(ElaborationUnitDescription.PCKEYBOARD_LISTENER_CAPNAME);
						/*DEBUG*/
                        factory.addEU(eu);
                        int numInstances = getElaborationUnitInstanceNumber(factoryIndex, categoryIndex, euIndex);
                        eu.InstancesCount = numInstances;
                        if (numInstances == -1)
                            eu.Physical = false;
                        else
                            eu.Physical = true;
                        for (int instanceIndex = 0; instanceIndex < numInstances; instanceIndex++)
                        {
                            StringBuilder instName = new StringBuilder(100);
                            int res = getNthInstanceName(instName, 100, factoryIndex, categoryIndex, euIndex, instanceIndex);
                            ElaborationUnitPhysicalInstance euInst = new ElaborationUnitPhysicalInstance(instName.ToString(), instanceIndex);
                            eu.addInstance(euInst);
                        }
                        int numPorts = getNthElaborationUnitInputPortNumber(factoryIndex, categoryIndex, euIndex);
                        for (int portIndex = 0; portIndex < numPorts; portIndex++)
                        {
                            //name = getNthElaborationInputUnitPortName(factoryIndex, categoryIndex, euIndex, portIndex);
                            StringBuilder portName = new StringBuilder(2000);
                            getNthElaborationInputUnitPortNameEx(factoryIndex, categoryIndex, euIndex, portIndex, portName, 2000);
                            StringBuilder portType = new StringBuilder(1000);
                            int len = getNthElaborationUnitInputPortType(factoryIndex, categoryIndex, euIndex, portIndex, portType, 1000);
                            //String type = getNthElaborationUnitInputPortType(factoryIndex, categoryIndex, euIndex, portIndex);
                            Port port = new Port(portName.ToString(), portIndex, Port.getTypeByString(portType.ToString()), Port.PortDirection.IN);
                            eu.addInputPort(port);
                        }
                        numPorts = getNthElaborationUnitOutputPortNumber(factoryIndex, categoryIndex, euIndex);
                        for (int portIndex = 0; portIndex < numPorts; portIndex++)
                        {
                            //name = getNthElaborationUnitOutputPortName(factoryIndex, categoryIndex, euIndex, portIndex);
                            StringBuilder portName = new StringBuilder(2000);
                            getNthElaborationUnitOutputPortNameEx(factoryIndex, categoryIndex, euIndex, portIndex, portName, 2000);
                            StringBuilder portType = new StringBuilder(1000);
                            getNthElaborationUnitOutputPortType(factoryIndex, categoryIndex, euIndex, portIndex, portType, 1000);
                            Port port = new Port(portName.ToString(), portIndex, Port.getTypeByString(portType.ToString()), Port.PortDirection.OUT);
                            eu.addOutputPort(port);
                        }

                        int numProperties = getElaborationUnitPropertyNumber(factoryIndex, categoryIndex, euIndex);
                        for (int propertyIndex = 0; propertyIndex < numProperties; propertyIndex++)
                        {
                            StringBuilder propName = new StringBuilder(1000);
                            getNthEUPropertyName(factoryIndex, categoryIndex, euIndex, propertyIndex, propName, 1000);
                            ElaborationUnitProperty prop = new ElaborationUnitProperty(propName.ToString(), propertyIndex);
                            int propType = getPropertyType(factoryIndex, categoryIndex, euIndex, propertyIndex);
                            StringBuilder low, high;
                            low = new StringBuilder();
                            high = new StringBuilder();
                            getPropertyRanges(factoryIndex, categoryIndex, euIndex, propertyIndex, low, high, low.Capacity);
                            prop.m_LowRange = new String(low.ToString().ToCharArray());
                            prop.m_HighRange = new String(high.ToString().ToCharArray());
                            int type = getPropertyType(factoryIndex, categoryIndex, euIndex, propertyIndex);
                            prop.m_Type = ElaborationUnitProperty.mapIntToPropertyType(type);
                        }
                    }
                }

            }

            return true;
        }

        public int getFactoryIdByName(String name)
        {
            int numFact = m_Factories.Count;
            for (int factoryIndex = 0; factoryIndex < numFact; factoryIndex++)
            {
                ElaborationUnitFactory fact = m_Factories[factoryIndex];
                if(fact.getName().Equals(name))
                    return factoryIndex;
            }

            throw new Exception("Factory not found: {0}" + name);
        }

        public int getEUByName(String name, int factoryIndex)
        {
            ElaborationUnitFactory fact = m_Factories[factoryIndex];

            int numEU = fact.getEUList().Count;
            for (int i = 0; i < numEU; i++)
                if (fact.getEUList()[i].Name.Equals(name))
                    return i;

            throw new Exception("Elaboration Unit not found: {0}" + name);
        }

        // Engine init/release
        [DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        private static extern int initEngine(StringBuilder plugInFolder, Int32 hwnd, Int32 samplFreq, Int32 numBits, Int32 numChannels);
        [DllImport("AudioEngine.dll")]
        private static extern void releaseEngine();
        [DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void setHwndEngine(Int32 hwnd);

		// Logging facilities
		[DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
		private static extern int logX(StringBuilder msg);
		[DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
		private static extern int forceLogFlush();

		// Factories
		[DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int getFactoryNumber();
        [DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern String getNthFactoryName(int factoryIndex);

        [DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        private static extern int getNthFactoryNameEx(int factoryIndex, StringBuilder name, int bufferSize);

        // Elaboration Units
        [DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int getElaborationUnitNumber(int factoryIndex, int categoryIndex);
        [DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int getElaborationUnitInstanceNumber(int factoryIndex, int categoryIndex, int elaborationUnitIndex);
        [DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        private static extern int getNthInstanceName(StringBuilder name, int bufferSize, int factoryIndex, int categoryIndex, int elaborationUnitIndex, int instanceIndex);
        [DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern String getNthElaborationUnitName(int factoryIndex, int categoryIndex, int elaborationUnitIndex);
        [DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        private static extern int getNthElaborationUnitNameEx(int factoryIndex, int categoryIndex, int elaborationUnitIndex, StringBuilder buffer, int bufferSize);
        [DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int getNthElaborationUnitInputPortNumber(int factoryIndex, int categoryIndex, int elaborationUnitIndex);
        [DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int getNthElaborationInputUnitPortName(int factoryIndex, int categoryIndex, int elaborationUnitIndex, int portIndex);
        [DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        private static extern int getNthElaborationInputUnitPortNameEx(int factoryIndex, int categoryIndex, int elaborationUnitIndex, int portIndex, StringBuilder buffer, int bufferSize);
        [DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int getNthElaborationUnitOutputPortNumber(int factoryIndex, int categoryIndex, int elaborationUnitIndex);
        [DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int getNthElaborationUnitOutputPortName(int factoryIndex, int categoryIndex, int elaborationUnitIndex, int portIndex);
        [DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        private static extern int getNthElaborationUnitOutputPortNameEx(int factoryIndex, int categoryIndex, int elaborationUnitIndex, int portIndex, StringBuilder buffer, int bufferSize);
        [DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        private static extern int getNthElaborationUnitInputPortType(int factoryIndex, int categoryIndex, int elaborationUnitIndex, int portIndex, StringBuilder buffer, int bufferSize);
        [DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        private static extern int getNthElaborationUnitOutputPortType(int factoryIndex, int categoryIndex, int elaborationUnitIndex, int portIndex, StringBuilder buffer, int bufferSize);
        
        // Algorithm management
        [DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int createAlgorithm();
		[DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern int destroyAlgorithm(int id);
        [DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int createElaborationUnit(int factoryIndex, int categoryIndex, int elaborationUnitIndex, int physicalInstanceIndex);
        [DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int addElaborationUnitToAlgorithm(int algorithmIndex, int elaborationUnitId);
        [DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int connectElaboratioUnits(int algorithmIndex, int elaborationUnitSource, int sourcePortId, int elaborationUnitDestination, int destinationPortId);
        [DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int playAlgorithm(int algorithmId);
        [DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int stopAlgorithm(int algorithmId);
		[DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
		//private static extern int sendMIDIMessage(int euIndex, string midiMsg);
		private static extern int sendMIDIMessage(int euIndex, byte[] midiMsg);

        // Properties management
        [DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int getElaborationUnitPropertyNumber(int factoryIndex, int categoryIndex, int elaborationUnitIndex);

        // TODO: Convert this function to StringBuilder too!!!!
        [DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int getNthEUPropertyName(int factoryIndex, int categoryIndex, int elaborationUnitIndex,int propertyIndex, StringBuilder buffer, int bufferSize);
        
        [DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        private static extern int setEUProperty(int elaborationUnitIndex,	int propertyIndex, String valueStr);
        [DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        private static extern int getEUProperty(int elaborationUnitIndex, int propertyIndex, StringBuilder value, int bufferSize);
        [DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int getPropertyType(int factoryIndex, int categoryIndex, int elaborationUnitIndex, int propertyIndex);
        [DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int getPropertyRanges(int factoryIndex, int categoryIndex, int elaborationUnitIndex, int propertyIndex, StringBuilder lowRange, StringBuilder highRange, int bufferSize);
        [DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int setEUDProperty(int elaborationUnitIndex, int propertyIndex, double value);
		[DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern int setEUIProperty(int elaborationUnitIndex, int propertyIndex, int value);
		[DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern int setEUBProperty(int elaborationUnitIndex, int propertyIndex, bool value);

		[DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl)]
		unsafe private static extern int getEUDProperty(int elaborationUnitIndex, int propertyIndex, double* value);
		[DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl)]
		unsafe private static extern int getEUIProperty(int elaborationUnitIndex, int propertyIndex, int* value);
		[DllImport("AudioEngine.dll", CallingConvention = CallingConvention.Cdecl)]
		unsafe private static extern int getEUBProperty(int elaborationUnitIndex, int propertyIndex, bool* value);
    }

}