
#include "stdafx.h"

#define _CRTDBG_MAP_ALLOC

#include <stdlib.h>
#include <crtdbg.h>
#include <windows.h>
#include <string>

#include "AudioEngineAPI.h"

void displayAudioEngine(void);

#include "Utilities.h"

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

#define BASIC_EU_FACTORY	0
#define INOUT_EU_FACTORY	1

	//#####################################
	//####### POLY KEAYBOARD SAMPLE #######
	//#####################################

	/*
	//Algorith build
	int algoId, oscId, dsId, polyId;
	algoId = ::createAlgorithm();
	//Create oscillator
	oscId = ::createElaborationUnit(BASIC_EU_FACTORY,VIRTUAL_EU_CATEGORY,0);
	//Create DirectSound
	dsId = ::createElaborationUnit(INOUT_EU_FACTORY,PHYSICAL_EU_CATEGORY,0);
	//Create Polytrack keyboard
	polyId = ::createElaborationUnit(INOUT_EU_FACTORY,PHYSICAL_EU_CATEGORY,2);


	//Add to algorithm
	::addElaborationUnitToAlgorithm(algoId,oscId);
	::addElaborationUnitToAlgorithm(algoId,dsId);
	::addElaborationUnitToAlgorithm(algoId,polyId);
	//Connect EU
	::connectElaboratioUnits(algoId,oscId,0,dsId,0);
	::connectElaboratioUnits(algoId,polyId,0,oscId,2);

	//Play the algorithm
	::playAlgorithm(algoId);
	*/

	//#####################################
	//####### POLY KEAYBOARD SAMPLE #######
	//#####################################


	//####################################
	//####### MIDI SEQUENCE SAMPLE #######
	//####################################
	//Algorith build
	int algoId, dsId, seqId;
	algoId = ::createAlgorithm();
#define OSCILLATOR
//#define PCMWAVEFORM
#ifdef OSCILLATOR
	//Create oscillator
	int oscId = ::createElaborationUnit(BASIC_EU_FACTORY,VIRTUAL_EU_CATEGORY,0, -1);
	//Create DirectSound
	dsId = ::createElaborationUnit(INOUT_EU_FACTORY,PHYSICAL_EU_CATEGORY,0, 0);
	//Create Polytrack keyboard
	//seqId = ::createElaborationUnit(BASIC_EU_FACTORY,VIRTUAL_EU_CATEGORY,1, -1);
	//Create PC keyboard
	int pcKId = ::createElaborationUnit(INOUT_EU_FACTORY, PHYSICAL_EU_CATEGORY, 3, 0);
	//Add to algorithm
	::addElaborationUnitToAlgorithm(algoId,dsId);
	::addElaborationUnitToAlgorithm(algoId,oscId);
	//::addElaborationUnitToAlgorithm(algoId,seqId);
	::addElaborationUnitToAlgorithm(algoId, pcKId);
	//Connect EU
	::connectElaboratioUnits(algoId,oscId,0,dsId,0);
	//::connectElaboratioUnits(algoId,seqId,0,oscId,2);
	::connectElaboratioUnits(algoId, pcKId, 0, oscId, 2);
#endif //OSCILLATOR







#ifdef PCMWAVEFORM
	//Create PCM Waveform
	int pcmId = ::createElaborationUnit(BASIC_EU_FACTORY,VIRTUAL_EU_CATEGORY,2, 0);
	::setEUProperty(pcmId,1,"D:\\Mine\\Audinos\\WaveSamples\\Piano\\Yamaha-TG-55-Piano-C3-mono.wav");
	::setEUProperty(pcmId,2,"48");

	int pcmId2 = ::createElaborationUnit(BASIC_EU_FACTORY,VIRTUAL_EU_CATEGORY,2, 0);
	::connectElaboratioUnits(algoId,pcmId,0,pcmId2,0);

	//Create DirectSound
	dsId = ::createElaborationUnit(INOUT_EU_FACTORY,PHYSICAL_EU_CATEGORY,0, 0);
	//Create Polytrack keyboard
	seqId = ::createElaborationUnit(BASIC_EU_FACTORY,VIRTUAL_EU_CATEGORY,1, 0);
	//Add to algorithm
	::addElaborationUnitToAlgorithm(algoId,dsId);
	::addElaborationUnitToAlgorithm(algoId,pcmId);
	::addElaborationUnitToAlgorithm(algoId,seqId);
	//Connect EU
	::connectElaboratioUnits(algoId,pcmId,0,dsId,0);
	::connectElaboratioUnits(algoId,seqId,0,pcmId,2);


	char* validity = ::checkAlgorithmValidity(algoId);
	if(strlen(validity)>0)
	{
		printf("Algorithm vailidity failure: %s\n", validity);
		return -1;
	}



#endif //PCMWAVEFORM

	::playAlgorithm(algoId);

	/// STUDY SENDING MIDI MSG
	unsigned char buffer[3];

	buffer[0] = 0x90;
	buffer[1] = 0x41;
	buffer[2] = 0x7F;
	::sendMIDIMessage(pcKId, buffer);
	::Sleep(1000);

	/* 
	buffer[0] = 0x90;
	buffer[1] = 0x41;
	buffer[2] = 0x7F;
	::sendMIDIMessage(pcKId, buffer);
	::Sleep(1000);
	
	buffer[0] = 0x80;
	buffer[1] = 0x41;
	buffer[2] = 0x00;
	::sendMIDIMessage(pcKId, buffer);
	::Sleep(2000);
	*/
	
	buffer[0] = 0x80;
	buffer[1] = 0x41;
	buffer[2] = 0x00;
	::sendMIDIMessage(pcKId, buffer);
	::Sleep(1000);
	





	::stopAlgorithm(algoId);


//#define PLAY_SOMETHING 1

#ifdef PLAY_SOMETHING
	//Play the algorithm
	::playAlgorithm(algoId);
	//####################################
	//####### MIDI SEQUENCE SAMPLE #######
	//####################################

	//::Sleep(200000);



	//Short sleep
	::Sleep(1000);
	//Long sleep
	//::Sleep(1000000);
	
	for(int rep=0;rep<10;rep++)
	{
		double pow;
		::Sleep(500);
		int res = ::setEUProperty(oscId, 0, "0.90");
	}
	
	::stopAlgorithm(algoId);

#endif //PLAY_SOMETHING

//###############################
//####### MODIFY PROPERTY #######
//###############################

	
/*
#define BUFFER_SIZE 30
	char buffer[BUFFER_SIZE];
	memset(buffer,'\0',BUFFER_SIZE);
	int res = ::getEUProperty(oscId,0,buffer,BUFFER_SIZE);
	printf("Osc property read = %s\n", buffer);

	res = ::setEUProperty(oscId,0,"0.90");

	res = ::getEUProperty(oscId,0,buffer,BUFFER_SIZE);
	printf("Osc property read = %s\n", buffer);
*/
	

	::releaseEngine();

	printf("Finished\n");

	_CrtDumpMemoryLeaks();

	return 0;
}

