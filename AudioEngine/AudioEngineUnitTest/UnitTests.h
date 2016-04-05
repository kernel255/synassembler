#pragma once

enum UnitTest
{
	SimpleOsc,
	MixerReadWrite,
	MixerPlay1Osc
} ;


int TestMixerReadWriteProps(int algoId);
int TestMixer1Osc(int algoId);
int TestSimpleOsc(int algoId);
