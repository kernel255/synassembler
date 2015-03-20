
#pragma once

#include <vector>

#include "EUKind.h"
#include "..\Facilities\General\Property.h"

class AbstractEUKind : public EUKind
{
public:
	virtual const int getPropertyNumber() const;
	virtual const char* getNthPropertyName(int propId) const;
	/**
	 * Return the type of the nth property
	 */
	virtual e_PropertyType getPropertyType(int propertyIndex) const;
	/**
	 * Read the ranges of the property
	 */
	virtual int getPropertyRanges(int propertyIndex, char* lowRange, char* highRange, int bufferSize) const;
	/**
	 * Read the direction of the property input, output or both
	 */
	virtual e_PropertyDirection getNthPropertyDirection(int propertyIndex) const;
	/**
	 * Retrieve the property interface
	 */
	virtual Property* getProperty(int propertyIndex) const;
protected:
	AbstractEUKind();
	~AbstractEUKind();
	void addProperty(Property* prop);
private:
	std::vector<Property*> m_Properties;
};