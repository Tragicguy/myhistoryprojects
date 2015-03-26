// BaseDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FileTransferLib.h"
#include "BaseDlg.h"
#include "ThreadBase.h"
#include "ThreadShell.h"
#include "FileDefines.h"
#include "GlobalFun.h"

//CEvent gl_StartEvent;

#pragma warning(disable:4800)
// CBaseDlg 对话框

IMPLEMENT_DYNAMIC(CBaseDlg, CDialog)

CBaseDlg::CBaseDlg(LPCTSTR lpszTemplateName, CWnd* pParentWnd/* = NULL*/) : CDialog(lpszTemplateName, pParentWnd)
{
	InitMember();
}

CBaseDlg::CBaseDlg(UINT nIDTemplate, CWnd* pParentWnd/* = NULL*/) : CDialog(nIDTemplate, pParentWnd)
{
	InitMember();
}

void CBaseDlg::InitMember()
{
	m_pThreadShell = NULL;
	m_pControlThread = NULL;

	m_bServer = false;
	m_bGetRemoteFile = false;
	m_MsgNotifyWnd = NULL;
	m_nQuitMessageID = 0;
	m_strLanguageId = _T("2052");

	m_nCtrlPort = 0;
	m_nSendPort = 1234;
	m_nRecevPort = 1235;

	m_strSendFile = _T("");
	m_strSendPath = _T("");
	m_strReceiveFile = _T("");
	m_strReceivePath = _T("");

	m_uSendPos = 0;
	m_uReceivePos = 0;

	memset(m_strServerAddress, 0, MAX_PATH);

	m_pCreateThread = NULL;

	m_bReBuildConnect = false;
	m_bBuildConnect = false;
	m_bHaveExited = false;
	//gl_StartEvent.ResetEvent();
}

CBaseDlg::~CBaseDlg()
{
	
}

void CBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBaseDlg, CDialog)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CBaseDlg 消息处理程序
void CBaseDlg::OnOK()
{
	Stop();
	this->EndDialog(IDOK);
}

void CBaseDlg::Stop()
{
	if (m_bHaveExited)
	{
		try
		{
			if (gl_pLogger)
				gl_pLogger->log_info("CBaseDlg::Stop has already executed");
		}
		catch (...)
		{			
		}
		return;
	}
	if (gl_pLogger)
		gl_pLogger->log_info("CBaseDlg::Stop enter.");
	m_ServerCtrlSocket.StopServerSocket();
	m_ClientCtrlSocket.StopClientSocket();

	//m_SendThread.StopWorking();
	//m_ReceiveThread.StopWorking();
	if (m_pControlThread)
	{
		m_pControlThread->StopWorking();		
	}
	if (m_pThreadShell)
	{
		m_pThreadShell->SetExit(true);
	}
	m_bHaveExited = true;
	m_bBuildConnect = false;

}

BOOL CBaseDlg::BuildConnect(int nPort)
{
	try
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CBaseDlg::BuildConnect(int nPort) Port:%d, m_bBuildConnect:%d", nPort, m_bBuildConnect);
		if (!m_bBuildConnect)
		{
			this->m_nCtrlPort = nPort;
			Stop();
			if (m_pThreadShell)
			{
				m_pThreadShell->SetExit(false);
			}
			m_bHaveExited = false;
			m_bBuildConnect = true;
			return CreateTunnel(CONTROL);
		}
	}
	catch(...){}
	return FALSE;
}

BOOL CBaseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	try
	{
		//gl_StartEvent.SetEvent();
		if (m_pThreadShell)
			m_pThreadShell->SetDlgWnd(this->m_hWnd);
	}
	catch(...){}

	return TRUE;  
}

