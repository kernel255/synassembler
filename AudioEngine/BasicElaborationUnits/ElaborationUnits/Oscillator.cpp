//#include <stdafx.h>
#include "Oscillator.h"
#include <string.h>
#include <math.h>
#include "OscillatorKind.h"
//#include "..\..\AudioEngineDLL\MIDIChannelMessage.h"
#include "MIDIChannelMessage.h"


const OscillatorKind Oscillator::kinna;

Oscillator::Oscillator(ModuleServices* pService) : SimpleGenerator(pService, Oscillator::kinna.getPropertyNumber(), &Oscillator::kinna),
PhaseInPort(ElaborationUnitPort::INPUT_PORT,ElaborationUnitPort::AUDIO_PORT,ElaborationUnitPort::SINGLE_PORT),
AmplitudeInPort(ElaborationUnitPort::INPUT_PORT,ElaborationUnitPort::AUDIO_PORT,ElaborationUnitPort::SINGLE_PORT),
MainOutPort(ElaborationUnitPort::OUTPUT_PORT,ElaborationUnitPort::AUDIO_PORT,ElaborationUnitPort::MULTI_PORT),
MIDIInPort(ElaborationUnitPort::INPUT_PORT,ElaborationUnitPort::MIDI_PORT,ElaborationUnitPort::SINGLE_PORT)
{
	pService->pLogger->writeLine("Oscillator constructor");
	iNumInput = 3;
	iNumOutput = 1;
	PhaseInPort.setName(OscillatorKind::PhasePortName);
	AmplitudeInPort.setName(OscillatorKind::AmplitudePortName);
	MainOutPort.setName(OscillatorKind::MainOutPortName);
	MIDIInPort.setName(OscillatorKind::MIDIPortName);
	m_WaveKind = WaveKind::e_Sine;
	//m_WaveKind = e_Square;
	m_Amplitude = 1.0;
	m_FreqAccumulator = 0.0;

	m_NumOn = 200;

	m_Acc = 0;
	m_bLevelHigh = true;

	m_SamplingFrequency = 48000.0;
	m_SamplingTime = 1.0 / m_SamplingFrequency;

	m_pPhaseInBuffer = NULL;
	m_pAmplitudeInBuffer = NULL;

	m_bActive = false;

	m_SamplesBufferMaxSize = 0;

	transpose = 0;
	tune = 0;

	EngineSettings* engineSettings = m_pModuleServices->getEngineSettings();
	double samplingPeriod = 1.0 / engineSettings->samplingFrequence;
	initVoices(samplingPeriod);
}

void Oscillator::initVoices(EAG_SAMPLE_TYPE updatePeriod)
{
}

Oscillator::~Oscillator()
{
	/*
	if( m_pPhaseInBuffer )
		delete m_pPhaseInBuffer;
	if( m_pAmplitudeInBuffer )
		delete m_pAmplitudeInBuffer;
	for(int i=0;i<MIDIChannelMessage::NumMIDINotes;i++)
	{
		delete m_pVoices[i];
	}
	*/
	this->m_pModuleServices->pLogger->writeLine("Oscillator destructor id=%d", iId);
}

#define POLYPHONIC_ATTENUATION .5

SimpleGenerator::SampleCalculationResult Oscillator::calculateSample(EAG_SAMPLE_TYPE& result, SimpleVoice& simpleVoice)
{
	result = WaveGeneratorFacilities::getSample(&simpleVoice, m_WaveKind, m_pModuleServices->getEngineSettings()->samplingFrequence, m_pFrequencyLFO);
	result *= POLYPHONIC_ATTENUATION;

	return CALCULATION_CONTINUE;
}

bool Oscillator::IsPortMine(ElaborationUnitPort* pPort)
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

ElaborationUnitPort* Oscillator::getNthInputPort(int n)
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

ElaborationUnitPort* Oscillator::getNthOutputPort(int n)
{
	if( n == C_MainOutIndex)
		return &MainOutPort;
	else
		return NULL;
}

bool Oscillator::setInputEU(ElaborationUnitPort* pPort, ElaborationUnit* pInputEU, ElaborationUnitPort* pInputPort)
{
	if( !IsPortMine(pPort) )
		return false;

	if( pPort == &PhaseInPort )
	{
		PhaseInPort.setNthEUandPort(pInputEU,pInputPort,0);
		pInputEU->setSamplesBufferMaximumSize(m_SamplesBufferMaxSize);
		return true;
	}
	else
	{
		if( pPort == &AmplitudeInPort )
		{
			AmplitudeInPort.setNthEUandPort(pInputEU,pInputPort,0);
			pInputEU->setSamplesBufferMaximumSize(m_SamplesBufferMaxSize);
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

bool Oscillator::setOutputEU(ElaborationUnitPort* pPort, ElaborationUnit* pOutputEU, ElaborationUnitPort* pOutputPort)
{
	if( pPort == &MainOutPort )
	{
		MainOutPort.setNthEUandPort(pOutputEU,pOutputPort,C_MainOutIndex);
		return true;
	}
	else
		return false;
}

ElaborationUnitPort* Oscillator::getInputPortByEU(ElaborationUnit* pEU, int& n)
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

ElaborationUnitPort* Oscillator::getOutputPortByEU(ElaborationUnit* pEU, int& n)
{
	ElaborationUnit* peu = MainOutPort.getNthEU(0);
	if( peu == pEU )
	{
		n = 1;
		return &MainOutPort;
	}
	return NULL;
}

int Oscillator::getInputPortNumber(void)
{
	return 3;
}

int Oscillator::getOutputPortNumber(void)
{
	return 1;
}

void Oscillator::allocate(void)
{
	SimpleGenerator::allocate();
}
void Oscillator::deallocate(void)
{
	SimpleGenerator::deallocate();
}

void Oscillator::play(void)
{
	int i = 0;
}

void Oscillator::pause(void)
{
}


void Oscillator::stop(void)
{
	this->m_pModuleServices->pLogger->writeLine("Stopping oscillator");
}
const EUKind* Oscillator::getKind(void)
{
	return s_GetKind();
}

const EUKind* Oscillator::s_GetKind(void)
{
	return (EUKind *) &kinna;
}

double Oscillator::GetCurrentFrequence()
{
	return MIDIChannelMessage::GetFreqByMIDINote(this->iNumInput, transpose, tune);
}