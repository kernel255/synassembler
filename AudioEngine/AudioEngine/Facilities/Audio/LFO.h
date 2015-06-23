
#pragma once

#include "TimeAccumulatedVoice.h"

class LFO : public TimeAccumulatedVoice
{
public:
	virtual double getTimeAccumulated();
	virtual double getPeriod();
private:
	double m_TimeAccumulated;
	double m_Period = 1.0 / 20.0;
};