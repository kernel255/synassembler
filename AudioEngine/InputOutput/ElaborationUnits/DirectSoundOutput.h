#pragma once

#define DEBUG_OUTPUT 1

#include "..\gg.h"

#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include "AudioOutput.h"
#include "ConcretePort.h"
#include "MIDIChannelMessage.h"
#include "..\Facilities\Audio\WaveLogger.h"

class DirectSoundOutputKind;
class EUKind;
class ModuleServices;

/**
 * \class DirectSoundOutput 
 * \brief Concrete class for MS DirectSound Audio Output
 */

class DirectSoundOutput : public AudioOutput
{
friend DWORD WINAPI NotifyThreadProc(LPVOID param);
public:
	static const int C_MainInIndex;
	DirectSoundOutput(ModuleServices* pService);
	//virtual bool serialize(bool bSaving, class AbstractXMLParser* pParser, class AbstractXMLElement* pElement);
	virtual ~DirectSoundOutput();
	void updateAudioSamples(EAG_SAMPLE_TYPE *pSamplesBuffer,int numsamples);
	void receiveMIDIMessage(MIDIChannelMessage& midimsg);
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
	const EUKind* getKind(void);
	static EUKind* s_GetKind(void);
	virtual int GetAllocatedDevices(void);
	virtual const char* GetDeviceName(void);
	virtual bool AllocateDevice(void);
	static int m_SamplingFrequency;
	static int m_BitsResolution;
	static const int m_DefaultWaveFormat;
	static int m_NumChannels;
	static int m_BufferSize;
	static int m_HalfBufferSize;
	static bool createSoundBuffers(DirectSoundOutput* pdirectsoundoutput, LPDIRECTSOUND8 pDSound8);
	bool IsActive(void) { return m_bActive; }
	virtual void setSamplesBufferMaximumSize(int size);
	virtual int setProperty(int propertyIndex, const char* value);
	virtual int getProperty(int propertyIndex, char* value, int bufferSize);
private:
	static const DirectSoundOutputKind kinna;
	//DirectSound specific data structs
	LPDIRECTSOUNDBUFFER m_pSoundBuffer;
	DSBPOSITIONNOTIFY m_Notify[2];
	HANDLE m_hNotifyHandles[2];
	LPDIRECTSOUNDNOTIFY m_pDirectSoundNotify;
	LPDIRECTSOUND8 m_pDirectSound8;
	bool m_bActive;
	HANDLE m_hThreadHandle;
	bool UpdateSoundBuffer(int numsection);
	static int getNumSampleByBytes(int bufferSize);
	static int getBytesByNumSamples(int numSamples);
	/*
	* @var Buffer to hold the samples to be sent to DirectSound
	*/
	EAG_SAMPLE_TYPE* m_pSamplesBuffer;
	/**
	* @var Number of samples hold by the DirectSound output
	*/
	int m_SoundBufferNumSamples;
	/********DEBUG FACILITIES*******/
#ifdef DEBUG_OUTPUT
	WaveLogger* waveLogger;
#endif //DEBUG_OUPUT
	/********DEBUG FACILITIES*******/
protected:
	ConcretePort MainInPort;
private:
	short* debugWaveSamples;
	short* debugWaveSamplesPtr;
	int debugWaveNumSamples;
};
