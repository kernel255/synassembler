#pragma once

#include "GainProperty.h"

class GainIndexedProperty : public GainProperty 
{
public:
	GainIndexedProperty(char* name, int index);
protected:
	int index;

};