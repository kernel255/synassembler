
// AudioEngine.cpp : Defines the exported functions for the DLL application.

//#include "stdafx.h"

#ifdef WIN32
#include <windows.h>
#endif //WIN32

#include <stdlib.h>

#include "AudioEngine.h"
#include "ElaborationUnits\Algorithm.h"
#include "ElaborationUnits\ElaborationUnitPort.h"
#include "ElaborationUnits\EUKind.h"
#include "Facilities\General\Logger.h"
#include "ElaborationUnits\MIDIChannelMessage.h"

using namespace std;

static AudioEngine* audioEngine = NULL;
//static Logger* logger;

AudioEngine::AudioEngine(const wchar_t* folderName, ModuleServices* _pServices)
{
	pModuleServices = _pServices;
	factoryRetriever = new FactoryRetriever(folderName, pModuleServices);
	m_bPlaying = false;
	currentAlgorithmId = 0;
	currentElaborationUnitId = 0;
}

AudioEngine::~AudioEngine()
{
	cleanUpAlgorithms();
	pModuleServices->pLogger->writeLine("Releasing factories");


	//factoryRetriever->freeFactories();
	factoryRetriever->releaseFactories();



	delete factoryRetriever;
	pModuleServices->pLogger->writeLine("Closing library");
	pModuleServices->pLogger->close();

	printf("Closing Log\n");
}

void AudioEngine::setSpecifiOSInfo(SpecificOSInfo* osInfo)
{
	memcpy(&(pModuleServices->specOSInfo), osInfo, sizeof(SpecificOSInfo));
}

std::vector<ElaborationUnitFactory*> AudioEngine::getFactories()
{
	return factoryRetriever->getFactories(this->pModuleServices);
}

int AudioEngine::createAlgorithm()
{
	Algorithm* algo = new Algorithm(pModuleServices, 0);
	int id = currentAlgorithmId++;
	mapIdToAlgorithms[id] = algo;
	AlgorithmProxy algoProxy(algo, id);
	algorithms.push_back(algoProxy);

	return id;
}

int AudioEngine::removeAlgorithm(int algorithmIndex)
{
	mapIdToAlgorithms.erase(algorithmIndex);
	std::vector<AlgorithmProxy>::iterator it;
	for (it=algorithms.begin(); it != algorithms.end(); ++it)
	{
		if (it->id == algorithmIndex)
		{
			delete it->algo;
			algorithms.erase(it);
			break;
		}
	}

	return ALGORITHM_REMOVED;
}

void AudioEngine::cleanUpAlgorithms(void)
{
	std::vector<AlgorithmProxy>::iterator it;
	for(it=algorithms.begin();it!=algorithms.end();++it)
	{
		AlgorithmProxy algoProxy = *it;
		delete algoProxy.algo;
	}
	algorithms.clear();
}

Algorithm* AudioEngine::getAlgorithmById(int id)
{
	std::vector<AlgorithmProxy>::iterator it;
	for(it=algorithms.begin();it!=algorithms.end();++it)
	{
		AlgorithmProxy algoProxy = *it;
		if(algoProxy.id==id)
			return algoProxy.algo;
	}
	return NULL;
}

ElaborationUnit* AudioEngine::getElaborationUnitById(int id)
{
	std::map<int, ElaborationUnit*>::iterator it;

	it = mapIdToElaborationUnits.find(id);
	if (it == mapIdToElaborationUnits.end())
		return NULL;
	/*
	if(it==NULL)
		return NULL;
	else
		return it->second;
	*/
	return it->second;
}

int AudioEngine::createVirtualElaborationUnit(int factoryIndex, int elaborationUnitIndex, int instanceIndex)
{
	ElaborationUnitFactory* pFactory = audioEngine->factoryRetriever->getNthFactory(factoryIndex);
	if(pFactory==NULL)
		return -1;
	ElaborationUnit* elaborationUnit = pFactory->createVirtualElaborationUnit(elaborationUnitIndex);
	if(elaborationUnit==NULL)
		return -1;
	int euId = currentElaborationUnitId++;
	//Store the pointer with Id
	mapIdToElaborationUnits[euId] = elaborationUnit;
	//Store the id of the factory
	mapEUIdToFactoryId[euId] = factoryIndex;

	return euId;
}

int AudioEngine::createPhysicalElaborationUnit(int factoryIndex, int elaborationUnitIndex, int instanceIndex)
{
	ElaborationUnitFactory* pFactory = audioEngine->factoryRetriever->getNthFactory(factoryIndex);
	if(pFactory==NULL)
		return -1;
	ElaborationUnit* elaborationUnit = pFactory->createPhysicalElaborationUnit(elaborationUnitIndex, instanceIndex);
	if(elaborationUnit==NULL)
		return -2;
	int euId = currentElaborationUnitId++;
	//Store the pointer with Id
	mapIdToElaborationUnits[euId] = elaborationUnit;
	//Store the id of the factory
	mapEUIdToFactoryId[euId] = factoryIndex;

	return euId;
}

