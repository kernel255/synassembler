
#include <assert.h>
#include "DirectSoundOutput.h"
#include "DirectSoundOutputKind.h" 

//#define USE_WAVELOGGER 1

const DirectSoundOutputKind DirectSoundOutput::kinna;
const int DirectSoundOutput::C_MainInIndex = 0;
int DirectSoundOutput::m_SamplingFrequency;
int DirectSoundOutput::m_BitsResolution;
const int DirectSoundOutput::m_DefaultWaveFormat = WAVE_FORMAT_PCM;
int DirectSoundOutput::m_NumChannels;
// WORKING VALUE
int DirectSoundOutput::m_BufferSize = 4000 * 2;
// Values lower, e.g. <4000 generates a clic in the output, but the saved WAV is not affected, in Release configuration the problem persists, maybe it is a DirectSound issue
// while this implementation does not use the primary buffer
int DirectSoundOutput::m_HalfBufferSize = DirectSoundOutput::m_BufferSize / 2;

int DirectSoundOutput::getNumSampleByBytes(int bufferSize)
{
	return bufferSize / ((m_BitsResolution / 8) * m_NumChannels);
}

int DirectSoundOutput::getBytesByNumSamples(int numSamples)
{
	return numSamples * (m_BitsResolution / 8) * m_NumChannels;
}

DirectSoundOutput::DirectSoundOutput(ModuleServices* pService) : AudioOutput(pService, DirectSoundOutput::kinna.getPropertyNumber(), &DirectSoundOutput::kinna),
MainInPort(ElaborationUnitPort::INPUT_PORT,ElaborationUnitPort::AUDIO_PORT,ElaborationUnitPort::SINGLE_PORT)
{
	/* Read wave parameters from library (engine) settings */
	EngineSettings* pEngSetts = pService->getEngineSettings();
	m_SamplingFrequency = (int) pEngSetts->samplingFrequence;
	m_BitsResolution = pEngSetts->bitResolution;
	m_NumChannels = pEngSetts->numChannels;

	iNumInput = 1;
	m_SoundBufferNumSamples = getNumSampleByBytes(m_HalfBufferSize);
	m_bActive = false;

	m_pSamplesBuffer = new EAG_SAMPLE_TYPE [m_SoundBufferNumSamples];
	for(int i=0;i<m_SoundBufferNumSamples;++i)
	{
		*m_pSamplesBuffer = EAG_SAMPLE_ZERO;
	}

	MainInPort.setName(DirectSoundOutputKind::MAIN_IN_PORT_NAME);
	
#ifdef USE_WAVELOGGER
	waveLogger = new WaveLogger();
	waveLogger->open("dsoundoutput",DirectSoundOutput::m_SamplingFrequency,DirectSoundOutput::m_NumChannels,DirectSoundOutput::m_BitsResolution,
		this->m_pModuleServices->pLogger);
#endif //USE_WAVELOGGER

#ifdef DEBUG_BUFFER_WITH_STREAM
	// DEBUG WAVE BUFFER WITH STREAMING
	FILE *fp = fopen("tada.wav","rb");
	if(fp!=NULL)
	{
		fseek(fp,0,SEEK_END);
		long size = ftell(fp);
		size -= 44;
		debugWaveSamples = (short int *) malloc(size);
		debugWaveNumSamples = size / sizeof(short int);
		//short int sample;
		short int *pSample;
		pSample = debugWaveSamples;
		fseek(fp,44,SEEK_SET);
		int numRead = fread(debugWaveSamples,sizeof(short int), debugWaveNumSamples, fp);
		debugWaveSamplesPtr = debugWaveSamples;
	}
	// DEBUG WAVE BUFFER WITH STREAMING
#endif //DEBUG_BUFFER_WITH_STREAM

}


