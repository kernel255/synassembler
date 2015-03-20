
#pragma once

#include "ElaborationUnit.h"
#include "VirtualElaborationUnit.h"
#include "ProxyElaborationUnit.h"
#include "AbstractElaborationUnit.h"
#include "ElaborationUnitPort.h"

//template <class EUtype, class Factorytype>
class Algorithm : public VirtualElaborationUnit
{
public:
	Algorithm(ModuleServices* pServices, int numProperties);
	~Algorithm();
	virtual void Update(EAG_SAMPLE_TYPE *pSamplesBuffer,int numsamples);
	virtual void updateAudioSamples(EAG_SAMPLE_TYPE *pSamplesBuffer,int numsamples);
	virtual void receiveMIDIMessage(MIDIChannelMessage& midimsg);
	int AddElaborationUnit( ElaborationUnit* pEU, ElaborationUnitFactory *pFact);
	char* checkAlgorithmValidity(void);
	//bool connectElaborationUnits(ElaborationUnit* pSourceEU, ElaborationUnit* pDestinationEU, ElaborationUnitPort* pSourcePort, ElaborationUnitPort* pDestinationPort);
	bool loadAlgorithm(const char* pcAlgofilename, const char** ppcError);
	bool saveAlgorithm(const char* pcAlgofilename, const char** ppcError);
	virtual void allocate(void);
	virtual void deallocate(void);
	virtual void play(void);
	virtual void stop(void);
	virtual void pause(void);
	virtual void setSamplesBufferMaximumSize(int size);
	const EUKind* getKind(void);
	int GetBiggestId();
	void setEUFactoryDirectory(/*EUFactoryDirectory<Factorytype> * pEUFactDir*/);
	bool erase(void);
	ElaborationUnit* getEUById(int id);
	/**
	* @fn Check if the Algorithm has been modified and return false if modified
	*/
	bool isSaved(void);
	ElaborationUnitPort* getNthInputPort(int n);
	ElaborationUnitPort* getNthOutputPort(int n);
	virtual bool setInputEU(ElaborationUnitPort* pLocalPort, ElaborationUnit* pInputEU, ElaborationUnitPort* pInputPort);
	virtual bool setOutputEU(ElaborationUnitPort* pLocalPort, ElaborationUnit* pOutputEU, ElaborationUnitPort* pOutputPort);
	int getInputPortNumber(void);
	int getOutputPortNumber(void);
	ElaborationUnitPort* getInputPortByEU(ElaborationUnit* pEU, int& n);
	ElaborationUnitPort* getOutputPortByEU(ElaborationUnit* pEU, int& n);
	virtual int setProperty(int propertyIndex, const char* value);
	virtual int getProperty(int propertyIndex, char* value, int bufferSize);



protected:
	/**
	\var The last id assigned to an ElaborationUnit
	*/
	int EUidCounter;
	//typedef typename std::list<ProxyEU<EUtype,Factorytype> > ProxyElaborationUnitsList;
	//typedef typename std::list<ProxyEU<EUtype,Factorytype> >::iterator ProxyElaborationUnitsListIterator;
	typedef std::list<ProxyElaborationUnit> ProxyElaborationUnitsList;
	typedef std::list<ProxyElaborationUnit>::iterator ProxyElaborationUnitsListIterator;
	/**
	* @var List of the ElaborationUnits contained inside the Algorithm
	*/
	ProxyElaborationUnitsList m_ProxyEUList;
	/**
	* @var True if the Algorithm has been saved
	*/
	bool m_bSaved;
	//AbstractXMLElement* saveEUId(AbstractXMLParser* pParser, EUtype* pEU);
	//AbstractXMLElement* saveFactory(AbstractXMLParser* pParser, ProxyElaborationUnitsListIterator it);
private:
	//static EUFactoryDirectory<Factorytype>* m_pEUFactoryDirectory;
	int idCounter;
};
