
#include "VoiceAllocator.h"
#include "ModuleServices.h"
#include <list>

VoiceAllocator::VoiceAllocator(double samplingPeriod, ModuleServices* services)
{
	m_pModuleServices = services;
	for(int i=0;i<VoiceAllocator::MAX_VOICES;i++)
	{
		m_Voices[i] = new SimpleVoice(samplingPeriod, i, this->m_pVoiceUpdater);
	}
	m_pVoiceUpdater = new SimpleVoiceUpdater(this);
}

VoiceAllocator::~VoiceAllocator()
{
	for(int i=0;i<VoiceAllocator::MAX_VOICES;i++)
	{
		delete m_Voices[i];
	}
	delete m_pVoiceUpdater;
}

void VoiceAllocator::resetAllVoices()
{
	m_FreeVoicesList.clear();
	for(int i=0;i<MAX_VOICES;i++)
	{
		m_FreeVoicesList.push_back(m_Voices[i]);
	}
	
	for(int i=0;i<MAX_VOICES;i++)
	{
		m_Voices[i]->dispose();

	}
	m_NextFreeVoiceIt = m_FreeVoicesList.begin();
}

void VoiceAllocator::moveOnNextVoice(void)
{
	m_NextFreeVoiceIt++;
	if(m_NextFreeVoiceIt==m_FreeVoicesList.end())
	{
		m_NextFreeVoiceIt = m_FreeVoicesList.begin();
	}
}

//TODO: probably these functions need some synchronization mechanism, like a semaphore

VoiceAllocator::AllocationResult VoiceAllocator::allocateVoice(double initialFreq)
{
	if(m_FreeVoicesList.size()==0)
	{
		this->m_pModuleServices->pLogger->writeLine("No more voices available.");
		return ALLOCATION_FAILURE;
	}
	SimpleVoice* voice = *(m_NextFreeVoiceIt);
	voice->activate(initialFreq);
	moveOnNextVoice();
	return ALLOCATION_SUCCEES;
}

SimpleVoice* VoiceAllocator::getVoiceById(int voiceId)
{
	return m_Voices[voiceId];
}

void VoiceAllocator::voiceCompleted(int voiceId)
{

	SimpleVoice* voice = getVoiceById(voiceId);
	m_FreeVoicesList.push_back(voice);
}