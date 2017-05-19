
#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
/*
#ifdef _DEBUG
#ifndef DEBUG_NEW
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#endif
*/

#define INVALID_ALGORITHM_ID		-1

#include <vector>

#include "ElaborationUnits\Algorithm.h"
#include "FactoryRetriever.h"
#include "ElaborationUnits\ModuleServices.h"
#include "Facilities/General/TimeBase.h"

#define AUDIO_ENGINE_LOGIFLENAME		"audioengine.log"

#include "AudioEngineAPI.h"

class AudioEngine;

class AudioEngine {
public:
	AudioEngine(const wchar_t* folderName, ModuleServices* pServices);
	~AudioEngine();
	void setSpecifiOSInfo(SpecificOSInfo* osInfo);
	// Factories
	std::vector<ElaborationUnitFactory*> getFactories();
	// Algorithms
	int createAlgorithm();
	int removeAlgorithm(int algorithmIndex);
	// ElaborationUnits/Virtual
	int createVirtualElaborationUnit(int factoryIndex, int elaborationUnitIndex, int instanceIndex);
	// ElaborationUnits/Physical
	int createPhysicalElaborationUnit(int factoryIndex, int elaborationUnitIndex, int instanceIndex);
	int addElaboratioUnitToAlgorithm(int elabrationUnitIndex, int algorithmIndex);
	int connectElaboratioUnits(int algorithmIndex, int elaborationUnitSource, int sourcePortId, int elaborationUnitDestination, int destinationPortId);
	char* checkAlgorithmValidity(int algorithmIndex);

	int playAlgorithm(int algorithmId);
	int stopAlgorithm(int algorithmId);

	int setEUProperty(int elaborationUnitIndex, int propertyIndex, const wchar_t* value);
	int getEUProperty(int elaborationUnitIndex, int propertyIndex, wchar_t* value, int bufferSize);

	int getEUDProperty(int elaborationUnitIndex, int propertyIndex, double *value);
	int setEUDProperty(int elaborationUnitIndex, int propertyIndex, double value);

	int setEUIProperty(int elaborationUnitIndex, int propertyIndex, int value);
	int getEUIProperty(int elaborationUnitIndex, int propertyIndex, int* value);

	int setEUBProperty(int elaborationUnitIndex, int propertyIndex, bool value);
	int getEUBProperty(int elaborationUnitIndex, int propertyIndex, bool* value);

	int setPropertyByName(int factoryIndex, int categoryIndex, int elaborationUnitIndex, int propertyIndex, const char* value);
	const char* getPropertyByName(int factoryIndex, int categoryIndex, int elaborationUnitIndex, int propertyIndex);
	//int sendMIDIMessage(int euIndex, wchar_t* msg);
	int sendMIDIMessage(int euIndex, unsigned char* msg);
	int log(const wchar_t* message);
	int forceLogFlush(void);
	int reopenLog(void);
protected:
	Algorithm* newAlgorithm();
	void deleteAlgorithm(Algorithm* algo);
	MIDIChannelMessage* getMIDIMsgByString(const wchar_t* str);
	MIDIChannelMessage* getMIDIMsgByBytes(unsigned char* buffer);
#ifdef WIN32
	friend __declspec( dllexport ) void setHwndEngine(int hwnd);
#endif //WIN32

	// Factory
	friend __declspec( dllexport ) int getFactoryNumber();
	friend __declspec( dllexport ) const char* getNthFactoryName(int factoryIndex);


	friend __declspec( dllexport ) int getNthFactoryNameEx(int factoryIndex, wchar_t* buffer, int bufferLen);


	// ElaborationUnits query
	friend __declspec( dllexport ) int getElaborationUnitNumber(int factoryIndex, int categoryIndex);
	friend __declspec( dllexport ) int getElaborationUnitInstanceNumber(int factoryIndex, int categoryIndex, int elaborationUnitIndex);
	friend __declspec( dllexport ) int getNthInstanceName(wchar_t* buffer, int bufferLen, int factoryIndex, int categoryIndex, int elaborationUnitIndex, int instanceIndex);
	friend __declspec( dllexport ) const char* getNthElaborationUnitName(int factoryIndex, int categoryIndex, int elaborationUnitIndex);



	friend __declspec( dllexport ) int getNthElaborationUnitName(int factoryIndex, int categoryIndex, int elaborationUnitIndex, wchar_t* buffer, int bufferLen);
	friend __declspec( dllexport ) int getNthElaborationUnitNameEx(int factoryIndex, int categoryIndex, int elaborationUnitIndex, wchar_t* buffer, int bufferLen);



