#include "FrequencySmoother.h"
#include <string.h>


#define DEBUG_BUFFER_SIZE 20000

FrequencySmoother::FrequencySmoother()
{
	active = false;
	dumpSamples = new double[DEBUG_BUFFER_SIZE];
	//memset(&dumpSamples, 0, DEBUG_BUFFER_SIZE*sizeof(double));
	bufPtr = dumpSamples;
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
	if (bufPtr - dumpSamples < DEBUG_BUFFER_SIZE)
	{
		*bufPtr = res;
		bufPtr++;
	}

	acc += step;
	stepNumber--;
	if (stepNumber == 0)
	{
		active = false;
	}
	return res;
}
