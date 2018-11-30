#include "stdafx.h"
#include "MyAudioSource.h"
#include <mmreg.h>
#include <stdint.h>

MyAudioSource::MyAudioSource()
{
	first = true;
}


MyAudioSource::~MyAudioSource()
{
}




HRESULT MyAudioSource::SetFormat(WAVEFORMATEX *wpfx) {
	if (wpfx->wFormatTag == 0xFFFE) {
		memset(&m_Wfe, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memcpy(&m_Wfe, wpfx, sizeof(WAVEFORMATEXTENSIBLE));

		printf("BitsPerSample=%d\n", m_Wfe.Format.wBitsPerSample);
		printf("ValidBitsPerSample=%d\n", m_Wfe.Samples.wValidBitsPerSample);
		if (IsEqualGUID(m_Wfe.SubFormat, KSDATAFORMAT_SUBTYPE_PCM)) {
			printf("PCM format\n");
		}

		if (IsEqualGUID(m_Wfe.SubFormat, KSDATAFORMAT_SUBTYPE_IEEE_FLOAT)) {
			printf("IEEE float format\n");
		}
	} else {

		memcpy(&m_Wfe, wpfx, sizeof(WAVEFORMATEX));
	}
	return S_OK;
}



HRESULT MyAudioSource::LoadData(UINT32 bufferFrameCount, BYTE* pData, DWORD* flags) {
	//printf("Request Load Data size=%d\n", bufferFrameCount);
	float* pSample = (float*)pData;
	if (first) {
		for (int i = 0; i < bufferFrameCount / m_Wfe.Format.nChannels; i++) {
			*(pSample++) = 0.0;
		}
		first = false;
	}
	else {
		int index = 0;
		int period = 1000;
		float min = 1.0;
		float max = -1.0;
		int numSamples = bufferFrameCount;
		for (int i = 0; i < numSamples; i++) {

			if (index < period / 2) {
				*pSample = min;
				pSample++;
				*pSample = min;
				pSample++;
			}
			else {
				*pSample = max;
				pSample++;
				*pSample = max;
				pSample++;
			}

			index++;
			if (index == 1000)
				index = 0;
		}
	}



	return S_OK;
}

