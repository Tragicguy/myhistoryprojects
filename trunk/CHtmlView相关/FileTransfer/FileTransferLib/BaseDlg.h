#pragma once

#include "threadbase.h"
#include "ServerSocket.h"
#include "ClientSocket.h"

class CThreadShell;

enum TunnelMode
{ 
	NONE = 0,
	CONTROL,
	SEND,
	RECEIVE
};

typedef struct _ConnectParam
{
	CServerSocket* pServerSocket;
	CClientSocket* pClientSocket;
	char IpAddress[MAX_PATH];
	int nPort;
	HWND hWnd;
	bool bServer;	
	bool bMaking;
	TunnelMode mode;
	_ConnectParam()
	{
		pServerSocket = NULL;
		pClientSocket = NULL;
		memset(IpAddress, 0, MAX_PATH);
		nPort = 0;
		hWnd = NULL;
		bServer = false;
		mode = NONE;
		bMaking = false;
	}
}ConnectParam;

class CBaseDlg : public CDialog
{
protected:
	DECLARE_DYNAMIC(CBaseDlg)

public:
	//CBaseDlg(CWnd* pParent = NULL);   // 标准构造函数
	explicit CBaseDlg(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	explicit CBaseDlg(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	virtual ~CBaseDlg();
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	void InitMember();
public:
	void SetThreadShell(CThreadShell* pThreadShell){ m_pThreadShell = pThreadShell; }
	void SetControlThread(CThreadBase* pThread) { m_pControlThread = pThread; }
	virtual void OnOK();
	void SetServerDlgInfo(int nPort, HWND MSGNotifyWnd, int nQuitMessageID, LPCTSTR strLanguageId)
	{
		m_nCtrlPort = nPort;
		m_bServer = true;
		m_MsgNotifyWnd = MSGNotifyWnd;
		m_nQuitMessageID = nQuitMessageID;
		m_strLanguageId = strLanguageId;
	}
	void SetClientDlgInfo(char* pServerAddress, int nPort, HWND MSGNotifyWnd, int nQuitMessageID, bool bGetRemoteFile)
	{
		m_nCtrlPort = nPort;
		m_bServer = false;
		m_MsgNotifyWnd = MSGNotifyWnd;
		m_nQuitMessageID = nQuitMessageID;
		m_bGetRemoteFile = bGetRemoteFile;
		lstrcpy(m_strServerAddress, pServerAddress);
	}
	bool SendFile();
	bool ReceiveFile();

	static UINT MakeTunnelThread(LPVOID param);
	virtual void OnMakeTunnelEnd(bool bSuccess, TunnelMode mode);
public:
	CThreadShell*	m_pThreadShell;		//DLL线程

	CThreadBase*	m_pControlThread;	//控制线程
	//CSendThread		m_SendThread;		//文件发送线程
	//CReceiveThread	m_ReceiveThread;	//文件接收线程

	bool m_bServer;
	bool m_bGetRemoteFile;
	HWND m_MsgNotifyWnd;
	int m_nQuitMessageID;
	CString m_strLanguageId; 

	char m_strServerAddress[MAX_PATH];

	int m_nCtrlPort;
	int m_nSendPort;
	int m_nRecevPort;

	CServerSocket m_ServerCtrlSocket;
	CServerSocket m_ServerSendSocket;
	CServerSocket m_ServerReceiveSocket;

	CClientSocket m_ClientCtrlSocket;
	CClientSocket m_ClientSendSocket;
	CClientSocket m_ClientReceiveSocket;

	CString m_strSendFile;
	CString m_strSendPath;
	CString m_strReceiveFile;
	CString m_strReceivePath;

	ULONGLONG m_uSendPos;
	ULONGLONG m_uReceivePos;

	ConnectParam m_ConnectNode;

	CWinThread* m_pCreateThread;
	bool m_bReBuildConnect;		//build reconnection
	bool m_bBuildConnect;		//normal build connection
public:
	virtual BOOL OnInitDialog();
	BOOL CreateTunnel(TunnelMode nMode);
	void StopTunnelThread();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void Stop();
	BOOL BuildConnect(int nPort);
	bool m_bHaveExited;
public:
	afx_msg void OnClose();
};