int AudioEngine::addElaboratioUnitToAlgorithm(int algorithmIndex, int elaborationUnitIndex)
{
	Algorithm* algo;
	int factId;
	algo = getAlgorithmById(algorithmIndex);
	if(algo==NULL)
		return ALGORITHM_NOT_FOUND;
	std::map<int, Algorithm*>::iterator algoIt;
	algoIt = mapIdToAlgorithms.find(algorithmIndex);
	if(algoIt==mapIdToAlgorithms.end())
		return ALGORITHM_NOT_FOUND;
	ElaborationUnit* eu = getElaborationUnitById(elaborationUnitIndex);
	if(eu==NULL)
		return ELABORATIONUNIT_NOT_FOUND;
	map<int,int>::iterator it = mapEUIdToFactoryId.find(elaborationUnitIndex);
	if(it==mapEUIdToFactoryId.end())
		return FACTORY_NOT_FOUND;
	else
		factId = it->second;

	ElaborationUnitFactory* pFactory = factoryRetriever->getNthFactory(factId);
	if(pFactory==NULL)
		return FACTORY_NOT_FOUND;
	algo->AddElaborationUnit(eu,pFactory);

	return EU_ADDED_TO_ALGORITHM;
}

int AudioEngine::connectElaboratioUnits(int algorithmIndex, int elaborationUnitSource, int sourcePortId, 
		int elaborationUnitDestination, int destinationPortId)
{
	//Check algorithm
	Algorithm* algo;
	algo = getAlgorithmById(algorithmIndex);
	if(algo==NULL)
		return ALGORITHM_NOT_FOUND;
	//Check EU on connection
	ElaborationUnit *pEUSource, *pEUDest;
	pEUSource = getElaborationUnitById(elaborationUnitSource);
	if(pEUSource==NULL)
		return SOURCE_EU_NOT_FOUND;
	pEUDest = getElaborationUnitById(elaborationUnitDestination);
	if(pEUDest==NULL)
		return DEST_EU_NOT_FOUND;
	ElaborationUnitPort* pSourcePort = pEUSource->getNthOutputPort(sourcePortId);
	if(pSourcePort==NULL)
		return SOURCE_EU_PORT_NOT_FOUND;
	ElaborationUnitPort* pDestPort = pEUDest->getNthInputPort(destinationPortId);
	if(pDestPort==NULL)
		return DEST_EU_PORT_NOT_FOUND;
	//Check port kind
	ElaborationUnitPort::e_PortKind kind = pSourcePort->getKind();
	if(pDestPort->getKind()!=kind)
		return PORT_KIND_DIFFERS;
	//Check direction
	if(pSourcePort->getDirection()==ElaborationUnitPort::INPUT_PORT)
		return SOURCE_PORT_IS_INPUT;
	if(pDestPort->getDirection()==ElaborationUnitPort::OUTPUT_PORT)
		return DESTINATION_PORT_IS_OUTPUT;
	//Finally connect
	switch(kind)
	{
	case ElaborationUnitPort::AUDIO_PORT:
		{
			pEUDest->setInputEU(pDestPort,pEUSource,pSourcePort);
			return 0;
		}
		break;
	case ElaborationUnitPort::MIDI_PORT:
		{
			pEUSource->setOutputEU(pSourcePort,pEUDest,pDestPort);
			return 0;
		}
		break;
	default:
		return UNKNOWN_PORT_KIND;
	}

	/*
	if(pSourcePort->getDirection()==ElaborationUnitPort::INPUT_PORT)
	{
		pEUSource->setInputEU(pSourcePort,pEUDest,pDestPort);
	}
	else
	{
		if(pDestPort->getDirection()==ElaborationUnitPort::OUTPUT_PORT)
		{
			pEUSource->setOutputEU(pSourcePort,pEUDest,pDestPort);
			pEUDest->setInputEU(pDestPort,pEUSource,pSourcePort);
		}
		if(pDestPort->getDirection()==ElaborationUnitPort::INPUT_PORT)
		{
			pEUDest->setInputEU(pDestPort,pEUSource,pSourcePort);
		}
	}

	return 0;
	*/
}

int AudioEngine::playAlgorithm(int algorithmId)
{
	Algorithm* algo;
	algo = getAlgorithmById(algorithmId);
	if(algo==NULL)
	{
		std::string str = "Invalid algorithm Id";
		pModuleServices->pLogger->writeLine(str.c_str());
		return INVALID_ALGORITHM_ID;
	}
	m_bPlaying = true;
	pModuleServices->timeBase->start();
	algo->play();
	pModuleServices->pLogger->writeLine("Playing algorithm");

	return 0;
}

int AudioEngine::stopAlgorithm(int algorithmId)
{
	Algorithm* algo;
	algo = getAlgorithmById(algorithmId);
	if(algo==NULL)
	{
		std::string str = "Invalid algorithm Id";
		pModuleServices->pLogger->writeLine(str.c_str());
		return INVALID_ALGORITHM_ID;
	}
	m_bPlaying = false;
	pModuleServices->timeBase->stop();
	algo->stop();
	pModuleServices->pLogger->writeLine("Stopping algorithm");


	return 0;
}

int AudioEngine::setPropertyByName(int factoryIndex, int categoryIndex, int elaborationUnitIndex, int propertyIndex, const char* value)
{
	return 0;
}

const char* AudioEngine::getPropertyByName(int factoryIndex, int categoryIndex, int elaborationUnitIndex, int propertyIndex)
{
	return NULL;
}

