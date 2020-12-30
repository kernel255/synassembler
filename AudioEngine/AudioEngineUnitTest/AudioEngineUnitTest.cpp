
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

bool initCompleted = false;
int algoId;

BOOL WINAPI consoleHandler(DWORD signal) {

	if (signal == CTRL_C_EVENT)
	{
		printf("Ctrl-C handled\n");
		// Cleanup
		if (initCompleted)
		{
			printf("Releasing AudioEngine\n");
			::stopAlgorithm(algoId);
			::destroyAlgorithm(algoId);
			::releaseEngine();
		}

	}
	return FALSE;
}



int _tmain(int argc, _TCHAR* argv[])
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	printf("AudioEngine unit test\n");

	if (!SetConsoleCtrlHandler(consoleHandler, TRUE)) {
		printf("\nERROR: Could not set control handler");
		return 1;
	}

	std::wstring folderName = argv[0];
	size_t found = folderName.find_last_of(L"\\");
	if(found!=std::wstring::npos)
	{
		folderName = folderName.substr(0,found);
	}

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
	enum UnitTest unitTest = SimpleOscMIDI;
	//UnitTest unitTest = WrongEUAddedToAlgo;
	//Algorith build
	algoId = ::createAlgorithm();

	initCompleted = true;
	int result;
	switch (unitTest)
	{
	case SimpleOsc:
		result = TestSimpleOsc(algoId);
		break;
	case MixerReadWrite:
		result = TestMixerReadWriteProps(algoId);
		break;
	case MixerPlay1Osc:
		result = TestMixer1Osc(algoId);
		break;
	case MixerPlay2OSc:
		result = TestMixer2Osc(algoId);
		break;
	case WrongEUAddedToAlgo:
		result = TestWrongIdAddedToAlgo(algoId);
		break;
	case MixerWAV:
		result = TestMixerWAV(algoId);
		break;
	case SimpleOscTransposeTune:
		result = TestSimpleOscTransposeTune(algoId);
		break;
	case SimpleOscMIDI:
		result = TestSimpleOscMIDI(algoId);
		break;
	default:
		result = 0;
		break;
	}

	printf("Test result = %d", result);

	::stopAlgorithm(algoId);
	::destroyAlgorithm(algoId);

	::releaseEngine();

	printf("Finished\n");

	_CrtDumpMemoryLeaks();

	return result;
}

