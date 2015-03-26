#include "StdAfx.h"
#include "FileClient.h"
#include "FileDefines.h"
#include "FileClientDialog.h"
#include "FileAskDlg.h"
#include "GlobalFun.h"

#pragma warning(disable : 4800)
CCriticalSection gl_ActiveSection;
bool gl_bActive = true;	//无需处理


CFileClient::CFileClient(void)
{
	if (gl_pLogger)
		gl_pLogger->log_info("CFileClient::CFileClient(void)");
	m_bWrapper = false;	
	m_pClientSocket = NULL;
}

CFileClient::~CFileClient(void)
{
	if (gl_pLogger)
		gl_pLogger->log_info("CFileClient::~CFileClient");
}

void CFileClient::ThreadProc()
{
	try
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClient::ThreadProc()");


		int nDataToRead = sizeof(FilePackHeader);
		char* pFileBuf = new char[nDataToRead];

		if (!m_bRunning)
		{
			OutputDebugString("CFileClient::ThreadProc() !m_bRunning\n");
		}
		while (m_bRunning)
		{
			SOCKET hSocket = m_pClientSocket->GetWorkingSocket();
			if (hSocket == INVALID_SOCKET)
			{
				if (!m_bRunning)
				{
					break;
				}
				if (IsWindow(this->m_hWnd))
				{
					OutputDebugString("CFileClient SendMessage(m_hWnd, WM_MAKETUNNEL, CONTROL, 0);\n");
#pragma pack(push)
#pragma warning(disable:4800)
					bool bRes = SendMessage(m_hWnd, WM_MAKETUNNEL, CONTROL, 0);
#pragma pack(pop)
				}
				Sleep(50);
			}
			else if (m_pClientSocket->IsConnected())
			{
				bool bRead = m_pClientSocket->ReadExact(hSocket, pFileBuf, nDataToRead);
				if (!bRead)
				{
					if (gl_pLogger)	gl_pLogger->log_error("m_pClientSocket->ReadExact false");
					m_pClientSocket->StopClientSocket();
					if (!m_bRunning)
					{
						break;
					}
#pragma pack(push)
#pragma warning(disable:4800)					
					bool bRes = SendMessage(m_hWnd, WM_MAKETUNNEL, CONTROL, 0);
#pragma pack(pop)
				}
				FilePackHeader* pFileHeader = (FilePackHeader*)pFileBuf;

				if (!ProcessFileHeader(pFileHeader))
				{
					if (gl_pLogger)	gl_pLogger->log_info("CFileClient !ProcessFileHeader");
					break;
				}
			}
			else
				Sleep(50);
		}

		delete [] pFileBuf;

		//Quit after this function finishes
		this->MarkTheadToStop();

		if (gl_pLogger)
			gl_pLogger->log_info("CFileClient::ThreadProc() end...");
		OutputDebugString("CFileClient::ThreadProc() end...\n");
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClient::ThreadProc() unkown exception.");
	}
}

void CFileClient::OnThreadStop()
{
	try
	{
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClient::OnThreadStop() unkown exception.");
	}
}

