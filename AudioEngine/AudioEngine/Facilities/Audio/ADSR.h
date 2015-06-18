
#pragma once

class ADSR
{
public:
	double attackTime;
	double decayTime;
	double releaseTime;
	double attackLevel;
	double sustainLevel;
	ADSR();
	/*
	double getNormalizedAT(double at);
	double getDenormalizedAT(double at);
	double getNormalizedDT(double dt);
	double getDenormalizedDT(double dt);
	double getNormalizedRT(double rt);
	double getDenormalizedRT(double rt);
	*/
};