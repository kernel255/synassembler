
#include <stdio.h>

#include "DirectSoundOutputKind.h"

DirectSoundOutputKind::DirectSoundOutputKind()
{

}

const char* DirectSoundOutputKind::getName(void) const
{
	return "DirectSound Output";
}

bool DirectSoundOutputKind::isPhysical(void) const
{
	return true;
}

int DirectSoundOutputKind::getInputPortNumber(void) const
{
	return 1;
}

int DirectSoundOutputKind::getOutputPortNumber(void) const
{
	return 0;
}

const char* DirectSoundOutputKind::getNthInputPortName(int portIndex) const
{
	if(portIndex==0)
		return DirectSoundOutputKind::MAIN_IN_PORT_NAME;
	else
		return NULL;
}

const char* DirectSoundOutputKind::getNthOutputPortName(int portIndex) const
{
	return NULL;
}

const char* DirectSoundOutputKind::getNthInputPortKind(int portIndex) const
{
	if(portIndex==0)
		return getAudioPortKind();
	else
		return NULL;
}

const char* DirectSoundOutputKind::getNthOutputPortKind(int portIndex) const
{
	return NULL;
}

bool DirectSoundOutputKind::isFinalElaborationUnit() const
{
	return true;
}

bool DirectSoundOutputKind::isInputPortConnectionMandatory(int portIndex) const
{
	if(portIndex==0)
		return true;
	else
		return false;
}

bool DirectSoundOutputKind::isOutputPortConnectionMandatory(int portIndex) const
{
	return false;
}

const int DirectSoundOutputKind::getPropertyNumber() const
{
	return 0;
}

/*
const int DirectSoundOutputKind::getPropertyNumber() const
{
	return 0;
}
const char* DirectSoundOutputKind::getNthPropertyName(int propId) const
{
	return NULL;
}

const int DirectSoundOutputKind::getNthPropertyDirection(int propId) const
{
	return 0;
}
*/
const char DirectSoundOutputKind::MAIN_IN_PORT_NAME[]  = "In";