//Process the Chat Data
bool CFileClient::ProcessFileHeader(FilePackHeader* pFileHeader)
{
	try
	{
		switch (pFileHeader->nType)
		{

		case FILE_RESPONSE:
			{

				switch (pFileHeader->nParam)
				{
				case CMD_REJECTWHENNOTFREE:
					SendMessage(m_hWnd, WM_FREEUI, 0, 0);
					return true;
				case CMD_PARAM_LISTFILE:
					{
						Sleep(20);
						return ReceiveListFileInDestinationFolder();
					}

				case CMD_PARAM_TRANSFILE_C2S:
					{
						return SendFileToServer(); 
					}

				case CMD_PARAM_TRANSFILE_C2S_FINISH:
					{
						FileToSend distfile;
						if (!m_pClientSocket->ReadExact(m_pClientSocket->GetWorkingSocket(), (char*)(&distfile), sizeof(FileToSend)))
						{
							return false;
						}

						CString strDestionFile = distfile.FileName;
						SendMessage(m_MSGNotifyWnd, m_nQuitMessageID, 1, (LPARAM) (LPCTSTR) strDestionFile);
						return true;
					}
				}
			}
			break;
		case FILE_REQUEST:
			{
				switch (pFileHeader->nParam)
				{
				case CMD_PARAM_TRANSFILE_S2C_SENDFILEREQUEST:
					return ResponseSendFileRequest();

				case CMD_PARAM_DRIVER:
					{
						//Sleep(1000);
						OutputDebugString("ReceiveDriverList()...\n");
						return ReceiveDriverList();
					}

					//end***

				case CMD_PARAM_TRANSFILE_C2S_ACEEPT:
					{
						FileTransProgress progress;

						if (!m_pClientSocket->ReadExact(m_pClientSocket->GetWorkingSocket(), (char*)(&progress), sizeof(FileTransProgress)))
						{
							return false;
						}
						//gl_llSize = progress.uDataRead;
						gl_mapllSize[m_hWnd] = progress.uDataRead;
						if(IsWindow(m_hWnd))
						{
							SendMessage(this->m_hWnd, WM_FILE_ENABLEBUTTON_MESSAGE, (WPARAM)0, 0);
							PostMessage(this->m_hWnd, WM_SHOWSTOPBUTTON, (WPARAM)true, 0);
						}
						Sleep(200);
						return SendFileToServer();
						//return 1;
					}
				case CMD_PARAM_TRANSFILE_S2C_ACEEPT:
					{
						bool bEnable = false;
						if(IsWindow(m_hWnd))
						{
							SendMessage(this->m_hWnd, WM_FILE_DISABLEBUTTON_MESSAGE, (WPARAM)0, 0);
							PostMessage(this->m_hWnd, WM_SHOWSTOPBUTTON, (WPARAM)true, 0);
						}
						//***panpeng 2008-12-01
						FilePackHeader header;
						header.nType = FILE_REQUEST;
						header.nParam = CMD_PARAM_TRANSFILE_S2C;
						if (!m_pClientSocket->WriteExact(m_pClientSocket->GetWorkingSocket(), (char*)(&header), sizeof(header)))
							return false;
						//end***
						Sleep(200);
						bool bResult = ReceiveFileFromServer();
						bEnable = true;

						return bResult;
					}
				case CMD_PARAM_TRANSFILE_C2S_REJECT:
					if(IsWindow(m_hWnd))
					{	
						SendMessage(this->m_hWnd, WM_FILE_ENABLEBUTTON_MESSAGE, (WPARAM)0, 0);
						PostMessage(this->m_hWnd, WM_SHOWSTOPBUTTON, (WPARAM)true, 0);
						PostMessage(this->m_hWnd, WM_REJECT_SENDREQUEST, 1, 0);
					}
					return true;
				case CMD_PARAM_TRANSFILE_S2C_REJECT:
					if(IsWindow(m_hWnd))
					{
						SendMessage(this->m_hWnd, WM_FILE_ENABLEBUTTON_MESSAGE, (WPARAM)0, 0);
						PostMessage(this->m_hWnd, WM_SHOWSTOPBUTTON, (WPARAM)true, 0);
						PostMessage(this->m_hWnd, WM_REJECT_SENDREQUEST, 0, 0);
					}
					return true;
				}
			}
		}
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClient::ProcessFileHeader() unkown exception.");
	}

	return true;
}

bool CFileClient::ReceiveListFileInDestinationFolder()
{
	try
	{
		FileCount fileCountInfo;

		if (!m_pClientSocket->ReadExact(m_pClientSocket->GetWorkingSocket(), (char*)(&fileCountInfo), sizeof(fileCountInfo)))
		{
			return false;
		}

		int nCount = fileCountInfo.nFileCount;

		char* fileList = new char[nCount * sizeof(BasicFileInfo)];
		memset(fileList, 0, nCount * sizeof(BasicFileInfo));

		for (int i = 0; i < nCount; i++)
		{
			BasicFileInfo fileInfo;
			if (!m_pClientSocket->ReadExact(m_pClientSocket->GetWorkingSocket(), (char*)(&fileInfo), sizeof(fileInfo)))
			{
				delete [] fileList;
				return false;
			}

			CopyMemory(fileList + i * sizeof(BasicFileInfo), &fileInfo, sizeof(BasicFileInfo));
		}

		if(IsWindow(m_hWnd))
			SendMessage(this->m_hWnd, WM_FILE_LISTFILE_MESSAGE, (WPARAM)fileList, (LPARAM)nCount);

		delete [] fileList;
		return true;
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClient::ReceiveListFileInDestinationFolder() unkown exception.");
	}
	return false;
}

