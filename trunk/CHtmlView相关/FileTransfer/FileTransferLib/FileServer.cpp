#include "StdAfx.h"
#include "FileServer.h"

#include "FileAskDlg.h"
#include "GlobalFun.h"

#include "FileClient.h"
#include "PromptDlg.h"

CFileServer::CFileServer(void)
{
	m_IsDestFolderPreviouslyExisted = false;

	char strSystemDirectory[MAX_PATH];
	GetSystemDirectory(strSystemDirectory, MAX_PATH);
	
	char strDesktop[MAX_PATH];
	LPITEMIDLIST pidl;
	SHGetSpecialFolderLocation(0, CSIDL_DESKTOP, &pidl);
	SHGetPathFromIDList(pidl, strDesktop);
	m_strReceivePath = strDesktop;

	m_bStopMyselft = false;

	if (gl_pLogger)
		gl_pLogger->log_info("CFileServer::CFileServer(void)");

	m_bWrapper = false;

	m_hWnd = NULL;
	m_pServerSocket = NULL;
}

CFileServer::~CFileServer(void)
{
	if (gl_pLogger)
		gl_pLogger->log_info("CFileServer::~CFileServer(void)");
}

//Remove local client folder
void CFileServer::RemoveDestFolder()
{
	try
	{
		CDestFolderHelper folderHelper;
		folderHelper.SetDestFolder(m_strReceivePath);
		if (folderHelper.IsDestFolderExist())
		{
			if (m_IsDestFolderPreviouslyExisted == false)
			{
				int nCount;
				BasicFileInfoList* pList = folderHelper.GetFileListInDestFolder(&nCount);

				if (pList == NULL)
				{
					::RemoveDirectory(m_strReceivePath);
				}
				else
				{
					while (pList != NULL)
					{
						BasicFileInfoList* pNext = pList->pNext;

						CString strFullFileName(m_strReceivePath);
						CString strFileName(pList->fileInfo.FileName);
						strFullFileName += "\\" + strFileName;
						DeleteFile(strFullFileName);

						delete pList;
						pList = pNext;
					}
					::RemoveDirectory(m_strReceivePath);
				}
			}
		}
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileServer::RemoveDestFolder() unkown exception.");
	}
}

/*
Main procedure to handle in File Server thread
Overriden from CThreadBase
*/
void CFileServer::ThreadProc()
{
	try
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileServer::ThreadProc()");

		int nDataToRead = sizeof(FilePackHeader);
		char* pFileBuf = new char[nDataToRead];

		if (!m_pServerSocket->ReadExact(m_pServerSocket->GetWorkingSocket(), pFileBuf, nDataToRead))
		{
			if (gl_pLogger)
				gl_pLogger->log_info("m_pServerSocket->ReadExact(for testing connection) for the first time return false.");
			//NotifyWrapperWindow(1);
		}
		else
		{
			OutputDebugString("CFileServer::ThreadProc ProcessFileHeader(pFileHeader)\n");
			FilePackHeader* pFileHeader = (FilePackHeader*)pFileBuf;
			if (!ProcessFileHeader(pFileHeader))
			{
				OutputDebugString("CFileServer::ThreadProc !ProcessFileHeader(pFileHeader)\n");
			}
		}

		while (m_pServerSocket->ReadExact(m_pServerSocket->GetWorkingSocket(), pFileBuf,nDataToRead))
		{
			FilePackHeader* pFileHeader = (FilePackHeader*)pFileBuf;

			if (!ProcessFileHeader(pFileHeader))
			{
				if (gl_pLogger)
					gl_pLogger->log_info("CFileServer !ProcessFileHeader");
				break;
			}
		}

		delete [] pFileBuf;

		//Quit after this function finishes
		this->MarkTheadToStop();
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileServer::ThreadProc() unkown exception.");
	}
}

//Close Socket, Close FileDialog, and Notify the Wrapper
void CFileServer::OnThreadStop()
{
	try
	{

	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileServer::OnThreadStop() unkown exception.");
	}
}

