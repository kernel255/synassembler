
#ifndef AUDIOENGINE_API
#define AUDIOENGINE_API 1

#define ALGORITHM_REMOVED			0
#define ALGORITHM_NOT_FOUND			-1
#define ELABORATIONUNIT_NOT_FOUND	-2
#define EU_ADDED_TO_ALGORITHM		0
#define FACTORY_NOT_FOUND			-3
#define EU_CONNECTED				0
#define SOURCE_EU_NOT_FOUND			-2
#define DEST_EU_NOT_FOUND			-3
#define SOURCE_EU_PORT_NOT_FOUND	-4
#define DEST_EU_PORT_NOT_FOUND		-5
#define PORT_KIND_DIFFERS			-6
#define UNKNOWN_PORT_KIND			-7
#define SOURCE_PORT_IS_INPUT		-8
#define DESTINATION_PORT_IS_OUTPUT	-9

#define UNKNOWN_CATEGORY			-1

#define INFINITE_INSTANCES			-1

// EU category (maybe it will be evolved)
#define VIRTUAL_EU_CATEGORY				0
#define PHYSICAL_EU_CATEGORY			1

#include "SpecificOSInfo.h"
#include "Facilities\General\EngineSettings.h"

#ifdef WIN32

//extern "C" __declspec( dllexport ) AudioEngine* getAudioEngine(wchar_t* folderName);
//extern "C" __declspec( dllexport ) void releaseAudioEngine(void);

// procedural interface to the AudioEngine

/**
 * @fn Initialize the library by supplying startup parameters
 * @param folderName Folder where the audio factories are stored
 * @param osInfo OS Specific informations to supply to Engine
 * @param pEngSettings General settings of the Engine
 */

/*
extern "C" __declspec( dllexport ) void initEngine(const wchar_t* folderName, SpecificOSInfo* osInfo, EngineSettings* pEngSettings);
*/

extern "C" __declspec( dllexport ) int initEngine(const wchar_t* folderName, int hwnd, int sampligFrequence, int bitResolution, int numChannels);

extern "C" __declspec( dllexport ) void releaseEngine(void);

#ifdef WIN32

extern "C" __declspec( dllexport ) void setHwndEngine(int hwnd);

#endif //WIN32

//#####################
//### FACTORY QUERY ###
//#####################

/**
 * @return The number of available factories (0...n) accordig to the found dll/so plugin fuond during init phase
 */
extern "C" __declspec( dllexport ) int getFactoryNumber(void);
/**
 * @return The Nth factory displayable name
 */
extern "C" __declspec( dllexport ) const char* getNthFactoryName(int factoryIndex);




extern "C" __declspec( dllexport ) int getNthFactoryNameEx(int factoryIndex, wchar_t* buffer, int bufferLen);



//##############################
//### ELABORATION UNIT QUERY ###
//##############################

/**
 * @param factoryIndex The factory by which we want to get infos
 * @param categoryIndex The category of the EU
 * @return Get the number of kind of ElaborationUnits the factory can create
 */
extern "C" __declspec( dllexport ) int getElaborationUnitNumber(int factoryIndex, int categoryIndex);

/**
 * Given the ElaborationUnit index, return how many of these can be instanced
 * @return The number of instantiable EUs or INFINITE_INSTANCES e.g. in case of virtual EUs
 */

extern "C" __declspec( dllexport ) int getElaborationUnitInstanceNumber(int factoryIndex, int categoryIndex, int elaborationUnitIndex);

/**
 * @return the Nth instance name
 */
extern "C" __declspec( dllexport ) int getNthInstanceName(wchar_t* buffer, int bufferLen, int factoryIndex, int categoryIndex, int elaborationUnitIndex, int instanceIndex);

/**
 * @return Get the name of the Nth elaboration unit inside a factory
 */
extern "C" __declspec( dllexport ) const char* getNthElaborationUnitName(int factoryIndex, int categoryIndex, int elaborationUnitIndex);




extern "C" __declspec( dllexport ) int getNthElaborationUnitNameEx(int factoryIndex, int categoryIndex, int elaborationUnitIndex, wchar_t* buffer, int bufferLen);




/**
 * @return the number of input ports for an elaboration unit
 */
extern "C" __declspec( dllexport ) int getNthElaborationUnitInputPortNumber(int factoryIndex, int categoryIndex, int elaborationUnitIndex);
/**
 * @return the name of the Nth input port
 */
extern "C" __declspec( dllexport ) const char* getNthElaborationInputUnitPortName(int factoryIndex, int categoryIndex, int elaborationUnitIndex,
																	   int portIndex);



extern "C" __declspec( dllexport ) int getNthElaborationInputUnitPortNameEx(int factoryIndex, int categoryIndex,
																				  int elaborationUnitIndex, int portIndex, wchar_t* buffer, int bufferLen);



/**
 * @return The number of output ports for an elaboration unit
 */
extern "C" __declspec( dllexport ) int getNthElaborationUnitOutputPortNumber(int factoryIndex, int categoryIndex, int elaborationUnitIndex);
/**
 * @return The name of the Nth output port
 */
extern "C" __declspec( dllexport ) const char* getNthElaborationUnitOutputPortName(int factoryIndex, int categoryIndex, int elaborationUnitIndex,
																   int portIndex);


extern "C" __declspec( dllexport ) int getNthElaborationUnitOutputPortNameEx(int factoryIndex, int categoryIndex,
																				  int elaborationUnitIndex, int portIndex, wchar_t* buffer, int bufferLen);




