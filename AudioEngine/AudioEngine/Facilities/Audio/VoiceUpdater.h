
#pragma once

/**
 * @class This class is used by the Voice class to update about the current voice status.
 * This class is the instrument to notify to the VoiceAllocator about the voice status, without the need of know directly the VoiceAllocator class, then avoiding the circular dependancy.
 */

class VoiceUpdater {
public:
	virtual void voiceCompleted(int voiceId) = 0;
};