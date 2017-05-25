#include "ArrayLoggerFactory.h"
#include "ArrayLogger.h"

IArrayLogger* ArrayLoggerFactory::getNewArrayLogger(char* name)
{
	ArrayLogger* pArrayLogger = new ArrayLogger(name);

	m_LoggersMap[name] = pArrayLogger;

	return pArrayLogger;
}

void ArrayLoggerFactory::dumpAllCSV()
{
	// TODO: save all loggers as CSV
}
