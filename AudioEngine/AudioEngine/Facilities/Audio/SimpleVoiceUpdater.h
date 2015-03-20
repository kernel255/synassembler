
#pragma once

#include "VoiceUpdater.h"


// Even if this reference is very "light" maybe there is a cleaner way to do this
class VoiceAllocator;

/**
 * @class This class the simplest implementation of the VoiceUpdater
 */

class SimpleVoiceUpdater : public VoiceUpdater
{
private:
	VoiceAllocator* m_pAllocator;
public:
	SimpleVoiceUpdater(VoiceAllocator* allocator);
	virtual void voiceCompleted(int voiceId);
};