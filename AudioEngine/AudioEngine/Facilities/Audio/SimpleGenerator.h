
#pragma once

#include "ConcretePort.h"
#include "VirtualElaborationUnit.h"
#include "SimpleVoice.h"
#include "VoiceLIFO.h"
#include "..\General\MutexProxy.h"
#include "..\Audio\LFO.h"

class SimpleGenerator : public VirtualElaborationUnit, FrequencyRetriever {
public:
	SimpleGenerator(ModuleServices* pService, int numProperties, const EUKind* kinna);
	virtual ~SimpleGenerator();
	virtual bool isFinalElaborationUnit();
	bool setInputEU(ElaborationUnitPort* pPort, ElaborationUnit* pInputEU, ElaborationUnitPort* pInputPort);
	bool setOutputEU(ElaborationUnitPort* pPort, ElaborationUnit* pOutputEU, ElaborationUnitPort* pOutputPort);
	void setSamplesBufferMaximumSize(int size);
	ElaborationUnitPort* getNthInputPort(int n);
	ElaborationUnitPort* getNthOutputPort(int n);
	ElaborationUnitPort* getInputPortByEU(ElaborationUnit* pEU, int& n);
	ElaborationUnitPort* getOutputPortByEU(ElaborationUnit* pEU, int& n);
	int getInputPortNumber(void);
	int getOutputPortNumber(void);
	virtual void receiveMIDIMessage(MIDIChannelMessage& midimsg);
	virtual void updateAudioSamples(EAG_SAMPLE_TYPE *pSamplesBuffer,int numsamples);
	virtual void allocate(void);
	virtual void deallocate(void);
protected:
	enum SampleCalculationResult {
		CALCULATION_CONTINUE,
		CALCULATION_COMPLETED
	};
	virtual SampleCalculationResult calculateSample(EAG_SAMPLE_TYPE& result, SimpleVoice& simpleVoice) = 0;
	bool IsPortMine(ElaborationUnitPort* pPort);
	void initVoices(EAG_SAMPLE_TYPE updatePeriod);

	static const int C_PhaseInIndex = 0;
	static const int C_AmplitudeInIndex = 1;
	static const int C_MIDIInIndex = 2;
	static const int C_MainOutIndex = 0;

	static const int C_DefaultBufferSize = 10;

	ConcretePort PhaseInPort;
	ConcretePort AmplitudeInPort;
	ConcretePort MainOutPort;
	ConcretePort MIDIInPort;

	LFO* m_pAmplitudeLFO;
	LFO* m_pFrequencyLFO;

	int m_SamplesBufferMaxSize;

	EAG_SAMPLE_TYPE* m_pPhaseInBuffer;
	EAG_SAMPLE_TYPE* m_pAmplitudeInBuffer;
	double m_SamplingFrequency;
	double m_SamplingTime;
	double m_Amplitude;

	int lastVoiceActNumber;

	/**
	 * @var Array of the voices, one for each note
	 */
	//SimpleVoice* m_pVoices[MIDIChannelMessage::NumMIDINotes];
	VoiceLIFO* m_pVoicesLIFO[MIDIChannelMessage::NumMIDINotes];

	MutexProxy* m_MutexProxy;

	bool playing;

	int transpose;
	int tune;

	// Functions to access to ADSR
	// AttackTime, DecayTime, ReleaseTime,
	// AttackLevel, SustainLevel
	ADSR adsr;
public:

	// ATTACK
	static void* getAttackTime(void* pEU)
	{
		SimpleGenerator* pSG = (SimpleGenerator *)pEU;
		pSG->m_pModuleServices->pLogger->writeLine("Read SG AT: %f", pSG->adsr.attackTime);
		return &(pSG->adsr.attackTime);
	}
	static bool setAttackTime(void* pEU, void* value) {
		SimpleGenerator* pSG = (SimpleGenerator *)pEU;
		double* d = (double*)value;
		pSG->adsr.attackTime = *d;
		char buf[50];
		memset(buf, 0, 50);
		sprintf(buf, "Write SG AT: %f", pSG->adsr.attackTime);
		pSG->m_pModuleServices->pLogger->writeLine(buf);
		return true;
	}

	// DECAY
	static void* getDecayTime(void* pEU)
	{
		SimpleGenerator* pSG = (SimpleGenerator *)pEU;
		pSG->m_pModuleServices->pLogger->writeLine("Read SG DT: %f", pSG->adsr.decayTime);
		return &(pSG->adsr.decayTime);
	}
	static bool setDecayTime(void* pEU, void* value) {
		SimpleGenerator* pSG = (SimpleGenerator *)pEU;
		double* d = (double*)value;
		pSG->adsr.decayTime = *d;
		char buf[50];
		memset(buf, 0, 50);
		sprintf(buf, "Write SG DT: %f", pSG->adsr.decayTime);
		pSG->m_pModuleServices->pLogger->writeLine(buf);
		return true;
	}

