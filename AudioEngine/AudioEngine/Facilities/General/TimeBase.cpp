
#include "TimeBase.h"

#ifdef WIN32
#include "windows.h"
#endif //WIN32

TimeBase::TimeBase(Logger* _logger)
{
	stopThread = false;
	logger = _logger;
}

int TimeBase::start(void)
{
	logger->writeLine("Started timebase");
	threadHandle = ::CreateThread(NULL,0,&TimeBaseThreadProc,this,0,&threadId);
	if(threadHandle==NULL)
	{
		logger->writeLine("Error while creating thread");
		return ERROR_DURING_THREAD_CREATION;
	}
	return 0;
}

int TimeBase::stop(void)
{
	logger->writeLine("Stopped timebase");
	stopThread = true;
	::TerminateThread(this->threadHandle,0);
	return 0;
}

void TimeBase::beat(void)
{
	std::vector<BeatHook*>::iterator it;
	for(it=beatHookVect.begin();it!=beatHookVect.end();++it)
	{
		(*it)->beat();
	}
}

void TimeBase::registerBeatObject(BeatHook* pBeatHook)
{
	beatHookVect.push_back(pBeatHook);
}

void TimeBase::setThreadStopped(void)
{
	stopThread = false;
}

const int TimeBase::TIMEBASE_PERIOD = 10;

DWORD WINAPI TimeBaseThreadProc(LPVOID param)
{
	TimeBase* timeBase = (TimeBase *) param;
	timeBase->logger->writeLine("Started timebase thread");
	while(1)
	{

#ifdef WIN32
		Sleep(TimeBase::TIMEBASE_PERIOD);
#endif //WIN32
		if(timeBase->stopThread)
		{
			timeBase->logger->writeLine("Timebase thread stopped");
			return 0;
		}
		timeBase->beat();
		//timeBase->logger->writeLine("Beating");

	}
	
	return 0;
}

