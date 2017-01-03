
#include "PCMWaveform.h"
#include "..\Facilities\Audio\WavFileFormat.h"

PCMWaveform::PCMWaveform(ModuleServices *pService) : SimpleGenerator(pService, PCMWaveform::kinna.getPropertyNumber(), &PCMWaveform::kinna)
{
	m_cSampleBuffer = NULL;
	m_siSampleBuffer = NULL;
	m_SampleBuffer = NULL;
	m_pModuleServices->pLogger->writeLine("PCMWaveform constructor");
}

PCMWaveform::~PCMWaveform()
{
	m_pModuleServices->pLogger->writeLine("PCM Waveform destructor");
	if(m_cSampleBuffer != NULL)
		delete m_cSampleBuffer;
	if(m_siSampleBuffer != NULL)
		delete m_siSampleBuffer;
	if(m_SampleBuffer != NULL)
		delete m_SampleBuffer;
}

void PCMWaveform::allocate(void)
{
}

void PCMWaveform::deallocate(void)
{
}

void PCMWaveform::play(void)
{
}

void PCMWaveform::pause(void)
{
}

void PCMWaveform::stop(void)
{
}

const EUKind* PCMWaveform::getKind(void)
{
	return s_GetKind();
}

const EUKind* PCMWaveform::s_GetKind(void)
{
	return (EUKind *) &kinna;
}

int PCMWaveform::readWaveform(const char* filename)
{
	FILE* fp = ::fopen(filename,"rb");
	if(fp==NULL)
		return PCMWaveform::C_FailedOpenFile;
	::fseek(fp,0,SEEK_END);
	long fileLen = ::ftell(fp);
	::fseek(fp,0,SEEK_SET);

	char riffBuffer[C_RiffSize];
	if(::fread(riffBuffer,C_RiffSize,1,fp)!=1)
		return PCMWaveform::C_ErrorWhileReadingFile;
	if(strncmp(riffBuffer,"RIFF",C_RiffSize)!=0)
		return PCMWaveform::C_FormatError;
	int fileSize;
	if(::fread(&fileSize,sizeof(fileSize),1,fp)!=1)
		return PCMWaveform::C_FormatError;
	char waveFmtBuffer[C_WaveFmtSize];
	if(::fread(waveFmtBuffer,C_WaveFmtSize,1,fp)!=1)
		return PCMWaveform::C_ErrorWhileReadingFile;
	if(strncmp(waveFmtBuffer,"WAVEfmt ",C_WaveFmtSize)!=0)
		return PCMWaveform::C_FormatError;

	int formatSize;
	if(::fread(&formatSize,sizeof(formatSize),1,fp)!=1)
		return PCMWaveform::C_FormatError;
	if(fread(&m_WaveFileFormat,sizeof(m_WaveFileFormat),1,fp)!=1)
		return PCMWaveform::C_ErrorWhileReadingFile;
	if(m_WaveFileFormat.formatTag!=WavFileFormat::C_PCMFormat)
		return PCMWaveform::C_FormatError;
	if(m_WaveFileFormat.numChannels!=C_MaxChannels)
		return PCMWaveform::C_TooManyChannels;
	if(::fread(riffBuffer,C_RiffSize,1,fp)!=1)
		return PCMWaveform::C_FormatError;
	if(strncmp(riffBuffer,"data",C_RiffSize)!=0)
		return PCMWaveform::C_FormatError;
	int dataSize;
	if(::fread(&dataSize,sizeof(dataSize),1,fp)!=1)
		return PCMWaveform::C_FormatError;

	char* sampleBuffer = new char[dataSize];
	if(::fread(sampleBuffer,dataSize,1,fp)!=1)
		return PCMWaveform::C_FormatError;

	int numSamples;
	switch(m_WaveFileFormat.bitsPerSample)
	{
	case 8:
		{
			numSamples = dataSize;
			break;
		}
	case 16:
		{
			numSamples = dataSize / 2;
			break;
		}
	default:
		this->m_pModuleServices->pLogger->writeLine("Unsupported bits per sample");
		return PCMWaveform::C_FormatError;
	}
	m_NumSamples = numSamples;
	m_SampleBuffer = new EAG_SAMPLE_TYPE[numSamples];
	char* cWavPtr = sampleBuffer;
	short int* siWavPtr = (short int*) sampleBuffer;
	for(int i=0;i<numSamples;i++)
	{
		EAG_SAMPLE_TYPE currentSample;
		if(m_WaveFileFormat.bitsPerSample==8)
		{
			currentSample = *(cWavPtr++);
			currentSample = (currentSample - 128.0) /128.0;
			m_SampleBuffer[i] = currentSample;
		}
		else
		{
			currentSample = *(siWavPtr++);
			currentSample /= 32768;
			m_SampleBuffer[i] = currentSample;
		}
	}
	delete sampleBuffer;


	::fclose(fp);
	return 0;
}

