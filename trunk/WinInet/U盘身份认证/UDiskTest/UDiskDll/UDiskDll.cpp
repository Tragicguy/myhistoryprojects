// UDiskDll.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "UDiskDll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// 初始化 MFC 并在失败时显示错误
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: MFC 初始化失败\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: 在此处为应用程序的行为编写代码。
	}

	return nRetCode;
}


CString GetDriverInfomation(char* bAlpha)
{
	int iGTI_TYPE = GDI_VOLUMESERIALNUMBER;
	CString strResult = _T("");
	{
		/**********获取驱动器名字、序列号和文件系统部分**********/
		CString strRootPathName;
		strRootPathName.Format(_T("%s:\\"),bAlpha);
		if (GetDriveType(strRootPathName) != DRIVE_REMOVABLE)
		{
			return "";
		}
		LPCTSTR lpRootPathName = strRootPathName;
		LPTSTR lpVolumeNameBuffer = new char[_MAX_FNAME];
		DWORD nVolumeNameSize = _MAX_FNAME;
		DWORD nVolumeSerialNumber = 0;//便于驱动器无效时做判断
		DWORD nMaximumComponentLength;
		DWORD nFileSystemFlags;
		LPTSTR lpFileSystemNameBuffer =new char[20];//文件系统(NTFS,FAT)多大有定义好的宏吗
		DWORD nFileSystemNameSize = 20;
		GetVolumeInformation(
			lpRootPathName,
			lpVolumeNameBuffer,
			nVolumeNameSize,
			&nVolumeSerialNumber, 
			&nMaximumComponentLength,
			&nFileSystemFlags,
			lpFileSystemNameBuffer,
			nFileSystemNameSize);

		/**********获取驱动器类型部分**********/
		CString strDriveType;

		/**********获取驱动器总大小和剩余大小部分**********/
		LPCTSTR lpDirectoryName =new char[2];
		lpDirectoryName = (LPCTSTR)strRootPathName.Mid(0,2);
		_ULARGE_INTEGER FreeBytesAvailable,TotalNumberOfBytes,TotalNumberOfFreeBytes ;

		__int64 iVolumeSize = 0,iVolumeFreeSize = 0;

		GetDiskFreeSpaceEx(strRootPathName.Mid(0,2),&FreeBytesAvailable,&TotalNumberOfBytes,&TotalNumberOfFreeBytes );
		iVolumeSize = TotalNumberOfBytes.QuadPart / 1024 / 1024;
		iVolumeFreeSize = FreeBytesAvailable.QuadPart / 1024 / 1024;

		/**********根据参数得出响应的驱动器信息**********/
		switch(iGTI_TYPE)
		{
		case GDI_VOLUMENAME:
			if (lpVolumeNameBuffer != NULL)
				strResult.Format(_T("驱动器 %c 的名字为:%s."),bAlpha,lpVolumeNameBuffer);
			else
				strResult.Format(_T("驱动器 %c 的名字为:%s."),bAlpha,lpVolumeNameBuffer);
			//strResult.Format(_T("获取驱动器名字失败!"));
			break;
		case GDI_VOLUMESERIALNUMBER:
			if (nVolumeSerialNumber != 0)
				strResult.Format(_T("%X"),nVolumeSerialNumber);
			else
				strResult.Format(_T(""));
			break;
		case GDI_VOLUMEFILESYSTEM:
			if (lpFileSystemNameBuffer != NULL)
				strResult.Format(_T("驱动器 %c 的文件系统为:%s."),bAlpha,lpFileSystemNameBuffer);
			else
				strResult.Format(_T("获取驱动器文件系统失败!"));
			break;
		case GDI_VOLUMESIZE:
			if (iVolumeSize != 0)
				strResult.Format(_T("驱动器 %c 的总大小为:%.2fGB."),bAlpha,(float)iVolumeSize/1024);
			else
				strResult.Format(_T("获取驱动器总大小失败!"));
			break;
		case GDI_VOLUMEFREESIZE:
			if (iVolumeFreeSize != 0)
				strResult.Format(_T("驱动器 %c 的剩余大小为:%.2fGB."),bAlpha,(float)iVolumeFreeSize/1024);
			else
				strResult.Format(_T("获取驱动器剩余大小失败!"));
			break;
		case GDI_VOLUMETYPE:
			switch(GetDriveType(lpRootPathName))
			{
			case DRIVE_UNKNOWN:
				strDriveType = _T("未知类型!");
				break;
			case DRIVE_NO_ROOT_DIR:
				strResult = _T("获取驱动器类型时参数设置错误!");
				return strResult;
			case DRIVE_REMOVABLE:
				strDriveType = _T("可移动磁盘");
				break;
			case DRIVE_FIXED:
				strDriveType = _T("硬盘");
				break;
			case DRIVE_REMOTE:
				strDriveType = _T("远程设备");
				break;
			case DRIVE_CDROM:
				strDriveType = _T("CD");
				break;
			case DRIVE_RAMDISK:
				strDriveType = _T("RAM");
				break;
			default:
				strResult = _T("未知错误!");
				return strResult;
			}
			strResult.Format(_T("驱动器 %c 的类型为:%s."),bAlpha,strDriveType);
			break;
		default:
			strResult = _T("获取驱动器信息时参数设置错误!");
			break;
		}

	}
	return strResult;
}

UDISKDLL_API bool GetDiskSerial(char* szDisk,char* szSerial)
{
	CString strRet = GetDriverInfomation(szDisk);
	if (strRet == "")
	{
		return false;
	}
	CString strNumber = "";
	while(strRet.GetLength() > 0)
	{
		int iLen = strRet.GetLength();
		if (iLen < 1)
		{
			break;
		}
		strNumber += strRet.Left(iLen > 2 ? 2:iLen) + "-";
		strRet = strRet.Mid(iLen > 2 ? 2:iLen);
	}
	if (strNumber.GetLength() > 0 && strNumber.Right(1) == "-")
	{
		strNumber = strNumber.Left(strNumber.GetLength() -1);
	}
	strcpy(szSerial,strNumber);
	return true;
}



UDISKDLL_API bool GetDisks(char* szDisks)
{
	TCHAR szBuf[100];
	memset(szBuf,0,100);
	CString strRet = "";
	DWORD len = GetLogicalDriveStrings(sizeof(szBuf)/sizeof(TCHAR),szBuf);
	for (TCHAR* s = szBuf; *s; s += _tcslen(s)+1)
	{
		CString sDrivePath = s;
		sDrivePath.MakeLower();
		if (sDrivePath.Find('a') >=0 )
		{
			continue;
		}
		UINT uType = GetDriveType(sDrivePath);
		if (uType == DRIVE_REMOVABLE)
		{
			strRet += sDrivePath;
			strRet += ",";
		}
	}
	strcpy(szDisks,strRet);
	return true;
}
