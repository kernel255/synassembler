#pragma once

#include "../../ElaborationUnits/ModuleServices.h"
#include "VoiceProxy.h"

class VoiceLIFO
{
public:

#define NUM_VOICES 20

	VoiceProxy* preallocatedVoices[NUM_VOICES];

	VoiceProxy* firstFree;

	VoiceProxy* getFirstFreeVoice();

	VoiceProxy* getLatestAllocated();

	VoiceProxy* firstAllocated;
	VoiceProxy* last;

	VoiceProxy* getFirstAllocatedVoice();
	VoiceProxy* getNextAllocatedVoice();
	VoiceProxy* allocatedIterator;
	int numActiveVoices;
	VoiceProxy* getFirstAllocated();
public:
	VoiceLIFO(double samplingPeriod, int samplesBufferSize, ModuleServices* pModuleServices);
	~VoiceLIFO();

	void Activate(double initialfreq);
	void Deactivate();
	void Deallocate(VoiceProxy* toBeDeallocated);
	bool isActive();

	ModuleServices* m_pModuleServices;
};

