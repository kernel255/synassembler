#pragma once

#include <vector>

class WAVFile
{
public:
	WAVFile(const wchar_t * name, int samplingFreq, int sampleResolution, int numChannels, int bitPerSample);
	~WAVFile();
	int writeFile();
	void addSample(double sample, int channel);
	void play();
private:
	std::vector<double>** m_SamplesVectors;
	int m_SamplingFreq;
	int m_SampleResolution;
	int m_NumChannels;
	int m_BitPerSample;
	wchar_t* m_Name;
	void writeSamples(std::ofstream &wavbinstream);
};