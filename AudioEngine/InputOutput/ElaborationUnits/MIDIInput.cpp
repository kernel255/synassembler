
#include "MIDIInput.h"

const MIDIInputKind MIDIInput::kinna;
unsigned int MIDIInput::m_NumMIDIDevices;


MIDIInput::MIDIInput(ModuleServices* pService) : PhysicalElaborationUnit(pService, MIDIInput::kinna.getPropertyNumber(), &MIDIInput::kinna),  
messagesOutPort(ElaborationUnitPort::OUTPUT_PORT,ElaborationUnitPort::MIDI_PORT,ElaborationUnitPort::MULTI_PORT)
{
	messagesOutPort.setName(MIDIInputKind::OUT_PORT_NAME);
}

MIDIInput::~MIDIInput()
{
	::midiInClose(m_hMIDIIn);
}

bool MIDIInput::setInputEU(ElaborationUnitPort* pPort, ElaborationUnit* pInputEU, ElaborationUnitPort* pInputPort)
{
	return false;
}

bool MIDIInput::setOutputEU(ElaborationUnitPort* pPort, ElaborationUnit* pOutputEU, ElaborationUnitPort* pOutputPort)
{
	if( pPort == &messagesOutPort )
	{
		messagesOutPort.setNthEUandPort(pOutputEU,pOutputPort,C_AddNewConnection);
		return true;
	}
	else
		return false;
}

ElaborationUnitPort* MIDIInput::getNthInputPort(int n)
{
	return NULL;
}

ElaborationUnitPort* MIDIInput::getNthOutputPort(int n)
{
	if( n == 0 )
		return &messagesOutPort;
	else
		return NULL;
}

int MIDIInput::getInputPortNumber(void)
{
	return 0;
}

int MIDIInput::getOutputPortNumber(void)
{
	return 1;
}

void MIDIInput::setSamplesBufferMaximumSize(int size)
{
	//MIDI should not care about buffer samples size
	assert(false);
}

ElaborationUnitPort* MIDIInput::getInputPortByEU(ElaborationUnit* pEU, int& n)
{
	return NULL;
}

ElaborationUnitPort* MIDIInput::getOutputPortByEU(ElaborationUnit* pEU, int& n)
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

void MIDIInput::allocate(void) {}
void MIDIInput::deallocate(void) {}

const EUKind* MIDIInput::getKind(void)
{ 
	return s_GetKind();
}

EUKind* MIDIInput::s_GetKind(void)
{
	return (EUKind *) &MIDIInput::kinna;
}

void MIDIInput::updateAudioSamples(EAG_SAMPLE_TYPE *pSamplesBuffer,int numsamples)
{
	//MIDI cannot update samples...
	assert(false);
}

void MIDIInput::receiveMIDIMessage(MIDIChannelMessage& midimsg)
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

void MIDIInput::SendMIDIMessageToConnectedEUs(unsigned char* buffer, int size)
{
//	ElaborationUnit* pEU = MessagesOutPort.GetOutputEU();
//	if( pEU )
//	{
//       pEU->receiveMIDIMessage(buffer,size);
//	}
}

unsigned int MIDIInput::GetMIDIDevicesNumber(void)
{
	m_NumMIDIDevices = ::midiInGetNumDevs();
	return m_NumMIDIDevices;
}

void MIDIInput::play(void)
{
	MMRESULT mmres;
	mmres = ::midiInStart(m_hMIDIIn);
	assert(mmres==MMSYSERR_NOERROR);
}

void MIDIInput::pause(void)
{
	MMRESULT mmres;
	mmres = ::midiInStop(m_hMIDIIn);
	assert(mmres==MMSYSERR_NOERROR);
}

