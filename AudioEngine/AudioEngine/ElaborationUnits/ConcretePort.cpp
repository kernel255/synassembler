
#include "ConcretePort.h"

ConcretePort::ConcretePort(void)
{
}

ConcretePort::ConcretePort(e_PortDirection dir, e_PortKind kinna, e_PortMultiplicity multi, unsigned int maxsize)
	{
		//Nothing is connected on creation
//		m_pInputEU = NULL;
//		m_pOutputEU = NULL;
		direction = dir;
		kind = kinna;
		multiplicity = multi;
		switch( multiplicity )
		{
		case SINGLE_PORT:
			{
				PortConnection uniquePort;
				connectionsArray.push_back(uniquePort);
				break;
			}
		case MULTI_PORT:
			{
				//At the beginning, the array is empty
				connectionsArray.resize(0);
				break;
			}
		case FIXED_MULTIPORT:
			{
				connectionsArray.resize(maxsize);
				break;
			}
		default:
			{
				assert(false);
			}
		}
	}

void ConcretePort::setNthEUandPort(ElaborationUnit* peu, ElaborationUnitPort* port, int n)
{
	setNthPortConnection(peu,port,n);
}

void ConcretePort::setNthPortConnection(ElaborationUnit* peu, ElaborationUnitPort* port, int n)
{
	ElaborationUnitPort::PortConnectionsIterator it;
	//ConcretePort::ConnectionListIterator it;
	if( n == C_AddNewConnection )
	{
		if( multiplicity == ElaborationUnitPort::SINGLE_PORT)
		{
			//A new connection cannot be added to a SinglePort
			assert(false);
		}
		else
		{
			//Add a new connction to a MultiPort
			PortConnection p((peu != NULL)? peu : NULL,(port != NULL)? port : NULL);

			//Trying to remove the connectionList
			this->connectionsArray.push_back(p);
			//m_ConnectionsList.push_back(p);

		}
	}
	else
	{
		if( multiplicity == ElaborationUnitPort::SINGLE_PORT )
		{
			//SinglePort has only one ConnecionPort
			assert(n==0);
			//Set the connection to the first element
			//Trying to remove the connectionList
			//it = m_ConnectionsList.begin();
			it = connectionsArray.begin();
			setPortConnection(it,peu,port);
		}
		else
		{
			int num = 0;
			for(it=connectionsArray.begin();it!=connectionsArray.end();++it)
			{
				if( n == num )
				{
					setPortConnection(it,peu,port);
					return;
				}
				num++;
			}
			/*
			for(it=m_ConnectionsList.begin();it!=m_ConnectionsList.end();++it)
			{
				if( n == num )
				{
					setPortConnection(it,peu,port);
					return;
				}
				num++;
				//j = it - m_ConnectionsList.begin();
			}
			*/
			//n has to be found
			assert(false);
		}
	}
}


void ConcretePort::setPortConnection(ElaborationUnitPort::PortConnectionsIterator& it, ElaborationUnit* peu, ElaborationUnitPort* port)
{
	if( port != NULL )
		(*it).m_pPort = port;
	if( peu != NULL )
		(*it).m_pEU = peu;
}


/*
void ConcretePort::setPortConnection(ConcretePort::ConnectionListIterator& it, ElaborationUnit* peu, ElaborationUnitPort* port)
{
	if( port != NULL )
		(*it).m_pPort = port;
	if( peu != NULL )
		(*it).m_pEU = peu;
}
*/


