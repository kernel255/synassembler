
#pragma once

#include "EUKind.h"
#include "..\ElaborationUnits\AbstractEUKind.h"

#define PCMWAV_INPUT_PORT_NUMBER			3
#define PCMWAV_OUTPUT_PORT_NUMBER			1

#define MAIN_OUTPUT_INDEX					0
#define PHASE_INPUT_INDEX					0
#define AMPLITUDE_INPUT_INDEX				1
#define MIDI_INPUT_INDEX					2

class PCMWaveformKind : public AbstractEUKind
{
public:
	PCMWaveformKind();
	~PCMWaveformKind();
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
	//virtual const int getNthPropertyDirection(int propId) const;

	static const char PhasePortName[];
	static const char AmplitudePortName[];
	static const char MIDIPortName[];
	static const char MainOutPortName[];

	static const int C_OutputLevelIndex =		0;
	static const int C_WaveFilename		=		1;
	static const int C_WavePitch		=		2;

};