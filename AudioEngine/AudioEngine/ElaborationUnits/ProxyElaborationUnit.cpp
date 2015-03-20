
#include "ProxyElaborationUnit.h"
#include "ElaborationUnitFactory.h"

//template <class EUtype>
ProxyElaborationUnit::ProxyElaborationUnit(ElaborationUnit* pEU, ElaborationUnitFactory* pFact)
{
	m_pEU = pEU;
	m_pFactory = pFact;
	m_RefCount = 0;
}
ProxyElaborationUnit::~ProxyElaborationUnit()
{
	/*
	if( m_RefCount == 1 )
		//Ask to the Factory to destroy his EU
		m_pFactory->DestroyElaborationUnit( m_pEU );
	assert(m_RefCount <= 1);
	*/
}
ProxyElaborationUnit& ProxyElaborationUnit::operator=(ProxyElaborationUnit& right)
{
	m_pEU = right.m_pEU;
	return *this;
}
ProxyElaborationUnit::ProxyElaborationUnit(const ProxyElaborationUnit& PEU)
{
	//Copy by "hand" all the members because the = operator causes an error:
	//*this = PEU;
	m_pEU = PEU.m_pEU;
	m_pFactory = PEU.m_pFactory;
	m_RefCount = PEU.m_RefCount;
	m_RefCount++;
}
