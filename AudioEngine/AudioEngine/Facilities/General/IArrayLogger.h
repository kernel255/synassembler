#pragma once

class IArrayLogger
{
	virtual void addValue(double val) = 0;
	virtual const char* getContent() = 0;
	virtual const char* getName() = 0;
};