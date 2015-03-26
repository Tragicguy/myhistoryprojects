// R_Assistant.cpp : 定义 DLL 应用程序的入口点。
//

#include <string>
#include <windows.h>
#include <time.h>
#include <io.h>
#define  TESTTESTSHOW 
using namespace std;
char g_szExePath[256] = {0};
char g_szPath[256] = {0};

int g_iStartTimeOut = 0;
int g_iCheckTimeOut = 0;
int g_iCheckFileTimeOut = 0;
int g_iLogFlag = 0;
CRITICAL_SECTION g_Section;
CRITICAL_SECTION g_SectionClip;
void WriteLog(const char *szInfo);
BOOL StartProcess(PROCESS_INFORMATION &pi,STARTUPINFO &si);
HWND GetChildWindow(HWND hwnd,int iIndex);
BOOL CopyStringToClipboard(HWND hWnd, LPCTSTR lpszText);

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		char szIniPath[256] ={0};
		::GetModuleFileName((HMODULE)hModule,g_szPath,256);
		char *p = strrchr(g_szPath,'\\');
		*(p+1) = '\0';

		sprintf_s(szIniPath,"%sAssistantSet.ini",g_szPath);
		::GetPrivateProfileString("config","path", "",(LPSTR)g_szExePath,256, szIniPath);
		g_iStartTimeOut = ::GetPrivateProfileInt("config","starttimeout",0, szIniPath);
		g_iCheckTimeOut = ::GetPrivateProfileInt("config","checktimeout",0, szIniPath);
		g_iCheckFileTimeOut = ::GetPrivateProfileInt("config","chekfiletimeout",0, szIniPath);
		g_iLogFlag = ::GetPrivateProfileInt("config","log",0, szIniPath);
		if (g_iStartTimeOut < 20) g_iStartTimeOut = 20;
		if (g_iCheckTimeOut < 20) g_iCheckTimeOut = 20;
		if (g_iCheckFileTimeOut < 20)  g_iCheckFileTimeOut = 20;
		if (g_iStartTimeOut > 1000) g_iStartTimeOut = 1000;
		if (g_iCheckTimeOut > 5000) g_iCheckTimeOut = 5000;
		if (g_iCheckFileTimeOut > 1000)  g_iCheckFileTimeOut = 1000;
		if (strlen(g_szExePath) < 1)
		{
			return FALSE;
		}
		InitializeCriticalSection(&g_Section);
		InitializeCriticalSection(&g_SectionClip);
	}
	else if (DLL_PROCESS_DETACH == ul_reason_for_call)
	{
		DeleteCriticalSection(&g_Section);
		DeleteCriticalSection(&g_SectionClip);
	}
	return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//枚举主窗体结构体
struct   EnumParam
{
	HWND hMainWnd;
	DWORD dwProcessID;
};

