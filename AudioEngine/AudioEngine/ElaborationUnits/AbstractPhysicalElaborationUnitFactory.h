
#pragma once

#include <vector>
#include <string>

#include "AbstractElaborationUnitFactory.h"

#define INFO_BUFFER_SIZE		100;

/**
 * @class This class has the responsibility of storing the available physical resources available
 */

class AbstractPhysicalElaborationUnitFactory : public AbstractElaborationUnitFactory
{
public:
	/**
	 * @class Holds the information about a specific instance of the Physical EU
	 */
	class PhysicalEUDescription 
	{
	public:
		std::wstring name;
		char* specificInfoStr;
		int specInfoSize;
		/**
		 * Index of the EU kind inside the Factory
		 */
		int euIndex;
		/**
		 * Index of the instance inside the EU kind
		 */
		int instanceIndex;
		PhysicalEUDescription(const wchar_t* _name, char* infoStr, int infoStrSize, int _euIndex, int _instanceIndex)
		{
			specInfoSize = infoStrSize;
			specificInfoStr = (char *) malloc(infoStrSize);
			memcpy(specificInfoStr, infoStr, infoStrSize);
			name = _name;
			euIndex = _euIndex;
			instanceIndex = _instanceIndex;
		}
		~PhysicalEUDescription() 
		{
			free(specificInfoStr);
		}
	};
	AbstractPhysicalElaborationUnitFactory(ModuleServices* moduleServices);
	virtual ~AbstractPhysicalElaborationUnitFactory(void);
	virtual void addInstance(PhysicalEUDescription* instance);
	virtual void removeInstance(PhysicalEUDescription* instance);
	virtual unsigned int getPhysicalEUInstanceNumber(int euIndex);
	virtual const wchar_t* getNthPhysicalEUInstanceName(int euIndex, int instanceIndex);
	virtual PhysicalEUDescription* getNthEUDescription(int euIndex, int instanceIndex);
	virtual PhysicalEUDescription* getEUDescrByName(int euIndex, std::wstring name);
private:
	std::vector<PhysicalEUDescription*> instancesVect;
};