/*
Process File Server package header
pFileHeader = Pointer to the package header
*/
bool CFileServer::ProcessFileHeader(FilePackHeader* pFileHeader)
{
	try
	{
		switch (pFileHeader->nType)
		{
		case FILE_REQUEST:
			{
				switch (pFileHeader->nParam)
				{
				case CMD_PARAM_LISTFILE:
					{
						return ListFileInDestinationFolder();
					}

				case CMD_PARAM_TRANSFILE_C2S:
					{
						//RequestUser(true, _T("C:\\"), _T("a.exe"));
						return ReceiveFileFromClient();
					}

				case CMD_PARAM_TRANSFILE_S2C:
					{
						return SendFileToClient();
					}

				case CMD_PARAM_TRANSFILE_C2S_FINISH:
					{
						return true;
					}
				case CMD_PARAM_TRANSFILE_C2S_QUERYSTATUS:
					{
						FilePackHeader fph;
						fph.nType = FILE_RESPONSE;					
						fph.nParam = CMD_PARAM_TRANSFILE_C2S_STATUSOK;
						m_pServerSocket->WriteExact(m_pServerSocket->GetWorkingSocket(), (char*) &fph, sizeof(FilePackHeader));

						bool bContinue = false;
						SendMessage(m_hWnd, WM_GETSTOPSTATE, 0, (LPARAM)&bContinue);
						FilePackHeader fph1;
						fph1.nType = bContinue ? FILE_RESPONSE : FILE_REQUEST;					
						fph1.nParam = CMD_PARAM_TRANSFILE_SSTOPRECEIVE;
						m_pServerSocket->WriteExact(m_pServerSocket->GetWorkingSocket(), (char*) &fph1, sizeof(FilePackHeader));
						if(!bContinue)
						{
							m_bStopMyselft = true;
							State state;
							state = State::StopReceive;
							SendMessage(m_hWnd, WM_SETLISTITEM, 0, (LPARAM)&state);
							SendMessage(this->m_hWnd, WM_FILE_SENDFILETOSERVER_PROGRESS_MESSAGE, 0, 0);
						}
						return true; 
					}
				case CMD_PARAM_TRANSFILE_C2S_STOP:
					{
						return true;
					}
				case CMD_PARAM_DRIVER:
					{
						return SendDrivers();
					}
				case CMD_PARAM_TRANSFILE_C2S_REQUEST:
					{
						return ResponseC2S();
					}
				case CMD_PARAM_TRANSFILE_S2C_REQUEST:
					{
						return ResponseS2C();
					}
				case CMD_PARAM_TESTCONNECTION:
					return true;
				}
			}
		case FILE_RESPONSE:
			{
				switch (pFileHeader->nParam)
				{
				case CMD_REJECTWHENNOTFREE:
					SendMessage(m_hWnd, WM_FREEUI, 0, 0);
					return true;
				case CMD_PARAM_TRANSFILE_S2C_SENDFILEACCEPT:
					{
						//Send ready to send file to client.
						char pathname[MAX_PATH];
						SendMessage(this->m_hWnd, WM_GETSENDFILEPATHNAME, (WPARAM)pathname, 0);
						CString strFilePathName = pathname;
						int nfind = strFilePathName.ReverseFind('\\');
						if(nfind != -1)
						{
							this->m_strReceivePath = strFilePathName.Left(nfind);
							this->m_strRecFileName = strFilePathName.Right(strFilePathName.GetLength() - nfind -1);
						}

						FileTransProgress progress;
						bool bRes = m_pServerSocket->ReadExact(m_pServerSocket->GetWorkingSocket(), (char*)&progress, sizeof(FileTransProgress));
						if (!bRes)
							return false;
						//gl_llSize = progress.uDataRead;
						gl_mapllSize[m_hWnd] = progress.uDataRead;
						return PackAndSendRequest(CMD_PARAM_TRANSFILE_S2C_ACEEPT);
						//if(!bRes)
						//	return false;
						//return SendFileToClient();
					}
				case CMD_PARAM_TRANSFILE_S2C_SENDFILEREJECT:
					{
						SendMessage(this->m_hWnd, WM_SETSENDING, 0, 0);
						State state;
						state = State::CRejectReceive;
						SendMessage(m_hWnd, WM_SETLISTITEM, 0, (LPARAM)&state);
						PostMessage(m_hWnd, WM_REJECT_SENDREQUEST, 0, 0);
						CString strStatus = _T("");
						SendMessage(m_hWnd, WM_SETSTSTUS_TEXT, 0, (LPARAM)&strStatus);
						return true;
					}
				case CMD_PARAM_TESTCONNECTION:
						return true;
				}
			}
		}
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileServer::ProcessFileHeader() unkown exception.");
	}
	return false;
}

