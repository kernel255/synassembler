#pragma once

#include "WAVFile.h"

class IWaveCalculator
{
public:
	virtual void calculateSample(WAVFile* wavFile) = 0;
};