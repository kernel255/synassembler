
#include "SimpleGenerator.h"

SimpleGenerator::SimpleGenerator(ModuleServices* pService, int numProperties, const EUKind* kinna) : VirtualElaborationUnit(pService, numProperties, kinna),
PhaseInPort(ElaborationUnitPort::INPUT_PORT,ElaborationUnitPort::AUDIO_PORT,ElaborationUnitPort::SINGLE_PORT),
AmplitudeInPort(ElaborationUnitPort::INPUT_PORT,ElaborationUnitPort::AUDIO_PORT,ElaborationUnitPort::SINGLE_PORT),
MainOutPort(ElaborationUnitPort::OUTPUT_PORT,ElaborationUnitPort::AUDIO_PORT,ElaborationUnitPort::MULTI_PORT),
MIDIInPort(ElaborationUnitPort::INPUT_PORT,ElaborationUnitPort::MIDI_PORT,ElaborationUnitPort::SINGLE_PORT)
{
	m_pPhaseInBuffer = NULL;
	m_pAmplitudeInBuffer = NULL;
	m_Amplitude = 1.0;
	EngineSettings* engineSettings = m_pModuleServices->getEngineSettings();
	m_SamplingTime = 1.0 / engineSettings->samplingFrequence;
	//m_SamplingTime = 1.0 / m_SamplingFrequency;
	initVoices(m_SamplingTime);
	lastVoiceActNumber = 0;
	playing = false;
	m_pAmplitudeLFO = new LFO(pService);
	m_pFrequencyLFO = new LFO(pService);
}

SimpleGenerator::~SimpleGenerator()
{
	if( m_pPhaseInBuffer )
		delete m_pPhaseInBuffer;
	if( m_pAmplitudeInBuffer )
		delete m_pAmplitudeInBuffer;
	for(int i=0;i<MIDIChannelMessage::NumMIDINotes;i++)
	{
		//delete m_pVoices[i];
		delete m_pVoicesLIFO[i];
	}
	m_pModuleServices->pLogger->writeLine("Simple generator destructor");

}

void SimpleGenerator::initVoices(EAG_SAMPLE_TYPE updatePeriod)
{
	for(int i=0;i<MIDIChannelMessage::NumMIDINotes;i++)
	{
		//m_pVoices[i] = new SimpleVoice(updatePeriod, C_DefaultBufferSize, m_pModuleServices);
		m_pVoicesLIFO[i] = new VoiceLIFO(updatePeriod, C_DefaultBufferSize, m_pModuleServices);
	}
}


bool SimpleGenerator::setInputEU(ElaborationUnitPort* pPort, ElaborationUnit* pInputEU, ElaborationUnitPort* pInputPort)
{
	if( !IsPortMine(pPort) )
		return false;

	if( pPort == &PhaseInPort )
	{
		PhaseInPort.setNthEUandPort(pInputEU,pInputPort,0);
		//pInputEU->setSamplesBufferMaximumSize(m_SamplesBufferMaxSize);
		return true;
	}
	else
	{
		if( pPort == &AmplitudeInPort )
		{
			AmplitudeInPort.setNthEUandPort(pInputEU,pInputPort,0);
			//pInputEU->setSamplesBufferMaximumSize(m_SamplesBufferMaxSize);
			return true;
		}
		else
		{
			if( pPort == &MIDIInPort )
			{
				MIDIInPort.setNthEUandPort(pInputEU,pInputPort,0);
				return true;
			}
			else
				return false;
		}
	}
	return false;
}

bool SimpleGenerator::IsPortMine(ElaborationUnitPort* pPort)
{
	if( (pPort == &PhaseInPort) ||
		(pPort == &AmplitudeInPort) ||
		(pPort == &MainOutPort) ||
		(pPort == &MIDIInPort)
		)
		return true;
	else
		return false;
}

bool SimpleGenerator::setOutputEU(ElaborationUnitPort* pPort, ElaborationUnit* pOutputEU, ElaborationUnitPort* pOutputPort)
{
	if( pPort == &MainOutPort )
	{
		MainOutPort.setNthEUandPort(pOutputEU,pOutputPort,C_MainOutIndex);
		return true;
	}
	else
		return false;
}

