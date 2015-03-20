
#include "EUKind.h"
#include "AbstractElaborationUnit.h"

AbstractElaborationUnit::AbstractElaborationUnit(ModuleServices* pServices, int numProperties, const EUKind* kind) : m_ValuesHolder(pServices, numProperties, kind, this)
{
	m_pModuleServices = pServices;
	stopped = false;
	playing = false;
}

void AbstractElaborationUnit::play(void)
{
	playing = true;
}

int AbstractElaborationUnit::getId() 
{ 
	return iId; 
}

void AbstractElaborationUnit::setId(int id) 
{ 
	iId = id; 
}


AbstractElaborationUnit::~AbstractElaborationUnit()
{
}

void AbstractElaborationUnit::stop()
{
	stopped = true;
	playing = false;
}

bool AbstractElaborationUnit::isPlaying()
{
	return playing;
}

int AbstractElaborationUnit::getProperty(int propertyIndex, char* value, int bufferSize)
{
	//PropertyValuesHolder holder = m_ValuesHolder
	switch(this->getKind()->getPropertyType(propertyIndex))
	{
	case C_Gain:
		{
			this->m_ValuesHolder.getValue(propertyIndex,value,bufferSize);
			break;
		}
	}

	return 0;
}

int AbstractElaborationUnit::setProperty(int propertyIndex, const char* value)
{
	switch(getKind()->getPropertyType(propertyIndex))
	{
	case C_Gain:
		{
			m_ValuesHolder.setValue(propertyIndex, value);
			break;
		}
	}
	return 0;
}

int AbstractElaborationUnit::getDProperty(int propertyIndex, double* value)
{
	switch(getKind()->getPropertyType(propertyIndex))
	{
	case C_Gain:
		{
			m_ValuesHolder.getDValue(propertyIndex, value);
			break;
		}
	default:
		{
		return UNAVAILABLE_PROPERTY;
		}
	}
	return 0;
}

int AbstractElaborationUnit::setDProperty(int propertyIndex, double value)
{
	switch(getKind()->getPropertyType(propertyIndex))
	{
	case C_Gain:
		{
			m_ValuesHolder.setDValue(propertyIndex, value);
			break;
		}
	}

	return 0;
}

int AbstractElaborationUnit::setIProperty(int propertyIndex, int value)
{
	return 0;
}

int AbstractElaborationUnit::getIProperty(int propertyIndex, int* value)
{
	return 0;
}
