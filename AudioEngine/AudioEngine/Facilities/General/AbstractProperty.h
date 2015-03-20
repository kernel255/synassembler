
#pragma once

#include "Property.h"

class AbstractProperty : public Property
{
public:
	virtual char* getName();
	virtual char* getDescription();
	virtual e_PropertyType getType();
	virtual e_PropertyDirection getDirection();

	virtual void setGetter(pfnGetValue fGet);
	virtual void setSetter(pfnSetValue fSet);

	virtual pfnGetValue getGetter();
	virtual pfnSetValue getSetter();

protected:
	AbstractProperty(e_PropertyType type, char* name);
	AbstractProperty(e_PropertyType type, char* name, char* description);
	char* m_Name;
	char* m_Description;
	e_PropertyType m_Type;
	e_PropertyDirection m_Direction;

	pfnGetValue m_pfnGetter;
	pfnSetValue m_pfnSetter;

};