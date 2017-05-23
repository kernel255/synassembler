#pragma once

class FrequencySmoother
{
private:
	double initialFreq;
	double finalFrequency;
public:
	void start(double initialF, double finalF);
};