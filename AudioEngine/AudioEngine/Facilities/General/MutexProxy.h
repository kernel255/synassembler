
#ifdef WIN32
#include <Windows.h>
#endif //WIN32
#include <basetsd.h>
#include <stdint.h>

class MutexProxy
{
#ifdef WIN32
	HANDLE hMutex;
#endif //WIN32

public:
#ifdef WIN32
	static const int WAIT_INFINITE = INFINITE;
#endif //WIN32
	MutexProxy(wchar_t* name);
	uint32_t WaitMutex(UINT32 timeoutMillisecs);
	int ReleaseMutex();
};