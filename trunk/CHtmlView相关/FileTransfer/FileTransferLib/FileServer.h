#pragma once
#include ".\FileManagerBase.h"
#include "ThreadShell.h"
#include "ServerSocket.h"
#include "DestFolderHelper.h"
#include "ThreadBase.h"

//#include "SFileDlgWrapper.h"

#include "FileDefines.h"


class CFileServer :
	public CThreadBase,
	public CFileManagerBase
{
public:
	CFileServer(void);
	~CFileServer(void);

public:
	//Overriden functions
	virtual void	ThreadProc();
	virtual void	OnThreadStop();
private:
	//Wrapped socket for File Server
	CServerSocket		*m_pServerSocket;

	//Indicates whether File Server is still running
	bool				m_bRunning;
	bool				m_bWrapper;

	//Indicates whether the local client folder is existed before starting File Server
	bool				m_IsDestFolderPreviouslyExisted;
private:

	/*
	Process File Server package header
		pFileHeader = Pointer to the package header
	*/
	bool			ProcessFileHeader(FilePackHeader* pFileHeader);

	/*
	List files in the local client folder and send the list to the File Client
	*/
	bool			ListFileInDestinationFolder();

	//Send List File count info as ResponseHeader to File Client
	bool			SendListFileResponseHeader(FileCount FileCountInfo);

	/*
	Send info of a local file to File Client
	pFileList = Pointer to the BasicFileInfoList which contains local file Info
	*/
	bool			SendLocalFileInfo(BasicFileInfoList* pFileList);

	//Receive a file from File Client
	bool			ReceiveFileFromClient();

	//Send a file to File Client
	bool			SendFileToClient();

	//Pack a Request struct and send it to File Client
	bool			PackAndSendRequest(short nParam);

	//Remove local client folder
	void			RemoveDestFolder();

	CString			m_strReceivePath;
	CString			m_strSendPath;

	CString			m_strRecFileName;
	CString			m_strSendFileName;

	bool			SendDrivers();
	Driver*			GetDrivers(int &Count);
	CString			GetAskMessage(bool bSend, LPCTSTR strPath, LPCTSTR strFileName);
	bool			ResponseC2S();
	bool			ResponseS2C();

	BasicFileInfo	m_FileInfo;

	bool SendSaveAs(LPCTSTR strDistinationFolder);

	HWND m_hWnd;
	bool m_bStopMyselft;

private:
	void GetLocalRemotePath(char *pPath);
public:
	void SetWnd(HWND hWnd) { m_hWnd = hWnd; }
	void SetServerSocket(CServerSocket* pSocket){ m_pServerSocket = pSocket; }
};


