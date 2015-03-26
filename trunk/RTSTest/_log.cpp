#include <stdafx.h>
#include <fstream>
#include <string>
#include <ctime>
#include <clocale>
using namespace std;
#include <stdarg.h>
#include "_log.h"
#include <sys/stat.h>
#include <io.h>
using namespace extension;
//add by shaojun at 2010-08-11 将同步对象定义为全局唯一的，使日志记录能真正同步
//extension::CLocker g_Lock;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Constructor and destructor
event_log::event_log(char* szFile, bool bLogging)
{
	// add by ZhuJie for chinese directory or chinese filename. 2008-06-19.
	setlocale(LC_ALL, "");
	m_strFileName = szFile;
	m_bLogging = bLogging;
	del_hugelog(szFile);
	log_event(event_type_information, "Connection to log file established");
}

void event_log::del_hugelog(char* szFile)
{
	struct _stat buf;
	if(_stat(szFile, &buf) != -1)
	{
		if (buf.st_size > 5 * 1024 * 1024)
		{
			remove(szFile);
			return;
		}
	}
	else
	{
		DWORD dword = GetLastError();
	}
}
event_log::~event_log()
{
	log_event(event_type_information, "Connection to log file closed");
}

void event_log::log_info(const char* szFormat, ...)
{
	try
	{
		va_list arg_list;
		va_start(arg_list, szFormat);
		vlog_event(event_type_information, szFormat, arg_list);
		va_end(arg_list);
	}
	catch (...) {}
}

void event_log::log_warning(const char* szFormat, ...)
{
	try
	{
		va_list arg_list;
		va_start(arg_list, szFormat);
		vlog_event(event_type_warning, szFormat, arg_list);
		va_end(arg_list);
	}
	catch (...) {}
}

void event_log::log_error(const char* szFormat, ...)
{
	try
	{
		va_list arg_list;
		va_start (arg_list, szFormat);
		vlog_event(event_type_error, szFormat, arg_list);
		va_end(arg_list);
	}
	catch (...) {}
}

void event_log::log_event(event_type evtType, const char* szFormat, ...)
{
	if (!m_bLogging) return;
//	CLockMgr<CLocker> Lock(g_Lock);
	static const int iBufferSize = 10240;
	char pBuffer[iBufferSize];
	pBuffer[iBufferSize - 1] = '\0';

	// Print the parameters into a memory buffer
	try
	{
		va_list arg_list;
		va_start(arg_list, szFormat);
		try
		{
			::vsnprintf_s(pBuffer, sizeof(pBuffer) - 1, sizeof(pBuffer) - 1, szFormat, arg_list);
		}
		catch (...) {}
		
		va_end(arg_list);
	}
	catch (...) {}

	try
	{
		// Write data to the log file	
		setlocale(LC_ALL, "Chinese-simplified");
		fstream fsLog(m_strFileName.c_str(), ios_base::out | ios_base::app);
		SYSTEMTIME st;
		GetSystemTime(&st);
		char szTime[30] = {0};
		sprintf_s(szTime,"%04d-%02d-%02d %02d:%02d:%02d:%03d",st.wYear,st.wMonth,st.wDay,st.wHour+8,st.wMinute,st.wSecond,st.wMilliseconds);
		try
		{
			fsLog << endl << szTime << "\t " << event_type_to_string(evtType) << "\t " << pBuffer;
			fsLog.flush();
		}
		catch (...) {}

		fsLog.close();
	}
	catch (...) {}
}

void event_log::enable_logging(bool bLogging)
{
	m_bLogging = bLogging;
}

void event_log::vlog_event(event_type evtType, const char* szFormat, va_list arg_list)
{
	if (!m_bLogging) return;

	//CLockMgr<CLocker> Lock(g_Lock);
	// Print the parameters into a memory buffer
	static const int iBufferSize = 10240;
	char pBuffer[iBufferSize];
	pBuffer[iBufferSize - 1] = '\0';

	try
	{
		::vsnprintf_s(pBuffer, sizeof(pBuffer)-1, -1,szFormat, arg_list);
	//	_snprintf()
	}
	catch (...) {}

	try
	{
		// Write data to the log file
		fstream fsLog(m_strFileName.c_str(), ios_base::out | ios_base::app);
		SYSTEMTIME st;
		GetSystemTime(&st);
		char szTime[30] = {0};
		sprintf_s(szTime,"%04d-%02d-%02d %02d:%02d:%02d:%03d",st.wYear,st.wMonth,st.wDay,st.wHour+8,st.wMinute,st.wSecond,st.wMilliseconds);

		try
		{
			fsLog << endl << szTime << "\t " << event_type_to_string(evtType) << "\t " << pBuffer;
			fsLog.flush();
		}
		catch (...) {}

		fsLog.close();
	}
	catch (...) {}
}

const char* event_log::event_type_to_string(event_type evtType)
{
	static const char* events[] =
	{
		"Error  ",
		"Warning",
		"Info   "
	};
	return events[evtType];
}
