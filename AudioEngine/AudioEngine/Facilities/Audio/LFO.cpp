
#include <stddef.h>
#include "LFO.h"
#include "WaveGeneratorFacilities.h"

LFO::LFO(ModuleServices* pService)
{
	m_Enable = false;
	m_TimeAccumulated = 0.0;
	m_pModuleServices = pService;
}

double LFO::getTimeAccumulated()
{
	return m_TimeAccumulated;
}

EAG_SAMPLE_TYPE LFO::getSample(double samplingFrequency)
{
	if (m_TimeAccumulated > m_Delay)
	{
		return WaveGeneratorFacilities::getSample(this, m_WaveKind, samplingFrequency, NULL);
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
	if (m_FixRatio)
	{
		
		return 1.0 / (0.002 * m_CarrFreq);
	}
	else
		return 1.0 / m_Frequency;
}

void LFO::setCarrierFrequence(double freq) {
	m_pModuleServices->pLogger->writeLine("LFO Freq: %f", freq);
	m_CarrFreq = freq;
}