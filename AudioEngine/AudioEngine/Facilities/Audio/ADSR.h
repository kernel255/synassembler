
#pragma once

class ADSR
{
public:
	int attackTime;
	int decayTime;
	int releaseTime;
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