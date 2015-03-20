
#include "AbstractElaborationUnitFactory.h"

AbstractElaborationUnitFactory::AbstractElaborationUnitFactory(ModuleServices* moduleServices) : ElaborationUnitFactory(moduleServices)
{
}

void AbstractElaborationUnitFactory::addElaborationUnit(ElaborationUnit* peu)
{
	elaborationUnitVect.push_back(peu);
}

void AbstractElaborationUnitFactory::removeElaborationUnit(ElaborationUnit* peu)
{
	int numEu = elaborationUnitVect.size();
	vector<ElaborationUnit*>::iterator it;

	for(it=elaborationUnitVect.begin();it!=elaborationUnitVect.end();it++)
	{
		ElaborationUnit* pCurrentEU = *it;
		if(pCurrentEU==peu)
		{
			delete peu;
			elaborationUnitVect.erase(it);
			return;
		}
	}
}

void AbstractElaborationUnitFactory::cleanUpElaborationUnits(void)
{
	vector<ElaborationUnit*>::iterator it;
	for(it=elaborationUnitVect.begin();it!=elaborationUnitVect.end();it++)
	{
		ElaborationUnit* eu = *it;
		delete eu;
	}
	elaborationUnitVect.clear();
}