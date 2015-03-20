
#include "..\..\ElaborationUnits\ModuleServices.h"
#include "SimpleVoice.h"

#define STACK_DEPTH 20

class MultipleVoice
{
	class SimpleVoiceProxy
	{
	public:
		SimpleVoice simpleVoice;
	};
	class SimpleVoiceStack
	{
	public:
		int stackPos = 0;
		SimpleVoiceProxy m_Voices[STACK_DEPTH];
	};
public:
	MultipleVoice(ModuleServices* pServices);
	~MultipleVoice();

	void activate(double initialFrequency);
	void deactivated(void);
private:
	ModuleServices m_pModuleServices;
	SimpleVoiceStack voiceStack;
};