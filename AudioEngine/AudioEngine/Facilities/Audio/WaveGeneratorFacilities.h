
#include "..\..\DSPTypes.h"
#include "WaveKind.h"
#include "TimeAccumulatedVoice.h"

class WaveGeneratorFacilities
{
public:
	static EAG_SAMPLE_TYPE getSample(TimeAccumulatedVoice* pVoice, WaveKind kind, double samplingFrequence);
};

