
#pragma once

#include <string>

#include <vector>

#include "ElaborationUnits\ElaborationUnitFactory.h"
#include "ElaborationUnits\ElaborationUnit.h"
#include "ElaborationUnits\EUKind.h"
#include "ElaborationUnits\ModuleServices.h"

using namespace std;

typedef ElaborationUnitFactory* (*pfnRetrieveConcreteFactory) (ModuleServices*);
typedef void (*pfnDeallocateConcreteFactory) (ElaborationUnitFactory*);

class FactoryRetriever
{
public:
	FactoryRetriever(const wchar_t* folderName, ModuleServices* pServices);
	~FactoryRetriever();
	std::vector<ElaborationUnitFactory*> getFactories(ModuleServices* pServices);
	int getFactoriesNumber();
	ElaborationUnitFactory* getNthFactory(unsigned int factoryIndex);
	int getElaborationUnitNumber(int factoryIndex);
	const EUKind* getNthElaborationUnitKind(int factoryIndex, int elaborationUnitIndex);
	ElaborationUnit* createNthElaborationUnit(int factoryIndex, int elaborationUnitIndex);
	void removeElaborationUnit(int factoryIndex, ElaborationUnit* peu);
	/**
	 * Releases the libraries modules (DLL)
	 */
	void freeFactories(void);
	/**
	 * Releses the factories inside the modules
	 */
	void releaseFactories(void);
protected:
	/**
	 * @class Hold local informations about factories and their modules
	 */
	class FactoryModuleInfo {
	public:
		FactoryModuleInfo() {
			m_pFactory = NULL;
			m_pfnFactoryDeallocator = NULL;
		}
		ElaborationUnitFactory* m_pFactory;
		pfnDeallocateConcreteFactory m_pfnFactoryDeallocator;
#ifdef WIN32
		HMODULE m_DllhModule;
#endif //WIN32
	};
private:
	ElaborationUnitFactory* extractFactoryByLibrary(wchar_t* dllname, ModuleServices *pServices);
	bool extractInfoByLibrary(const wchar_t* dllname, ModuleServices *pServices, FactoryRetriever::FactoryModuleInfo& factInfo);

	wstring folderName;
	ModuleServices* m_pServices;

	std::vector<FactoryModuleInfo> m_FactoryInfos;
	
	std::vector<ElaborationUnitFactory*> m_Factories;

	/*
	class LibraryHandler
	{
	public:
		LibraryHandler(HMODULE _hModule) {
			hModule = _hModule;
		}
		HMODULE hModule;
	};
	std::vector<LibraryHandler> m_Libraries;
	*/
	
};