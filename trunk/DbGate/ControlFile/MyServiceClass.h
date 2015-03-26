#pragma once
#include "../StdAfx.h"
#include <atlbase.h>
#include <winsvc.h>
class CMyServiceClass
{
public:
	CMyServiceClass(void);
public:
	~CMyServiceClass(void);
	BOOL Install();
	BOOL Uninstall();
	BOOL IsInstalled();
	void SetServiceName(CString strName);
	void Run();
	void SetServiceStatus(DWORD dwState);
	void WINAPI _Handler(DWORD dwOpcode);
	void WINAPI _ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
	inline void Handler(DWORD dwOpcode);
	inline void ServiceMain(DWORD /* dwArgc */, LPTSTR* /* lpszArgv */);
	inline void Start();
	void LogEvent(LPCTSTR pFormat, ...);
	inline void Init(_ATL_OBJMAP_ENTRY* p, HINSTANCE h, UINT nServiceNameID, const GUID* plibid);
    LONG Unlock();
	char m_szServiceName[255];
	SERVICE_STATUS_HANDLE m_hServiceStatus;
	SERVICE_STATUS m_status;
	DWORD dwThreadID;
	BOOL m_bService;
};