BOOL CALLBACK EnumWinProc(HWND hwnd, LPARAM lParam)
{
	DWORD dwID;
	EnumParam* pep = (EnumParam*)lParam;
	GetWindowThreadProcessId(hwnd, &dwID);
	if (dwID == pep->dwProcessID)
	{
		pep->hMainWnd = hwnd;
		char strClassName[256] = {0};
		GetClassName(hwnd,strClassName,sizeof(strClassName));
		if(strcmp(strClassName,"Rgui Workspace")==0)
		return FALSE;
	}
	return  TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//对外函数
int __stdcall Calculate(const char *szPath)
{
	//判断文件是否存在
	if(_access(szPath,0)!=0)
	{
		return -2;
	}

	PROCESS_INFORMATION    pi; 
	STARTUPINFO            si; 
	EnumParam ep;
	char szInfo[800] = {0};
	char szPathEx[256] = {0};
	DWORD dRet = 0;
	int iRet = -1;
	HWND hwnd = NULL;
	HWND hwndPaste = NULL;
	HWND hwndClient = NULL;
	char szPathExEx[256] = {0};	
	/************************************************************************/
	char szRpath[256] ={0};
	strcpy_s(szRpath,szPath);
	char *p = strrchr(szRpath,'\\');
	*(p+1) = '\0';
	char szRfilePath[256] = {0};
	sprintf_s(szRfilePath,"%s%s",szRpath,"gr.csv");
	const int iDataArraylen = 4;
	char szDataFileArray[iDataArraylen][250] = {"forecasts.csv","elasticity.csv","gr.csv","report.csv"};
	char szRet[iDataArraylen] = {0};
	for(int i=0;i<iDataArraylen;i++)
	{
		sprintf_s(szRfilePath,"%s%s",szRpath,szDataFileArray[i]);
		//删除原有数据文件
		if (_access(szRfilePath,0)==0)
		{
			try
			{
				BOOL bRet = DeleteFile(szRfilePath);
				if (!bRet)
				{
					sprintf_s(szInfo,"delete file failed[%d]:%s",GetLastError(),szRfilePath);
				}
				else
				{
					sprintf_s(szInfo,"delete file succuss:%s",szRfilePath);
				}			
				WriteLog(szInfo);
			}
			catch (...)
			{
				return -5;
			}			
		}
	}
	
	//拼接R程序需要的命令字符串
	int i=0;
	int j = 0;
	while (szPath[i]!='\0')
	{
		if (szPath[i] !='\\')
		{
			szPathExEx[j]=szPath[i];
			i++;
			j++;
		}
		else
		{
			szPathExEx[j]='\\';
			szPathExEx[j+1]='\\';
			i++;
			j+=2;
		}
	}
	sprintf_s(szPathEx,"source(\"%s\")\r\n",szPathExEx);
	//启动程序
	BOOL bRet = StartProcess(pi,si);
	if (!bRet)	//启动程序失败
	{
		dRet = GetLastError();
		sprintf_s(szInfo,"StartProcess Failed :%d",(int)dRet);
		WriteLog(szInfo);
		return -3;
	}
	//开始监测目标程序是否正常启动
	DWORD dwExitCode;
	DWORD dwTimeStart = ::GetTickCount();
	while(true)
	{
		//启动程序超时，退出
		if (::GetTickCount()-dwTimeStart >g_iStartTimeOut * 1000)
		{
			sprintf_s(szInfo,"StartProcess Failed TimeOut:%d",(int)g_iStartTimeOut);
			WriteLog(szInfo);
			if (pi.hProcess)
			{
				bRet = TerminateProcess(pi.hProcess,0);
				if (!bRet)
				{
					dRet = GetLastError();
					sprintf_s(szInfo,"StartProcess TerminateProcess Failed :%d",(int)dRet);
					WriteLog(szInfo);
				}
			}
			iRet = -4;
			return iRet;
		}
		
		if(GetExitCodeProcess(pi.hProcess, &dwExitCode))//监控状态
		{
			if (dwExitCode == STILL_ACTIVE)
			{
				//查找主窗体句柄
				ep.hMainWnd = NULL;
				ep.dwProcessID = pi.dwProcessId;
				EnumWindows((WNDENUMPROC)EnumWinProc, (long)&ep);
				if (ep.hMainWnd != NULL)
				{
					//隐藏目标窗体
#ifdef TESTTESTSHOW
					int iCount = 0;
					while (true)
					{
						iCount++;
						if (iCount>10)
						{
							break;
						}
						if (::ShowWindow(ep.hMainWnd,SW_HIDE))
						{
							break;
						}
						Sleep(100);
					}
					
#endif
					
					//查找子窗体
					hwnd = NULL;
					hwnd = GetChildWindow(ep.hMainWnd,1);					
					hwndPaste = GetChildWindow(ep.hMainWnd,2);
					if (hwndPaste)
					{
						hwndPaste = GetChildWindow(hwndPaste,5);
						if (!hwndPaste)
						{
							continue;
						}
						else
						{
							sprintf_s(szInfo,"Find Paste Button HWND :%d",(int)hwndPaste);
							WriteLog(szInfo);
						}
					}
					else
					{
						continue;
					}
					if (hwnd)
					{
						hwndClient = hwnd;
						hwnd = GetChildWindow(hwnd,1);
					}
					if (hwnd)
					{
						char strClassName[256] = {0};
						char strWindowName[256] = {0};
						GetClassName(hwnd,strClassName,sizeof(strClassName));
						::GetWindowText(hwnd,(LPTSTR)&strWindowName,256);
						if (strcmp(strClassName,"Rgui Document")==0 &&strcmp(strWindowName,"R Console")==0)
						{
							sprintf_s(szInfo,"Find Rgui Document HWND :%d",(int)hwnd);
							WriteLog(szInfo);
							break;
						}
					}
					
				}				
			}
		}
		else
		{
			dRet = GetLastError();
			sprintf_s(szInfo,"GetExitCodeProcess Failed :%d",(int)dRet);
			WriteLog(szInfo);
		}
		Sleep(500);
	}
	
	//填写资源文件字符串，进行解析
	EnterCriticalSection(&g_SectionClip);
	CopyStringToClipboard(NULL,szPathEx);	
	::PostMessage(hwndPaste,WM_LBUTTONDOWN,0,0);
	Sleep(10);
	::PostMessage(hwndPaste,WM_LBUTTONUP,0,0);
	Sleep(10);
	LeaveCriticalSection(&g_SectionClip);
	//正在解析，开始监控是否已经解析完成
	HWND hwndResult = NULL;
	bool bIsSuccess = false;
	dwTimeStart = ::GetTickCount();
	/*
	while (!bIsSuccess)
	{
		//R主程序运算超时，退出
		if (::GetTickCount()-dwTimeStart >g_iCheckTimeOut * 1000)
		{
			sprintf_s(szInfo,"CheckCalculate Failed TimeOut:%d",(int)g_iCheckTimeOut);
			WriteLog(szInfo);
			if (pi.hProcess)
			{
				bRet = TerminateProcess(pi.hProcess,0);
				if (!bRet)
				{
					dRet = GetLastError();
					sprintf_s(szInfo,"CheckCalculate TerminateProcess Failed :%d",(int)dRet);
					WriteLog(szInfo);
				}
			}
			iRet = -5;
			return iRet;
		}
		Sleep(100);
		if (!hwndClient)
		{
			break;
		}
		//循环遍历子窗口，查找结果窗体是否出现
		for (int i=1;i<3;i++)
		{
			hwndResult = GetChildWindow(hwndClient,i);
			if (!hwndResult)
			{
				break;
			}
			char strClassName[256] = {0};
			char strWindowName[256] = {0};
			GetClassName(hwndResult,strClassName,sizeof(strClassName));
			::GetWindowText(hwndResult,(LPTSTR)&strWindowName,256);
			if (strcmp(strClassName,"Rgui Document")==0 &&strcmp(strWindowName,"R Graphics: Device 2 (ACTIVE)")==0)
			{
				sprintf_s(szInfo,"Find Rgui Graphics HWND :%d,Caculate Success",(int)hwnd);
				WriteLog(szInfo);
				bIsSuccess = true;
				break;
			}
		}		
	}
*/
	//成功了
	bIsSuccess = true;
	iRet = bIsSuccess==true?0:-1;
	//此处判断数据文件是否已经生成
	dwTimeStart = ::GetTickCount();
	int iFileGenCount = 0;
	memset(szRet,0,iDataArraylen);
	while(true)
	{	
		if (::GetTickCount()-dwTimeStart >g_iCheckFileTimeOut * 1000)
		{
			sprintf_s(szInfo,"Check Data File Failed TimeOut:%d",(int)g_iCheckFileTimeOut);
			WriteLog(szInfo);
			iRet = -6;
			break;
		}
		for(int i=0;i<iDataArraylen;i++)
		{
			sprintf_s(szRfilePath,"%s%s",szRpath,szDataFileArray[i]);
			if(szRet[i] == 1)
			{
				continue;
			}
			//检测数据文件
			if (_access(szRfilePath,0)==0)
			{
				sprintf_s(szInfo,"Check %s File Success",szDataFileArray[i]);
				WriteLog(szInfo);
				szRet[i] = 1;
				iFileGenCount++;
			}
		}
		if(iFileGenCount >= iDataArraylen)
		{
			iRet = 0;
			break;
		}
		Sleep(100);
	}
	
#ifdef TESTTESTSHOW
	//结束目标进程
	if (pi.hProcess)
	{
		bRet = FALSE;
		int iCount = 0;
		while (!bRet)
		{
			if (iCount>5)
			{
				break;
			}
			bRet = TerminateProcess(pi.hProcess,0);
			if (!bRet)
			{
				dRet = GetLastError();
				sprintf_s(szInfo,"TerminateProcess Failed :%d",(int)dRet);
				WriteLog(szInfo);
				iCount++;
			}
			else
			{
				break;
			}
		}		
	}	
#endif	
	//返回结果
	return iRet;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
HWND GetChildWindow(HWND hwnd,int iIndex)
{
	HWND hwndChind = NULL;
	int i = 1;
	hwndChind = GetWindow(hwnd,GW_CHILD);
	while (hwndChind)
	{	
		if (i == iIndex)
		{
			return hwndChind;
		}
		hwndChind = ::GetNextWindow(hwndChind,GW_HWNDNEXT);
		i++;
	}
	return hwndChind;
}
BOOL StartProcess(PROCESS_INFORMATION &pi,STARTUPINFO &si)
{
	ZeroMemory(&si, sizeof(si));
	si.cb =  sizeof(STARTUPINFO); 
	//GetStartupInfo(&si);
	si.dwFlags =STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;;
	si.wShowWindow = SW_HIDE;
// 	si.dwX = -10000;
// 	si.dwY = -10000;
// 	si.dwXSize = 0;
// 	si.dwYSize = 0;
	//return CreateProcess(g_szExePath,NULL,NULL,NULL,false,NORMAL_PRIORITY_CLASS, NULL,NULL,&si,&pi); 	
	return CreateProcess(NULL,g_szExePath,NULL,NULL,TRUE,CREATE_NO_WINDOW, NULL,NULL,&si,&pi); 
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CopyStringToClipboard(HWND hWnd, LPCTSTR lpszText)
{
	// 将szText中的内容复制到剪贴板
	char szInfo[800] ={0};
	DWORD dRet = 0;	
	HGLOBAL hglbCopy;
	LPTSTR  lptstrCopy;
	int nlen = strlen(lpszText);
	if (nlen == 0)
	{
		return FALSE;
	}
	if (!::OpenClipboard(hWnd))
	{
		dRet = GetLastError();
		sprintf_s(szInfo,"OpenClipboard Failed :%d",(int)dRet);
		WriteLog(szInfo);
		return FALSE;
	}
	BOOL bRet = EmptyClipboard();	
	if (!bRet)
	{
		dRet = GetLastError();		
		sprintf_s(szInfo,"EmptyClipboard Failed :%d",(int)dRet);
		WriteLog(szInfo);
		return FALSE;
	}

	hglbCopy = GlobalAlloc(GMEM_DDESHARE, 
		(nlen + 1) * sizeof(char)); 

	if (hglbCopy == NULL) 
	{ 
		dRet = GetLastError();
		sprintf_s(szInfo,"GlobalAlloc Failed :%d",(int)dRet);
		WriteLog(szInfo);
		CloseClipboard(); 
		return FALSE; 
	}
	lptstrCopy = (LPTSTR)GlobalLock(hglbCopy); 
	memcpy(lptstrCopy, lpszText, nlen);
	lptstrCopy[nlen] = (TCHAR)0;  //null character
	GlobalUnlock(lptstrCopy);
	SetClipboardData(CF_TEXT, hglbCopy);
	CloseClipboard();
	return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//写日志
void WriteLog(const char * szInfo)
{
	if (g_iLogFlag == 0)
	{
		return;
	}
	//EnterCriticalSection(&g_Section);
	try
	{
		char szPath[256] = {0};
		char szInfoEx[1000] = {0};
		time_t t = time(NULL);
		string strTime = ctime(&t);
		strTime = strTime.substr(0, strTime.size() - 1);
		sprintf_s(szInfoEx,"%s\t%s",strTime.c_str(),szInfo);		
		sprintf_s(szPath,"%sAssistantlog_%d.log",g_szPath,::GetCurrentThreadId());
		FILE *fp;
		fp=fopen(szPath,"a+");
		fprintf(fp,"%s\r\n",szInfoEx);
		fclose(fp);
	}
	catch (...)
	{
	}
	//LeaveCriticalSection(&g_Section);
}