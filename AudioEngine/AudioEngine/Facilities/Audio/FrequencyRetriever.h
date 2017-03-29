#pragma once

/**
 * Class used to retrieve frequency from inside the oscillators/generators
*/

class FrequecyRetriever
{
public:
	virtual double GetCurrentFrequence() = 0;
};