#include "GainIndexedProperty.h"

GainIndexedProperty::GainIndexedProperty(char* name, int index) : GainProperty(name)
{
	this->index = index;
}