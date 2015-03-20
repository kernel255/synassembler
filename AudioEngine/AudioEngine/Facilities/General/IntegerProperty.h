
#include "Property.h"
#include "AbstractProperty.h"

class IntegerProperty : public AbstractProperty
{
public:
	IntegerProperty(char* name);
	IntegerProperty(char* name, int low, int hi);
	int getRange(char* lowRange, char* highRange, unsigned int bufferSize);
};