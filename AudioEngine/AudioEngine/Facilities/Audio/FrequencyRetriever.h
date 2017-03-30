#pragma once

/**
 * Class used to retrieve frequency from inside the oscillators/generators
*/

class FrequencyRetriever
{
public:
	virtual double GetCurrentFrequence(int midiNote) = 0;
};