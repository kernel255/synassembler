
#pragma once

#include "Property.h"
#include "AbstractProperty.h"

/**
 * @class Represent a property like an oscillator/noise gain the range is 0.0 to 1.0
 */

class GainProperty : public AbstractProperty {
public:
	GainProperty(char* name);
	GainProperty(char* name, char* description);
	//virtual char* getName();
	//virtual char* getDescription();
	virtual int getRange(char* lowRange, char* highRange, unsigned int bufferSize);
	//virtual e_PropertyType getPropertyType();
private:
	//char* m_Name;
	//char* m_Description;
};