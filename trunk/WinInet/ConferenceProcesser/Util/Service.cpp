#include "stdafx.h"
#include "Service.h"

extern CService _Module;

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()

CServiceBase::CServiceBase(void)
{
}

CServiceBase::~CServiceBase(void)
{
}

bool CServiceBase::Install(LPCTSTR lpFilePathName, LPCTSTR lpServiceName, DWORD dwStartType/* = SERVICE_AUTO_START*/)
{
	if (Isinstalled(lpServiceName))
		return true;

	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM == NULL)
		return false;

	SC_HANDLE hService = CreateService( 
		hSCM,					/* SCManager database      */ 
		lpServiceName,			/* name of service         */ 
		lpServiceName,			/* service name to display */ 
		SERVICE_ALL_ACCESS,		/* desired access          */ 
		SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS , /* service type            */ 
		dwStartType,			/* start type              */ 
		SERVICE_ERROR_NORMAL,	/* error control type      */ 
		lpFilePathName,			/* service's binary        */ 
		NULL,					/* no load ordering group  */ 
		NULL,					/* no tag identifier       */ 
		NULL,					/* no dependencies         */ 
		NULL,					/* LocalSystem account     */ 
		NULL
		);

	if (hService == NULL)
	{
		::CloseServiceHandle(hSCM);
		return false;
	}

	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);

	return true;
}

bool CServiceBase::Uninstall(LPCTSTR lpName)
{
	if (!Isinstalled(lpName))
		return true;

	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM == NULL)
	{
		return false;
	}

	SC_HANDLE hService = ::OpenService(hSCM, lpName, SERVICE_STOP | DELETE);

	if (hService == NULL)
	{
		::CloseServiceHandle(hSCM);
		return false;
	}

	SERVICE_STATUS status;
	::ControlService(hService, SERVICE_CONTROL_STOP, &status);

	BOOL bDelete = ::DeleteService(hService);
	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);

	return bDelete ? true : false;
}

bool CServiceBase::Isinstalled(LPCTSTR lpName)
{
	bool bResult = false;

	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM != NULL)
	{
		SC_HANDLE hService = ::OpenService(hSCM, lpName, SERVICE_QUERY_CONFIG);
		if (hService != NULL)
		{
			bResult = true;
			::CloseServiceHandle(hService);
		}
		::CloseServiceHandle(hSCM);
	}
	return bResult;
}

bool CServiceBase::SetServiceStatus(LPCTSTR lpName, DWORD dwStatus/* = SERVICE_CONTROL_STOP*/) 
{ 
	// kill service with given name
	SC_HANDLE schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS); 
	if (schSCManager==0) 
	{
		long nError = GetLastError();
		char pTemp[121];
		sprintf(pTemp, "OpenSCManager failed, error code = %d\n", nError);
	}
	else
	{
		// open the service
		SC_HANDLE schService = OpenService( schSCManager, lpName, SERVICE_ALL_ACCESS);
		if (schService==0) 
		{
			long nError = GetLastError();
			char pTemp[121];
			sprintf(pTemp, "OpenService failed, error code = %d\n", nError);
		}
		else
		{
			// call ControlService to kill the given service
			SERVICE_STATUS status;
			if(::ControlService(schService, dwStatus, &status))
			{
				CloseServiceHandle(schService); 
				CloseServiceHandle(schSCManager); 
				return true;
			}
			else
			{
				long nError = GetLastError();
				char pTemp[121];
				sprintf(pTemp, "ControlService failed, error code = %d\n", nError);
				
			}
			CloseServiceHandle(schService); 
		}
		CloseServiceHandle(schSCManager); 
	}
	return false;
}

bool CServiceBase::GetServiceStatus(LPCTSTR lpName, DWORD &dwStatus)
{
	DWORD dwstatus = -1;
	SC_HANDLE schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS); 
	if (schSCManager == NULL) 
	{
		long nError = GetLastError();
		char pTemp[121];
		sprintf(pTemp, "OpenSCManager(GetServiceStatus) failed, error code = %d\n", nError);
		//WriteLog(pTemp);
	}
	else
	{
		SC_HANDLE schService = OpenService(schSCManager, lpName, SERVICE_ALL_ACCESS);
		if (schService == NULL) 
		{
			long nError = GetLastError();
			char pTemp[121];
			sprintf(pTemp, "OpenService(GetServiceStatus) failed, error code = %d\n", nError);
			//WriteLog(pTemp);
		}
		else
		{
			SERVICE_STATUS ss; 

			if(::QueryServiceStatus(schService, &ss)) 
			{ 
				dwstatus = ss.dwCurrentState;
				CloseServiceHandle(schService);  
			}
			CloseServiceHandle(schSCManager);
		}
	}

	dwStatus = dwstatus;

	return dwstatus != -1;
}

