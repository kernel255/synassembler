using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using SynthPanels;
using System.IO;
using System.Windows.Controls;
using System.Windows.Input;


namespace ClayAudioEngine
{

    public partial class AudioEngineWrapper
    {

        private static int m_NumFactories = 0;
        private static List<ElaborationUnitFactory> m_Factories;


        public static int init(StringBuilder path, Int32 hwnd, int samplingFrequence, int bitResolution, int numChannels)
        {
            String currentFolder = Directory.GetCurrentDirectory();
            // Try to init engine
            int result = -1;
            try
            {
                result = initEngine(path, hwnd, samplingFrequence, bitResolution, numChannels);
            }
            catch (Exception)
            {
                return result;
            }

            try
            {
                queryEUFactories();
                Directory.SetCurrentDirectory(currentFolder);
                euPlumbing = initPlumbing();
                querySynthPanels(path);
            }
            catch (Exception)
            {
                releaseEngine();
                return -1;
            }
            return result;
        }

        private static EUPropertyPlumbing initPlumbing()
        {
            EUPropertyPlumbing plumbing = new EUPropertyPlumbing();

            return plumbing;
        }

        private static void querySynthPanels(StringBuilder path)
        {
            SynthPanelManager.WriteEUProperty euPlumb = new SynthPanelManager.WriteEUProperty(EUPropertyPlumbing.writeEUProperty);
            SynthPanelManager.WriteEUDoubleProperty euDoublePlumb = new SynthPanelManager.WriteEUDoubleProperty(EUPropertyPlumbing.writeEUDoubleProperty);

            List<String> requestedFactories = new List<string>();
            foreach(ElaborationUnitFactory factory in m_Factories)
            {
                requestedFactories.Add(factory.getName());
            }

            SynthPanelManager.getDefault().initSynthPanelsFactories(path, requestedFactories, euPlumb, euDoublePlumb);
        }

        public static void release()
        {
            releaseEngine();
        }

        public static void _setHwnd(Int32 hwnd)
        {
            setHwndEngine(hwnd);
        }

        public static int getNumFactories()
        {
            m_NumFactories = getFactoryNumber();
            return m_NumFactories;
        }
        public static String getFactoryName(int factoryIndex)
        {
            if (factoryIndex > m_NumFactories)
                return "";
            else
                return getNthFactoryName(factoryIndex);
        }

        static public IList<ElaborationUnitFactory> getFactories()
        {
            return m_Factories;
        }

        public static int createNewAlgorithm()
        {
            return createAlgorithm();
        }

		public static int addElaborationUnit(int algoId, int euId)
		{
			return addElaborationUnitToAlgorithm(algoId, euId);
		}

        private const int PHYSICAL_CATEGORY = 1;
        private const int VIRTUAL_CATEGORY = 0;

        public static int createNewVirtualElaborationUnit(int factoryIndex, int euIndex)
        {
            return createNewElaborationUnit(factoryIndex, VIRTUAL_CATEGORY, euIndex, 0);
        }

        public static int createNewPhysicalElaborationUnit(int factoryIndex, int euIndex, int euInstance, int physicalInstanceId)
        {
            return createElaborationUnit(factoryIndex, PHYSICAL_CATEGORY, euIndex, physicalInstanceId);
        }

        public static int createNewElaborationUnit(int factoryIndex, int euIndex, int euInstance, int physicalInstanceId)
        {
            //TODO: fix the category issue
            return createElaborationUnit(factoryIndex, 0, euIndex, physicalInstanceId);
        }

        public static String getElaborationUnitCreationError(int error)
        {
            return "";
        }

        public static int connectElaborationUnits(int algorithmId, int sourceId, int sourcePortIndex, int destinationId, int destinationPortIndex)
        {
            try
            {
                return connectElaboratioUnits(algorithmId, sourceId, sourcePortIndex, destinationId, destinationPortIndex);
            }
            catch (Exception)
            {
                releaseEngine();
                return -1;
            }
        }

        internal static int writeEUProperty(int elaborationUnitIndex, int propertyIndex, String valueStr)
        {
            setEUProperty(elaborationUnitIndex, propertyIndex, valueStr);
            return 0;
        }

        internal static int writeEUDProperty(int elaborationUnitIndex, int propertyIndex, double value)
        {
            return setEUDProperty(elaborationUnitIndex, propertyIndex, value);
        }

        public static EUPropertyPlumbing euPlumbing;

        public static ISynthPanel createNewPanel(String factoryName, String name, int id)
        {
            return SynthPanelManager.getDefault().createSynthPanel(factoryName, name, id);
        }

		internal static PCKeyboardProcessor keybProcessor = new PCKeyboardProcessor();

		public static byte[] FirePCKeyboardEvent(KeyEventArgs args)
		{
			byte[] msg = keybProcessor.ProcessPCKeyboardEvent(args);
			if(msg!=null)
			{
				sendMIDIMessage(keybProcessor.EUId, msg);
			}
			return msg;
		}
		
		public static int PlayAlgorithm(int id)
		{
			return playAlgorithm(id);
		}

		public static int StopAlgorithm(int id)
		{
			return stopAlgorithm(id);
		}
		
    }
}