	// RELEASE
	static void* getReleaseTime(void* pEU)
	{
		SimpleGenerator* pSG = (SimpleGenerator *)pEU;
		pSG->m_pModuleServices->pLogger->writeLine("Read SG RT: %f", pSG->adsr.releaseTime);
		return &(pSG->adsr.releaseTime);
	}
	static bool setReleaseTime(void* pEU, void* value) {
		SimpleGenerator* pSG = (SimpleGenerator *)pEU;
		double* d = (double*)value;
		pSG->adsr.releaseTime = *d;
		char buf[50];
		memset(buf, 0, 50);
		sprintf(buf, "Write SG RT: %f", pSG->adsr.releaseTime);
		pSG->m_pModuleServices->pLogger->writeLine(buf);
		return true;
	}

	// ATTACK LEVEL
	static void* getAttackLevel(void* pEU)
	{
		SimpleGenerator* pSG = (SimpleGenerator *)pEU;
		pSG->m_pModuleServices->pLogger->writeLine("Read SG AL: %f", pSG->adsr.attackLevel);
		return &(pSG->adsr.attackLevel);
	}
	static bool setAttackLevel(void* pEU, void* value) {
		SimpleGenerator* pSG = (SimpleGenerator *)pEU;
		double* d = (double*)value;
		pSG->adsr.attackLevel = *d;
		char buf[50];
		memset(buf, 0, 50);
		sprintf(buf, "Write SG AL: %f", pSG->adsr.attackLevel);
		pSG->m_pModuleServices->pLogger->writeLine(buf);
		return true;
	}

	// SUSTAIN LEVEL
	static void* getSustainLevel(void* pEU)
	{
		SimpleGenerator* pSG = (SimpleGenerator *)pEU;
		pSG->m_pModuleServices->pLogger->writeLine("Read SG SL: %f", pSG->adsr.sustainLevel);
		return &(pSG->adsr.sustainLevel);
	}
	static bool setSustainLevel(void* pEU, void* value) {
		SimpleGenerator* pSG = (SimpleGenerator *)pEU;
		double* d = (double*)value;
		pSG->adsr.sustainLevel = *d;
		char buf[50];
		memset(buf, 0, 50);
		sprintf(buf, "Write SG SL: %f", pSG->adsr.sustainLevel);
		pSG->m_pModuleServices->pLogger->writeLine(buf);
		return true;
	}

	static void* getAmpLFOEnable(void* pEU)
	{
		SimpleGenerator* pSG = (SimpleGenerator *)pEU;
		pSG->m_pModuleServices->pLogger->writeLine("Read AMP LFO Ena: %d", pSG->m_pAmplitudeLFO->m_Enable);
		return &(pSG->m_pAmplitudeLFO->m_Enable);
	}
	static bool setAmpLFOEnable(void* pEU, void* value)
	{
		SimpleGenerator* pSG = (SimpleGenerator *)pEU;
		bool* b = (bool*)value;
		pSG->m_pAmplitudeLFO->m_Enable = *b;
		char buf[50];
		memset(buf, 0, 50);
		sprintf(buf, "Write AMP LFO Ena: %d", pSG->m_pAmplitudeLFO->m_Enable);
		pSG->m_pModuleServices->pLogger->writeLine(buf);
		return true;
	}

	static void* getFreqLFOEnable(void* pEU)
	{
		SimpleGenerator* pSG = (SimpleGenerator *)pEU;
		pSG->m_pModuleServices->pLogger->writeLine("Read Freq LFO Ena: %d", pSG->m_pFrequencyLFO->m_Enable);
		return &(pSG->m_pFrequencyLFO->m_Enable);
	}
	static bool setFreqLFOEnable(void* pEU, void* value)
	{
		SimpleGenerator* pSG = (SimpleGenerator *)pEU;
		bool* b = (bool*)value;
		pSG->m_pFrequencyLFO->m_Enable = *b;
		char buf[50];
		memset(buf, 0, 50);
		sprintf(buf, "Write Freq LFO Ena: %d", pSG->m_pFrequencyLFO->m_Enable);
		pSG->m_pModuleServices->pLogger->writeLine(buf);
		return true;
	}

	// AMP LFO LEVEL
	static void* getAmpLFOLevel(void* pEU)
	{
		SimpleGenerator* pSG = (SimpleGenerator *)pEU;
		pSG->m_pModuleServices->pLogger->writeLine("Read SG Amp LFO LVL: %f", pSG->m_pAmplitudeLFO->m_Amplitude);
		return &(pSG->m_pAmplitudeLFO->m_Amplitude);
	}
	static bool setAmpLFOLevel(void* pEU, void* value) {
		SimpleGenerator* pSG = (SimpleGenerator *)pEU;
		double* d = (double*)value;
		pSG->m_pAmplitudeLFO->m_Amplitude = *d;
		char buf[50];
		memset(buf, 0, 50);
		sprintf(buf, "Write SG Amp LFO LVL: %f", pSG->m_pAmplitudeLFO->m_Amplitude);
		pSG->m_pModuleServices->pLogger->writeLine(buf);
		return true;
	}

