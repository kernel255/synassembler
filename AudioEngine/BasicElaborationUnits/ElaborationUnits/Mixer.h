

#pragma once

#include "VirtualElaborationUnit.h"
#include "ConcretePort.h"
#include "MixerKind.h"

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

#define GET_INPUTMUTE_NAME(port) getInputMute_##port
#define SET_INPUTMUTE_NAME(port) setInputMute_##port

#define GET_INPUTMUTE_DEF(port) static void* getInputMute_##port(void* pEU) { return getInputMute(pEU, port); }
#define SET_INPUTMUTE_DEF(port) static bool setInputMute_##port(void* pEU, void* value) { return setInputMute(pEU, value, port); }

	GET_INPUTMUTE_DEF(0)
	GET_INPUTMUTE_DEF(1)
	GET_INPUTMUTE_DEF(2)
	GET_INPUTMUTE_DEF(3)
	SET_INPUTMUTE_DEF(0)
	SET_INPUTMUTE_DEF(1)
	SET_INPUTMUTE_DEF(2)
	SET_INPUTMUTE_DEF(3)

	static void* getInputMute(void* pEU, int index)
	{
		Mixer* pMix = (Mixer*)pEU;
		pMix->m_pModuleServices->pLogger->writeLine("Read Mixer # %d Input Mute: %s", index, pMix->m_InputMute[index] ? "on" : "off");
		return &(pMix->m_InputMute[index]);
	}
	static bool setInputMute(void* pEU, void* value, int index)
	{
		Mixer* pMix = (Mixer*)pEU;
		bool* mute = (bool*)value;
		pMix->m_InputMute[index] = *mute;
		pMix->m_pModuleServices->pLogger->writeLine("Write Mixer # %d Input Mute: %s", index, *mute ? "on" : "off");
		return true;
	}

	static void* getOutputMute(void* pEU)
	{
		Mixer* pMix = (Mixer*)pEU;
		pMix->m_pModuleServices->pLogger->writeLine("Read Mixer Output Mute: %s", pMix->m_OutputMute ? "on" : "off");
		return &(pMix->m_OutputMute);
	}
	static bool setOutputMute(void* pEU, void* value)
	{
		Mixer* pMix = (Mixer*)pEU;
		bool* mute = (bool*)value;
		pMix->m_OutputMute = *mute;
		pMix->m_pModuleServices->pLogger->writeLine("Write Mixer Output Mute: %s", *mute ? "on" : "off");
		return true;
	}

#define GET_INPUT_NAME(port) getInput_##port
#define SET_INPUT_NAME(port) setInput_##port

#define GET_INPUT_DEF(x) static void* getInput_##x(void* pEU) { return getInput(pEU, x); }
#define SET_INPUT_DEF(x) static bool setInput_##x(void* pEU, void* value) { return setInput(pEU, value, x); }

	GET_INPUT_DEF(0)
	GET_INPUT_DEF(1)
	GET_INPUT_DEF(2)
	GET_INPUT_DEF(3)
	SET_INPUT_DEF(0)
	SET_INPUT_DEF(1)
	SET_INPUT_DEF(2)
	SET_INPUT_DEF(3)

	static void* getInput(void* pEU, int index)
	{
		Mixer* pMix = (Mixer*)pEU;
		pMix->m_pModuleServices->pLogger->writeLine("Read Mixer # %d Input: %f", index, pMix->m_InputLevel[index]);
		return &(pMix->m_InputLevel[index]);
	}

	static bool setInput(void* pEU, void* value, int index)
	{
		Mixer* pMix = (Mixer*)pEU;
		double* amplitude = (double*)value;
		pMix->m_InputLevel[index] = *amplitude;
		pMix->m_pModuleServices->pLogger->writeLine("Write Mixer # %d Input: %f", index, *amplitude);
		return true;
	}

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

	ConcretePort* InputPorts[MixerKind::C_NumInputPorts];
	ConcretePort* OutputPort;
private:

	bool IsPortConnected(int port);
	ElaborationUnit* getConnectedEU(int port);

	double m_OutputLevel;
	double m_InputLevel[MixerKind::C_NumInputPorts];

	bool m_InputMute[MixerKind::C_NumInputPorts];
	bool m_OutputMute;

	class InputBuffers {
	public:
		InputBuffers() {
			for (int port = 0; port < MixerKind::C_NumInputPorts; port++)
			{
				m_pInputBuffers[port] = NULL;
				m_LastNumSamples = 0;
			}
		}
		int prepareBuffers(int nSamples)
		{
			//Only in case of bigger buffer the port buffers are reallocated, otherwise old ones are re-used
			if (nSamples > m_LastNumSamples)
			{
				m_LastNumSamples = nSamples;
				for (int port = 0; port < MixerKind::C_NumInputPorts; port++)
				{
					if (m_pInputBuffers[port] != NULL)
						delete [] m_pInputBuffers;
					m_pInputBuffers[port] = new EAG_SAMPLE_TYPE[nSamples];
					for (int i = 0; i < nSamples; i++)
					{
						m_pInputBuffers[port][i] = EAG_SAMPLE_ZERO;
					}
				}
				return 1;
			}
			return 0;
		}
		EAG_SAMPLE_TYPE* getNthBuffer(int port)
		{
			return m_pInputBuffers[port];
		}
	private:
		EAG_SAMPLE_TYPE* m_pInputBuffers[MixerKind::C_NumInputPorts];
		int m_LastNumSamples;
	};

	InputBuffers m_InputBuffers;
	bool IsPortMine(ElaborationUnitPort* port);
};