/*
List files in the local client folder and send the list to the File Client
*/
bool CFileServer::ListFileInDestinationFolder()
{
	try
	{
		//***Pan peng 2008-11-28
		RemoteFolderPath path;
		if (!m_pServerSocket->ReadExact(m_pServerSocket->GetWorkingSocket(), (char*)(&path), sizeof(RemoteFolderPath)))
			return false;

		//if(m_strReceivePath.Find(" 桌面") != -1)
		if(m_strReceivePath.Find(szDesktop_CH) != -1 && m_strReceivePath.Find(szDesktop_EN) != -1)
		{
			char strDesktop[MAX_PATH];
			LPITEMIDLIST pidl;
			SHGetSpecialFolderLocation(0, CSIDL_DESKTOP, &pidl);
			SHGetPathFromIDList(pidl, strDesktop);
			m_strReceivePath = strDesktop;
		}
		else
			m_strReceivePath = path.RemotePath;
		//end***
		CDestFolderHelper folderHelper;
		folderHelper.SetDestFolder(m_strReceivePath);
		FileCount structFileCount;

		if (folderHelper.IsDestFolderExist())
		{
			int nFileCount = 0;
			BasicFileInfoList* pFileList = folderHelper.GetFileListInDestFolder(&nFileCount);

			if (pFileList == NULL)
			{
				structFileCount.nFileCount = 0;
				return SendListFileResponseHeader(structFileCount);
			}
			else
			{
				structFileCount.nFileCount = nFileCount;
				SendListFileResponseHeader(structFileCount);

				bool bResult = SendLocalFileInfo(pFileList);

				while (pFileList != NULL)
				{
					BasicFileInfoList* pNext =  pFileList->pNext;
					delete pFileList;
					pFileList = pNext;
				}

				return bResult;
			}
		}
		else
		{
			structFileCount.nFileCount = 0;
			return SendListFileResponseHeader(structFileCount);
		}
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileServer::ProcessFileHeader() unkown exception.");
	}
	return false;
}

//Send List File count info as ResponseHeader to File Client
bool CFileServer::SendListFileResponseHeader(FileCount FileCountInfo)
{
	try
	{
		FilePackHeader header;
		header.nType = FILE_RESPONSE;
		header.nParam = CMD_PARAM_LISTFILE;
		if (!m_pServerSocket->WriteExact(m_pServerSocket->GetWorkingSocket(), (char*)(&header), sizeof(header)))
		{
			return false;
		}

		return m_pServerSocket->WriteExact(m_pServerSocket->GetWorkingSocket(), (char*)(&FileCountInfo), sizeof(FileCountInfo));
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileServer::SendListFileResponseHeader() unkown exception.");
	}
	return false;
}

/*
Send info of a local file to File Client
pFileList = Pointer to the BasicFileInfoList which contains local file Info
*/
bool CFileServer::SendLocalFileInfo(BasicFileInfoList* pFileList)
{
	try
	{
		while (pFileList != NULL)
		{
			if (!m_pServerSocket->WriteExact(m_pServerSocket->GetWorkingSocket(), (char*)(&(pFileList->fileInfo)), sizeof(BasicFileInfo)))
			{
				return false;
			}

			pFileList = pFileList->pNext;
		}

		return true;
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileServer::SendLocalFileInfo() unkown exception.");
	}
	return false;
}

