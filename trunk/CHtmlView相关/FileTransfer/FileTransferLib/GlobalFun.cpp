
#include "stdafx.h"
#include "GlobalFun.h"
#include <shlwapi.h>   
#pragma  comment(lib,"shlwapi.lib")   

//
CString gl_strLanguageId = _T("2052");	//无需处理
bool gl_bAutoControl = false;
ULONGLONG gl_llSize = 0;	//已经处理
map<HWND,ULONGLONG> gl_mapllSize;
bool gl_SenderStop = false;		//已经处理
map<HWND,bool> gl_mapSenderStop;
bool gl_bOverWrite = false;		//已经处理
map<HWND,bool> gl_mapOverWrite;


//Add by PanPeng 2009-08-19
BOOL IsFileExist(LPCTSTR lpszPathName)   
{   
	if (lpszPathName == NULL)
		return FALSE;

	CFileFind find;
	if (!find.FindFile(lpszPathName))
		return FALSE;

	find.FindNextFile();
	if (!find.IsDirectory() && !find.IsDots())
		return TRUE;
	return FALSE;
}

BOOL DropReadOnly(LPCTSTR lpszPathName)
{
	if (lpszPathName == NULL)
		return FALSE;

	try
	{
		CFileStatus rStatus;
		CFile::GetStatus(lpszPathName, rStatus);			//获得文件的属性设置
		rStatus.m_attribute = rStatus.m_attribute & 0x3E;	//如果文件为只读的，将只读属性去掉
		CFile::SetStatus(lpszPathName, rStatus );			//更改文件的属性设置
	}
	catch(...)
	{
		return FALSE;
	}
	return TRUE;
}
//end

//Add by pan peng 2009-03-08
bool SendData(SOCKET socket, char *pData, int nSize)
{
	if(pData == NULL)
		return false;

	int nLeft = nSize;
	while(nLeft > 0)
	{
		int nSend = send(socket, &pData[nSize - nLeft], nLeft, 0);
		if(nSend == SOCKET_ERROR)
			return false;
		nLeft = nLeft - nSend;
	}
	return true;
}

bool ReceiveData(SOCKET socket, char *pData, int nSize)
{
	if(pData == NULL)
		return false;
	
	int nLeft = nSize;
	while(nLeft > 0)
	{
		int nReceive = recv(socket, &pData[nSize - nLeft], nLeft, 0);
		if(nReceive == SOCKET_ERROR)
			return false;
		nLeft = nLeft - nReceive;
	}
	return true;
}

bool PackAndSendHeader(SOCKET socket, short nParam, bool bRequest)
{
	FilePackHeader header;
	header.nType = bRequest ? FILE_REQUEST : FILE_RESPONSE;
	header.nParam = nParam;

	return SendData(socket, (char*)(&header), sizeof(header));
}