	friend __declspec( dllexport ) int getNthElaborationUnitInputPortNumber(int factoryIndex, int categoryIndex, int elaborationUnitIndex);
	friend __declspec( dllexport ) const char* getNthElaborationInputUnitPortName(int factoryIndex, int categoryIndex, int elaborationUnitIndex,
																	   int portIndex);


	friend __declspec( dllexport ) int getNthElaborationInputUnitPortNameEx(int factoryIndex, int categoryIndex,
																				  int elaborationUnitIndex, int portIndex, wchar_t* buffer, int bufferLen);



	friend __declspec( dllexport ) int getNthElaborationUnitOutputPortNumber(int factoryIndex, int categoryIndex, int elaborationUnitIndex);
	friend __declspec( dllexport ) const char* getNthElaborationUnitOutputPortName(int factoryIndex, int categoryIndex, int elaborationUnitIndex,
																	   int portIndex);


	friend __declspec( dllexport ) int getNthElaborationUnitOutputPortNameEx(int factoryIndex, int categoryIndex,
																				  int elaborationUnitIndex, int portIndex, wchar_t* buffer, int bufferLen);



	friend __declspec( dllexport ) int getNthElaborationUnitInputPortType(int factoryIndex, int categoryIndex, int elaborationUnitIndex,
		int portIndex, wchar_t* buffer, int bufferLen);
	friend __declspec( dllexport ) int getNthElaborationUnitOutputPortType(int factoryIndex, int categoryIndex, int elaborationUnitIndex,
		int portIndex, wchar_t* buffer, int bufferLen);

	// Algorithms
	friend __declspec( dllexport ) int createAlgorithm();
	friend __declspec( dllexport ) int createElaborationUnit(int factoryIndex, int categoryIndex, int elaborationUnitIndex);
	friend __declspec( dllexport ) int addElaborationUnitToAlgorithm(int algorithmIndex, int elaborationUnitId);
	friend __declspec( dllexport ) int connectElaboratioUnits(int algorithmIndex, 
															  int elaborationUnitSource, int sourcePortId, 
															  int elaborationUnitDestination, int destinationPortId);
	friend __declspec( dllexport ) char* checkAlgorithmValidity(int algorithmIndex);
	friend __declspec( dllexport ) int playAlgorithm(int algorithmIndex);
	// Properties
	friend __declspec( dllexport ) int getElaborationUnitPropertyNumber(int factoryIndex, int categoryIndex, int elaborationUnitIndex);
	friend __declspec( dllexport ) int getNthEUPropertyName(int factoryIndex, int categoryIndex, int elaborationUnitIndex, int propertyIndex, wchar_t* buffer, int bufferLen);
	friend __declspec( dllexport ) int setEUProperty(int elaborationUnitIndex,	int propertyIndex, char* valueStr);
	friend __declspec( dllexport ) int getEUProperty(int elaborationUnitIndex,	int propertyIndex, char* value, int bufferSize);
	friend __declspec( dllexport ) int setPropertyByName(int factoryIndex, int categoryIndex, int elaborationUnitIndex, int propertyIndex, const char* value);
	friend __declspec( dllexport ) const char* getPropertyByName(int factoryIndex, int categoryIndex, int elaborationUnitIndex, int propertyIndex);
	friend __declspec( dllexport ) int getPropertyType(int factoryIndex, int categoryIndex, int elaborationUnitIndex, int propertyIndex);
	friend __declspec( dllexport ) int getPropertyRanges(int factoryIndex, int categoryIndex, int elaborationUnitIndex, int propertyIndex,
	 char* lowRange, char* highRange, int bufferSize);



private:

	class AlgorithmProxy
	{
	public:
		Algorithm* algo;
		int id;
		AlgorithmProxy(Algorithm* _algo, int _id)
		{
			algo = _algo;
			id = _id;
		}
	};
	/**
	* True if some algorithm is playing
	*/
	bool m_bPlaying;
	void cleanUpAlgorithms(void);
	Algorithm* getAlgorithmById(int id);
	ElaborationUnit* getElaborationUnitById(int id);
	const EUKind* getEUKind(int factoryIndex, int elaborationUnitIndex, int categoryIndex);
	static FactoryRetriever* factoryRetriever;
	static std::vector<AlgorithmProxy> algorithms;
	/**
	 * Map the Algorithm pointers to the id (as seen outside the library)
	 */
	static std::map<int, Algorithm*> mapIdToAlgorithms;
	static int currentAlgorithmId;
	/**
	 * Map the ElaborationUnit pointers to the id (as seen outside the library)
	 */
	static std::map<int, ElaborationUnit*> mapIdToElaborationUnits;
	static int currentElaborationUnitId;
	/**
	 * Map the ElaborationUnit Id to the factory Id
	 */
	static std::map<int, int> mapEUIdToFactoryId;
	static ModuleServices* pModuleServices;
	static EngineSettings s_Settings;
};