//Receive a file from File Client
bool CFileServer::ReceiveFileFromClient()
{
	try
	{
		SendMessage(m_hWnd, WM_SETRECEIVE, 1, 0);
		State fstate;
		fstate = State::Receiving;
		SendMessage(m_hWnd, WM_SETLISTITEM, 0, (LPARAM)&fstate);

		CString DestFileName(m_strReceivePath);
		if(DestFileName.GetAt(DestFileName.GetLength()-1) != '\\')
			DestFileName += "\\";
		//DestFileName += fileInfo.FileName;
		DestFileName += m_strRecFileName;
		ULONGLONG ullSize = 0;
		if (gl_mapllSize.find(m_hWnd) != gl_mapllSize.end())
		{
			ullSize = gl_mapllSize[m_hWnd];
		}
		FileState state = ReceiveFile(m_pServerSocket->GetWorkingSocket(), m_hWnd, DestFileName, ullSize);
		if (gl_pLogger) gl_pLogger->log_info("ReceiveFile return %d", state);
		return ::ProcessState(state, m_hWnd, false);
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileServer::ReceiveFileFromClient() unkown exception.");
	}
	return false;
}

//Pack a Request struct and send it to File Client
bool CFileServer::PackAndSendRequest(short nParam)
{
	try
	{
		FilePackHeader header;
		header.nType = FILE_REQUEST;
		header.nParam = nParam;

		return m_pServerSocket->WriteExact(m_pServerSocket->GetWorkingSocket(), (char*)(&header), sizeof(header));
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileServer::PackAndSendRequest() unkown exception.");
	}
	return false;
}

//Send a file to File Client
bool CFileServer::SendFileToClient()
{
	try
	{
		SendMessage(m_hWnd, WM_SETSENDING, 1, 0);


		CString strSendFilePath;
		CString strSendFileName;

		if(!m_strSendPath.IsEmpty())
			strSendFilePath = m_strSendPath;
		else
			SendMessage(m_hWnd, WM_GETSENDPATH, 0, (LPARAM)&strSendFilePath);

		if(!m_strSendFileName.IsEmpty())
			strSendFileName = m_strSendFileName;
		else
			SendMessage(m_hWnd, WM_GETFILENAMETOSEND, 0, (LPARAM)&strSendFileName);

		if(strSendFilePath.Right(1) != _T("\\"))
			strSendFilePath = strSendFilePath + _T("\\");

		CString strFilePathName = strSendFilePath + strSendFileName;

		m_strSendPath = _T("");
		m_strSendFileName = _T("");
		ULONGLONG ullSize = 0;
		if (gl_mapllSize.find(m_hWnd) != gl_mapllSize.end())
		{
			ullSize = gl_mapllSize[m_hWnd];
		}
		FileState state = SendFile(m_pServerSocket->GetWorkingSocket(), m_hWnd, strFilePathName, ullSize);
		if (gl_pLogger) gl_pLogger->log_info("SendFile return %d", state);
		return ProcessState(state, m_hWnd);
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileServer::SendFileToClient() unkown exception.");
	}
	return false;
	
}

//***Add by Pan peng 2008-11-27
bool CFileServer::SendDrivers()
{
	try
	{
		if(!PackAndSendRequest(CMD_PARAM_DRIVER))
		{
			if (gl_pLogger)
				gl_pLogger->log_info("CFileServer::SendDrivers() unkown exception.");
			OutputDebugString("CFileServer::SendDrivers() unkown exception.\n");
			return false;
		}
		FileCount DirverCount;
		Driver *driver = GetDrivers(DirverCount.nFileCount);
		Driver *p = driver;
		bool bRes = false;
		if(DirverCount.nFileCount > 0)
		{
			OutputDebugString("CFileServer::SendDrivers ...\n");
			bRes = m_pServerSocket->WriteExact(m_pServerSocket->GetWorkingSocket(), (char*)(&DirverCount), sizeof(DirverCount));
			if(bRes)
			{
				while(p)
				{
					bool bResLoopSend =	m_pServerSocket->WriteExact(m_pServerSocket->GetWorkingSocket(), (char*)(p), sizeof(Driver));
					if(!bResLoopSend)
					{
						if(StrCmp(gl_strLanguageId, CHINESE) == 0)
							AfxMessageBox(szSendDirverFailed_CH);
						else
							AfxMessageBox(szSendDirverFailed_EN);

						break;
					}
					else
					{
						OutputDebugString("CFileServer::Send a Driver successfull.\n");
						OutputDebugString(p->DriverName);
						OutputDebugString("\n");
					}
					p = p->next;
				}
			}
			else
			{
				DirverCount.nFileCount = 0;
				bRes = m_pServerSocket->WriteExact(m_pServerSocket->GetWorkingSocket(), (char*)(&DirverCount), sizeof(DirverCount));
			}
		}
		FileToSend desktop;
		/*LPITEMIDLIST pidl;
		SHGetSpecialFolderLocation(0, CSIDL_DESKTOP, &pidl);
		SHGetPathFromIDList(pidl, desktop.FileName);*/
		memset(desktop.FileName, 0, MAX_PATH);
		GetLocalRemotePath(desktop.FileName);
		this->m_strReceivePath = desktop.FileName;

		bRes = m_pServerSocket->WriteExact(m_pServerSocket->GetWorkingSocket(), (char*)(&desktop), sizeof(FileToSend));
		//delete disk driver list.
		while(driver)
		{
			p = driver;
			driver = driver->next;
			delete p;
		}

		return true;
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileServer::SendDrivers() unkown exception.");
	}
	return false;
}

