// ReStartApp.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "ReStartApp.h"
#include "ReStartAppDlg.h"
#include "UserFile.h"
#include <io.h>
#include <direct.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CReStartAppApp

BEGIN_MESSAGE_MAP(CReStartAppApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CReStartAppApp 构造

CReStartAppApp::CReStartAppApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CReStartAppApp 对象

CReStartAppApp theApp;


// CReStartAppApp 初始化

BOOL CReStartAppApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControls()。否则，将无法创建窗口。
	CString strAppPath="";
	CUserFile *pFile = CUserFile::GetInstance();
	if ( pFile )
	{
		pFile->GetAppPath(strAppPath);
		if (strAppPath.GetLength() > 16)
		{
			strAppPath = strAppPath.Right(16);
		}
		strAppPath = pFile->StrEncode(strAppPath);
	}
	if (strAppPath == "")
	{
		return FALSE;
	}
	if (strAppPath.GetLength() > 32)
	{
		strAppPath = strAppPath.Right(32);
	}
	strAppPath +="help";
	// 检测是否已经创建Mutex
	// 如果已经创建，就终止进程的启动
	m_hMutex = CreateMutex(NULL, TRUE, strAppPath);	
	if ((m_hMutex != NULL) && (GetLastError() == ERROR_ALREADY_EXISTS)) 
	{
		WriteLog("辅助程序重复启动",0);
		ReleaseMutex(m_hMutex);
		//MessageBox(NULL, "该程序已经在运行中,请不要运行多个!", "严重警告", MB_OK);
		return FALSE;
	}

	InitCommonControls();

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

	CReStartAppDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用“确定”来关闭
		//对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用“取消”来关闭
		//对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	// 而不是启动应用程序的消息泵。
	return FALSE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CReStartAppApp::ExitInstance()
{
	CUserFile *pFile = CUserFile::GetInstance();
	if ( pFile )
	{
		delete pFile;
		pFile = NULL;
	}
	return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//写日志
void CReStartAppApp::WriteLog(CString srcStr,int iFlag)
{

	CTime currTime = CTime::GetCurrentTime();
	try
	{
		CString filepath = "";
		CString destStr = "";
		CString folder = "";
		CString filename = "";
		CString strAppPath = "";
		if (iFlag == 0)	//重启日志
		{
			folder = "restartlog";
			filename.Format("%s_restartlog.txt",currTime.Format("%Y%m%d"));
		}
		else
		{
			return;
		}
		CUserFile *pFile = CUserFile::GetInstance();
		if ( pFile )
		{
			pFile->GetAppPath(strAppPath);
		}
		CString folderName = "";
		folderName.Format("%s%s",strAppPath,folder);
		if(_access(folderName,0)!=0)
		{
			if(mkdir(folderName) != 0)//创建目录
			{
				return;
			}
		}					
		destStr.Format("%s	%s",currTime.Format("%Y-%m-%d %H:%M:%S"),srcStr);
		filepath.Format("%s\\%s",folderName,filename);
		FILE *fp;
		char szfile[1000]={0};
		sprintf(szfile,filepath);
		fp=fopen(szfile,"a+");
		fprintf(fp,"%s\r\n",destStr);
		fclose(fp);
	}
	catch (...)
	{
	}
}