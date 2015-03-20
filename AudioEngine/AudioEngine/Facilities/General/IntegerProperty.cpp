#include "IntegerProperty.h"

IntegerProperty::IntegerProperty(char* name) : AbstractProperty(C_Integer, name, NULL)
{

}

IntegerProperty::IntegerProperty(char* name, int low, int hi) : AbstractProperty(C_Integer, name, NULL)
{

}

int IntegerProperty::getRange(char* lowRange, char* highRange, unsigned int bufferSize)
{
	const char* low = "0.0";
	const char* high = "1.0";
	if (bufferSize<strlen(low))
		return -1;
	else
	{
		strncpy(lowRange, low, sizeof(low));
		strncpy(highRange, high, sizeof(high));
		return 0;
	}

}