bool CServiceBase::RunService(LPCTSTR lpName) 
{ 
	// run service with given name
	SC_HANDLE schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS); 
	if (schSCManager==0) 
	{
		long nError = GetLastError();
		char pTemp[121];
		sprintf(pTemp, "OpenSCManager failed, error code = %d\n", nError);
		//WriteLog(pTemp);
	}
	else
	{
		// open the service
		SC_HANDLE schService = OpenService( schSCManager, lpName, SERVICE_ALL_ACCESS);
		if (schService==0) 
		{
			long nError = GetLastError();
			char pTemp[121];
			sprintf(pTemp, "OpenService failed, error code = %d\n", nError);
			//WriteLog(pTemp);
		}
		else
		{
			// call StartService to run the service
			if(StartService(schService, 0, (const char**)NULL))
			{
				CloseServiceHandle(schService); 
				CloseServiceHandle(schSCManager); 
				return true;
			}
			else
			{
				long nError = GetLastError();
				char pTemp[121];
				sprintf(pTemp, "StartService failed, error code = %d\n", nError);
				//WriteLog(pTemp);
			}
			CloseServiceHandle(schService); 
		}
		CloseServiceHandle(schSCManager); 
	}
	return false;
}

bool CServiceBase::GetServiceStartType(LPCTSTR lpName, DWORD &dwStartType)
{
	bool bRes = false;
	DWORD dwStatus = 0;
	SC_HANDLE schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS); 
	if (schSCManager == NULL) 
	{
		long nError = GetLastError();
		char pTemp[121];
		sprintf(pTemp, "OpenSCManager(GetServiceStatus) failed, error code = %d\n", nError);
		//WriteLog(pTemp);
	}
	else
	{
		SC_HANDLE schService = OpenService(schSCManager, lpName, SERVICE_ALL_ACCESS);
		if (schService == NULL) 
		{
			long nError = GetLastError();
			char pTemp[121];
			sprintf(pTemp, "OpenService(GetServiceStatus) failed, error code = %d\n", nError);
			//WriteLog(pTemp);
		}
		else
		{
			DWORD dwNeeded = 0;
			LPQUERY_SERVICE_CONFIG config = (LPQUERY_SERVICE_CONFIG)LocalAlloc(LPTR, 4096);  
			BOOL isSuccess = QueryServiceConfig(schService, config, 4096, &dwNeeded);
			if (isSuccess)
			{
				dwStartType = config->dwStartType; //SERVICE_AUTO_START SERVICE_BOOT_START
				bRes = true;
			}
			LocalFree((HLOCAL)config);
			CloseServiceHandle(schService);
		}

		CloseServiceHandle(schSCManager);
	}

	return bRes;
}

bool CServiceBase::SetServiceStartType(LPCTSTR lpName, DWORD dwStartType/* = SERVICE_AUTO_START*/)
{
	bool bRes = false;
	DWORD dwStatus = 0;
	SC_HANDLE schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS); 
	if (schSCManager == NULL) 
	{
		long nError = GetLastError();
		char pTemp[121];
		sprintf(pTemp, "OpenSCManager(GetServiceStatus) failed, error code = %d\n", nError);
		//WriteLog(pTemp);
	}
	else
	{
		SC_HANDLE schService = OpenService(schSCManager, lpName, SERVICE_ALL_ACCESS);
		if (schService == NULL) 
		{
			long nError = GetLastError();
			char pTemp[121];
			sprintf(pTemp, "OpenService(GetServiceStatus) failed, error code = %d\n", nError);
			//WriteLog(pTemp);
		}
		else
		{
			DWORD dwNeeded = 0;
			LPQUERY_SERVICE_CONFIG config = (LPQUERY_SERVICE_CONFIG)LocalAlloc(LPTR, 4096);  
			BOOL isSuccess = QueryServiceConfig(schService, config, 4096, &dwNeeded);
			if (isSuccess)
			{
				if (config->dwStartType != dwStartType)
				{
					isSuccess = ChangeServiceConfig(schService, SERVICE_NO_CHANGE, dwStartType, 
						SERVICE_NO_CHANGE, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
					bRes = isSuccess == TRUE;
				}
			}

			LocalFree((HLOCAL)config);

			CloseServiceHandle(schService);
		}

		CloseServiceHandle(schSCManager);
	}

	return bRes;
}

CService::CService()
{

}

CService::~CService()
{

}

void CService::Init(HINSTANCE h, LPCTSTR lpName)
{

	m_bService = TRUE;
	lstrcpy(m_szServiceName, lpName);

	m_hServiceStatus = NULL;
	m_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	m_status.dwCurrentState = SERVICE_STOPPED;
	m_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	m_status.dwWin32ExitCode = 0;
	m_status.dwServiceSpecificExitCode = 0;
	m_status.dwCheckPoint = 0;
	m_status.dwWaitHint = 0;
}