Driver *CFileServer::GetDrivers(int &Count)
{
	try
	{
		TCHAR szDrivesList[256]; // Format when filled : "C:\<NULL>D:\<NULL>....Z:\<NULL><NULL>"
		TCHAR szDrive[4];
		TCHAR szTheDrive[32];
		// TCHAR szName[255];
		TCHAR szType[16];
		UINT nType = 0;
		DWORD dwLen;
		int nIndex = 0;
		dwLen = GetLogicalDriveStrings(256, szDrivesList);
		Driver *driver = NULL;
		Driver *p = NULL;
		int k = 0;

		//Add the desktop to the driver list
		driver = new Driver;
		if(StrCmp(gl_strLanguageId, CHINESE) == 0)
			sprintf_s(driver->DriverName, "%s", szDesktop_CH);
		else
			sprintf_s(driver->DriverName, "%s", szDesktop_EN);
		p = driver;
		k ++;
		if(StrCmp(gl_strLanguageId, CHINESE) == 0)
			sprintf_s(szType, "%s", szLocalDisk_CH);
		else
			sprintf_s(szType, "%s", szLocalDisk_EN);


		// Parse the list of drives	
		while (nIndex < (int)(dwLen - 3))
		{
			strcpy_s(szDrive, szDrivesList + nIndex);
			nIndex += 4;
			szDrive[2] = '\0'; // remove the '\'
			sprintf_s(szTheDrive, "(%s)", szDrive);

			// szName[0] = '\0';
			szType[0] = '\0';

			strcat_s(szDrive, "\\");

			// GetVolumeInformation(szDrive, szName, sizeof(szName), NULL, NULL, NULL, NULL, NULL);

			// Get infos on the Drive (type and Name)
			nType = GetDriveType(szDrive);
			switch (nType)
			{
			case DRIVE_FIXED:
				if(StrCmp(gl_strLanguageId, CHINESE) == 0)
					sprintf_s(szType, "%s", szLocalDisk_CH);
				else
					sprintf_s(szType, "%s", szLocalDisk_EN);
				break;

			case DRIVE_REMOVABLE:
				if(StrCmp(gl_strLanguageId, CHINESE) == 0)
					sprintf_s(szType, "%s", szFloppy_CH);
				else
					sprintf_s(szType, "%s", szFloppy_EN);
				break;

			case DRIVE_CDROM:
				sprintf_s(szType, "%s", "CD-ROM");
				break;

			case DRIVE_REMOTE:
				if(StrCmp(gl_strLanguageId, CHINESE) == 0)
					sprintf_s(szType, "%s", szNetDriver_CH);
				else
					sprintf_s(szType, "%s", szNetDriver_EN);
				break;

			default:
				if(StrCmp(gl_strLanguageId, CHINESE) == 0)
					sprintf_s(szType, "%s", szUnknowDriver_CH);
				else
					sprintf_s(szType, "%s", szUnknowDriver_EN);
				break;
			}

			// Prepare it for Combo Box and add it
			strcat_s(szTheDrive, " - ");
			strcat_s(szTheDrive, szType);

			Driver *newDriver = new Driver;
			sprintf_s(newDriver->DriverName, szTheDrive);
			if(NULL == driver)
			{
				driver = newDriver;
				p = driver;
			}
			else
			{
				p->next = newDriver;
				p = newDriver;
			}
			k++;
		}
		Count = k;
		return driver;
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileServer::GetDrivers() unkown exception.");
	}
	return NULL;
}

