
#pragma once

#include "EUKind.h"
#include "..\ElaborationUnits\AbstractEUKind.h"

#define OSC_INPUT_PORT_NUMBER			3
#define OSC_OUTPUT_PORT_NUMBER			1

#define MAIN_OUTPUT_INDEX					0
#define PHASE_INPUT_INDEX					0
#define AMPLITUDE_INPUT_INDEX				1
#define MIDI_INPUT_INDEX					2


class OscillatorKind : public AbstractEUKind
{
public:
	OscillatorKind();
	const char* getName(void) const;
	virtual bool isPhysical(void) const;
	virtual bool isFinalElaborationUnit() const;
	virtual int getInputPortNumber(void) const;
	virtual int getOutputPortNumber(void) const;
	virtual const char* getNthInputPortName(int portIndex) const;
	virtual const char* getNthOutputPortName(int portIndex) const;
	virtual const char* getNthInputPortKind(int portIndex) const;
	virtual const char* getNthOutputPortKind(int portIndex) const;
	virtual bool isInputPortConnectionMandatory(int portIndex) const;
	virtual bool isOutputPortConnectionMandatory(int portIndex) const;

	//virtual const int getPropertyNumber() const;
	//virtual const char* getNthPropertyName(int propId) const;
	//virtual e_PropertyDirection getNthPropertyDirection(int propId) const;
	//virtual e_PropertyType getPropertyType(int propertyIndex) const;
	//virtual int getPropertyRanges(int propertyIndex, char* lowRange, char* highRange, int bufferSize) const;



	static const int C_OutputLevelProperty = 0;

	static const char PhasePortName[];
	static const char AmplitudePortName[];
	static const char MIDIPortName[];
	static const char MainOutPortName[];

};