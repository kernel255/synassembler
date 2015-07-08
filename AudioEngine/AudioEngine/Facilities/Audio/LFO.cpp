
#include "LFO.h"
#include "WaveGeneratorFacilities.h"

double LFO::getTimeAccumulated()
{
	return m_TimeAccumulated;
}

EAG_SAMPLE_TYPE LFO::getSample(double samplingFrequency)
{
	if (m_TimeAccumulated > m_Delay)
	{
		return WaveGeneratorFacilities::getSample(this, m_WaveKind, samplingFrequency);
	}
	else
	{
		return EAG_SAMPLE_ZERO;
	}
}

void LFO::increaseAccumulatedTime(double time)
{
	m_TimeAccumulated += time;
}

double LFO::getPeriod()
{
	return 1.0 / m_Frequency;
}