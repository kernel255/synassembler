#include "WAVFile.h"
#include <string.h>
#include <wchar.h>
#include <fstream>

using namespace std;

WAVFile::WAVFile(const wchar_t * name, int samplingFreq, int sampleResolution, int numChannels, int bitPerSample)
{
	m_SamplingFreq = samplingFreq;
	m_SampleResolution = sampleResolution;
	m_NumChannels = numChannels;
	m_BitPerSample = bitPerSample;
	size_t len = wcslen(name);
	m_Name = new wchar_t[len + 1];
	memset(m_Name, 0, len + 1);
	wcscpy_s(m_Name, len+1, name);
	m_SamplesVectors = new vector<double>*[numChannels];
	for (int channel = 0; channel < numChannels; channel++)
	{
		m_SamplesVectors[channel] = new vector<double>();
	}

}

int WAVFile::writeFile()
{
	std::ofstream wavbinstream;
	wavbinstream.open(m_Name, ios::out | ios::binary);
	int numSamples = m_SamplesVectors[0]->size();
	int32_t numSamplesBytes = numSamples * sizeof(double);
	int32_t totalSize = numSamples + 44;

	// RIFF
	const char* riff = "RIFF";
	wavbinstream.write(riff, sizeof(riff));
	// Size
	wavbinstream.write((char*)&totalSize, sizeof(int32_t));
	// WAV
	const char* wave = "WAVE";
	wavbinstream.write(wave, sizeof(wave));
	// fmt
	const char* fmt = "fmt ";
	wavbinstream.write(fmt, sizeof(fmt));
	int32_t subChunkSize = 16;
	wavbinstream.write((char*)&subChunkSize, sizeof(int32_t));
	// Audio Format
	int16_t audioFormat = 1;
	wavbinstream.write((char*)&audioFormat, sizeof(int16_t));
	// Num Channels
	int16_t channels = m_NumChannels;
	wavbinstream.write((char*)&channels, sizeof(int16_t));
	// Sample rate
	int32_t samplerate = m_SamplingFreq;
	wavbinstream.write((char*)&samplerate, sizeof(int32_t));
	// Byte rate
	int32_t byteRate = m_SamplingFreq * m_NumChannels * m_BitPerSample / 8;
	wavbinstream.write((char*)&byteRate, sizeof(int32_t));
	// Block align
	int16_t blockAlign = m_NumChannels * m_BitPerSample / 8;
	wavbinstream.write((char*)&blockAlign, sizeof(int16_t));
	// Bit per sample
	int16_t bitPerSample = m_BitPerSample;
	wavbinstream.write((char*)&bitPerSample, sizeof(int16_t));
	// Subchunk data
	const char* data = "data";
	wavbinstream.write((char*)data, sizeof(data));
	// Subchunk  size
	int32_t dataLen = numSamples * m_BitPerSample / 8 * m_NumChannels;
	wavbinstream.write((char*)&dataLen, sizeof(int32_t));
	// Samples
	writeSamples(wavbinstream);

	wavbinstream.close();

	return 0;
}

void WAVFile::addSample(double sample, int channel)
{
	m_SamplesVectors[channel]->push_back(sample);
}

void WAVFile::play()
{
}

void WAVFile::writeSamples(std::ofstream &wavbinstream)
{
	int numSamples = m_SamplesVectors[0]->size();
	for (int i = 0; i < numSamples; i++)
	{
		for (int channel = 0; channel < m_NumChannels; channel++)
		{
			double sample = m_SamplesVectors[channel]->at(i);
			int16_t iSample = 32767.0 * sample;
			wavbinstream.write((char*)&iSample, sizeof(int16_t));
		}
	}
}

WAVFile::~WAVFile()
{
	for (int channel = 0; channel < m_NumChannels; channel++)
	{
		delete m_SamplesVectors[channel];
	}
	delete m_SamplesVectors;

}