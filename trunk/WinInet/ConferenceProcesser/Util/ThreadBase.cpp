#include "stdafx.h"
#include "ThreadBase.h"
#include <process.h>

//#include "GlobalFun.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



CThreadBase::CThreadBase()
{
	m_hThread = NULL;
	SetRunning(false);
	m_uiThreadId = 0;
}


CThreadBase::~CThreadBase()
{
	StopWorking();
}

bool CThreadBase::StartWorking()
{
	//CThreadLocker lock(&m_locker);
	HANDLE hThread = NULL;

	SetRunning();

	hThread = (HANDLE)_beginthreadex(NULL,	
		0,	
		StartThread,	
		this,	
		CREATE_SUSPENDED, 	
		&m_uiThreadId);	
	
	if (NULL != hThread)
	{
		DWORD dwResult = ResumeThread( hThread );   //当线程挂起时，唤醒。
		if (dwResult == -1)
		{
			DWORD dwError = GetLastError();
			
			SetRunning(false);
			return false;
		}
		m_hThread = hThread;

		return true;
	}
	SetRunning(false);
	return false;
}

HANDLE CThreadBase::StartWorkingEx()
{
	SetRunning();
	HANDLE hThread = (HANDLE)_beginthreadex(NULL,	
		0,	
		StartThread,	
		this,	
		CREATE_SUSPENDED, 	
		&m_uiThreadId);	

	if (NULL != hThread)
	{
		DWORD dwResult = ResumeThread( hThread );   //当线程挂起时，唤醒。
		if (dwResult == -1)
		{
			DWORD dwError = GetLastError();			
			return NULL;
		}
		return hThread;
	}
	return NULL;
}

void CThreadBase::SetRunning(bool bRunning/* = true*/)
{
	//CThreadLocker lock(&m_locker);
	m_bRunning = bRunning;
}

void CThreadBase::StopWorking(HANDLE threadHandle/* = NULL*/, DWORD dwWait/* = 3000*/)
{
	MarkTheadToStop();
	HANDLE handle = NULL;
	if(threadHandle == NULL)
	{
		handle = m_hThread;
	}
	else
	{
		handle = threadHandle;
	}
	if(handle != NULL)
	{
		DWORD dwExitCode;
		bool bExit = false;
		DWORD tOld = ::GetTickCount();
		DWORD tNow = ::GetTickCount();

		while ((tNow - tOld) < dwWait)
		{
			if(GetExitCodeThread(handle,&dwExitCode))
			{
				if(dwExitCode == STILL_ACTIVE)
				{
					Sleep(30);
				}
				else
				{
					bExit = true;
					break;
				}
			}
			tNow = ::GetTickCount();
		}
		
		if(bExit == false)
		{
			//We have been waiting for 5 seconds and the thread is still running
			//Then we have to terminate the thread manually, sorry, thread.
			TerminateThread(handle, WAIT_TIMEOUT);
		}

		CloseHandle(handle);
		handle = NULL;
		m_hThread = NULL;
	}

	return;
}

void CThreadBase::ThreadLoop()
{
	while(m_bRunning == true)
	{
		ThreadProc();
	}

	OnThreadStop();   // 无操作。
	return;
}

UINT WINAPI CThreadBase::StartThread(LPVOID pParam)
{
	CThreadBase* pSource = (CThreadBase*)pParam;
	pSource->ThreadLoop();
	return 0;
}


void CThreadBase::ThreadProc()
{
	return;
}

void CThreadBase::MarkTheadToStop()
{
	//OutputDebugString("CThreadBase::MarkTheadToStop()\n");
	SetRunning(false);
}

void CThreadBase::OnThreadStop()
{

}
