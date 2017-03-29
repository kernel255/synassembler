
#pragma once

#include <math.h>
#include <assert.h>

/**
 * \class MIDIChannelMessage Data struct to hold a MIDI message
 */

#define twototwelvesqrt pow(2.0,(1.0/12.0))
#define A3Freq 440.0
#define TREESHOLD 57

class MIDIChannelMessage
{
private:
	static bool IsNoteOutOfRange(int note)
	{
		return (note < 0) || (note > 127);
	}
	static int GetNormalizedToA3(int note)
	{
		return (note >= TREESHOLD) ? (note - TREESHOLD) : (TREESHOLD - note);
	}
public:
	static const int NumMIDINotes = 128;
	static const double GetFreqByMIDINote(int note)
	{
		if(IsNoteOutOfRange(note))
			return 0.0;
		int iA3Norm = GetNormalizedToA3(note);
		if( note >= TREESHOLD)
		{
			return 440.0*pow(twototwelvesqrt,iA3Norm);
		}
		else
		{
			return 440.0/pow(twototwelvesqrt, iA3Norm);
		}
	}
	static const double GetFreqByMIDINote(int note, int transpose, int tunecent)
	{
		if (IsNoteOutOfRange(note))
			return 0.0;
		double dNote = (double)note;
		double dTranspose = (double)transpose;
		double dCent = (double)tunecent;
		double dTreeshold = (double)TREESHOLD;
		double delta = (dNote + dTranspose + dCent / 100.0) - dTreeshold;

		double result = A3Freq * pow(twototwelvesqrt, delta);
		
		return result;
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
