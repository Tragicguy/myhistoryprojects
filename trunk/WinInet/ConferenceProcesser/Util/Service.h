// Service Manager
// Code by Pan Peng 2010-05-04

#pragma once

#include <atlbase.h>
#include <winsvc.h>
#include "Callback.h"
#include <atlcom.h>
#pragma warning(disable:4996)
class CServiceBase
{
public:
	CServiceBase(void);
	~CServiceBase(void);
public:
	static bool Install(LPCTSTR lpFilePathName, LPCTSTR lpServiceName, DWORD dwStartType = SERVICE_AUTO_START);
	static bool Uninstall(LPCTSTR lpName);
	static bool Isinstalled(LPCTSTR lpName);
	static bool SetServiceStatus(LPCTSTR lpName, DWORD dwStatus = SERVICE_CONTROL_STOP);
	static bool GetServiceStatus(LPCTSTR lpName, DWORD &dwStatus);
	static bool RunService(LPCTSTR lpName);
	static bool GetServiceStartType(LPCTSTR lpName, DWORD &dwRunType);
	static bool SetServiceStartType(LPCTSTR lpName, DWORD dwRunType = SERVICE_AUTO_START);
};

class CService : public CServiceBase, public CComModule
{
public:
	CService();
	~CService();

public:
	void Init(HINSTANCE h, LPCTSTR lpName);
	int  Start();
	void ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
	void Handler(DWORD dwOpcode);
	int  Run();
	bool IsInstalled();
	bool Install();
	bool Uninstall();
	LONG Unlock();
	void LogEvent(LPCTSTR pszFormat, ...);
	void SetServiceStatus(DWORD dwState);
	void SetupAsLocalServer();
private:
	static void WINAPI _ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
	static void WINAPI _Handler(DWORD dwOpcode);

private:
	TCHAR m_szServiceName[256];
	SERVICE_STATUS_HANDLE m_hServiceStatus;
	SERVICE_STATUS m_status;
	DWORD dwThreadID;
public:
	bool m_bService;
	Callback<bool, int> m_callback;
	Callback<bool, int> m_servicecallback;
};

extern CService _Module;
