
#include "ElaborationUnitPort.h"

bool ElaborationUnitPort::isPortCompatible(ElaborationUnitPort* const pSource, ElaborationUnitPort* const pDestination)
{
	bool bDirectionCompatibility = false;
	bool bKindCompatibility = false;

	if( (pSource==NULL) || (pDestination==NULL) )
		return false;
	if( 
		((pSource->getDirection() == INPUT_PORT) &&
		(pDestination->getDirection() == OUTPUT_PORT)) ||
		((pSource->getDirection() == OUTPUT_PORT) &&
		(pDestination->getDirection() == INPUT_PORT))
		)
		bDirectionCompatibility = true;

	if( pSource->kind == pDestination->kind )
		bKindCompatibility = true;

	//Remember to add the inout conditions too
	return bDirectionCompatibility && bKindCompatibility;
}

bool ElaborationUnitPort::isPortAvailable(const ElaborationUnitPort& port)
{
	switch( port.multiplicity )
	{
	case SINGLE_PORT:
		{
			if( port.connectionsArray[0].m_pEU )
				return false;
			else
				return true;
			break;
		}
	case MULTI_PORT:
		{
			//Is always possible to add an element
			return true;
			break;
		}
	case FIXED_MULTIPORT:
		{
			//assert(false);
			return false;
			break;
		}
	default:
		return false;
		break;
	}
}

int ElaborationUnitPort::getConnectionsNumber(void) 
{ 
	return (int) connectionsArray.size();
}
class ElaborationUnit* ElaborationUnitPort::getNthEU(int n) 
{ 
	return connectionsArray[n].m_pEU; 
}
ElaborationUnitPort* ElaborationUnitPort::getNthEUPort(int n) 
{ 
	return connectionsArray[n].m_pPort; 
}
void ElaborationUnitPort::setNthEU(int n, class ElaborationUnit* peu) 
{ 
	connectionsArray[n].m_pEU = peu; 
}
void ElaborationUnitPort::setNthEUPort(int n, ElaborationUnitPort* port) 
{ 
	connectionsArray[n].m_pPort = port; 
}
ElaborationUnitPort::e_PortDirection ElaborationUnitPort::getDirection() 
{ 
	return direction; 
}
ElaborationUnitPort::e_PortKind ElaborationUnitPort::getKind() 
{ 
	return kind; 
}
void ElaborationUnitPort::setName(const char *pName) 
{ 
	name = pName; 
}
const char* ElaborationUnitPort::getName()
{
	return name;
}