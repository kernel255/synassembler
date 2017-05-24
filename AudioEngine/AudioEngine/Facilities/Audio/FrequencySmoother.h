#pragma once

#define C_SMOOTH_STEPS	30;

class FrequencySmoother
{
private:
	double initialFreq;
	double finalFrequency;
	double step;
	bool active;
	int stepNumber;
	double acc;
public:
	FrequencySmoother();
	void start(double initialF, double finalF);
	bool isActive();
	double getCurrentPeriod();
};