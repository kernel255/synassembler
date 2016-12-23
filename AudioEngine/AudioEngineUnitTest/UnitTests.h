#pragma once

enum UnitTest
{
	SimpleOsc,
	MixerReadWrite,
	MixerPlay1Osc,
	MixerPlay2OSc,
	WrongEUAddedToAlgo,
	MixerWAV
} ;

int TestSimpleOsc(int algoId);
int TestMixerReadWriteProps(int algoId);
int TestMixer1Osc(int algoId);
int TestMixer2Osc(int algoId);
int TestWrongIdAddedToAlgo(int algoId);
int TestMixerWAV(int algoId);
