
#pragma once

#include <vector>

#include "ElaborationUnitFactory.h"
#include "AbstractElaborationUnitFactory.h"

/**
* \class Creates a set of commonly used elements, like oscillators
*/

class BasicEUFactory : public AbstractElaborationUnitFactory
{
public:
	BasicEUFactory();
	BasicEUFactory(ModuleServices* pService);
/**
 * @fn 
 * @return The number of different EU that the class can create
 */
	unsigned int GetEUKindNumber(void);
/**
 * @fn 
 * @param Index of the EU to retrieve info
 * @return Info about the nth EU
 */
	const EUKind* getNthEUKind(int n);
/**
 * @fn Create the factory when called the first time, after this return the pointer to it
 */
	//static GraphEUFactory* GetFactory(void);
/**
 * @fn Collective Module cleanup process
 */
	//static void CleanUp(void);
/**
 * @return Name of the factory
 */
	virtual const char *getName(void);
	virtual unsigned int getVirtualEUKindNumber(void);
	virtual const EUKind* getNthVirtualEUKind(int n);
	virtual ElaborationUnit* createVirtualElaborationUnit(unsigned euIndex);
	virtual unsigned int getPhysicalEUKindNumber(void);
	virtual const EUKind* getNthPhysicalEUKind(int n);
	virtual unsigned int getPhysicalEUInstanceNumber(int euIndex);
	virtual ElaborationUnit* createPhysicalElaborationUnit(unsigned euIndex, unsigned instanceIndex);
/**
 * @param Index of the Elaboration Unit to be created
 * @return Pointer to the specified EU
 */
	ElaborationUnit* createElaborationUnit(unsigned u);
	const int getEUIndex(ElaborationUnit* pEU);
/**
 * @fn Deallocate a specific EU
 * @param Pointer to the EU to be destroyed
 */
	void destroyElaborationUnit(ElaborationUnit* pGEU);

	virtual const int getPhysicalEUInstanceNumber(void);
	virtual const wchar_t* getNthPhysicalEUInstanceName(int euIndex, int instanceIndex);

	virtual ~BasicEUFactory();
private:
	static const int LocalEUNumber;
};