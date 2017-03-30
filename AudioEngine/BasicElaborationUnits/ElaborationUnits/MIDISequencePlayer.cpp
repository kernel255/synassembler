
#include "MIDISequencePlayer.h"

#include <sstream>

#define NUM_NOTES		8

MIDISequencePlayer::MIDISequencePlayer(ModuleServices* pService) : VirtualElaborationUnit(pService,MIDISequencePlayer::kinna.getPropertyNumber(),&MIDISequencePlayer::kinna) ,MainOutPort(ElaborationUnitPort::OUTPUT_PORT,ElaborationUnitPort::MIDI_PORT,ElaborationUnitPort::MULTI_PORT)
{
	beatCounter = 0;
	currentNoteIndex = 0;
	m_pModuleServices->timeBase->registerBeatObject(this);

	for(int i=0;i<NUM_NOTES;i++)
	{
		NotesInfoArray[i].NoteOn = noteOnOffArray[i];
		NotesInfoArray[i].Note = noteArray[i];
		NotesInfoArray[i].duration = noteDurationArray[i];
	}

}

MIDISequencePlayer::~MIDISequencePlayer()
{
	this->m_pModuleServices->pLogger->writeLine("MIDISequencer destructor");
}

void MIDISequencePlayer::updateAudioSamples(EAG_SAMPLE_TYPE *pSamplesBuffer,int numsamples)
{
}

void MIDISequencePlayer::receiveMIDIMessage(MIDIChannelMessage& midimsg)
{
}

bool MIDISequencePlayer::setInputEU(ElaborationUnitPort* pPort, ElaborationUnit* pInputEU, ElaborationUnitPort* pInputPort)
{
	return false;
}

bool MIDISequencePlayer::setOutputEU(ElaborationUnitPort* pPort, ElaborationUnit* pOutputEU, ElaborationUnitPort* pOutputPort)
{
	if( pPort == &MainOutPort )
	{
		//MainOutPort.setNthEUandPort(pOutputEU,pOutputPort,MAIN_OUT_PORT_INDEX);
		MainOutPort.setNthEUandPort(pOutputEU,pOutputPort,-1);
		return true;
	}
	else
		return false;
}

ElaborationUnitPort* MIDISequencePlayer::getNthInputPort(int n)
{
	return NULL;
}


ElaborationUnitPort* MIDISequencePlayer::getNthOutputPort(int n)
{
	switch(n)
	{
	case MAIN_OUT_PORT_INDEX:
		return &MainOutPort;
	default:
		return NULL;
	}
}

int MIDISequencePlayer::getInputPortNumber(void)
{
	return 0;
}

int MIDISequencePlayer::getOutputPortNumber(void)
{
	return 1;
}

ElaborationUnitPort* MIDISequencePlayer::getInputPortByEU(ElaborationUnit* pEU, int& n)
{
	return NULL;
}

ElaborationUnitPort* MIDISequencePlayer::getOutputPortByEU(ElaborationUnit* pEU, int& n)
{
	return NULL;
}

void MIDISequencePlayer::allocate(void) 
{
}

void MIDISequencePlayer::deallocate(void)
{
}

void MIDISequencePlayer::play(void)
{
	m_pModuleServices->pLogger->writeLine("Started MIDISequencer");
	beatCounter = 0;
	currentNoteIndex = 0;
	AbstractElaborationUnit::play();
}

void MIDISequencePlayer::pause(void)
{
}

void MIDISequencePlayer::stop(void)
{
}

const EUKind* MIDISequencePlayer::getKind(void)
{
	return s_GetKind();
}

void MIDISequencePlayer::setSamplesBufferMaximumSize(int size)
{
}

const int MIDISequencePlayer::NULL_NOTE = -1;

int MIDISequencePlayer::noteArray[] = {				50,		50,		54,		54,		57,		57,		62,		62 };
int MIDISequencePlayer::noteDurationArray[] =  {	10000,	10,		100,	10,		100,	10,		100,	10 };
bool MIDISequencePlayer::noteOnOffArray[] = {		true,	false,	true,	false,	true,	false,	true,	false };

class MIDISequencePlayer::MIDINoteInfo MIDISequencePlayer::NotesInfoArray[NUM_NOTES];

int MIDISequencePlayer::getNextMIDINoteDuration(void)
{
	//Loops continously
	if(currentNoteIndex==NUM_NOTES)
		currentNoteIndex = 0;
	//return noteDurationArray[currentNoteIndex];
	return NotesInfoArray[currentNoteIndex].duration;
}

int MIDISequencePlayer::getNextMIDINote()
{
	//return noteArray[currentNoteIndex];
	return NotesInfoArray[currentNoteIndex].Note;
}

bool MIDISequencePlayer::getNextMIDIOnOff(void)
{
	//return noteOnOffArray[currentNoteIndex];
	return NotesInfoArray[currentNoteIndex].NoteOn;
}

void MIDISequencePlayer::beat(void)
{
	if(isPlaying())
	{
		if(beatCounter==0)
		{
			int duration = getNextMIDINoteDuration();
			beatCounter = duration;
			int numConn = MainOutPort.getConnectionsNumber();
			if(numConn!=0)
			{
				for(int i=0;i<numConn;i++)
				{
					ElaborationUnit* pEU = MainOutPort.getNthEU(i);
					//Send MIDI note to connected EUs
					MIDIChannelMessage midiMsg;
					midiMsg.channel = 0;
					midiMsg.data.NoteMessage.Note = getNextMIDINote();
					midiMsg.data.NoteMessage.Velocity = 127;
					midiMsg.data.NoteMessage.Frequency = MIDIChannelMessage::GetFreqByMIDINote(midiMsg.data.NoteMessage.Note);
					if(getNextMIDIOnOff())
					{
						midiMsg.Kind = MIDIChannelMessage::e_NoteOn;
						m_pModuleServices->pLogger->writeLine("Playing note On");

						std::stringstream sstr;

						sstr << "Frequency=";
						sstr << midiMsg.data.NoteMessage.Frequency;
						sstr << " Note=";
						sstr << (int) midiMsg.data.NoteMessage.Note;

						//printf("Note=%d\n", midiMsg.data.NoteMessage.Note);

						m_pModuleServices->pLogger->writeLine(sstr.str().c_str());
					}
					else
					{
						midiMsg.Kind = MIDIChannelMessage::e_NoteOff;
						m_pModuleServices->pLogger->writeLine("Playing note Off");
					}
					pEU->receiveMIDIMessage(midiMsg);
					
					currentNoteIndex++;
				}
			}
		}
		else
			beatCounter--;
	}
}

const MIDISequencePlayerKind MIDISequencePlayer::kinna;

const EUKind* MIDISequencePlayer::s_GetKind(void)
{
	return (EUKind *) &kinna;
}

int MIDISequencePlayer::setProperty(int propertyIndex, const char* value)
{
	return 0;
}

int MIDISequencePlayer::getProperty(int propertyIndex, char* value, int bufferSize)
{
	return NULL;
}
