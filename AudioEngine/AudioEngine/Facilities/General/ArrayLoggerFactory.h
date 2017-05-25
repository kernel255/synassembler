#pragma once

using namespace std;

#include <map>

#include "IArrayLogger.h"



class ArrayLoggerFactory
{
public:
	IArrayLogger* getNewArrayLogger(char* name);
	void dumpAllCSV();
private:
	map<char*, IArrayLogger*> m_LoggersMap;
};