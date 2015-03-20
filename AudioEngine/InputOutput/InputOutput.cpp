// InputOutput.cpp : Defines the entry point for the DLL application.
//

//#include "stdafx.h"

#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//#include "resource.h"
#include "InputOutputFactory.h"
#include "InputOutput.h"
#include "ModuleServices.h"

void AddAllResources(ModuleServices& moduleservice);
//Instance of the factory of this module
static ModuleServices* s_pModuleService;
//Instance of the factory of this module
InputOutputFactory* pInOutFact = NULL;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch( ul_reason_for_call )
	{
		/*
	case DLL_THREAD_ATTACH:
		{
			break;
		}
	case DLL_THREAD_DETACH:
		{
			break;
		}
		*/
	case DLL_PROCESS_ATTACH:
		{
			/*
			ModuleServices::SpecificOSData specData;
			specData.handle = hModule;
			//ModuleServices* services = new ModuleServices(&specData);

			m_pInOutFact = new InputOutputFactory(pmoduleservice);
			*/

			break;
		}
	case DLL_PROCESS_DETACH:
		{

			//Clean up factory
			//delete pInOutFact;

			s_pModuleService->pLogger->writeLine("Detaching InputOutput DLL");
			printf("Detaching InputOutput DLL\n");

			//delete pInOutFact;

			break;
		}
	}
    return TRUE;
}

ElaborationUnitFactory* RetrieveConcreteFactory(ModuleServices* pServices)
{
	s_pModuleService = pServices;
	if(pInOutFact==NULL)
	{
		pInOutFact = new InputOutputFactory(pServices);
		return pInOutFact;
	}
	else
		return pInOutFact;
}

void releaseConcreteFactory(ElaborationUnitFactory* pFactory)
{
	delete pFactory;
}