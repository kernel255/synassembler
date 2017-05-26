#pragma once

class IArrayLogger
{
public:
	virtual void addValue(double val) = 0;
	virtual const char* getContent() = 0;
	virtual const char* getName() = 0;
};