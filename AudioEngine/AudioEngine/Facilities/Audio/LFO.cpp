
#include "LFO.h"
#include "WaveGeneratorFacilities.h"

double LFO::getTimeAccumulated()
{
	return m_TimeAccumulated;
}
double LFO::getPeriod()
{
	return m_Period;
}

void LFO::setFrequency(double freq)
{
	m_Period = 1.0 / freq;
}
double LFO::getFrequency(void)
{
	return 1.0 / m_Period;
}

void LFO::setAmplitude(double amplitude)
{
	m_Amplitude = amplitude;
}

double LFO::getAmplitude()
{
	return m_Amplitude;
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
