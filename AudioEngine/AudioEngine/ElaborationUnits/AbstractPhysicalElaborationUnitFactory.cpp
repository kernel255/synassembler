
#include "AbstractPhysicalElaborationUnitFactory.h"

using namespace std;

AbstractPhysicalElaborationUnitFactory::AbstractPhysicalElaborationUnitFactory(ModuleServices* moduleServices) : AbstractElaborationUnitFactory(moduleServices)
{
	int i = 0;
}

AbstractPhysicalElaborationUnitFactory::~AbstractPhysicalElaborationUnitFactory(void)
{
	PhysicalEUDescription* desc;
	vector<PhysicalEUDescription*>::iterator it;
	for(it=instancesVect.begin();it!=instancesVect.end();++it)
	{
		desc = *it;
		delete desc;
	}
}

void AbstractPhysicalElaborationUnitFactory::addInstance(PhysicalEUDescription* instance)
{
	instancesVect.push_back(instance);
}

void AbstractPhysicalElaborationUnitFactory::removeInstance(PhysicalEUDescription* instance)
{
	PhysicalEUDescription* desc;
	std::vector<PhysicalEUDescription*>::iterator it;
	for(it=instancesVect.begin();it!=instancesVect.end();++it)
	{
		desc = *it;
		if(desc==instance)
		{
			delete *it;
			instancesVect.erase(it);
			return;
		}
	}
}

unsigned int AbstractPhysicalElaborationUnitFactory::getPhysicalEUInstanceNumber(int euIndex)
{
	int num = 0;
	PhysicalEUDescription* desc;
	std::vector<PhysicalEUDescription*>::iterator it;
	for(it=instancesVect.begin();it!=instancesVect.end();++it)
	{
		desc = *it;
		if(desc->euIndex==euIndex)
			num++;
	}
	return num;	
}

const wchar_t* AbstractPhysicalElaborationUnitFactory::getNthPhysicalEUInstanceName(int euIndex, int instanceIndex)
{
	PhysicalEUDescription* desc;
	std::vector<PhysicalEUDescription*>::iterator it;
	//int index = 0;
	for(it=instancesVect.begin();it!=instancesVect.end();++it)
	{
		desc = *it;
		//if(desc->euIndex==euIndex && instanceIndex==index)
		if(desc->euIndex==euIndex && desc->instanceIndex==instanceIndex)
			return desc->name.c_str();
		//index++;
	}

	return NULL;
}

AbstractPhysicalElaborationUnitFactory::PhysicalEUDescription* AbstractPhysicalElaborationUnitFactory::getNthEUDescription(int euIndex, int instanceIndex)
{
	PhysicalEUDescription* desc;
	std::vector<PhysicalEUDescription*>::iterator it;
	//int index = 0;
	for(it=instancesVect.begin();it!=instancesVect.end();++it)
	{
		desc = *it;
		if(desc->euIndex==euIndex && desc->instanceIndex==instanceIndex)
			return desc;
		//index++;
	}

	return NULL;

}

AbstractPhysicalElaborationUnitFactory::PhysicalEUDescription* AbstractPhysicalElaborationUnitFactory::getEUDescrByName(int euIndex, std::wstring name)
{
	PhysicalEUDescription* desc;
	std::vector<PhysicalEUDescription*>::iterator it;
	//int index = 0;
	for (it = instancesVect.begin(); it != instancesVect.end(); ++it)
	{
		desc = *it;
		if (desc->euIndex == euIndex)
		{
			if (desc->name.compare(name) == 0)
				return desc;
		}
	}

	return NULL;
}