CString CFileServer::GetAskMessage(bool bSend, LPCTSTR strPath, LPCTSTR strFileName)
{
	try
	{
		CString strMsg;
		if(bSend)
		{
			if(StrCmp(gl_strLanguageId, CHINESE) == 0)
			{
				strMsg = szFileRequestSp_CH;
				strMsg = strMsg + strFileName ;
				strMsg = strMsg + szTo_CH;
				strMsg = strMsg + strPath + szAcceptReciveFile_CH;
			}
			else
			{
				strMsg = szFileRequestSp_EN;
				strMsg = strMsg + strFileName ;
				strMsg = strMsg + szTo_EN;
				strMsg = strMsg + strPath + szAcceptReciveFile_EN;
			}
		}
		else
		{
			if(StrCmp(gl_strLanguageId, CHINESE) == 0)
			{
				strMsg = szSpGetFileRequest_CH;
				strMsg = strMsg + strPath ;
				strMsg = strMsg + _T("\\") ;
				strMsg = strMsg + strFileName + szAcceptSendFile_CH;
			}
			else
			{
				strMsg = szSpGetFileRequest_EN;
				strMsg = strMsg + strPath ;
				strMsg = strMsg + _T("\\") ;
				strMsg = strMsg + strFileName + szAcceptSendFile_EN;
			}
		}
		//return MessageBox(NULL, strMsg, _T("询问提示"), MB_YESNO);
		return strMsg;
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileServer::GetAskMessage() unkown exception.");
	}
	return "";
}

