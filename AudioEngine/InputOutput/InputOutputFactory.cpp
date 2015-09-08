
#include "InputOutputFactory.h"
#include ".\ElaborationUnits\DirectSoundOutput.h"
#include ".\ElaborationUnits\MIDIInput.h"
#include ".\ElaborationUnits\PCKeyboard.h"

int InputOutputFactory::m_NumDevices;
bool InputOutputFactory::m_bDirectSoundAvailable;
HMODULE InputOutputFactory::m_hDSoundModule;
std::vector<InputOutputFactory::DirectSoundOutputDescription> InputOutputFactory::m_DirectSoundOutputDescriptions;
std::vector<InputOutputFactory::MIDIInDescription> InputOutputFactory::m_MIDIInDescriptions;
td_pfn_DirectSoundEnumerate InputOutputFactory::m_td_pfn_DirectSoundEnumerate;
td_pfn_DirectSoundCreate InputOutputFactory::m_td_pfn_DirectSoundCreate;

const int InputOutputFactory::LocalEUNumber = 4; //(DirectSoundOutput)+(MIDIInput)+(PolyKeyboard)+(PCKeyboard)

const int NUM_PC_KEYBOARDS = 1;

InputOutputFactory::InputOutputFactory(ModuleServices* pServices) : AbstractPhysicalElaborationUnitFactory(pServices)
{
	moduleServices->pLogger->writeLine("InputOutputFactory constructor");
	enumDirectSoundDevices();
	enumMidiInDevices();
	enumPolyKeyboard();
	enumPCKeyboard();
	pcKeyAllocated = false;
}

InputOutputFactory::~InputOutputFactory(void)
{
	moduleServices->pLogger->writeLine("InputOutputFactory: destructor");
	cleanUpElaborationUnits();
}

BOOL CALLBACK DSEnumCallBack(LPGUID  lpGuid, LPCWSTR  lpcstrDescription, LPCWSTR  lpcstrModule, LPVOID  lpContext)
{
	static int index = 0;
	//Does not consider the default device
	if( lpGuid == NULL )
		return TRUE;
	InputOutputFactory* pIoFact = (InputOutputFactory *) lpContext;
	AbstractPhysicalElaborationUnitFactory::PhysicalEUDescription* pDesc = new 
		AbstractPhysicalElaborationUnitFactory::PhysicalEUDescription(lpcstrDescription,(char*) lpGuid,sizeof(GUID),
		DIRECTSOUND_INDEX,index++);

	pIoFact->addInstance(pDesc);

	return TRUE;
}

bool InputOutputFactory::AddDevice(LPCWSTR description, GUID* pguid)
{
	DirectSoundOutputDescription dsoundoutdescription;
	//TODO: find this error: cannot compile!
	dsoundoutdescription.m_wstrBoardDescription = description;
	if( pguid == NULL )
		memset(&dsoundoutdescription.m_guid,0,sizeof(GUID));
	else
		dsoundoutdescription.m_guid = *pguid;

	m_DirectSoundOutputDescriptions.push_back(dsoundoutdescription);

	return true;
}

void InputOutputFactory::enumDirectSoundDevices(void)
{
	m_hDSoundModule = ::LoadLibrary(L"dsound.dll");
	if( m_hDSoundModule == NULL )
	{
		m_bDirectSoundAvailable = false;
		return;
	}
	m_td_pfn_DirectSoundEnumerate = (td_pfn_DirectSoundEnumerate) ::GetProcAddress(m_hDSoundModule,"DirectSoundEnumerateA");
	if( m_td_pfn_DirectSoundEnumerate == NULL )
	{
		DWORD err = ::GetLastError();
		m_bDirectSoundAvailable = false;
		return;
	}
	m_td_pfn_DirectSoundCreate = (td_pfn_DirectSoundCreate) ::GetProcAddress(m_hDSoundModule,"DirectSoundCreate8");
	if( m_td_pfn_DirectSoundCreate == NULL )
	{
		DWORD err = ::GetLastError();
		m_bDirectSoundAvailable = false;
		return;
	}

	m_bDirectSoundAvailable = true;

	HRESULT hr;
	m_NumDevices = 0;

	hr = ::DirectSoundEnumerate(&DSEnumCallBack,this);

}

