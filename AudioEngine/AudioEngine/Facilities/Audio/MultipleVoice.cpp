#include "MultipleVoice.h"

MultipleVoice::MultipleVoice(ModuleServices* pServices)
{
	m_pModuleServices = pServices;
}

MultipleVoice::~MultipleVoice()
{
}

void MultipleVoice::activate(double initialFrequency)
{
	if (voiceStack.stackPos < STACK_DEPTH)
	{

	}
}

void MultipleVoice::deactivated(void)
{

}