	
#pragma once

struct WavFileFormat
{
	static const int C_PCMFormat = 0x0001;
	//static const char* C_RIFF_STR = "RIFF";

	unsigned short int formatTag;
	unsigned short int numChannels;
	unsigned int samplesPerSec;
	unsigned int bytesPerSec;
	unsigned short blockAlign;
	unsigned short int bitsPerSample;
};