int AudioEngine::setEUProperty(int elaborationUnitIndex, int propertyIndex, const wchar_t* value)
{
	ElaborationUnit* pEU = getElaborationUnitById(elaborationUnitIndex);
	if(pEU!=NULL)
	{
		return pEU->setProperty(propertyIndex, value);
	}
	else
		return 0;
}

int AudioEngine::getEUDProperty(int elaborationUnitIndex, int propertyIndex, double *value)
{
	ElaborationUnit* pEU = getElaborationUnitById(elaborationUnitIndex);
	if(pEU!=NULL)
	{
		return pEU->getDProperty(propertyIndex, value);
	}
	else
		return 0;
}

int AudioEngine::setEUDProperty(int elaborationUnitIndex, int propertyIndex, double value)
{
	ElaborationUnit* pEU = getElaborationUnitById(elaborationUnitIndex);
	if(pEU!=NULL)
	{
		return pEU->setDProperty(propertyIndex, value);
	}
	else
		return 0;
}

int AudioEngine::setEUIProperty(int elaborationUnitIndex, int propertyIndex, int value)
{
	ElaborationUnit* pEU = getElaborationUnitById(elaborationUnitIndex);
	if (pEU != NULL)
	{
		return pEU->setIProperty(propertyIndex, value);
	}
	else
	{
		return 0;
	}
}

int AudioEngine::getEUIProperty(int elaborationUnitIndex, int propertyIndex, int* value)
{
	ElaborationUnit* pEU = getElaborationUnitById(elaborationUnitIndex);
	if (pEU != NULL)
	{
		return pEU->getIProperty(propertyIndex, value);
	}
	else
	{
		return 0;
	}
}

int AudioEngine::setEUBProperty(int elaborationUnitIndex, int propertyIndex, bool value)
{
	ElaborationUnit* pEU = getElaborationUnitById(elaborationUnitIndex);
	if (pEU != NULL)
	{
		return pEU->setBProperty(propertyIndex, value);
	}
	else
	{
		return 0;
	}
}

int AudioEngine::getEUBProperty(int elaborationUnitIndex, int propertyIndex, bool* value)
{
	ElaborationUnit* pEU = getElaborationUnitById(elaborationUnitIndex);
	if (pEU != NULL)
	{
		return pEU->getBProperty(propertyIndex, value);
	}
	else
	{
		return 0;
	}
}

int AudioEngine::getEUProperty(int elaborationUnitIndex, int propertyIndex, wchar_t* value, int bufferSize)
{
	ElaborationUnit* pEU = getElaborationUnitById(elaborationUnitIndex);
	if(pEU!=NULL)
	{
		return pEU->getProperty(propertyIndex,value,bufferSize);
	}
	else
		return 0;
}

char* AudioEngine::checkAlgorithmValidity(int algorithmIndex) 
{
	Algorithm* algo;
	algo = getAlgorithmById(algorithmIndex);
	if(algo==NULL)
	{
		std::string str = "Invalid algorithm Id";
		pModuleServices->pLogger->writeLine(str.c_str());
		return "";
	}
	else
		return algo->checkAlgorithmValidity();
}



/*
extern "C" __declspec( dllexport ) void initEngine(const wchar_t* folderName, SpecificOSInfo* osInfo, EngineSettings* pEngSettings)
{
	ModuleServices* moduleServices = new ModuleServices(AUDIO_ENGINE_LOGIFLENAME, pEngSettings);
	moduleServices->pLogger->writeLine("AudioEngine library started");
	memcpy(&moduleServices->specOSInfo,osInfo,sizeof(SpecificOSInfo));
	if(audioEngine==NULL)
		audioEngine = new AudioEngine(folderName, moduleServices);
}
*/


extern "C" __declspec( dllexport ) int initEngine(const wchar_t* folderName, int hwnd, int samplingFrequence, int bitResolution, int numChannels)
{
	SpecificOSInfo specOSInfo;
	specOSInfo.hwnd = (HWND) hwnd;
	EngineSettings* pEngSettings = new EngineSettings(samplingFrequence, bitResolution, numChannels);
	ModuleServices* moduleServices = new ModuleServices(AUDIO_ENGINE_LOGIFLENAME, pEngSettings);
	moduleServices->pLogger->writeLine("AudioEngine library started");
	memcpy(&moduleServices->specOSInfo,&specOSInfo,sizeof(SpecificOSInfo));

	if(audioEngine==NULL)
		audioEngine = new AudioEngine(folderName, moduleServices);

	return 0;
}

extern "C" __declspec( dllexport ) int logX(const wchar_t* message)
{
	if (audioEngine == NULL)
	{
		return -1;
	} 
	else 
	{
		return audioEngine->log(message);
	}
}

extern "C" __declspec( dllexport ) int forceLogFlush(void)
{
	if (audioEngine == NULL)
	{
		return -1;
	}
	else
	{
		return audioEngine->forceLogFlush();
	}
}

extern "C" __declspec(dllexport) int reopenLog(void)
{
	if (audioEngine == NULL)
	{
		return -1;
	}
	else
	{
		return audioEngine->reopenLog();
	}
}

extern "C" __declspec( dllexport ) void releaseEngine(void)
{
	delete audioEngine;
}

extern "C" __declspec( dllexport ) void setHwndEngine(int hwnd)
{
	SpecificOSInfo specOSInfo;
	specOSInfo.hwnd = (HWND) hwnd;
	audioEngine->setSpecifiOSInfo(&specOSInfo);
}

