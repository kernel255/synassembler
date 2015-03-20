
#pragma once

#include <vector>

using namespace std;

#include "ElaborationUnitFactory.h"
#include "ElaborationUnit.h"

/**
 * This class holds the responsability of storing the created EUs.
 */

class AbstractElaborationUnitFactory : public ElaborationUnitFactory
{
protected:
	AbstractElaborationUnitFactory(ModuleServices* moduleServices);
	/**
	 * @fn Store an EU inside the internal collection
	 */
	void addElaborationUnit(ElaborationUnit* peu);
	/**
	 * @fn Remove an EU from the internal collection
	 */
	void removeElaborationUnit(ElaborationUnit* peu);
	/**
	 * @fn Remove all the stored EUs
	 */
	void cleanUpElaborationUnits(void);
private:
	/**
	 * @var Store all the EU created by the factory
	 */
	vector<ElaborationUnit*> elaborationUnitVect;
};