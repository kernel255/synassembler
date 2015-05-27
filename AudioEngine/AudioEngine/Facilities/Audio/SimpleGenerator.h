
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
};