extern "C" __declspec( dllexport ) AudioEngine* getAudioEngine(wchar_t* folderName)
{
	/*
	if(audioEngine==NULL)
		audioEngine = new AudioEngine(folderName);
	return audioEngine;
	*/
	return NULL;
}

extern "C" __declspec( dllexport ) void releaseAudioEngine(void)
{
	delete audioEngine;
}

extern "C" __declspec( dllexport ) int getFactoryNumber()
{
	return audioEngine->factoryRetriever->getFactoriesNumber();
}

extern "C" __declspec( dllexport ) const char* getNthFactoryName(int factoryIndex)
{
	const char* factoryName = NULL;
	ElaborationUnitFactory* factory = audioEngine->factoryRetriever->getNthFactory(factoryIndex);
	if(factory!=NULL)
		factoryName = factory->getName();

	return factoryName;
}








extern "C" __declspec( dllexport ) int getNthFactoryNameEx(int factoryIndex, wchar_t* buffer, int bufferLen)
{
	const char* factoryName = NULL;
	ElaborationUnitFactory* factory = audioEngine->factoryRetriever->getNthFactory(factoryIndex);
	if(factory!=NULL)
		factoryName = factory->getName();
	else
		return 0;
	factoryName = factory->getName();
	size_t convChars;
	convChars = ::mbstowcs(buffer, factoryName, bufferLen);

	return convChars;
}







extern "C" __declspec( dllexport ) int getElaborationUnitNumber(int factoryIndex, int categoryIndex)
{
	ElaborationUnitFactory* factory = audioEngine->factoryRetriever->getNthFactory(factoryIndex);
	if(factory==NULL)
		return 0;
	if(categoryIndex==VIRTUAL_EU_CATEGORY)
		return factory->getVirtualEUKindNumber();
	else if(categoryIndex==PHYSICAL_EU_CATEGORY)
		return factory->getPhysicalEUKindNumber();
	else
		return 0;
}

extern "C" __declspec( dllexport ) int getElaborationUnitInstanceNumber(int factoryIndex, int categoryIndex, int elaborationUnitIndex)
{
	ElaborationUnitFactory* factory = audioEngine->factoryRetriever->getNthFactory(factoryIndex);
	if(factory==NULL)
		return 0;
	switch(categoryIndex)
	{
	case VIRTUAL_EU_CATEGORY:
		return INFINITE_INSTANCES;
		break;
	case PHYSICAL_EU_CATEGORY:
		return factory->getPhysicalEUInstanceNumber(elaborationUnitIndex);
		break;
	default:
		return 0;
	}
}

extern "C" __declspec( dllexport ) int getNthInstanceName(wchar_t* buffer, int bufferLen, int factoryIndex, int categoryIndex, int elaborationUnitIndex, int instanceIndex)
{
	ElaborationUnitFactory* factory = audioEngine->factoryRetriever->getNthFactory(factoryIndex);
	if(factory==NULL)
		return 0;
	if(categoryIndex==VIRTUAL_EU_CATEGORY)
	{
		return 0;
	}
	else if(categoryIndex==PHYSICAL_EU_CATEGORY)
	{
		const wchar_t* name = factory->getNthPhysicalEUInstanceName(elaborationUnitIndex,instanceIndex);
		if(wcslen(name)>bufferLen)
			return 0;
		else
		{
			wcscpy(buffer, name);
			return wcslen(name);
		}
	}
	else
		return 0;
}

#define SIZE 100
char debugBuffer[SIZE];

extern "C" __declspec( dllexport ) const char* getNthElaborationUnitName(int factoryIndex, int categoryIndex, int elaborationUnitIndex)
{
	memset(debugBuffer, 0, SIZE);
	sprintf(debugBuffer,"%d",rand() % 100 + 1);

	ElaborationUnitFactory* factory = audioEngine->factoryRetriever->getNthFactory(factoryIndex);
	if(factory==NULL)
		return 0;
	if(categoryIndex==VIRTUAL_EU_CATEGORY)
	{
		const EUKind* euKind = factory->getNthVirtualEUKind(elaborationUnitIndex);
		return euKind->getName();
	}
	else if(categoryIndex==PHYSICAL_EU_CATEGORY)
	{
		const EUKind* euKind = factory->getNthPhysicalEUKind(elaborationUnitIndex);
		return euKind->getName();
	}
	else
		return NULL;
}



extern "C" __declspec( dllexport ) int getNthElaborationUnitNameEx(int factoryIndex, int categoryIndex, int elaborationUnitIndex, wchar_t* buffer, int bufferLen)
{
	ElaborationUnitFactory* factory = audioEngine->factoryRetriever->getNthFactory(factoryIndex);
	if(factory==NULL)
		return 0;
	const char* euName;
	if(categoryIndex==VIRTUAL_EU_CATEGORY)
	{
		const EUKind* euKind = factory->getNthVirtualEUKind(elaborationUnitIndex);
		euName = euKind->getName();
	}
	else if(categoryIndex==PHYSICAL_EU_CATEGORY)
	{
		const EUKind* euKind = factory->getNthPhysicalEUKind(elaborationUnitIndex);
		euName = euKind->getName();
	}
	if(strlen(euName + 1)>bufferLen)
		return 0;

	size_t convChars;
	convChars = ::mbstowcs(buffer, euName, bufferLen);
	
	return convChars;
}


















