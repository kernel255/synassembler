
#include "PropertyValuesHolder.h"
#include "..\..\ElaborationUnits\EUKind.h"
#include "..\..\ElaborationUnits\ElaborationUnit.h"



PropertyValuesHolder::PropertyValuesHolder(ModuleServices* pServices, int numProperties, const EUKind* kind, ElaborationUnit* eu) : m_pKind(kind)
{
	m_Values.resize(numProperties);
	m_Dvalues.resize(numProperties);
	m_IValues.resize(numProperties);
	//m_pKind = *kind;
	m_pEU = eu;
	m_pServices = pServices;
}

int PropertyValuesHolder::setValue(int propertyIndex, const char* value)
{
	//TODO: add some semaphore here!
	m_Values[propertyIndex].strValue.assign(value);

	Property* prop = m_pKind->getProperty(propertyIndex);
	pfnSetValue pfn = m_pKind->getProperty(propertyIndex)->getSetter();
	switch(prop->getType())
	{
	case C_Gain:
		static float fVal;
		static double dVal;
		if(sscanf(value,"%f",&fVal)!=1)
		{
			m_pServices->pLogger->writeLine("setValue: Unable to convert: %s", value);
			return -1;
		}
		dVal = fVal;
		pfn(m_pEU,&(dVal));
		m_pServices->pLogger->writeLine("Setted value");
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

int PropertyValuesHolder::getValue(int propertyIndex, char* value, int bufferSize)
{
	Property* prop = m_pKind->getProperty(propertyIndex);
	switch(prop->getType())
	{
	case C_Gain:
		static double *pdVal;
		pdVal = (double*) prop->getGetter()(m_pEU);
		if(sprintf(value,"%f",*pdVal)==1)
		{
			return 1;
		}
		break;
	case C_Integer:
		static int *piVal;
		piVal = (int*)prop->getGetter()(m_pEU);
		if (sprintf(value, "%d", *piVal) == 1)
		{
			return 1;
		}
		break;
	default:
		break;
	}
	if(m_Values[propertyIndex].strValue.size()<=(unsigned int) bufferSize)
	{
		strncpy(value,m_Values[propertyIndex].strValue.c_str(),m_Values[propertyIndex].strValue.size());
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
		return 0;
}
