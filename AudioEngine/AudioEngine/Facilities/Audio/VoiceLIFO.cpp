
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

	firstAllocated = NULL;
	lastAllocated = NULL;
}

VoiceLIFO::~VoiceLIFO()
{
	for (int i = 0; i < NUM_VOICES; i++)
	{
		delete preallocatedVoices[i];
	}
}

// Scan all the voices until a free one is found
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
	{
		m_pModuleServices->pLogger->writeLine("No voice yet allocated");
		return NULL;
	}
	VoiceProxy* ptr = firstAllocated;
	while (ptr->nextAllocated != NULL)
	{
		ptr = ptr->nextAllocated;
	}
	return ptr;
}

void VoiceLIFO::Activate(double initialfreq, ADSR adsr)
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
			lastAllocated = firstFree;
			m_pModuleServices->pLogger->writeLine("First voice allocated");
		}
		else
		{
			firstFree->nextAllocated = NULL;
			lastAllocated->nextAllocated = firstFree;
			lastAllocated = firstFree;
			m_pModuleServices->pLogger->writeLine("Some voice already allocated");
			// Some other voice is already allocated
			/*
			VoiceProxy* latestAllocated = getLatestAllocated();
			latestAllocated->nextAllocated = firstFree;
			firstFree->nextAllocated = NULL;
			*/
		}

		firstFree->activate(initialfreq, adsr);
		numActiveVoices++;
		m_pModuleServices->pLogger->writeLine("Voices allocated: %d", numActiveVoices);
	}
	else
		m_pModuleServices->pLogger->writeLine("ERROR: voice not allocated while no free voice found");
}

VoiceProxy* VoiceLIFO::getFirstAllocated()
{
	VoiceProxy* first = firstAllocated;
	if (first == NULL)
	{
		m_pModuleServices->pLogger->writeLine("firstAllocated NULL");
		return NULL;
	}
	while (first->simpleVoice.m_bFinalRelease)
	{
		first = first->nextAllocated;
		if (first == NULL)
			return NULL;
	}
	return first;
}

VoiceProxy* VoiceLIFO::getFirstDeactivable()
{
	if (firstAllocated == NULL)
		return NULL;
	VoiceProxy* pVoice = firstAllocated;
	while (pVoice->isDeactivating())
	{
		pVoice = pVoice->nextAllocated;
		if (pVoice == NULL)
		{
			m_pModuleServices->pLogger->writeLine("ERROR: No voice deactivable!");
			return NULL;
		}
	}
	return pVoice;
}

void VoiceLIFO::Deactivate()
{
	if (firstAllocated == NULL)
	{
		m_pModuleServices->pLogger->writeLine("ERROR: no voice allocated, cannot deallocate");
		return;
	}
	VoiceProxy* pDeactivable = getFirstDeactivable();
	if (pDeactivable != NULL)
	{
		m_pModuleServices->pLogger->writeLine("%s Found deactivable: %d", __FUNCTION__, pDeactivable->id);
		pDeactivable->deactivate();
	}
}

VoiceProxy* VoiceLIFO::getMyPredecessor(VoiceProxy* myVProxy)
{
	if (firstAllocated == NULL)
		return NULL;
	VoiceProxy* pVProxy = firstAllocated;
	while (pVProxy != NULL)
	{
		if (pVProxy->nextAllocated == myVProxy)
			return pVProxy;
		pVProxy = pVProxy->nextAllocated;
	}
	return NULL;
}

void VoiceLIFO::Deallocate(VoiceProxy* toBeDeallocated)
{
	m_pModuleServices->pLogger->writeLine("Deallocate #%d", toBeDeallocated->id);
	// The voice to deallocate is the first
	if (toBeDeallocated == firstAllocated)
	{
		firstAllocated = toBeDeallocated->nextAllocated;
	}
	if (toBeDeallocated == lastAllocated)
	{
		VoiceProxy* pPrevious = getMyPredecessor(toBeDeallocated);
		if (pPrevious != NULL)
		{
			lastAllocated = pPrevious;
			pPrevious->nextAllocated = NULL;
		} 
		else
		{
			lastAllocated = NULL;
		}
	}

	VoiceProxy* pPrevious = getMyPredecessor(toBeDeallocated);
	if (pPrevious != NULL)
	{
		pPrevious->nextAllocated = toBeDeallocated->nextAllocated;
	}
	toBeDeallocated->nextAllocated = NULL;
	toBeDeallocated->freeVoice();

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