bool CFileClient::SendFileToServer()
{
	try
	{
		bool bRes = ::PackAndSendHeader(m_pClientSocket->GetWorkingSocket(), CMD_PARAM_TRANSFILE_C2S, true);
		if(!bRes)
			return false;

		CString strFileToSend;
		SendMessage(m_hWnd, WM_GETFILENAMETOSEND, 0, (LPARAM)&strFileToSend);
		ULONGLONG ullSize = 0;
		if (gl_mapllSize.find(m_hWnd) != gl_mapllSize.end())
		{
			ullSize = gl_mapllSize[m_hWnd];
		}
		FileState state = SendFile(m_pClientSocket->GetWorkingSocket(), m_hWnd, strFileToSend, ullSize);
		if (gl_pLogger) gl_pLogger->log_info("SendFile return %d", state);
		return ProcessState(state, m_hWnd);
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClient::SendFileToServer() unkown exception.");
	}
	return false;
}

bool CFileClient::ReceiveFileFromServer()
{
	try
	{
		CString strFilePath, strFileName;

		if(!m_strDistinationFolder.IsEmpty())
			strFilePath = m_strDistinationFolder;
		else
			SendMessage(m_hWnd, WM_GETCURRENTPATH, 0, (LPARAM)&strFilePath);

		if(!m_strFileToReceive.IsEmpty())
			strFileName = m_strFileToReceive;
		else
			SendMessage(m_hWnd, WM_GETFILENAMETORECEIVE, 0, (LPARAM)&m_strFileToReceive);

		if(strFilePath.Right(1) != _T("\\"))
			strFilePath = strFilePath + _T("\\");
		strFileName = strFilePath + m_strFileToReceive;

		m_strDistinationFolder = _T("");
		m_strFileToReceive = _T("");
		ULONGLONG ullSize = 0;
		if (gl_mapllSize.find(m_hWnd) != gl_mapllSize.end())
		{
			ullSize = gl_mapllSize[m_hWnd];
		}
		FileState state = ReceiveFile(m_pClientSocket->GetWorkingSocket(), m_hWnd, strFileName, ullSize);
		if (gl_pLogger) gl_pLogger->log_info("ReceiveFile return %d", state);
		return ProcessState(state, m_hWnd, false);
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClient::ReceiveFileFromServer() unkown exception.");
	}
	return false;

}

//***Add by pan peng 2008-11-28
bool CFileClient::ReceiveDriverList()
{
	try
	{
		FileCount DirverCount;
		if(!m_pClientSocket->ReadExact(m_pClientSocket->GetWorkingSocket(), (char*)(&DirverCount), sizeof(DirverCount)))
		{
			AfxMessageBox("读取远端磁盘驱动器列表数量失败");
			return false;
		}
		Driver *driver = new Driver[DirverCount.nFileCount];

		for(int i = 0; i < DirverCount.nFileCount; i ++)
		{
			if(!m_pClientSocket->ReadExact(m_pClientSocket->GetWorkingSocket(), (char*)(&driver[i]), sizeof(Driver)))
			{
				delete []driver;
				AfxMessageBox("读取远端磁盘驱动器列表失败");
				return false;
			}
		}

		FileToSend desktop;
		if(!m_pClientSocket->ReadExact(m_pClientSocket->GetWorkingSocket(), (char*)(&desktop), sizeof(FileToSend)))
		{
			delete []driver;
			return false;
		}

		if(IsWindow(m_hWnd))
		{
			OutputDebugString("CFileClient::ReceiveDriverList() SendMessage(this->m_hWnd, WM_FILE_LISTDRIVERS_MESSAGE, (WPARAM)(driver), (LPARAM)DirverCount.nFileCount);\n");
			SendMessage(this->m_hWnd, WM_FILE_LISTDRIVERS_MESSAGE, (WPARAM)(driver), (LPARAM)DirverCount.nFileCount);
			SendMessage(this->m_hWnd, WM_FILE_DESKTOP_MESSAGE, m_pClientSocket->GetWorkingSocket(), (LPARAM)&desktop);
		}
		else
		{
			OutputDebugString("CFileClient::ReceiveDriverList() !IsWindow(m_hWnd)\n");
		}

		delete []driver;
		return true;
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClient::ReceiveDriverList() unkown exception.");
	}
	return false;
}


