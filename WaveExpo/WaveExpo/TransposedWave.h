#pragma once

#include "IWaveCalculator.h"

class TransposedWave : public IWaveCalculator
{
public:
	virtual void calculateSample(WAVFile* wavFile);
};