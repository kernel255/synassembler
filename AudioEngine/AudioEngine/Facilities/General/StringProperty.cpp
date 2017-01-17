
#include "StringProperty.h"

StringProperty::StringProperty(char* name) : AbstractProperty(C_String, name)
{

}

int StringProperty::getRange(char * lowRange, char * highRange, unsigned int bufferSize)
{
	return 0;
}
