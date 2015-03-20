
#ifndef SPECOSINFO
#define SPECOSINFO 1

#define MAX_OSINFO_SIZE			100

#ifdef WIN32
#include <windows.h>
#endif //WIN32

struct SpecificOSInfo
{
#ifdef WIN32
	HWND hwnd;
#endif //WIN32
};

#endif //SPECOSINFO