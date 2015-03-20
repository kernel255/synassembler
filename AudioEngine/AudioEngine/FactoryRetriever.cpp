
#include <windows.h>
#include <string.h>
#include <cstring>
#include <wchar.h>

#include "ElaborationUnits\ElaborationUnitFactory.h"
#include "FactoryRetriever.h"

FactoryRetriever::FactoryRetriever(const wchar_t* _folderName, ModuleServices* pServices)
{
	m_pServices = pServices;
	folderName = _folderName;
	getFactories(pServices);
}
FactoryRetriever::~FactoryRetriever()
{

}
int FactoryRetriever::getFactoriesNumber()
{
	return m_FactoryInfos.size();
}

ElaborationUnitFactory* FactoryRetriever::getNthFactory(unsigned int factoryIndex)
{
	if(factoryIndex>=this->m_FactoryInfos.size())
		return NULL;
	else
		return m_FactoryInfos[factoryIndex].m_pFactory;
}

void FactoryRetriever::removeElaborationUnit(int factoryIndex, ElaborationUnit* peu)
{
	ElaborationUnitFactory* factory = getNthFactory(factoryIndex);
	if(factory==NULL)
		return;
	factory->destroyElaborationUnit(peu);
}

#ifdef WIN32
std::vector<ElaborationUnitFactory*> FactoryRetriever::getFactories(ModuleServices* pServices)
{
	char buffer[200];
	memset(buffer,'\0',200);
	
	HANDLE hfile;
	WIN32_FIND_DATA finddata;
	int factorycount = 0;

	pServices->pLogger->writeLine("Folder name:");
	pServices->pLogger->writeLine(folderName.c_str());

	memset(&finddata,0,sizeof(WIN32_FIND_DATA));
	if( folderName.length() != 0 )
	{
		/*
		//Try to set the new directory
		const wchar_t* folder = folderName.c_str();
		if( !SetCurrentDirectory(folder) )
		{
			sprintf(buffer,"Unable to set folder:%s", folder);
			m_pServices->pLogger->writeLine(buffer);

			DWORD errorCode = ::GetLastError();
			sprintf(buffer,"Error Code=%d", errorCode);
			m_pServices->pLogger->writeLine(buffer);

			wchar_t wbuffer[200];
			memset(wbuffer,'\0',200*sizeof(wchar_t));
			::FormatMessage(FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ARGUMENT_ARRAY,
				NULL, errorCode, 0, wbuffer, 200, NULL);
			m_pServices->pLogger->writeLine((wchar_t *) wbuffer);

			return m_Factories;
		}
		*/
	}
	//Scan all the dll and try to open them
	BOOL filefound = TRUE;

	wstring path(folderName);
	wstring pathNoBackSlash(folderName);
	path += L"\\";
	wstring wildcard(path);
	wildcard += L"*.dll";
	hfile = FindFirstFile(wildcard.c_str() ,&finddata);
	m_pServices->pLogger->writeLine("Find file=%08x", hfile);
	if(hfile==INVALID_HANDLE_VALUE)
	{
			DWORD error = ::GetLastError();
			m_pServices->pLogger->writeLine("FindFirstFile failure = %d", error);
	}

	//Scan all the dlls and opens them
	while( (hfile != INVALID_HANDLE_VALUE) && filefound )
	{
		FactoryModuleInfo factInfo;
		wstring libraryFullPath(path);
		//Try to open the library
		libraryFullPath += finddata.cFileName;

		m_pServices->pLogger->writeLine("Trying to open library: %s", libraryFullPath);

		extractInfoByLibrary(libraryFullPath.c_str(), pServices, factInfo);
		if(factInfo.m_pFactory!=NULL && factInfo.m_pfnFactoryDeallocator!=NULL)
		{
			m_FactoryInfos.push_back(factInfo);
			m_Factories.push_back(factInfo.m_pFactory);
			factorycount++;
			m_pServices->pLogger->writeLine("Library open successful");
		}
		else
			m_pServices->pLogger->writeLine("Library open failure");
		/*
		//Try to open the library
		pfactory = extractFactoryByLibrary(finddata.cFileName, pServices);
		if( pfactory != NULL )
		{
			//Store the factory and related info
			factoryNames.push_back(factInfo);
			factories.push_back(factInfo.m_pFactory);
			//Successfully opened library
			factorycount++;
		}
		*/

		//BOOL result = ::SetCurrentDirectory(pathNoBackSlash.c_str());
		BOOL result = ::SetCurrentDirectory(path.c_str());
		if(!result)
		{
			DWORD error = ::GetLastError();
			m_pServices->pLogger->writeLine("SetCurrentDirectory failure = %d", error);
		}
		filefound = FindNextFile(hfile,&finddata);
		m_pServices->pLogger->writeLine("FindNextFile=%d", filefound);
		if(filefound==0)
		{
			DWORD error = ::GetLastError();
			m_pServices->pLogger->writeLine("FindNextFile failure = %d", error);
		}
	}
	m_pServices->pLogger->writeLine("Factories=%d", factorycount);

	if(hfile!=INVALID_HANDLE_VALUE)
		::FindClose(hfile);

	printf(buffer);

	return m_Factories;
}

