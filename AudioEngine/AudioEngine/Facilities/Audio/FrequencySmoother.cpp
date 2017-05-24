#include "FrequencySmoother.h"

FrequencySmoother::FrequencySmoother()
{
	active = false;
}

void FrequencySmoother::start(double initialF, double finalF)
{
	initialFreq = initialF;
	finalFrequency = finalF;
	step = (finalF - initialF) / C_SMOOTH_STEPS;
	active = true;
	stepNumber = C_SMOOTH_STEPS;
	acc = 0.0;
}

bool FrequencySmoother::isActive()
{
	return active;
}

double FrequencySmoother::getCurrentPeriod()
{
	double res = initialFreq + acc;
	acc += step;
	stepNumber--;
	if (stepNumber == 0)
	{
		active = false;
	}
	return res;
}
