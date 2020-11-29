
#include "stdafx.h"

#define _CRTDBG_MAP_ALLOC

#include <stdlib.h>
#include <crtdbg.h>
#include <windows.h>
#include <string>

#include "AudioEngineAPI.h"

void displayAudioEngine(void);

#include "Utilities.h"
#include "UnitTests.h"

int _tmain(int argc, _TCHAR* argv[])
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	printf("AudioEngine unit test\n");

	std::wstring folderName = argv[0];
	size_t found = folderName.find_last_of(L"\\");
	if(found!=std::wstring::npos)
	{
		folderName = folderName.substr(0,found);
	}

	/*
#ifdef _DEBUG
	wchar_t* suffix = L"\\Debug\\";
#else
	wchar_t* suffix = L"\\Release\\";
#endif //_DEBUG

	DWORD size = ::GetCurrentDirectoryW(0, NULL);
	wchar_t* currentFolderName = new wchar_t[size];
	::GetCurrentDirectoryW(size, currentFolderName);

	std::wstring folderName = currentFolderName;
	size_t found = folderName.find_last_of(L"\\");
	if(found!=std::wstring::npos)
	{
		folderName = folderName.substr(0,found);
		folderName += suffix;
	}
	*/

	HWND hwnd = getConsoleHwnd();

	SpecificOSInfo specOSInfo;
	specOSInfo.hwnd = hwnd;
	EngineSettings engineSettings;

	engineSettings.bitResolution = 16;
	engineSettings.numChannels = 1;
	engineSettings.samplingFrequence = 44100;
	
	//::initEngine(folderName.c_str(), &specOSInfo, &engineSettings);
	::initEngine(folderName.c_str(),(int) hwnd,44100,16,1);
	//AudioEngine* audioEngine = ::getAudioEngine(folderName);
	displayAudioElements();

	if(::getFactoryNumber()==0)
	{
		printf("No factory available. Exiting...\n");
		::releaseEngine();

		return 0;
	}

	//UnitTest unitTest = MixerReadWrite;
	//UnitTest unitTest = SimpleOscTransposeTune;
	UnitTest unitTest = SimpleOscMIDI;
	//UnitTest unitTest = WrongEUAddedToAlgo;
	//Algorith build
	int algoId;
	algoId = ::createAlgorithm();

	int result;
	switch (unitTest)
	{
	case SimpleOsc:
		result = TestSimpleOsc(algoId);
	case MixerReadWrite:
		result = TestMixerReadWriteProps(algoId);
	case MixerPlay1Osc:
		result = TestMixer1Osc(algoId);
	case MixerPlay2OSc:
		result = TestMixer2Osc(algoId);
	case WrongEUAddedToAlgo:
		result = TestWrongIdAddedToAlgo(algoId);
	case MixerWAV:
		result = TestMixerWAV(algoId);
	case SimpleOscTransposeTune:
		result = TestSimpleOscTransposeTune(algoId);
	case SimpleOscMIDI:
		result = TestSimpleOscMIDI(algoId);
	default:
		result = 0;
	}

	printf("Test result = %d", result);

	::stopAlgorithm(algoId);
	::destroyAlgorithm(algoId);

	::releaseEngine();

	printf("Finished\n");

	_CrtDumpMemoryLeaks();

	return result;
}

