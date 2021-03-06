

#include "WaveGeneratorFacilities.h"
#include "LFO.h"
#include <stddef.h>
#include <math.h>

EAG_SAMPLE_TYPE WaveGeneratorFacilities::getSample(TimeAccumulatedVoice* pVoice, WaveKind kind, double samplingFrequence, LFO* freqLFO)
{
	double lfo = (freqLFO != NULL && freqLFO->m_Enable) ? freqLFO->getSample(samplingFrequence) : 0.0;
	switch (kind)
	{
		case WaveKind::e_Sine:
		{
			double period = pVoice->getPeriod();
			//return sin((pVoice->getTimeAccumulated() * 2.0 * 3.14159265358979323846 + lfo * 1.0) / period);
			return sin(((pVoice->getTimeAccumulated() + lfo) * 2.0 * 3.14159265358979323846) / period);
		}
		case WaveKind::e_Square:
		{
			double time = pVoice->getTimeAccumulated();
			double period = pVoice->getPeriod();
			double currentAccu = time - ((int)(time / period))*period;
			if (currentAccu >= (period / 2.0))
				return +1.0;
			else
				return -1.0;
			break;
		}
		case WaveKind::e_Saw:
		{
			double time = pVoice->getTimeAccumulated();
			double period = pVoice->getPeriod();

			double currentAccu = time - ((int)(time / period))*period;
			EAG_SAMPLE_TYPE level = (currentAccu/period) * 2.0 - 1.0;
			return level;
			break;
		}
		case WaveKind::e_Triangle:
		{
			double time = pVoice->getTimeAccumulated();
			double period = pVoice->getPeriod();
			double currentAccu = time - ((int)(time / period))*period;
			EAG_SAMPLE_TYPE level;
			if (currentAccu < period / 4.0)
			{
				// First grow up
				level = (currentAccu / (period))*4.0;
			}
			else
			{
				if (currentAccu >= (period*3.0) / 4.0)
				{
					// Final grow up
					level = -1.0 + ( (currentAccu - (3.0/4.0*period)) / (period))*4.0;
				}
				else
				{
					// Go down
					level = 1.0 - ((currentAccu - (1.0/4.0*period)) / (period))*4.0;
				}
			}
			return level;
		}
		default:
		{
			return 0.0;
		}
	}
}