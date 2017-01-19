
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

	std::wstring m_WaveFilename;
	long m_LoopStartPoint;
	long m_LoopEndPoint;
	double m_OutputLevel;
	/**
	 @var MIDI note number of the original sample
	 */
	int m_iMIDIPitch;
	/**
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

public:
	static void* getAmplitude(void* pEU)
	{
		PCMWaveform* pPCMWav = (PCMWaveform *)pEU;
		pPCMWav->m_pModuleServices->pLogger->writeLine("Read PCMWaveform Amplitude: %f", pPCMWav->m_Amplitude);
		return &(pPCMWav->m_Amplitude);
	}
	static bool setAmplitude(void* pEU, void* value) {
		PCMWaveform* pPCMWav = (PCMWaveform *)pEU;
		double* amplitude = (double*)value;
		pPCMWav->m_Amplitude = *amplitude;
		char buf[50];
		memset(buf, 0, 50);
		sprintf(buf, "Write PCMWaveform Amplitude: %f", pPCMWav->m_Amplitude);
		pPCMWav->m_pModuleServices->pLogger->writeLine(buf);
		return true;
	}

	static void* getPitch(void* pEU)
	{
		PCMWaveform* pWav = (PCMWaveform*)pEU;
		pWav->m_pModuleServices->pLogger->writeLine("getPicth: %d", pWav->m_Pitch);
		return &(pWav->m_iMIDIPitch);
	}
	static bool setPitch(void* pEU, void* value)
	{
		PCMWaveform* pWav = (PCMWaveform *)pEU;
		int* pitch = (int*)value;
		pWav->m_pModuleServices->pLogger->writeLine("setPitch: %d", *pitch);
		pWav->m_iMIDIPitch = (*pitch);
		pWav->m_Pitch = MIDIChannelMessage::GetFreqByMIDINote(pWav->m_iMIDIPitch);
		return true;
	}

	static void* getWavename(void* pEU)
	{
		PCMWaveform* pWav = (PCMWaveform*)pEU;
		pWav->m_pModuleServices->pLogger->writeLine("getWavename: %s", pWav->m_WaveFilename);
		const wchar_t* ptr = pWav->m_WaveFilename.c_str();
		return (wchar_t*)ptr;
	}
	static bool setWavefilename(void* pEU, void* value)
	{
		PCMWaveform* pWav = (PCMWaveform *)pWav;
		wchar_t* waveFilename = (wchar_t*)value;
		pWav->m_WaveFilename.assign(waveFilename);
		pWav->m_pModuleServices->pLogger->writeLine("setWavename: %s", waveFilename);
		return true;
	}
};