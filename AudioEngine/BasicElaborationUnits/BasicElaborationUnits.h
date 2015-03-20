#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <windows.h>
#include "BasicEUFactory.h"

extern "C" __declspec( dllexport ) ElaborationUnitFactory* RetrieveConcreteFactory(ModuleServices*);
extern "C" __declspec( dllexport ) void releaseConcreteFactory(ElaborationUnitFactory* pFactory);