bool CFileServer::ResponseC2S()
{
	try
	{
		FileRequest filerequest;
		bool bResult = m_pServerSocket->ReadExact(m_pServerSocket->GetWorkingSocket(), (char*)(&filerequest), sizeof(filerequest));
		if(!bResult)
			return false;

		BasicFileInfo fileInfo;
		if (!m_pServerSocket->ReadExact(m_pServerSocket->GetWorkingSocket(), (char*)(&fileInfo), sizeof(BasicFileInfo)))
		{
			return false;
		}
		this->m_strRecFileName = fileInfo.FileName;
		//gl_bOverWrite = fileInfo.bOverWrite;
		gl_mapOverWrite[m_hWnd] = fileInfo.bOverWrite;
		bool bSending, bReceive;
		SendMessage(m_hWnd, WM_GETSENDING, (WPARAM)&bSending, 0);
		SendMessage(m_hWnd, WM_GETRECEIVE, (WPARAM)&bReceive, 0);
		if(bSending || bReceive)
		{
			return PackAndSendHeader(m_pServerSocket->GetWorkingSocket(), CMD_REJECTWHENNOTFREE, false);
		}

		State state;
		state = State::CRequestSend;

		BasicFileInfo info;
		info.bOverWrite = fileInfo.bOverWrite;
		info.CreationTime = fileInfo.CreationTime;

		CString strPath = this->m_strReceivePath;
		if (strPath.Right(1) != _T("\\"))
			strPath = strPath + _T("\\");
		strPath = strPath + fileInfo.FileName;
		lstrcpy(info.FileName, strPath);
		info.Size = fileInfo.Size;

		SendMessage(m_hWnd, WM_ADDLISTITEM, (WPARAM)&info, (LPARAM)&state);
		SendMessage(m_hWnd, WM_SETRECEIVE, 1, 0);
		m_FileInfo = fileInfo;

		CString strMsg = GetAskMessage(true, filerequest.FilePath, m_strRecFileName);
		CPromptDlg dlg;
		dlg.SetMsgText(strMsg);
		dlg.SetHwnd(m_hWnd);
		SetPrompt(true,m_hWnd);
		bool bAccept = gl_bAutoControl ? true : (IDOK == dlg.DoModal());
		if(bAccept)
		{
			PostMessage(m_hWnd, WM_CLOSEPROMPTDLG, 0, 0);
			SetPrompt(false,m_hWnd);
			//don't need to show ask dialog.
			PostMessage(m_hWnd, WM_CLOSEPROMPTDLG, 0, 0);
			bool bRes = PackAndSendRequest(CMD_PARAM_TRANSFILE_C2S_ACEEPT);
			if (!bRes)
				return false;

			bool bOverWrite = false;
			if (gl_mapOverWrite.find(m_hWnd) != gl_mapOverWrite.end())
			{
				bOverWrite = gl_mapOverWrite[m_hWnd];
			}
			else
			{
				bOverWrite = false;
			}

			//if (!gl_bOverWrite)
			if (!bOverWrite)
			{
				CString strFileName = filerequest.FilePath;
				strFileName = strFileName + _T("\\");
				strFileName = strFileName + m_strRecFileName;
				CFile file;
				if (!file.Open(strFileName, CFile::modeRead))
					//gl_llSize = 0;
					gl_mapllSize[m_hWnd] = 0;
				else
				{
					//gl_llSize = file.GetLength();
					gl_mapllSize[m_hWnd] = file.GetLength();
					file.Close();
				}
			}
			else
				//gl_llSize = 0;
				gl_mapllSize[m_hWnd] = 0;
			FileTransProgress progress;
			ULONGLONG ullSize = 0;
			if (gl_mapllSize.find(m_hWnd) != gl_mapllSize.end())
			{
				ullSize = gl_mapllSize[m_hWnd];
			}
			progress.uDataRead = ullSize;
			if (!m_pServerSocket->WriteExact(m_pServerSocket->GetWorkingSocket(), (char*)(&progress), sizeof(FileTransProgress)))
			{
				return false;
			}
			return true;
		}
		else
		{
			SetPrompt(false,m_hWnd);
			PostMessage(m_hWnd, WM_CLOSEPROMPTDLG, 0, 0);
			SendMessage(m_hWnd, WM_SETRECEIVE, 0, 0);
			State state;
			state = State::SRejectReceive;
			SendMessage(m_hWnd, WM_SETLISTITEM, (WPARAM)&fileInfo, (LPARAM)&state);
			PostMessage(m_hWnd, WM_CLOSEPROMPTDLG, 0, 0);
			return PackAndSendRequest(CMD_PARAM_TRANSFILE_C2S_REJECT);
		}
		return false;

		//

		while(1)
		{
			CFileAskDlg askdlg;
			askdlg.SetRapperHwnd(m_hWnd);
			CString strMsg = GetAskMessage(true, filerequest.FilePath, m_strRecFileName);
			askdlg.SetMsgText(strMsg);
			askdlg.m_strDistinationFolder = filerequest.FilePath;
			askdlg.m_strFileName = m_strRecFileName;
			SetPrompt(true,m_hWnd);

			bool bAccept = gl_bAutoControl ? true : (IDOK == askdlg.DoModal());
			if(bAccept)
			{
				m_strReceivePath = askdlg.m_strDistinationFolder; 
				SetPrompt(false,m_hWnd);
				PostMessage(m_hWnd, WM_CLOSEPROMPTDLG, 0, 0);
				return PackAndSendRequest(CMD_PARAM_TRANSFILE_C2S_ACEEPT);
			}
			else
			{
				SetPrompt(false,m_hWnd);
				PostMessage(m_hWnd, WM_CLOSEPROMPTDLG, 0, 0);
				SendMessage(m_hWnd, WM_SETRECEIVE, 0, 0);
				State state;
				state = State::SRejectReceive;
				SendMessage(m_hWnd, WM_SETLISTITEM, (WPARAM)&fileInfo, (LPARAM)&state);
				return PackAndSendRequest(CMD_PARAM_TRANSFILE_C2S_REJECT);
			}
		}
		return true;
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileServer::ResponseC2S() unkown exception.");
	}
	return false;
}

