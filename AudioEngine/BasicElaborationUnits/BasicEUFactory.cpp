
#include "BasicEUFactory.h"
#include "ElaborationUnits\Oscillator.h"
#include "ElaborationUnits\MIDISequencePlayer.h"
#include "ElaborationUnits\PCMWaveform.h"
#include "ElaborationUnits\Mixer.h"

#define C_OscillatorIndex	0
#define C_MIDISequenceIndex	1
#define C_PCMWaveformIndex	2
#define C_MixerIndex		3

//GraphEUFactory* BasicEUFactory::s_pFactory = NULL;
const int BasicEUFactory::LocalEUNumber = 4; //(Oscillator)+(MIDISequencePlayer)+(PCMWaveform)

BasicEUFactory::BasicEUFactory(ModuleServices* pService) : AbstractElaborationUnitFactory(pService) 
{
	moduleServices = pService;
	moduleServices->pLogger->writeLine("Basic EU factory constructor");
}

BasicEUFactory::~BasicEUFactory()
{
	moduleServices->pLogger->writeLine("Basic EU factory destructor");
	cleanUpElaborationUnits();
}

ElaborationUnit* BasicEUFactory::createVirtualElaborationUnit(unsigned euIndex)
{
	if(euIndex < GetEUKindNumber() )
	{
		switch(euIndex)
		{
		case C_OscillatorIndex:
			{
				ElaborationUnit* peu = new Oscillator(moduleServices);
				addElaborationUnit(peu);
				return peu;
				break;
			}
		case C_MIDISequenceIndex:
			{
				ElaborationUnit* peu = new MIDISequencePlayer(moduleServices);
				addElaborationUnit(peu);
				return peu;
				break;
			}
		case C_PCMWaveformIndex:
			{
				ElaborationUnit* peu = new PCMWaveform(moduleServices);
				addElaborationUnit(peu);
				return peu;
				break;
			}
		case C_MixerIndex:
			{
				ElaborationUnit* peu = new Mixer(moduleServices);
				addElaborationUnit(peu);
				return peu;
				break;
			}
		default:
			{
				return NULL;
				break;
			}
		}
	}
	else
		return NULL;
}

unsigned int BasicEUFactory::GetEUKindNumber(void)
{
	return LocalEUNumber;
}

const int BasicEUFactory::getEUIndex(ElaborationUnit* pEU)
{
	return -1;
}
const char *BasicEUFactory::getName(void)
{
	return "Basic Elements";
}

unsigned int BasicEUFactory::getVirtualEUKindNumber(void)
{
	return 1 + 1 + 1 + 1;
}

const EUKind* BasicEUFactory::getNthVirtualEUKind(int n)
{
	switch(n)
	{
	case C_OscillatorIndex:
		{
			return Oscillator::s_GetKind();
			break;
		}
	case C_MIDISequenceIndex:
		{
			return MIDISequencePlayer::s_GetKind();
			break;
		}
	case C_PCMWaveformIndex:
		{
			return PCMWaveform::s_GetKind();
			break;
		}
	case C_MixerIndex:
		{
			return Mixer::s_GetKind();
			break;
		}
	default:
		{
			return NULL;
			break;
		}
	}
}


unsigned int BasicEUFactory::getPhysicalEUKindNumber(void)
{
	return 0;
}

const EUKind* BasicEUFactory::getNthPhysicalEUKind(int n)
{
	return NULL;
}

unsigned int BasicEUFactory::getPhysicalEUInstanceNumber(int euIndex)
{
	return 0;
}

ElaborationUnit* BasicEUFactory::createPhysicalElaborationUnit(unsigned euIndex, unsigned instanceIndex)
{
	return NULL;
}

const int BasicEUFactory::getPhysicalEUInstanceNumber(void)
{
	return 0;
}

const wchar_t* BasicEUFactory::getNthPhysicalEUInstanceName(int euIndex, int instanceIndex)
{
	return NULL;
}


void BasicEUFactory::destroyElaborationUnit(ElaborationUnit* peu)
{
	removeElaborationUnit(peu);	
}

ElaborationUnit* BasicEUFactory::createNamedPhysicalElaborationUnit(unsigned euIndex, std::wstring name)
{
	return NULL;
}
