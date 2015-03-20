
#pragma once

#include <string>

#include "..\Facilities\Audio\SimpleVoice.h"
#include "..\Facilities\Audio\SimpleGenerator.h"
#include "..\Facilities\Audio\WavFileFormat.h"
#include "PCMWaveformKind.h"

class ModuleServices;

class PCMWaveform : public SimpleGenerator {
public:
	PCMWaveform(ModuleServices* pService);
	virtual ~PCMWaveform();
	virtual void allocate(void);
	virtual void deallocate(void);
	virtual void play(void);
	virtual void pause(void);
	virtual void stop(void);
	virtual const EUKind* getKind(void);
	static const EUKind* s_GetKind(void);
	virtual int setProperty(int propertyIndex, const char* value);
	virtual int getProperty(int propertyIndex, char* value, int bufferSize);
protected:
	virtual SampleCalculationResult calculateSample(EAG_SAMPLE_TYPE& result, SimpleVoice& simpleVoice);
private:
	int readWaveform(const char* filename);
	EAG_SAMPLE_TYPE getNthSampleFrom(int startFrom, int delta);

	static const int C_FailedOpenFile = -1;
	static const int C_ErrorWhileReadingFile = -2;
	static const int C_FormatError = -3;
	static const int C_TooManyChannels = -4;
	static const int C_RiffSize	= 4;
	static const int C_WaveFmtSize = 8;
	static const int C_MaxChannels = 1;

	std::string m_WaveFilename;
	long m_LoopStartPoint;
	long m_LoopEndPoint;
	double m_OutputLevel;
	/**
	 @var MIDI note number of the original sample
	 */
	double m_Pitch;
	/**
	 @var Number of samples in the wave
	 */
	int m_NumSamples;
	char* m_cSampleBuffer;
	short int* m_siSampleBuffer;
	EAG_SAMPLE_TYPE* m_SampleBuffer;
	WavFileFormat m_WaveFileFormat;

	static PCMWaveformKind kinna;
};