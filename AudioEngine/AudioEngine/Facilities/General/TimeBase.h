
#pragma once
#include <vector>
#include <Windows.h>
#include "Logger.h"

#define ERROR_DURING_THREAD_CREATION	-1

class BeatHook {
public:
	virtual void beat(void) = 0;
};

class TimeBase {
public:
	void registerBeatObject(BeatHook* pBeatHook);
	TimeBase(Logger* log);
	int start(void);
	int stop(void);
	void beat(void);
	void setThreadStopped(void);
	static const int TIMEBASE_PERIOD;
private:
	friend DWORD WINAPI TimeBaseThreadProc(LPVOID param);
	std::vector<BeatHook*> beatHookVect;
	bool stopThread;
#ifdef WIN32
	HANDLE threadHandle;
	DWORD threadId;
#endif //WIN32
	Logger* logger;
};