extern "C" __declspec( dllexport ) int getNthElaborationUnitInputPortNumber(int factoryIndex, int categoryIndex, int elaborationUnitIndex)
{
	ElaborationUnitFactory* pFactory = audioEngine->factoryRetriever->getNthFactory(factoryIndex);
	if(pFactory==NULL)
		return 0;

	if(categoryIndex==VIRTUAL_EU_CATEGORY)
	{
		const EUKind* euKind = pFactory->getNthVirtualEUKind(elaborationUnitIndex);
		return euKind->getInputPortNumber();
	}
	else if(categoryIndex==PHYSICAL_EU_CATEGORY)
	{
		const EUKind* euKind = pFactory->getNthPhysicalEUKind(elaborationUnitIndex);
		return euKind->getInputPortNumber();
	}
	else
		return 0;
}

extern "C" __declspec( dllexport ) const char* getNthElaborationInputUnitPortName(int factoryIndex, int categoryIndex,
																				  int elaborationUnitIndex, int portIndex)
{
	ElaborationUnitFactory* pFactory = audioEngine->factoryRetriever->getNthFactory(factoryIndex);
	if(pFactory==NULL)
		return NULL;

	if(categoryIndex==VIRTUAL_EU_CATEGORY)
	{
		const EUKind* euKind = pFactory->getNthVirtualEUKind(elaborationUnitIndex);
		return euKind->getNthInputPortName(portIndex);
	}
	else if(categoryIndex==PHYSICAL_EU_CATEGORY)
	{
		const EUKind* euKind = pFactory->getNthPhysicalEUKind(elaborationUnitIndex);
		return euKind->getNthInputPortName(portIndex);
	}
	else
		return NULL;
}




extern "C" __declspec( dllexport ) int getNthElaborationInputUnitPortNameEx(int factoryIndex, int categoryIndex,
																				  int elaborationUnitIndex, int portIndex, wchar_t* buffer, int bufferLen)
{
	ElaborationUnitFactory* pFactory = audioEngine->factoryRetriever->getNthFactory(factoryIndex);
	if(pFactory==NULL)
		return NULL;
	const char* portName;

	if(categoryIndex==VIRTUAL_EU_CATEGORY)
	{
		const EUKind* euKind = pFactory->getNthVirtualEUKind(elaborationUnitIndex);
		portName = euKind->getNthInputPortName(portIndex);
	}
	else if(categoryIndex==PHYSICAL_EU_CATEGORY)
	{
		const EUKind* euKind = pFactory->getNthPhysicalEUKind(elaborationUnitIndex);
		portName = euKind->getNthInputPortName(portIndex);
	}
	else
		return 0;
	if(strlen(portName) + 1>bufferLen)
		return 0;

	size_t convChars;
	convChars = ::mbstowcs(buffer, portName, bufferLen);
	
	return convChars;

}





extern "C" __declspec( dllexport ) int getNthElaborationUnitOutputPortNumber(int factoryIndex, int categoryIndex, int elaborationUnitIndex)
{
	ElaborationUnitFactory* pFactory = audioEngine->factoryRetriever->getNthFactory(factoryIndex);
	if(pFactory==NULL)
		return 0;

	if(categoryIndex==VIRTUAL_EU_CATEGORY)
	{
		const EUKind* euKind = pFactory->getNthVirtualEUKind(elaborationUnitIndex);
		return euKind->getOutputPortNumber();
	}
	else if(categoryIndex==PHYSICAL_EU_CATEGORY)
	{
		const EUKind* euKind = pFactory->getNthPhysicalEUKind(elaborationUnitIndex);
		return euKind->getOutputPortNumber();
	}
	else
		return 0;
}

extern "C" __declspec( dllexport ) const char* getNthElaborationUnitOutputPortName(int factoryIndex, int categoryIndex, 
																				   int elaborationUnitIndex, int portIndex)
{
	ElaborationUnitFactory* pFactory = audioEngine->factoryRetriever->getNthFactory(factoryIndex);
	if(pFactory==NULL)
		return NULL;

	if(categoryIndex==VIRTUAL_EU_CATEGORY)
	{
		const EUKind* euKind = pFactory->getNthVirtualEUKind(elaborationUnitIndex);
		return euKind->getNthOutputPortName(portIndex);
	}
	else if(categoryIndex==PHYSICAL_EU_CATEGORY)
	{
		const EUKind* euKind = pFactory->getNthPhysicalEUKind(elaborationUnitIndex);
		return euKind->getNthOutputPortName(portIndex);
	}
	else
		return NULL;
}





extern "C" __declspec( dllexport ) int getNthElaborationUnitOutputPortNameEx(int factoryIndex, int categoryIndex, 
																				   int elaborationUnitIndex, int portIndex, wchar_t* buffer, int bufferLen)
{
	ElaborationUnitFactory* pFactory = audioEngine->factoryRetriever->getNthFactory(factoryIndex);
	if(pFactory==NULL)
		return 0;
	const char* portName;

	if(categoryIndex==VIRTUAL_EU_CATEGORY)
	{
		const EUKind* euKind = pFactory->getNthVirtualEUKind(elaborationUnitIndex);
		portName = euKind->getNthOutputPortName(portIndex);
	}
	else if(categoryIndex==PHYSICAL_EU_CATEGORY)
	{
		const EUKind* euKind = pFactory->getNthPhysicalEUKind(elaborationUnitIndex);
		portName = euKind->getNthOutputPortName(portIndex);
	}
	else
		return 0;
	if(strlen(portName) + 1>bufferLen)
	return 0;

	size_t convChars;
	convChars = ::mbstowcs(buffer, portName, bufferLen);
	
	return convChars;

}

