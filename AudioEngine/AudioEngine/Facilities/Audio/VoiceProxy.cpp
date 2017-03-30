
#include "VoiceProxy.h"

VoiceProxy::VoiceProxy(int id, double samplingPeriod, int samplesBufferSize, FrequencyRetriever* fRetr, ModuleServices* pModuleServices) : simpleVoice(id, samplingPeriod, samplesBufferSize, fRetr, pModuleServices)
{
	m_pModuleServices = pModuleServices;
	free = true;
	deactivating = false;
	nextAllocated = NULL;
	this->id = id;
};
void VoiceProxy::activate(double initialfreq, int initialMIDINote, ADSR adsr) {
	free = false;
	simpleVoice.activate(initialfreq, initialMIDINote, adsr);
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