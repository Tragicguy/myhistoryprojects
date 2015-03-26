// ReStartAppDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ReStartApp.h"
#include "ReStartAppDlg.h"
#include ".\restartappdlg.h"
#include "UserFile.h"
#include <io.h>
#include <direct.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define  WM_USER_MODIFYINI	WM_USER + 15454
#define  WM_SET_FILEINFO "mwgate.ini"//"MonGate.ini"

BOOL g_iExitFlag = TRUE;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

/************************************************************************/
/*                                                                      */
/************************************************************************/
//线程中控制延时的函数
void Delay(int iMiniSecond,BOOL *bExit)
{
	for (int i=0;i<iMiniSecond;i+=5)
	{
		if (*bExit)
		{
			break;
		}
		Sleep(5);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//写日志
void WriteLog(CString srcStr,int iFlag)
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

/************************************************************************/
/*                                                                      */
/************************************************************************/
CString GetContrlClassNameEx(HWND hwnd)
{
	CString strRet = "";
	TCHAR  szClassName[256]={0};	
	GetClassName(hwnd,(LPTSTR)&szClassName,256);
	strRet.Format("%s",szClassName);
	return strRet;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CString GetContrlWindowTextEx(HWND hwnd)
{
	CString strRet = "";
	TCHAR  szWindName[256]={0};	
	::GetWindowText(hwnd,(LPTSTR)&szWindName,256);
	strRet.Format("%s",szWindName);
	return strRet;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CloseErrorWindowEx(HWND hwnd)
{
	try
	{
		HWND hwndChind = NULL;
		CString strWindowName = "";
		CString strClassName = "";
		hwndChind = GetWindow(hwnd,GW_CHILD);
		CWnd *pWnd = CWnd::FromHandle(hwndChind);
		bool bExist = false;
		while (pWnd)
		{	
			hwndChind = pWnd->GetSafeHwnd();
			strWindowName = GetContrlWindowTextEx(hwndChind);
			strClassName = GetContrlClassNameEx(hwndChind);
			if(strClassName =="Button" && strWindowName == "确定")
			{
				WriteLog("关闭错误框",0);
				::PostMessage(hwndChind,WM_LBUTTONDOWN,0,MAKELONG(1,1));
				Sleep(20);
				::PostMessage(hwndChind,WM_LBUTTONUP,0,MAKELONG(1,1));
				bExist = true;
				break;
			}
			pWnd = pWnd->GetNextWindow(GW_HWNDNEXT);
		}
		if (bExist == false)
		{
			WriteLog("未找到确定按钮,发送WM_CLOSE消息",0);
			::PostMessage(hwnd,WM_CLOSE,0,0);
		}
	}	
	catch (...)
	{
	}	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CALLBACK EnumChildProcEx(HWND hwnd, LPARAM lParam)
{   
	CString strWindowName = GetContrlWindowTextEx(hwnd);
	if (strWindowName.Find("应用程序错误")>0)		//满足条件
	{
		//遍历其子窗口，如果找到确定按钮，发送双击消息
		WriteLog("找到错误框:"+strWindowName,0);
		try
		{
			CloseErrorWindowEx(hwnd);
		}
		catch (...)
		{
		}		
		return FALSE;
	}
	else
	{
		Delay(20,&g_iExitFlag);
		if (g_iExitFlag == TRUE)
		{
			return FALSE;
		}
		EnumChildWindows(hwnd,EnumChildProcEx,lParam);
	}
	return TRUE;   
}


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CReStartAppDlg 对话框



CReStartAppDlg::CReStartAppDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReStartAppDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CReStartAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CReStartAppDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START, OnBnClickedBtnStart)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBnClickedBtnExit)
	ON_MESSAGE(WM_USER_MODIFYINI,OnReInitparam)
	ON_WM_SIZE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CReStartAppDlg 消息处理程序

BOOL CReStartAppDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);		// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	m_iFailTime = 0;
	MoveWindow(10000,10000,0,0);		//隐藏窗口
	//PostMessage(WM_SIZE,0,0);
	ShowWindow(SW_HIDE);
	::SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, WS_EX_TOOLWINDOW); 
	ShowWindow(SW_SHOWNORMAL);
	InitParams();
	OnBnClickedBtnStart();	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CReStartAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CReStartAppDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CReStartAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CReStartAppDlg::OnBnClickedBtnStart()
{
	ReStartApp(0);
	SetTimer(1,m_iCheckInterval,NULL);
	g_iExitFlag = FALSE;
	AfxBeginThread((AFX_THREADPROC)CloseProcFun, (LPVOID)this);
	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//返回应用程序所在目录，包含"\"
int CReStartAppDlg::GetAppPath(CString &strAppPath)
{
	int iRet = -3;
	try
	{
		TCHAR szBuff[_MAX_PATH];
		VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szBuff, _MAX_PATH));
		strAppPath.Format("%s",szBuff);
		int pos=strAppPath.ReverseFind('\\');
		strAppPath=strAppPath.Left(pos+1);		
		iRet = 0;
	}
	catch (...)
	{
	}
	return iRet;	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CReStartAppDlg::OnTimer(UINT nIDEvent)
{
	if (nIDEvent == 1)
	{
		//重启程序
		int iRet = ReStartApp(1);
		if (iRet == 0)
		{
			m_iFailTime ++;
			if (m_iFailTime > 2)
			{
				SetDlgItemText(IDC_SHOWINFO,CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S")+" 多次启动失败,重新加载配置信息");
				InitParams();
				m_iFailTime = 0;
			}
		}
		
	}
	CDialog::OnTimer(nIDEvent);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CReStartAppDlg::ReStartApp(int iFlag)
{
	int iRetVal = 0;	//0表示失败
	try
	{
		if (m_iRunStatus == 1)
		{
			SetDlgItemText(IDC_SHOWINFO,CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S")+"	程序正常关闭，不启动");	
			WriteLog("程序正常关闭，不启动",0);
			return 0;
		}
		CWnd *pWnd = NULL;
		pWnd = FindWindow(NULL,m_strWindowName);
		if (pWnd != NULL)
		{
			SetDlgItemText(IDC_SHOWINFO,CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S")+"	目标程序正常运行");			
			iRetVal = 1;
			return iRetVal;
		}
		CString strShow = "";
		CString strAppPath = "";
		GetAppPath(strAppPath);
		HINSTANCE iRet = ShellExecute(NULL, "open", strAppPath + m_strStrAppName, NULL, NULL, SW_SHOW);
		if (iFlag == 0)
		{
			strShow.Format("第一次启动程序%s",(int)iRet <32?"失败":"成功");
			SetDlgItemText(IDC_SHOWINFO,CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S")+"	"+strShow);
			WriteLog(strShow,0);
		}
		else
		{
			strShow.Format("重启程序%s",(int)iRet <32?"失败":"成功");
			SetDlgItemText(IDC_SHOWINFO,CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S")+"	"+strShow);
			WriteLog(strShow,0);
		}
		iRetVal = (int)iRet<32?0:1;
	}
	catch (...)
	{
		iRetVal = 0;
	}
	return iRetVal;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CReStartAppDlg::OnBnClickedBtnExit()
{
	PostMessage(WM_CLOSE,0,0);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CReStartAppDlg::InitParams()
{
	CUserFile *pFile = NULL;
	pFile = CUserFile::GetInstance();
	CString strTemp = "";
	int iTemp = 0;
	if (pFile)
	{
		CString strAppPath = "";
		pFile->GetAppPath(strAppPath);
		strAppPath += WM_SET_FILEINFO; //mwgate.ini
		//数据库信息
		pFile->ReadInitInfo(strAppPath,"window","name",m_strWindowName,iTemp,0);
		if (m_strWindowName == "")
		{
			m_strWindowName = "梦网短信网关 v1.0.6.2";
		}
		pFile->ReadInitInfo(strAppPath,"window","appname",m_strStrAppName,iTemp,0);
		if (m_strStrAppName == "")
		{
			m_strStrAppName = "MonGate.exe";
		}
 		CString strVersion = "";
 		pFile->ReadInitInfo(strAppPath,"window","version",strVersion,iTemp,0);
 		m_strWindowName += " "+strVersion;	
		pFile->ReadInitInfo(strAppPath,"window","interval",strTemp,m_iCheckInterval,1);
		pFile->CheckInitValue(m_iCheckInterval,2000,200000);
		pFile->ReadInitInfo(strAppPath,"window","runstatus",strTemp,m_iRunStatus,1);
		pFile->CheckInitValue(m_iRunStatus,0,1);
		SetDlgItemText(IDC_APPNAME,"应用程序名: "+m_strStrAppName);
		
		SetDlgItemText(IDC_WINDOWNAME,"窗口标题: "+m_strWindowName);
		strTemp.Format("时间间隔[毫秒]: %d",m_iCheckInterval);
		SetDlgItemText(IDC_INTERVAL,strTemp);
		SetWindowText("网关辅助程序_"+m_strWindowName);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
LRESULT CReStartAppDlg::OnReInitparam(WPARAM wParam,LPARAM lParam)
{
	InitParams();
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CReStartAppDlg::OnSize(UINT nType, int cx, int cy)
{
	ShowWindow(SW_HIDE);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CReStartAppDlg::CloseErrorWindow()
{
	//EnumChildWindows(NULL,EnumChildProcEx,(LPARAM)0);
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
//加入待发缓冲区线程函数
void CReStartAppDlg::CloseProcFun(LPVOID lpParam)
{
	WriteLog("关闭错误框线程启动",0);
	CReStartAppDlg *pWindow = (CReStartAppDlg *)lpParam;
	pWindow->CloseProc();	
	WriteLog("关闭错误框线程退出",0);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CReStartAppDlg::CloseProc()
{
	HWND hwnd = NULL;
	CString strWindowName1 = "";
	CString strWindowName2 = "";
	//m_strWindowName = "号码简易生成器";
	//m_strStrAppName = "TESTstr.exe";
	while (g_iExitFlag == FALSE)
	{
		Delay(3000,&g_iExitFlag);
		if (g_iExitFlag == TRUE)
		{
			break;
		}
		hwnd = NULL;
		strWindowName1.Format("%s - 应用程序错误",m_strStrAppName);
		strWindowName2.Format("%s: %s - 应用程序错误",m_strWindowName,m_strStrAppName);
		//strWindowName1.Format("%s - 应用程序错误","TESTstr.exe");
		//strWindowName2.Format("%s: %s - 应用程序错误","号码简易生成器","TESTstr.exe");
		//WriteLog(strWindowName1,0);
		//WriteLog(strWindowName2,0);
		hwnd = ::FindWindow("#32770",strWindowName1);
		if (hwnd!=NULL)
		{
			WriteLog("找到错误窗口:"+strWindowName1,0);
			CloseErrorWindowEx(hwnd);
		}
// 		else
// 		{
// 			SetDlgItemText(IDC_SHOWINFO,CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S")+"	目标程序正常运行");	
// 		}
		hwnd = NULL;
		hwnd = ::FindWindow("#32770",strWindowName2);
		if (hwnd!=NULL)
		{
			WriteLog("找到错误窗口:"+strWindowName2,0);
			CloseErrorWindowEx(hwnd);
		}
// 		else
// 		{
// 			SetDlgItemText(IDC_SHOWINFO,CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S")+"	未发现");	
// 		}
		//EnumChildWindows(NULL,EnumChildProcEx,(LPARAM)0);		
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CReStartAppDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	g_iExitFlag = TRUE;
	KillTimer(1);
	Sleep(200);
	CDialog::OnClose();
}