FileState SendFile(SOCKET socket, HWND hWrapperWnd, LPCTSTR strFilePathName, ULONGLONG llStartAt)
{
	//gl_SenderStop = false;
	gl_mapSenderStop[hWrapperWnd] = false;
	PostMessage(hWrapperWnd, WM_FILE_OPERATE_BEGIN, 1, 0);

	FileState MyFileState = S_FileOK, YouFileState = S_FileOK;

	//check dir
	CString strFileName = strFilePathName;
	CDestFolderHelper folderHelper;
	if (!folderHelper.IsDestFileExist(strFileName))
		MyFileState = S_FileNotExits;

	//open file
	CFile LocalFile;
	if(!LocalFile.Open(strFileName, CFile::modeRead | CFile::typeBinary))
	{
		if(MyFileState == S_FileOK)
			MyFileState = S_OpenFileFailed;
	}

	if (MyFileState != S_OpenFileFailed)
	{
		ULONGLONG uSeeked = LocalFile.Seek(llStartAt, CFile::begin);
		if (uSeeked != llStartAt)
		{
			MyFileState = S_CreateFileFailed;
		}
	}

	//get file time
	FILETIME fileTime;
	if(!GetFileTime(LocalFile.m_hFile, NULL, NULL, &fileTime))
	{
		if(MyFileState == S_FileOK)
			MyFileState = S_GetFileInfoFailed;
	}

	//send my file operate state
	if(!SendData(socket, (char*)&MyFileState, sizeof(FileState)))
	{
		if(MyFileState != S_FileNotExits && MyFileState != S_OpenFileFailed)
			LocalFile.Close();
		return S_SocketError;
	}

	//receive you file operate state
	if(!ReceiveData(socket, (char*)&YouFileState, sizeof(FileState)))
	{
		LocalFile.Close();
		return S_SocketError;
	}

	if(MyFileState != S_FileOK) return MyFileState;
	if(YouFileState != S_FileOK) return YouFileState;

	//init basicFileInfo
	BasicFileInfo basicFileInfo;
	basicFileInfo.CreationTime = fileTime;
	char* pFileName = (LPSTR)(LPCTSTR)(LocalFile.GetFileName());
	memset(&(basicFileInfo.FileName), 0, MAX_PATH);
	CopyMemory(&(basicFileInfo.FileName), pFileName, LocalFile.GetFileName().GetLength());
	basicFileInfo.Size = LocalFile.GetLength();

	//send file info
	if (!SendData(socket, (char*)(&basicFileInfo), sizeof(basicFileInfo)))
	{
		LocalFile.Close();
		return S_SocketError;
	}
	ULONGLONG nDes = LocalFile.GetLength() > 1024 * 1024 * 1024 ? 1024 * 1024 : 1024;	//file's size larger than 1G.
	PostMessage(hWrapperWnd, WM_FILE_SETRANGE_MESSAGE, (WPARAM)(LocalFile.GetLength() / nDes), (LPARAM)0);

	ULONGLONG nRead = llStartAt;
	ULONGLONG nResCount = 0;
	ULONGLONG nFileLength = LocalFile.GetLength();
	FileDataEx fileData;
	
	PostMessage(hWrapperWnd, WM_FILE_SENDFILETOSERVER_PROGRESS_MESSAGE, (WPARAM)(nRead / nDes), 0);
	if (gl_pLogger) gl_pLogger->log_info("SendFile Begin to send: %s", strFilePathName);

	bool *pbstatus = NULL;
	SendMessage(hWrapperWnd, WM_GETSENDSTATE, 0, (LPARAM)&pbstatus);
	//send loop
	while(1)
	{
		int nCount = 0;
		try{
			nCount= LocalFile.Read(fileData.data.Buf, FILEDATASIZE);
		}
		catch(...){
			fileData.state = S_ReadFileFailed;
		}
		fileData.data.nLength = nCount;
		nRead += nCount;

		fileData.state = (*pbstatus) ? S_FileContinue : S_IStop;
		if(nRead == nFileLength)
			fileData.state = S_Finished;
		
		//if(gl_SenderStop)
		if (gl_mapSenderStop.find(hWrapperWnd) != gl_mapSenderStop.end() && gl_mapSenderStop[hWrapperWnd] == true)
		{
			fileData.state = S_IStop;
		}		

		if (!SendData(socket, (char*)(&fileData), sizeof(fileData)))
		{
			LocalFile.Close();
			return S_SocketError;
		}

		if (fileData.data.nLength == 0)
			break;

		if(fileData.state == S_IStop)// || fileData.state == S_ReadFileFailed
		{
			LocalFile.Close();
			return fileData.state;
		}
		
		if(fileData.state == S_Finished)// || fileData.state == S_ReadFileFailed
		{
			LocalFile.Close();
			return fileData.state;
		}

		nResCount++;
		if(nResCount % READWRITESTATUS == 0)
		{
			Sleep(3);
			nResCount = 0;
			
			FileState MyState,YouState;

			if(fileData.state == S_ReadFileFailed)
				MyState = S_ReadFileFailed;
			else
				MyState = (*pbstatus) ? S_FileContinue : S_IStop;

			if(!SendData(socket, (char*)&MyState, sizeof(FileState)))
			{
				LocalFile.Close();
				return S_SocketError;
			}
			if(!ReceiveData(socket, (char*)&YouState, sizeof(FileState)))
			{
				LocalFile.Close();
				return S_SocketError;
			}
			
			if(YouState == S_WriteFileFailed || YouState == S_IStop)
			{
				LocalFile.Close();
				if (YouState == S_IStop)
					return S_OtherSideStop;
				return YouState;
			}

			if(MyState == S_ReadFileFailed || MyState == S_IStop)
			{
				LocalFile.Close();
				if (MyState == S_IStop)
					return S_IStop;
				return MyState;
			}
		}

		PostMessage(hWrapperWnd, WM_FILE_SENDFILETOSERVER_PROGRESS_MESSAGE, (WPARAM)(nRead / nDes), 0);
	}

	return S_Finished;
}

