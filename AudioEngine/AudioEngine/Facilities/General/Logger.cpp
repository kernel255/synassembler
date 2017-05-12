
#include "Logger.h"

#include <string.h>
#include <wchar.h>
#include <stdio.h>
#include <stdarg.h>
#include <xstring>
#include <wchar.h>
#include <Windows.h>

//Logger::Logger(const char* filename)
Logger::Logger()
{
	//errno_t err = fopen_s(&m_fp, filename, "wt");
	m_Opened = false;
}

int Logger::open(const wchar_t* filename)
{
	m_Filename = filename;
	if(!m_TextFile.Open(filename, TF_WRITE, TF_UTF8))
		m_Opened = true;

	return 0;
}

void Logger::close(void)
{
	//fclose(m_fp);
	m_Opened = false;
	m_TextFile.Close();
}

#define TIME_HDR_LEN 3+3+3+3+2

char* addTime(const char* line)
{
	size_t lineLen = strlen(line) + 1;
	SYSTEMTIME time;
	memset(&time, 0, sizeof(SYSTEMTIME));
	::GetSystemTime(&time);
	char buffer[TIME_HDR_LEN];
	sprintf(buffer, "%02d:%02d:%02d:%03d ", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
	int totLen = lineLen + TIME_HDR_LEN;
	char* buffer2 = new char[totLen];
	strncpy(buffer2, buffer, TIME_HDR_LEN);
	strncpy(buffer2 + TIME_HDR_LEN-1, line, lineLen);

	return buffer2;
}

wchar_t* addTime(const wchar_t* line)
{
	size_t lineLen = wcslen(line);
	SYSTEMTIME time;
	memset(&time, 0, sizeof(SYSTEMTIME));
	::GetSystemTime(&time);
	wchar_t buffer[TIME_HDR_LEN];
	wsprintf(buffer, L"%02d:%02d:%02d:%03d ", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
	int totLen = lineLen + TIME_HDR_LEN;
	wchar_t* buffer2 = new wchar_t[totLen];
	memset(buffer2, 0, totLen);
	wcsncpy(buffer2, buffer, TIME_HDR_LEN);
	wcsncpy(buffer2 + TIME_HDR_LEN - 1, line, lineLen+1);
	//wcsncpy(buffer2 + TIME_HDR_LEN - 1, line, 2);

	return buffer2;
}

void Logger::writeLine(const char* line)
{
	if(!m_Opened)
		return;
	
	char* timedLine = addTime(line);

	//int result = fwrite(line,strlen(line),1,m_fp);
	//fwrite("\n",1,1,m_fp);
	size_t strLen = strlen(timedLine) + 1;
	size_t convertedChars;
	wchar_t wcBuffer[1000];
	//mbstowcs_s(&convertedChars,wcBuffer,strLen,line,_TRUNCATE);
	mbstowcs_s(&convertedChars, wcBuffer, strLen, timedLine, _TRUNCATE);
	delete timedLine;

	m_TextFile.WriteString(NULL, wcBuffer);
	m_TextFile.WriteString(NULL,L"\n");
}

void Logger::writeLine(const wchar_t* line)
{
	if(!m_Opened)
		return;
	//int result = fwrite(line,wcslen(line),1,m_fp);
	//fwrite("\n",1,1,m_fp);
	wchar_t* timedLine = addTime(line);

	m_TextFile.WriteString(NULL, timedLine);
	m_TextFile.WriteString(NULL,L"\n");
}

void Logger::writeLine(char* format, ...)
{
	if(!m_Opened)
		return;

	va_list listPointer;
	va_start(listPointer, format);

	static char buffer[1024];
	memset(buffer,'\0',1024);
	vsprintf(buffer, format, listPointer);
	char* timeLine = addTime(buffer);
	size_t strLen = strlen(timeLine) + 1;
	size_t convertedChars;
	wchar_t wcBuffer[1000];
	mbstowcs_s(&convertedChars, wcBuffer, strLen, timeLine, _TRUNCATE);

	m_TextFile.WriteString(NULL, wcBuffer);
	m_TextFile.WriteString(NULL,L"\n");
	//int result = fwrite(buffer,strlen(buffer),1,m_fp);
	//fwrite("\n",1,1,m_fp);
}


