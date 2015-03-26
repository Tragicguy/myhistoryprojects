#include "StdAfx.h"
#include ".\consoleprinter.h"
#include <fcntl.h>
#include <io.h>
#include <iostream>
CConsolePrinter* CConsolePrinter::_instance = 0;   
/************************************************************************/
/*                                                                      */
/************************************************************************/
CConsolePrinter::CConsolePrinter()
{
	// create a new console to the process
	AllocConsole();
	int hCrun;
	hCrun = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
	file  = _fdopen(hCrun, "w");
	setvbuf(file, NULL, _IONBF, 0);
	*stdout = *file;
	m_iCount = 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CConsolePrinter::~CConsolePrinter()   
{   
	FreeConsole();
	fclose(file);
}   
/************************************************************************/
/*                                                                      */
/************************************************************************/
CConsolePrinter* CConsolePrinter::Instance()
{
	if (_instance == 0)
	{
		_instance = new CConsolePrinter;
	}
	return _instance;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CConsolePrinter::Destroy()   
{
	if (_instance)
	{
		delete _instance;
	}
	_instance = 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CConsolePrinter::print(const char *str)
{
	m_PrinterLock.Lock();
	m_iCount++;
#ifdef _DEBUG
	std::cout << str << std::endl;
#endif
	m_PrinterLock.Unlock();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CConsolePrinter::PrintEx(const char *str)
{
	m_PrinterLock.Lock();
	m_iCount++;
	std::cout << str << std::endl;
	m_PrinterLock.Unlock();
}

