
#include "Mixer.h"
#include "MixerKind.h"

const MixerKind Mixer::kinna;

Mixer::Mixer(ModuleServices* pServices) : VirtualElaborationUnit(pServices, kinna.getPropertyNumber(), &Mixer::kinna)
{
	// Init ports
	OutputPort = new ConcretePort(ElaborationUnitPort::OUTPUT_PORT, ElaborationUnitPort::AUDIO_PORT, ElaborationUnitPort::SINGLE_PORT);
	for (int port = 0; port < MixerKind::C_NumInputPorts; port++)
	{
		InputPorts[port] = new ConcretePort(ElaborationUnitPort::INPUT_PORT, ElaborationUnitPort::AUDIO_PORT, ElaborationUnitPort::SINGLE_PORT);
	}
	m_OutputLevel = 1.0;
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
	if (n < MixerKind::C_NumInputPorts)
	{
		return InputPorts[n];
	}
	return NULL;
}

ElaborationUnitPort* Mixer::getNthOutputPort(int n)
{
	if (n == 0)
		return OutputPort;
	return NULL;
}

int Mixer::getInputPortNumber(void)
{
	return MixerKind::C_NumInputPorts;
}

int Mixer::getOutputPortNumber(void)
{
	return 1;
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
	m_InputBuffers.prepareBuffers(numsamples);
	for (int port = 0; port < MixerKind::C_NumInputPorts; port++)
	{
		ElaborationUnitPort* pEUPort = getNthInputPort(port);
		ElaborationUnit* pEU = pEUPort->getNthEU(0);
		if (pEU != NULL)
		{
			EAG_SAMPLE_TYPE* buffer = m_InputBuffers.getNthBuffer(port);
			pEU->updateAudioSamples(buffer, numsamples);
		}
	}

	for (int i = 0; i < numsamples; i++)
	{
		EAG_SAMPLE_TYPE acc = 0.0;
		for (int port = 0; port < MixerKind::C_NumInputPorts; port++)
		{
			acc += m_InputBuffers.getNthBuffer(port)[i] * m_InputLevel[port];
		}
		acc *= m_OutputLevel;
		pSamplesBuffer[i] = acc;
	}
}

void Mixer::setSamplesBufferMaximumSize(int size)
{

}