extern "C" __declspec( dllexport ) int getNthElaborationUnitInputPortType(int factoryIndex, int categoryIndex, int elaborationUnitIndex,
	int portIndex, wchar_t* buffer, int bufferLen)
{
	ElaborationUnitFactory* pFactory = audioEngine->factoryRetriever->getNthFactory(factoryIndex);
	if(pFactory==NULL)
		return -1;

	if(categoryIndex==VIRTUAL_EU_CATEGORY)
	{
		const EUKind* euKind = pFactory->getNthVirtualEUKind(elaborationUnitIndex);
		//return euKind->getNthInputPortKind(portIndex);
		const char* portName = euKind->getNthInputPortKind(portIndex);
		size_t convChars;
		convChars = ::mbstowcs(buffer, portName, bufferLen);
		return convChars;
	}
	else if(categoryIndex==PHYSICAL_EU_CATEGORY)
	{
		const EUKind* euKind = pFactory->getNthPhysicalEUKind(elaborationUnitIndex);
		const char* portName = euKind->getNthInputPortKind(portIndex);
		size_t convChars;
		convChars = ::mbstowcs(buffer, portName, bufferLen);
		return convChars;
	}
	else
		return -1;
}

extern "C" __declspec( dllexport ) int getNthElaborationUnitOutputPortType(int factoryIndex, int categoryIndex, int elaborationUnitIndex,
	int portIndex, wchar_t* buffer, int bufferLen)
{
	ElaborationUnitFactory* pFactory = audioEngine->factoryRetriever->getNthFactory(factoryIndex);
	if(pFactory==NULL)
		return -1;

	if(categoryIndex==VIRTUAL_EU_CATEGORY)
	{
		const EUKind* euKind = pFactory->getNthVirtualEUKind(elaborationUnitIndex);
		const char* portName = euKind->getNthOutputPortKind(portIndex);
		size_t convChars;
		convChars = ::mbstowcs(buffer, portName, bufferLen);
		return convChars;
	}
	else if(categoryIndex==PHYSICAL_EU_CATEGORY)
	{
		const EUKind* euKind = pFactory->getNthPhysicalEUKind(elaborationUnitIndex);
		const char* portName = euKind->getNthOutputPortKind(portIndex);
		size_t convChars;
		convChars = ::mbstowcs(buffer, portName, bufferLen);
		return convChars;
	}
	else
		return NULL;
}


//##################################
//#######                    #######
//####### PROPERTIES CONTROL #######
//#######                    #######
//##################################

extern "C" __declspec( dllexport ) int getElaborationUnitPropertyNumber(int factoryIndex, int categoryIndex, 
				int elaborationUnitIndex)
{
	ElaborationUnitFactory* pFactory = audioEngine->factoryRetriever->getNthFactory(factoryIndex);
	if(pFactory==NULL)
		return NULL;

	if(categoryIndex==VIRTUAL_EU_CATEGORY)
	{
		const EUKind* euKind = pFactory->getNthVirtualEUKind(elaborationUnitIndex);
		return euKind->getPropertyNumber();
	}
	else if(categoryIndex==PHYSICAL_EU_CATEGORY)
	{
		const EUKind* euKind = pFactory->getNthPhysicalEUKind(elaborationUnitIndex);
		return euKind->getPropertyNumber();
	}
	else
		return NULL;
}



extern "C" __declspec( dllexport ) int getNthEUPropertyName(int factoryIndex, int categoryIndex, 
				int elaborationUnitIndex, int propertyIndex, wchar_t* buffer, int bufferSize)
{
	const EUKind* euKind = audioEngine->getEUKind(factoryIndex, categoryIndex,elaborationUnitIndex);
	if(euKind==NULL)
		return -1;
	else
	{
		const char* propName = euKind->getNthPropertyName(propertyIndex);
		size_t convChars;
		convChars = ::mbstowcs(buffer, propName, bufferSize);
		return convChars;
	}
		
}

extern "C" __declspec( dllexport ) int setEUProperty(int elaborationUnitIndex,	int propertyIndex, const wchar_t* value)
{
	return audioEngine->setEUProperty(elaborationUnitIndex, propertyIndex, value);
}

extern "C" __declspec( dllexport ) int setEUDProperty(int elaborationUnitIndex, int propertyIndex, double value)
{
	return audioEngine->setEUDProperty(elaborationUnitIndex, propertyIndex, value);
}

extern "C" __declspec( dllexport ) int getEUDProperty(int elaborationUnitIndex, int propertyIndex, double* value)
{
	return audioEngine->getEUDProperty(elaborationUnitIndex, propertyIndex, value);
}

extern "C" __declspec(dllexport) int setEUIProperty(int elaborationUnitIndex, int propertyIndex, int value)
{
	return audioEngine->setEUIProperty(elaborationUnitIndex, propertyIndex, value);
}

