
#include "MIDISequencePlayerKind.h"
#include "MIDISequencePlayer.h"
#include "..\Facilities\General\StringProperty.h"

MIDISequencePlayerKind::MIDISequencePlayerKind()
{
	// #0 MIDI Filename
	StringProperty* fName = new StringProperty("MIDI Filename");
	fName->setGetter(MIDISequencePlayer::GetMIDIFilename);
	fName->setSetter(MIDISequencePlayer::SetMIDIFilename);
	addProperty(fName);
}

MIDISequencePlayerKind::~MIDISequencePlayerKind()
{
}

const char* MIDISequencePlayerKind::getName(void) const
{
	return "MIDI Sequence Player";
}

bool MIDISequencePlayerKind::isPhysical(void) const
{
	return false;
}

int MIDISequencePlayerKind::getInputPortNumber(void) const
{
	return 0;
}

int MIDISequencePlayerKind::getOutputPortNumber(void) const
{
	return 1;
}

const char* MIDISequencePlayerKind::getNthInputPortName(int portIndex) const
{
	return "";
}

const char* MIDISequencePlayerKind::getNthOutputPortName(int portIndex) const
{
	return MIDISequencePlayerKind::MainOutPortName;
}

const char* MIDISequencePlayerKind::getNthInputPortKind(int portIndex) const
{
	return NULL;
}

const char* MIDISequencePlayerKind::getNthOutputPortKind(int portIndex) const
{
	if(portIndex==0)
		return getMIDIPortKind();
	else
		return NULL;
}

bool MIDISequencePlayerKind::isInputPortConnectionMandatory(int portIndex) const
{
	return false;
}

bool MIDISequencePlayerKind::isOutputPortConnectionMandatory(int portIndex) const
{
	if(portIndex==MIDIINPUT_PORT_INDEX)
		return true;
	else
		return false;
}

bool MIDISequencePlayerKind::isFinalElaborationUnit() const
{
	return false;
}

/*
const int MIDISequencePlayerKind::getPropertyNumber() const
{
	return 0;
}
const char* MIDISequencePlayerKind::getNthPropertyName(int propId) const
{
	return "";
}
const int MIDISequencePlayerKind::getNthPropertyDirection(int propId) const
{
	return 0;
}
*/
const char MIDISequencePlayerKind::MainOutPortName[] = "MIDI Out";