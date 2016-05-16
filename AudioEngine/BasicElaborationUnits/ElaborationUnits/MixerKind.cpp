
#include "MixerKind.h"
#include "Mixer.h"
#include "..\Facilities\General\GainProperty.h"
#include "..\Facilities\General\GainIndexedProperty.h"
#include "..\Facilities\General\BooleanProperty.h"

MixerKind::MixerKind()
{
	GainProperty* gain = new GainProperty("Output level");
	gain->setGetter(Mixer::getOutput);
	gain->setSetter(Mixer::setOutput);
	addProperty(gain);

	GainIndexedProperty** props = new GainIndexedProperty*[C_NumInputPorts];

	static const char* PREFIX = "Input ";
	for (int i = 0; i < C_NumInputPorts; i++)
	{
		char* buffer = new char[30];
		memset(buffer, 0, 30);
		sprintf(buffer, "Input %d", i);
		InputPortNames[i] = buffer;
		GainIndexedProperty* igain = new GainIndexedProperty(buffer, i);
		igain->setSetter(Mixer::setOutput);
		props[i] = igain;
	}
	props[0]->setGetter(Mixer::GET_INPUT_NAME(0));
	props[1]->setGetter(Mixer::GET_INPUT_NAME(1));
	props[2]->setGetter(Mixer::GET_INPUT_NAME(2));
	props[3]->setGetter(Mixer::GET_INPUT_NAME(3));
	props[0]->setSetter(Mixer::SET_INPUT_NAME(0));
	props[1]->setSetter(Mixer::SET_INPUT_NAME(1));
	props[2]->setSetter(Mixer::SET_INPUT_NAME(2));
	props[3]->setSetter(Mixer::SET_INPUT_NAME(3));
	for (int i = 0; i < C_NumInputPorts; i++)
	{
		addProperty(props[i]);
	}

	BooleanProperty* bProp = new BooleanProperty("Output Mute");
	bProp->setSetter(Mixer::setOutputMute);
	bProp->setGetter(Mixer::getOutputMute);
	addProperty(bProp);

	BooleanProperty** bProps = new BooleanProperty*[C_NumInputPorts];
	static const char* BPREFIX = "Mute ";
	for (int i = 0; i < C_NumInputPorts; i++)
	{
		char* buffer = new char[30];
		memset(buffer, 0, 30);
		sprintf(buffer, "%s %d", BPREFIX, i);
		//TODO
		BooleanProperty* bMute = new BooleanProperty(buffer);
		bProps[i] = bMute;
	}
	bProps[0]->setGetter(Mixer::GET_INPUTMUTE_NAME(0));
	bProps[1]->setGetter(Mixer::GET_INPUTMUTE_NAME(1));
	bProps[2]->setGetter(Mixer::GET_INPUTMUTE_NAME(2));
	bProps[3]->setGetter(Mixer::GET_INPUTMUTE_NAME(3));
	bProps[0]->setSetter(Mixer::SET_INPUTMUTE_NAME(0));
	bProps[1]->setSetter(Mixer::SET_INPUTMUTE_NAME(1));
	bProps[2]->setSetter(Mixer::SET_INPUTMUTE_NAME(2));
	bProps[3]->setSetter(Mixer::SET_INPUTMUTE_NAME(3));
	for (int i = 0; i < C_NumInputPorts; i++)
	{
		addProperty(bProps[i]);
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

