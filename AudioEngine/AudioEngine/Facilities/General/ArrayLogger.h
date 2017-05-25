#pragma once

#include <vector>

#include "IArrayLogger.h"

using namespace std;

class ArrayLogger : public IArrayLogger
{
public:
	ArrayLogger(char* name);
	void addValue(double val);
	const char* getContent();
	virtual const char* getName();
private:
	vector<double> content;
	string name;
};