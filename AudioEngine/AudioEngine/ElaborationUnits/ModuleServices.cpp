
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
	m_pArrayLoggerFactory = new ArrayLoggerFactory(log);
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

void ModuleServices::dumpSampleBlock(double* buffer, int numSamples)
{
	int lineElems = 4;
	for (int i = 0; i < numSamples;i++)
	{
		printf("[%f]", buffer[i]);
		if (i%lineElems == 0 && i != 0)
			printf("\n");
	}
}

IArrayLogger * ModuleServices::getArrayLogger(char* name)
{
	IArrayLogger* result = m_pArrayLoggerFactory->getNewArrayLogger(name);
	return result;
}

void ModuleServices::dumpAllArrayLogger()
{
	m_pArrayLoggerFactory->dumpAllCSV();
	delete m_pArrayLoggerFactory;
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