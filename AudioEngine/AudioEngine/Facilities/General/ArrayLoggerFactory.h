#pragma once

using namespace std;

#include <map>

#include "IArrayLogger.h"
#include "Logger.h"


class ArrayLoggerFactory
{
public:
	ArrayLoggerFactory(Logger* logger);
	IArrayLogger* getNewArrayLogger(char* name);
	void dumpAllCSV();
private:
	map<char*, IArrayLogger*> m_LoggersMap;
	Logger* logger;
};