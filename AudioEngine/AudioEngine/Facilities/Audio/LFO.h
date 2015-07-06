
#pragma once

#include "TimeAccumulatedVoice.h"
#include "..\..\DSPTypes.h"
#include "WaveKind.h"

class LFO : public TimeAccumulatedVoice
{
public:

	void setFrequency(double freq);
	double getFrequency(void);

	void setAmplitude(double amplitude);
	double getAmplitude();

	double getTimeAccumulated();
	double getPeriod();

	void setEnable(bool enable) { m_Enable = enable; }
	bool getEnable() { return m_Enable; }

	void setDelay(double delay) { m_Delay = delay;  }
	double getDelay() { return m_Delay; }

	EAG_SAMPLE_TYPE getSample(double samplingFrequency);

	void increaseAccumulatedTime(double time);

private:
	double m_TimeAccumulated;
	double m_Period = 1.0 / 20.0;
	double m_Amplitude;
	/**
	* Delay before starting to oscillate
	*/
	double m_Delay = 0.0;
	WaveKind m_WaveKind = WaveKind::e_Sine;
	bool m_Enable = false;
};