	// FREQ LFO LEVEL
	static void* getFreqLFOLevel(void* pEU)
	{
		SimpleGenerator* pSG = (SimpleGenerator *)pEU;
		pSG->m_pModuleServices->pLogger->writeLine("Read SG Freq LFO LVL: %f", pSG->m_pFrequencyLFO->m_Amplitude);
		return &(pSG->m_pFrequencyLFO->m_Amplitude);
	}
	static bool setFreqLFOLevel(void* pEU, void* value) {
		SimpleGenerator* pSG = (SimpleGenerator *)pEU;
		double* d = (double*)value;
		pSG->m_pFrequencyLFO->m_Amplitude = *d;
		char buf[50];
		memset(buf, 0, 50);
		sprintf(buf, "Write SG Freq LFO LVL: %f", pSG->m_pFrequencyLFO->m_Amplitude);
		pSG->m_pModuleServices->pLogger->writeLine(buf);
		return true;
	}

	// AMP LFO FREQ
	static void* getAmpLFOFreq(void* pEU)
	{
		SimpleGenerator* pSG = (SimpleGenerator *)pEU;
		pSG->m_pModuleServices->pLogger->writeLine("Read SG Amp LFO Freq: %f", pSG->m_pAmplitudeLFO->m_Frequency);
		return &(pSG->m_pAmplitudeLFO->m_Frequency);
	}
	static bool setAmpLFOFreq(void* pEU, void* value) {
		SimpleGenerator* pSG = (SimpleGenerator *)pEU;
		double* d = (double*)value;
		pSG->m_pAmplitudeLFO->m_Frequency = *d;
		char buf[50];
		memset(buf, 0, 50);
		sprintf(buf, "Write SG Amp LFO Freq: %f", pSG->m_pAmplitudeLFO->m_Frequency);
		pSG->m_pModuleServices->pLogger->writeLine(buf);
		return true;
	}

	// FREQ LFO FREQ
	static void* getFreqLFOFreq(void* pEU)
	{
		SimpleGenerator* pSG = (SimpleGenerator *)pEU;
		pSG->m_pModuleServices->pLogger->writeLine("Read Freq LFO Freq: %f", pSG->m_pFrequencyLFO->m_Frequency);
		return &(pSG->m_pFrequencyLFO->m_Frequency);
	}
	static bool setFreqLFOFreq(void* pEU, void* value) {
		SimpleGenerator* pSG = (SimpleGenerator *)pEU;
		double* d = (double*)value;
		pSG->m_pFrequencyLFO->m_Frequency = *d;
		char buf[50];
		memset(buf, 0, 50);
		sprintf(buf, "Write Freq LFO Freq: %f", pSG->m_pFrequencyLFO->m_Frequency);
		pSG->m_pModuleServices->pLogger->writeLine(buf);
		return true;
	}
	// TRANSPOSE
	static void* getTranspose(void* pEU)
	{
		SimpleGenerator* pSG = (SimpleGenerator*)pEU;
		pSG->m_pModuleServices->pLogger->writeLine("getTranspose: %d", pSG->transpose);
		return &(pSG->transpose);
	}
	static bool setTranspose(void* pEU, void* value)
	{
		SimpleGenerator* pSG = (SimpleGenerator *)pEU;
		int* tr = (int*)value;
		pSG->m_pModuleServices->pLogger->writeLine("setTranspose: %d", *tr);
		pSG->transpose = *tr;
		char buf[50];
		memset(buf, 0, 50);
		sprintf(buf, "Transpose: %d", *tr);
		pSG->m_pModuleServices->pLogger->writeLine(buf);
		return true;
	}

	static void* getTune(void* pEU)
	{
		SimpleGenerator* pSG = (SimpleGenerator*)pEU;
		pSG->m_pModuleServices->pLogger->writeLine("getTune: %d", pSG->tune);
		return &(pSG->tune);
	}
	static bool setTune(void* pEU, void* value)
	{
		SimpleGenerator* pSG = (SimpleGenerator *)pEU;
		int* tu = (int*)value;
		pSG->m_pModuleServices->pLogger->writeLine("setTune: %d", *tu);
		pSG->tune = *tu;
		char buf[50];
		memset(buf, 0, 50);
		sprintf(buf, "Tune: %d", *tu);
		pSG->m_pModuleServices->pLogger->writeLine(buf);
		return true;
	}

};