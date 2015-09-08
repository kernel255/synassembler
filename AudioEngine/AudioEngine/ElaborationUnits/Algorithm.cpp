
#include "Algorithm.h"


/**
* @class Algorithm
* Abstract Algorithm definition
* Manage a set of connected ElaborationUnits.
*/

Algorithm::Algorithm(ModuleServices* pServices, int numProperties) : VirtualElaborationUnit(pServices, numProperties, NULL)
{
	EUidCounter = 0;
}

int Algorithm::GetBiggestId() 
{ 
	return EUidCounter; 
}


Algorithm::~Algorithm()
{
	//Destroy the list of EUs
	ProxyElaborationUnitsListIterator it;
	m_pModuleServices->pLogger->writeLine("Algorithm destructor");

	it = m_ProxyEUList.begin();
	while( it != m_ProxyEUList.end() )
	{
		ElaborationUnitFactory* factory = it->getFactory();
		ElaborationUnit* pEU = it->getEU();
		factory->destroyElaborationUnit(pEU);
		it = m_ProxyEUList.erase( it );
	}

}


/*
NEVER USED...FOR NOW COMMENTED
bool Algorithm::connectElaborationUnits(ElaborationUnit* pSourceEU, ElaborationUnit* pDestinationEU, ElaborationUnitPort* pSourcePort, ElaborationUnitPort* pDestinationPort)
{
	//ElaborationUnit *pSrcEU = static_cast<ElaborationUnit *>(pSourceEU);
	//ElaborationUnit *pDstEU = static_cast<ElaborationUnit *>(pDestinationEU);
	if( pSourcePort->getDirection() == ElaborationUnitPort::INPUT_PORT )
	{
		pSourceEU->setInputEU(pSourcePort,pDestinationEU,pDestinationPort);
		pDestinationEU->setOutputEU(pDestinationPort,pSourceEU,pSourcePort);
		return true;
	}
	else
	{
		if( pSourcePort->getDirection() == ElaborationUnitPort::OUTPUT_PORT )
		{
			pSourceEU->setOutputEU(pSourcePort,pDestinationEU,pDestinationPort);
			pDestinationEU->setInputEU(pDestinationPort,pSourceEU,pSourcePort);
			return true;
		}
		else
		{
			return false;
		}
	}
}
*/

void Algorithm::Update(EAG_SAMPLE_TYPE *pSamplesBuffer,int numsamples)
{
}

//Algorithm's inputs and outputs will be defined later but will allow to create macro-blocks
ElaborationUnitPort* Algorithm::getNthOutputPort(int n )
{
	return NULL;
}

ElaborationUnitPort* Algorithm::getNthInputPort(int n )
{
	return NULL;
}

void Algorithm::allocate(void)
{
	ProxyElaborationUnitsListIterator it;
	it = m_ProxyEUList.begin();
	while (it != m_ProxyEUList.end())
	{
		ProxyElaborationUnit&  proxy = *it;
		proxy.getEU()->allocate();
		it++;
	}
}

void Algorithm::deallocate(void)
{
	ProxyElaborationUnitsListIterator it;
	it = m_ProxyEUList.begin();
	while (it != m_ProxyEUList.end())
	{
		ProxyElaborationUnit&  proxy = *it;
		proxy.getEU()->deallocate();
		it++;
	}
}

void Algorithm::play(void)
{
	// Ask to all the involved EU to allocate
	allocate();
	//Play all the EUs
	ProxyElaborationUnitsListIterator it;
	it = m_ProxyEUList.begin();
	while( it != m_ProxyEUList.end() )
	{
		ProxyElaborationUnit& proxy = *it;
		proxy.getEU()->play();
		it++;
	}

}

void Algorithm::stop()
{
	ProxyElaborationUnitsListIterator it;
	it = m_ProxyEUList.begin();
	while( it != m_ProxyEUList.end() )
	{
		ProxyElaborationUnit& proxy = *it;
		proxy.getEU()->stop();
		it++;
	}
}

void Algorithm::pause(void)
{
}

const EUKind* Algorithm::getKind(void)
{
	return NULL;
}

