#include "StdAfx.h"
#include "ThreadShell.h"
#include "GlobalFun.h"

#include "FileClientDialog.h"
#include "FileServerDlg.h"

CThreadShell::CThreadShell()
{
	m_hWnd = NULL;

	m_nPort = 0;
	m_bServer = false;
	m_bGetRemoteFile = false;
	m_MsgNotifyWnd = NULL;
	m_strLanguageId = _T("2052");
	m_bExit = false;
	memset(m_ServerAddress, 0, MAX_PATH);
	m_pDlg = NULL;
}

CThreadShell::~CThreadShell(void)
{
}

bool CThreadShell::CreateFileClient(char* pServerAddress, int nPort, HWND MSGNotifyWnd, int nQuitMessageID, bool bGetRemoteFile)
{
	try
	{
		m_nPort = nPort;
		m_bServer = false;
		m_bGetRemoteFile = bGetRemoteFile;
		m_MsgNotifyWnd = MSGNotifyWnd;
		lstrcpy(m_ServerAddress, pServerAddress);
		m_nQuitMessageID = nQuitMessageID;
		bool bRes = StartWorking();
		int nRetry = 0;
		while(!bRes && nRetry < 3)
		{
			Sleep(300);
			nRetry ++;
			bRes = StartWorking();
		}
		m_bExit = false;
		return bRes;
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CThreadShell::CreateFileClient unkown exception.");
		//OutputDebugString("CThreadShell::CreateFileClient unkown exception.");
	}
	return false;
}


bool CThreadShell::CreateFileServer(int nPort, HWND MSGNotifyWnd, int nQuitMessageID, LPCTSTR strLanguageId, bool bAutoControl)
{
	try
	{
		m_nPort = nPort;
		m_bServer = true;
		m_MsgNotifyWnd = MSGNotifyWnd;
		m_strLanguageId = strLanguageId;
		gl_strLanguageId = m_strLanguageId;
		gl_bAutoControl = bAutoControl;
		m_nQuitMessageID = nQuitMessageID;
		
		bool bRes = StartWorking();
		int nRetry = 0;
		while(!bRes && nRetry < 3)
		{
			Sleep(300);
			nRetry ++;
			bRes = StartWorking();
		}
		return bRes;
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CThreadShell::CreateFileServer unkown exception.");
		//OutputDebugString("CThreadShell::CreateFileServer unkown exception.");
	}
	return false;
}

void CThreadShell::Stop()
{
	try
	{	
		if (m_bExit)
		{
			return;
		}
		MarkTheadToStop();
		if (IsWindow(m_hWnd))
		{		
			PostMessage(m_hWnd, WM_COMMAND, IDOK, 0);
			if (gl_pLogger)
				gl_pLogger->log_info("PostMessage m_hWnd.");			
			DWORD dwWait = ::WaitForSingleObject(m_hThread, 3500);
			if (dwWait == WAIT_TIMEOUT)
			{
				if (m_pDlg)
				{
					m_pDlg->EndDialog(0);
				}
				m_pDlg = NULL;
				if (gl_pLogger)
					gl_pLogger->log_info("CThreadShell::Stop() WAIT_TIMEOUT.");
				::TerminateThread(m_hThread, 0);
			}
		}
		else
		{
			if (m_pDlg)
			{
				m_pDlg->EndDialog(0);
			}
			m_pDlg = NULL;
			::TerminateThread(m_hThread, 0);
		}
		if (gl_pLogger)
			gl_pLogger->log_info("CThreadShell::Stop() return.");
	}
	catch(...){}
}

void CThreadShell::OnThreadStop()
{
	if (gl_pLogger)
		gl_pLogger->log_info("CThreadShell::OnThreadStop()");
}

void CThreadShell::ThreadProc()
{
	try
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CThreadShell::ThreadProc()");

		MarkTheadToStop();
		m_pDlg = NULL;
		if (m_bServer)
		{
			CFileServerDlg FileDialog;
			FileDialog.SetServerDlgInfo(m_nPort, m_MsgNotifyWnd, m_nQuitMessageID, m_strLanguageId);
			FileDialog.SetThreadShell(this);
			m_pDlg = &FileDialog;
			FileDialog.DoModal();
		}
		else
		{
			CFileClientDialog FileDialog;
			FileDialog.SetClientDlgInfo(m_ServerAddress, m_nPort, m_MsgNotifyWnd, m_nQuitMessageID, m_bGetRemoteFile);
			FileDialog.SetThreadShell(this);
			m_pDlg = &FileDialog;
			FileDialog.DoModal();
		}
		m_pDlg = NULL;
		if (gl_pLogger)
			gl_pLogger->log_info("CThreadShell::ThreadProc() end.");
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CThreadShell::ThreadProc() unkown exception.");
	}
}