
#include "AbstractEUKind.h"

using namespace std;

AbstractEUKind::AbstractEUKind()
{
}

AbstractEUKind::~AbstractEUKind()
{
	int numProps = m_Properties.size();
	for(int i=0;i<numProps;i++)
	{
		Property* prop = m_Properties[i];
		delete prop;
	}
}

const int AbstractEUKind::getPropertyNumber() const
{
	return m_Properties.size();
}

const char* AbstractEUKind::getNthPropertyName(int propId) const
{
	if(propId>m_Properties.size())
		return NULL;
	Property* prop = m_Properties[propId];
	return prop->getName();
}

void AbstractEUKind::addProperty(Property* prop)
{
	m_Properties.push_back(prop);
}

e_PropertyType AbstractEUKind::getPropertyType(int propertyIndex) const
{
	if (propertyIndex >= m_Properties.size())
		return C_None;
	else
		return m_Properties[propertyIndex]->getType();
}

e_PropertyDirection AbstractEUKind::getNthPropertyDirection(int propertyIndex) const
{
	return m_Properties[propertyIndex]->getDirection();
}

int AbstractEUKind::getPropertyRanges(int propertyIndex, char* lowRange, char* highRange, int bufferSize) const
{
	return m_Properties[propertyIndex]->getRange(lowRange,highRange,bufferSize);
}

Property* AbstractEUKind::getProperty(int propertyIndex) const
{
	return m_Properties[propertyIndex];
}