bool CFileServer::ResponseS2C()
{
	try
	{
		FileRequest filerequest;
		bool bResult = m_pServerSocket->ReadExact(m_pServerSocket->GetWorkingSocket(), (char*)(&filerequest), sizeof(filerequest));
		if(!bResult)
			return false;

		FileToSend fileInfo;
		if (!m_pServerSocket->ReadExact(m_pServerSocket->GetWorkingSocket(), (char*)(&fileInfo), sizeof(fileInfo)))
		{
			return false;
		}

		//gl_llSize = fileInfo.llStart;
		gl_mapllSize[m_hWnd] = fileInfo.llStart;
		bool bSending, bReceive;
		SendMessage(m_hWnd, WM_GETSENDING, (WPARAM)&bSending, 0);
		SendMessage(m_hWnd, WM_GETRECEIVE, (WPARAM)&bReceive, 0);
		if(bSending || bReceive)
		{
			return PackAndSendHeader(m_pServerSocket->GetWorkingSocket(), CMD_REJECTWHENNOTFREE, false);
		}

		CString strPath = filerequest.FilePath;
		CString strPathName = filerequest.FileName;
		if(strPath.Right(1) != _T("\\"))
			strPath = strPath + _T("\\");
		strPathName = strPath + strPathName;
		BasicFileInfo info;
		CString strFileName = strPathName;
		strcpy_s(info.FileName, sizeof(info.FileName), strFileName);
		info.Size = 0;
		CFile file;
		if(file.Open(strPathName, CFile::modeRead))
		{
			info.Size = file.GetLength();
			file.Close();
		}
		State state;
		state = State::RequestGet;
		SendMessage(m_hWnd, WM_ADDLISTITEM, (WPARAM)&info, (LPARAM)&state);
		SendMessage(m_hWnd, WM_SETSENDING, 1, 0);

		this->m_strSendFileName = fileInfo.FileName;

		CString strMsg = GetAskMessage(false, filerequest.FilePath, filerequest.FileName);
		//This code is different from rising version.
		//if(IDYES == MessageBox(NULL, strMsg, _T("询问提示"), MB_YESNO))
		CPromptDlg dlg;
		dlg.SetHwnd(m_hWnd);
		SetPrompt(true,m_hWnd);
		dlg.SetMsgText(strMsg);

		bool bAccept = gl_bAutoControl ? true : (IDOK == dlg.DoModal());
		if(bAccept)
		{
			PostMessage(m_hWnd, WM_CLOSEPROMPTDLG, 0, 0);
			m_strSendPath = filerequest.FilePath;
			SetPrompt(false,m_hWnd);
			return PackAndSendRequest(CMD_PARAM_TRANSFILE_S2C_ACEEPT);
		}
		else
		{
			State state;
			state = State::RejectGet;
			SendMessage(m_hWnd, WM_SETLISTITEM, (WPARAM)&fileInfo, (LPARAM)&state);
			SendMessage(m_hWnd, WM_SETSENDING, 0, 0);
			SetPrompt(false,m_hWnd);
			PostMessage(m_hWnd, WM_CLOSEPROMPTDLG, 0, 0);
			return PackAndSendRequest(CMD_PARAM_TRANSFILE_S2C_REJECT);
		}
		return true;
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileServer::ResponseS2C() unkown exception.");
	}
	return false;
}

void CFileServer::GetLocalRemotePath(char *pPath)
{
	try
	{
		char szMyRemotePath[MAX_PATH];
		memset(szMyRemotePath, 0, MAX_PATH);
		strcpy_s(szMyRemotePath, MAX_PATH, DESTINATION_FOLDER);

		CFileFind find;
		BOOL bRes = find.FindFile(szMyRemotePath);
		if (bRes)
		{
			find.FindNextFile();
			if (find.IsDirectory())
			{
				lstrcpy(pPath, DESTINATION_FOLDER);
				return;
			}
		}
		if (!bRes)	//create the directory
		{
			bRes = ::CreateDirectory(DESTINATION_FOLDER, NULL);
			if (bRes) 
			{
				lstrcpy(pPath, DESTINATION_FOLDER);
				return;
			}
		}

		if (!bRes)
		{
			char strSystemDirectory[MAX_PATH];
			UINT i = GetSystemDirectory(strSystemDirectory, MAX_PATH);
			if (i > 3)
				memcpy(szMyRemotePath, strSystemDirectory, 3);

			bRes = find.FindFile(szMyRemotePath);
			if (bRes)
			{
				find.FindNextFile();
				if (find.IsDirectory())
				{
					lstrcpy(pPath, szMyRemotePath);
					return;
				}
			}

			bRes = ::CreateDirectory(szMyRemotePath, NULL);
			if (bRes) 
			{
				lstrcpy(pPath, szMyRemotePath);
				return;
			}
		}
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileServer::GetLocalRemotePath() unkown exception.");
	}
}