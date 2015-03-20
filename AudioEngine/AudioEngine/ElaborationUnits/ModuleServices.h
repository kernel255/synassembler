#pragma once

#include <map>
#include <string>

#include "../Facilities/General/Logger.h"
#include "../Facilities/General/TimeBase.h"
#include "../Facilities/General/EngineSettings.h"
#include "../SpecificOSInfo.h"

#ifdef WIN32
#include <windows.h>
#endif //WIN32

class AbstractModuleInfo;
class AbstractResource;

/**
 * @class Holds some infos to share with plug-ins. Some info are OS-specific, see defines.
 */

class ModuleServices
{
/////// INFO COLLECTION ///////
public:
	SpecificOSInfo specOSInfo;
	Logger* pLogger;
/////// INFO COLLECTION ///////
	TimeBase* timeBase;
	//SpecificOSData* getSpecificData();
	/**
	 * The constructor requires an opaque OS-specific data, the ElaborationUnits that requires this values,
	 * will read them accordingly to the specifi OS
	 */
	//ModuleServices(SpecificOSData* specData);
	AbstractModuleInfo* GetModuleInfo(void) { return m_pModuleInfo; }
	void SetModuleInfo(AbstractModuleInfo* pModInfo) { m_pModuleInfo = pModInfo; }
	//Abstract resource management
	//void AddResource(AbstractResource* pAbsRes);
	//AbstractResource* GetResourceByName(const char* resname);
	virtual ~ModuleServices();
	ModuleServices(char* logFilename, EngineSettings* pEngineSettings);
	EngineSettings* getEngineSettings();
private:
	AbstractModuleInfo* m_pModuleInfo;
	EngineSettings* m_pEngineSettings;
	//SpecificOSData specOSData;
};