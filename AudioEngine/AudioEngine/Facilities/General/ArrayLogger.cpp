#include "ArrayLogger.h"

#include <stdio.h>
#include <string.h>

ArrayLogger::ArrayLogger(char* _name)
{
	name.assign(_name);
}

const char* ArrayLogger::getName()
{
	return name.c_str();
}

void ArrayLogger::addValue(double val)
{
	content.push_back(val);
}

const char* ArrayLogger::getContent()
{
	std::string content;
	content.append("ArrayLogger name: ");
	content.append(name);
	content.append("\n");
	int size = content.size();
	for (int i = 0; i < size; i++)
	{
		double val = content[i];
		char* buffer = new char[100];
		memset(buffer, 0, 100);
		snprintf(buffer, 100, "%f\n", val);
		content.append(buffer);
	}

	size_t len = content.length();
	char* result = new char[len];
	memcpy(result, content.c_str(), len);

	return result;
}
