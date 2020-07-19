#include "TransposedWave.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <Windows.h>

void TransposedWave::calculateSample(WAVFile * wavFile)
{
	double step = 1.0 / 44100.0;
	double time = 0.0;
	double sample;
	int totalSamples = 400000;
	double freq;
	int guard = totalSamples;//11025;// 1000;
	double startFreq = 220.0, endFreq = 440;
	double fStep = (endFreq - startFreq) / ((double)guard * 2);
	int fStepCounter = 0;
	for (int i = 0; i < totalSamples; i++)
	{
		freq = (i < totalSamples / 2) ? startFreq : endFreq;
		// Applied only while crossing between the two areas
		if (i > (totalSamples / 2 - guard) && i < (totalSamples / 2 + guard))
		{

			freq = startFreq + (endFreq - startFreq) / 2 * time;


			//freq = loFreq + ((double)fStepCounter++) * fStep;
		}
		

		// Print freq
		/*
		char* buffer = new char[1000];
		memset(buffer, 0, 1000);
		sprintf_s(buffer, 1000, "i=%d freq=%f\n",i , freq);
		OutputDebugStringA(buffer);
		*/

		sample = sin(2 * M_PI * time * freq);
		time += step;
		wavFile->addSample(sample, 0);
		wavFile->addSample(sample, 1);
	}

}

/*
void TransposedWave::calculateSample1(WAVFile * wavFile)
{
	double step = 1.0 / 44100.0;
	double time = 0.0;
	double sample;
	int totalSamples = 40000;
	double freq;
	int guard = 1000;
	double hiFreq = 440.0, loFreq = 415.30;
	double fStep = (hiFreq - loFreq) / ((double)guard * 2);
	int fStepCounter = 0;
	for (int i = 0; i < totalSamples; i++)
	{
		freq = (i < totalSamples / 2) ? loFreq : hiFreq;
		if (i >(totalSamples / 2 - guard) && i < (totalSamples / 2 + guard))
		{
			freq = loFreq + ((double)fStepCounter++) * fStep;
		}
		// Print freq
		char* buffer = new char[1000];
		memset(buffer, 0, 1000);
		sprintf_s(buffer, 1000, "i=%d freq=%f\n", i, freq);
		OutputDebugStringA(buffer);

		sample = sin(2 * M_PI * time * freq);
		time += step;
		wavFile->addSample(sample, 0);
		wavFile->addSample(sample, 1);
	}

}
*/