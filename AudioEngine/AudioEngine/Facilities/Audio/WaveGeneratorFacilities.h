
#include "..\..\DSPTypes.h"
#include "WaveKind.h"
#include "TimeAccumulatedVoice.h"
#include "LFO.h"

class WaveGeneratorFacilities
{
public:
	static EAG_SAMPLE_TYPE getSample(TimeAccumulatedVoice* pVoice, WaveKind kind, double samplingFrequence, LFO* freqLFO);
};