BOOL CBaseDlg::CreateTunnel(TunnelMode mode)
{
	try
	{

		StopTunnelThread();

		m_ConnectNode.bServer = this->m_bServer;
		m_ConnectNode.hWnd = this->m_hWnd;
		lstrcpy(m_ConnectNode.IpAddress, m_strServerAddress);
		m_ConnectNode.mode = mode;
		switch(mode)
		{
		case CONTROL:
			m_ConnectNode.pClientSocket = &m_ClientCtrlSocket;
			m_ConnectNode.pServerSocket = &m_ServerCtrlSocket;
			m_ConnectNode.nPort = this->m_nCtrlPort;
			break;
		case SEND:
			m_ConnectNode.pClientSocket = &m_ClientSendSocket;
			m_ConnectNode.pServerSocket = &m_ServerSendSocket;
			m_ConnectNode.nPort = this->m_nSendPort;
			break;
		case RECEIVE:
			m_ConnectNode.pClientSocket = &m_ClientReceiveSocket;
			m_ConnectNode.pServerSocket = &m_ServerReceiveSocket;
			m_ConnectNode.nPort = this->m_nRecevPort;
			break;
		}

		m_pCreateThread = AfxBeginThread(MakeTunnelThread, &m_ConnectNode);
		if (gl_pLogger)
			gl_pLogger->log_info("CBaseDlg::CreateTunnel->AfxBeginThread(MakeTunnelThread) %s. mode:%d", m_pCreateThread ? "OK" : "Failed", (int)mode);
		
		return m_pCreateThread != NULL;
	}
	catch(...){}
	return FALSE;
}

void CBaseDlg::StopTunnelThread()
{
	try
	{
		if(m_pCreateThread == NULL)
			return;

		HANDLE hThread = m_pCreateThread->m_hThread;
		DWORD dwExit = ::WaitForSingleObject(hThread, 3000);
		if(WAIT_TIMEOUT == dwExit)
		{
			DWORD dwExitCode = 0;
			::GetExitCodeThread(hThread, &dwExitCode);
			if (dwExitCode == STILL_ACTIVE)
			{
				/*if (gl_pLogger)
					gl_pLogger->log_info("CThreadBase::TerminateThread: %d", m_hThread);*/
				TerminateThread(hThread, dwExitCode);
			}
			//m_pThread = NULL;
		}
		/*if(m_pThread != NULL && m_pThread->m_hThread != NULL && m_hThread != INVALID_HANDLE_VALUE)
			CloseHandle(m_hThread);*/
		//CloseHandle(hThread);
		m_pCreateThread = NULL;
		hThread = NULL;
		m_ConnectNode.bMaking = false;
	}
	catch(...){}
}

bool CBaseDlg::SendFile()
{
	try
	{
		SOCKET socket = NULL;
		if (m_bServer)
			socket = m_ServerSendSocket.GetWorkingSocket();
		else
			socket = m_ClientSendSocket.GetWorkingSocket();

// 		m_SendThread.StopWorking();
// 		m_SendThread.SetInfo(m_hWnd, socket, m_strSendFile, m_strSendPath, m_uSendPos);
// 		m_SendThread.StartWorking();
	}
	catch(...){}
	return FALSE;
}

bool CBaseDlg::ReceiveFile()
{
	try
	{
		SOCKET socket = NULL;
		if (m_bServer)
			socket = m_ServerReceiveSocket.GetWorkingSocket();
		else
			socket = m_ClientReceiveSocket.GetWorkingSocket();

		//m_ReceiveThread.StopWorking();
		//m_ReceiveThread.SetInfo(m_hWnd, socket, m_strReceiveFile, m_strReceivePath, m_uSendPos);
		//m_ReceiveThread.StartWorking();
	}
	catch(...){}
	return FALSE;
}

