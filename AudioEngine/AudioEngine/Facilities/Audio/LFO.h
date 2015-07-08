
#pragma once

#include "TimeAccumulatedVoice.h"
#include "..\..\DSPTypes.h"
#include "WaveKind.h"

class LFO : public TimeAccumulatedVoice
{
public:
	double getTimeAccumulated();
	double getPeriod();

	EAG_SAMPLE_TYPE getSample(double samplingFrequency);

	void increaseAccumulatedTime(double time);

	double m_Amplitude;
	/**
	* Delay before starting to oscillate
	*/
	double m_Delay = 0.0;
	WaveKind m_WaveKind = WaveKind::e_Sine;
	bool m_Enable = false;
	double m_Frequency = 20;

private:
	double m_TimeAccumulated;
};