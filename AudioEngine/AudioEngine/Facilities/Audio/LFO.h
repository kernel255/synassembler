
#pragma once

#include "TimeAccumulatedVoice.h"
#include "..\..\DSPTypes.h"
#include "WaveKind.h"
#include "..\..\ElaborationUnits\ModuleServices.h"

class LFO : public TimeAccumulatedVoice
{
public:
	LFO(ModuleServices* pService);
	double getTimeAccumulated();
	double getPeriod();

	EAG_SAMPLE_TYPE getSample(double samplingFrequency);

	void increaseAccumulatedTime(double time);

	double m_Amplitude;
	/**
	* Delay before starting to oscillate
	*/
	double m_Delay = 0.0;
	WaveKind m_WaveKind = WaveKind::e_Sine;
	bool m_Enable = false;
	double m_Frequency = 100.0;
	bool m_FixRatio = true;
	double m_CarrFreq;
	void setCarrierFrequence(double freq);
	ModuleServices* m_pModuleServices;
private:
	double m_TimeAccumulated;
};