extern "C" __declspec(dllexport) int getEUIProperty(int elaborationUnitIndex, int propertyIndex, int* value)
{
	return audioEngine->getEUIProperty(elaborationUnitIndex, propertyIndex, value);
}

extern "C" __declspec(dllexport) int setEUBProperty(int elaborationUnitIndex, int propertyIndex, bool value)
{
	return audioEngine->setEUBProperty(elaborationUnitIndex, propertyIndex, value);
}

extern "C" __declspec(dllexport) int getEUBProperty(int elaborationUnitIndex, int propertyIndex, bool* value)
{
	return audioEngine->getEUBProperty(elaborationUnitIndex, propertyIndex, value);
}

extern "C" __declspec( dllexport ) int getEUProperty(int elaborationUnitIndex, int propertyIndex, wchar_t* value, int bufferSize)
{
	return audioEngine->getEUProperty(elaborationUnitIndex, propertyIndex, value, bufferSize);
}

extern "C" __declspec( dllexport ) int getPropertyType(int factoryIndex, int categoryIndex, int elaborationUnitIndex, int propertyIndex)
{
	const EUKind* euKind = audioEngine->getEUKind(factoryIndex, categoryIndex,elaborationUnitIndex);
	if(euKind==NULL)
		return -1;
	else
		return euKind->getPropertyType(propertyIndex);
}

 
extern "C" __declspec( dllexport ) int getPropertyRanges(int factoryIndex, int categoryIndex, int elaborationUnitIndex, int propertyIndex,
	 char* lowRange, char* highRange, int bufferSize)
{
	const EUKind* euKind = audioEngine->getEUKind(factoryIndex, categoryIndex,elaborationUnitIndex);
	if(euKind==NULL)
		return -1;
	else
		return euKind->getPropertyRanges(propertyIndex, lowRange, highRange, bufferSize);

}






extern "C" __declspec( dllexport ) int setPropertyByName(int factoryIndex, int categoryIndex, int elaborationUnitIndex, int propertyIndex, const char* value)
{
	ElaborationUnitFactory* pFactory = audioEngine->factoryRetriever->getNthFactory(factoryIndex);
	if(pFactory==NULL)
		return NULL;

	return NULL;
}

extern "C" __declspec( dllexport ) const char* getPropertyByName(int factoryIndex, int categoryIndex, int elaborationUnitIndex, int propertyIndex)
{
	ElaborationUnitFactory* pFactory = audioEngine->factoryRetriever->getNthFactory(factoryIndex);
	if(pFactory==NULL)
		return NULL;

	return NULL;
}

extern "C" __declspec(dllexport) int sendMIDIMessage(int euIndex, unsigned char* midiMsg)
{
	return audioEngine->sendMIDIMessage(euIndex, midiMsg);
}

int AudioEngine::sendMIDIMessage(int euIndex, unsigned char* strMsg)
{
	if (!m_bPlaying)
	{
		pModuleServices->pLogger->writeLine("Received MIDI message but not executed while not playing");
		return -1;
	}
	ElaborationUnit* pEU = getElaborationUnitById(euIndex);
	if (pEU == NULL)
		return -1;
	char buffer[10];
	memset(buffer, 0, 10);
	for (int i = 0; i < 3; i++)
	{
		sprintf(buffer+i*2, "%02x", strMsg[i]);
	}
	pModuleServices->pLogger->writeLine("Received MIDI message: %s", buffer);
	
	MIDIChannelMessage* midiMsg = getMIDIMsgByBytes(strMsg);
	pEU->receiveMIDIMessage(*midiMsg);
	
	return 0;
}

int getChannelByWChar(const wchar_t* ch)
{
	int iCh;
	
	int result = swscanf_s(ch, L"%1x", &iCh);
	if (result != 1)
		return -1;
	else
		return iCh;
}

int getKeyByWchar(const wchar_t* key)
{
	int iKey;
	int result = swscanf_s(key, L"%2x", &iKey);
	if (result != 1)
		return -1;
	else
		return iKey;
}

int getVelByWchar(const wchar_t* vel)
{
	int iVel;
	int result = swscanf_s(vel, L"%2x", &iVel);
	if (result != 1)
		return -1;
	else
		return iVel;
}


MIDIChannelMessage* AudioEngine::getMIDIMsgByBytes(unsigned char* buffer)
{
	byte NOTE_ON = 0x90;
	byte NOTE_OFF = 0x80;
	MIDIChannelMessage* msg = new MIDIChannelMessage();
	byte nibble = buffer[0] & 0xF0;
	if (nibble == NOTE_ON)
	{
		int ch = buffer[0] & 0x0F;
		msg->channel = ch;
		msg->Kind = MIDIChannelMessage::e_NoteOn;
		int key = buffer[1];
		// Filter any strange value received
		key = key & 0x7F;
		msg->data.NoteMessage.Note = key;
		int vel = buffer[2];
		msg->data.NoteMessage.Velocity = vel;
		msg->data.NoteMessage.Frequency = msg->GetFreqByMIDINote(key);
		if (msg->data.NoteMessage.Frequency == 0.0)
			pModuleServices->pLogger->writeLine("ERROR: invalid note %d", key);
	}
	else
		if (nibble == NOTE_OFF)
		{
			int ch = buffer[0] & 0x0F;
			msg->channel = ch;
			msg->Kind = MIDIChannelMessage::e_NoteOff;
			int key = buffer[1];
			msg->data.NoteMessage.Note = key;
			int vel = buffer[2];
			msg->data.NoteMessage.Velocity = vel;
		}
		else
			return NULL;

	return msg;
}