LONG CService::Unlock()
{
	LONG l = CComModule::Unlock();

	if (l == 0 && !m_bService)
		PostThreadMessage(dwThreadID, WM_QUIT, 0, 0);

	return l;
}

bool CService::Install()
{
	TCHAR szPathName[MAX_PATH] = {0};
	GetModuleFileName(NULL, szPathName, MAX_PATH*sizeof(TCHAR));
	return CServiceBase::Install(szPathName, m_szServiceName) == true;
}

bool CService::Uninstall()
{
	return CServiceBase::Uninstall(m_szServiceName) == true;
}

///////////////////////////////////////////////////////////////////////////////////////
// Logging functions
void CService::LogEvent(LPCTSTR pFormat, ...)
{
	TCHAR    chMsg[256];
	HANDLE  hEventSource;
	LPTSTR  lpszStrings[1];
	va_list pArg;

	va_start(pArg, pFormat);
	_vstprintf(chMsg, pFormat, pArg);
	va_end(pArg);

	lpszStrings[0] = chMsg;

	if (m_bService)
	{
		/* Get a handle to use with ReportEvent(). */
		hEventSource = RegisterEventSource(NULL, m_szServiceName);
		if (hEventSource != NULL)
		{
			/* Write to event log. */
			ReportEvent(hEventSource, EVENTLOG_INFORMATION_TYPE, 0, 0, NULL, 1, 0, (LPCTSTR*) &lpszStrings[0], NULL);
			DeregisterEventSource(hEventSource);
		}
	}
	else
	{
		// As we are not running as a service, just write the error to the console.
		_putts(chMsg);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Service startup and registration
int  CService::Start()
{
	int nRet = 0;
	
	DWORD dwStatus = -1;
	
	if (GetServiceStatus(m_szServiceName,dwStatus))
	{
		if (dwStatus == SERVICE_STOPPED || dwStatus == SERVICE_RUNNING || dwStatus == SERVICE_CONTINUE_PENDING)
			m_bService = false;
	}

	if (m_bService)
	{
		SERVICE_TABLE_ENTRY st[] =
		{
			{ m_szServiceName, _ServiceMain },
			{ NULL, NULL }
		};
		if (!::StartServiceCtrlDispatcher(st))
			m_bService = FALSE;
	}
	if (m_bService == FALSE)
		nRet = Run();

	return nRet;
}

void CService::ServiceMain(DWORD /* dwArgc */, LPTSTR* /* lpszArgv */)
{
	// Register the control request handler
	m_status.dwCurrentState = SERVICE_START_PENDING;
	m_hServiceStatus = RegisterServiceCtrlHandler(m_szServiceName, _Handler);
	if (m_hServiceStatus == NULL)
	{
		LogEvent(_T("Handler not installed"));
		return;
	}

	if (m_servicecallback.IsEnableCallback())
		m_servicecallback.call(0);

	SetServiceStatus(SERVICE_START_PENDING);

	m_status.dwWin32ExitCode = S_OK;
	m_status.dwCheckPoint = 0;
	m_status.dwWaitHint = 0;

	// When the Run function returns, the service has stopped.
	int nRet = Run();

	SetServiceStatus(SERVICE_STOPPED);
	CString strLog;
	strLog.Format("Service stopped, return %d", nRet);
	LogEvent(strLog);
}

void CService::Handler(DWORD dwOpcode)
{
	switch (dwOpcode)
	{
	case SERVICE_CONTROL_STOP:
		SetServiceStatus(SERVICE_STOP_PENDING);
		PostThreadMessage(dwThreadID, WM_QUIT, 0, 0);
		break;
	case SERVICE_CONTROL_PAUSE:
		break;
	case SERVICE_CONTROL_CONTINUE:
		break;
	case SERVICE_CONTROL_INTERROGATE:
		break;
	case SERVICE_CONTROL_SHUTDOWN:
		break;
	default:
		LogEvent(_T("Bad service request"));
	}
}

void WINAPI CService::_ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
	_Module.ServiceMain(dwArgc, lpszArgv);
}
void WINAPI CService::_Handler(DWORD dwOpcode)
{
	_Module.Handler(dwOpcode); 
}

void CService::SetServiceStatus(DWORD dwState)
{
	m_status.dwCurrentState = dwState;
	::SetServiceStatus(m_hServiceStatus, &m_status);
}

int CService::Run()
{
	int nRet = 0;
	_Module.dwThreadID = GetCurrentThreadId();
	LogEvent(_T("Service started"));
	if (m_bService)
		SetServiceStatus(SERVICE_RUNNING);

	if (m_callback.IsEnableCallback())
	{
		LogEvent(_T("Service run callback."));
		nRet = m_callback.call(0);
	}

	LogEvent(_T("Service run end."));
	return nRet;
}

