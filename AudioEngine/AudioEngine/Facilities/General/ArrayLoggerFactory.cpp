#include "ArrayLoggerFactory.h"
#include "ArrayLogger.h"

ArrayLoggerFactory::ArrayLoggerFactory(Logger * _logger)
{
	this->logger = _logger;
}

IArrayLogger* ArrayLoggerFactory::getNewArrayLogger(char* name)
{
	ArrayLogger* pArrayLogger = new ArrayLogger(name);

	m_LoggersMap[name] = pArrayLogger;

	return pArrayLogger;
}

void ArrayLoggerFactory::dumpAllCSV()
{
	// TODO: save all loggers as CSV
	for (auto const &ent : m_LoggersMap)
	{
		char* name = ent.first;
		IArrayLogger* aLogger = ent.second;
		const char* content = aLogger->getContent();
		const size_t len = strlen(content) + 1;
		wchar_t* wc = new wchar_t[len];
		mbstowcs(wc, content, len);
		logger->writeLine(wc);
		delete wc;
	}
}
