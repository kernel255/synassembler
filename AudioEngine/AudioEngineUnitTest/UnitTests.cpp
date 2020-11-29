
#include "UnitTests.h"
#include "Utilities.h"
#include <stdio.h>
#include "AudioEngineAPI.h"

int TestSimpleOsc(int algoId)
{
	//Create oscillator
	int oscId = ::createElaborationUnit(BASIC_EU_FACTORY, VIRTUAL_EU_CATEGORY, 0, -1);
	//Create DirectSound
	int dsId = ::createElaborationUnit(INOUT_EU_FACTORY, PHYSICAL_EU_CATEGORY, 0, 0);
	//Create Polytrack keyboard
	int seqId = ::createElaborationUnit(BASIC_EU_FACTORY, VIRTUAL_EU_CATEGORY, 1, -1);
	//Add to algorithm
	::addElaborationUnitToAlgorithm(algoId, dsId);
	::addElaborationUnitToAlgorithm(algoId, oscId);
	::addElaborationUnitToAlgorithm(algoId, seqId);
	//Connect EU
	::connectElaboratioUnits(algoId, oscId, 0, dsId, 0);
	::connectElaboratioUnits(algoId, seqId, 0, oscId, 2);

	::playAlgorithm(algoId);

	::Sleep(600000);

	::stopAlgorithm(algoId);
	::destroyAlgorithm(algoId);


	return 0;
}

int TestMixerReadWriteProps(int algoId)
{
	//Create oscillator
	int oscId = ::createElaborationUnit(BASIC_EU_FACTORY, VIRTUAL_EU_CATEGORY, 0, -1);
	//Create DirectSound
	int dsId = ::createElaborationUnit(INOUT_EU_FACTORY, PHYSICAL_EU_CATEGORY, 0, 0);
	//Create MIDI Sequencer
	int seqId = ::createElaborationUnit(BASIC_EU_FACTORY, VIRTUAL_EU_CATEGORY, 1, -1);
	// Create Mixer
	int mixId = ::createElaborationUnit(BASIC_EU_FACTORY, VIRTUAL_EU_CATEGORY, 3, -1);
	//Add to algorithm
	::addElaborationUnitToAlgorithm(algoId, dsId);
	::addElaborationUnitToAlgorithm(algoId, oscId);
	::addElaborationUnitToAlgorithm(algoId, seqId);
	::addElaborationUnitToAlgorithm(algoId, mixId);
	//::addElaborationUnitToAlgorithm(algoId, pcKId);
	//Connect EU
	::connectElaboratioUnits(algoId, oscId, 0, mixId, 0);
	::connectElaboratioUnits(algoId, seqId, 0, oscId, 2);
	::connectElaboratioUnits(algoId, mixId, 0, dsId, 0);

	double lvl = .3f;
	double res;
	for (int i = 0; i < 5; i++)
	{
		//::setEUDProperty(mixId, i, lvl);
		::getEUDProperty(mixId, i, &res);
		lvl += .1f;
	}


	bool mute = true;
	::setEUBProperty(mixId, 7, mute);
	bool muted;
	::getEUBProperty(mixId, 7, &muted);

	int transp;
	::getEUIProperty(oscId, 13, &transp);
	transp++;


	return 0;
}

int TestMixer1Osc(int algoId)
{
	//Create oscillator
	int oscId = ::createElaborationUnit(BASIC_EU_FACTORY, VIRTUAL_EU_CATEGORY, 0, -1);
	//Create DirectSound
	int dsId = ::createElaborationUnit(INOUT_EU_FACTORY, PHYSICAL_EU_CATEGORY, 0, 0);
	//Create MIDI Sequencer
	int seqId = ::createElaborationUnit(BASIC_EU_FACTORY, VIRTUAL_EU_CATEGORY, 1, -1);
	// Create Mixer
	int mixId = ::createElaborationUnit(BASIC_EU_FACTORY, VIRTUAL_EU_CATEGORY, 3, -1);
	//Add to algorithm
	::addElaborationUnitToAlgorithm(algoId, dsId);
	::addElaborationUnitToAlgorithm(algoId, oscId);
	::addElaborationUnitToAlgorithm(algoId, seqId);
	::addElaborationUnitToAlgorithm(algoId, mixId);
	//Connect EU
	::connectElaboratioUnits(algoId, seqId, 0, oscId, 2);
	::connectElaboratioUnits(algoId, oscId, 0, mixId, 0);
	//::connectElaboratioUnits(algoId, oscId, 0, dsId, 0);
	::connectElaboratioUnits(algoId, mixId, 0, dsId, 0);

	::playAlgorithm(algoId);

	::Sleep(600000);

	return 0;
}

