
#include "GainProperty.h"

GainProperty::GainProperty(char *name) : AbstractProperty(C_Gain,name, NULL)
{
}

GainProperty::GainProperty(char *name, char* description) : AbstractProperty(C_Gain,name, description)
{
}

int GainProperty::getRange(char* lowRange, char* highRange, unsigned int bufferSize)
{
	const char* low = "0.0";
	const char* high = "1.0";
	if(bufferSize<strlen(low))
		return -1;
	else
	{
		strncpy(lowRange, low, sizeof(low));
		strncpy(highRange, high, sizeof(high));
		return 0;
	}
}

/*
e_PropertyType GainProperty::getPropertyType()
{
	return C_Gain;
}
*/

/*
char* GainProperty::getName()
{
	return m_Name;
}

char* GainProperty::getDescription()
{
	return m_Description!=NULL ? m_Description : m_Name;
}
*/