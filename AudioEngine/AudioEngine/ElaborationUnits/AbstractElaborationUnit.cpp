
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

int AbstractElaborationUnit::getProperty(int propertyIndex, wchar_t* value, int bufferSize)
{
	//PropertyValuesHolder holder = m_ValuesHolder
	switch(this->getKind()->getPropertyType(propertyIndex))
	{
	case C_String:
		{
		const wchar_t* pwString;
			this->m_ValuesHolder.getValue(propertyIndex,&pwString,bufferSize);
			swprintf(value, bufferSize, pwString);
			break;
		}
	}

	return 0;
}

int AbstractElaborationUnit::setProperty(int propertyIndex, const wchar_t* value)
{
	switch(getKind()->getPropertyType(propertyIndex))
	{
	case C_String:
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
			m_pModuleServices->pLogger->writeLine("getIProperty: property not available propertyIndex=%d", propertyIndex);
			return UNAVAILABLE_PROPERTY;
		}
	}
	return 0;
}

int AbstractElaborationUnit::getIProperty(int propertyIndex, int* value)
{
	switch (getKind()->getPropertyType(propertyIndex))
	{
		case C_Integer:
		{
			m_ValuesHolder.getIValue(propertyIndex, value);
			break;
		}
		default:
		{
			m_pModuleServices->pLogger->writeLine("getIProperty: property not available propertyIndex=%d", propertyIndex);
			return UNAVAILABLE_PROPERTY;
		}
	}
	return 0;
}

int AbstractElaborationUnit::getBProperty(int propertyIndex, bool* value)
{
	switch (getKind()->getPropertyType(propertyIndex))
	{
		case C_Boolean:
		{
			m_ValuesHolder.getBValue(propertyIndex, value);
			break;
		}
		default:
		{
			m_pModuleServices->pLogger->writeLine("getBProperty: property not available propertyIndex=%d", propertyIndex);
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
	default:
		{
			m_pModuleServices->pLogger->writeLine("getIProperty: property not available propertyIndex=%d", propertyIndex);
			return UNAVAILABLE_PROPERTY;
		}
	}

	return 0;
}

int AbstractElaborationUnit::setIProperty(int propertyIndex, int value)
{
	switch (getKind()->getPropertyType(propertyIndex))
	{
	case C_Integer:
	{
		m_ValuesHolder.setIValue(propertyIndex, value);
		break;
	}
	default:
		{
			m_pModuleServices->pLogger->writeLine("getIProperty: property not available propertyIndex=%d", propertyIndex);
			return UNAVAILABLE_PROPERTY;
		}
	}
	return 0;
}

int AbstractElaborationUnit::setBProperty(int propertyIndex, bool value)
{
	switch (getKind()->getPropertyType(propertyIndex))
	{
	case C_Boolean:
	{
		m_ValuesHolder.setBValue(propertyIndex, value);
		break;
	}
	default:
	{
		m_pModuleServices->pLogger->writeLine("getIProperty: property not available propertyIndex=%d", propertyIndex);
		return UNAVAILABLE_PROPERTY;
	}
	}
	return 0;
}
