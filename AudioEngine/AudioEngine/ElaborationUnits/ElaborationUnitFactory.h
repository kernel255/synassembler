#pragma once

//#include ".\CommonAudioLayer\ElaborationUnit.h"
#include "ElaborationUnit.h"

#define C_UnknownIndex -1

class EUKind;
class ModuleServices;

/**
 * @class ElaborationUnitFactory This class has the responsability of creating and destroying the ElaborationUnits.
 * Two kind of ElaborationUnit can be produced: virtual a physical, for each class appropriate method groups are available.
 */

class ElaborationUnitFactory
{
public:
	/**
	* @fn Create the EUFactory and set the factory's module service
	*/
	ElaborationUnitFactory(ModuleServices* pServices);
	/**
	* @return Name of the factory
	*/
	virtual const char *getName(void) = 0;

	//######### VIRTUAL ELABORATION UNITS #########

	/**
	* @fn Retrieve the different kind of virtual EU that the factory can instance
	*/
	virtual unsigned int getVirtualEUKindNumber(void) = 0;
	/**
	* @fn Retrieve the n-th Kind description of virtual EU that the EUFactory can instance
	*/
	virtual const EUKind* getNthVirtualEUKind(int n) = 0;
	/**
	 * @fn Create a virtual elaboration unit
	 * @param euIndex Index of the Elaboration Unit to be created
	 * @return Pointer to the specified EU
	 */
	virtual ElaborationUnit* createVirtualElaborationUnit(unsigned euIndex) = 0;

	//######### PHYSICAL ELABORATION UNITS #########

	/**
	* @fn Retrieve the different kind of physical EU that the factory can instance
	*/
	virtual unsigned int getPhysicalEUKindNumber(void) = 0;
	/**
	* @fn Retrieve the n-th Kind description of virtual EU that the EUFactory can instance
	*/
	virtual const EUKind* getNthPhysicalEUKind(int n) = 0;
	/**
	 * @fn Retrieve the number of available instances of a physical ElaborationUnit
	 */
	virtual unsigned int getPhysicalEUInstanceNumber(int euIndex) = 0;

	virtual const wchar_t* getNthPhysicalEUInstanceName(int euIndex, int instanceIndex) = 0;

	/**
	 * @fn Create a physical elaboration unit. It is necessary to specify the instance to create
	 * @param euIndex Index of the Elaboration Unit to be created
	 * @param instanceIndex Index of the instance to be created
	 */
	virtual ElaborationUnit* createPhysicalElaborationUnit(unsigned euIndex, unsigned instanceIndex) = 0;

	/**
	* @fn Retrieve the given EU Factory's internal index
	* @return Index of the EU in the factory, C_UnknownIndex if EU is not found.
	*/
	virtual const int getEUIndex(ElaborationUnit* pEU) = 0;
	/**
	* @fn Destroy the specified EU
	*/
	virtual void destroyElaborationUnit(ElaborationUnit* eu);
	/**
	* virtual destructor
	*/
	virtual ~ElaborationUnitFactory();
private:
	ElaborationUnitFactory* m_hinstance;
protected:
	ModuleServices* moduleServices;
};

