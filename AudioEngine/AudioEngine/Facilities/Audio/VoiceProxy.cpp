#include "VoiceProxy.h"

VoiceProxy::VoiceProxy(int id, double samplingPeriod, int samplesBufferSize, ModuleServices* pModuleServices) : simpleVoice(id, samplingPeriod, samplesBufferSize, pModuleServices)
{
	m_pModuleServices = pModuleServices;
	free = true;
	deactivating = false;
	nextAllocated = NULL;
	this->id = id;
};
void VoiceProxy::activate(double initialfreq) {
	free = false;
	simpleVoice.activate(initialfreq);
	m_pModuleServices->pLogger->writeLine("VoiceProxy.activate#%d", id);
}
void VoiceProxy::deactivate(void) {
	deactivating = true;
	simpleVoice.deactivate();
	m_pModuleServices->pLogger->writeLine("VoiceProxy.deactivate#%d", id);
}
bool VoiceProxy::isFree() {
	return free;
}

void VoiceProxy::freeVoice() {
	free = true;
	deactivating = false;
}

void VoiceProxy::allocate() {
	free = false;
	m_pModuleServices->pLogger->writeLine("VoiceProxy.allocate#%d", id);
}

bool VoiceProxy::isDeactivating()
{
	return deactivating;
}