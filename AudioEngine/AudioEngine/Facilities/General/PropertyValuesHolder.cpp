
#include "PropertyValuesHolder.h"
#include "..\..\ElaborationUnits\EUKind.h"
#include "..\..\ElaborationUnits\ElaborationUnit.h"



PropertyValuesHolder::PropertyValuesHolder(ModuleServices* pServices, int numProperties, const EUKind* kind, ElaborationUnit* eu) : m_pKind(kind)
{
	m_Values.resize(numProperties);
	m_Dvalues.resize(numProperties);
	m_IValues.resize(numProperties);
	m_BValues.resize(numProperties);
	//m_pKind = *kind;
	m_pEU = eu;
	m_pServices = pServices;
}

int PropertyValuesHolder::setValue(int propertyIndex, const wchar_t* value)
{
	//TODO: add some semaphore here!
	//m_Values[propertyIndex].wstrValue.assign(value);

	Property* prop = m_pKind->getProperty(propertyIndex);
	pfnSetValue pfn = m_pKind->getProperty(propertyIndex)->getSetter();
	switch(prop->getType())
	{
	case C_String:
		m_Values[propertyIndex].wstrValue.assign(value);
		//m_pServices->pLogger->writeLine("Written StringValue propIndex=%d value=%s", propertyIndex, value);
		break;
	default:
		return -2;
			break;
	}

	return 0;
}

int PropertyValuesHolder::setDValue(int propertyIndex, double value)
{
	m_Dvalues[propertyIndex].dValue = value;

	Property* prop = m_pKind->getProperty(propertyIndex);
	pfnSetValue pfn = m_pKind->getProperty(propertyIndex)->getSetter();
	switch(prop->getType())
	{
	case C_Gain:
		static double dVal;
		pfn(m_pEU,&(value));
		m_pServices->pLogger->writeLine("Setted value");
			break;
	default:
		return -2;
			break;
	}


	return 0;
}

int PropertyValuesHolder::getDValue(int propertyIndex, double* value)
{
	if (propertyIndex >= m_Dvalues.size())
		return UNAVALIABLE_VALUE;
	Property* prop = m_pKind->getProperty(propertyIndex);
	pfnGetValue pfn = m_pKind->getProperty(propertyIndex)->getGetter();
	switch (prop->getType())
	{
	case C_Gain:
	{
		double* pDVal = (double*)pfn(m_pEU);
		*value = *pDVal;
		m_Dvalues[propertyIndex].dValue = *pDVal;
		break;
	}
	default:
		return UNAVALIABLE_VALUE;
		break;
	}

	return 0;
}

int PropertyValuesHolder::getValue(int propertyIndex, const wchar_t** value, int bufferSize)
{
	Property* prop = m_pKind->getProperty(propertyIndex);
	switch(prop->getType())
	{
	case C_String:
		{
			unsigned int len = m_Values[propertyIndex].wstrValue.length();
			if (len > ((unsigned int)bufferSize))
				return -1;
			*value = m_Values[propertyIndex].wstrValue.c_str();
		}
		break;
	default:
		break;
	}

	return 0;
}

int PropertyValuesHolder::setIValue(int propertyIndex, int value)
{
	m_IValues[propertyIndex].iValue = value;
	Property* prop = m_pKind->getProperty(propertyIndex);
	pfnSetValue pfn = m_pKind->getProperty(propertyIndex)->getSetter();
	pfn(m_pEU, &(value));
	m_pServices->pLogger->writeLine("Written IValue propIndex=%d value=%d", propertyIndex, value);

	return 0;
}

int PropertyValuesHolder::getIValue(int propertyIndex, int* value)
{
	if (propertyIndex >= m_IValues.size())
		return UNAVALIABLE_VALUE;
	Property* prop = m_pKind->getProperty(propertyIndex);
	pfnGetValue pfn = prop->getGetter();
	switch (prop->getType())
	{
	case C_Integer:
	{
		int* pIVal = (int*)pfn(m_pEU);
		*value = *pIVal;
		m_IValues[propertyIndex].iValue = *pIVal;
		break;
	}
	default:
		return UNAVALIABLE_VALUE;
		break;
	}
		return 0;
}

int PropertyValuesHolder::setBValue(int propertyIndex, bool value)
{
	m_BValues[propertyIndex].bValue = value;
	Property* prop = m_pKind->getProperty(propertyIndex);
	pfnSetValue pfn = m_pKind->getProperty(propertyIndex)->getSetter();
	pfn(m_pEU, &(value));
	m_pServices->pLogger->writeLine("Written BValue propIndex=%d value=%d", propertyIndex, value);

	return 0;
}

int PropertyValuesHolder::getBValue(int propertyIndex, bool* value)
{
	if (propertyIndex >= m_BValues.size())
		return UNAVALIABLE_VALUE;
	Property* prop = m_pKind->getProperty(propertyIndex);
	pfnGetValue pfn = prop->getGetter();
	switch (prop->getType())
	{
	case C_Boolean:
	{
		bool* pBVal = (bool*)pfn(m_pEU);
		*value = *pBVal;
		m_BValues[propertyIndex].bValue = *pBVal;
		break;
	}
	default:
		return UNAVALIABLE_VALUE;
		break;
	}
	return 0;
}