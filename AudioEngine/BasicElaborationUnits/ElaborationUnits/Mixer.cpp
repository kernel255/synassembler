
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
	for (int port = 0; port < MixerKind::C_NumInputPorts; port++)
		m_InputLevel[port] = 1.0;
}

Mixer::~Mixer()
{
	m_pModuleServices->pLogger->writeLine("Mixer destructor");
}

bool Mixer::IsPortMine(ElaborationUnitPort* port)
{
	if (port == OutputPort)
		return true;
	for (int i = 0; i < MixerKind::C_NumInputPorts; i++)
	{
		if (port == InputPorts[i])
			return true;
	}
	return false;
}

bool Mixer::setInputEU(ElaborationUnitPort* pPort, ElaborationUnit* pInputEU, ElaborationUnitPort* pInputPort)
{
	if (!IsPortMine(pPort))
		return false;
	for (int port = 0; port < MixerKind::C_NumInputPorts; port++)
	{
		if (InputPorts[port] == pPort)
		{
			InputPorts[port]->setNthEUandPort(pInputEU, pInputPort, 0);
			pInputEU->setSamplesBufferMaximumSize(0);
			return true;
		}
	}
	return false;
}

bool Mixer::setOutputEU(ElaborationUnitPort* pPort, ElaborationUnit* pOutputEU, ElaborationUnitPort* pOutputPort)
{
	if (!IsPortMine(pPort))
		return false;
	if (pPort == OutputPort)
	{
		OutputPort->setNthEUandPort(pOutputEU, pOutputPort, 0);
		return true;
	}
	return false;
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
	for (int port = 0; port < MixerKind::C_NumInputPorts; port++)
	{
		if (InputPorts[port]->getNthEU(0) == pEU)
		{
			n = 1;
			return InputPorts[port];
		}
	}
	return NULL;
}

ElaborationUnitPort* Mixer::getOutputPortByEU(ElaborationUnit* pEU, int& n)
{
	if (OutputPort->getNthEU(0) == pEU)
	{
		n = 1;
		return OutputPort;
	}
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

bool Mixer::IsPortConnected(int port)
{
	ElaborationUnitPort* pEUPort = getNthInputPort(port);
	ElaborationUnit* pEU = pEUPort->getNthEU(0);
	return pEU != NULL;
}

ElaborationUnit* Mixer::getConnectedEU(int port)
{
	ElaborationUnitPort* pEUPort = getNthInputPort(port);
	ElaborationUnit* pEU = pEUPort->getNthEU(0);

	return pEU;
}

void Mixer::updateAudioSamples(EAG_SAMPLE_TYPE *pSamplesBuffer, int numsamples)
{
	int reallocBuffer = m_InputBuffers.prepareBuffers(numsamples);
	for (int port = 0; port < MixerKind::C_NumInputPorts; port++)
	{
		ElaborationUnit* pEU = getConnectedEU(port);
		if (pEU!=NULL)
		{
			if (reallocBuffer)
				pEU->setSamplesBufferMaximumSize(numsamples);
			pEU->updateAudioSamples(m_InputBuffers.getNthBuffer(port), numsamples);
		}
	}

	for (int i = 0; i < numsamples; i++)
	{
		EAG_SAMPLE_TYPE acc = 0.0;
		for (int port = 0; port < MixerKind::C_NumInputPorts; port++)
		{
			if(IsPortConnected(port))
			acc += m_InputBuffers.getNthBuffer(port)[i] * m_InputLevel[port];
		}
		acc *= m_OutputLevel;
		pSamplesBuffer[i] = acc;
	}
}

void Mixer::setSamplesBufferMaximumSize(int size)
{

}
