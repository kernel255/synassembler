
#include "PCKeyboard.h"

//NOTE THE THREAD PROC STILL NEED TO BE STARTED!



PCKeyboard::PCKeyboard(ModuleServices* pService) : PhysicalElaborationUnit(pService, PCKeyboard::kinna.getPropertyNumber(), &PCKeyboard::kinna),
messagesOutPort(ElaborationUnitPort::OUTPUT_PORT, ElaborationUnitPort::MIDI_PORT, ElaborationUnitPort::MULTI_PORT)
{
	for (int i = 0; i < NOTE_NUMBER; i++)
		noteOn[i] = false;
	messagesOutPort.setName(PCKeyboardKind::PC_OUT_PORT_NAME);
}

PCKeyboard::~PCKeyboard()
{
	m_pModuleServices->pLogger->writeLine("PCKeyboard destructor id=%d", iId);
}

void PCKeyboard::updateAudioSamples(EAG_SAMPLE_TYPE *pSamplesBuffer, int numsamples)
{
}

bool PCKeyboard::isNoteMsgNotDuplicated(MIDIChannelMessage midimsg)
{
	/*
	if (midimsg.Kind == midimsg.e_NoteOn)
	{
		if (!noteOn[midimsg.data.NoteMessage.Note])
			return true;
		else
			return false;
	}
	if (midimsg.Kind == midimsg.e_NoteOff)
	{
		if (noteOn[midimsg.data.NoteMessage.Note])
			return true;
		else
			return false;
	}
	*/
	return true;
}

void PCKeyboard::receiveMIDIMessage(MIDIChannelMessage& midimsg)
{
	//Propagate MIDI messages to all the connected outputs
	int numports = messagesOutPort.getConnectionsNumber();
	if (isNoteMsgNotDuplicated(midimsg))
	{
		if (midimsg.Kind == midimsg.e_NoteOn)
			noteOn[midimsg.data.NoteMessage.Note] = true;
		if (midimsg.Kind == midimsg.e_NoteOff)
			noteOn[midimsg.data.NoteMessage.Note] = false;
		for (int i = 0; i < numports; ++i)
		{
			ElaborationUnit* pEU = messagesOutPort.getNthEU(i);
			if (pEU)
			{
				pEU->receiveMIDIMessage(midimsg);
			}
		}
	}
}

bool PCKeyboard::setInputEU(ElaborationUnitPort* pPort, ElaborationUnit* pInputEU, ElaborationUnitPort* pInputPort)
{
	return false;
}

bool PCKeyboard::setOutputEU(ElaborationUnitPort* pPort, ElaborationUnit* pOutputEU, ElaborationUnitPort* pOutputPort)
{
	if (pPort == &messagesOutPort)
	{
		messagesOutPort.setNthEUandPort(pOutputEU, pOutputPort, C_AddNewConnection);
		return true;
	}
	else
		return false;

}

ElaborationUnitPort* PCKeyboard::getNthInputPort(int n)
{
	return NULL;
}

ElaborationUnitPort* PCKeyboard::getNthOutputPort(int n)
{
	if (n == 0)
		return &messagesOutPort;
	else
		return NULL;

}

int PCKeyboard::getInputPortNumber(void)
{
	return 0;
}

int PCKeyboard::getOutputPortNumber(void)
{
	return 1;
}

ElaborationUnitPort* PCKeyboard::getInputPortByEU(ElaborationUnit* pEU, int& n)
{
	return NULL;
}

ElaborationUnitPort* PCKeyboard::getOutputPortByEU(ElaborationUnit* pEU, int& n)
{
	int numports = messagesOutPort.getConnectionsNumber();
	//Try with all the connected EUs
	for (int i = 0; i<numports; ++i)
	{
		if (messagesOutPort.getNthEU(i) == pEU)
			return &messagesOutPort;
	}
	return NULL;
}

void PCKeyboard::allocate(void)
{
}

void PCKeyboard::deallocate(void)
{
}

void PCKeyboard::play(void)
{
}

void PCKeyboard::pause(void)
{
}

void PCKeyboard::stop(void)
{

}

