// AudioWASAPI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <Windows.h>
#include <winbase.h>
#include <winnt.h>
#include <combaseapi.h>
#include <initguid.h>
#include <mmdeviceapi.h>
#include <Audioclient.h>

#include "MyAudioSource.h"

// REFERENCE_TIME time units per second and per millisecond
#define REFTIMES_PER_SEC  10000000
#define REFTIMES_PER_MILLISEC  10000

const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
const IID IID_IAudioClient = __uuidof(IAudioClient);
const IID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);

#define EXIT_ON_ERROR(hres)  \
              if (FAILED(hres)) { goto Exit; }
#define SAFE_RELEASE(punk)  \
              if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }

int main()
{
	IMMDeviceEnumerator *pEnumerator = NULL;
	IMMDevice *pDevice = NULL;
	IAudioClient *pAudioClient = NULL;
	IAudioRenderClient *pRenderClient = NULL;
	REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;
	WAVEFORMATEX *pwfx = NULL;
	UINT32 bufferFrameCount;
	REFERENCE_TIME hnsActualDuration;
	BYTE *pData;
	UINT32 numFramesAvailable;
	UINT32 numFramesPadding;
	DWORD flags = 0;

	MyAudioSource* pMySource = new MyAudioSource();

	CoInitialize(NULL);

	HRESULT hr = CoCreateInstance(
		CLSID_MMDeviceEnumerator, NULL,
		CLSCTX_ALL, IID_IMMDeviceEnumerator,
		(void**)&pEnumerator);

	EXIT_ON_ERROR(hr)

	hr = pEnumerator->GetDefaultAudioEndpoint(
		eRender, eConsole, &pDevice);

	EXIT_ON_ERROR(hr)

	hr = pDevice->Activate(
		IID_IAudioClient, CLSCTX_ALL,
		NULL, (void**)&pAudioClient);

	EXIT_ON_ERROR(hr)
	hr = pAudioClient->GetMixFormat(&pwfx);
	EXIT_ON_ERROR(hr)

		hr = pAudioClient->Initialize(
			AUDCLNT_SHAREMODE_SHARED,
			0,
			hnsRequestedDuration,
			0,
			pwfx,
			NULL);
		EXIT_ON_ERROR(hr)


		// Tell the audio source which format to use.
		hr = pMySource->SetFormat(pwfx);
		EXIT_ON_ERROR(hr)

			// Get the actual size of the allocated buffer.
			hr = pAudioClient->GetBufferSize(&bufferFrameCount);
		printf("Buffer Size:%d\n", bufferFrameCount);
		EXIT_ON_ERROR(hr)

			hr = pAudioClient->GetService(
				IID_IAudioRenderClient,
				(void**)&pRenderClient);
		EXIT_ON_ERROR(hr)

			// Grab the entire buffer for the initial fill operation.
			hr = pRenderClient->GetBuffer(bufferFrameCount, &pData);
		EXIT_ON_ERROR(hr)







			// Load the initial data into the shared buffer.
			hr = pMySource->LoadData(bufferFrameCount, pData, &flags);
		EXIT_ON_ERROR(hr)

			hr = pRenderClient->ReleaseBuffer(bufferFrameCount, flags);
		EXIT_ON_ERROR(hr)

			// Calculate the actual duration of the allocated buffer.
			hnsActualDuration = (double)REFTIMES_PER_SEC *
			bufferFrameCount / pwfx->nSamplesPerSec;

		hr = pAudioClient->Start();  // Start playing.
		EXIT_ON_ERROR(hr)

			// Each loop fills about half of the shared buffer.
			while (flags != AUDCLNT_BUFFERFLAGS_SILENT)
			{
				// Sleep for half the buffer duration.
				Sleep((DWORD)(hnsActualDuration / REFTIMES_PER_MILLISEC / 2));

				// See how much buffer space is available.
				hr = pAudioClient->GetCurrentPadding(&numFramesPadding);
				EXIT_ON_ERROR(hr)

					numFramesAvailable = bufferFrameCount - numFramesPadding;

				// Grab all the available space in the shared buffer.
				hr = pRenderClient->GetBuffer(numFramesAvailable, &pData);
				EXIT_ON_ERROR(hr)

					// Get next 1/2-second of data from the audio source.
					hr = pMySource->LoadData(numFramesAvailable, pData, &flags);
				EXIT_ON_ERROR(hr)

					hr = pRenderClient->ReleaseBuffer(numFramesAvailable, flags);
				EXIT_ON_ERROR(hr)
			}







		return 0;

	Exit:
	CoTaskMemFree(pwfx);
	SAFE_RELEASE(pEnumerator)
		SAFE_RELEASE(pDevice)
		SAFE_RELEASE(pAudioClient)
		SAFE_RELEASE(pRenderClient)


    return hr;
}