EAG_SAMPLE_TYPE thirdSplineInterp(const EAG_SAMPLE_TYPE x,const EAG_SAMPLE_TYPE L1,const EAG_SAMPLE_TYPE L0,const
EAG_SAMPLE_TYPE H0,const EAG_SAMPLE_TYPE H1)
{
	return
	L0 +
	.5f*
	x*(H0-L1 +
	x*(H0 + L0*(-2) + L1 +
	x*( (H0 - L0)*9 + (L1-H1)*3 +
	x*((L0 - H0)*15 + (H1-L1)*5 +
	x*((H0 - L0)*6 + (L1 - H1)*2 )))));
} 

EAG_SAMPLE_TYPE PCMWaveform::getNthSampleFrom(int startFrom, int delta)
{
	int adjPos = startFrom + delta;
	if(adjPos<0 || adjPos>=this->m_NumSamples)
	{
		return EAG_SAMPLE_ZERO
	}
	else
		return m_SampleBuffer[adjPos];
}

SimpleGenerator::SampleCalculationResult PCMWaveform::calculateSample(EAG_SAMPLE_TYPE& result, SimpleVoice& simpleVoice)
{
	
	EAG_SAMPLE_TYPE alpha = ((EAG_SAMPLE_TYPE) m_WaveFileFormat.samplesPerSec) / m_SamplingTime;
	EAG_SAMPLE_TYPE beta = simpleVoice.m_InitialFrequency / m_Pitch;
	
	EAG_SAMPLE_TYPE timeAcc = (simpleVoice.m_TimeAccumulator / m_SamplingTime) * beta;
	EAG_SAMPLE_TYPE pureTimeAcc = timeAcc;
	// The JLear trick!
	timeAcc += .5;
	int samplePointer = (int) timeAcc;
	if(samplePointer>timeAcc)
	{
		//Notify voice has finished
		return SampleCalculationResult::CALCULATION_COMPLETED;
	}
	else
	{
#define SPLINE 0
#ifndef SPLINE
		result = m_SampleBuffer[samplePointer];
		return SampleCalculationResult::CALCULATION_CONTINUE;
#else
		EAG_SAMPLE_TYPE x = pureTimeAcc - samplePointer;
		EAG_SAMPLE_TYPE L0 = getNthSampleFrom(samplePointer, 0);
		EAG_SAMPLE_TYPE H0 = getNthSampleFrom(samplePointer, +1);
		EAG_SAMPLE_TYPE L1 = getNthSampleFrom(samplePointer, -1);
		EAG_SAMPLE_TYPE H1 = getNthSampleFrom(samplePointer, +2);
			
		result = thirdSplineInterp(x,L1,L0,H0,H1);
		return SampleCalculationResult::CALCULATION_CONTINUE;
#endif //SPLINE
	}
}

int PCMWaveform::setProperty(int propertyIndex, const char* value)
{
	switch(propertyIndex)
	{
	case PCMWaveformKind::C_OutputLevelIndex:
		{
			double dBuffer = 0.0;
			int res = sscanf(value,"%f", &dBuffer);
			if(res==1)
				this->m_OutputLevel = dBuffer;
			else
				this->m_pModuleServices->pLogger->writeLine("Error while converting output level");
			break;
		}
	case PCMWaveformKind::C_WaveFilename:
		{
		m_pModuleServices->pLogger->writeLine("WaveFilename selected: %s", value);
			int res = readWaveform(value);
			if(!res)
				m_WaveFilename.assign(value);
			else
			{
			}
			break;
		}
	case PCMWaveformKind::C_WavePitch:
		{
			//Defualt value set to C4
			int iBuffer =50;
			int res = sscanf(value,"%d", &iBuffer);
			if(res==1)
			{
				m_Pitch = MIDIChannelMessage::GetFreqByMIDINote(iBuffer);
				if (m_Pitch == 0.0)
					this->m_pModuleServices->pLogger->writeLine("Error while calculating note %d", iBuffer);
			}
			else
				this->m_pModuleServices->pLogger->writeLine("Error while converting pitch");
			break;
		}

	}
	return 0;
}

int PCMWaveform::getProperty(int propertyIndex, char* value, int bufferSize)
{
	memset(value,'\0',bufferSize);
	switch(propertyIndex)
	{
	case PCMWaveformKind::C_OutputLevelIndex:
		{
			sprintf_s(value,bufferSize,"%f",this->m_OutputLevel);
			return C_PropertyGetSucceess;
			break;
		}
	case PCMWaveformKind::C_WaveFilename:
		{
			if(m_WaveFilename.size()>bufferSize)
				return C_PropertyGetFailure;
			else
			{
				strncpy(value, m_WaveFilename.c_str(),bufferSize);
				return C_PropertyGetSucceess;
			}
			break;
		}
	case PCMWaveformKind::C_WavePitch:
		{
			sprintf_s(value,bufferSize,"%d",m_Pitch);
			return C_PropertyGetSucceess;
			break;
		}
	default:
		return C_PropertyGetFailure;
	}
}

PCMWaveformKind PCMWaveform::kinna;