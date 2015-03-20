
#pragma once

// Property categories
#define INTEGER_PROPERTY			0
#define STRING_PROPERTY				1

/**
 * @class Describes the properties available from extern of the EU
 */
class ElaborationUnitProperty
{
public:
	virtual const char* getPropertyName() = 0;
	virtual void* getPropertyValue(int propertyCategory);
	virtual void setPropertyValue(int propertyCategory, void* value);
};