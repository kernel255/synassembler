#pragma once

#include "AbstractProperty.h"

class StringProperty : public AbstractProperty
{
public:
	StringProperty(char* name);
	int getRange(char* lowRange, char* highRange, unsigned int bufferSize);
};