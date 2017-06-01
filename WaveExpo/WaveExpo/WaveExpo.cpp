// WaveExpo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "WAVFile.h"
#include "TransposedWave.h"

int main()
{
	WAVFile wav(_T("transposed.wav"), 44100, 16, 2, 16);
	TransposedWave transp;

	transp.calculateSample(&wav);
	wav.writeFile();

    return 0;
}

