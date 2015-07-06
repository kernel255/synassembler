
#pragma once

#include <string>

enum e_PropertyType {
	C_Integer = 0,
	C_Double = 1,
	C_String = 2,
	C_StringArray = 3,
	C_MIDINote = 4,
	C_Gain = 5,
	C_Boolean = 6,
	C_None = 100
};

enum e_PropertyDirection {
	C_Input = 0,
	C_Output = 1,
	C_InputOutput = 2
};

typedef void* (*pfnGetValue)(void* eu);
typedef bool (*pfnSetValue)(void* eu, void* value);
// Specialized types callbacks:
// double
typedef double (*pfnGetDValue)(void* eu);
typedef bool (*pfnSetDValue)(void* eu, double);
// int
typedef int(*pfnGetIValue)(void* eu);
typedef bool(*pfnSetIValue)(void* eu, int);


class Property {
public:
	virtual char* getName() = 0;
	virtual char* getDescription() = 0;
	virtual int getRange(char* lowRange, char* highRange, unsigned int bufferSize) = 0;
	virtual e_PropertyType getType() = 0;
	virtual e_PropertyDirection getDirection() = 0;
	// PROPERTIES
	virtual void setGetter(pfnGetValue fGet) = 0;
	virtual void setSetter(pfnSetValue fSet) = 0;
	virtual pfnGetValue getGetter() = 0;
	virtual pfnSetValue getSetter() = 0;
};