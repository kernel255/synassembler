
#pragma once

#include "../../ElaborationUnits/ModuleServices.h"
#include "../Facilities/Audio/SimpleEnvelope.h"
#include "../Audio/TimeAccumulatedVoice.h"

class SimpleVoice : public TimeAccumulatedVoice
{
public:
	SimpleVoice(int id, double samplingPeriod, int samplesBufferSize, ModuleServices* pModuleServices);
	virtual ~SimpleVoice();
	/**
	 * @fn Starts to play the sound (e.g. MIDI note on)
	 */
	void activate(double initialfreq, ADSR adsr);
	/**
	 * @fn Stop to play a sound (e.g. MIDI note off)
	 */
	void deactivate(void);
	/**
	 * @fn Called when no sound is necessary e.g. envelope completed.
	 */
	void dispose(void);
	/**
	* \var m_bActive True when the Oscillator is oscillating
	*/
	bool m_bActive;
	/**
	* \var m_bFinalRelease True when the midi off has been received but the envelope is still in place
	*/
	bool m_bFinalRelease;
	double m_Frequency;
	double m_TimeAccumulator;
	double m_Period;
	double m_InitialFrequency;
	SimpleEnvelope *m_pEnvelope;
	EAG_SAMPLE_TYPE* m_SamplesBuffer;
	ModuleServices* m_pModuleServices;

	virtual double getTimeAccumulated() { return m_TimeAccumulator; }
	virtual double getPeriod() { return m_Period; }
private:
	int id;
};