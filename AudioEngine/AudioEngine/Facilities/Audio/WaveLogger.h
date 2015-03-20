
#pragma once

#include <stdio.h>
#include "..\General\Logger.h"
#include  "WavFileFormat.h"
#include <string>

class WaveLogger {
private:
	FILE* file;
	char* outputFilename;
	char* inputFilename;
	bool valid;
	class InternalWaveFormat {
	public :
		int samplingFrequency;
		int numChannels;
		int bitResolution;
	};
	InternalWaveFormat internalWaveFormat;
	Logger* logger;
	std::string rawFilename, waveFilename;
	void writeWaveFile(FILE* fpWave, int samplingFrequency, int bitResolution, int numChannels, char* data, int size);
public:
	~WaveLogger();
	//WaveLogger(char* filename, int samplingFrequency, int numChannels, int bytesPerSamples, Logger* logger);
	void open(char* filename, int samplingFrequency, int numChannels, int bitResolution, Logger* logger);
	void close(void);
	void write(char* samples, int numSamples);

};