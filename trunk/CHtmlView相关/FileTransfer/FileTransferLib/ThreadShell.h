//code by Pan Peng 
//Data: 2009-12-11
//Email:abcpanpeng@163.com

#pragma once
#include "ThreadBase.h"
#include "afxmt.h"
#include "BaseDlg.h"
class CThreadShell : public CThreadBase
{
public:
	CThreadShell(void);
	~CThreadShell(void);
public:
	bool CreateFileServer(int nPort, HWND MSGNotifyWnd, int nQuitMessageID, LPCTSTR strLanguageId, bool bAutoControl);
	bool CreateFileClient(char* pServerAddress, int nPort, HWND MSGNotifyWnd, int nQuitMessageID, bool bGetRemoteFile);
	void Stop();
	void SetDlgWnd(HWND hWnd){ m_hWnd = hWnd; }
	void SetExit(bool bExit){ m_bExit = bExit; }
protected:
	virtual void ThreadProc();
	virtual void OnThreadStop();   
protected:
	bool m_bServer;
	HWND m_hWnd;

	int	 m_nPort;
	bool m_bGetRemoteFile;
	HWND m_MsgNotifyWnd;
	int  m_nQuitMessageID;
	CString m_strLanguageId;
	bool m_bExit;

	char m_ServerAddress[MAX_PATH];
	CBaseDlg *m_pDlg;
	
};

//extern CEvent gl_StartEvent;
