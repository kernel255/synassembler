
#pragma once

#include "ConcretePort.h"
#include "VirtualElaborationUnit.h"
#include "SimpleVoice.h"
#include "VoiceLIFO.h"
#include "..\General\MutexProxy.h"

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
	double m_AttackTime, m_DecayTime, m_ReleaseTime, m_AttackLevel, m_SustainLevel;

public:

	// ATTACK
	static void* getAttackTime(void* pEU)
	{
		SimpleGenerator* pSG = (SimpleGenerator *)pEU;
		pSG->m_pModuleServices->pLogger->writeLine("Read SG AT: %f", pSG->m_AttackTime);
		return &(pSG->m_AttackTime);
	}
	static bool setAttackTime(void* pEU, void* value) {
		SimpleGenerator* pSG = (SimpleGenerator *)pEU;
		double* d = (double*)value;
		pSG->m_AttackTime = *d;
		char buf[50];
		memset(buf, 0, 50);
		sprintf(buf, "Write SG AT: %f", pSG->m_AttackTime);
		pSG->m_pModuleServices->pLogger->writeLine(buf);
		return true;
	}

	// DECAY
	static void* getDecayTime(void* pEU)
	{
		SimpleGenerator* pSG = (SimpleGenerator *)pEU;
		pSG->m_pModuleServices->pLogger->writeLine("Read SG DT: %f", pSG->m_DecayTime);
		return &(pSG->m_DecayTime);
	}
	static bool setDecayTime(void* pEU, void* value) {
		SimpleGenerator* pSG = (SimpleGenerator *)pEU;
		double* d = (double*)value;
		pSG->m_DecayTime = *d;
		char buf[50];
		memset(buf, 0, 50);
		sprintf(buf, "Write SG DT: %f", pSG->m_DecayTime);
		pSG->m_pModuleServices->pLogger->writeLine(buf);
		return true;
	}

	// RELEASE
	static void* getReleaseTime(void* pEU)
	{
		SimpleGenerator* pSG = (SimpleGenerator *)pEU;
		pSG->m_pModuleServices->pLogger->writeLine("Read SG RT: %f", pSG->m_ReleaseTime);
		return &(pSG->m_ReleaseTime);
	}
	static bool setReleaseTime(void* pEU, void* value) {
		SimpleGenerator* pSG = (SimpleGenerator *)pEU;
		double* d = (double*)value;
		pSG->m_ReleaseTime = *d;
		char buf[50];
		memset(buf, 0, 50);
		sprintf(buf, "Write SG RT: %f", pSG->m_ReleaseTime);
		pSG->m_pModuleServices->pLogger->writeLine(buf);
		return true;
	}

	// ATTACK LEVEL
	static void* getAttackLevel(void* pEU)
	{
		SimpleGenerator* pSG = (SimpleGenerator *)pEU;
		pSG->m_pModuleServices->pLogger->writeLine("Read SG AL: %f", pSG->m_AttackLevel);
		return &(pSG->m_AttackLevel);
	}
	static bool setAttackLevel(void* pEU, void* value) {
		SimpleGenerator* pSG = (SimpleGenerator *)pEU;
		double* d = (double*)value;
		pSG->m_AttackLevel = *d;
		char buf[50];
		memset(buf, 0, 50);
		sprintf(buf, "Write SG AL: %f", pSG->m_AttackLevel);
		pSG->m_pModuleServices->pLogger->writeLine(buf);
		return true;
	}

	// SUSTAIN LEVEL
	static void* getSustainLevel(void* pEU)
	{
		SimpleGenerator* pSG = (SimpleGenerator *)pEU;
		pSG->m_pModuleServices->pLogger->writeLine("Read SG SL: %f", pSG->m_SustainLevel);
		return &(pSG->m_SustainLevel);
	}
	static bool setSustainLevel(void* pEU, void* value) {
		SimpleGenerator* pSG = (SimpleGenerator *)pEU;
		double* d = (double*)value;
		pSG->m_SustainLevel = *d;
		char buf[50];
		memset(buf, 0, 50);
		sprintf(buf, "Write SG SL: %f", pSG->m_SustainLevel);
		pSG->m_pModuleServices->pLogger->writeLine(buf);
		return true;
	}







};