void SimpleGenerator::setSamplesBufferMaximumSize(int size)
{
	m_SamplesBufferMaxSize = size;
	//Allocate two new buffers to hold input samples
	if( m_pPhaseInBuffer )
		delete [] m_pPhaseInBuffer;
	if( m_pAmplitudeInBuffer )
		delete [] m_pAmplitudeInBuffer;
	m_pAmplitudeInBuffer = new EAG_SAMPLE_TYPE[size];
	m_pPhaseInBuffer = new EAG_SAMPLE_TYPE[size];
	//Tell the buffer size to connected EUs
	ElaborationUnitPort* pPort = PhaseInPort.getNthEUPort(0);
	if( pPort )
	{
		ElaborationUnit* pEU;
		pEU = PhaseInPort.getNthEU(0);
		pEU->setSamplesBufferMaximumSize(size);
	}
	else
		for(int i=0;i<size;i++)
			m_pPhaseInBuffer[i] = EAG_SAMPLE_ZERO;
	pPort = AmplitudeInPort.getNthEUPort(0);
	if( pPort )
	{
		ElaborationUnit* pEU;
		pEU = AmplitudeInPort.getNthEU(0);
		pEU->setSamplesBufferMaximumSize(size);
	}
	else
		for(int i=0;i<size;i++)
			m_pAmplitudeInBuffer[i] = EAG_SAMPLE_ONE;
}

ElaborationUnitPort* SimpleGenerator::getNthInputPort(int n)
{
	switch(n)
	{
		case C_PhaseInIndex:
			return &PhaseInPort;
		case C_AmplitudeInIndex:
			return &AmplitudeInPort;
		case C_MIDIInIndex:
			return &MIDIInPort;
		default:
			return NULL;
	}
}

ElaborationUnitPort* SimpleGenerator::getNthOutputPort(int n)
{
	if( n == C_MainOutIndex)
		return &MainOutPort;
	else
		return NULL;
}

ElaborationUnitPort* SimpleGenerator::getInputPortByEU(ElaborationUnit* pEU, int& n)
{
	if( MIDIInPort.getNthEU(0) == pEU )
	{
		n = 1;
		return &MIDIInPort;
	}
	if( AmplitudeInPort.getNthEU(0) == pEU )
	{
		n = 1;
		return &AmplitudeInPort;
	}
	if( PhaseInPort.getNthEU(0) == pEU )
	{
		n = 1;
		return &PhaseInPort;
	}
	return NULL;
}

ElaborationUnitPort* SimpleGenerator::getOutputPortByEU(ElaborationUnit* pEU, int& n)
{
	ElaborationUnit* peu = MainOutPort.getNthEU(0);
	if( peu == pEU )
	{
		n = 1;
		return &MainOutPort;
	}
	return NULL;
}

int SimpleGenerator::getInputPortNumber(void)
{
	return 3;
}

int SimpleGenerator::getOutputPortNumber(void)
{
	return 1;
}

void SimpleGenerator::receiveMIDIMessage(MIDIChannelMessage& midimsg)
{
	switch( midimsg.Kind )
	{
	case MIDIChannelMessage::e_NoteOn:
		{
			//When NoteOn is received with velocity 0, is a note off
			if( midimsg.data.NoteMessage.Velocity == 0 )
			{
				m_pModuleServices->pLogger->writeLine("Waiting Mutex # 1");
				int res = m_MutexProxy->WaitMutex(MutexProxy::WAIT_INFINITE);
				m_pModuleServices->pLogger->writeLine("Elapsed: %d", res);
				m_pModuleServices->pLogger->writeLine("Note Off %d", midimsg.data.NoteMessage.Note);
				m_pVoicesLIFO[midimsg.data.NoteMessage.Note]->Deactivate();
				m_MutexProxy->ReleaseMutex();
			}
			else
			{
				//This is a true NoteOn
				m_pModuleServices->pLogger->writeLine("Waiting Mutex # 1");
				int res = m_MutexProxy->WaitMutex(MutexProxy::WAIT_INFINITE);
				m_pModuleServices->pLogger->writeLine("Elapsed: %d", res);
				m_pModuleServices->pLogger->writeLine("Note On %d", midimsg.data.NoteMessage.Note);
				m_pAmplitudeLFO->setCarrierFrequence(midimsg.data.NoteMessage.Frequency);
				m_pFrequencyLFO->setCarrierFrequence(midimsg.data.NoteMessage.Frequency);
				m_pVoicesLIFO[midimsg.data.NoteMessage.Note]->Activate(midimsg.data.NoteMessage.Frequency, adsr);
				m_MutexProxy->ReleaseMutex();
			}
			break;
		}
	case MIDIChannelMessage::e_NoteOff:
		{
			m_pModuleServices->pLogger->writeLine("Waiting Mutex # 1");
			m_MutexProxy->WaitMutex(MutexProxy::WAIT_INFINITE);
			m_pModuleServices->pLogger->writeLine("Note Off %d", midimsg.data.NoteMessage.Note);
			m_pVoicesLIFO[midimsg.data.NoteMessage.Note]->Deactivate();
			int res = m_MutexProxy->ReleaseMutex();
			m_pModuleServices->pLogger->writeLine("Elapsed: %d", res);
			break;
		}
	default:
		{
			break;
		}
	}
}

