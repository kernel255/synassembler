
#include "SimpleVoiceUpdater.h"
#include "VoiceAllocator.h"

SimpleVoiceUpdater::SimpleVoiceUpdater(VoiceAllocator* allocator)
{
	m_pAllocator = allocator;
}

void SimpleVoiceUpdater::voiceCompleted(int voiceId)
{
	m_pAllocator->voiceCompleted(voiceId);
}