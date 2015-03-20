
#include "ElaborationUnitFactory.h"

ElaborationUnitFactory::ElaborationUnitFactory(ModuleServices* pServices)
{
	 moduleServices = pServices;
}

//template <class ElaborationUnitType>
//void EUFactory<ElaborationUnitType>::DestroyElaborationUnit(ElaborationUnitType* t)
void ElaborationUnitFactory::destroyElaborationUnit(ElaborationUnit* pEU)
{
	//delete pEU;
}

//template <class ElaborationUnitType>
//EUFactory<ElaborationUnitType>::~EUFactory(void)
ElaborationUnitFactory::~ElaborationUnitFactory()
{
}

