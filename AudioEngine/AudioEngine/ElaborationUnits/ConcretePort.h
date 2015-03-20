#pragma once

#include <assert.h>
#include "ElaborationUnitPort.h"
#include "ElaborationUnit.h"

class ConcretePort : public ElaborationUnitPort
{
private:
	typedef std::list<PortConnection> ConnectionsList;
	typedef std::list<PortConnection>::iterator ConnectionListIterator;
	ConcretePort(void);
public:
	ConcretePort(e_PortDirection dir, e_PortKind kind, e_PortMultiplicity multi, unsigned int maxsize = 0);
	void setNthEUandPort(ElaborationUnit* peu, ElaborationUnitPort* port, int n);
	void setNthPortConnection(ElaborationUnit* peu, ElaborationUnitPort* port, int n);
	void setPortConnection(ElaborationUnitPort::PortConnectionsIterator& it, ElaborationUnit* peu, ElaborationUnitPort* port);
private:
	e_PortMultiplicity multiplicity;
	//ConnectionsList m_ConnectionsList;
};