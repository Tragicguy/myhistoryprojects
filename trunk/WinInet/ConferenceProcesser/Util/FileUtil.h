#pragma once

#include "_log.h"
#include "shlobj.h"

const int m_nFileUsedCount = 20;
const int m_nFileTotalCount = 25;

class CFileUtil
{
public:
	// Wait for the close of a file for nTimeOut seconds
	static int WaitForFileClose(LPCSTR strFileName, int nTimeOut)
	{
		for (int nSleepCount = 0; nSleepCount < nTimeOut; nSleepCount ++)
		{
			CFile hookFile;
			CFileException e;
			CFileStatus status;

			if (!CFile::GetStatus(strFileName, status ) )   // static function
			{
				return nSleepCount;
			}
			if (!hookFile.Open(strFileName, CFile::modeRead | CFile::shareDenyNone, &e))
			{
				return nSleepCount;
			}
			hookFile.Close();
			
			if (hookFile.Open(strFileName, CFile::modeRead | CFile::shareExclusive, &e))
			{
				return nSleepCount;
			}
			Sleep(1000);
		}
		return nTimeOut;
	}

	static int TryDeleteFile(LPCSTR strFileName, int nTimeOut)
	{
		for (int nSleepCount = 0; nSleepCount < nTimeOut; nSleepCount ++)
		{
			try
			{
				CFile::Remove(strFileName);
				return nSleepCount;
			}
			catch (CFileException *e)
			{
				if (e->m_cause == CFileException::fileNotFound)
				{
					e->Delete();
					return nSleepCount;
				}
				e->Delete();
			}
			Sleep(1000);
		}
		return nTimeOut;
	}

	static void SaveTryRenameFile(LPCSTR strFileName, int nTimeOut)
	{
		for (int nSleepCount = 0; nSleepCount < nTimeOut; nSleepCount ++)
		{
			try
			{
				CFile::Rename(strFileName, "aaa");
				CFile::Rename(strFileName, "aaa");
			}
			catch (CFileException *e)
			{
				if (e->m_cause == CFileException::fileNotFound)
				{
					e->Delete();
					break;
				}
				e->Delete();
			}
			Sleep(1000);
		}
	}

	// Check for the close of a file for nTimeOut seconds
	static bool CheckOpenFile(LPCSTR strFileName)
	{
		CFile hookFile;
		CFileException e;
		CFileStatus status;
	
		if (!CFile::GetStatus(strFileName, status ) )   // static function
		{
			return true;
		}
		if (!hookFile.Open(strFileName, CFile::modeRead | CFile::shareDenyNone, &e))
		{
			return true;
		}
		hookFile.Close();

		if (hookFile.Open(strFileName, CFile::modeRead | CFile::shareExclusive, &e))
		{
			hookFile.Close();
			return true;
		}
		return false;
	}

	static bool CheckDeleteFile(LPCSTR strFileName)
	{
		try
		{
			CFile::Remove(strFileName);
			return true;
		}
		catch (CFileException *e)
		{
			if (e->m_cause == CFileException::fileNotFound)
			{
				e->Delete();
				return true;
			}
			e->Delete();
		}
		return false;
	}