/**
 * @return The port kind: MIDI or Audio
 */
extern "C" __declspec( dllexport ) int getNthElaborationUnitInputPortType(int factoryIndex, int categoryIndex, int elaborationUnitIndex,
																	int portIndex, wchar_t* buffer, int bufferLen);
extern "C" __declspec( dllexport ) int getNthElaborationUnitOutputPortType(int factoryIndex, int categoryIndex, int elaborationUnitIndex,
																	int portIndex, wchar_t* buffer, int bufferLen);


//################################################
//### ALGORITHM AND ELABORATION UNITS CREATION ###
//################################################

/**
 * Create an algorithm inside the AudioEngine
 * @return The unique id of the algorithm
 */
extern "C" __declspec( dllexport ) int createAlgorithm();
/**
 * Create an elaboration unit by id, category id and factory id
 */
extern "C" __declspec( dllexport ) int createElaborationUnit(int factoryIndex, int categoryIndex, int elaborationUnitIndex, int physicalInstanceIndex);
/**
 * Add an elaboration unit to an algorithm
 */
extern "C" __declspec( dllexport ) int addElaborationUnitToAlgorithm(int algorithmIndex, int elaborationUnitId);
/**
 * Connect two elaboration units inside an algorithm
 * The following preconditions must be satisfied:
 * 1. The source port shall be an output and the destination port shall be an input
 * 2. The two ports are of the same kind (MIDI or Audio,...)
 */
extern "C" __declspec( dllexport ) int connectElaboratioUnits(int algorithmIndex, 
															  int elaborationUnitSource, int sourcePortId, 
															  int elaborationUnitDestination, int destinationPortId);


/**
 * @fn Test if the specified algorithm is valid
 * @return A string with the error description. An empty string if the algorithm is valid.
 */
extern "C" __declspec( dllexport ) char* checkAlgorithmValidity(int algorithmIndex);

/**
 * Plays the selected algorithm
 */
extern "C" __declspec( dllexport ) int playAlgorithm(int algorithmId);

extern "C" __declspec( dllexport ) int stopAlgorithm(int algorithmId);

//########################################
//### EXTERNALLY CONTROLLED PROPERTIES ###
//########################################

#define INTEGER_CATEGORY		0
#define STRING_CATEGORY			1

#define PROPERTY_DIRECTION_READABLE			0
#define PROPERTY_DIRECTION_WRITABLE			1
#define PROPERTY_DIRECTION_READABLEWRITABLE	2

/**
 * @fn Query the available run-time property for a given elaboration unit inside a factory
 */
extern "C" __declspec( dllexport ) int getElaborationUnitPropertyNumber(int factoryIndex, int categoryIndex, 
				int elaborationUnitIndex);

/**
 * @fn Get the nth property name inside a factory, category and elaboration unit
 */
extern "C" __declspec( dllexport ) int getNthEUPropertyName(int factoryIndex, int categoryIndex, 
				int elaborationUnitIndex,int propertyIndex, wchar_t* buffer, int bufferSize);


/**
 * @fn Set the Nth property of an EU
 */
 extern "C" __declspec( dllexport ) int setEUProperty(int elaborationUnitIndex,	int propertyIndex, char* valueStr);
/**
 * @fn Get the Nth property of an EU
   @param value The value to be read from EU
   @param bufferSize The size of the value buffer
 */
 extern "C" __declspec( dllexport ) int getEUProperty(int elaborationUnitIndex, int propertyIndex, char* value, int bufferSize);


 extern "C" __declspec( dllexport ) int setEUDProperty(int elaborationUnitIndex, int propertyIndex, double value);

 extern "C" __declspec( dllexport ) int getEUDProperty(int elaborationUnitIndex, int propertyIndex, double* value);

 extern "C" __declspec(dllexport) int setEUIProperty(int elaborationUnitIndex, int propertyIndex, int value);

 extern "C" __declspec(dllexport) int getEUIProperty(int elaborationUnitIndex, int propertyIndex, int* value);

 /**
  * @fn Read the property type accordingly to the table in the API specification
  */

 extern "C" __declspec( dllexport ) int getPropertyType(int factoryIndex, int categoryIndex, int elaborationUnitIndex, int propertyIndex);

 /**
  * @fn Read the maximum, minimum or list of available values
  */

 extern "C" __declspec( dllexport ) int getPropertyRanges(int factoryIndex, int categoryIndex, int elaborationUnitIndex, int propertyIndex,
	 char* lowRange, char* highRange, int bufferSize);



 // I wrote those function but I don't know why...

/**
 * @fn Set the nth property inside an Elaboration Unit
 */ 
extern "C" __declspec( dllexport ) int setPropertyByName(int factoryIndex, int categoryIndex, int elaborationUnitIndex, int propertyIndex, const char* value);
/**
 * @fn Get the nth property from an Elaboration Unit
 */
extern "C" __declspec( dllexport ) const char* getPropertyByName(int factoryIndex, int categoryIndex, int elaborationUnitIndex, int propertyIndex);
/**
 * @fn Pass a MIDI message directly to an EU in hexadecimal format
*/
//extern "C" __declspec(dllexport) int sendMIDIMessage(int euIndex, wchar_t* midiMsg);
extern "C" __declspec(dllexport) int sendMIDIMessage(int euIndex, unsigned char* midiMsg);

#endif //WIN32

#endif //AUDIOENGINE_API