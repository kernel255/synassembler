#pragma once

#include "PhysicalElaborationUnit.h"

class ModuleServices;

/**
 * \class AudioOutput
 * \brief Interface class for all kind of ElaborationUnits that produce Audio data.
 */
class AudioOutput : public PhysicalElaborationUnit
{
public:
	AudioOutput(ModuleServices* pService, int numProperties, const EUKind* kind);
	virtual ~AudioOutput();
	virtual void updateAudioSamples(EAG_SAMPLE_TYPE *pSamplesBuffer,int numsamples) = 0;
	//Input/Output
	virtual void Allocate(void);
	virtual void Deallocate(void);
	virtual void Play(void);
	virtual void Pause(void);
};
