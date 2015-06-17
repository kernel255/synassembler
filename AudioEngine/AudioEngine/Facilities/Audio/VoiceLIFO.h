#pragma once

#include "../../ElaborationUnits/ModuleServices.h"
#include "ADSR.h"
#include "VoiceProxy.h"

class VoiceLIFO
{
public:

#define NUM_VOICES 20

	/**
	Array of voices to support polyphony for the same note
	*/
	VoiceProxy* preallocatedVoices[NUM_VOICES];

	VoiceProxy* getFirstFreeVoice();

	VoiceProxy* getLatestAllocated();

	VoiceProxy* firstAllocated;
	VoiceProxy* lastAllocated;

	VoiceProxy* getFirstAllocatedVoice();
	VoiceProxy* getNextAllocatedVoice();
	VoiceProxy* allocatedIterator;
	int numActiveVoices;
	VoiceProxy* getFirstAllocated();
	VoiceProxy* getMyPredecessor(VoiceProxy* myVProxy);
	VoiceProxy* getFirstDeactivable();
public:
	VoiceLIFO(double samplingPeriod, int samplesBufferSize, ModuleServices* pModuleServices);
	~VoiceLIFO();

	void Activate(double initialfreq, ADSR adsr);
	void Deactivate();
	void Deallocate(VoiceProxy* toBeDeallocated);
	bool isActive();

	ModuleServices* m_pModuleServices;
};

