
#pragma once

#include <stdio.h>
#include <cstring>
#include <string>
#include <xstring>

#include <Windows.h>

#include "textfile.h"

class Logger
{
public:
	Logger();
	int open(const wchar_t* filename);
	void close(void);
	void writeLine(const char* line);
	void writeLine(const wchar_t* line);
	void writeLine(char* format, ...);
	void Flush(void);
	int ReOpen(void);
private:
	FILE* m_fp;
	std::wstring m_Filename;
	TextFile m_TextFile;
	bool m_Opened;
};