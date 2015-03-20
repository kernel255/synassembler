
#pragma once

#include "VirtualElaborationUnit.h"
#include "ConcretePort.h"
#include "MIDISequencePlayerKind.h"
#include "../Facilities/General/TimeBase.h"

#define MAIN_OUT_PORT_INDEX		0

class MIDISequencePlayer : public VirtualElaborationUnit, BeatHook {
public:
	// VirtualElaborationUnit methods override
	MIDISequencePlayer(ModuleServices* moduleServices);
	virtual ~MIDISequencePlayer();
	virtual void updateAudioSamples(EAG_SAMPLE_TYPE *pSamplesBuffer,int numsamples);
	virtual void receiveMIDIMessage(MIDIChannelMessage& midimsg);
	virtual bool setInputEU(ElaborationUnitPort* pPort, ElaborationUnit* pInputEU, ElaborationUnitPort* pInputPort);
	virtual bool setOutputEU(ElaborationUnitPort* pPort, ElaborationUnit* pOutputEU, ElaborationUnitPort* pOutputPort);
	virtual ElaborationUnitPort* getNthInputPort(int n);
	virtual ElaborationUnitPort* getNthOutputPort(int n);
	virtual int getInputPortNumber(void);
	virtual int getOutputPortNumber(void);
	virtual ElaborationUnitPort* getInputPortByEU(ElaborationUnit* pEU, int& n);
	virtual ElaborationUnitPort* getOutputPortByEU(ElaborationUnit* pEU, int& n);
	virtual void allocate(void);
	virtual void deallocate(void);
	virtual void play(void);
	virtual void pause(void);
	virtual void stop(void);
	virtual const EUKind* getKind(void);
	virtual void setSamplesBufferMaximumSize(int size);
	// BeatHook methods override
	void beat(void);
	// Other methods
	static const EUKind* s_GetKind(void);
	virtual int setProperty(int propertyIndex, const char* value);
	virtual int getProperty(int propertyIndex, char* value, int bufferSize);
private:
	static const int NULL_NOTE;
	static const MIDISequencePlayerKind kinna;
	ConcretePort MainOutPort;
	//Read the next MIDI note accordingly to the file, now dummy version with fixed array
	int beatCounter;
	int currentNoteIndex;
	// Hardcoded MIDI sequence
	static int noteDurationArray[];
	static int noteArray[];
	static bool noteOnOffArray[];

	// Hardcoded MIDI sequence
	int getNextMIDINoteDuration(void);
	int getNextMIDINote(void);
	bool getNextMIDIOnOff(void);

	class MIDINoteInfo {
	public:
		MIDINoteInfo() {
		}
		bool NoteOn;
		int Note;
		int duration;
	};
	static MIDINoteInfo NotesInfoArray[];
};