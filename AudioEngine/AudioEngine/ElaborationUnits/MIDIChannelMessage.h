
#pragma once

#include <math.h>
#include <assert.h>

/**
 * \class MIDIChannelMessage Data struct to hold a MIDI message
 */

#define twototwelvesqrt pow(2.0,(1.0/12.0))

class MIDIChannelMessage
{
	static const int treeshold = 57;
public:
	static const int NumMIDINotes = 128;
	static const double GetFreqByMIDINote(int note)
	{
		if((note<0)&&(note>127))
			return 0.0;
		if( note >= treeshold )
		{
			return 440.0*pow(twototwelvesqrt,note-treeshold);
		}
		else
		{
			return 440.0/pow(twototwelvesqrt,treeshold-note);
		}
	}
	/**
	* \enum Defines all available kind of MIDI message
	*/ 
	enum MIDIMessage
	{
		e_NoteOn = 0x90,
		e_NoteOff = 0x80,
		e_ProgramChange = 0xC0
	};
	struct MIDINoteMessage
	{
		unsigned char Note;
		unsigned char Velocity;
		double Frequency;
	};
	struct MIDIProgramChangeMessage
	{
		unsigned char Program;
	};
	union MIDIChannelMessageData
	{
		MIDINoteMessage NoteMessage;
		MIDIProgramChangeMessage ProgramMessage;
	};
	MIDIMessage Kind;
	unsigned char channel;
	MIDIChannelMessageData data;
};
