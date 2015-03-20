#include "VoiceProxy.h"

VoiceProxy::VoiceProxy(int id, double samplingPeriod, int samplesBufferSize, ModuleServices* pModuleServices) : simpleVoice(id, samplingPeriod, samplesBufferSize, pModuleServices)
{
	m_pModuleServices = pModuleServices;
	free = true;
	nextAllocated = NULL;
	nextFree = NULL;
	this->id = id;
};
void VoiceProxy::activate(double initialfreq) {
	free = false;
	simpleVoice.activate(initialfreq);
	m_pModuleServices->pLogger->writeLine("VoiceProxy.activate#%d", id);
}
void VoiceProxy::deactivate(void) {
	simpleVoice.deactivate();
	m_pModuleServices->pLogger->writeLine("VoiceProxy.deactivate#%d", id);
}
bool VoiceProxy::isFree() {
	return free;
}

void VoiceProxy::allocate() {
	free = false;
	m_pModuleServices->pLogger->writeLine("VoiceProxy.allocate#%d", id);
}
