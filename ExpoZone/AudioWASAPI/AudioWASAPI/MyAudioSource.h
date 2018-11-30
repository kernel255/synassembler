#pragma once

#include <Windows.h>
#include <mmreg.h>

class MyAudioSource
{
private:
	WAVEFORMATEXTENSIBLE m_Wfe;
	bool first;
public:
	MyAudioSource();
	~MyAudioSource();

	HRESULT SetFormat(WAVEFORMATEX *wpfx);
	HRESULT LoadData(UINT32 bufferFrameCOunt, BYTE* pData, DWORD* flags);
};

