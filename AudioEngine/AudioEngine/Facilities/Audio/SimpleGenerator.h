
#pragma once

#include "ConcretePort.h"
#include "VirtualElaborationUnit.h"
#include "SimpleVoice.h"
#include "VoiceLIFO.h"
#include "..\General\MutexProxy.h"
#include "..\Audio\LFO.h"

class SimpleGenerator : public VirtualElaborationUnit {
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

	LFO m_AmplitudeLFO;
	LFO m_FrequencyLFO;

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

};