void InputOutputFactory::enumMidiInDevices(void)
{
	int num = ::midiInGetNumDevs();
	int index = 0;
	for(int i=0;i<num;++i)
	{
		MIDIINCAPS midiInCaps;
		memset(&midiInCaps,0,sizeof(MIDIINCAPS));
		::midiInGetDevCaps(i,&midiInCaps,sizeof(midiInCaps));
		AbstractPhysicalElaborationUnitFactory::PhysicalEUDescription* pDesc = new 
			AbstractPhysicalElaborationUnitFactory::PhysicalEUDescription(midiInCaps.szPname,NULL,0,MIDIIN_INDEX,index++);

		addInstance(pDesc);

	}

}

#define NUM_SERIALPORT_TEST		3

void InputOutputFactory::enumPolyKeyboard(void)
{
	//Look for all the serial port where the PolyKeyboard can be connected to
	wchar_t buffer[10];
	int index = 0;
	//memset(buffer,0,sizeof(buffer));
	for(int i=0;i<NUM_SERIALPORT_TEST;i++)
	{
		wsprintf(buffer,L"COM%d\0",i);
		HANDLE hSerial;
		hSerial = ::CreateFile(buffer,GENERIC_READ | GENERIC_WRITE,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
		if(hSerial!=INVALID_HANDLE_VALUE)
		{
			AbstractPhysicalElaborationUnitFactory::PhysicalEUDescription* pDesc = new 
				AbstractPhysicalElaborationUnitFactory::PhysicalEUDescription(buffer,NULL,0,POLYKEYBOARD_INDEX,index++);

			addInstance(pDesc);
			::CloseHandle(hSerial);
		}
	}
}

void InputOutputFactory::enumPCKeyboard(void)
{
	wchar_t buffer[20];

	for (int i = 0; i < NUM_PC_KEYBOARDS; i++)
	{
		wsprintf(buffer, L"PCKEYB%d\0", i);
		AbstractPhysicalElaborationUnitFactory::PhysicalEUDescription* pDescr = new AbstractPhysicalElaborationUnitFactory::PhysicalEUDescription(buffer, NULL, 0, PCKEYBOARD_INDEX, i);
		addInstance(pDescr);
	}
}


//######### VIRTUAL ELABORATION UNITS #########

unsigned int InputOutputFactory::getVirtualEUKindNumber(void)
{
	return 0;
}

const EUKind* InputOutputFactory::getNthVirtualEUKind(int n)
{
	return NULL;
}

ElaborationUnit* InputOutputFactory::createVirtualElaborationUnit(unsigned euIndex)
{
	return NULL;
}

//######### PHYSICAL ELABORATION UNITS #########

unsigned int InputOutputFactory::getPhysicalEUKindNumber(void)
{
	return LocalEUNumber;
}

const EUKind* InputOutputFactory::getNthPhysicalEUKind(int n)
{
	switch( n )
	{
	case DIRECTSOUND_INDEX:
		{
			return DirectSoundOutput::s_GetKind();
			break;
		}
	case MIDIIN_INDEX:
		{
			return MIDIInput::s_GetKind();
			break;
		}
	case POLYKEYBOARD_INDEX:
		{
			return PolyKeyboard::s_GetKind();
			break;
		}
	case PCKEYBOARD_INDEX:
	{
		return PCKeyboard::s_GetKind();
		break;
	}
	default:
		{
			return NULL;
			break;
		}
	}
}

ElaborationUnit* InputOutputFactory::getDirectSoundOutput(int instanceIndex)
{
	moduleServices->pLogger->writeLine("Creating DirectSound output");
	LPDIRECTSOUND8 pDSound8;
	HRESULT hr;
	AbstractPhysicalElaborationUnitFactory::PhysicalEUDescription *pDesc;
	pDesc = this->getNthEUDescription(DIRECTSOUND_INDEX,instanceIndex);
	if(pDesc==NULL)
	{
		moduleServices->pLogger->writeLine("Unable to get EU description");
		return NULL;
	}
	GUID* pguid = (GUID *) pDesc->specificInfoStr;
	//memcpy(&guid,pDesc->specificInfoStr,pDesc->specInfoSize);
	hr = m_td_pfn_DirectSoundCreate(pguid,&pDSound8,NULL);
	if(hr==DS_OK)
	{
		HWND handle = this->moduleServices->specOSInfo.hwnd;
		moduleServices->pLogger->writeLine("HWND=0x%08x", handle);
#ifdef PRIMARY_BUFFER
		//hr = pDSound8->SetCooperativeLevel((HWND) handle,DSSCL_WRITEPRIMARY);
		hr = pDSound8->SetCooperativeLevel((HWND) handle,DSSCL_PRIORITY);
#else
		hr = pDSound8->SetCooperativeLevel((HWND) handle,DSSCL_NORMAL);
#endif //PRIMARY_BUFFER
		if(hr==DS_OK)
		{
			DirectSoundOutput* pdso = new DirectSoundOutput(moduleServices);
			//Try to create the DirectSoundBuffer
			if( DirectSoundOutput::createSoundBuffers( pdso,pDSound8) )
			{
				addElaborationUnit(pdso);
				return pdso;
			}
			else
			{
				moduleServices->pLogger->writeLine("Unable to create Direct Sound BUffer");
				delete pdso;
				//Something went wrong during sound buffers creation...
				return NULL;
			}
		}
		else
		{
			moduleServices->pLogger->writeLine("Unable to set cooperative level");
			return NULL;
		}
	}
	else
	{
		moduleServices->pLogger->writeLine("Unable to create Direct Sound");
		return NULL;
	}
}

ElaborationUnit* InputOutputFactory::getMidiIn(int instanceIndex)
{
	int num;
	//Check if a MIDIIn device is available
	if( (num = MIDIInput::GetMIDIDevicesNumber()) > 0 )
	{
		//First of all, create the object
		MIDIInput *pMidiIn = new MIDIInput(moduleServices);
		//Check for a MIDI device available
		for(int i=0;i<num;++i)
		{
			//if( !m_MIDIInDescriptions[i].m_bAllocated )
			//{
				HMIDIIN hMIDIIn;
				DWORD ThId;

				HANDLE h = ::CreateThread(NULL,0,&MIDIThreadProc,pMidiIn,0,&ThId);
				assert(h);

				//Try to allocate it
				//MMRESULT mmres = ::midiInOpen(&hMIDIIn,i,(DWORD_PTR) &MIDIInput::MidiInProc,(DWORD_PTR) pMIDIInput,CALLBACK_FUNCTION);
				MMRESULT mmres = ::midiInOpen(&hMIDIIn,i,(DWORD_PTR) ThId,NULL,CALLBACK_THREAD);
				if( mmres == MMSYSERR_NOERROR )
				{
					//Object created successfully
					pMidiIn->SetMIDIInHandle(hMIDIIn);
					//m_MIDIInDescriptions[i].m_bAllocated = true;
					addElaborationUnit(pMidiIn);
					return pMidiIn;
				}
				else
				{
					//Stop the Thread
					if(h)
						::CloseHandle(h);
					delete pMidiIn;

					return NULL;
				}
			//}
		}
		delete pMidiIn;
		return NULL;				
	}
	else
		return NULL;
}

ElaborationUnit* InputOutputFactory::getPolyKeyboard(int instanceIndex)
{
	HANDLE hSerial;
	AbstractPhysicalElaborationUnitFactory::PhysicalEUDescription *pDesc;
	pDesc = getNthEUDescription(POLYKEYBOARD_INDEX,instanceIndex);
	if(pDesc==NULL)
		return NULL;
	//hSerial = ::CreateFile(pDesc->name.c_str(),GENERIC_READ | GENERIC_WRITE,0,0,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,0);
	hSerial = ::CreateFile(pDesc->name.c_str(),GENERIC_READ | GENERIC_WRITE,0,0,OPEN_EXISTING,0,0);
	if(hSerial!=INVALID_HANDLE_VALUE)
	{
		DCB dcbSerialParams = {0};
		dcbSerialParams.DCBlength=sizeof(dcbSerialParams);
		if (!GetCommState(hSerial, &dcbSerialParams)) 
		{
			//moduleServices->pLogger->writeLine("Error while getting state");
			return NULL;
		}
		dcbSerialParams.BaudRate=CBR_9600;
		dcbSerialParams.ByteSize=8;
		dcbSerialParams.StopBits=ONESTOPBIT;
		dcbSerialParams.Parity=NOPARITY;
		dcbSerialParams.Parity = FALSE;
		if(!SetCommState(hSerial, &dcbSerialParams))
		{
			//moduleServices->pLogger->writeLine("Error while getting the serial port state");
			return NULL;
		}
		COMMTIMEOUTS timeouts={0};
		timeouts.ReadIntervalTimeout=50;
		timeouts.ReadTotalTimeoutConstant=50;
		timeouts.ReadTotalTimeoutMultiplier=10;
		timeouts.WriteTotalTimeoutConstant=50;
		timeouts.WriteTotalTimeoutMultiplier=10;
		if(!SetCommTimeouts(hSerial, &timeouts))
		{
			//moduleServices->pLogger->writeLine("Error while setting timeout");
			return NULL;
		}

		/*
		OVERLAPPED osReader = { 0 };
		osReader.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
		if(osReader.hEvent==NULL)
		{
			moduleServices->pLogger->writeLine("Unable to create Event");
			return NULL;
		}
		*/

#ifdef SEND_INITIALIZATION_MESSAGE
		//Send an intialization char
		char initChar = INITIALIZATION_MESSAGE;
		DWORD written;
		if(::WriteFile(hSerial,&initChar,1,&written,&osReader)==FALSE)
		{
			DWORD error = ::GetLastError();
			if(error!=ERROR_IO_PENDING)
			{
				//moduleServices->pLogger->writeLine("Unable to send intialization char");
				return NULL;
			}
		}
#endif //SEND_INITIALIZATION_MESSAGE
		ElaborationUnit* pEU = new PolyKeyboard(moduleServices, hSerial);
		//Create the thread
		DWORD ThId;
		HANDLE h = ::CreateThread(NULL,0,&PolyKeyboardThreadProc,pEU,0,&ThId);
		assert(h);


		return pEU;
	}
	else
		return NULL;
}

ElaborationUnit* InputOutputFactory::getPCKeyboard(int instanceIndex)
{
	if (!pcKeyAllocated)
	{
		pcKeyAllocated = true;
		ElaborationUnit* pcKeyb = new PCKeyboard(moduleServices);
		return pcKeyb;
	}
	else
	{
		return NULL;
	}
}

ElaborationUnit* InputOutputFactory::createPhysicalElaborationUnit(unsigned euIndex, unsigned instanceIndex)
{
	switch( euIndex )
	{
	case DIRECTSOUND_INDEX:
		return getDirectSoundOutput(instanceIndex);
	case MIDIIN_INDEX:
		return getMidiIn(instanceIndex);
	case POLYKEYBOARD_INDEX:
		return getPolyKeyboard(instanceIndex);
	case PCKEYBOARD_INDEX:
		return getPCKeyboard(instanceIndex);
	default:
		return NULL;
	}
}

const int InputOutputFactory::getEUIndex(ElaborationUnit* pEU)
{
	const EUKind* pKind = pEU->getKind();
	if( !strcmp("DirectSound output",pKind->getName()) )
		return DIRECTSOUND_INDEX;
	else
	{
		if( !strcmp("MIDI Input",pKind->getName()) )
			return MIDIIN_INDEX;
		else
			return C_UnknownIndex;
	}
}

bool InputOutputFactory::GetNextAvailableDirectSoundOutput(GUID& guid, LPDIRECTSOUND8& pDSound8)
{
	bool bDeviceAvailable = false;
	HRESULT hr;

	std::vector<DirectSoundOutputDescription>::iterator it;
	for(it=m_DirectSoundOutputDescriptions.begin();it!=m_DirectSoundOutputDescriptions.end();++it)
	{
		hr = m_td_pfn_DirectSoundCreate(&((*it).m_guid),&pDSound8,NULL);
		if( hr == DS_OK )
		{
			bDeviceAvailable =true;
			guid = (*it).m_guid;
		}
	}
	return bDeviceAvailable;
}

void InputOutputFactory::destroyElaborationUnit(ElaborationUnit* peu)
{
	const char* euName = peu->getKind()->getName();
	if (!strcmp(PCKEYB_NAME, euName))
	{
		pcKeyAllocated = false;
	}
	removeElaborationUnit(peu);
	//ElaborationUnitFactory::DestroyElaborationUnit(pGEU);
}

const char* InputOutputFactory::getName(void)
{
	return "Input Output";
}