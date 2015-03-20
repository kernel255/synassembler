
#include <stdio.h>

#include "PolyKeyboardKind.h"

const char* PolyKeyboardKind::getName(void) const
{
	return "Polytrack Keyboard";
}

bool PolyKeyboardKind::isPhysical(void) const
{
	return true;
}

int PolyKeyboardKind::getInputPortNumber(void) const
{
	return 0;
}

int PolyKeyboardKind::getOutputPortNumber(void) const
{
	return 1;
}

const char* PolyKeyboardKind::getNthInputPortName(int portIndex) const
{
	return NULL;
}
const char* PolyKeyboardKind::getNthOutputPortName(int portIndex) const
{
	if(portIndex==0)
		return SERIAL_OUT_PORT_NAME;
	else
		return NULL;
}
const char* PolyKeyboardKind::getNthInputPortKind(int portIndex) const
{
	return NULL;
}

const char* PolyKeyboardKind::getNthOutputPortKind(int portIndex) const
{
	if(portIndex==0)
		return getMIDIPortKind();
	else
		return NULL;
}

bool PolyKeyboardKind::isFinalElaborationUnit() const
{
	return false;
}

bool PolyKeyboardKind::isInputPortConnectionMandatory(int portIndex) const
{
	return false;
}

bool PolyKeyboardKind::isOutputPortConnectionMandatory(int portIndex) const
{
	if(portIndex==MAIN_OUTPUT_INDEX)
		return true;
	else
		return false;
}


/*
const int PolyKeyboardKind::getPropertyNumber() const
{
	return 0;
}
const char* PolyKeyboardKind::getNthPropertyName(int propId) const
{
	return NULL;
}
const int PolyKeyboardKind::getNthPropertyDirection(int propId) const
{
	return -1;
}
*/

const char PolyKeyboardKind::SERIAL_OUT_PORT_NAME[] = "MIDI Out";