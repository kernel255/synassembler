
#include <stdio.h>

#include "ElaborationUnitProperty.h"
#include "OscillatorKind.h"
#include "..\Facilities\General\GainProperty.h"
#include "..\Facilities\General\IntegerProperty.h"
#include "..\..\AudioEngine\Facilities\Audio\SimpleGenerator.h"
#include "Oscillator.h"

OscillatorKind::OscillatorKind()
{
	// #0
	GainProperty* gain = new GainProperty("Output level");
	gain->setGetter(Oscillator::getAmplitude);
	gain->setSetter(Oscillator::setAmplitude);
	addProperty(gain);
	// #1
	IntegerProperty* waveKind = new IntegerProperty("Wave Kind");
	waveKind->setSetter(Oscillator::setWaveKind);
	waveKind->setGetter(Oscillator::getWaveKind);
	addProperty(waveKind);
	// #2
	gain = new GainProperty("AttackTime");
	gain->setGetter(SimpleGenerator::getAttackTime);
	gain->setSetter(SimpleGenerator::setAttackTime);
	addProperty(gain);
	// #3
	gain = new GainProperty("DecayTime");
	gain->setGetter(SimpleGenerator::getDecayTime);
	gain->setSetter(SimpleGenerator::setDecayTime);
	addProperty(gain);
	// #4
	gain = new GainProperty("ReleaseTime");
	gain->setGetter(SimpleGenerator::getReleaseTime);
	gain->setSetter(SimpleGenerator::setReleaseTime);
	addProperty(gain);
	// #5
	gain = new GainProperty("AttackLevel");
	gain->setGetter(SimpleGenerator::getAttackLevel);
	gain->setSetter(SimpleGenerator::setAttackLevel);
	addProperty(gain);
	// #6
	gain = new GainProperty("SustainLevel");
	gain->setGetter(SimpleGenerator::getSustainLevel);
	gain->setSetter(SimpleGenerator::setSustainLevel);
	addProperty(gain);
}

const char* OscillatorKind::getName(void) const
{
	return "Oscillator";
}

bool OscillatorKind::isPhysical(void) const
{
	return false;
}

int OscillatorKind::getInputPortNumber(void) const
{
	return OSC_INPUT_PORT_NUMBER;
}

int OscillatorKind::getOutputPortNumber(void) const
{
	return OSC_OUTPUT_PORT_NUMBER;
}

const char* OscillatorKind::getNthInputPortName(int portIndex) const
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

const char* OscillatorKind::getNthOutputPortName(int portIndex) const
{
	switch(portIndex)
	{
	case 0:
		return MainOutPortName;
	default:
		return NULL;
	}
}

const char* OscillatorKind::getNthInputPortKind(int portIndex) const
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

const char* OscillatorKind::getNthOutputPortKind(int portIndex) const
{
	if(portIndex==0)
		return getAudioPortKind();
	else
		return NULL;
}

bool OscillatorKind::isInputPortConnectionMandatory(int portIndex) const
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

bool OscillatorKind::isOutputPortConnectionMandatory(int portIndex) const
{
	if(portIndex==MAIN_OUTPUT_INDEX)
		return true;
	else
		return false;
}

bool OscillatorKind::isFinalElaborationUnit() const
{
	return false;
}

const char OscillatorKind::PhasePortName[] = "Phase In";
const char OscillatorKind::AmplitudePortName[] = "Amp In";
const char OscillatorKind::MIDIPortName[] = "MIDI In";
const char OscillatorKind::MainOutPortName[] = "Out";