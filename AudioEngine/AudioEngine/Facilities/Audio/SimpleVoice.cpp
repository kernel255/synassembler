
#include <assert.h>

#include "SimpleVoice.h"

SimpleVoice::SimpleVoice(int id, double samplingPeriod, int samplesBufferSize, ModuleServices* pModuleServices) : m_bActive(false)
{
	m_pEnvelope = new SimpleEnvelope(pModuleServices, samplingPeriod);
	m_SamplesBuffer = new EAG_SAMPLE_TYPE[samplesBufferSize];
	m_pModuleServices = pModuleServices;
	this->id = id;
}

SimpleVoice::~SimpleVoice()
{
	delete m_pEnvelope;
	delete m_SamplesBuffer;
}

void SimpleVoice::activate(double initialfreq)
{
	if (m_bActive)
		m_pModuleServices->pLogger->writeLine("ERROR: voice #%d already active");
	m_pModuleServices->pLogger->writeLine("Started Voice #%d", id);
	m_InitialFrequency = initialfreq;
	m_TimeAccumulator = 0.0;
	m_Period = 1.0 / m_InitialFrequency;
	m_pEnvelope->start();
	//Activated by last, so the note can begin with the correct parameters
	m_bActive = true;
	m_bFinalRelease = false;
}
void SimpleVoice::deactivate(void)
{
	if (!m_bActive)
		m_pModuleServices->pLogger->writeLine("ERROR: voice #%d not active");
	m_pEnvelope->stop();
	//m_bActive = false;
	m_bFinalRelease = true;
	m_pModuleServices->pLogger->writeLine("Ended Voice #%d", id);
}
void SimpleVoice::dispose(void)
{
	if (!m_bActive)
		m_pModuleServices->pLogger->writeLine("ERROR: voice #%d not active");
	m_pModuleServices->pLogger->writeLine("Disposed Voice %d", id);
	m_bFinalRelease = false;
	m_bActive = false;
}
