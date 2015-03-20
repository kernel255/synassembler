#pragma once

#include <AbstractElaborationUnit.h>

#include "PhysicalElaborationUnit.h"
#include "ConcretePort.h"
#include "MIDIChannelMessage.h"
#include "PCKeyboardKind.h"

#define NOTE_NUMBER 128

class PCKeyboard : public PhysicalElaborationUnit
{
public:
	PCKeyboard(ModuleServices* pService);
	virtual ~PCKeyboard();
	void updateAudioSamples(EAG_SAMPLE_TYPE *pSamplesBuffer, int numsamples);
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
	virtual void setSamplesBufferMaximumSize(int size);

	static const PCKeyboardKind kinna;
	virtual int setProperty(int propertyIndex, const char* value);
	virtual int getProperty(int propertyIndex, char* value, int bufferSize);
protected:
	ConcretePort messagesOutPort;
	void receiveFromSerial(char key);
private:
	bool noteOn[NOTE_NUMBER];
	bool isNoteMsgNotDuplicated(MIDIChannelMessage midimsg);
};
