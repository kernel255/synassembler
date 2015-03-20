#pragma once

#include "AbstractElaborationUnit.h"

/**
 * \class PhysicalElaborationUnit
 * \brief Interface for all the classes that references with some hardware device,
 * like Audio in/out or MIDI in/out
 */

class ModuleServices;

class PhysicalElaborationUnit : public AbstractElaborationUnit
{
public:
	PhysicalElaborationUnit(ModuleServices* pService, int numProperties, const EUKind* kind);
	virtual ~PhysicalElaborationUnit();
	virtual void updateAudioSamples(EAG_SAMPLE_TYPE *pSamplesBuffer,int numsamples) = 0;
	virtual void allocate(void) = 0;
	virtual void deallocate(void) = 0;
	virtual void play(void) = 0;
	virtual void pause(void) = 0;
	PhysicalElaborationUnit* GetPhisicalInterface(void) { return this; }
	/**
	* @fn Retrieve the number of devices already allocated
	*/
	virtual int GetAllocatedDevices(void) = 0;
	/**
	* @fn Name of the device owned by the PhisicalElaborationUnit
	*/
	virtual const char* GetDeviceName(void) = 0;
};
