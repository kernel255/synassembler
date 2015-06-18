
#include "ADSR.h"

ADSR::ADSR()
{
	attackTime = 60;
	decayTime = 60;
	releaseTime = 400;
	attackLevel = 1.0;
	sustainLevel = 0.6;
}

/*
#define C_AT_RATIO 100
double ADSR::getNormalizedAT(double at)
{
	return at / C_AT_RATIO;
}

double ADSR::getDenormalizedAT(double at)
{
	return at * C_AT_RATIO;
}

#define C_DT_RATIO 100
double ADSR::getNormalizedDT(double dt)
{
	return dt / C_DT_RATIO;
}

double ADSR::getDenormalizedDT(double dt)
{
	return dt * C_DT_RATIO;
}

#define C_RT_RATIO 600
double ADSR::getNormalizedRT(double rt)
{
	return rt / C_RT_RATIO;
}
double ADSR::getDenormalizedRT(double rt)
{
	return rt * C_RT_RATIO;
}
*/