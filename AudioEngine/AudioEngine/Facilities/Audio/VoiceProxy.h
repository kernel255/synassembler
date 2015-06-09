
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
	bool isDeactivating();
	void allocate();
	void freeVoice();

	VoiceProxy *nextAllocated;

	ModuleServices* m_pModuleServices;

	SimpleVoice simpleVoice;
	int id;
private:

	bool free;
	bool deactivating;
};
