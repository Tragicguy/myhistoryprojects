// FileTransferLib.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "FileTransferLib.h"
//#include "FileServer.h"
//#include "FileClient.h"
#include "GlobalFun.h"
#include "ThreadShell.h"
#include <map>
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//


// CFileTransferLibApp

BEGIN_MESSAGE_MAP(CFileTransferLibApp, CWinApp)
END_MESSAGE_MAP()


// CFileTransferLibApp 构造

CFileTransferLibApp::CFileTransferLibApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CFileTransferLibApp 对象

CFileTransferLibApp theApp;


// CFileTransferLibApp 初始化

BOOL CFileTransferLibApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxInitRichEdit2();

	return TRUE;
}

FileTransferLog gl_logger;
CThreadShell m_ServerManager;
//add by shaojun at 2010-09-28 通过数组来保存控制文件传输的对象
CThreadShell m_ClientManagerEx[10];
static map<HWND,CThreadShell *> gl_mapClientManager;

//客户端，无需变动
extern "C" __declspec(dllexport) bool CreateFileServer(int nPort, HWND MSGNotifyWnd, int nQuitMessageID, LPCTSTR strLanguageId, bool bAutoControl)
{
	bool bRes = m_ServerManager.CreateFileServer(nPort, MSGNotifyWnd, nQuitMessageID, strLanguageId, bAutoControl);

	if (gl_pLogger) 
		gl_pLogger->log_info("\r\n\r\nMain proccess CreateFileServer. Port:%d, LanguageId:%s return %d", nPort, strLanguageId, bRes);

	return bRes;
}
//客户端，无需变动
extern "C" __declspec(dllexport) void StopFileServer(bool bLocal)
{
	if (gl_pLogger) 
		gl_pLogger->log_info("Main proccess StopFileServer.");
	return m_ServerManager.Stop();
}

extern "C" __declspec(dllexport) bool CreateFileClient(char* pServerAddress, int nPort, HWND MSGNotifyWnd, int nQuitMessageID, bool bGetRemoteFile)
{
	CThreadShell *pShell = NULL;
	if (gl_mapClientManager.find(MSGNotifyWnd) != gl_mapClientManager.end())
	{
		pShell = gl_mapClientManager[MSGNotifyWnd];
	}
	else
	{
		pShell = new CThreadShell;
		gl_mapClientManager[MSGNotifyWnd] = pShell;
	}
	if (!pShell)
	{
		if (gl_pLogger) gl_pLogger->log_info("CreateFileClient pShell NULL.");
		return false;
	}
	bool bRes = pShell->CreateFileClient(pServerAddress, nPort, MSGNotifyWnd, nQuitMessageID, bGetRemoteFile);

	if (gl_pLogger) 
		gl_pLogger->log_info("\r\n\r\nMain proccess CreateFileClient. ServerAddress:%s, Port:%d, GetRemoteFile:%d,HWND:%d,return %d", pServerAddress, nPort, bGetRemoteFile, MSGNotifyWnd,bRes);
	return bRes;
}
	
extern "C" __declspec(dllexport) void StopFileClient(bool bLocal,HWND hwnd)
{
	CThreadShell *pShell = NULL;
	map<HWND,CThreadShell *>::iterator it = gl_mapClientManager.find(hwnd);
	if ( it != gl_mapClientManager.end())
	{
		pShell = gl_mapClientManager[hwnd];
		gl_mapClientManager.erase(it);
	}	
	if (!pShell)
	{
		if (gl_pLogger) gl_pLogger->log_info("StopFileClient pShell NULL.");
		return ;
	}
	if (gl_pLogger) gl_pLogger->log_info("Main proccess StopFileClient. HWND:%d",hwnd);
	pShell->Stop();
	delete pShell;
	pShell = NULL;
	return;
}