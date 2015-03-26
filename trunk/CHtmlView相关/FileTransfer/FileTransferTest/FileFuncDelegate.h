#pragma once

/*
constances and viables list

-----Begin-----------------------------------------------------
*/
const char FileDllName[] = "FileTransferLib.dll";

const char CreateServerFuncName[] = "CreateFileServer";
const char StopServerFuncName[] = "StopFileServer";	

const char CreateClientFuncName[] = "CreateFileClient";
const char StopClientFuncName[] = "StopFileClient";	

typedef  bool (*CreateFileServer)(int nPort, HWND MSGNotifyWnd, int nQuitMessageID, LPCTSTR strLanguageId);
typedef  void (*StopFileServer)();

typedef  bool (*CreateFileClient)(char* pServerAddress, int nPort, HWND MSGNotifyWnd, int nQuitMessageID, bool bGetRemoteFile);
typedef  void (*StopFileClient)();

//---end----------------------------------------------//


class CFileFuncDelegate
{
public:
	CFileFuncDelegate(void);
	~CFileFuncDelegate(void);
public:
	CreateFileServer m_funcCreateFileServer;
	StopFileServer m_funcStopFileServer;

	CreateFileClient m_funcCreateFileClient;
	StopFileClient m_funcStopFileClient;

	bool InitFileDLLFunction();
	void ReleaseFileDLL();
private:
	HINSTANCE m_hInst; 
};