DirectSoundOutput::~DirectSoundOutput()
{
	this->m_pModuleServices->pLogger->writeLine("DirectSoundOutput destructor id=%d", iId);
	HRESULT hr;
	//Release all DSound objects
	stop();
	hr = m_pDirectSoundNotify->Release();
	hr = m_pSoundBuffer->Stop();
	hr = m_pSoundBuffer->Release();
	hr = m_pDirectSound8->Release();
	//delete [] m_SamplesBuffer;
	delete [] m_pSamplesBuffer;
#ifdef USE_WAVELOGGER
	waveLogger->close();
	delete waveLogger;
#endif //USE_WAVELOGGER
}

bool DirectSoundOutput::createSoundBuffers(DirectSoundOutput* pdirectsoundoutput, LPDIRECTSOUND8 pDSound8)
{
	//Hold the DirectSound8 object
	pdirectsoundoutput->m_pDirectSound8 = pDSound8;

	HRESULT hr;
	DSBUFFERDESC dsbdesc;
    WAVEFORMATEX wfx;
    ZeroMemory(&dsbdesc, sizeof(DSBUFFERDESC));
    dsbdesc.dwSize = sizeof(DSBUFFERDESC);
	dsbdesc.dwBufferBytes = DirectSoundOutput::m_BufferSize;
	dsbdesc.dwFlags = DSBCAPS_CTRLPOSITIONNOTIFY;
    memset(&wfx, 0, sizeof(WAVEFORMATEX));
	wfx.wFormatTag = DirectSoundOutput::m_DefaultWaveFormat; 
	wfx.nChannels = DirectSoundOutput::m_NumChannels; 
	wfx.nSamplesPerSec = DirectSoundOutput::m_SamplingFrequency;
	wfx.wBitsPerSample = DirectSoundOutput::m_BitsResolution; 
    wfx.nBlockAlign = wfx.wBitsPerSample / 8 * wfx.nChannels;
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
	dsbdesc.lpwfxFormat = &wfx;
	

	hr = pDSound8->CreateSoundBuffer(&dsbdesc,&(pdirectsoundoutput->m_pSoundBuffer),NULL);
	if( hr != DS_OK )
		return false;
	HANDLE h1,h2;
	h1 = ::CreateEvent(NULL,FALSE,FALSE,NULL);
	h2 = ::CreateEvent(NULL,FALSE,FALSE,NULL);
	if( (h1==NULL) || (h2==NULL) )
		return false;
	pdirectsoundoutput->m_Notify[0].hEventNotify = h1;
	pdirectsoundoutput->m_Notify[1].hEventNotify = h2;
	pdirectsoundoutput->m_Notify[0].dwOffset = 0;
	pdirectsoundoutput->m_Notify[1].dwOffset = DirectSoundOutput::m_HalfBufferSize;
	pdirectsoundoutput->m_hNotifyHandles[0] = h1;
	pdirectsoundoutput->m_hNotifyHandles[1] = h2;

	LPDIRECTSOUNDNOTIFY pNotify;
	hr = pdirectsoundoutput->m_pSoundBuffer->QueryInterface(IID_IDirectSoundNotify,(VOID **)&pNotify);
	if( hr !=  DS_OK )
		return false;
	pdirectsoundoutput->m_pDirectSoundNotify = pNotify;
	hr = pdirectsoundoutput->m_pDirectSoundNotify->SetNotificationPositions(2,pdirectsoundoutput->m_Notify);
	assert(hr==DS_OK);
	if( hr !=  DS_OK )
		return false;
	return true;
}

void DirectSoundOutput::updateAudioSamples(EAG_SAMPLE_TYPE *pSamplesBuffer,int numsamples)
{
}

void DirectSoundOutput::receiveMIDIMessage(MIDIChannelMessage& midimsg)
{
	//Should not receive any MIDI message
	assert(false);
}

void DirectSoundOutput::setSamplesBufferMaximumSize(int size)
{
}

bool DirectSoundOutput::setInputEU(ElaborationUnitPort* pPort, ElaborationUnit* pInputEU, ElaborationUnitPort* pInputPort)
{
	if( pPort == &MainInPort )
	{
		MainInPort.setNthEUandPort(pInputEU,pInputPort,0);
		//Tell to the connected EU what is the sample buffer size
		//pInputEU->setSamplesBufferMaximumSize(m_BufferSize);
		pInputEU->setSamplesBufferMaximumSize(m_SoundBufferNumSamples);
		return true;
	}
	return false;
}