UINT CBaseDlg::MakeTunnelThread(LPVOID param)
{
	ConnectParam* pThis = (ConnectParam*)param;
	bool bRes = false;
	HWND hWnd = NULL;
	TunnelMode mode = NONE;
	pThis->bMaking = true;

	if (gl_pLogger)
		gl_pLogger->log_info("CBaseDlg::MakeTunnelThread begin...");
#define _ServerSocket pThis->pServerSocket
#define _ClientSocket pThis->pClientSocket

	try
	{
		if (pThis)
		{
			hWnd = pThis->hWnd;
			mode =pThis->mode;

			if (_ServerSocket && _ClientSocket)
			{
				if (pThis->bServer)
				{
					if (_ServerSocket->CreateServerSocket(pThis->nPort))
					{
						::PostMessage(hWnd, WM_MAKETUNNELWAIT, 0, 0);
						bRes = _ServerSocket->WaitForConnection();

					}
				}
				else
				{
					int nRetry = 0;
					while (!bRes && nRetry < 5)	//try to create connection for 5 times.
					{
						bRes = _ClientSocket->CreateClientSocket(pThis->IpAddress, pThis->nPort);
						nRetry ++;
						if (!bRes)
							Sleep(1000);
					}
					if (bRes)
						OutputDebugString("_ClientSocket->CreateClientSocket OK\n");
					else
						OutputDebugString("_ClientSocket->CreateClientSocket failed.\n");
				}
			}
		}
	}
	catch(...){}
	if (IsWindow(hWnd))
		::PostMessage(hWnd, WM_MAKETUNNELEND, (WPARAM)bRes, (LPARAM)mode);
	pThis->bMaking = true;
	AfxEndThread(0);
	return 0;
}

LRESULT CBaseDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_MAKETUNNELWAIT:
		{
			if (m_bReBuildConnect)
			{
				m_bReBuildConnect = false;
				if (IsWindow(m_MsgNotifyWnd))
					::PostMessage(m_MsgNotifyWnd, m_nQuitMessageID, FF_REBUILDCHANNELAk, 1);	//rebuild status: wait for connection.
				else
					OutputDebugString("CBaseDlg::WindowProc WM_MAKETUNNELWAIT !IsWindow(m_MsgNotifyWnd).\n");
			}
			else
				OutputDebugString("CBaseDlg::WindowProc WM_MAKETUNNELWAIT !m_bReBuildConnect.\n");
		}
		break;
	case WM_MAKETUNNEL:
		{
			TunnelMode mode = (TunnelMode)wParam;
			if (!m_ConnectNode.bMaking)
			{
				return CreateTunnel(mode);
			}
			return false;
		}
		break;
	case WM_MAKETUNNELEND:
		{
			bool bRes = (bool)wParam;
			TunnelMode mode = (TunnelMode)lParam;
			OnMakeTunnelEnd(bRes, mode);
		}
		break;
	case FF_REBUILDCHANNEL:
		{
			m_bReBuildConnect = true;
			int nPort = (int)wParam;
			return BuildConnect(nPort);
		}
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void CBaseDlg::OnMakeTunnelEnd(bool bSuccess, TunnelMode mode)
{
	if (m_bReBuildConnect && !bSuccess)
	{
		//failed to crate server socket.
		if (IsWindow(m_MsgNotifyWnd))
		{
			::PostMessage(m_MsgNotifyWnd, m_nQuitMessageID, FF_REBUILDCHANNELAk, 0);
		}
	}
	if (gl_pLogger)
		gl_pLogger->log_info("CBaseDlg::OnMakeTunnelEnd Sussess:%d, mode:%d", (int)bSuccess, (int)mode);
	m_bReBuildConnect = false;
	m_bBuildConnect = false;
}

void CBaseDlg::OnClose()
{
	if (IsWindow(m_MsgNotifyWnd))
	{
		::PostMessage(m_MsgNotifyWnd, m_nQuitMessageID, FF_FILECLOSE, 0);
		if (gl_pLogger)
			gl_pLogger->log_info("CBaseDlg::OnClose PostMessage:%d, id:%d", m_MsgNotifyWnd, (int)m_nQuitMessageID);
	}
	this->OnOK();
}
