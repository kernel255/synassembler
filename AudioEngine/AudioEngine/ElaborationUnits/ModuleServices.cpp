
#include <string>
#include <xstring>
#include <cstring>
#include <wchar.h>
#include <map>
#include "ModuleServices.h"
#include "../Facilities/General/TimeBase.h"
#include "../Facilities/General/EngineSettings.h"


ModuleServices::ModuleServices(char* logFilename, EngineSettings* pEngineSettings)
{
	Logger* log = new Logger();
	size_t strLen = strlen(logFilename) + 1;
	size_t convertedChars;
	wchar_t wcBuffer[1000];
	mbstowcs_s(&convertedChars,wcBuffer,strLen, logFilename,_TRUNCATE);
	log->open(wcBuffer);
	pLogger = log;
	timeBase = new TimeBase(log);
	m_pEngineSettings = pEngineSettings;
}

ModuleServices::~ModuleServices()
{
	/*
	std::map<std::string, AbstractResource*>::iterator it;
	//Destroy all the pointers to Abstract resources
	for(it = m_pResourceMap->begin(); it != m_pResourceMap->end(); ++it)
	{
		delete (*it).second;
	}
	delete m_pModuleInfo; 
	m_pResourceMap->clear();
	delete m_pResourceMap;
	*/
	delete pLogger;
	delete timeBase;
}

EngineSettings* ModuleServices::getEngineSettings()
{
	return m_pEngineSettings;
}

/*
ModuleServices::ModuleServices(SpecificOSData* specData)
{
	memcpy(&specOSData, specData, sizeof(SpecificOSData));
}
*/

/*
ModuleServices::SpecificOSData* ModuleServices::getSpecificData()
{
	return &specOSData;
}
*/