bool DirectSoundOutput::setOutputEU(ElaborationUnitPort* pPort, ElaborationUnit* pOutputEU, ElaborationUnitPort* pOutputPort)
{
	//DirectSound has output only in the DAC
	return false;
}

ElaborationUnitPort* DirectSoundOutput::getNthInputPort(int n)
{
	if( n == C_MainInIndex )
		return &MainInPort;
	else 
		return NULL;
}

ElaborationUnitPort* DirectSoundOutput::getNthOutputPort(int n)
{
	return NULL;
}

int DirectSoundOutput::getInputPortNumber(void)
{
	return 1;
}

int DirectSoundOutput::getOutputPortNumber(void)
{
	return 0;
}

void DirectSoundOutput::allocate(void)
{
}
void DirectSoundOutput::deallocate(void)
{
}

DWORD WINAPI NotifyThreadProc(LPVOID param)
{
	DirectSoundOutput *pdsoutput = reinterpret_cast<DirectSoundOutput*>(param);
	DWORD dwEvt;

	while(pdsoutput->IsActive())
	{
		dwEvt = ::MsgWaitForMultipleObjects(2,pdsoutput->m_hNotifyHandles,FALSE,INFINITE,QS_ALLINPUT);

		if( dwEvt == WAIT_OBJECT_0 )
		{
			pdsoutput->UpdateSoundBuffer(0);
		}
		else
		{
			pdsoutput->UpdateSoundBuffer(1);
		}
	}
	return 0;
}

bool DirectSoundOutput::UpdateSoundBuffer(int numsection)
{
	void *LockPtr1,*LockPtr2;
	short int *psample,sample;
	EAG_SAMPLE_TYPE eagsample;
	DWORD LockSize1,LockSize2,LockPos;
	HRESULT hr;
	int i;

	if( m_bActive )
	{
		if( numsection != 0 )
			LockPos = 0;
		else
			LockPos = DirectSoundOutput::m_HalfBufferSize;
		hr = m_pSoundBuffer->Lock(LockPos,DirectSoundOutput::m_HalfBufferSize,&LockPtr1,&LockSize1,&LockPtr2,&LockSize2,0);
		assert(LockSize1==DirectSoundOutput::m_HalfBufferSize);
		assert(LockSize2==0);
		assert(LockPtr2==NULL);
		if( hr == DS_OK )
		{
			ElaborationUnit* peu = MainInPort.getNthEU(0);
			if( peu )
			{
#if (!defined(DEBUG_BUFFER_WITH_STREAM) && !defined(DEBUG_WITH_SQUARE_WAVE))
				//Require the elaboration to the EU on input
				printf("m_SoundBufferNumSamples=%d LockSize1=%d\n",m_SoundBufferNumSamples,LockSize1);
				peu->updateAudioSamples(m_pSamplesBuffer,m_SoundBufferNumSamples);
				//Convert double samples to short int ...
				psample = (short int *) LockPtr1;
				for(i=0;i<m_SoundBufferNumSamples;++i)
				{
					eagsample = 32767 * m_pSamplesBuffer[i];
					sample = (short int) eagsample;
					*psample = sample;
					psample++;
				}
#ifdef USE_WAVELOGGER
				waveLogger->write((char *) LockPtr1,m_SoundBufferNumSamples);
#endif //USE_WAVELOGGER
#endif //(!defined(DEBUG_BUFFER_WITH_STREAM) && !defined(DEBUG_WITH_SQUARE_WAVE))
#ifdef DEBUG_WITH_SQUARE_WAVE
				static int env = 0;
				short int* pSample = (short int *) LockPtr1;
				for(unsigned int i=0;i<LockSize1/2;i++)
				{
					if(i<LockSize1/4)
						*pSample = 3000 + env;
					else
						*pSample = -3000 - env;
					pSample++;
				}
				env+=500;
#endif //DEBUG_WITH_SQUARE_WAVE
#ifdef DEBUG_BUFFER_WITH_STREAM
				short int* pSample = (short int *) LockPtr1;
				for(unsigned int i=0;i<LockSize1/2;i++)
				{
					if((debugWaveSamplesPtr-debugWaveSamples) < debugWaveNumSamples)
						*pSample = *debugWaveSamplesPtr;
					else
						*pSample = 0;
					pSample++;
					debugWaveSamplesPtr++;
				}
				waveLogger->write((char *) LockPtr1,m_SoundBufferNumSamples);
#endif //DEBUG_BUFFER_WITH_STREAM
				//Unlock the buffer
				hr = m_pSoundBuffer->Unlock(LockPtr1,LockSize1,LockPtr2,LockSize2);
				assert(hr==DS_OK);
			}
			else
			{
				assert(false);
				return false;
			}
		}
		else
		{
			assert(false);
			return false;
		}
	}
	return true;
}

