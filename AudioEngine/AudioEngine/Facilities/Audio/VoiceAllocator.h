
#pragma once

#include <list>

#include "SimpleVoice.h"
#include "ModuleServices.h"
#include "SimpleVoiceUpdater.h"

class VoiceAllocator {
public:
	enum AllocationResult {
		ALLOCATION_SUCCEES = 0,
		ALLOCATION_FAILURE = 1
	};

	VoiceAllocator(double samplingPeriod, ModuleServices* services);
	~VoiceAllocator();
	void voiceCompleted(int voiceId);
	AllocationResult allocateVoice(double initialFreq);

private:
	ModuleServices* m_pModuleServices;
	static const int MAX_VOICES = 100;
	SimpleVoice* m_Voices[MAX_VOICES];
	SimpleVoiceUpdater* m_pVoiceUpdater;

	std::list<SimpleVoice*>::iterator m_NextFreeVoiceIt;
	std::list<SimpleVoice*> m_FreeVoicesList;
	void resetAllVoices();
	SimpleVoice* getVoiceById(int voiceId);
	void moveOnNextVoice(void);
};