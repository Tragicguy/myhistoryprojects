#include "stdafx.h"
#include "MyServiceClass.h"
#include <winsvc.h>

CMyServiceClass _Module;
/************************************************************************/
/*                                                                      */
/************************************************************************/
CMyServiceClass::CMyServiceClass(void)
{
	memset(m_szServiceName,0,sizeof(m_szServiceName));
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CMyServiceClass::~CMyServiceClass(void)
{
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CMyServiceClass::Install()
{
	if (IsInstalled())
	{
		return TRUE;
	}
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM == NULL)
	{
		MessageBox(NULL, _T("Couldn't open service manager"), m_szServiceName, MB_OK);
		return FALSE;
	}
	// Get the executable file path
	TCHAR szFilePath[_MAX_PATH];
	::GetModuleFileName(NULL, szFilePath, _MAX_PATH);

	SC_HANDLE hService = ::CreateService(
		hSCM, m_szServiceName, m_szServiceName,
		SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS,
		SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL,
		szFilePath, NULL, NULL, _T("RPCSS\0"), NULL, NULL);

	if (hService == NULL)
	{
		::CloseServiceHandle(hSCM);
		MessageBox(NULL, _T("Couldn't create service"), m_szServiceName, MB_OK);
		return FALSE;
	}
	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);
	return TRUE;
}

BOOL CMyServiceClass::Uninstall()
{
	if (!IsInstalled())
		return TRUE;

	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM == NULL)
	{
		MessageBox(NULL, _T("Couldn't open service manager"), m_szServiceName, MB_OK);
		return FALSE;
	}

	SC_HANDLE hService = ::OpenService(hSCM, m_szServiceName, SERVICE_STOP | DELETE);
	if (hService == NULL)
	{
		::CloseServiceHandle(hSCM);
		MessageBox(NULL, _T("Couldn't open service"), m_szServiceName, MB_OK);
		return FALSE;
	}
	SERVICE_STATUS status;
	::ControlService(hService, SERVICE_CONTROL_STOP, &status);

	BOOL bDelete = ::DeleteService(hService);
	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);
	if (bDelete)
		return TRUE;
	MessageBox(NULL, _T("Service could not be deleted"), m_szServiceName, MB_OK);
	return FALSE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CMyServiceClass::IsInstalled()
{
	BOOL bResult = FALSE;
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM != NULL)
	{
		SC_HANDLE hService = ::OpenService(hSCM, m_szServiceName, SERVICE_QUERY_CONFIG);
		if (hService != NULL)
		{
			bResult = TRUE;
			::CloseServiceHandle(hService);
		}
		::CloseServiceHandle(hSCM);
	}
	return bResult;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMyServiceClass::SetServiceName(CString strName)
{
	strncpy(m_szServiceName,strName,255);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
inline void CMyServiceClass::Init(_ATL_OBJMAP_ENTRY* p, HINSTANCE h, UINT nServiceNameID, const GUID* plibid)
{
	//CComModule::Init(p, h, plibid);

	m_bService = TRUE;
	LoadString(h, nServiceNameID, m_szServiceName, sizeof(m_szServiceName) / sizeof(TCHAR));

	// set up the initial service status 
	m_hServiceStatus = NULL;
	m_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	m_status.dwCurrentState = SERVICE_STOPPED;
	m_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	m_status.dwWin32ExitCode = 0;
	m_status.dwServiceSpecificExitCode = 0;
	m_status.dwCheckPoint = 0;
	m_status.dwWaitHint = 0;
}

LONG CMyServiceClass::Unlock()
{
	LONG l = CComModule::Unlock();
	if (l == 0 && !m_bService)
		PostThreadMessage(dwThreadID, WM_QUIT, 0, 0);
	return l;
}

void CMyServiceClass::LogEvent(LPCTSTR pFormat, ...)
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
inline void CMyServiceClass::Start()
{
	SERVICE_TABLE_ENTRY st[] =
	{
		{ m_szServiceName, _ServiceMain },
		{ NULL, NULL }
	};
	if (m_bService && !::StartServiceCtrlDispatcher(st))
	{
		m_bService = FALSE;
	}
	if (m_bService == FALSE)
		Run();
}

inline void CMyServiceClass::ServiceMain(DWORD /* dwArgc */, LPTSTR* /* lpszArgv */)
{
	// Register the control request handler
	m_status.dwCurrentState = SERVICE_START_PENDING;
	m_hServiceStatus = RegisterServiceCtrlHandler(m_szServiceName, _Handler);
	if (m_hServiceStatus == NULL)
	{
		LogEvent(_T("Handler not installed"));
		return;
	}
	SetServiceStatus(SERVICE_START_PENDING);

	m_status.dwWin32ExitCode = S_OK;
	m_status.dwCheckPoint = 0;
	m_status.dwWaitHint = 0;

	// When the Run function returns, the service has stopped.
	Run();

	SetServiceStatus(SERVICE_STOPPED);
	LogEvent(_T("Service stopped"));
}

inline void CMyServiceClass::Handler(DWORD dwOpcode)
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

void WINAPI CMyServiceClass::_ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
	_Module.ServiceMain(dwArgc, lpszArgv);
}
void WINAPI CMyServiceClass::_Handler(DWORD dwOpcode)
{
	_Module.Handler(dwOpcode); 
}

void CMyServiceClass::SetServiceStatus(DWORD dwState)
{
	m_status.dwCurrentState = dwState;
	::SetServiceStatus(m_hServiceStatus, &m_status);
}

void CMyServiceClass::Run()
{
	_Module.dwThreadID = GetCurrentThreadId();
	LogEvent(_T("Service started"));
	if (m_bService)
		SetServiceStatus(SERVICE_RUNNING);

	MSG msg;
	while (GetMessage(&msg, 0, 0, 0))
		DispatchMessage(&msg);
}
