
#pragma once

#include "gg.h"

extern "C" __declspec( dllexport ) ElaborationUnitFactory* RetrieveConcreteFactory(ModuleServices*);
extern "C" __declspec( dllexport ) void releaseConcreteFactory(ElaborationUnitFactory* pFactory);