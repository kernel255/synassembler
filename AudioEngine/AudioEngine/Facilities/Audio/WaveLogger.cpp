#include "WaveLogger.h"
#include "../General/Logger.h"
#include "WavFileFormat.h"
#include <stdlib.h>
#include <assert.h>

void WaveLogger::open(char* filename, int samplingFrequency, int numChannels, int bitResolution, Logger* _logger)
{
	valid = false;
	outputFilename = filename;
	rawFilename = filename;
	rawFilename += ".raw";
	waveFilename = filename;
	waveFilename += ".wav";
	internalWaveFormat.samplingFrequency = samplingFrequency;
	internalWaveFormat.numChannels = numChannels;
	internalWaveFormat.bitResolution = bitResolution;
	logger = _logger;
	errno_t err = fopen_s(&file, rawFilename.c_str(), "wb");
	if(file!=NULL)
	{
		valid = true;
	}
}

void WaveLogger::close(void)
{
	
	if(valid)
	{
		
		//Close the raw file;
		fclose(file);
		//Open the WAV file
		FILE *fpOut, *fpIn;
		errno_t err1, err2;
		err1 = fopen_s(&fpOut, waveFilename.c_str(), "wb");
		if(err1!=0)
		{
			logger->writeLine("Failed to open output file");
			return;
		}
		err2 = fopen_s(&fpIn, rawFilename.c_str(), "rb");
		if(err2!=0)
		{
			logger->writeLine("Failed to open input file");
			fclose(fpOut);
			return;
		}
		fseek(fpIn,0,SEEK_END);
		long size = ftell(fpIn);
		fseek(fpIn,0,SEEK_SET);
		char* rawWave = (char *) malloc(size);
		fread(rawWave,size,1,fpIn);
		fclose(fpIn);

		writeWaveFile(fpOut,internalWaveFormat.samplingFrequency,internalWaveFormat.bitResolution,internalWaveFormat.numChannels, rawWave, size);


		free(rawWave);

		fclose(fpOut);

	}
}

/**
 * Write wav file data but letfs the open close responsibility to the caller
 */

#define PCM_WAVE_FMT 0x0001

void WaveLogger::writeWaveFile(FILE* fpWave, int samplingFrequency, int bitResolution, int numChannels, char* data, int size)
{
	char buffer[10];
	memset(buffer,'\0',10);
	sprintf(buffer,"%d",size);
	std::string str = "File size=";
	str += buffer;
	this->logger->writeLine(str.c_str());
	const char RIFF[] = "RIFF";
	const char RIFF_WAVE_FMT[] = "WAVEfmt ";
	const char DATA_FMT[] = "data";
	unsigned int riffSize = size + strlen(RIFF_WAVE_FMT) + sizeof(WavFileFormat) + strlen(DATA_FMT) + sizeof(size)*2;
	unsigned int fmtSize = sizeof(WavFileFormat);
	fwrite(RIFF,strlen(RIFF),1,fpWave);
	fwrite(&riffSize,sizeof(riffSize),1,fpWave);
	fwrite(RIFF_WAVE_FMT,strlen(RIFF_WAVE_FMT),1,fpWave);
	fwrite(&fmtSize,sizeof(fmtSize),1,fpWave);
	//HEADER DATA SIZE: 20 bytes
	WavFileFormat format;
	format.formatTag = PCM_WAVE_FMT;
	format.numChannels = numChannels;
	format.samplesPerSec = samplingFrequency * numChannels;
	format.bytesPerSec = samplingFrequency * numChannels * (bitResolution / 8);
	format.bitsPerSample = bitResolution;
	format.blockAlign = bitResolution / 8 * numChannels;
	fwrite(&format,sizeof(format),1,fpWave);

	fwrite(DATA_FMT,strlen(DATA_FMT),1,fpWave);
	fwrite(&size,sizeof(size),1,fpWave);
	fwrite(data,size,1,fpWave);
}

void WaveLogger::write(char* samples, int numSamples)
{
	static int totSamples = 0;

	if(valid)
	{
		switch(internalWaveFormat.bitResolution)
		{
		case 8:
			{
				unsigned char* sample = (unsigned char*) samples;
				for(int i=0;i<numSamples;i++)
					fwrite(&sample[i],1,1,file);
				break;
			}
		case 16:
			{
				totSamples += numSamples;
				printf("Wrinting %d samples %d total\n", numSamples, totSamples);
				short* sample = (short *) samples;
				for(int i=0;i<numSamples;i++)
					fwrite(&sample[i],2,1,file);
				break;
			}
		default:
			assert("BitResolution unknown!");
			break;
		}
	}
}

WaveLogger::~WaveLogger()
{
}