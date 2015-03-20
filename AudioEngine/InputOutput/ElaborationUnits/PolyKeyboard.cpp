
#include "PolyKeyboard.h"

//NOTE THE THREAD PROC STILL NEED TO BE STARTED!

PolyKeyboard::PolyKeyboard(ModuleServices* pService, HANDLE _hSerial, OVERLAPPED _osReader) : PhysicalElaborationUnit(pService, PolyKeyboard::kinna.getPropertyNumber(), &PolyKeyboard::kinna),
messagesOutPort(ElaborationUnitPort::OUTPUT_PORT,ElaborationUnitPort::MIDI_PORT,ElaborationUnitPort::MULTI_PORT)
{
	messagesOutPort.setName(PolyKeyboardKind::SERIAL_OUT_PORT_NAME);
	hSerial = _hSerial;
	osReader = _osReader;
}

PolyKeyboard::PolyKeyboard(ModuleServices* pService, HANDLE _hSerial) : PhysicalElaborationUnit(pService, PolyKeyboard::kinna.getPropertyNumber(), &PolyKeyboard::kinna),
messagesOutPort(ElaborationUnitPort::OUTPUT_PORT,ElaborationUnitPort::MIDI_PORT,ElaborationUnitPort::MULTI_PORT)
{
	messagesOutPort.setName(PolyKeyboardKind::SERIAL_OUT_PORT_NAME);
	hSerial = _hSerial;
}

PolyKeyboard::~PolyKeyboard()
{
	if(!::CloseHandle(hSerial))
	{
		m_pModuleServices->pLogger->writeLine("Error while closing serial handle");
	}
	m_pModuleServices->pLogger->writeLine("PolyKeyboard destructor");
}

void PolyKeyboard::updateAudioSamples(EAG_SAMPLE_TYPE *pSamplesBuffer,int numsamples)
{
}

void PolyKeyboard::receiveMIDIMessage(MIDIChannelMessage& midimsg)
{
	//Propagate MIDI messages to all the connected outputs
	int numports = messagesOutPort.getConnectionsNumber();
	for(int i=0;i<numports;++i)
	{
		ElaborationUnit* pEU = messagesOutPort.getNthEU(i);
		if( pEU )
		{
			pEU->receiveMIDIMessage(midimsg);
		}
	}

}

bool PolyKeyboard::setInputEU(ElaborationUnitPort* pPort, ElaborationUnit* pInputEU, ElaborationUnitPort* pInputPort)
{
	return false;
}

bool PolyKeyboard::setOutputEU(ElaborationUnitPort* pPort, ElaborationUnit* pOutputEU, ElaborationUnitPort* pOutputPort)
{
	if( pPort == &messagesOutPort )
	{
		messagesOutPort.setNthEUandPort(pOutputEU,pOutputPort,C_AddNewConnection);
		return true;
	}
	else
		return false;

}

ElaborationUnitPort* PolyKeyboard::getNthInputPort(int n)
{
	return NULL;
}

ElaborationUnitPort* PolyKeyboard::getNthOutputPort(int n)
{
	if( n == 0 )
		return &messagesOutPort;
	else
		return NULL;

}

int PolyKeyboard::getInputPortNumber(void)
{
	return 0;
}

int PolyKeyboard::getOutputPortNumber(void)
{
	return 1;
}

ElaborationUnitPort* PolyKeyboard::getInputPortByEU(ElaborationUnit* pEU, int& n)
{
	return NULL;
}

ElaborationUnitPort* PolyKeyboard::getOutputPortByEU(ElaborationUnit* pEU, int& n)
{
	int numports = messagesOutPort.getConnectionsNumber();
	//Try with all the connected EUs
	for(int i=0;i<numports;++i)
	{
		if( messagesOutPort.getNthEU(i) == pEU )
			return &messagesOutPort;
	}
	return NULL;
}

void PolyKeyboard::allocate(void)
{
}

void PolyKeyboard::deallocate(void)
{
}

void PolyKeyboard::play(void)
{
}

void PolyKeyboard::pause(void)
{
}

void PolyKeyboard::stop(void)
{
	
}

const EUKind* PolyKeyboard::getKind(void)
{
	return s_GetKind();
}

EUKind* PolyKeyboard::s_GetKind(void)
{
	return (EUKind *) &PolyKeyboard::kinna;
}

int PolyKeyboard::GetAllocatedDevices(void)
{
	return 0;
}

const char* PolyKeyboard::GetDeviceName(void)
{
	return NULL;
}

bool PolyKeyboard::AllocateDevice(void)
{
	return false;
}

void PolyKeyboard::setSamplesBufferMaximumSize(int size)
{
}

#define NOTE_ON_FLAG	0x40
//TODO: Recalculate this value, accordingly to the real PolyTrack keyboard
#define MIDI_C2			30

void PolyKeyboard::receiveFromSerial(char key)
{
	MIDIChannelMessage midiMsg;
	//TODO: fix this stuff accordingly to the PIC values received
	int column = key & 0xF8;
	int row = key & 0x07;
	int note = column * 8 + row;
	note += MIDI_C2;
	//TODO: Provide a way to change MIDI channel
	midiMsg.channel = 0;
	if(key & NOTE_ON_FLAG)
		midiMsg.Kind = MIDIChannelMessage::e_NoteOn;
	else
		midiMsg.Kind = MIDIChannelMessage::e_NoteOff;
	receiveMIDIMessage(midiMsg);
}

#define READ_TIMEOUT 200


DWORD WINAPI PolyKeyboardThreadProc(LPVOID param)
{
	PolyKeyboard* pPolyKey = (PolyKeyboard *) param;
	char key;
	DWORD dwLen, dwRead;

	while(1)
	{
		dwRead = ::ReadFile(pPolyKey->hSerial, &key, 1, &dwLen, NULL);
		if(!dwRead)
		{
			DWORD error = ::GetLastError();
			if(error != ERROR_IO_PENDING)
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
			if(dwLen!=0)
			{
				pPolyKey->receiveFromSerial(key);
				printf("Receiving: 0x%02x received size: %d", key, dwLen);
			}
		}
	}
}


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

int PolyKeyboard::setProperty(int propertyIndex, const char* value)
{
	return 0;
}
int PolyKeyboard::getProperty(int propertyIndex, char* value, int bufferSize)
{
	return NULL;
}

const PolyKeyboardKind PolyKeyboard::kinna;