MIDIChannelMessage* AudioEngine::getMIDIMsgByString(const wchar_t* str)
{
	wchar_t* NOTE_ON = L"9";
	wchar_t* NOTE_OFF = L"8";
	MIDIChannelMessage* msg = new MIDIChannelMessage();
	if (str[0]==NOTE_ON[0])
	{
		int ch = getChannelByWChar(str + 1);
		msg->channel = ch;
		msg->Kind = MIDIChannelMessage::e_NoteOn;
		int key = getKeyByWchar(str + 2);
		msg->data.NoteMessage.Note = key;
		int vel = getVelByWchar(str + 4);
		msg->data.NoteMessage.Velocity = vel;
	} else 
	if (str[0] == NOTE_OFF[0])
	{
		int ch = getChannelByWChar(str + 1);
		msg->channel = ch;
		msg->Kind = MIDIChannelMessage::e_NoteOff;
		int key = getKeyByWchar(str + 2);
		msg->data.NoteMessage.Note = key;
		int vel = getVelByWchar(str + 4);
		msg->data.NoteMessage.Velocity = vel;
	}
	else
		return NULL;

	return msg;
}

int AudioEngine::log(const wchar_t* message) 
{
	pModuleServices->pLogger->writeLine(message);
	return 0;
}

int AudioEngine::forceLogFlush(void)
{
	pModuleServices->pLogger->Flush();
	return 0;
}

int AudioEngine::reopenLog(void)
{
	return pModuleServices->pLogger->ReOpen();
}

//##################################
//#######                    #######
//####### PROPERTIES CONTROL #######
//#######                    #######
//##################################

//const EUKind* AudioEngine::getEUKind(int factoryIndex, int elaborationUnitIndex, int categoryIndex)
const EUKind* AudioEngine::getEUKind(int factoryIndex, int categoryIndex, int elaborationUnitIndex)
{
	ElaborationUnitFactory* pFactory = audioEngine->factoryRetriever->getNthFactory(factoryIndex);
	if(pFactory==NULL)
		return NULL;

	if(categoryIndex==VIRTUAL_EU_CATEGORY)
	{
		const EUKind* euKind = pFactory->getNthVirtualEUKind(elaborationUnitIndex);
		return euKind;
	}
	else if(categoryIndex==PHYSICAL_EU_CATEGORY)
	{
		const EUKind* euKind = pFactory->getNthPhysicalEUKind(elaborationUnitIndex);
		return euKind;
	}
	else
		return NULL;
}

extern "C" __declspec( dllexport ) int createAlgorithm()
{
	return audioEngine->createAlgorithm();
}

extern "C" __declspec(dllexport) int destroyAlgorithm(int id)
{
	return audioEngine->removeAlgorithm(id);
}

extern "C" __declspec( dllexport ) int createElaborationUnit(int factoryIndex, int categoryIndex, int elaborationUnitIndex, int physicalInstanceIndex)
{
	switch(categoryIndex)
	{
	case VIRTUAL_EU_CATEGORY:
		return audioEngine->createVirtualElaborationUnit(factoryIndex, elaborationUnitIndex, 0);
	case PHYSICAL_EU_CATEGORY:
		return audioEngine->createPhysicalElaborationUnit(factoryIndex, elaborationUnitIndex, physicalInstanceIndex);
	default:
		return UNKNOWN_CATEGORY;
	}
	
}

extern "C" __declspec( dllexport ) int addElaborationUnitToAlgorithm(int algorithmIndex, int elaborationUnitIndex)
{
	return audioEngine->addElaboratioUnitToAlgorithm(algorithmIndex, elaborationUnitIndex);
}

extern "C" __declspec( dllexport ) int connectElaboratioUnits(int algorithmIndex, 
															  int elaborationUnitSource, int sourcePortId, 
															  int elaborationUnitDestination, int destinationPortId)
{
	return audioEngine->connectElaboratioUnits(algorithmIndex, elaborationUnitSource,sourcePortId,elaborationUnitDestination,destinationPortId);
}

extern "C" __declspec( dllexport ) char* checkAlgorithmValidity(int algorithmIndex)
{
	return audioEngine->checkAlgorithmValidity(algorithmIndex);
}


extern "C" __declspec( dllexport ) int playAlgorithm(int algorithmIndex)
{
	return audioEngine->playAlgorithm(algorithmIndex);
}

extern "C" __declspec( dllexport ) int stopAlgorithm(int algorithmIndex)
{
	return audioEngine->stopAlgorithm(algorithmIndex);
}

FactoryRetriever* AudioEngine::factoryRetriever;
std::vector<AudioEngine::AlgorithmProxy> AudioEngine::algorithms;

std::map<int, Algorithm*> AudioEngine::mapIdToAlgorithms;
int AudioEngine::currentAlgorithmId = 0;
std::map<int, ElaborationUnit*> AudioEngine::mapIdToElaborationUnits;
int AudioEngine::currentElaborationUnitId = 0;
std::map<int, int> AudioEngine::mapEUIdToFactoryId;

ModuleServices* AudioEngine::pModuleServices;