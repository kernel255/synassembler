
#include <stdio.h>
#include "VoiceLIFO.h"

VoiceLIFO::VoiceLIFO(double samplingPeriod, int samplesBufferSize, ModuleServices* pModuleServices)
{
	numActiveVoices = 0;
	m_pModuleServices = pModuleServices;
	for (int i = 0; i < NUM_VOICES; i++)
	{
		preallocatedVoices[i] = new VoiceProxy(i, samplingPeriod, samplesBufferSize, pModuleServices);
	}

	firstFree = preallocatedVoices[0];
	firstAllocated = NULL;
	for (int i = 0; i < NUM_VOICES; i++)
	{
		if (i == NUM_VOICES - 1)
		{
			preallocatedVoices[i]->nextFree = preallocatedVoices[0];
		}
		else
		{
			preallocatedVoices[i]->nextFree = preallocatedVoices[i + 1];
		}
	}

}

VoiceLIFO::~VoiceLIFO()
{
	for (int i = 0; i < NUM_VOICES; i++)
	{
		delete preallocatedVoices[i];
	}
}

VoiceProxy* VoiceLIFO::getFirstFreeVoice()
{
	for (int i = 0; i < NUM_VOICES; i++)
	{
		if (preallocatedVoices[i]->isFree())
			return preallocatedVoices[i];
	}
	return NULL;
}

VoiceProxy* VoiceLIFO::getLatestAllocated()
{
	//Nothing has been allocated
	if (firstAllocated == NULL)
		return NULL;
	VoiceProxy* ptr = firstAllocated;
	while (ptr->nextAllocated != NULL)
	{
		ptr = ptr->nextAllocated;
	}
	return ptr;
}

void VoiceLIFO::Activate(double initialfreq)
{
	// Search for first free voice
	VoiceProxy* firstFree = getFirstFreeVoice();
	if (firstFree != NULL)
	{
		//m_pModuleServices->pLogger->writeLine("firstFree=%d", firstFree->id);
		firstFree->allocate();
		if (firstAllocated == NULL) //Nothing is allocated
		{
			firstAllocated = firstFree;
			firstAllocated->nextAllocated = NULL;
		}
		else
		{
			// Some other voice is already allocated
			VoiceProxy* latestAllocated = getLatestAllocated();
			latestAllocated->nextAllocated = firstFree;
			firstFree->nextAllocated = NULL;
		}

		firstFree->activate(initialfreq);
		numActiveVoices++;
		m_pModuleServices->pLogger->writeLine("Voices allocated: %d", numActiveVoices);
	}
	else
		m_pModuleServices->pLogger->writeLine("ERROR: voice not allocated while no voice free");
}

VoiceProxy* VoiceLIFO::getFirstAllocated()
{
	VoiceProxy* first = firstAllocated;
	if (first == NULL)
		return NULL;
	while (first->simpleVoice.m_bFinalRelease)
	{
		first = first->nextAllocated;
		if (first == NULL)
			return NULL;
	}
	return first;
}

void VoiceLIFO::Deactivate()
{
	//VoiceProxy* first = firstAllocated;
	VoiceProxy* first = getFirstAllocated();
	if (first == NULL)
	{
		m_pModuleServices->pLogger->writeLine("ERROR: No voice allocated (voice id=%d)");
		return;
	}
	/*
	firstAllocated = first->nextAllocated;
	first->nextAllocated = NULL;
	*/
	first->deactivate();
}

void VoiceLIFO::Deallocate(VoiceProxy* toBeDeallocated)
{
	m_pModuleServices->pLogger->writeLine("Deallocate #%d", toBeDeallocated->id);
	toBeDeallocated = toBeDeallocated->nextAllocated;
	firstAllocated = toBeDeallocated;
	if (toBeDeallocated != NULL)
	{
		toBeDeallocated->nextAllocated = NULL;
	}
	numActiveVoices--;
	m_pModuleServices->pLogger->writeLine("Voices allocated: %d", numActiveVoices);
}

bool VoiceLIFO::isActive()
{
	for (int i = 0; i < NUM_VOICES; i++)
	{
		if (preallocatedVoices[i]->simpleVoice.m_bActive)
			return true;
	}
	return false;
}	

VoiceProxy* VoiceLIFO::getFirstAllocatedVoice()
{
	allocatedIterator = firstAllocated;
	return allocatedIterator;
}

VoiceProxy* VoiceLIFO::getNextAllocatedVoice()
{
	if (allocatedIterator == NULL)
		return NULL;
	if (allocatedIterator->nextAllocated == NULL)
		return NULL;
	allocatedIterator = allocatedIterator->nextAllocated;
	return allocatedIterator;
}