/**
 *	\fn iAddElaborationUnit
 *	\brief Add the EU to Algorithm container
 *  \param pEU Pointer to the EU buid by a factory
 *  \param pFact Ptr to the builder factory
 */

int Algorithm::AddElaborationUnit(ElaborationUnit* pEU, ElaborationUnitFactory *pFact)
{
	ProxyElaborationUnit* proxy = new ProxyElaborationUnit(pEU,pFact);
	m_ProxyEUList.push_back( *proxy );
	return 0;
}

bool Algorithm::isSaved(void) 
{ 
	return m_bSaved; 
}

bool Algorithm::setInputEU(ElaborationUnitPort* pLocalPort, ElaborationUnit* pInputEU, ElaborationUnitPort* pInputPort)
{
	return false;
}

bool Algorithm::setOutputEU(ElaborationUnitPort* pLocalPort, ElaborationUnit* pOutputEU, ElaborationUnitPort* pOutputPort)
{
	return false;
}

int Algorithm::getInputPortNumber(void)
{
	return 0;
}

int Algorithm::getOutputPortNumber(void)
{
	return 0;
}

ElaborationUnitPort* Algorithm::getInputPortByEU(ElaborationUnit* pEU, int& n)
{
	return NULL;
}

ElaborationUnitPort* Algorithm::getOutputPortByEU(ElaborationUnit* pEU, int& n)
{
	return NULL;
}

void Algorithm::setSamplesBufferMaximumSize(int size)
{
}

void Algorithm::updateAudioSamples(EAG_SAMPLE_TYPE *pSamplesBuffer,int numsamples)
{
}

void Algorithm::receiveMIDIMessage(MIDIChannelMessage& midimsg)
{
}

int Algorithm::setProperty(int propertyIndex, const char* value)
{
	return 0;
}

int Algorithm::getProperty(int propertyIndex, char* value, int bufferSize)
{
	return NULL;
}

#define VALID_ALGORITHM ""
#define MESSAGE_BUFFER_SIZE		1000

char* Algorithm::checkAlgorithmValidity(void)
{
	ProxyElaborationUnitsListIterator it;
	ElaborationUnit* pFinalEU = NULL;

	static char s_MessageBuffer[MESSAGE_BUFFER_SIZE];
	memset(s_MessageBuffer,0,MESSAGE_BUFFER_SIZE);

	std::vector<ElaborationUnit*> finalEUVector;
	// Check if there is at least a final EU
	it = m_ProxyEUList.begin();
	while( it != m_ProxyEUList.end() )
	{
		ElaborationUnit* pEU = (*it).getEU();

		if(pEU->getKind()->isFinalElaborationUnit())
		{
			pFinalEU = pEU;
			finalEUVector.push_back(pEU);
		}
		it++;
	}
	// Verify all the connections
	it = m_ProxyEUList.begin();
	while( it != m_ProxyEUList.end() )
	{
		ElaborationUnit* pEU = (*it).getEU();
		const EUKind* kind = pEU->getKind();
		const char* portName;
		const char* euName = kind->getName();
		//Check inputs
		int numInput = kind->getInputPortNumber();
		for(int i=0;i<numInput;i++)
		{
			portName = kind->getNthInputPortName(i);
			if(kind->isInputPortConnectionMandatory(i))
			{
				/*
				if(pEU->getNthInputPort(i)->isPortAvailable())
				{
					sprintf_s(s_MessageBuffer,"The input %s of Elaboration Unit %s need to be connected", portName, euName);
					return s_MessageBuffer;
				}
				*/
			}
		}
		// Check outputs
		int numOutput = kind->getOutputPortNumber();
		for(int i=0;i<numOutput;i++)
		{
			portName = kind->getNthOutputPortName(i);
			if(kind->isOutputPortConnectionMandatory(i))
			{
				/*
				if(pEU->getNthOutputPort(i)->isPortAvailable())
				{
					sprintf_s(s_MessageBuffer,"The input %s of Elaboration Unit %s need to be connected", portName, euName);
					return s_MessageBuffer;
				}
				*/
			}
		}
		it++;
	}

	if(finalEUVector.size()==0)
		return "ERROR: The algorithm does not contains a final Elaboration Unit";


	return VALID_ALGORITHM;
}