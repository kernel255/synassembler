
#include <stdio.h>

#include "PCKeyboardKind.h"


PCKeyboardKind::PCKeyboardKind()
{
}


PCKeyboardKind::~PCKeyboardKind()
{
}

const char* PCKeyboardKind::getName(void) const
{
	return PCKEYB_NAME;
}

bool PCKeyboardKind::isPhysical(void) const
{
	return true;
}

int PCKeyboardKind::getInputPortNumber(void) const
{
	return 0;
}

int PCKeyboardKind::getOutputPortNumber(void) const
{
	return 1;
}

const char* PCKeyboardKind::getNthInputPortName(int portIndex) const
{
	return NULL;
}
const char* PCKeyboardKind::getNthOutputPortName(int portIndex) const
{
	if (portIndex == 0)
		return PC_OUT_PORT_NAME;
	else
		return NULL;
}
const char* PCKeyboardKind::getNthInputPortKind(int portIndex) const
{
	return NULL;
}

const char* PCKeyboardKind::getNthOutputPortKind(int portIndex) const
{
	if (portIndex == 0)
		return getMIDIPortKind();
	else
		return NULL;
}

bool PCKeyboardKind::isFinalElaborationUnit() const
{
	return false;
}

bool PCKeyboardKind::isInputPortConnectionMandatory(int portIndex) const
{
	return false;
}

bool PCKeyboardKind::isOutputPortConnectionMandatory(int portIndex) const
{
	if (portIndex == MAIN_OUTPUT_INDEX)
		return true;
	else
		return false;
}


const char PCKeyboardKind::PC_OUT_PORT_NAME[] = "MIDI Out";