
#include "AbstractProperty.h"

AbstractProperty::AbstractProperty(e_PropertyType type, char* name)
{
	m_Type = type;
	m_Name = name;
	m_Description = "";
	m_Direction = C_InputOutput;
}

AbstractProperty::AbstractProperty(e_PropertyType type, char* name,char* description)
{
	m_Type = type;
	m_Name = name;
	m_Description = description;
	m_Direction = C_InputOutput;
}

char* AbstractProperty::getName()
{
	return this->m_Name;
}

char* AbstractProperty::getDescription()
{
	return this->m_Description;
}


e_PropertyType AbstractProperty::getType()
{
	return m_Type;
}

e_PropertyDirection AbstractProperty::getDirection()
{
	return m_Direction;
}

void AbstractProperty::setGetter(pfnGetValue fGet)
{
	this->m_pfnGetter = fGet;
}

void AbstractProperty::setSetter(pfnSetValue fSet)
{
	this->m_pfnSetter = fSet;
}

pfnGetValue AbstractProperty::getGetter()
{
	return this->m_pfnGetter;
}

pfnSetValue AbstractProperty::getSetter()
{
	return this->m_pfnSetter;
}
