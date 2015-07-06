
#pragma once

class TimeAccumulatedVoice
{
public:

	virtual double getTimeAccumulated() = 0;
	virtual double getPeriod() = 0;
	virtual void increaseAccumulatedTime(double time) = 0;
};