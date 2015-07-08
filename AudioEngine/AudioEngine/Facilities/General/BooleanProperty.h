
#include "AbstractProperty.h"

class BooleanProperty : public AbstractProperty
{
public:
	BooleanProperty(char* name);
	int getRange(char* lowRange, char* highRange, unsigned int bufferSize);
};