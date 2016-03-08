
#pragma once

#include "EUKind.h"
#include "..\ElaborationUnits\AbstractEUKind.h"

#define MIXER_INPUT_PORT_NUMBER 4

class MixerKind : public AbstractEUKind
{
public:
	MixerKind();
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

	static const int C_NumInputPorts = 4;
	static const int C_NumOutputPorts = 1;

	static const int C_OutputLevelProperty = 0;

	static const char OutputPortName[];
	static char* InputPortNames[MIXER_INPUT_PORT_NUMBER];
};