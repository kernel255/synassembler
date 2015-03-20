
#include "AudioOutput.h"

AudioOutput::AudioOutput(ModuleServices* pService, int numProperties, const EUKind* kind) : PhysicalElaborationUnit(pService, numProperties, kind)
{
	iNumInput = 1;
	iNumOutput = 0;
}

AudioOutput::~AudioOutput()
{
}

void AudioOutput::Allocate(void)
{
}
void AudioOutput::Deallocate(void)
{
}
void AudioOutput::Play(void)
{
}
void AudioOutput::Pause(void)
{
}