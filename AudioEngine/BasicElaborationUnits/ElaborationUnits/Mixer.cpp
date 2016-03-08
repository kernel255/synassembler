
#include "Mixer.h"
#include "MixerKind.h"

const MixerKind Mixer::kinna;

Mixer::Mixer(ModuleServices* pServices) : VirtualElaborationUnit(pServices, kinna.getPropertyNumber(), &Mixer::kinna)
{

}

Mixer::~Mixer()
{

}

bool Mixer::setInputEU(ElaborationUnitPort* pPort, ElaborationUnit* pInputEU, ElaborationUnitPort* pInputPort)
{
	return true;
}

bool Mixer::setOutputEU(ElaborationUnitPort* pPort, ElaborationUnit* pOutputEU, ElaborationUnitPort* pOutputPort)
{
	return true;
}

ElaborationUnitPort* Mixer::getNthInputPort(int n)
{
	return NULL;
}

ElaborationUnitPort* Mixer::getNthOutputPort(int n)
{
	return NULL;
}

int Mixer::getInputPortNumber(void)
{
	return 0;
}

int Mixer::getOutputPortNumber(void)
{
	return 0;
}

ElaborationUnitPort* Mixer::getInputPortByEU(ElaborationUnit* pEU, int& n)
{
	return NULL;
}

ElaborationUnitPort* Mixer::getOutputPortByEU(ElaborationUnit* pEU, int& n)
{
	return NULL;
}

void Mixer::allocate(void)
{

}

void Mixer::deallocate(void)
{

}

void Mixer::play(void)
{

}

void Mixer::pause(void)
{

}

void Mixer::stop(void)
{

}

const EUKind* Mixer::getKind(void)
{
	return s_GetKind();
}

const EUKind* Mixer::s_GetKind(void)
{
	return (EUKind *)&kinna;
}

void Mixer::receiveMIDIMessage(MIDIChannelMessage& midimsg) 
{

}

void Mixer::updateAudioSamples(EAG_SAMPLE_TYPE *pSamplesBuffer, int numsamples)
{

}

void Mixer::setSamplesBufferMaximumSize(int size)
{

}
