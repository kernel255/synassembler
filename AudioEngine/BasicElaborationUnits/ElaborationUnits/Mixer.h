
#pragma once

#include "VirtualElaborationUnit.h"
#include "ConcretePort.h"

class EUKind;
class MixerKind;
class ModuleServices;

/**
 * \class Mixer
*/

class Mixer : public VirtualElaborationUnit {
public:
	Mixer(ModuleServices* pServices);
	virtual ~Mixer();

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

	virtual void receiveMIDIMessage(MIDIChannelMessage& midimsg);
	virtual void updateAudioSamples(EAG_SAMPLE_TYPE *pSamplesBuffer, int numsamples);
	void setSamplesBufferMaximumSize(int size);

	static void* getOutput(void* pEU)
	{
		Mixer* pMix = (Mixer*)pEU;
		pMix->m_pModuleServices->pLogger->writeLine("Read Mixer Output: %f", pMix->m_OutputLevel);
		return &(pMix->m_OutputLevel);
	}
	static bool setOutput(void* pEU, void* value) {
		Mixer* pMix = (Mixer *)pEU;
		double* amplitude = (double*)value;
		pMix->m_OutputLevel = *amplitude;
		char buf[50];
		memset(buf, 0, 50);
		sprintf(buf, "Write Mixer Output: %f", pMix->m_OutputLevel);
		pMix->m_pModuleServices->pLogger->writeLine(buf);
		return true;
	}

	static const MixerKind kinna;

	double m_OutputLevel;
	double m_InputLevel[MixerKind::C_NumInputPorts];

	static const MixerKind kinna;

};