
#pragma once

#include "VirtualElaborationUnit.h"
#include "ConcretePort.h"
#include "../Facilities/Audio/SimpleEnvelope.h"
#include "../Facilities/Audio/SimpleGenerator.h"
#include "../../AudioEngine/Facilities/Audio/WaveGeneratorFacilities.h"
#include "../../AudioEngine/Facilities/Audio/WaveKind.h"
#include "../../AudioEngine/Facilities/Audio/LFO.h"

class EUKind;
class OscillatorKind;
class ModuleServices;

/**
 *	\class Oscillator
 *	\brief Simple oscillator with preset waveforms
 */

class Oscillator : public SimpleGenerator
{
public:
	/**
	* \class Describe a MIDI voice
	*/
	Oscillator(ModuleServices* pService);
	//virtual	bool serialize(bool bSaving, class AbstractXMLParser* pParser, class AbstractXMLElement* pElement);
	virtual ~Oscillator();
	//virtual void updateAudioSamples(EAG_SAMPLE_TYPE *pSamplesBuffer,int numsamples);
	//virtual void receiveMIDIMessage(MIDIChannelMessage& midimsg);
	//Input/Output
	virtual bool setInputEU(ElaborationUnitPort* pPort, ElaborationUnit* pInputEU, ElaborationUnitPort* pInputPort);
	virtual bool setOutputEU(ElaborationUnitPort* pPort, ElaborationUnit* pOutputEU, ElaborationUnitPort* pOutputPort);
	virtual ElaborationUnitPort* getNthInputPort(int n);
	virtual ElaborationUnitPort* getNthOutputPort(int n);
	virtual int getInputPortNumber(void);
	virtual int getOutputPortNumber(void);
	virtual ElaborationUnitPort* getInputPortByEU(ElaborationUnit* pEU, int& n);
	virtual ElaborationUnitPort* getOutputPortByEU(ElaborationUnit* pEU, int& n);

	virtual void allocate(void);
	virtual void deallocate(void);
	virtual void play(void);
	virtual void pause(void);
	virtual void stop(void);
	virtual const EUKind* getKind(void);
	static const EUKind* s_GetKind(void);

	static void* getAmplitude(void* pEU) 
	{
		Oscillator* pOsc = (Oscillator *) pEU;
		pOsc->m_pModuleServices->pLogger->writeLine("Read Osc Amplitude: %f", pOsc->m_Amplitude);
		return &(pOsc->m_Amplitude);
	}
	static bool setAmplitude(void* pEU, void* value) {
		Oscillator* pOsc = (Oscillator *) pEU;
		double* amplitude = (double*) value;
		pOsc->m_Amplitude = *amplitude;
		char buf[50];
		memset(buf, 0, 50);
		sprintf(buf, "Write Osc Amplitude: %f", pOsc->m_Amplitude);
		pOsc->m_pModuleServices->pLogger->writeLine(buf);
		return true;
	}

	static void* getWaveKind(void* pEU)
	{
		Oscillator* pOsc = (Oscillator*)pEU;
		pOsc->m_pModuleServices->pLogger->writeLine("getWaveKind: %d", pOsc->m_WaveKind);
		return &(pOsc->m_WaveKind);
	}
	static bool setWaveKind(void* pEU, void* value)
	{
		Oscillator* pOsc = (Oscillator *)pEU;
		int* kind = (int*)value;
		pOsc->m_pModuleServices->pLogger->writeLine("setWaveKind: %d", *kind);
		pOsc->m_WaveKind = (WaveKind)(*kind);
		char buf[50];
		memset(buf, 0, 50);
		sprintf(buf, "Osc Wave: %d", *kind);
		pOsc->m_pModuleServices->pLogger->writeLine(buf);
		return true;
	}

protected:
	SimpleGenerator::SampleCalculationResult calculateSample(EAG_SAMPLE_TYPE& result, SimpleVoice& simpleVoice);

	static const int C_PhaseInIndex = 0;
	static const int C_AmplitudeInIndex = 1;
	static const int C_MIDIInIndex = 2;
	static const int C_MainOutIndex = 0;

	ConcretePort PhaseInPort;
	ConcretePort AmplitudeInPort;
	ConcretePort MIDIInPort;


	WaveKind m_WaveKind;
	double m_AmplitudePort;
	ConcretePort MainOutPort;
	double m_FreqAccumulator;
	double m_SamplingFrequency;
	double m_SamplingTime;
	bool m_bActive;
	int m_NumOn;
	int m_Acc;
	bool m_bLevelHigh;
	int m_SamplesBufferMaxSize;
	EAG_SAMPLE_TYPE* m_pPhaseInBuffer;
	EAG_SAMPLE_TYPE* m_pAmplitudeInBuffer;

	static const OscillatorKind kinna;
private:
	bool IsPortMine(ElaborationUnitPort* pPort);
	void initVoices(EAG_SAMPLE_TYPE updatePeriod);
};