#pragma once

#include "Psapi.h"
#pragma comment(lib, "Psapi.lib")

class CProcessManager
{
public:
	CProcessManager(void)
	{
		GetDebugPriv();
	}
	~CProcessManager(void){}
public:
	int FindProcess(TCHAR *strProcessName, DWORD *pPIDList, CStringArray &FileNames)
	{
		BOOL bRes = FALSE;
		int nCount = 0;
		FileNames.RemoveAll();
		DWORD aProcesses[1024], cbNeeded, cbMNeeded;
		HMODULE hMods[1024];
		HANDLE hProcess;
		TCHAR szProcessName[MAX_PATH];

		if (!EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )  return nCount;
		for(int i=0; i< (int) (cbNeeded / sizeof(DWORD)); i++)
		{
			hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);
			EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbMNeeded);
			GetModuleFileNameEx( hProcess, hMods[0], szProcessName,sizeof(szProcessName));
			
#ifndef UNICODE
			if(strstr(szProcessName, strProcessName))
#else
			if(wcsstr(szProcessName, strProcessName))
#endif
			{
				//_tprintf(_T("%s;"), szProcessName);
				CString strProcessName = szProcessName;
				FileNames.Add(strProcessName);
				pPIDList[nCount] = aProcesses[i];
				nCount ++;
				//return(aProcesses[i]);
			}
			//_tprintf(_T("\n"));
		}
		return nCount;
	}
	
	BOOL KillProcess(DWORD dwProcessId)
	{
		if(dwProcessId == 0)
			return TRUE;

		HANDLE hYourTargetProcess = OpenProcess(PROCESS_QUERY_INFORMATION |   // Required by Alpha
			PROCESS_CREATE_THREAD     |   // For CreateRemoteThread
			PROCESS_VM_OPERATION      |   // For VirtualAllocEx/VirtualFreeEx
			PROCESS_VM_WRITE		  |   // For WriteProcessMemory
			PROCESS_TERMINATE,             
			FALSE, dwProcessId);

		if(hYourTargetProcess == NULL)
		{
			return TRUE;
		}
		DWORD lpExitCode;
		BOOL bResult;
		bResult = GetExitCodeProcess(hYourTargetProcess, &lpExitCode);
		bResult = TerminateProcess(hYourTargetProcess, lpExitCode);
		return bResult;
	}

	BOOL GetDebugPriv()
	{
		HANDLE hToken;
		LUID sedebugnameValue;
		TOKEN_PRIVILEGES tkp;

		if(!OpenProcessToken( GetCurrentProcess(),
			TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken ) )
		{
			return FALSE;
		}


		if(!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue ) )
		{
			CloseHandle( hToken );
			return FALSE;
		}

		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Luid = sedebugnameValue;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		if(!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL ) )
		{
			CloseHandle(hToken );
			return FALSE;
		}
		return TRUE;
	}
public:
	BOOL KillProcess(TCHAR *strProcessName)
	{
		DWORD dwPIdList[MAX_PATH];
		CStringArray FileNames;
		int nCount = FindProcess(strProcessName, dwPIdList, FileNames);
		//如果进程不存在，则直接返回TRUE，以免造成杀掉进程失败的假象
		if (nCount < 1) return TRUE;

		DWORD dwMyId = ::GetCurrentProcessId();

		if (nCount > 0)
		{
			for (int i = 0; i < nCount; i ++)
			{
				if (dwMyId != dwPIdList[i])
					KillProcess(dwPIdList[i]);
			}
			return TRUE;
		}
		return FALSE;
	}

	BOOL KillSameProcess(TCHAR *strProcessName)
	{
		DWORD dwPIdList[MAX_PATH];
		CStringArray FileNames;
		int nCount = FindProcess(strProcessName, dwPIdList, FileNames);
		if (nCount < 1) return FALSE;

		DWORD dwMyId = ::GetCurrentProcessId();
		char szInstance[MAX_PATH];
		ZeroMemory(szInstance, MAX_PATH);
		GetModuleFileName(NULL, szInstance, MAX_PATH);
		CString strInstance = szInstance;
		strInstance.MakeLower();
		strInstance.Trim();

		if (nCount > 0)
		{
			for (int i = 0; i < nCount; i ++)
			{
				CString strFileName = FileNames[i];
				if (strFileName.IsEmpty()) continue;
				strFileName.MakeLower();
				strFileName.Trim();

				if (strFileName == strInstance)
				{
					if (dwMyId != dwPIdList[i])
					{
#ifdef _DEBUG
						CString strMsg = "Kill Same Process ";
						strMsg = strMsg + strFileName;
						AfxMessageBox(strMsg);
#endif
						KillProcess(dwPIdList[i]);
					}
				}
			}
			return TRUE;
		}
		return FALSE;
	}
};
