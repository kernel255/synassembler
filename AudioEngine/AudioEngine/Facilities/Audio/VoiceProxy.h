
#include <stdio.h>
#include "../../ElaborationUnits/ModuleServices.h"
#include "SimpleVoice.h"

class VoiceProxy
{
public:
	static int index;

	VoiceProxy(int id,double samplingPeriod, int samplesBufferSize, ModuleServices* pModuleServices);
	void activate(double initialfreq);
	void deactivate(void);
	bool isFree();
	void allocate();

	VoiceProxy *nextAllocated;
	VoiceProxy *nextFree;

	ModuleServices* m_pModuleServices;

	SimpleVoice simpleVoice;
	int id;
private:

	bool free;
};