/*
ElaborationUnitFactory* FactoryRetriever::extractFactoryByLibrary(wchar_t* dllname, ModuleServices *pServices)
{
	HINSTANCE hLibrary;
	pfnRetrieveConcreteFactory pfnRetConcre;

	hLibrary = LoadLibrary(dllname);
	if( hLibrary != NULL )
	{
		pfnRetConcre = (pfnRetrieveConcreteFactory) GetProcAddress(hLibrary,"RetrieveConcreteFactory");
		if(pfnRetConcre!=NULL)
		{
			//Call immediately the function to get factory
			ElaborationUnitFactory* pfact = (*pfnRetConcre)(pServices);

			LibraryHandler libHnd(hLibrary); 
			m_Libraries.push_back(libHnd);

			return pfact;
		}
		else
			return NULL;
	}
	else
		return NULL;
}
*/

bool FactoryRetriever::extractInfoByLibrary(const wchar_t* dllname, ModuleServices *pServices, FactoryRetriever::FactoryModuleInfo& factInfo)
{
	HINSTANCE hLibrary;
	pfnRetrieveConcreteFactory pfnRetConcre;
	pfnDeallocateConcreteFactory pfnDealloc;

	hLibrary = LoadLibrary(dllname);
	if( hLibrary != NULL )
	{
		pfnRetConcre = (pfnRetrieveConcreteFactory) GetProcAddress(hLibrary,"RetrieveConcreteFactory");
		pfnDealloc = (pfnDeallocateConcreteFactory) ::GetProcAddress(hLibrary,"releaseConcreteFactory");
		if(pfnRetConcre!=NULL && pfnDealloc!=NULL)
		{
			//Call immediately the function to get factory
			ElaborationUnitFactory* pFact = (*pfnRetConcre)(pServices);
			factInfo.m_pFactory = pFact;
			factInfo.m_DllhModule = hLibrary;
			factInfo.m_pfnFactoryDeallocator = pfnDealloc;

			return true;
		}
		else
			pServices->pLogger->writeLine("Library is not an ElaborationUnitFactory");
	}
	else
	{
		pServices->pLogger->writeLine("Library load failure %s", dllname);
		DWORD res = ::GetLastError();
		pServices->pLogger->writeLine("Error=%d", res);
	}

	return false;
}

void FactoryRetriever::releaseFactories(void)
{
	int size = m_FactoryInfos.size();
	for(int i=0;i<size;i++)
	{
		(m_FactoryInfos[i].m_pfnFactoryDeallocator)(m_FactoryInfos[i].m_pFactory);
	}
}

void FactoryRetriever::freeFactories(void)
{
	/*
	int numLibs = m_Libraries.size();
	for(int i=0;i<numLibs;i++)
	{
		HMODULE hLib = m_Libraries[i].hModule;
		for(int i=0;i<2;i++)
			::FreeLibrary(hLib);
	}
	*/
}

#endif //WIN32
