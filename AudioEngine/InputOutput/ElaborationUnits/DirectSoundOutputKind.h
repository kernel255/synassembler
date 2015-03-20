#pragma once

#include "..\gg.h"

#include "AbstractEUKind.h"


class DirectSoundOutputKind : public AbstractEUKind
{
public:
	DirectSoundOutputKind();
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
	virtual const int getPropertyNumber() const;
	//virtual const int getPropertyNumber() const;
	//virtual const char* getNthPropertyName(int propId) const;
	//virtual const int getNthPropertyDirection(int propId) const;
	static const char MAIN_IN_PORT_NAME[];
};