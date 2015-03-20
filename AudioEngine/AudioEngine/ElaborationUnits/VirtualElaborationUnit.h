#pragma once

#include "ElaborationUnit.h"
#include "ModuleServices.h"
#include "AbstractElaborationUnit.h"

/**
 * \class VirtualElaborationUnit
 * \brief Interface class for all the software-calculated ElaborationUnits
 */

class VirtualElaborationUnit : public AbstractElaborationUnit
{
public:
	VirtualElaborationUnit(ModuleServices* pService, int numProperties, const EUKind* kind);
	virtual void updateAudioSamples(EAG_SAMPLE_TYPE *pSamplesBuffer,int numsamples) = 0;
	virtual ~VirtualElaborationUnit();
	virtual void allocate(void) = 0;
	virtual void deallocate(void) = 0;
	virtual void play(void) = 0;
	virtual void pause(void) = 0;
};
