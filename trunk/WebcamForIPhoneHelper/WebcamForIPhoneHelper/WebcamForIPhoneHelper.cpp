// WebcamForIPhoneHelper.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "WebcamForIPhoneHelper.h"
#include "WindowFind.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// 初始化 MFC 并在失败时显示错误
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: 更改错误代码以符合您的需要
			_tprintf(_T("错误: MFC 初始化失败\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: 在此处为应用程序的行为编写代码。
		}
	}
	else
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: GetModuleHandle 失败\n"));
		nRetCode = 1;
	}

	return nRetCode;
}

bool CheckDeleteFile(LPCSTR strFileName)
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


void ClearPics(char* strDefaultPath)
{
	CString strFolder = strDefaultPath;
	CFileFind fileFinder;
	BOOL bRes = fileFinder.FindFile(strFolder + "\\*.jpg");		
	while(bRes)
	{
		bRes = fileFinder.FindNextFile();
		CString strPath = fileFinder.GetFileName();
		if(fileFinder.IsDirectory())
		{
			continue;
		}
		strPath = strFolder + "\\" + strPath;
		CheckDeleteFile(strPath);			
	}
}

CString GetSrcPicPath(char* strDefaultPath)
{
	CString strFolder = strDefaultPath;
	CFileFind fileFinder;
	BOOL bRes = fileFinder.FindFile(strFolder + "\\*.jpg");	
	CString strPath = "";
	while(bRes)
	{
		bRes = fileFinder.FindNextFile();
		strPath = fileFinder.GetFileName();
		if(fileFinder.IsDirectory())
		{
			continue;
		}
		strPath = strFolder + "\\" + strPath;
		break;
	}
	return strPath;
}

WEBCAMFORIPHONEHELPER_API int CapturePictrue(char* strDefaultPath, char* strPath)
{
	CWindowFind find;
	HWND hwndMost = ::FindWindow("com.shapeservices.webcamera_iphone", "Mobiola WebCamera for iPhone");
	if(hwndMost == NULL)
	{
		return -1;
	}
	HWND hwnd = find.GetChildWindow(hwndMost, 1);
	if(hwnd == NULL)
	{
		return -2;
	}
	hwnd = find.GetChildWindow(hwnd, 3);
	if(hwnd == NULL)
	{
		return -3;
	}
	hwnd = find.GetChildWindow(hwnd, 5);
	if(hwnd == NULL)
	{
		return -4;
	}
	//清理数据
	ClearPics(strDefaultPath);	
	//截图
	find.PostBtnClick(hwnd);
	int nCount = 0;
	CString strSrcPath = "";
	while(nCount ++ < 20 && strSrcPath == "")
	{
		Sleep(80);
		strSrcPath = GetSrcPicPath(strDefaultPath);
	}
	if(strSrcPath == "")
		return -5;
	Sleep(50);
	BOOL bRet = ::CopyFile(strSrcPath, strPath, FALSE);
	Sleep(10);
	ClearPics(strDefaultPath);
	Sleep(10);
	ClearPics(strDefaultPath);
	return bRet;	
}