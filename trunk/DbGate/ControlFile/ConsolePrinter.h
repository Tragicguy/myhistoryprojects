#pragma once
#include <afxmt.h>
#include <afxtempl.h>
class CConsolePrinter     
{   
public:   
	void print(const char* str);   
	static void Destroy();   
	static CConsolePrinter* Instance();
	virtual ~CConsolePrinter();
protected:   
	CConsolePrinter();   
	void PrintEx(const char *str);
private:   
	static CConsolePrinter* _instance;   
	FILE *file;
	long m_iCount;
	CCriticalSection m_PrinterLock;
};