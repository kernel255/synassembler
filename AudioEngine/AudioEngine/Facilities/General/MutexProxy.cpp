
#include "MutexProxy.h"

MutexProxy::MutexProxy(wchar_t* name)
{

#ifdef WIN32
	if (name != NULL)
		hMutex = ::CreateMutex(NULL, FALSE, name);
	else
		hMutex = ::CreateMutex(NULL, FALSE, FALSE);
#endif //WIN32
}

UINT32 MutexProxy::WaitMutex(UINT32 timeoutMillisecs)
{
#ifdef WIN32
	uint32_t res = ::WaitForSingleObject(hMutex, timeoutMillisecs);
	return res;
#endif //WIN32
}

int MutexProxy::ReleaseMutex()
{
#ifdef WIN32
	return ::ReleaseMutex(hMutex);
#endif //WIN32
}