/*
void CALLBACK MIDIInput::MidiInProc(HMIDIIN hMidiIn,UINT wMsg,DWORD dwInstance,DWORD dwParam1,DWORD dwParam2)
{
	int size = sizeof(DWORD)*2+sizeof(UINT);
	unsigned char buffer[sizeof(DWORD)*2+sizeof(UINT)];
	//DWORD* pdwPars;
	//Retrieve the MIDIIn obj
	//MIDIInput *pMIDIIn = reinterpret_cast<MIDIInput *>(dwInstance);
	ElaborationUnit* pEU = reinterpret_cast<ElaborationUnit *>(dwInstance);
	MIDIInput *pMIDIIn = reinterpret_cast<MIDIInput *>(pEU);
	//Compact message
	//*buffer = wMsg;
	//pdwPars = (DWORD *) (buffer + sizeof(UINT));
	//*pdwPars = dwParam1;
	//pdwPars = (DWORD *) (buffer + sizeof(DWORD)+sizeof(UINT));
	//*pdwPars = dwParam2;
	pMIDIIn->SendMIDIMessageToConnectedEUs(buffer,size);
}
*/

void MIDIInput::CreateMIDIMessage(unsigned char* pbData, MIDIChannelMessage& midimsg, ModuleServices* pService)
{
	unsigned char kind, channel;

	kind = *pbData & 0xF0;
	channel = *pbData & 0x0F;

	midimsg.channel = channel;
	midimsg.Kind = (MIDIChannelMessage::MIDIMessage) kind;
	//Manage the different MIDI message
	switch(kind)
	{
	case (int) MIDIChannelMessage::e_NoteOn:
		{
			midimsg.data.NoteMessage.Note = *(pbData+1);
			midimsg.data.NoteMessage.Velocity = *(pbData+2);
			midimsg.data.NoteMessage.Frequency = MIDIChannelMessage::GetFreqByMIDINote(*(pbData+1));
			//pService->pLogger->writeLine("CreateMIDIMessage:Note On=%d Vel=%d", midimsg.data.NoteMessage.Note, midimsg.data.NoteMessage.Velocity);
			break;
		}
	case (int) MIDIChannelMessage::e_NoteOff:
	{
		midimsg.data.NoteMessage.Note = *(pbData + 1);
		midimsg.data.NoteMessage.Velocity = *(pbData + 2);
		//pService->pLogger->writeLine("CreateMIDIMessage:Note Off=%d Vel=%d", midimsg.data.NoteMessage.Note, midimsg.data.NoteMessage.Velocity);
		break;
	}
	default:
		{
			break;
		}
	}
}

int MIDIInput::GetAllocatedDevices(void)
{
	return -1;
}

const char* MIDIInput::GetDeviceName(void)
{
	return "";
}

/**
 * \fn Handle the MIDIInput thread.
 * \param param Is a void* to a MIDIInputProxy which holds the true pointer to MIDIInput obj
 */

DWORD WINAPI MIDIThreadProc(LPVOID param)
{
	MIDIInput* pMIDIIn = (MIDIInput *) param;

	MSG msg;
	while(::GetMessage(&msg,NULL,0,0))
	{
		//assert( pMIDIIn->m_hMIDIIn == (HMIDIIN) msg.wParam );
		switch( msg.message )
		{
		case MM_MIM_OPEN:
			{
				break;
			}
		case MM_MIM_CLOSE:
			{
				int jj=0;
				break;
			}
		case MM_MIM_DATA:
			{
				assert(pMIDIIn->m_hMIDIIn == (HMIDIIN) msg.wParam);
				MIDIChannelMessage midimsg;
				MIDIInput::CreateMIDIMessage(((unsigned char *) &msg.lParam),midimsg, pMIDIIn->m_pModuleServices);
				pMIDIIn->receiveMIDIMessage(midimsg);
				break;
			}
		case MM_MIM_LONGDATA:
			{
				int jj=0;
				break;
			}
		case MM_MIM_ERROR:
			{
				int jj=0;
				break;
			}
		case MM_MIM_LONGERROR:
			{
				int jj=0;
				break;
			}
		case MM_MIM_MOREDATA:
			{
				int jj=0;
				break;
			}

		default:
			{
//				HANDLE h = pMIDIInput->m_hMIDIIn;
				int jj=0;
				break;
			}
		}
	}
	
	return 0;
}

int MIDIInput::setProperty(int propertyIndex, const char* value)
{
	return 0;
}

int MIDIInput::getProperty(int propertyIndex, char* value, int bufferSize)
{
	return NULL;
}
