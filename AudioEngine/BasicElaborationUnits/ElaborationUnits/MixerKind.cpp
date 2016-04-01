
#include "MixerKind.h"
#include "Mixer.h"
#include "..\Facilities\General\GainProperty.h"


MixerKind::MixerKind()
{
	GainProperty* gain = new GainProperty("Output level");
	gain->setGetter(Mixer::getOutput);
	gain->setSetter(Mixer::setOutput);
	addProperty(gain);


	static const char* PREFIX = "Input ";
	for (int i = 0; i < C_NumInputPorts; i++)
	{
		char* buffer = new char[30];
		memset(buffer, 0, 30);
		sprintf(buffer, "Input %d", i);
		InputPortNames[i] = buffer;
		gain = new GainProperty(buffer);
		gain->setGetter(Mixer::getOutput);
		gain->setSetter(Mixer::setOutput);
		addProperty(gain);
	}

	// TODO: Add input ports maybe in a parametrizable way
}

const char* MixerKind::getName(void) const
{
	return "Mixer";
}

bool MixerKind::isPhysical(void) const
{
	return false;
}

int MixerKind::getInputPortNumber(void) const
{
	return C_NumInputPorts;
}

int MixerKind::getOutputPortNumber(void) const
{
	return C_NumOutputPorts;
}

const char* MixerKind::getNthInputPortName(int portIndex) const
{
	if (portIndex < C_NumInputPorts)
	{
		return InputPortNames[portIndex];
	}
	else
	{
		return NULL;
	}
}

const char* MixerKind::getNthOutputPortName(int portIndex) const
{
	switch (portIndex)
	{
	case 0:
		return OutputPortName;
	default:
		return NULL;
	}
}

const char* MixerKind::getNthInputPortKind(int portIndex) const
{
	if (portIndex < C_NumInputPorts)
	{
		return getAudioPortKind();
	}
	else
	{
		return NULL;
	}
}

const char* MixerKind::getNthOutputPortKind(int portIndex) const
{
	if (portIndex == 0)
		return getAudioPortKind();
	else
		return NULL;
}

bool MixerKind::isInputPortConnectionMandatory(int portIndex) const
{
	return false;
}

bool MixerKind::isOutputPortConnectionMandatory(int portIndex) const
{
	return false;
}

bool MixerKind::isFinalElaborationUnit() const
{
	return false;
}

const char MixerKind::OutputPortName[] = "Output";

char* MixerKind::InputPortNames[C_NumInputPorts];