bool CFileClient::ResponseSendFileRequest()
{
	try
	{
		FileToSend filetosend;
		if(!m_pClientSocket->ReadExact(m_pClientSocket->GetWorkingSocket(), (char*)(&filetosend), sizeof(FileToSend)))
		{
			//AfxMessageBox("读取远端磁盘驱动器列表数量失败");
			return false;
		}

		bool bSending, bReceive;
		SendMessage(m_hWnd, WM_GETSENDING, (WPARAM)&bSending, 0);
		SendMessage(m_hWnd, WM_GETRECEIVE, (WPARAM)&bReceive, 0);
		if(bSending || bReceive)
		{
			return PackAndSendHeader(m_pClientSocket->GetWorkingSocket(), CMD_REJECTWHENNOTFREE, false);
		}

		CString strFileName = filetosend.FileName;
		int rfind = strFileName.ReverseFind('\\');
		if(rfind != -1)
		{
			strFileName = strFileName.Right(strFileName.GetLength() - rfind - 1);
		}

		this->m_strFileToReceive = strFileName;

		while(1)
		{
			m_strDistinationFolder = _T("");
			CFileAskDlg askdlg;
			askdlg.SetRapperHwnd(m_hWnd);
			CString strMsg;
			strMsg.Format(_T("客户请求向你传输文件%s，请确定是否接受？"), strFileName);
			askdlg.SetMsgText(strMsg);
			if(m_strDistinationFolder.IsEmpty())
				SendMessage(m_hWnd, WM_GETCURRENTPATH, 0, (LPARAM)&m_strDistinationFolder);
			askdlg.m_strDistinationFolder = m_strDistinationFolder;
			askdlg.m_strFileName = strFileName;//m_strRecFileName;
			SetPrompt(true,m_hWnd);

			if(IDOK == askdlg.DoModal())
			{
				SetPrompt(false,m_hWnd);
				PostMessage(m_hWnd, WM_CLOSEPROMPTDLG, 0, 0);
				m_strDistinationFolder = askdlg.m_strDistinationFolder; 
				m_strFileToReceive = strFileName;

				bool bRes = PackAndSendHeader(m_pClientSocket->GetWorkingSocket(), CMD_PARAM_TRANSFILE_S2C_SENDFILEACCEPT, false);
				if (!bRes)
					return false;

				FileTransProgress progress;
				ULONGLONG ullSize = 0;
				if (gl_mapllSize.find(m_hWnd) != gl_mapllSize.end())
				{
					ullSize = gl_mapllSize[m_hWnd];
				}
				progress.uDataRead = ullSize;
				bRes = m_pClientSocket->WriteExact(m_pClientSocket->GetWorkingSocket(), (char*)&progress, sizeof(FileTransProgress));
				if (!bRes)
					return false;
				CString strFullName;
				if (m_strDistinationFolder.Right(1) == _T("\\"))
					strFullName = m_strDistinationFolder + m_strFileToReceive;
				else
				{
					strFullName = m_strDistinationFolder + _T("\\");
					strFullName = strFullName + m_strFileToReceive;
				}
				SendMessage(m_hWnd, WM_REFRESHLOCALLIST, 0, (LPARAM)&strFullName);
				return true;
				//return PackAndSendRequest(CMD_PARAM_TRANSFILE_S2C_SENDFILEACCEPT);
			}
			else
			{
				SetPrompt(false,m_hWnd);
				PostMessage(m_hWnd, WM_CLOSEPROMPTDLG, 0, 0);
				return PackAndSendHeader(m_pClientSocket->GetWorkingSocket(), CMD_PARAM_TRANSFILE_S2C_SENDFILEREJECT, false);
				//return PackAndSendRequest(CMD_PARAM_TRANSFILE_S2C_SENDFILEACCEPT);
			}
		}
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClient::ResponseSendFileRequest() unkown exception.");
	}
	return false;
}