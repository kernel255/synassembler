// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

#include "AudioEngine.h"

AudioEngine* audioEngine = NULL;

wchar_t* plugInFolderName; //????

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			//if(audioEngine==NULL)
			//	audioEngine = ::getAudioEngine();
			
			int i = 0;
			break;
		}
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		//::releaseAudioEngine();
		break;
	}
	return TRUE;
}