void SimpleGenerator::updateAudioSamples(EAG_SAMPLE_TYPE *pSamplesBuffer,int numsamples)
{
	//Reset to silence the output buffer
	for(int i=0;i<numsamples;++i)
	{
		pSamplesBuffer[i] = EAG_SAMPLE_ZERO;
	}
	if (m_MutexProxy == NULL)
		return;
	//Read inputs buffer if available
	ElaborationUnitPort* pPort;
	ElaborationUnit* pEU;
	pPort = PhaseInPort.getNthEUPort(0);
	if( pPort )
	{
		pEU = PhaseInPort.getNthEU(0);
		pEU->updateAudioSamples(m_pPhaseInBuffer,numsamples);
	}
	pPort = AmplitudeInPort.getNthEUPort(0);
	if( pPort )
	{
		pEU = AmplitudeInPort.getNthEU(0);
		pEU->updateAudioSamples(m_pAmplitudeInBuffer,numsamples);
	}
	//Now It's time for processing...
	for(int noteIndex=0;noteIndex<MIDIChannelMessage::NumMIDINotes;++noteIndex)
	{
		//m_pModuleServices->pLogger->writeLine("Waiting mutex on updateSamples");
		int res = m_MutexProxy->WaitMutex(MutexProxy::WAIT_INFINITE);
		if (res!=WAIT_OBJECT_0)
			m_pModuleServices->pLogger->writeLine("Elapsed updateSamples: %d", res);
		if (m_pVoicesLIFO[noteIndex]->isActive())
		{
			for (int sampleIndex = 0; sampleIndex < numsamples; sampleIndex++)
			{
				int numLoops = 0;
				VoiceProxy* voiceIterator = m_pVoicesLIFO[noteIndex]->getFirstAllocatedVoice();
				while (voiceIterator != NULL)
				{
					EAG_SAMPLE_TYPE envLevel = voiceIterator->simpleVoice.m_pEnvelope->getUpdatedLevel();
					if (voiceIterator->simpleVoice.m_bFinalRelease)
					{
						if (voiceIterator->simpleVoice.m_pEnvelope->releaseCompleted())
						{
							voiceIterator->simpleVoice.dispose();
							m_pVoicesLIFO[noteIndex]->Deallocate(voiceIterator);
							break;
						}
					}
					EAG_SAMPLE_TYPE currSample;
					// The value CALCULATION_COMPLETED is returned in case of fixed len samples
					if (calculateSample(currSample, voiceIterator->simpleVoice) == CALCULATION_COMPLETED)
					{
						voiceIterator->simpleVoice.dispose();
					}
					else
					{
						if (envLevel < 0.0)
							envLevel = 0.0;
						double lfoAmpl = 1.0;
						if (m_pAmplitudeLFO->m_Enable)
						{
							lfoAmpl = m_pAmplitudeLFO->getSample(m_SamplingFrequency);
							lfoAmpl *= m_pAmplitudeLFO->m_Amplitude;
						}
						m_pAmplitudeLFO->increaseAccumulatedTime(m_SamplingTime);
						/*
						double lfoFreq = 0.0;
						if (m_FrequencyLFO.m_Enable)
							lfoFreq = m_FrequencyLFO.getSample(m_SamplingFrequency);
						*/
						m_pFrequencyLFO->increaseAccumulatedTime(m_SamplingTime);
						// Mixing between Amp modulated signal and normal signal
						double currentAmplifiedSample = m_Amplitude*envLevel*m_pAmplitudeInBuffer[sampleIndex] * currSample;
						currentAmplifiedSample = (currentAmplifiedSample + lfoAmpl*currentAmplifiedSample) / 2.0;
						pSamplesBuffer[sampleIndex] += currentAmplifiedSample;
						voiceIterator->simpleVoice.increaseAccumulatedTime(m_SamplingTime);
					}
					voiceIterator = m_pVoicesLIFO[noteIndex]->getNextAllocatedVoice();
					numLoops++;
				}
			}
		}
		m_MutexProxy->ReleaseMutex();
	}

}

bool SimpleGenerator::isFinalElaborationUnit()
{
	return false;
}

void SimpleGenerator::allocate()
{
	m_MutexProxy = new MutexProxy(NULL);
	playing = true;
}

void SimpleGenerator::deallocate()
{
	m_MutexProxy->ReleaseMutex();
	delete m_MutexProxy;
	m_MutexProxy = NULL;
	playing = false;
}