const EUKind* PCKeyboard::getKind(void)
{
	return s_GetKind();
}

EUKind* PCKeyboard::s_GetKind(void)
{
	return (EUKind *)&PCKeyboard::kinna;
}

int PCKeyboard::GetAllocatedDevices(void)
{
	return 0;
}

const char* PCKeyboard::GetDeviceName(void)
{
	return NULL;
}

bool PCKeyboard::AllocateDevice(void)
{
	return false;
}

void PCKeyboard::setSamplesBufferMaximumSize(int size)
{
}

#define NOTE_ON_FLAG	0x40
//TODO: Recalculate this value, accordingly to the real PolyTrack keyboard
#define MIDI_C2			30

void PCKeyboard::receiveFromSerial(char key)
{
	MIDIChannelMessage midiMsg;
	//TODO: fix this stuff accordingly to the PIC values received
	int column = key & 0xF8;
	int row = key & 0x07;
	int note = column * 8 + row;
	note += MIDI_C2;
	//TODO: Provide a way to change MIDI channel
	midiMsg.channel = 0;
	if (key & NOTE_ON_FLAG)
		midiMsg.Kind = MIDIChannelMessage::e_NoteOn;
	else
		midiMsg.Kind = MIDIChannelMessage::e_NoteOff;
	receiveMIDIMessage(midiMsg);
}

#define READ_TIMEOUT 200

/*
DWORD WINAPI PCKeyboardThreadProc(LPVOID param)
{
	PolyKeyboard* pPolyKey = (PolyKeyboard *)param;
	char key;
	DWORD dwLen, dwRead;

	while (1)
	{
		dwRead = ::ReadFile(pPolyKey->hSerial, &key, 1, &dwLen, NULL);
		if (!dwRead)
		{
			DWORD error = ::GetLastError();
			if (error != ERROR_IO_PENDING)
			{
				std::string str;
				str = "Error while reading";
				char buffer[10];
				sprintf_s(buffer, 10, "%d\0", error);
				str += buffer;
				pPolyKey->m_pModuleServices->pLogger->writeLine(str.c_str());
				return 0;
			}
		}
		else
		{
			if (dwLen != 0)
			{
				pPolyKey->receiveFromSerial(key);
				printf("Receiving: 0x%02x received size: %d", key, dwLen);
			}
		}
	}
}
*/

/*
DWORD WINAPI PolyKeyboardThreadProc(LPVOID param)
{
PolyKeyboard* pPolyKey = (PolyKeyboard *) param;
char key;
DWORD dwLen, dwRead;

while(1)
{
dwRead = ::ReadFile(pPolyKey->hSerial, &key, 1, &dwLen, &(pPolyKey->osReader));
if(!dwRead)
{
DWORD error = ::GetLastError();
if(error != ERROR_IO_PENDING)
{
std::string str;
str = "Error while reading";
char buffer[10];
sprintf(buffer, "%d\0", error);
str += buffer;
pPolyKey->m_pModuleServices->pLogger->writeLine(str.c_str());
return 0;
}
}
DWORD dwRes;
dwRes = WaitForSingleObject(pPolyKey->osReader.hEvent, READ_TIMEOUT);
switch(dwRes)
{
case WAIT_OBJECT_0:
if(!::GetOverlappedResult(pPolyKey->hSerial, &(pPolyKey->osReader), &dwRead, FALSE))
{
//Some error
pPolyKey->m_pModuleServices->pLogger->writeLine("Error in comunication");
return 0;
}
else
{
pPolyKey->receiveFromSerial(key);
}
break;
case WAIT_TIMEOUT:
//Check if the EU has been stopped
if(pPolyKey->stopped)
//Close the thread interely
pPolyKey->m_pModuleServices->pLogger->writeLine("Closing thread while the EU has been stopped");
return 0;
break;
default:
//Some error occoured
break;
}
}

return 0;
}
*/

int PCKeyboard::setProperty(int propertyIndex, const char* value)
{
	return 0;
}
int PCKeyboard::getProperty(int propertyIndex, char* value, int bufferSize)
{
	return NULL;
}

const PCKeyboardKind PCKeyboard::kinna;