FileState ReceiveFile(SOCKET socket, HWND hWrapperWnd, LPCTSTR strFilePathName, ULONGLONG llStartAt)
{

	PostMessage(hWrapperWnd, WM_FILE_OPERATE_BEGIN, 0, 0);

	FileState MyFileState = S_FileOK, YouFileState = S_FileOK;

	//open file
	CString strFileName = strFilePathName;
	CFile LocalFile;
	UINT uOpMode = llStartAt > 0 ? (CFile::modeReadWrite | CFile::typeBinary) : (CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);

	if (IsFileExist(strFileName))
	{
		if (!DropReadOnly(strFileName))
			MyFileState = S_CreateFileFailed;
	}

	if(!LocalFile.Open(strFileName, uOpMode))
	{
		if(MyFileState == S_FileOK)
			MyFileState = S_CreateFileFailed;
	}

	if (MyFileState != S_CreateFileFailed)
	{
		ULONGLONG uSeeked = LocalFile.Seek(llStartAt, CFile::begin);
		if (uSeeked != llStartAt)
		{
			MyFileState = S_CreateFileFailed;
		}
	}

	//receive you file operate state
	if(!ReceiveData(socket, (char*)&YouFileState, sizeof(FileState)))
	{
		LocalFile.Close();
		return S_SocketError;
	}

	//send my file operate state
	if(!SendData(socket, (char*)&MyFileState, sizeof(FileState)))
	{
		if(MyFileState != S_CreateFileFailed)
			LocalFile.Close();
		return S_SocketError;
	}

	if(MyFileState != S_FileOK) return MyFileState;
	if(YouFileState != S_FileOK) return YouFileState;

	BasicFileInfo basicFileInfo;
	if(!ReceiveData(socket, (char*)(&basicFileInfo), sizeof(basicFileInfo)))
	{
		LocalFile.Close();
		//DeleteFile(strFilePathName);
		return S_SocketError;
	}
	
	ULONGLONG nTotalRead = llStartAt;
	ULONGLONG nDes = basicFileInfo.Size > 1024 * 1024 * 1024 ? 1024 * 1024 : 1024;
	PostMessage(hWrapperWnd, WM_FILE_SETRANGE_MESSAGE, (WPARAM)( basicFileInfo.Size / nDes), (LPARAM)0);

	PostMessage(hWrapperWnd, WM_FILE_SENDFILETOSERVER_PROGRESS_MESSAGE, (WPARAM)(nTotalRead / nDes), 0);
	ULONGLONG nRead = llStartAt;
	ULONGLONG nResCount = 0;
	ULONGLONG nRealRead = 0;

	FileDataEx fileData;

	if (gl_pLogger) gl_pLogger->log_info("ReceiveFile Begin to send: %s", strFilePathName);
	
	bool *pbstatus = NULL;
	SendMessage(hWrapperWnd, WM_GETSENDSTATE, 0, (LPARAM)&pbstatus);
	//receive loop
	while(1)
	{		
		if (!ReceiveData(socket, (char*)(&fileData), sizeof(fileData)))
			return S_SocketError;

		if (fileData.data.nLength == 0)
			break;

		if (fileData.state == S_IStop)
		{
			LocalFile.Close();
			//DeleteFile(strFilePathName);
			return S_OtherSideStop;
		}

		FileState MyState = S_FileOK;
		try{
			if (fileData.data.nLength > 0)
				LocalFile.Write(fileData.data.Buf, fileData.data.nLength);
		}
		catch(...){
			MyState = S_WriteFileFailed;
		}
		nRealRead = nRealRead + fileData.data.nLength;

		nTotalRead = nTotalRead + fileData.data.nLength;
		PostMessage(hWrapperWnd, WM_FILE_SENDFILETOSERVER_PROGRESS_MESSAGE, (WPARAM)(nTotalRead / nDes), 0);

		if(fileData.state == S_Finished)
		{
			if (nRealRead > 0)
				LocalFile.Flush();
			LocalFile.Close();
			return fileData.state;
		}
		
		if(fileData.state == S_ReadFileFailed)
		{
			LocalFile.Close();
			//DeleteFile(strFilePathName);
			return fileData.state;
		}


		nResCount++;
		if(nResCount % READWRITESTATUS == 0)
		{
			nResCount = 0;
			
			FileState MyState,YouState;

			MyState = (*pbstatus) ? S_FileContinue : S_IStop;
			//SendMessage(hWrapperWnd, WM_GETSENDSTATE, 0, (LPARAM)&MyState);
			
			if(!ReceiveData(socket, (char*)&YouState, sizeof(FileState)))
			{
				LocalFile.Close();
				//DeleteFile(strFilePathName);
				return S_SocketError;
			}

			if(!SendData(socket, (char*)&MyState, sizeof(FileState)))
			{
				LocalFile.Close();
				//DeleteFile(strFilePathName);
				return S_SocketError;
			}
			
			if(MyState == S_IStop)
			{
				LocalFile.Close();
				//DeleteFile(strFilePathName);
				return S_IStop;
			}

			if(YouState == S_WriteFileFailed || YouState == S_IStop)
			{
				LocalFile.Close();
				//DeleteFile(strFilePathName);
				if(YouState == S_IStop) 
					YouState = S_OtherSideStop;
				return YouState;
			}
		}
	}

	return S_Finished;
}

