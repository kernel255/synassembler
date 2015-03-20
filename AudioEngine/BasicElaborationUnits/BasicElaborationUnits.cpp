// BasicElaborationUnits.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
//Allow memory leaks detection
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "ModuleServices.h"

#include "BasicElaborationUnits.h"
#include "BasicEUFactory.h"

//Module service to supply data specific to this module
static ModuleServices* pModuleService;
//Instance of the factory of this module
BasicEUFactory* pBasicEUFact;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{


	switch( ul_reason_for_call )
	{
	case DLL_PROCESS_ATTACH:
		{

			break;
		}
	case DLL_PROCESS_DETACH:
		{

			break;
		}
	}
    return TRUE;
}

/**
* /fn Retrieve the factory of the DLL
*/

ElaborationUnitFactory* RetrieveConcreteFactory(ModuleServices* pServices)
{
	pModuleService = pServices;
	if(pBasicEUFact==NULL)
		pBasicEUFact = new BasicEUFactory(pServices);

	return pBasicEUFact;
}

void releaseConcreteFactory(ElaborationUnitFactory* pFactory)
{
	delete pFactory;
}