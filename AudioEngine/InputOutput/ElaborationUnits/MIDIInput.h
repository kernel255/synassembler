#pragma once

#include "..\gg.h"

#include <assert.h>
#include <windows.h>
#include <mmsystem.h>
#include "PhysicalElaborationUnit.h"
#include "ConcretePort.h"
#include "MIDIChannelMessage.h"
#include "MIDIInputKind.h"

class MIDIInput : public PhysicalElaborationUnit
{
friend DWORD WINAPI MIDIThreadProc(LPVOID param);
public:
	MIDIInput(ModuleServices* pService);
	virtual ~MIDIInput();
	//virtual bool serialize(bool bSaving, class AbstractXMLParser* pParser, class AbstractXMLElement* pElement);
	void SetMIDIInHandle(HMIDIIN hmidiin) { m_hMIDIIn = hmidiin; }
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
	virtual const EUKind* getKind(void);
	virtual void updateAudioSamples(EAG_SAMPLE_TYPE *pSamplesBuffer,int numsamples);
	virtual void receiveMIDIMessage(MIDIChannelMessage& midimsg);
	static unsigned int GetMIDIDevicesNumber(void);
	static EUKind* s_GetKind(void);
	static void CALLBACK MidiInProc(HMIDIIN hMidiIn,UINT wMsg,DWORD dwInstance,DWORD dwParam1,DWORD dwParam2);
	void SetThreadHandle(HANDLE h) { m_ThreadHandle = h; }
	virtual void setSamplesBufferMaximumSize(int size);
	//Declaration of PhysicalElaborationUnit methods
	virtual int GetAllocatedDevices(void);
	virtual const char* GetDeviceName(void);
	virtual int setProperty(int propertyIndex, const char* value);
	virtual int getProperty(int propertyIndex, char* value, int bufferSize);
private:
	void SendMIDIMessageToConnectedEUs(unsigned char* buffer, int size);
	static void CreateMIDIMessage(unsigned char* pbData, MIDIChannelMessage& midimsg);
	static const MIDIInputKind kinna;
	static unsigned int m_NumMIDIDevices;
	unsigned int m_MIDIInId;
	HMIDIIN m_hMIDIIn;
	HANDLE m_ThreadHandle;
protected:
	ConcretePort messagesOutPort;

};