int TestMixer2Osc(int algoId)
{
	//Create oscillators
	int oscId1 = ::createElaborationUnit(BASIC_EU_FACTORY, VIRTUAL_EU_CATEGORY, 0, -1);
	int oscId2 = ::createElaborationUnit(BASIC_EU_FACTORY, VIRTUAL_EU_CATEGORY, 0, -1);
	//Create DirectSound
	int dsId = ::createElaborationUnit(INOUT_EU_FACTORY, PHYSICAL_EU_CATEGORY, 0, 0);
	//Create MIDI Sequencer
	int seqId = ::createElaborationUnit(BASIC_EU_FACTORY, VIRTUAL_EU_CATEGORY, 1, -1);
	// Create Mixer
	int mixId = ::createElaborationUnit(BASIC_EU_FACTORY, VIRTUAL_EU_CATEGORY, 3, -1);
	//Add to algorithm
	int res = ::addElaborationUnitToAlgorithm(algoId, dsId);
	res = ::addElaborationUnitToAlgorithm(algoId, oscId1);
	res = ::addElaborationUnitToAlgorithm(algoId, oscId2);
	res = ::addElaborationUnitToAlgorithm(algoId, seqId);
	res = ::addElaborationUnitToAlgorithm(algoId, mixId);
	//Connect EU
	res = ::connectElaboratioUnits(algoId, seqId, 0, oscId1, 2);
	res = ::connectElaboratioUnits(algoId, seqId, 0, oscId2, 2);
	res = ::connectElaboratioUnits(algoId, oscId1, 0, mixId, 0);
	res = ::connectElaboratioUnits(algoId, oscId2, 0, mixId, 1);
	res = ::connectElaboratioUnits(algoId, mixId, 0, dsId, 0);

	::playAlgorithm(algoId);

	::Sleep(600000);

	return 0;

}

int TestWrongIdAddedToAlgo(int algoId)
{
	int res = ::addElaborationUnitToAlgorithm(algoId, 1);
	
	return res;
}

int TestMixerWAV(int algoId)
{
	int wavId1 = ::createElaborationUnit(BASIC_EU_FACTORY, VIRTUAL_EU_CATEGORY, 2, -1);
	int dsId = ::createElaborationUnit(INOUT_EU_FACTORY, PHYSICAL_EU_CATEGORY, 0, 0);
	int res = ::addElaborationUnitToAlgorithm(algoId, wavId1);
	res = ::addElaborationUnitToAlgorithm(algoId, dsId);
	res = ::connectElaboratioUnits(algoId, wavId1, 0, dsId, 0);
	
	::Sleep(100);

	::setEUProperty(wavId1, 1, L"pippo.wav");
	wchar_t* buffer = new wchar_t[100];
	memset(buffer, 0, 100);

	res = ::getEUProperty(wavId1, 1, buffer, 100);

	::setEUIProperty(wavId1, 2, 3);
	int pitch;
	res = ::getEUIProperty(wavId1, 2, &pitch);

	printf("Wave pitch: %d\n", pitch);

	return 0;
}

int TestSimpleOscTransposeTune(int algoId)
{
	//Create oscillator
	int oscId = ::createElaborationUnit(BASIC_EU_FACTORY, VIRTUAL_EU_CATEGORY, 0, -1);
	//Create DirectSound
	int dsId = ::createElaborationUnit(INOUT_EU_FACTORY, PHYSICAL_EU_CATEGORY, 0, 0);
	//Create Polytrack keyboard
	int seqId = ::createElaborationUnit(BASIC_EU_FACTORY, VIRTUAL_EU_CATEGORY, 1, -1);
	//Add to algorithm
	::addElaborationUnitToAlgorithm(algoId, dsId);
	::addElaborationUnitToAlgorithm(algoId, oscId);
	::addElaborationUnitToAlgorithm(algoId, seqId);
	//Connect EU
	::connectElaboratioUnits(algoId, oscId, 0, dsId, 0);
	::connectElaboratioUnits(algoId, seqId, 0, oscId, 2);

	::playAlgorithm(algoId);
	printf("Playing\n");
	::Sleep(100);
	//Change note
	/*
	for (int i = 0; i < 12; i++)
	{
		//printf("Change note %d\n", i);
		::setEUIProperty(oscId, 13, i);
		::Sleep(500);
	}
	*/
	for (int i = 0; i < 100; i++)
	{
		//printf("Change tune cent=%d\n", i);
		::setEUIProperty(oscId, 14, i);
		::Sleep(250);
	}
	/*
	printf("Change note +1\n");
	::setEUIProperty(oscId, 13, 1);
	::Sleep(1000);
	printf("Change note +2\n");
	::setEUIProperty(oscId, 13, 2);
	::Sleep(5000);
	*/



	::stopAlgorithm(algoId);
	::destroyAlgorithm(algoId);

	return 0;
}

int TestSimpleOscMIDI(int algoId)
{
	//Create oscillator
	int oscId = ::createElaborationUnit(BASIC_EU_FACTORY, VIRTUAL_EU_CATEGORY, 0, -1);
	//Create DirectSound
	int dsId = ::createElaborationUnit(INOUT_EU_FACTORY, PHYSICAL_EU_CATEGORY, 0, 0);
	//Create MIDI Input
	int midi = ::createElaborationUnit(INOUT_EU_FACTORY, PHYSICAL_EU_CATEGORY, 1, 1);
	//Add to algorithm
	::addElaborationUnitToAlgorithm(algoId, dsId);
	::addElaborationUnitToAlgorithm(algoId, oscId);
	::addElaborationUnitToAlgorithm(algoId, midi);
	//Connect EU
	::connectElaboratioUnits(algoId, oscId, 0, dsId, 0);
	::connectElaboratioUnits(algoId, midi, 0, oscId, 2);

	::playAlgorithm(algoId);

	::Sleep(600000);

	::stopAlgorithm(algoId);
	::destroyAlgorithm(algoId);


	return 0;



	return 0;
}
