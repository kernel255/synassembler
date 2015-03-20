
#pragma once

#include <list>
#include <vector>

class ElaborationUnitPort 
{
protected:
	/**
	 * @class PortConnection Holds an instance of what/how is connected to the EU
	 * Holds which EU is the connected and the Port by which is connected
	 */
	class PortConnection
	{
	public:
		class ElaborationUnit* m_pEU;
		ElaborationUnitPort* m_pPort;
		PortConnection()
		{
			//By default nothing is connected
			m_pEU = NULL;
			m_pPort = NULL;
		}
		PortConnection(class ElaborationUnit *_peu, ElaborationUnitPort* _pport)
		{
			m_pEU = _peu;
			m_pPort = _pport;
		}
	};
public:
	//typedef std::list<PortConnection>::iterator ConnectionsIter;
	/**
	 * @enum e_PortMultiplicity States how many EU can be connected to the Port
	 */
	enum e_PortMultiplicity
	{
		SINGLE_PORT = 0,
		MULTI_PORT = 1,
		FIXED_MULTIPORT = 2
	};
	/**
	 * @enum e_PortDirection Defines how the signal can flow through the port
	 */
	enum e_PortDirection
	{
		INPUT_PORT = 0,
		OUTPUT_PORT = 1,
	};
	/**
	 * @enum e_PortKind Nature of the signal that flows through the port
	 */
	enum e_PortKind
	{
		MIDI_PORT = 0,
		AUDIO_PORT = 1
	};
	static bool isPortCompatible(ElaborationUnitPort* const pSource, ElaborationUnitPort* const pDestination);
	bool isPortAvailable(const ElaborationUnitPort& Port);
	e_PortMultiplicity getMultiplicity(void);
	bool isSinglePort(void);
	int getConnectionsNumber(void);
	class ElaborationUnit* getNthEU(int n);
	ElaborationUnitPort* getNthEUPort(int n);
	void setNthEU(int n, class ElaborationUnit* peu);
	void setNthEUPort(int n, ElaborationUnitPort* port);
	void setNthEUandPort(ElaborationUnit* peu, ElaborationUnitPort* port, int n);
	e_PortDirection getDirection();
	e_PortKind getKind();
	virtual void setName(const char *pName);
	const char* getName();
protected:
	std::vector<PortConnection> connectionsArray;
	e_PortDirection direction;
	e_PortKind kind;
	e_PortMultiplicity multiplicity;
	const char* name;
public:
	typedef std::vector<PortConnection>::iterator PortConnectionsIterator;
};