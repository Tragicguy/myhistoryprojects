// AmazonRobot.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "AmazonRobot.h"
#include "AmazonRobotDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAmazonRobotApp

BEGIN_MESSAGE_MAP(CAmazonRobotApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

void InitParams(CString strPath)
{
	char szTemp[2048] = {0};
	CString strNode = "set";

	::GetPrivateProfileString(strNode,"strGoodNos", "",(LPSTR)szTemp,2047, strPath);
	gl_AmazonStr.strGoodNos = szTemp;

	memset(szTemp,0,2048);
	::GetPrivateProfileString(strNode,"strUserID", "",(LPSTR)szTemp,2047, strPath);
	gl_AmazonStr.strUserID = szTemp;

	memset(szTemp,0,2048);
	::GetPrivateProfileString(strNode,"strPwd", "",(LPSTR)szTemp,2047, strPath);
	gl_AmazonStr.strPwd = szTemp;

	memset(szTemp,0,2048);
	::GetPrivateProfileString(strNode,"strProvince", "",(LPSTR)szTemp,2047, strPath);
	gl_AmazonStr.strProvince = szTemp;

	memset(szTemp,0,2048);
	::GetPrivateProfileString(strNode,"strCity", "",(LPSTR)szTemp,2047, strPath);
	gl_AmazonStr.strCity = szTemp;

	memset(szTemp,0,2048);
	::GetPrivateProfileString(strNode,"strTown", "",(LPSTR)szTemp,2047, strPath);
	gl_AmazonStr.strTown = szTemp;

	memset(szTemp,0,2048);
	::GetPrivateProfileString(strNode,"strRecver", "",(LPSTR)szTemp,2047, strPath);
	gl_AmazonStr.strRecver = szTemp;

	memset(szTemp,0,2048);
	::GetPrivateProfileString(strNode,"strAddr", "",(LPSTR)szTemp,2047, strPath);
	gl_AmazonStr.strAddr = szTemp;

	memset(szTemp,0,2048);
	::GetPrivateProfileString(strNode,"strPostCode", "",(LPSTR)szTemp,2047, strPath);
	gl_AmazonStr.strPostCode = szTemp;

	memset(szTemp,0,2048);
	::GetPrivateProfileString(strNode,"strPhone", "",(LPSTR)szTemp,2047, strPath);
	gl_AmazonStr.strPhone = szTemp;

	memset(szTemp,0,2048);
	::GetPrivateProfileString(strNode,"strBillHeader", "",(LPSTR)szTemp,2047, strPath);
	gl_AmazonStr.strBillHeader = szTemp;

	memset(szTemp,0,2048);
	::GetPrivateProfileString(strNode,"strBillContent", "",(LPSTR)szTemp,2047, strPath);
	gl_AmazonStr.strBillContent = szTemp;

	memset(szTemp,0,2048);
	::GetPrivateProfileString(strNode,"strOrderNo", "",(LPSTR)szTemp,2047, strPath);
	gl_AmazonStr.strOrderNo = szTemp;

	gl_AmazonStr.eSendType = (_SendType)::GetPrivateProfileInt(strNode,"eSendType",1, strPath);
	gl_AmazonStr.bBill = ::GetPrivateProfileInt(strNode,"bBill",0, strPath);
	gl_AmazonStr.iShowFlag = ::GetPrivateProfileInt(strNode,"iShowFlag",1, strPath);
	gl_AmazonStr.iDebug = ::GetPrivateProfileInt(strNode,"iDebug",1, strPath);
	gl_AmazonStr.iOpFlag = ::GetPrivateProfileInt(strNode,"iOpFlag",2, strPath);	//默认下单
	gl_AmazonStr.eQueryType = (_QueryType)::GetPrivateProfileInt(strNode,"eQueryType",1, strPath);
}
// CAmazonRobotApp 构造
//清除指定网站的COOKIE
void ClearSiteCookie(CString strFlag)
{
	TCHAR szPath[MAX_PATH] = {0};
	if (SHGetSpecialFolderPath(NULL, szPath, CSIDL_COOKIES, FALSE))
	{//得到目录，并清空
		char szFind[MAX_PATH]={0};
		WIN32_FIND_DATA FindFileData;
		strcpy(szFind,szPath);
		strcat(szFind,"\\*.txt");
		HANDLE hFind=::FindFirstFile(szFind,&FindFileData);
		if(INVALID_HANDLE_VALUE == hFind)    return;
		while(TRUE)
		{
			if (1)
			{
				CString strPath = "";
				strPath.Format("%s\\%s",szPath,FindFileData.cFileName);
				if ( strPath.Find(strFlag) >=0 )
				{
					CFile::Remove(strPath);
				}
			}
			if(!FindNextFile(hFind,&FindFileData))
			{
				break;
			}
		}
		FindClose(hFind);
	}
}
CAmazonRobotApp::CAmazonRobotApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CAmazonRobotApp 对象

CAmazonRobotApp theApp;


// CAmazonRobotApp 初始化

BOOL CAmazonRobotApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	//同一台电脑同时只能执行一个进程
	HANDLE hMutex = CreateMutex(NULL, FALSE, MUTEX_DEFINE);
	if(hMutex != NULL)
	{
		if(GetLastError() == ERROR_ALREADY_EXISTS)
		{
			ReleaseMutex(hMutex);
			CloseHandle(hMutex); 
			hMutex = NULL;
			exit(HAVE_ALREADY_RUN);
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
	
	//获取参数
	CString strPath = AfxGetApp()->m_lpCmdLine;

 	strPath.Trim();
 	if (strPath != "")
 	{
		InitParams(strPath);
		gl_AmazonStr.iAutoRun = 1;
	}
	else
	{
		gl_AmazonStr.iShowFlag = 1;
		gl_AmazonStr.iAutoRun = 0;
		gl_AmazonStr.iDebug = 1;
// 		GetAppPath(strPath);
// 		InitParams(strPath + AMAZON_CONFIG_FILE);
// 		gl_AmazonStr.iAutoRun = 1;
	}
	//清除COOKIE
	ClearSiteCookie("amazon");	
	CAmazonRobotDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此处放置处理何时用“确定”来关闭
		//  对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用“取消”来关闭
		//  对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}


