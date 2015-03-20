
#include <stdio.h>

#include "ElaborationUnitProperty.h"
#include "PCMWaveformKind.h"
#include "..\Facilities\General\GainProperty.h"

PCMWaveformKind::PCMWaveformKind()
{
	//GainProperty* gain = new GainProperty("Output level");
	//addProperty(gain);
}

PCMWaveformKind::~PCMWaveformKind()
{
}

const char* PCMWaveformKind::getName(void) const
{
	return "PCM Waveform";
}

bool PCMWaveformKind::isPhysical(void) const
{
	return false;
}

int PCMWaveformKind::getInputPortNumber(void) const
{
	return PCMWAV_INPUT_PORT_NUMBER;
}

int PCMWaveformKind::getOutputPortNumber(void) const
{
	return PCMWAV_OUTPUT_PORT_NUMBER;
}

const char* PCMWaveformKind::getNthInputPortName(int portIndex) const
{
	switch(portIndex)
	{
	case 0:
		return PhasePortName;
		break;
	case 1:
		return AmplitudePortName;
		break;
	case 2:
		return MIDIPortName;
		break;
	default:
		return NULL;
	}
}

const char* PCMWaveformKind::getNthOutputPortName(int portIndex) const
{
	switch(portIndex)
	{
	case 0:
		return MainOutPortName;
	default:
		return NULL;
	}
}

const char* PCMWaveformKind::getNthInputPortKind(int portIndex) const
{
	switch(portIndex)
	{
	case 0:
	case 1:
		return getAudioPortKind();
	case 2:
		return getMIDIPortKind();
		break;
	default:
		return NULL;
	}
}

const char* PCMWaveformKind::getNthOutputPortKind(int portIndex) const
{
	if(portIndex==0)
		return getAudioPortKind();
	else
		return NULL;
}

bool PCMWaveformKind::isFinalElaborationUnit() const
{
	return false;
}

bool PCMWaveformKind::isInputPortConnectionMandatory(int portIndex) const
{
	switch(portIndex)
	{
	case PHASE_INPUT_INDEX:
		return false;
		break;
	case AMPLITUDE_INPUT_INDEX:
		return false;
		break;
	case MIDI_INPUT_INDEX:
		return true;
		break;
	default:
		return false;
	}
}

bool PCMWaveformKind::isOutputPortConnectionMandatory(int portIndex) const
{
	if(portIndex==MAIN_OUTPUT_INDEX)
		return true;
	else
		return false;
}


/*
const int PCMWaveformKind::getPropertyNumber() const
{
	return 1 + 1 + 1;
}
*/
/*
const char* PCMWaveformKind::getNthPropertyName(int propId) const
{
	switch(propId)
	{
	case C_OutputLevelIndex:
		{
			return "Output level";
			break;
		}
	case C_WaveFilename:
		{
			return "Wave filename";
			break;
		}
	case C_WavePitch:
		{
			return "Wave pitch";
			break;
		}
	default:
		return "";
	}
}
*/
/*
const int PCMWaveformKind::getNthPropertyDirection(int propId) const
{
	return 0;
}
*/
const char PCMWaveformKind::PhasePortName[] = "Phase In";
const char PCMWaveformKind::AmplitudePortName[] = "Ampl In";
const char PCMWaveformKind::MIDIPortName[] = "MIDI In";
const char PCMWaveformKind::MainOutPortName[] = "Out";