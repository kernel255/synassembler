#include "TransposedWave.h"

void TransposedWave::calculateSample(WAVFile * wavFile)
{
	for (int i = 0; i < 300; i++)
	{
		for (int j = 0; j < 200; j++)
		{
			int channel = j % 2;
			double sample = (i % 2) ? 1.0 : -1.0;
			if (channel)
			{
				wavFile->addSample(sample, channel);
			}
			else
			{
				wavFile->addSample(sample, channel);
			}
		}
	}
}