void DirectSoundOutput::play(void)
{
	HRESULT hr;
	void *ptr1,*ptr2;
	DWORD size1,size2;
	short int sample,*psample;

	//The first time, lock all the buffer and zeros it
	hr = m_pSoundBuffer->Lock(0,DirectSoundOutput::m_BufferSize,&ptr1,&size1,&ptr2,&size2,DSBLOCK_FROMWRITECURSOR);
	if( hr == DS_OK )
	{
		assert(size1==DirectSoundOutput::m_BufferSize);
		assert(ptr1);
		//Fill all the buffer with zeroes
		int i;
		sample = 0;
		psample = (short int *) ptr1;
		for(i=0;i<m_SoundBufferNumSamples*2;++i)
		{
			*psample++ = sample;
		}
		hr = m_pSoundBuffer->Unlock(ptr1,size1,ptr2,size2);
		if( hr == DS_OK )
		{
			
			assert(hr==DS_OK);
			if( hr == DS_OK )
			{
				m_bActive = true;
				//Create thread
				DWORD ThId;
				m_hThreadHandle = ::CreateThread(NULL,0,&NotifyThreadProc,this,0,&ThId);
				assert(m_hThreadHandle);
				hr = m_pSoundBuffer->Play(0,0,DSBPLAY_LOOPING);
			}

		}

	}
	else
		return;
}

void DirectSoundOutput::stop(void)
{
	this->m_pModuleServices->pLogger->writeLine("Stopping DirectSoundOutput");
	HRESULT hr;
	m_bActive = false;
	hr = m_pSoundBuffer->Stop();
	::TerminateThread(m_hThreadHandle,0);
}

void DirectSoundOutput::pause(void)
{
	HRESULT hr;
	hr = m_pSoundBuffer->Stop();
}

const EUKind* DirectSoundOutput::getKind(void)
{
	return s_GetKind();
}

EUKind* DirectSoundOutput::s_GetKind(void)
{
	return (EUKind *) &DirectSoundOutput::kinna;
}

int DirectSoundOutput::GetAllocatedDevices(void)
{
	return 1;
}

const char* DirectSoundOutput::GetDeviceName(void)
{
	return "Microsoft DirectX Wave Output";
}

bool DirectSoundOutput::AllocateDevice(void)
{
	return false;
}

ElaborationUnitPort* DirectSoundOutput::getOutputPortByEU(ElaborationUnit* pEU, int& n)
{
	return NULL;
}

ElaborationUnitPort* DirectSoundOutput::getInputPortByEU(ElaborationUnit* pEU, int& n)
{
	if( MainInPort.getNthEU(0) == pEU )
	{
		n = 0;
		return &MainInPort;
	}
	else
		return NULL;
}

int DirectSoundOutput::setProperty(int propertyIndex, const char* value)
{
	return 0;
}
int DirectSoundOutput::getProperty(int propertyIndex, char* value, int bufferSize)
{
	return NULL;
}
