#pragma once

#include "DestFolderHelper.h"
#include "StringTable.h"
#include "ProgramConfig.h"

#include "_log.h"
#include <map>
using namespace std;
using namespace extension;

#define READWRITESTATUS			40
#define FILEDATASIZE			4096

#define WM_GETSENDSTATE			WM_USER + 80000
#define WM_GETFILENAMETOSEND	WM_USER + 80001
#define WM_GETFILENAMETORECEIVE WM_USER + 80002
#define WM_FILE_OPERATE_END		WM_USER + 80003
#define WM_FILE_OPERATE_BEGIN	WM_USER + 80004
#define WM_GETCURRENTPATH		WM_USER + 80005
#define WM_GETSENDPATH			WM_USER + 80006
#define WM_GETSENDING			WM_USER + 80007
#define WM_GETRECEIVE			WM_USER + 80008
#define WM_FREEUI				WM_USER + 80009
#define WM_REFRESHLOCALLIST		WM_USER + 80011

#define CHINESE "2052"
#define ENGLISH "1033"

typedef struct FileData
{
	char Buf[FILEDATASIZE];
	int nLength;
	FileData()
	{
		nLength = 0;
	}
}FileData;

typedef enum { 
	S_SocketError = -1,
	S_FileNotExits,
	S_OpenFileFailed,
	S_CreateFileFailed,
	S_ReadFileFailed,
	S_WriteFileFailed,
	S_GetFileInfoFailed,
	S_ThreadExit,
	S_OtherSideStop,
	S_IStop,
	S_Finished,
	S_FileContinue,
	S_FileOK
}FileState;
	
typedef struct FileDataEx
{
	FileData data;
	FileState state;
}FileDataEx;

bool PackAndSendHeader(SOCKET socket, short nParam, bool bRequest);
bool SendData(SOCKET socket, char *pData, int nSize);
bool ReceiveData(SOCKET socket, char *pData, int nSize);

FileState SendFile(SOCKET socket, HWND hWrapperWnd, LPCTSTR strFilePathName, ULONGLONG llStartAt = 0);
FileState ReceiveFile(SOCKET socket, HWND hWrapperWnd, LPCTSTR strFilePathName, ULONGLONG llStartAt = 0);

bool ProcessState(FileState state, HWND hWrapperWnd, bool bSend = true);
CString GenNoticeMsg(FileState state, bool bSend);
BOOL IsFileExist(LPCTSTR lpszPathName);
BOOL DropReadOnly(LPCTSTR lpszPathName);

int FindMyItem(CListCtrl* pListCtrl, CString strItemName);

BOOL DoEvents();

extern bool gl_SenderStop;
extern map<HWND,bool> gl_mapSenderStop;
extern CString gl_strLanguageId;
extern bool gl_bAutoControl;
extern ULONGLONG gl_llSize;
extern map<HWND,ULONGLONG> gl_mapllSize;
extern bool gl_bOverWrite;
extern map<HWND,bool> gl_mapOverWrite;


const char szLogFileName[] = "FileTransferLog.dat";
class FileTransferLog
{
private:
	event_log* m_pLogger;
public:
	FileTransferLog()
	{
		char logFile[MAX_PATH];
		ZeroMemory(logFile, MAX_PATH);
		GetModuleFileName(NULL, logFile, MAX_PATH);
		*(strrchr(logFile, '\\') + 1) = '\0';
		lstrcat(logFile, "data\\");
		lstrcat(logFile, szLogFileName);
		
		m_pLogger = new event_log(logFile);
	}

	~FileTransferLog()
	{
		if (m_pLogger)
		{
			delete m_pLogger;
			m_pLogger = NULL;
		}
	}

	event_log* GetLogger()
	{
		return m_pLogger;
	}
};

extern FileTransferLog gl_logger;

#define gl_pLogger gl_logger.GetLogger()