bool ProcessState(FileState state, HWND hWrapperWnd, bool bSend)
{
	gl_mapllSize[hWrapperWnd] = 0;
	bool bReturn = true;
	if (S_SocketError == state)
	{
		bReturn = false;
	}
	if (gl_pLogger) gl_pLogger->log_info("ProcessState: %d, %d, %d", state, hWrapperWnd, bSend);
	//添加日志
	if(IsWindow(hWrapperWnd))
	{		
		PostMessage(hWrapperWnd, WM_FILE_OPERATE_END, (WPARAM)state, (LPARAM)bSend);
	}
	else
	{
		if (gl_pLogger) gl_pLogger->log_error("ProcessState %d is not a valid window.");
	}
	return bReturn;
}

CString GenNoticeMsg(FileState state, bool bSend)
{
	CString strMsg = _T("Unknow Message, Please retry agian.");
	if(lstrcmp(gl_strLanguageId, CHINESE) == 0)
	{
		strMsg = "未知错误，请重新尝试。";
		switch(state)
		{
		case S_SocketError:
			strMsg = szDataLinkError_CH;
			break;
		case S_FileNotExits:
			if(bSend)
				strMsg = szFileNotExist_CH;
			else 
				strMsg = szRemoteFileNotExist_CH;
			break;
		case S_OpenFileFailed:
			if(bSend)
				strMsg = szCannotOpenFile_CH;
			else
				strMsg = szRemoteCannotOpenFile_CH;
			break;
		case S_CreateFileFailed:
			if(bSend)
				strMsg = szRemoteFailedCreatingFile_CH;
			else
				strMsg = szFailedCreatingFile_CH;
			break;
		case S_ReadFileFailed:
			if(bSend)
				strMsg = szFailedReadingFile_CH;
			else
				strMsg = szRemoteFailedReadingFile_CH;
			break;
		case S_WriteFileFailed:
			if(bSend)
				strMsg = szRemoteFailedWritingFile_CH;
			else
				strMsg = szFailedWritingFile_CH;
			break;
		case S_GetFileInfoFailed:
			if(bSend)
				strMsg = szFailedGettingFileInfo_CH;
			else
				strMsg = szRemoteFailedGettingFileInfo_CH;
			break;
		case S_ThreadExit:
			if(bSend)
				strMsg = szFileSendingThreadAborted_CH;
			else
				strMsg = szFileReceivingThreadAborted_CH;
			break;
		case S_OtherSideStop:
			strMsg = szRemoteStoppedFileTransfer_CH;
			break;
		case S_IStop:
			strMsg = szYouStoppedFileTransfer_CH;
			break;
		case S_Finished:
			if(bSend)
				strMsg = szFinishedSendingFile_CH;
			else
				strMsg = szFinishedReceivingFile_CH;
			break;
		case S_FileContinue:
		case S_FileOK:
			break;
		}
	}
	else
	{
		switch(state)
		{
		case S_SocketError:
			strMsg = szDataLinkError_EN;
			break;
		case S_FileNotExits:
			if(bSend)
				strMsg = szFileNotExist_EN;
			else 
				strMsg = szRemoteFileNotExist_EN;
			break;
		case S_OpenFileFailed:
			if(bSend)
				strMsg = szCannotOpenFile_EN;
			else
				strMsg = szRemoteCannotOpenFile_EN;
			break;
		case S_CreateFileFailed:
			if(bSend)
				strMsg = szRemoteFailedCreatingFile_EN;
			else
				strMsg = szFailedCreatingFile_EN;
			break;
		case S_ReadFileFailed:
			if(bSend)
				strMsg = szFailedReadingFile_EN;
			else
				strMsg = szRemoteFailedReadingFile_EN;
			break;
		case S_WriteFileFailed:
			if(bSend)
				strMsg = szRemoteFailedWritingFile_EN;
			else
				strMsg = szFailedWritingFile_EN;
			break;
		case S_GetFileInfoFailed:
			if(bSend)
				strMsg = szFailedGettingFileInfo_EN;
			else
				strMsg = szRemoteFailedGettingFileInfo_EN;
			break;
		case S_ThreadExit:
			if(bSend)
				strMsg = szFileSendingThreadAborted_EN;
			else
				strMsg = szFileReceivingThreadAborted_EN;
			break;
		case S_OtherSideStop:
			strMsg = szRemoteStoppedFileTransfer_EN;
			break;
		case S_IStop:
			strMsg = szYouStoppedFileTransfer_EN;
			break;
		case S_Finished:
			if(bSend)
				strMsg = szFinishedSendingFile_EN;
			else
				strMsg = szFinishedReceivingFile_EN;
			break;
		case S_FileContinue:
		case S_FileOK:
			break;
		}
	}
	return strMsg;
}

int FindMyItem(CListCtrl* pListCtrl, CString strItemName)
{
	if (pListCtrl == NULL)
		return -1;
	for(int i = 0; i < pListCtrl->GetItemCount(); i ++)
	{
		CString strItemText = pListCtrl->GetItemText(i, 0);
		if (strItemName == strItemText)
			return i;
	}
	return -1;
}

BOOL DoEvents()
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) == TRUE)
	{
		if (GetMessage(&msg, NULL, 0, 0) )
		{
			/*before you translate and dispatch you might consider
			checking if that message does not close you window
			you should ignore it because if you doevents() in a loop
			and the user click the close buton it closes !
			and you loop continues untill it ends .*/
			if ((msg.message != WM_DESTROY) && (msg.message != WM_CLOSE) &&
				((msg.message != WM_SYSCOMMAND) && (msg.wParam != SC_CLOSE)))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				/*for closing prevention !*/
			}
		}
		else
		{
			/*if all messages in queue processed we can return to our 
			  time consuming operation*/
			return TRUE; 
		}
	}
	return FALSE;
}