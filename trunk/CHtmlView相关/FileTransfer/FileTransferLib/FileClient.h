#pragma once
#include "filemanagerbase.h"
#include "threadbase.h"
#include "ClientSocket.h"
#include "FileAskDlg.h"
#include "ThreadShell.h"

bool IsActive();
void SetNotActive();

class CFileClient :  
	public CThreadBase,
	public CFileManagerBase
{
public:
	CFileClient(void);
	~CFileClient(void);

public:
	virtual void	ThreadProc();
	virtual void	OnThreadStop();
	
private:
	CClientSocket*		m_pClientSocket;
	HWND m_hWnd;
	CString				m_strDistinationFolder;
	CString				m_strFileToReceive;
private:
	bool ProcessFileHeader(FilePackHeader* pFileHeader);
	bool ReceiveListFileInDestinationFolder();
	bool SendFileToServer();
	bool ReceiveFileFromServer();
	bool ReceiveDriverList();
	bool ReceiveNewPath();
	bool ResponseSendFileRequest();

	bool				m_bWrapper;
	
public:
	void SetWnd(HWND hWnd){ m_hWnd = hWnd; }
	void SetClientSocket(CClientSocket* pSocket){ m_pClientSocket = pSocket; }
};