	static bool DeleteFileAfterReboot(LPCSTR strFilePath)
	{
		HANDLE hSearch;
		int nFileCount = 0;
		WIN32_FIND_DATA FileData;
		SetCurrentDirectory(strFilePath);
		hSearch = FindFirstFile("*.*", &FileData);
		if (hSearch == INVALID_HANDLE_VALUE) return FALSE;
		while (TRUE)
		{ 
			nFileCount ++;
			if (!FindNextFile(hSearch, &FileData)) break;
		}
		FindClose(hSearch);

		CString m_strFilePath = strFilePath;
		CString strFileList[m_nFileTotalCount] = {"AESEncryption.dll", "CFClient.exe", "CFServer.exe", "ChatLib.dll",
			"FileTransferLib.dll", "Licence.sfx", "RTSClient.ini", "RTSClientTransaction.exe", "RTSKeyGenDll.Dll",
			"RTSServer.ini", "RTSServerTransaction.exe", "rtsui.ini", "ShowMaticEncryptionDLL.dll", "VoIPLib.dll",
			"zRTSHks.dll", "RTSClient.exe", "RTSServer.log", "RTSClient.log", "Multiplexer.log", "RTSServerLite.exe",
			"KeyboardHook.dll", "", "", ""};
		CFileFind findFile;
		for (int k = 0; k < m_nFileTotalCount; k++)
		{
			if (strFileList[k] != "" && !findFile.FindFile(strFileList[k]))
			{
				for (int j = k; j < m_nFileTotalCount - 1; j++)
				{
					strFileList[j] = strFileList[j+1];
				}
				k--;
			}
		}

		if (m_strFilePath.Mid(m_strFilePath.GetLength()-1) == "\\")
			m_strFilePath = m_strFilePath.Mid(0,m_strFilePath.GetLength() - 1);

		OSVERSIONINFO osversioninfo;
		osversioninfo.dwOSVersionInfoSize = sizeof(osversioninfo);
		if (!GetVersionEx(&osversioninfo)) return FALSE;
		if (osversioninfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
		{
			int secPos;
			CString strSection;
			CString strFileContent;
			char sysFileName[MAX_PATH];
			GetWindowsDirectory(sysFileName, MAX_PATH+1);
			lstrcat(sysFileName, "\\Wininit.ini");

			try
			{
				CFileFind cff;
				CStdioFile csf;
				if (cff.FindFile(sysFileName))
				{
					csf.Open(sysFileName,CFile::modeRead | CFile::typeText);
					while(csf.ReadString(strSection))
					{
						strFileContent += strSection + "\n";
					}
					csf.Close();

					secPos = strFileContent.Find("[rename]");
					if (secPos == -1)
					{
						if (nFileCount > m_nFileUsedCount)
						{
							CString strTempFileList = "[rename]";
							for(int i = 0; i < m_nFileTotalCount; i++)
							{
								if(strFileList[i].Trim() == "") break;
								strTempFileList.Format("%s\nnul=%s\\%s", strTempFileList.Mid(0), m_strFilePath, strFileList[i]);
							}
							strFileContent.Format("%s\n%s", strTempFileList, strFileContent.Mid(0));
						}
						else
						{
							strFileContent.Format("[rename]\ndirnul=%s\n%s", m_strFilePath, strFileContent.Mid(0));
						}
					}
					else
					{
						if (nFileCount > m_nFileUsedCount)
						{
							CString strTempFileList;
							for (int i = 0; i < m_nFileTotalCount; i++)
							{
								if(strFileList[i].Trim() == "") break;
								strTempFileList.Format("%s\nnul=%s\\%s", strTempFileList.Mid(0), m_strFilePath,strFileList[i]);
							}
							strFileContent.Format("%s%s%s", strFileContent.Mid(0, secPos + 8), strTempFileList, strFileContent.Mid(secPos + 8));
						}
						else
						{
							strFileContent.Format("%s\ndirnul=%s%s", strFileContent.Mid(0, secPos + 8), m_strFilePath, strFileContent.Mid(secPos + 8));
						}
					}
				}
				else
				{
					if (nFileCount > m_nFileUsedCount)
					{
						strFileContent = "[rename]";
						for (int i = 0; i < m_nFileTotalCount; i++)
						{
							if(strFileList[i].Trim() == "") break;
							strFileContent.Format("%s\nnul=%s\\%s", strFileContent.Mid(0), m_strFilePath, strFileList[i]);
						}
					}
					else
					{
						strFileContent.Format("[rename]\ndirnul=%s", m_strFilePath);
					}
				}
				csf.Open(sysFileName, CFile::modeCreate | CFile::modeWrite);
				csf.SeekToBegin();
				csf.WriteString(strFileContent);
				csf.Close();
				cff.Close();
			}
			catch(...)
			{
				return FALSE;
			}
		}
		else
		{
			try
			{
				char strFileName[MAX_PATH];
				for (int i = 0; i < m_nFileTotalCount; i++)
				{
					if(strFileList[i].Trim() == "") break;
					lstrcpy(strFileName, m_strFilePath);
					lstrcat(strFileName, "\\");
					lstrcat(strFileName, strFileList[i]);
					MoveFileEx(strFileName, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
				}
				MoveFileEx(strFilePath, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
			}
			catch(...)
			{
				return FALSE;
			}
		}
		return TRUE;
	}

	static bool CreateTempDirectory(LPSTR szTempDir) 
	{ 
		//Get the temporary files directory. 
		TCHAR szTempPath [MAX_PATH];
		DWORD dwResult= GetTempPath(MAX_PATH, szTempPath);
		if (dwResult == 0)
			strncpy(szTempPath, "C:\\", sizeof(szTempPath) - 1);
		//Create a unique temporary file. 
		UINT nResult = 0;
		while(nResult == 0)
		{
			nResult = GetTempFileName(szTempPath, _T("W18"), 0, szTempDir);
			DeleteFile(szTempDir);
		}
		return ::CreateDirectory(szTempDir, NULL) == TRUE;
	}

public:
	static bool SaveFilesForNextSession(LPCSTR lpszSourceDir, LPCSTR strTempDir)
	{
		WIN32_FIND_DATA FileData;
		HANDLE hSearch;
		BOOL fFinished = FALSE;

		char strSavedCurrentDir[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, strSavedCurrentDir);
		SetCurrentDirectory(lpszSourceDir);
		hSearch = FindFirstFile("*.*", &FileData); 
		if (hSearch == INVALID_HANDLE_VALUE) 
		{
			return false;
		}

		TCHAR szNewFile [MAX_PATH];
		bool bSuccessful = false;
		while (true)
		{
			if ((FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
			{
				lstrcpy(szNewFile, strTempDir);
				lstrcat(szNewFile, FileData.cFileName);
				if (CopyFile(FileData.cFileName, szNewFile, TRUE) == FALSE)
				{
					break;
				}
			}
			if (!FindNextFile(hSearch, &FileData))
			{
				bSuccessful = (GetLastError() == ERROR_NO_MORE_FILES);
				break;
			}
		}
		FindClose(hSearch);
		SetCurrentDirectory(strSavedCurrentDir);
		return bSuccessful;
	}

	static bool RemoveFilesForNextSession(LPSTR strTempDir)
	{
		WIN32_FIND_DATA FileData;
		HANDLE hSearch;
		BOOL fFinished = FALSE;

		char strSavedCurrentDir[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, strSavedCurrentDir);
		SetCurrentDirectory(strTempDir);

		hSearch = FindFirstFile("*.*", &FileData);
		if (hSearch == INVALID_HANDLE_VALUE)
		{
			return false;
		}

		TCHAR szNewFile [MAX_PATH];
		bool bSuccessful = false;
		while (true)
		{
			if ((FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
			{
				lstrcpy(szNewFile, strTempDir);
				lstrcat(szNewFile, "\\");
				lstrcat(szNewFile, FileData.cFileName);
				if (DeleteFile(FileData.cFileName) == FALSE)
				{
					break;
				}
			}
			if (!FindNextFile(hSearch, &FileData))
			{
				bSuccessful = (GetLastError() == ERROR_NO_MORE_FILES);
				break;
			}
		}
		FindClose(hSearch);
		SetCurrentDirectory(strSavedCurrentDir);
		if (RemoveDirectory(strTempDir) == FALSE)
			return false;
		return bSuccessful;
	}

	static bool SetupRunOnce(LPCSTR strCommand)
	{
		HKEY hKey;
		if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce", 0, KEY_WRITE, &hKey) != ERROR_SUCCESS)
		{
			return false;
		}

		if (RegSetValueEx(hKey, "Web1800AutoStart", 0, REG_SZ, (BYTE *) strCommand , strlen(strCommand)) != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			return false;
		}

		RegCloseKey(hKey);
		return true;
	}

	static bool ClearRunOnce(bool bRemoveFiles = true)
	{
		HKEY hKey;
		if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce", 0, KEY_READ | KEY_WRITE, &hKey) != ERROR_SUCCESS)
		{
			return false;
		}

		char strCommand[MAX_PATH];
		DWORD dwBufLen = MAX_PATH;
		DWORD dwType = REG_SZ;
/*
		if (RegQueryValueEx (hKey, "Web1800AutoStart", NULL, &dwType, (BYTE *) strCommand , &dwBufLen) != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			return false;
		}

		if (dwType != REG_SZ || dwBufLen > MAX_PATH)
		{
			RegCloseKey(hKey);
			return false;
		}
*/
		if (RegQueryValueEx (hKey, "Web1800AutoStart", NULL, &dwType, (BYTE *)strCommand , &dwBufLen) != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			return false;
		}
		bool bSuccess = (RegDeleteValue(hKey, "Web1800AutoStart") == ERROR_SUCCESS);
		RegCloseKey(hKey);

		char *pName = strrchr (strCommand, '\\');
		if (pName != NULL)
		{
			pName[0] = '\0';
			if (bRemoveFiles)
				RemoveFilesForNextSession(strCommand);
		}
		// Delete files

		return bSuccess;
	}

	//static CString GetRequestBody(LPCSTR strFunctionName)
	//{
	//	CString strBody;
	//	strBody.Format("<%s xmlns=\"http://tempuri.org/\">\r\n</%s>", strFunctionName, strFunctionName);
	//	strBody.Format("<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n<soap:Envelope xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\">\r\n<soap:Body>\r\n%s\r\n</soap:Body>\r\n</soap:Envelope>", strBody.Mid(0));
	//	return strBody;
	//}

	//static CString GetRequestHeader(LPCSTR strFunctionName, int nContentLength)
	//{
	//	CString strHeader;
	//	strHeader.Format("Content-Type: text/xml; charset=utf-8\r\nContent-Length:%d\r\nSOAPAction: \"http://tempuri.org/%s\"\r\n", nContentLength, strFunctionName);
	//	return strHeader;
	//}
/*	Changed by ZhuJie 2008-03-11.
	static void WaitForThreadToTerminate(HANDLE hThread)
	{
		try
		{
			DWORD dwRet = ::MsgWaitForMultipleObjects(1, &hThread, FALSE, INFINITE, QS_ALLINPUT);
			while ((dwRet != WAIT_OBJECT_0) && (dwRet != WAIT_FAILED))
			{
				dwRet = ::MsgWaitForMultipleObjects(1, &hThread, FALSE, INFINITE, QS_ALLINPUT);
				if (dwRet != WAIT_OBJECT_0)
				{
					MSG msg;
					while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
				}
			}
		}
		catch (...) {}
	}
*/
	static void WaitForThreadToTerminate(HANDLE hThread)
	{
		try
		{
			if (!hThread) return;

			//modified by pan peng 2009-05-27
			DWORD dwRet;
			DWORD dwTimeOut = 5000;
			bool bTimeout = false;		
			do
			{
				dwRet = ::MsgWaitForMultipleObjects(1, &hThread, FALSE, dwTimeOut/*INFINITE*/, QS_ALLINPUT);
				if (dwRet == WAIT_OBJECT_0 + 1)
				{
					Sleep(50);
					break;
				}
				if (dwRet == WAIT_TIMEOUT)
				{
					DWORD dwExitCode = -1;
					::GetExitCodeThread(hThread, &dwExitCode);
					::TerminateThread(hThread, dwExitCode);
				}
				else if (dwRet != WAIT_OBJECT_0)
				{
					MSG msg;
					while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
					DWORD dwExitCode = -1;
					::GetExitCodeThread(hThread, &dwExitCode);
					::TerminateThread(hThread, dwExitCode);
				}
			} while ((dwRet != WAIT_OBJECT_0) && (dwRet != WAIT_FAILED));
		}
		catch (...) {}
	}

	static BOOL IsDirectoryExist(CString strPath)
	{
		WIN32_FIND_DATA wfd;
		HANDLE hFind = FindFirstFile(strPath, &wfd);
		if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			FindClose(hFind);
			return TRUE;
		}

		return FALSE;
	}

	static BOOL IsFileExist(CString strFileName)
	{
		CFileFind fFind;
		return fFind.FindFile(strFileName);
	}

	static BOOL CreateDirectory(CString strPath)
	{
		SECURITY_ATTRIBUTES attrib;
		attrib.bInheritHandle = FALSE;
		attrib.lpSecurityDescriptor = NULL;
		attrib.nLength = sizeof(SECURITY_ATTRIBUTES);

		return ::CreateDirectory(strPath, &attrib);
	}

	//2010-03-29 for auto build webchat after rts client reboot
	// Reboot win NT/2000/xp
	static BOOL RebootNTx86()
	{
		HANDLE hToken = 0, hThread = 0;
		DWORD dwRtn = 0;
		hThread =(HANDLE)AfxGetApp()->m_hThread;

		TOKEN_PRIVILEGES tp;
		LUID luid;
		tp.PrivilegeCount = 1;

		if (!::OpenProcessToken(GetCurrentProcess(),
			TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		{
			AfxMessageBox(_T("Current proccess can't apply for the reboot priviledge!"));
			return FALSE;
		}
		else
		{
			if (!LookupPrivilegeValue(NULL,    // lookup privilege on local system
				SE_SHUTDOWN_NAME,				// privilege to lookup 
				&luid))
			{
				//AfxMessageBox(_T("当前进程没有重启电脑权限，请手动重启！"));
				return FALSE;
			}
			else
			{
				// receives LUID of privilege
				tp.PrivilegeCount = 1;
				tp.Privileges[0].Luid = luid;
				tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

				// Enable the privilege or disable all privileges.       
				AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES),
					&tp, &dwRtn);
				if (GetLastError() == ERROR_SUCCESS)
					return ExitWindowsEx(EWX_REBOOT, 0);
				else
				{
					//AfxMessageBox(_T("当前进程没有重启电脑权限，请手动重启！"));
					return FALSE;
				}
			}
			CloseHandle(hToken);
		}
		return FALSE;
	}

	static BOOL RestartWindow()
	{
		BOOL bRestarted = FALSE;
		try
		{
			bRestarted = ExitWindowsEx(EWX_REBOOT, 0);
		}
		catch(...){}
		if (bRestarted) return TRUE;

		//AfxMessageBox("Rebooting ......");
		//return TRUE;
		// 判断当前使用的操作系统，重新启动系统
		DWORD dwVersion = GetVersion();
		if (dwVersion < 0x80000000)// Windows NT/2000/XP
		{ 
			return RebootNTx86();   
		}
		else// Windows 9x/ME
		{       
			return ExitWindowsEx(EWX_REBOOT, 0);// Reboot win 9x/me
		}
		return FALSE;
	}

	static bool CreateShotCut(CString sExePath, CString sSavePath)
	{
		HRESULT hr = CoInitialize(NULL);
		if (SUCCEEDED(hr))
		{
			IShellLink *pisl;
			hr = CoCreateInstance(CLSID_ShellLink, NULL,
				CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pisl);
			if (SUCCEEDED(hr))
			{
				IPersistFile* pIPF;
				pisl->SetPath(sExePath);//L"c:\\windows\\notepad.exe");
				hr = pisl->QueryInterface(IID_IPersistFile, (void**)&pIPF);
				if (SUCCEEDED(hr))
				{
					USES_CONVERSION;
					LPCOLESTR lpo = A2COLE(sSavePath.GetBuffer());
					pIPF->Save(lpo, FALSE);
					pIPF->Release();
					return true;
				}
				pisl->Release();
			}
			CoUninitialize();
		}
		return false;
	}

	static CString GetSpecialFolderLocation(int nFolderType)
	{
		CString strPath = _T("");
		TCHAR szPath[MAX_PATH] = {0};   
		TCHAR szShortPath[MAX_PATH] = {0};

		LPITEMIDLIST pidl=NULL;   
		HRESULT hRes = SHGetSpecialFolderLocation(NULL, nFolderType, &pidl);   
		if (pidl && SHGetPathFromIDList(pidl, szPath))   
		{   
			GetShortPathName(szPath, szShortPath, MAX_PATH);   
		}

		strPath = szShortPath;
		return strPath;
	}
};
