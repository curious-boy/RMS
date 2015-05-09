// Log.h: interface for the CLog class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

class CLog  
{
public:
	CLog();
	virtual ~CLog();

	FILE* __fStdOut;
	FILE* __fStdErr;

	int WriteLogf(char* fmt1, long fmt2, char *fmt, ...);//д��־������
	int WriteErr(char *fmt,...);//д��������

#define WriteLog(fmt, ...) WriteLogf(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
	//printf("File: "__FILE__", Line: %05d: "format"/n", __LINE__, ##__VA_ARGS__)
protected:
	void startConsoleWin(char* fLogName,char* fErrName);
	void CloseWin();
	char Pre_Time[9];
};