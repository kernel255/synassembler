
#pragma once

#include "ElaborationUnit.h"
#include "ElaborationUnitFactory.h"

/**
* @class Placeholder for true EU, that has to be deleted directly by their Factory
*/
//template <class EUtype>
class ProxyElaborationUnit
{
private:
	ElaborationUnit*		m_pEU;
	ElaborationUnitFactory	*m_pFactory;
	int						m_RefCount;
public:
	//ProxyElaborationUnit(void) { m_pEU = NULL; }
	ProxyElaborationUnit(ElaborationUnit* pEU, ElaborationUnitFactory* pFact);
	~ProxyElaborationUnit();
	ProxyElaborationUnit& operator=(ProxyElaborationUnit& right);
	ProxyElaborationUnit(const ProxyElaborationUnit& PEU);
	ElaborationUnit* getEU(void) { return m_pEU; }
};
