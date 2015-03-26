#include "stdafx.h"
#include "mdump.h"


LPTOP_LEVEL_EXCEPTION_FILTER CMiniDumper::s_pPrevFilter = 0;
CMiniDumper::CMiniDumper()
{
	assert(!s_pPrevFilter);
	s_pPrevFilter = ::SetUnhandledExceptionFilter(UnhandledExceptionFilter);
}

long CMiniDumper::UnhandledExceptionFilter( struct _EXCEPTION_POINTERS *pExceptionInfo )
{
	long ret = EXCEPTION_CONTINUE_SEARCH;

	TCHAR szDbgHelpPath[_MAX_PATH] = {0};
	TCHAR szDumpPath[_MAX_PATH] = {0};
	TCHAR szPath[_MAX_PATH] = {0};
	if (GetModuleFileName(NULL, szPath, _MAX_PATH))
	{
		TCHAR szDrive[_MAX_DRIVE] = {0};
		TCHAR szDir[_MAX_DIR] = {0};
		TCHAR szFileName[_MAX_FNAME] = {0};
		_tsplitpath(szPath, szDrive, szDir, szFileName, 0);

		_tcsncat(szDbgHelpPath, szDrive, _MAX_PATH);
		_tcsncat(szDbgHelpPath, szDir, _MAX_PATH - _tcslen(szDbgHelpPath) - 1);
		_tcsncat(szDbgHelpPath, _T("dbghelp.dll"), _MAX_PATH - _tcslen(szDbgHelpPath) - 1);

		_tcsncat(szDumpPath, szDrive, _MAX_PATH);
		_tcsncat(szDumpPath, szDir, _MAX_PATH - _tcslen(szDumpPath) - 1);
		_tcsncat(szDumpPath, szFileName, _MAX_PATH - _tcslen(szDumpPath) - 1);
		_tcsncat(szDumpPath, _T(".dmp"), _MAX_PATH - _tcslen(szDumpPath) - 1);
	}

	HMODULE hDll = ::LoadLibrary(szDbgHelpPath);
	if (hDll==NULL)
		hDll = ::LoadLibrary(_T("dbghelp.dll"));
	assert(hDll);

	if (hDll)
	{
		MINIDUMPWRITEDUMP pWriteDumpFun = (MINIDUMPWRITEDUMP)::GetProcAddress(hDll, "MiniDumpWriteDump");
		if (pWriteDumpFun)
		{
			// create the file
			HANDLE hFile = ::CreateFile(szDumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL, NULL);

			if (hFile != INVALID_HANDLE_VALUE)
			{
				_MINIDUMP_EXCEPTION_INFORMATION ExInfo;
				ExInfo.ThreadId = ::GetCurrentThreadId();
				ExInfo.ExceptionPointers = pExceptionInfo;
				ExInfo.ClientPointers = FALSE;

				// write the dump
				if (pWriteDumpFun(GetCurrentProcess(), GetCurrentProcessId(), 
					hFile, MiniDumpNormal, pExceptionInfo!=0? &ExInfo: 0, NULL, NULL))
					ret = EXCEPTION_EXECUTE_HANDLER;
				::CloseHandle(hFile);
			}
		}
	}

	if (s_pPrevFilter)
		ret = s_pPrevFilter(pExceptionInfo);

	return ret;
}

