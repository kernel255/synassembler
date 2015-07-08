#pragma once

//#include "stdafx.h"

#include "ElaborationUnit.h"
#include "ModuleServices.h"
#include "..\Facilities\General\PropertyValuesHolder.h"

#define UNAVAILABLE_PROPERTY		-1

/**
 * @class Is responsible for the management of simple EU stuff: unique id, properties, etc...
 */

class AbstractElaborationUnit : public ElaborationUnit {
protected:
	/**
	 * @var Unique integer identified of the EU
	 */
	int iId;
	/**
	 * @var True if the EU has been stopped
	 */
	bool stopped;
	PropertyValuesHolder m_ValuesHolder;
public:
	AbstractElaborationUnit(ModuleServices* pServices, int numProperties, const EUKind* kind);
	int getId();
	void setId(int id);
	~AbstractElaborationUnit();
	void play(void);
	void stop(void);
	bool isPlaying();
	virtual int getProperty(int propertyIndex, char* value, int bufferSize);
	virtual int setProperty(int propertyIndex, const char* value);

	virtual int setDProperty(int propertyIndex, double value);
	virtual int getDProperty(int propertyIndex, double* value);

	virtual int setIProperty(int propertyIndex, int value);
	virtual int getIProperty(int propertyIndex, int* value);

	virtual int setBProperty(int propertyIndex, bool value);
	virtual int getBProperty(int propertyIndex, bool* value);

protected:
	ModuleServices* m_pModuleServices;
	int iNumInput;
	int iNumOutput;
	bool playing;
};