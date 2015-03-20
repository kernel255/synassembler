//#include <stdafx.h>

#include <stdio.h>
#include "MIDIInputKind.h"

const char* MIDIInputKind::getName(void) const
{
	return "MIDI Input";
}

bool MIDIInputKind::isPhysical(void) const
{
	return true;
}

int MIDIInputKind::getInputPortNumber(void) const
{
	return 0;
}

int MIDIInputKind::getOutputPortNumber(void) const
{
	return 1;
}

const char* MIDIInputKind::getNthInputPortName(int portIndex) const
{
	return NULL;
}

const char* MIDIInputKind::getNthOutputPortName(int portIndex) const
{
	if(portIndex==0)
		return MIDIInputKind::OUT_PORT_NAME;
	else
		return NULL;
}

const char* MIDIInputKind::getNthInputPortKind(int portIndex) const
{
	return NULL;
}

const char* MIDIInputKind::getNthOutputPortKind(int portIndex) const
{
	if(portIndex==0)
		return getMIDIPortKind();
	else
		return NULL;
}

bool MIDIInputKind::isFinalElaborationUnit() const
{
	return false;
}

bool MIDIInputKind::isInputPortConnectionMandatory(int portIndex) const
{
	return false;
}

bool MIDIInputKind::isOutputPortConnectionMandatory(int portIndex) const
{
	if(portIndex==MAIN_OUTPUT_INDEX)
		return true;
	else
		return false;
}


/*
const int MIDIInputKind::getPropertyNumber() const
{
	return 0;
}

const char* MIDIInputKind::getNthPropertyName(int propId) const
{
	return NULL;
}

const int MIDIInputKind::getNthPropertyDirection(int propId) const
{
	return 0;
}
*/
const char MIDIInputKind::OUT_PORT_NAME[] = "Out";
