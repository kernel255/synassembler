
#include <assert.h>
#include "ADSR.h"
#include "SimpleVoice.h"
#include "MIDIChannelMessage.h"

#define C_FIRST_PERIOD -1

SimpleVoice::SimpleVoice(int id, double samplingPeriod, int samplesBufferSize, FrequencyRetriever* fRetr, ModuleServices* pModuleServices) : m_bActive(false)
{
	m_pEnvelope = new SimpleEnvelope(pModuleServices, samplingPeriod);
	m_SamplesBuffer = new EAG_SAMPLE_TYPE[samplesBufferSize];
	m_pModuleServices = pModuleServices;
	this->id = id;
	m_FreqRetriever = fRetr;
	lastPeriod = C_FIRST_PERIOD;
	
}

SimpleVoice::~SimpleVoice()
{
	delete m_pEnvelope;
	delete m_SamplesBuffer;

}

void SimpleVoice::activate(double initialfreq, int initialMIDINote, ADSR adsr)
{
	if (m_bActive)
		m_pModuleServices->pLogger->writeLine("ERROR: voice #%d already active");
	m_pArrayLogger = m_pModuleServices->getArrayLogger("SimpleVoice");
	m_pModuleServices->pLogger->writeLine("Started Voice #%d", id);
	//m_InitialFrequency = initialfreq;
	m_InitialMIDINote = initialMIDINote;
	m_InitialFrequency = m_FreqRetriever->GetCurrentFrequence(initialMIDINote);
	m_TimeAccumulator = 0.0;
	//m_Period = 1.0 / m_InitialFrequency;
	m_pEnvelope->start(adsr);
	//Activated by last, so the note can begin with the correct parameters
	m_bActive = true;
	m_bFinalRelease = false;
	lastPeriod = C_FIRST_PERIOD;
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

void SimpleVoice::increaseAccumulatedTime(double time)
{
	m_TimeAccumulator += time;
}

double SimpleVoice::getPeriod()
{
	//m_pModuleServices->pLogger->writeLine("getPeriod");
	double period = m_FreqRetriever->GetCurrentFrequence(m_InitialMIDINote);
#define FREQ_SMOOTHER
#ifdef  FREQ_SMOOTHER

	if (period != lastPeriod && lastPeriod!=C_FIRST_PERIOD)
	{
		freqSmoother.start(lastPeriod, period);
		m_pModuleServices->pLogger->writeLine("StartedSmoother");
	}
	lastPeriod = period;
	if (freqSmoother.isActive())
	{
		period = freqSmoother.getCurrentPeriod();
	}
	
#endif //  FREQ_SMOOTHER
	m_pArrayLogger->addValue(period);
	return 1.0 / period;
}