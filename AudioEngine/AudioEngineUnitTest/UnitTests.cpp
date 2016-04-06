
#include "UnitTests.h"
#include "Utilities.h"
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
	::setEUDProperty(mixId, 1, lvl);
	double res;
	::getEUDProperty(mixId, 1, &res);

	if (lvl != res)
	{
		return -1;
	}

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

	::stopAlgorithm(algoId);
	::destroyAlgorithm(algoId);


	return 0;
}