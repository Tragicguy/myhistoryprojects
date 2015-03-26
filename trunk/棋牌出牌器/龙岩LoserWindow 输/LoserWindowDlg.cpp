// LoserWindowDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LoserWindow.h"
#include "LoserWindowDlg.h"
#include ".\loserwindowdlg.h"
#include "UserMessagePump.h"
#include "UserFile.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <afxmt.h>
#include <afxtempl.h>
#include "WindowFind.h"
#include <io.h>
#include <direct.h>
#include "afxinet.h"
#include <io.h>
//#define  ONLY_LOSER
// #define ONLY_WINNNER
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
typedef struct _MyData
{
	DWORD dwSendMsg; // sendmessage函数地址
	HWND hwndParant;
	NMHDR nmh;
}MyData;
//注入到远程进程中的执行代码
static DWORD WINAPI RemoteFunc(LPVOID pData)
{	
	MyData *pMyData = (MyData*)pData;
	typedef int(__stdcall*MySendMessage)(HWND,UINT,WPARAM,LPARAM);
	MySendMessage msgSend = (MySendMessage)pMyData->dwSendMsg;
	HWND hwndParant = pMyData->hwndParant;
	NMHDR nmhdr = pMyData->nmh;
	msgSend(hwndParant,WM_NOTIFY,(WPARAM)nmhdr.idFrom,(LPARAM)&nmhdr);		//发送双击消息
	return 0;
}

int g_ExitFlag = 1;
int g_iIsLogin = 0;
HTREEITEM g_hTreeItem = NULL;
CString g_strOldAcc = "";
CString g_strAppPath = "D:\\Program Files\\邵武游戏中心\\ShaoWu\\Lobby.exe";
CString g_strRoomPwd = "123";
int g_iWinOrLose = 0;	//1为赢,0为输家
int g_iLoseNum = 0;	//输多少局后换号
int g_iRoomNum = 0;
int g_iSetNum = 0;
int g_iMinBalance = 300;
int g_iMaxBalance = 1000000;
int g_iIsCheckGm = 0;
int g_iSpeed = 1;
int g_iIsHidePlayWindow = 0;
int g_iStopMinutes = 0;
int g_iCheckInterval = 0;
HWND g_hwndSet = NULL;
HWND g_hwndBack = NULL;
HWND g_hwndListView1 = NULL;
HWND g_hwndListView2 = NULL;
HWND g_hwndDesk = NULL;		//房间句柄
HWND g_hwndTopMost = NULL;
HWND g_hwndPlayWindow = NULL;
CString g_strBalance = "";

int g_iCheckGmWait = 0;
int g_iCheckGmWaitInterval = 0;
CString g_strGmStr = "";
int g_iMinMoney = 0;
int g_iIsSetPwd = 0;
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


// CLoserWindowDlg 对话框

typedef CArray<RegisterPack, RegisterPack> RegisterPackList;
RegisterPackList g_RegisterList;

CLoserWindowDlg::CLoserWindowDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoserWindowDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLoserWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ACC, m_ListAcc);
	DDX_Control(pDX, IDC_LIST_INFO, m_message);
	DDX_Control(pDX, IDC_SLIDER_SPEED, m_SpeedCtrl);
	DDX_Control(pDX, IDC_CHECK_GMCHECK, m_CheckGM);
	DDX_Control(pDX, IDC_CHECK_HIDEWINDOW, m_HideWindow);
	DDX_Control(pDX, IDC_CHECK_GMCHECKWAIT, m_gmCheckWait);
}
_declspec(dllimport) HookStruct SetHookEx(HWND m_hwndHelper);

BEGIN_MESSAGE_MAP(CLoserWindowDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START, OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnBnClickedButton7)
	ON_MESSAGE(USERWINDOWMSG_1,AddToList)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_BTN_INPORT, OnBnClickedBtnInport)
	ON_BN_CLICKED(IDC_RADIO_LOSE, OnBnClickedRadioLose)
	ON_BN_CLICKED(IDC_RADIO_WIN, OnBnClickedRadioWin)
	ON_BN_CLICKED(IDC_BTN_FIND, OnBnClickedBtnFind)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BUTTON8, OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, OnBnClickedButton9)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_KILLTHREAD,StopThread)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CHECK_GMCHECK, OnBnClickedCheckGmcheck)
	ON_BN_CLICKED(IDC_CHECK_HIDEWINDOW, OnBnClickedCheckHidewindow)
	ON_BN_CLICKED(IDC_CHECK_GMCHECKWAIT, OnBnClickedCheckGmcheckwait)
END_MESSAGE_MAP()


// CLoserWindowDlg 消息处理程序

BOOL CLoserWindowDlg::OnInitDialog()
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
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	SetTableStyle();
	InitListCtrl();
#ifdef _DEBUG
#else			//发布版要控制权限
if (GetPower() != 0)
{
	exit(0);
}
#endif
	CUserMessagePump *pPump = CUserMessagePump::GetInstance();
	if (pPump)
	{
		pPump->SetHwnd(USERWINDOWMSG_1,m_hWnd);
	}
	CString strAppPath = "";
	CString strTemp = "";
	int iTemp = 0;
	CUserFile *pFile = CUserFile::GetInstance();
	pFile->GetAppPath(strAppPath);	
	if (pFile)
	{
		pFile->ReadInitInfo(strAppPath+"set.ini","Fresh","path",g_strAppPath,iTemp,0);
		pFile->ReadInitInfo(strAppPath+"set.ini","Fresh","roompwd",g_strRoomPwd,iTemp,0);
		pFile->ReadInitInfo(strAppPath+"set.ini","Fresh","type",strTemp,g_iWinOrLose,1);
		pFile->ReadInitInfo(strAppPath+"set.ini","Fresh","losenum",strTemp,g_iLoseNum,1);
		pFile->ReadInitInfo(strAppPath+"set.ini","Fresh","desknum",strTemp,g_iRoomNum,1);
		pFile->ReadInitInfo(strAppPath+"set.ini","Fresh","setnum",strTemp,g_iSetNum,1);
		pFile->ReadInitInfo(strAppPath+"set.ini","Fresh","min",strTemp,g_iMinBalance,1);
		pFile->ReadInitInfo(strAppPath+"set.ini","Fresh","max",strTemp,g_iMaxBalance,1);
		pFile->CheckInitValue(g_iSetNum,0,5);
		pFile->CheckInitValue(g_iRoomNum,1,60);
		pFile->CheckInitValue(g_iLoseNum,1,60000);
		pFile->CheckInitValue(g_iWinOrLose,0,1);
		pFile->CheckInitValue(g_iMinBalance,10,3000);
		pFile->CheckInitValue(g_iMaxBalance,10000,100000000);
		pFile->ReadInitInfo(strAppPath+"set.ini","Fresh","checkgm",strTemp,g_iIsCheckGm,1);
		pFile->CheckInitValue(g_iIsCheckGm,0,1);
		
		pFile->ReadInitInfo(strAppPath +"set.ini","Fresh","hidewindow",strTemp,g_iIsHidePlayWindow,1);
		pFile->CheckInitValue(g_iIsHidePlayWindow,0,1);
		m_HideWindow.SetCheck(g_iIsHidePlayWindow);
		pFile->ReadInitInfo(strAppPath +"set.ini","Fresh","speed",strTemp,g_iSpeed,1);
		pFile->CheckInitValue(g_iSpeed,1,100);
		int iValueEx = 100 - g_iSpeed;
		pFile->CheckInitValue(iValueEx,1,100);
		m_SpeedCtrl.SetPos(iValueEx);

		pFile->ReadInitInfo(strAppPath +"set.ini","Fresh","stopminutes",strTemp,g_iStopMinutes,1);
		pFile->CheckInitValue(g_iStopMinutes,1,100);
		SetDlgItemInt(IDC_EDIT_STOPMINUTES,g_iStopMinutes);

		pFile->ReadInitInfo(strAppPath +"set.ini","Fresh","checkgmwait",strTemp,g_iCheckGmWait,1);
		pFile->ReadInitInfo(strAppPath +"set.ini","Fresh","checkinterval",strTemp,g_iCheckGmWaitInterval,1);
		pFile->ReadInitInfo(strAppPath+"set.ini","Fresh","gmstr",g_strGmStr,iTemp,0);
		pFile->CheckInitValue(g_iCheckGmWait,0,1);
		pFile->CheckInitValue(g_iCheckGmWaitInterval,2,100);
		SetDlgItemInt(IDC_EDIT_CHECKINTERVAL,g_iCheckGmWaitInterval);
		SetDlgItemText(IDC_EDIT_GMSTR,g_strGmStr);
		m_CheckGM.SetCheck(g_iIsCheckGm);
		m_gmCheckWait.SetCheck(g_iCheckGmWait);
		OnBnClickedCheckGmcheck();
		pFile->ReadInitInfo(strAppPath +"set.ini","Fresh","setpwd",strTemp,g_iIsSetPwd,1);
		pFile->CheckInitValue(g_iIsSetPwd,0,1);
		((CButton *)GetDlgItem(IDC_CHECK_SETPWD))->SetCheck(g_iIsSetPwd);
		
		pFile->ReadInitInfo(strAppPath +"set.ini","Fresh","minmoney",strTemp,g_iMinMoney,1);
		pFile->CheckInitValue(g_iMinMoney,0,10000);
		SetDlgItemInt(IDC_EDIT_MINMONEY,g_iMinMoney);
	}
	SetDlgItemInt(IDC_EDIT_MINNUM,g_iMinBalance);
	SetDlgItemInt(IDC_EDIT_MAXNUM,g_iMaxBalance);
	SetDlgItemText(IDC_EDIT_ROOMPWD,g_strRoomPwd);
	SetDlgItemText(IDC_EDIT_EXEPATH,g_strAppPath);
	SetDlgItemInt(IDC_EDIT_DESKNUM,g_iRoomNum);
	SetDlgItemInt(IDC_EDIT_SETNUM,g_iSetNum+1);
	SetDlgItemInt(IDC_EDIT_LOSENUM,g_iLoseNum);
#ifdef ONLY_LOSER
	GetDlgItem(IDC_RADIO_WIN)->ShowWindow(SW_HIDE);
	g_iWinOrLose = 0;
#else
#ifdef ONLY_WINNNER
	GetDlgItem(IDC_RADIO_LOSE)->ShowWindow(SW_HIDE);
	g_iWinOrLose = 1;
#endif
#endif
	if (g_iWinOrLose == 1)
	{
		OnBnClickedRadioWin();
		SetWindowText("赢家客户端");
	}
	else
	{
		OnBnClickedRadioLose();
		SetWindowText("输家客户端");
	}
	SetHook();
	m_iCurrTime = -100000;
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CLoserWindowDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CLoserWindowDlg::OnPaint() 
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
HCURSOR CLoserWindowDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
/************************************************************************/
/* 开始刷分                                                             */
/************************************************************************/
void CLoserWindowDlg::OnBnClickedBtnStart()
{
	if (SaveParams()<0)
	{
		return;
	}
	g_ExitFlag = 0;
	AfxBeginThread((AFX_THREADPROC)ThreadFunc,(LPVOID)this);
	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
}
/************************************************************************/
/*  刷分线程                                                            */
/************************************************************************/
void CLoserWindowDlg::ThreadFunc(LPVOID lParam)
{
	CLoserWindowDlg *pParam = ((CLoserWindowDlg *)lParam);
	if ( pParam )
	{
		pParam->RunFunc();
		pParam->ShowMessage("线程退出");
		pParam->GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
		pParam->GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	}
}
/************************************************************************/
/*  刷分线程函数                                                        */
/************************************************************************/
void CLoserWindowDlg::RunFunc()
{
	CString sendStr = "";
	ShowMessage("初始化成功");
	int iLoginError = 0;
	int iRet = -1;
	CWindowFind find;
	CString strTemp = "";
	while (!g_ExitFlag)
	{	
		RegisterPack rgPack;		//获取第一个玩家账号
		iRet = GetRegisterPack(rgPack);
		if (iRet != 0)
		{
			if (g_iWinOrLose == 1)
			{
				ShowMessage("没有账号可用,请导入");	//报警
			}
			else
			{
				ShowMessage("没有输家账号可用,请导入");	//报警
			}
			Sleep(1000);
			continue;
		}
		if (g_strOldAcc != rgPack.strAcc)
		{
			iRet = Login(rgPack.strAcc,rgPack.strPwd);
		}
		else
		{
			ShowMessage("此账号已经登录");
			Sleep(500);
			iRet = 0;
		}
		if (iRet < 0)	//登录失败
		{
			ShowMessage("程序无法继续运行下去,请检测配置!");
			return;
		}
		else if (iRet == 5)//账号密码错误
		{
			iLoginError++;
			if (iLoginError >=3)
			{
				AddFreshResult(rgPack,1);
				g_RegisterList.RemoveAt(0);
			}
			continue;
		}
		else if (iRet != 0)		//重启程序成功
		{
			continue;
		}
		g_strOldAcc = rgPack.strAcc;
		//登录成功,登录成功后开始进入游戏
		//是否已经进入房间
		sendStr.Format("扎金花游戏客户端 ［ 普通房间1 - 第 %d 桌 ］",g_iRoomNum);
		HWND hParant = ::FindWindow("#32770",sendStr);
		if (!hParant)
		{
			//从这里找设置和退出按钮,以及右侧的LIST,获取余额信息
			iRet = GetInitBtnInfo(g_hwndTopMost);
			if (iRet !=0 )	//获取信息失败
			{
				hParant = find.GetChildWindow(g_hwndTopMost,3);
				find.PostBtnClick(hParant);
				Sleep(100);
				TryIntoRoom();	//进入房间
				Sleep(200);
				continue;
			}
			if (g_iIsSetPwd == 1)			//如果需要设置密码
			{
				iRet = SetPassWord(g_strRoomPwd);
				if (iRet != 0)
				{
					continue;
				}
			}
			Sleep(100);
			//进入房间并且已经设置成功
			//获取余额,如果余额小于一定数量,不再进入房间
			//................
			if (IsGmHere(g_hwndListView1))
			{
				sendStr.Format("GM来了,退出,%d分钟后再进",g_iStopMinutes);
				ShowMessage(sendStr);
				BackToLoginStatus();
				int iTotalSeconds = 60*g_iStopMinutes;
				int iCount = 0;
				while(!g_ExitFlag)
				{
					Sleep(1000);
					if (iCount >= iTotalSeconds)
					{
						break;
					}
					iCount++;
					sendStr.Format("%d秒后将重新进入游戏房间...",iTotalSeconds - iCount);
					ShowMessage(sendStr);
				}
				continue;
			}
			//获取余额
			int iBalance = GetBalance(g_hwndListView1);
			g_strBalance.Format("%d",iBalance);
			rgPack.strBalance = g_strBalance;
			if (iBalance < g_iMinBalance)	//余额小于设定数,退出
			{
				strTemp.Format("余额已经小于设置值:%d,刷分完成,换号...",g_iMinBalance);
				ShowMessage(strTemp);
				AddFreshResult(rgPack,0);
				g_RegisterList.RemoveAt(0);
				continue;
			}
			if (g_iWinOrLose==1 && iBalance > g_iMaxBalance)//如果是赢家且余额数大于设定数,退出
			{
				strTemp.Format("余额已经大于设置值:%d,刷分完成,换号...",g_iMaxBalance);
				ShowMessage(strTemp);
				AddFreshResult(rgPack,0);
				g_RegisterList.RemoveAt(0);				
				continue;
			}
			iRet = TryIntoDesk(g_iRoomNum,g_iSetNum);
			if (iRet < 0)
			{
				if (iRet == -88)		//余额不够,换号
				{
					AddFreshResult(rgPack,0);
					g_RegisterList.RemoveAt(0);
				}
				ShowMessage("进入房间失败");
				BackToLoginStatus();
				continue;
			}
			//进入房间成功,开始游戏
			int iRet = PlayGame();
			BackToLoginStatus();
			ShutUpWindow();//点击强退			
			if (iRet == 0)		//换账号,如果是正常退出,说明此号已经刷完,退出
			{
				sendStr.Format("%s家游戏退出",g_iWinOrLose==0?"输":"赢");
				ShowMessage(sendStr);
				AddFreshResult(rgPack,0);
				g_RegisterList.RemoveAt(0);
				continue;
			}
			else if (iRet == 2)
			{
				sendStr.Format("程序控制退出");
				ShowMessage(sendStr);
				continue;
			}
			else if (iRet == 99)
			{
				ShowMessage("在游戏时检测到GM,退出...");
				BackToLoginStatus();
				int iTotalSeconds = 60*g_iStopMinutes;
				int iCount = 0;
				while(!g_ExitFlag)
				{
					Sleep(1000);
					if (iCount >= iTotalSeconds)
					{
						break;
					}
					iCount++;
					sendStr.Format("%d秒后将重新进入游戏房间...",iTotalSeconds - iCount);
					ShowMessage(sendStr);
				}
				continue;
			}
			else
			{
				TryIntoRoom();	//进入房间
			}
		}
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//获取发送数据,返回0表示正常,-1表示无数据,-2表示出错
int CLoserWindowDlg::GetRegisterPack(RegisterPack &submitPack)
{
	int iRet = 0;
	try
	{
		int iSize = g_RegisterList.GetSize();
		if (iSize == 0)	//缓冲区无数据
		{
			iRet = -1;
		}
		else
		{
			submitPack = g_RegisterList.GetAt( 0 );			
			iRet = 0;	//正常获取到数据
		}		
	}
	catch(...)
	{
		iRet = -2;		//出错
	}
	return iRet;
}
/************************************************************************/
/*  登录                                                                */
/************************************************************************/
int CLoserWindowDlg::Login(CString strUser,CString strPwd)
{
	//登录操作
	HWND hwnd = NULL;
	HWND hwndChind = NULL;
	CWnd *pWnd = NULL;
	CWnd *pParantWnd = NULL;
	CWindowFind find;
	CString strTemp  = "";
	TCHAR   szClassName[256]={0};
	CString strShow = "";
	g_hwndTopMost = NULL;
	g_hwndTopMost = ::FindWindow("7PMIGameLobby","游戏大厅");
	if (!g_hwndTopMost)
	{
		//启动程序
		HINSTANCE iRet = ShellExecute(NULL, "open", g_strAppPath, NULL, NULL, SW_SHOW);
		if ((int)iRet<32)
		{
			ShowMessage("启动游戏失败,请检查配置");
			return -3;
		}
		ShowMessage("启动程序成功");
		Sleep(1500);
		return 2;		//返回2表示重复登录
	}
	BackToLoginStatus();
	hwnd = find.MyFindWindow("Static","游戏帐号：");
	while (hwnd)
	{
		if (hwnd)	//登录框的子窗口
		{
			hwnd = ::GetParent(hwnd);
			if (hwnd)
			{
				//hwnd = pParantWnd->GetSafeHwnd();
				//找账号框
				hwndChind = find.GetChildWindow(hwnd,6);	//账号选择框
				Sleep(40);
				hwndChind = find.GetChildWindow(hwndChind,1);//账号输入编辑框
				Sleep(40);
				find.SetEditText(hwndChind,strUser);

				//密码框
				hwndChind = find.GetChildWindow(hwnd,7);
				Sleep(40);
				find.SetEditText(hwndChind,strPwd);
				//登录按钮
				hwndChind = find.GetChildWindow(hwnd,9);
				Sleep(20);
				::PostMessage(hwndChind,WM_LBUTTONDOWN,0,MAKELONG(1,1));
				Sleep(10);
				::PostMessage(hwndChind,WM_LBUTTONUP,0,MAKELONG(1,1));

				Sleep(1000);//等待1秒,看是否有异常消息框
				hwnd = find.MyFindWindow("#32770","提示信息");
				hwndChind = find.GetChildWindow(hwnd,4);
				if (hwndChind)
				{
					if (find.GetContrlWindowText(hwndChind) == "您的帐号不存在或者密码输入有误，请查证后再次尝试登录！")
					{
						if (::IsWindowVisible(hwndChind) && ::IsWindowVisible(hwnd))
						{
							ShowMessage("账号或者密码错误!");
							::PostMessage(hwnd,WM_CLOSE,0,0);
							return 5;
						}
					}
				}//查找树列表,并获取值,看是否有消息
				hwnd = find.MyFindWindow("Static","游戏帐号：");
				if (!hwnd)
				{
					return 0;//登录成功,执行下一步
				}
				else
				{
					return 3;
				}
			}
			else
			{
				ShowMessage("程序已经在运行中");
				Sleep(200);
				return 3;
			}
		}
		else
		{
			//判断主窗口是否已经启动		
			ShowMessage("程序已经在运行中");
			//切换账号
			hwnd = ::FindWindow("#32770","提示信息");
			if (!hwnd)
			{
				::PostMessage(g_hwndTopMost,WM_CLOSE,0,0);
				Sleep(400);
				return 2;
			}
			hwndChind = find.GetChildWindow(hwnd,3);
			if (hwndChind)
			{
				CString strWinName = find.GetContrlWindowText(hwndChind);
				if ( strWinName.Find("切换") != -1)
				{
					find.PostBtnClick(hwndChind);
					ShowMessage("切换账号");
					Sleep(200);
				}						
			}//查找树列表,并获取值,看是否有消息			
			return 1;
		}
	}
	
	return 0;
}
/************************************************************************/
/* 显示信息                                                             */
/************************************************************************/
void CLoserWindowDlg::ShowMessage(CString strMsg)
{
	CUserMessagePump *pMsgPump = NULL;
	pMsgPump = CUserMessagePump::GetInstance();
	if (pMsgPump)
	{
		pMsgPump->AddToMessagePump(USERWINDOWMSG_1,strMsg);
	}
}
void CLoserWindowDlg::OnBnClickedButton2()
{
	Login("","");
	//判断登录后的目录树
	TryIntoRoom();
}
/************************************************************************/
/* 将信息放入粘贴板                                                     */
/************************************************************************/
bool CLoserWindowDlg::CopyStringToClipboard(HWND hWnd, LPCTSTR lpszText)
{
	EmptyClipboard();
	HGLOBAL hglbCopy;
	LPTSTR  lptstrCopy;
	int nlen = strlen(lpszText);
	if (nlen == 0)
	{
		return FALSE;
	}
	if (!::OpenClipboard(hWnd))
		return FALSE;

	hglbCopy = GlobalAlloc(GMEM_DDESHARE, 
		(nlen + 1) * sizeof(char)); 

	if (hglbCopy == NULL) 
	{ 
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
/*  进入游戏房间                                                        */
/************************************************************************/
int CLoserWindowDlg::TryIntoRoom()
{
	HWND hwnd = NULL;
	HWND hwndChind = NULL;
	CWnd *pWnd = NULL;
	CWnd *pParantWnd = NULL;
	CWindowFind find;
	CString strTemp  = "";
	TCHAR   szClassName[256]={0};
	CString strShow = "";
	HWND hParant = ::FindWindow("7PMIGameLobby","游戏大厅");
	g_hwndTopMost = hParant;
	if (hParant)
	{
		hwnd = find.GetChildWindow(hParant,5);//"SysTreeView32"
		hwnd = find.GetChildWindow(hwnd,1);
		hwnd = find.GetChildWindow(hwnd,1);
		hwnd = find.GetChildWindow(hwnd,1);
		if (hwnd && find.GetContrlClassName(hwnd) =="SysTreeView32")	//登录框的子窗口
		{
			HTREEITEM hItem = TreeView_GetRoot(hwnd);
			HTREEITEM hChild = FindItem(hwnd,hItem,"诈金花");
			if (!hChild)
			{
				ShowMessage("没找到扎金花房间");
				return -1;
			}
			hChild = FindItem(hwnd,hChild,"银子房间1");
			if (!hChild)
			{
				ShowMessage("没找到普通房间1");
				return -1;
			}
			TreeView_EnsureVisible(hwnd,hChild);
			TreeView_Select(hwnd,hChild,TVGN_CARET);
			CRect rect;
			GetTreeItemRect(hwnd,hChild,rect);
			::SetCursorPos(rect.left+120,rect.top+210);
			::SendMessage(hwnd,WM_LBUTTONDBLCLK,0,0);

			//::SendMessage(hwnd,WM_NCHITTEST,0,MAKELONG(rect.left+120,rect.top+210));
			//::SendMessage(hwnd,WM_LBUTTONDBLCLK,0,MAKELONG(rect.left+120,rect.top+210));			
			//::SendMessage(hwnd,WM_LBUTTONDBLCLK,0,MAKELONG(rect.left+140,273));
			//DblClickItemNodeEx(hwnd);
		}
		else
		{
			Sleep(500);
			return -1;
		}
	}
	else
	{
		ShowMessage("程序未启动");
		g_strOldAcc = "";
		return -2;
	}
	return 0;
}
/************************************************************************/
/* 进入游戏桌                                                           */
/************************************************************************/
int CLoserWindowDlg::TryIntoDesk(int iIndex,int iSetIndex)
{
	CWindowFind find;
	CPoint pnt = DestIndexToPointEx(iIndex,iSetIndex);			//一个桌号
	CString strShow = "";
	pnt.x -= 9;
	pnt.y-=131;
	HWND tempHWND = g_hwndDesk;
	TCHAR   szClassName[256];     
	GetClassName(tempHWND,   szClassName,256);
	ShowMessage(szClassName);
	if ("AfxWnd70" == CString(szClassName))
	{//找到房间
		MoveScrollBar(iIndex);
		::SendMessage(tempHWND, WM_LBUTTONDOWN, 0,MAKELONG(pnt.x,pnt.y));//发送双击消息
		::SendMessage(tempHWND, WM_LBUTTONUP, 0,MAKELONG(pnt.x,pnt.y));//发送双击消息
		//查找游戏桌面是否已经启动
		Sleep(500);
		int iCount = 0;
		CString strTemp = "";
		g_hwndPlayWindow = NULL;
		while (!g_hwndPlayWindow)
		{
			strTemp.Format("扎金花游戏客户端 ［ 银子房间1 - 第 %d 桌 ］",iIndex);
			g_hwndPlayWindow = ::FindWindow("#32770",strTemp);
			if (g_hwndPlayWindow)	//找到进行游戏的窗口	控制它变小,移动,并且获取其中的小按钮
			{
				::SetWindowPos(g_hwndPlayWindow,HWND_TOP,0,0,0,0,SWP_NOSIZE|SWP_SHOWWINDOW|~SWP_NOMOVE);
				::MoveWindow(g_hwndPlayWindow,20,120,500,400,TRUE);
				ShowMessage("找到扎金花房间");
			}
			//此处找警告框,说余额不够
			HWND hwndWarn = ::FindWindow("#32770","提示信息");
			if (hwndWarn)
			{
				HWND hwndNotice = find.GetChildWindow(hwndWarn,4);
				if (find.GetContrlWindowText(hwndNotice) == "对不起，您的游戏币不满足桌主底分设置！")
				{
					ShowMessage("余额不足底分设置,退出");
					find.PostBtnClick(find.GetChildWindow(hwndWarn,1));
					::PostMessage(hwndWarn,WM_CLOSE,0,0);
					return -88;
				}
				else
				{	
					::PostMessage(hwndWarn,WM_CLOSE,0,0);
					return -1;
				}
			}
			iCount++;
			if (iCount > 200)
			{
				ShowMessage("未找到找到扎金花房间,请确定您配置的桌号和座位号正确");
				return -1;
			}
			Sleep(50);
		}

		//找到扎金花房间后,设置底注
		//如果是设置的赢家,查找设置30底注的按钮,点击之
		if (g_iWinOrLose == 1)
		{
			HWND hwndSet  = NULL;
			iCount = 0;
			while (!hwndSet)
			{
				iCount ++;
				if (iCount > 30)
				{
					ShowMessage("未找到设置窗口,退出");
					break;
				}
				hwndSet = ::FindWindow("#32770","请设置底注");
				if (hwndSet)
				{
					HWND hwndBtn = NULL;
					strTemp.Format("%d",g_iMinMoney);
					hwndBtn = find.GetSelectChildWindow(hwndSet,strTemp);
					if (hwndBtn)
					{
						strTemp.Format("找到%d底注",g_iMinMoney);
						ShowMessage(strTemp);
						find.PostBtnClick(hwndBtn);
						Sleep(200);
					}
					else
					{
// 						strTemp.Format("未找到%d底注,取第一个",g_iMinMoney);
// 						ShowMessage(strTemp);
// 						hwndBtn = find.GetChildWindow(hwndSet,1);
// 						find.PostBtnClick(hwndBtn);
// 						Sleep(200);
						hwndSet = NULL;
						break;
					}					
				}
				else
				{
					Sleep(50);
				}				
			}
			hwndSet = NULL;
			ShowMessage("未找到设置窗口,寻找询问窗口");
			iCount = 0;
			while (!hwndSet)
			{
				iCount ++;
				if (iCount > 100)
				{
					ShowMessage("未找到询问窗口,退出");
					return -1;
				}
				hwndSet = find.MyFindBtnWindowEx("#32770",NULL);
				if (hwndSet)
				{
					::PostMessage(hwndSet,WM_LBUTTONDOWN,0,MAKELONG(1,1));
					Sleep(10);
					::PostMessage(hwndSet,WM_LBUTTONUP,0,MAKELONG(1,1));
					Sleep(10);
				}
				else
				{
					Sleep(50);
				}
			}
		}
		else	//如果设置的是输家
		{
			HWND hwndSet  = NULL;
			iCount = 0;
			while (!hwndSet)
			{
				iCount ++;
				if (iCount > 30)
				{
					ShowMessage("未找到询问窗口,查找设置窗口");
					break;
				}
				hwndSet = find.MyFindBtnWindowEx("#32770",NULL);
				if (hwndSet)
				{
					::PostMessage(hwndSet,WM_LBUTTONDOWN,0,MAKELONG(1,1));
					Sleep(10);
					::PostMessage(hwndSet,WM_LBUTTONUP,0,MAKELONG(1,1));
					//return 0;
				}
				else
				{
					Sleep(50);
				}
			}
			hwndSet = NULL;
			iCount = 0;
			while (!hwndSet)
			{
				iCount ++;
				if (iCount > 2)
				{
					ShowMessage("未找到设置窗口,退出");
					return 0;
				}
				hwndSet = ::FindWindow("#32770","请设置底注");
				if (hwndSet)
				{
					HWND hwndBtn = NULL;
					strTemp.Format("%d",g_iMinMoney);
					hwndBtn = find.GetSelectChildWindow(hwndSet,strTemp);
					if (hwndBtn)
					{
						strTemp.Format("找到%d底注",g_iMinMoney);
						ShowMessage(strTemp);
						find.PostBtnClick(hwndBtn);
						Sleep(200);
					}
					else
					{
						::PostMessage(hwndSet,WM_CLOSE,0,0);
						return -88;
					}
				}
				else
				{
					Sleep(50);
				}				
			}
		}
	}
	else
	{
		ShowMessage("未找到房间,请保证游戏窗口显示正常!");
		return -1;
	}
	return 0;
}
/************************************************************************/
/* 开始刷分                                                             */
/************************************************************************/
int CLoserWindowDlg::PlayGame()
{
	HWND hwnd = NULL;
	HWND hwnd100 = NULL;
	HWND hwnd200 = NULL;
	HWND hwnd400 = NULL;
	HWND hwnd800 = NULL;
	HWND hwndGiveUp = NULL;
	HWND hwndShow = NULL;
	HWND hwndStart = NULL;
	HWND hwndGen= NULL;
	HWND hwndKaipai = NULL;
	CWindowFind find;
	CString strTemp = "";
	int iCount = 0;
	hwnd = find.GetChildWindow(g_hwndPlayWindow,4);//find.MyFindWindow("AfxWnd70","",hParant);
	if (hwnd)
	{
		SetLastWaitCheckTime();
		//1是语音,不管
		hwnd800 = find.GetChildWindow(hwnd,6);		//最大加注
		hwnd400 = find.GetChildWindow(hwnd,7);
		hwnd200 = find.GetChildWindow(hwnd,8);
		hwnd100 = find.GetChildWindow(hwnd,9);
		hwndGiveUp = find.GetChildWindow(hwnd,4);	//放弃
		hwndShow = find.GetChildWindow(hwnd,2);		//看牌
		hwndStart = find.GetChildWindow(hwnd,10);	//开始游戏
		hwndGen = find.GetChildWindow(hwnd,5);		//跟注
		hwndKaipai = find.GetChildWindow(hwnd,3);	//开牌
		int iTimeJuShu = 1;
		int iAddCount = 1;
		while(true)
		{
			if (g_ExitFlag)
			{
				strTemp.Format("程序控制退出,正常退出...");
				ShowMessage(strTemp);
				return 2;
			}
			strTemp.Format("扎金花游戏客户端 ［ 银子房间1 - 第 %d 桌 ］",g_iRoomNum);
			HWND hParant = ::FindWindow("#32770",strTemp);
			if (!hParant)
			{
				strTemp.Format("游戏窗口已经关闭,退出...");
				ShowMessage(strTemp);
				return 3;
			}			
			//判断开始按钮是否可用
			if(::IsWindowVisible(hwndStart))//::IsWindowEnabled(hwndStart)		//一局开始以前,到时候改成是否可用
			{
				SetLastWaitCheckTime();
				//检测GM				
				if (IsGmHere(g_hwndListView1))
				{
					ShowMessage("GM来了,退出...");
					return 99;
				}
				//获取余额
				int iBalance = GetBalance(g_hwndListView1);
				g_strBalance.Format("%d",iBalance);
				if (iBalance < g_iMinBalance)	//余额小于设定数,退出
				{
					strTemp.Format("余额已经小于设置值:%d,刷分完成,换号...",g_iMinBalance);
					ShowMessage(strTemp);					
					return 0;
				}
				if (g_iWinOrLose==1 && iBalance > g_iMaxBalance)//如果是赢家且余额数大于设定数,退出
				{
					strTemp.Format("余额已经大于设置值:%d,刷分完成,换号...",g_iMaxBalance);
					ShowMessage(strTemp);
					return 0;
				}
				find.PostBtnClick(hwndStart);
				strTemp.Format("开始,第%d局,余额:%d",iTimeJuShu,iBalance);
				ShowMessage(strTemp);
				iTimeJuShu ++;
				Sleep(400);
			}
			if (::IsWindowEnabled(hwnd800))
			{
				if (g_iWinOrLose == 0)		//如果是输家
				{
					if (::IsWindowEnabled(hwndGiveUp))
					{
						find.PostBtnClick(hwndGiveUp);
						ShowMessage("放弃");
						iAddCount = 1;
						Sleep(20*g_iSpeed);
						continue;
					}
// 					if (iAddCount > 4)		//如果已经加注四次
// 					{
// 						ShowMessage("输家已加注4次,放弃...");
// 						if (::IsWindowEnabled(hwndGiveUp))
// 						{
// 							find.PostBtnClick(hwndGiveUp);
// 							ShowMessage("放弃");
// 							iAddCount = 1;
// 							Sleep(20*g_iSpeed);
// 							continue;
// 						}
// 						find.PostBtnClick(hwndGiveUp);
// 						ShowMessage("放弃");
// 						iAddCount = 1;
// 						Sleep(20*g_iSpeed);
// 						continue;
// 					}
// 					find.PostBtnClick(hwnd800);
// 					strTemp.Format("第%d次加注,加注额 %s...",iAddCount,find.GetContrlWindowText(hwnd800));
// 					ShowMessage(strTemp);
// 					iAddCount ++;
				}
				else			//赢家
				{
					find.PostBtnClick(hwnd800);
					strTemp.Format("加注 %s...",find.GetContrlWindowText(hwnd800));
					ShowMessage(strTemp);
				}
				Sleep(20*g_iSpeed);
			}
			else		//此处处理输赢不同的流程
			{
				BOOL bRet = FALSE;
				if (g_iWinOrLose == 1)//赢的流程
				{
					bRet = ::IsWindowEnabled(hwndGen);
					if (bRet)
					{
						find.PostBtnClick(hwndGen);	
						ShowMessage("跟注");
					}					
				}
				else	//输
				{
					bRet = ::IsWindowEnabled(hwndGiveUp);					
					if (bRet)
					{
						find.PostBtnClick(hwndGiveUp);
						ShowMessage("放弃");
						iAddCount = 1;
					}
				}
				if (bRet)
				{
					Sleep(20*g_iSpeed);
				}
				else
				{
					if(IsOverCheckTime(g_iCheckGmWaitInterval))
					{
						ShowMessage("等待检测间隔已过,检测GM...");
						SetLastWaitCheckTime();
						if (IsGmHere(g_hwndListView1))
						{
							ShowMessage("GM来了,退出...");
							return 99;
						}
					}
					ShowMessage("等待中...");
					if (g_iWinOrLose == 0)
					{
						HWND hwndSet  = NULL;
						hwndSet = find.MyFindBtnWindowEx("#32770",NULL);
						if (hwndSet)
						{
							::PostMessage(hwndSet,WM_LBUTTONDOWN,0,MAKELONG(1,1));
							Sleep(20);
							::PostMessage(hwndSet,WM_LBUTTONUP,0,MAKELONG(1,1));
							Sleep(20);
						}
						else
						{
							hwndSet  = NULL;
							hwndSet = ::FindWindow("#32770","请设置底注");
							if (hwndSet && ::IsWindowVisible(hwndSet))
							{
								HWND hwndBtn = NULL;
								hwndBtn = find.GetSelectChildWindow(hwndSet,strTemp);
								if (hwndBtn)
								{
									strTemp.Format("找到%d底注",g_iMinMoney);
									ShowMessage(strTemp);
									find.PostBtnClick(hwndBtn);
								}
								else
								{
									::PostMessage(hwndSet,WM_CLOSE,0,0);
									return 0;
								}
							}
						}
					}
					else
					{
						HWND hwndSet  = NULL;
						hwndSet = ::FindWindow("#32770","请设置底注");
						if (hwndSet && ::IsWindowVisible(hwndSet))
						{
							HWND hwndBtn = NULL;
							hwndBtn = find.GetSelectChildWindow(hwndSet,strTemp);
							if (hwndBtn)
							{
								strTemp.Format("找到%d底注",g_iMinMoney);
								ShowMessage(strTemp);
								find.PostBtnClick(hwndBtn);
							}
							else
							{
								Sleep(3000);
								return -1;
							}
						}
						else
						{
							hwndSet  = NULL;
							hwndSet = find.MyFindBtnWindowEx("#32770",NULL);
							if (hwndSet)
							{
								::SendMessage(hwndSet,WM_LBUTTONDOWN,0,MAKELONG(1,1));
								Sleep(20);
								::SendMessage(hwndSet,WM_LBUTTONUP,0,MAKELONG(1,1));
								Sleep(20);
							}
						}
					}
					Sleep(150);
				}
			}
		}
	}
	else
	{
		ShowMessage("未找到操作按钮,退出");
	}
	return 0;
}
/************************************************************************/
/*设置密码函数                                                          */
/************************************************************************/
int CLoserWindowDlg::SetPassWord(CString strPwd)
{
	CWindowFind find;
	find.PostBtnClick(g_hwndSet);		//点击设置按钮
	Sleep(200);
	//判断是否找到设置按钮
	HWND hwnd = NULL;
	HWND hParant  = NULL;
	HWND hChild = NULL;
	int iCount = 0;
	while (!hParant)
	{
		hParant = ::FindWindow("#32770","配置选项");
		if (hParant)		//已经找到设置项,打开了设置框,现在设置密码
		{
			ShowMessage("找到密码设置框");
			hwnd = find.GetChildWindow(hParant,4);		//设置密码的
			hChild = find.GetChildWindow(hwnd,12);
			if (hChild)
			{
				((CButton*)FromHandle(hChild))->SetCheck(TRUE);
			}
			hChild = find.GetChildWindow(hwnd,10);
			if (hChild)
			{
				((CButton*)FromHandle(hChild))->SetCheck(FALSE);
			}
			hChild = find.GetChildWindow(hwnd,11);
			if (hChild)
			{
				((CButton*)FromHandle(hChild))->SetCheck(FALSE);
			}			
			hChild = find.GetChildWindow(hwnd,13);
			find.SetEditText(hChild,g_strRoomPwd);
			hwnd = find.MyFindWindow("Button","桌子携带密码：");
			if (hwnd)
			{
				((CButton*)FromHandle(hwnd))->SetCheck(TRUE);
			}			
			hwnd = find.GetChildWindow(hParant,1);
			find.PostBtnClick(hwnd);
			ShowMessage("密码已经设置");
			return 0;
		}
		ShowMessage("未找到密码设置框...");
		iCount++;
		if (iCount > 3)
		{
			ShowMessage("未找到密码设置框,请保证游戏窗口显示正常!");
			return -1;
		}
		Sleep(100);
	}
	return 0;
}

/************************************************************************/
/* 获取外部进程树结点所在区域                                           */
/************************************************************************/
bool  CLoserWindowDlg::GetTreeItemRect(HWND hwnd, HTREEITEM TreeItem, RECT& Rect)
{
	bool result = false;
	DWORD processId = 0;
	GetWindowThreadProcessId(hwnd, &processId);
	HANDLE process = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, processId);
	if (process != NULL)
	{
		LPVOID buffer = VirtualAllocEx(process, NULL, 4096, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		if (buffer != NULL)
		{
			SIZE_T bytes;
			WriteProcessMemory(process, buffer, &TreeItem, sizeof(TreeItem), &bytes);
			result = (::SendMessage(hwnd, TVM_GETITEMRECT, FALSE, (LPARAM)buffer) != 0);
			ReadProcessMemory(process, buffer, &Rect, sizeof(Rect), &bytes);
			VirtualFreeEx(process, buffer, 0, MEM_RELEASE);
		}
		CloseHandle(process);
	}
	return result;
}
/************************************************************************/
/*  获取外部进程树的结点文字信息                                        */
/************************************************************************/
CString CLoserWindowDlg::GetItemNodeText(HWND hwnd,HTREEITEM hItem)
{
	CString strText = "";
	DWORD PID;
	HANDLE hProcess;
	const int bufferLength = 512;
	LPTVITEM lpItem = (LPTVITEM)new BYTE[bufferLength];
	GetWindowThreadProcessId(hwnd, &PID);
	hProcess=OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, TRUE, PID);
	PVOID buffer = VirtualAllocEx(hProcess,NULL,bufferLength,MEM_COMMIT,PAGE_READWRITE);
	lpItem->cchTextMax=512;
	lpItem->mask = TVIF_TEXT;
	lpItem->hItem = hItem;
	lpItem->pszText = LPTSTR((PBYTE)buffer+sizeof(TVITEM));
	::SendMessage(hwnd,TVM_SELECTITEM,TVGN_CARET,(long)lpItem->hItem); 
	WriteProcessMemory(hProcess,buffer,lpItem,sizeof(TVITEM),NULL); 
	::SendMessage(hwnd,TVM_GETITEM,0,(LPARAM)buffer); 
	ReadProcessMemory(hProcess,buffer,lpItem,bufferLength,NULL); 
	strText = (LPTSTR)((PBYTE)lpItem+sizeof(TVITEM));
	CloseHandle(hProcess);
	VirtualFreeEx(hProcess, lpItem, 0, MEM_RELEASE);
	delete[] lpItem;
	lpItem = NULL;
	return strText;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CLoserWindowDlg::DblClickItemNode(HWND hwnd,HTREEITEM hItem)
{
	CString strText = "";
	DWORD PID;
	HANDLE hProcess;
	HWND hParant = ::GetParent(hwnd);
	GetWindowThreadProcessId(hParant, &PID);
	hProcess=OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	NMHDR *pnmh=(NMHDR*)VirtualAllocEx(hProcess, NULL, sizeof(NMHDR), MEM_COMMIT, PAGE_READWRITE); 
	NMHDR nmh; 
	nmh.code = NM_DBLCLK;    // Message type defined by control. 
	nmh.idFrom = ::GetDlgCtrlID(hwnd); 
	nmh.hwndFrom = hwnd; 
	WriteProcessMemory(hProcess, pnmh, &nmh, sizeof(NMHDR), NULL); 
	::SendMessage(hParant,WM_NOTIFY,(WPARAM)nmh.idFrom,(LPARAM)pnmh); 
	CloseHandle(hProcess);
	VirtualFreeEx(hProcess, pnmh, 0, MEM_RELEASE);
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CLoserWindowDlg::OnBnClickedButton1()
{
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CLoserWindowDlg::OnBnClickedButton3()
{
	SetTimer(1,1000,NULL);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//写日志
void CLoserWindowDlg::WriteLog(CString srcStr,int iFlag)
{
	CTime currTime = CTime::GetCurrentTime();
	try
	{
		CString filepath = "";
		CString destStr = "";
		CString folder = "";
		CString filename = "";
		CString strAppPath = "";
		if (iFlag == 0)			//注册成功列表
		{
			folder = "log";
			filename.Format("%s_freshList.txt",currTime.Format("%Y%m%d"));
		}
		else if(iFlag == 1)		//信息日志
		{
			folder = "log";
			filename.Format("%s_Complete.txt",currTime.Format("%Y%m%d"));
		}
		else if ( iFlag == 2 )	//缓冲日志
		{
			folder = "log";
			filename.Format("%s_log.txt",currTime.Format("%Y%m%d"));
		}
		else			//异常日志 (3)
		{
			folder = "exception";
			filename.Format("%s_exception.txt",currTime.Format("%Y%m%d"));
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
			if( mkdir(folderName) != 0 )//创建目录
			{
				return;
			}
		}					
		destStr.Format("%s	%s",srcStr,currTime.Format("%Y-%m-%d %H:%M:%S"));
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
void CLoserWindowDlg::OnBnClickedButton4()
{
	SetPassWord("123445");
}
/************************************************************************/
/*  根据桌号计算坐标                                                    */
/************************************************************************/
CPoint CLoserWindowDlg::DestIndexToPoint(int iIndex)
{
	int iFirstX = 624;
	int iFirstY = 686;
	int iOffSetY = 175;
	int iOffSetX = 175;
    int iDestX = 0;
	int iDestY = 0;
	int iRet= 0;
	int iTemp = 0;
	iTemp = 61-iIndex;
	iDestX = iFirstX - ((iTemp-1) % 4) * iOffSetX;
	//iIndex变化
	iTemp = iIndex % 12;
	if (iTemp == 9 ||iTemp == 10|| iTemp == 11 ||iTemp== 0)
	{
		iDestY = iFirstY;
	}
	else if (iTemp>=5 && iTemp <=8)
	{
		iDestY = iFirstY - iOffSetY * 1;
	}
	else
	{
		iDestY = iFirstY - iOffSetY * 2;
	}	
	return CPoint(iDestX,iDestY);
}
/************************************************************************/
/*  根据桌号计算坐标                                                    */
/************************************************************************/
CPoint CLoserWindowDlg::DestIndexToPointEx(int iIndex,int iSetIndex)
{
	CPoint pt = DestIndexToPoint(iIndex);
	int iOffSetX = 0;
	int iOffSetY = 0;
	switch (iSetIndex)
	{
	case 0:
		iOffSetX = 0;
		iOffSetY = 0;
		break;
	case 1:
		iOffSetX = -63;
		iOffSetY = -28;
		break;
	case 2:
		iOffSetX = -63;
		iOffSetY = -90;
		break;
	case 3:
		iOffSetX = 0;
		iOffSetY = -110;
		break;
	case 4:
		iOffSetX = 63;
		iOffSetY = -90;
		break;
	case 5:
		iOffSetX = 63;
		iOffSetY = -28;
		break;
	}
	pt.x += iOffSetX;
	pt.y += iOffSetY;
	return pt;
}
/************************************************************************/
/* 移动游戏桌面滚动条                                                   */
/************************************************************************/
void CLoserWindowDlg::MoveScrollBar(int iIndex)
{
	int iValue = ComputeScrollUpValue(iIndex);
	CWindowFind find;
	CPoint pnt = CPoint(102,353);
	HWND tempHWND = g_hwndDesk ;    //取得鼠标指针处窗口句柄
	TCHAR   szClassName[256];     
	TCHAR   szWindowText[256];
	GetClassName(tempHWND,   szClassName,256);     
	::GetWindowText(tempHWND,(LPTSTR)&szWindowText,256);
	if ("AfxWnd70" == CString(szClassName))
	{
		//移动到底部
		switch(iValue)
		{
		case 0:
			::SendMessage(tempHWND,WM_VSCROLL,SB_BOTTOM,1);
			break;
		case 1:
			::SendMessage(tempHWND,WM_VSCROLL,MAKELONG(SB_THUMBTRACK,1725),1);
			break;
		case 2:
			::SendMessage(tempHWND,WM_VSCROLL,MAKELONG(SB_THUMBTRACK,1195),1);
			break;
		case 3:
			::SendMessage(tempHWND,WM_VSCROLL,MAKELONG(SB_THUMBTRACK,665),1);
			break;
		case 4:
			::SendMessage(tempHWND,WM_VSCROLL,MAKELONG(SB_THUMBTRACK,135),1);
			break;
		}
	}
}
/************************************************************************/
/*计算滚动条向上移动的偏移                                              */
/************************************************************************/
int CLoserWindowDlg::ComputeScrollUpValue(int iIndex)
{
	int iRet= 0;
	if (iIndex <=60 && iIndex >= 49)
	{
		iRet = 0;
	}
	else if (iIndex <=48 && iIndex >= 37)
	{
		iRet = 1;
	}
	else if (iIndex <=36 && iIndex >= 25)
	{
		iRet = 2;
	}
	else if (iIndex <=24 && iIndex >= 13)
	{
		iRet = 3;
	}
	else
	{
		iRet = 4;
	}
	return iRet;
}
void CLoserWindowDlg::OnBnClickedButton5()
{
	RunFunc();
}

void CLoserWindowDlg::OnBnClickedButton6()
{
	MoveScrollBar(1);
}

void CLoserWindowDlg::OnBnClickedButton7()
{
	int iValue = GetDlgItemInt(IDC_EDIT1);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CLoserWindowDlg::SetTableStyle()
{
	CListCtrl* conList = &m_ListAcc;
	DWORD dwSytle=::GetWindowLong(conList->m_hWnd,GWL_STYLE);
	// 设置为报表形式
	SetWindowLong(conList->m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=conList->GetExtendedStyle();
	conList->SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CImageList image;
	image.Create(1,16, ILC_COLOR, 0, 0);
	conList->SetImageList(&image, LVSIL_STATE);
	conList->MoveWindow(7,35,450,200);
	conList->InsertColumn(0,"序号",LVCFMT_CENTER,50);
	conList->InsertColumn(1,"账名",LVCFMT_LEFT,110);
	conList->InsertColumn(2,"密码",LVCFMT_LEFT,110);
	conList->InsertColumn(3,"余额",LVCFMT_LEFT,80);
	conList->InsertColumn(4,"状态",LVCFMT_LEFT,50);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CLoserWindowDlg::InitListCtrl()
{
	DWORD dwSytle=::GetWindowLong(m_message.m_hWnd,GWL_STYLE);
	// 设置为报表形式
	SetWindowLong(m_message.m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=m_message.GetExtendedStyle();
	m_message.SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	::SendMessage(m_message.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	m_message.MoveWindow(7,250,450,270);
	m_message.InsertColumn(0,_T("时间"),LVCFMT_LEFT,125);
	m_message.InsertColumn(1,_T("信息"),LVCFMT_LEFT,280);
}
/************************************************************************/
/*  热键停止                                                            */
/************************************************************************/
LRESULT CLoserWindowDlg::StopThread(WPARAM wParam,LPARAM lParam)
{
	ShowMessage("收到停止线程的热键消息...");
	OnBnClickedBtnStop();
	return 0;
}
/************************************************************************/
/* 添加信息显示		                                                    */
/************************************************************************/
LRESULT CLoserWindowDlg::AddToList(WPARAM wParam,LPARAM lParam)
{
	CUserMessagePump *pMsgPump = NULL;
	pMsgPump = CUserMessagePump::GetInstance();
	if (!pMsgPump)
	{
		return 0;
	}
	CString msgBuf;
	pMsgPump->GetPumpMessage(USERWINDOWMSG_1,m_hWnd,msgBuf);
	if ( msgBuf == "" )
	{
		return 0;
	}
	int nCount = m_message.GetItemCount();
	if ( nCount >= 40 )
	{
		m_message.DeleteItem( 0 );
		m_message.DeleteItem( 0 );
		m_message.DeleteItem( 0 );
		nCount -= 3;
	}
	nCount = m_message.GetItemCount();
	CString strShow = m_message.GetItemText(nCount-1,1);
	if (msgBuf == strShow)
	{
		m_message.SetItemText( nCount-1 , 0, CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S") );
		::PostMessage(m_message.m_hWnd,WM_VSCROLL,SB_BOTTOM,0);
		return 0;
	}
	m_message.InsertItem( nCount, "");
	if ( nCount % 2 == 0 )
	{
		m_message.SetRowSelectedBgColor(nCount, RGB(0,0,0) , false);
		m_message.SetRowTxtColor(nCount , RGB(255,0,0) , false);
	}
	else
	{
		m_message.SetRowTxtColor(nCount , RGB(0,0,255) , false);
	}
	m_message.SetRowBgColor(nCount, RGB(230,190,220), false);
	m_message.SetRowStyle(nCount,  LIS_TXTCOLOR | LIS_BGCOLOR);

	m_message.SetItemText( nCount , 0, CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S") );
	m_message.SetItemText( nCount , 1, msgBuf );
	::PostMessage(m_message.m_hWnd,WM_VSCROLL,SB_BOTTOM,0);
	if (msgBuf != "等待中...")
	{
		WriteLog(msgBuf,2);
	}
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CLoserWindowDlg::OnBnClickedBtnStop()
{
	g_ExitFlag = 1;
	GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CLoserWindowDlg::OnBnClickedBtnExit()
{
	PostMessage(WM_CLOSE,0,0);
}
/************************************************************************/
/* 导入                                                                 */
/************************************************************************/
void CLoserWindowDlg::OnBnClickedBtnInport()
{
	CString strAcc="";
	CString strPwd="";
	int   lines   =   0;   
	CStdioFile   file;
	CString   cstr;
	CFileException   fe;
	CString strFilePath = "";
	char szFilter[]="Text files(*.txt)|*.txt";
	CFileDialog dlg(1,NULL,NULL,NULL,szFilter); 
	if(dlg.DoModal()==IDOK) 
	{
		strFilePath = dlg.GetPathName();
	}
	if (strFilePath=="")
	{
		ShowMessage("请选择账号文件");
		return;
	}
	if(file.Open(strFilePath,CFile::modeRead,&fe))   
	{ 
		file.SeekToBegin();
		int idx1=0;
		int idx2=0;
		while (file.ReadString(cstr))
		{
			strAcc = "";
			strPwd = "";
			idx2 =cstr.Find(",");
			if (idx2==-1)
			{
				continue;
			}
			strAcc = cstr.Left(idx2);
			cstr = cstr.Right(cstr.GetLength()-idx2-1);
			idx2 =cstr.Find(",");
			if (idx2==-1)
			{
				continue;
			}
			strPwd = cstr.Left(idx2);
			if (strAcc !="" && strPwd != "")
			{
				AddToAccList(strAcc,strPwd);
			}
		}
		file.Close();
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CLoserWindowDlg::AddToAccList(CString strAcc,CString strPwd)
{
	CListCtrl *pList = &m_ListAcc;
	int nCount = pList->GetItemCount();
	CString listIndex = "";
	for (int i=0;i<nCount;i++)
	{
		if (strAcc == pList->GetItemText(i,1))
		{
			CString strShow = "";
			strShow.Format("%s 重复导入",strAcc);
			ShowMessage(strShow);
			return;
		}
	}
	listIndex.Format("%d",nCount);
	pList->InsertItem(nCount,"",0);
	pList->SetItemText(nCount,0,listIndex);
	pList->SetItemText(nCount,1,strAcc);
	pList->SetItemText(nCount,2,strPwd);
	RegisterPack rgPack;
	rgPack.strAcc = strAcc;
	rgPack.strPwd = strPwd;
	g_RegisterList.Add(rgPack);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CLoserWindowDlg::OnBnClickedRadioLose()
{
	((CButton*)GetDlgItem(IDC_RADIO_LOSE))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_WIN))->SetCheck(FALSE);
	GetDlgItem(IDC_EDIT_LOSENUM)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_MINNUM)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_MAXNUM)->EnableWindow(FALSE);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CLoserWindowDlg::OnBnClickedRadioWin()
{
	((CButton*)GetDlgItem(IDC_RADIO_LOSE))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_WIN))->SetCheck(TRUE);
	GetDlgItem(IDC_EDIT_LOSENUM)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_MINNUM)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_MAXNUM)->EnableWindow(TRUE);
}

/************************************************************************/
/* 保存配置信息                                                         */
/************************************************************************/
int CLoserWindowDlg::SaveParams()
{
	if (!g_ExitFlag)
	{
		ShowMessage("线程运行中,不能更改配置,请先停止线程!");
	}
	CString strAppPath = "";
	CString strTemp = "";
	CUserFile *pFile = CUserFile::GetInstance();
	pFile->GetAppPath(strAppPath);
	GetDlgItemText(IDC_EDIT_ROOMPWD,g_strRoomPwd);
	GetDlgItemText(IDC_EDIT_EXEPATH,g_strAppPath);
	GetDlgItemText(IDC_EDIT_GMSTR,g_strGmStr);
	g_iRoomNum = GetDlgItemInt(IDC_EDIT_DESKNUM);
	g_iSetNum = GetDlgItemInt(IDC_EDIT_SETNUM)-1;
	g_iLoseNum = GetDlgItemInt(IDC_EDIT_LOSENUM);
	g_iMinBalance = GetDlgItemInt(IDC_EDIT_MINNUM);
	g_iMaxBalance = GetDlgItemInt(IDC_EDIT_MAXNUM);
	g_iStopMinutes = GetDlgItemInt(IDC_EDIT_STOPMINUTES);
	if (((CButton*)GetDlgItem(IDC_RADIO_LOSE))->GetCheck()==1)
	{
		g_iWinOrLose = 0;
	}
	else
	{
		g_iWinOrLose = 1;
	}
	g_strAppPath.Trim();
	g_strRoomPwd.Trim();
	g_strGmStr.Trim();
	if (g_strAppPath == "" || g_strRoomPwd == "" || g_strGmStr == "")
	{
		MessageBox("请输入完整的配置信息","错误");
		return -1;
	}
	
	pFile->CheckInitValue(g_iSetNum,0,5);
	pFile->CheckInitValue(g_iRoomNum,1,60);
	pFile->CheckInitValue(g_iLoseNum,1,60000);
	pFile->CheckInitValue(g_iWinOrLose,0,1);
	pFile->CheckInitValue(g_iMinBalance,10,3000);
	pFile->CheckInitValue(g_iMaxBalance,10000,100000000);
	pFile->CheckInitValue(g_iStopMinutes,1,100);
	SetDlgItemText(IDC_EDIT_ROOMPWD,g_strRoomPwd);
	SetDlgItemText(IDC_EDIT_EXEPATH,g_strAppPath);
	SetDlgItemInt(IDC_EDIT_DESKNUM,g_iRoomNum);
	SetDlgItemInt(IDC_EDIT_SETNUM,g_iSetNum+1);
	SetDlgItemInt(IDC_EDIT_LOSENUM,g_iLoseNum);
	SetDlgItemInt(IDC_EDIT_MINNUM,g_iMinBalance);
	SetDlgItemInt(IDC_EDIT_MAXNUM,g_iMaxBalance);
	g_iIsCheckGm = ((CButton *)GetDlgItem(IDC_CHECK_GMCHECK))->GetCheck();
	g_iIsHidePlayWindow = m_HideWindow.GetCheck();
	pFile->CheckInitValue(g_iIsCheckGm,0,1);
	
	g_iCheckGmWait = ((CButton *)GetDlgItem(IDC_CHECK_GMCHECKWAIT))->GetCheck();
	pFile->CheckInitValue(g_iCheckGmWait,0,1);
	g_iCheckGmWaitInterval = GetDlgItemInt(IDC_EDIT_CHECKINTERVAL);
	pFile->CheckInitValue(g_iCheckGmWaitInterval,2,100);

	g_iIsSetPwd = ((CButton *)GetDlgItem(IDC_CHECK_SETPWD))->GetCheck();
	g_iMinMoney = GetDlgItemInt(IDC_EDIT_MINMONEY);
	if (pFile)
	{
		pFile->WriteInitInfo(strAppPath+"set.ini","Fresh","path",g_strAppPath,0,0);
		pFile->WriteInitInfo(strAppPath+"set.ini","Fresh","roompwd",g_strRoomPwd,0,0);		
		pFile->WriteInitInfo(strAppPath+"set.ini","Fresh","type","",g_iWinOrLose,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Fresh","losenum","",g_iLoseNum,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Fresh","desknum","",g_iRoomNum,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Fresh","setnum","",g_iSetNum,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Fresh","min","",g_iMinBalance,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Fresh","max","",g_iMaxBalance,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Fresh","checkgm","",g_iIsCheckGm,1);
		pFile->WriteInitInfo(strAppPath +"set.ini","Fresh","hidewindow","",g_iIsHidePlayWindow,1);
		pFile->WriteInitInfo(strAppPath +"set.ini","Fresh","stopminutes","",g_iStopMinutes,1);
		pFile->WriteInitInfo(strAppPath +"set.ini","Fresh","checkgmwait","",g_iCheckGmWait,1);
		pFile->WriteInitInfo(strAppPath +"set.ini","Fresh","checkinterval","",g_iCheckGmWaitInterval,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Fresh","gmstr",g_strGmStr,0,0);
		pFile->WriteInitInfo(strAppPath +"set.ini","Fresh","setpwd","",g_iIsSetPwd,1);
		pFile->WriteInitInfo(strAppPath +"set.ini","Fresh","minmoney","",g_iMinMoney,1);		
	}
#ifdef ONLY_LOSER
	GetDlgItem(IDC_RADIO_WIN)->ShowWindow(SW_HIDE);
	g_iWinOrLose = 0;
#else
#ifdef ONLY_WINNNER
	GetDlgItem(IDC_RADIO_LOSE)->ShowWindow(SW_HIDE);
	g_iWinOrLose = 1;
#endif
#endif
	if (g_iWinOrLose == 0)
	{
		SetWindowText("输家客户端");
	}
	else
	{
		SetWindowText("赢家客户端");
	}
	return 0;
}
/************************************************************************/
/*  浏览号码文件                                                        */
/************************************************************************/
void CLoserWindowDlg::OnBnClickedBtnFind()
{
	CString strFilePath = "";
	char szFilter[]="Exe Files(*.exe)|*.exe";
	CFileDialog dlg(1,NULL,NULL,NULL,szFilter); 
	if(dlg.DoModal()==IDOK) 
	{
		SetDlgItemText(IDC_EDIT_EXEPATH,dlg.GetPathName());
	}	
}
/************************************************************************/
/* 保存配置信息                                                         */
/************************************************************************/
void CLoserWindowDlg::OnBnClickedBtnSave()
{
	SaveParams();
}
/************************************************************************/
/* 关闭游戏窗口															*/
/************************************************************************/
void CLoserWindowDlg::ClosePlayWindow()
{
	CString strTemp = "";
	strTemp.Format("扎金花游戏客户端 ［ 银子房间1 - 第 %d 桌 ］",g_iRoomNum);
	HWND hwnd = ::FindWindow("#32770",strTemp);
	if (hwnd)
	{
		::PostMessage(hwnd,WM_CLOSE,0,0);
		Sleep(100);
		//此处加上强退的控制信息
	}
}
/************************************************************************/
/* 回退到登录成功状态                                                   */
/************************************************************************/
void CLoserWindowDlg::BackToLoginStatus()
{
	CWindowFind find;
	GetInitBtnInfo(g_hwndTopMost);
	find.PostBtnClick(g_hwndBack);
	Sleep(100);	
}
/************************************************************************/
/* 获取外部进程CListCtrl的指定项的值                                    */
/************************************************************************/
CString CLoserWindowDlg::GetListItemText(HWND hwnd,int iItem,int iSubItem)
{
	LVITEM lvitem, *plvitem;
	char ItemBuf[512],*pItem;
	DWORD PID;
	HANDLE hProcess;
	CString strRet = "";
	GetWindowThreadProcessId(hwnd, &PID);
	hProcess=OpenProcess(PROCESS_ALL_ACCESS,false,PID);
	if (!hProcess)
	{
		return "";
	}	
	plvitem=(LVITEM*)VirtualAllocEx(hProcess, NULL, sizeof(LVITEM), MEM_COMMIT, PAGE_READWRITE);
	pItem=(char*)VirtualAllocEx(hProcess, NULL, 512, MEM_COMMIT, PAGE_READWRITE);
	if ((!plvitem)||(!pItem))
	{
		CloseHandle(hProcess);
		return "";
	}
	else    //获取房间名
	{
		lvitem.cchTextMax=512;
		lvitem.iSubItem=iSubItem;
		lvitem.pszText=pItem;
		WriteProcessMemory(hProcess, plvitem, &lvitem, sizeof(LVITEM), NULL);
		::SendMessage(hwnd,LVM_GETITEMTEXT, (WPARAM)iItem,(LPARAM)plvitem);
		ReadProcessMemory(hProcess, pItem, ItemBuf, 512, NULL);
		strRet.Format("%s",ItemBuf);
	}			
	VirtualFreeEx(hProcess, plvitem, 0, MEM_RELEASE);
	VirtualFreeEx(hProcess, pItem, 0, MEM_RELEASE);
	CloseHandle(hProcess);
	return strRet;
}
/************************************************************************/
/* 查询账号余额                                                         */
/************************************************************************/
int CLoserWindowDlg::GetBalance(HWND hwnd)
{
	CString strRet = GetListItemText(hwnd,0,4);
	return atoi(strRet);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CLoserWindowDlg::GetPreUsers(HWND hwnd,int iCount)
{
	CString strRet = "";
	for (int i=0;i<iCount;i++)
	{
		strRet += GetListItemText(hwnd,0,3)+",";
	}
	return strRet;
}
/************************************************************************/
/*检测GM是否来了                                                        */
/************************************************************************/
bool CLoserWindowDlg::IsGmHere(HWND hwnd)
{
	if (g_iIsCheckGm == 0)
	{
		return false;
	}
	CString strRet = GetListItemText(hwnd,1,3);
	if (strRet.Find(g_strGmStr) != -1)
	{
		return true;
	}
	return false;
}
/************************************************************************/
/*获取进入指定游戏后的初始化信息                                        */
/************************************************************************/
int CLoserWindowDlg::GetInitBtnInfo(HWND hwndTopParant)
{
	CWindowFind find;
	HWND hwndChild = NULL;
	g_hwndSet = NULL;
	g_hwndBack = NULL;
	g_hwndListView1 = NULL;
	g_hwndListView2 = NULL;
	g_hwndDesk = NULL;
	hwndChild = find.GetChildWindow(hwndTopParant,8);
	HWND hwndClildEx = find.GetChildWindow(hwndTopParant,8);
	if (!hwndChild)
	{
		return -1;
	}
	g_hwndSet = find.GetChildWindow(hwndChild,8);
	g_hwndBack = find.GetChildWindow(hwndChild,7);
	hwndChild = find.GetChildWindow(hwndChild,17);
	HWND hwndOld = hwndChild;
	hwndChild = find.GetChildWindow(hwndChild,1);	
	g_hwndListView1 = find.GetChildWindow(hwndChild,1);

	hwndChild = find.GetChildWindow(hwndClildEx,21);
	hwndChild = find.GetChildWindow(hwndChild,1);	
	g_hwndDesk = find.GetChildWindow(hwndChild,1);
	if (!g_hwndBack || !g_hwndSet || !g_hwndListView1 || !g_hwndDesk)
	{
		g_hwndSet = NULL;
		g_hwndBack = NULL;
		g_hwndListView1 = NULL;
		g_hwndListView2 = NULL;
		g_hwndDesk = NULL;
		return -1;
	}
	if (find.GetContrlClassName(g_hwndBack) != "Button" ||
		find.GetContrlClassName(g_hwndSet) != "Button" ||
		find.GetContrlClassName(g_hwndListView1) != "SysListView32" ||
		find.GetContrlClassName(g_hwndDesk) != "AfxWnd70")
	{
		g_hwndSet = NULL;
		g_hwndBack = NULL;
		g_hwndListView1 = NULL;
		g_hwndListView2 = NULL;
		g_hwndDesk = NULL;
		return -1;
	}
	return 0;
}
void CLoserWindowDlg::OnBnClickedButton8()
{
	GetPower();
}

void CLoserWindowDlg::OnBnClickedButton9()
{
	BackToLoginStatus();
}
/************************************************************************/
/*关闭热键                                                              */
/************************************************************************/
void CLoserWindowDlg::UnloadHook()
{
	if(m_KeyHook.hKeyBoardHook  != NULL)
		UnhookWindowsHookEx(m_KeyHook.hKeyBoardHook);
	if(m_KeyHook.hLowKeyBoardHook != NULL)
		UnhookWindowsHookEx(m_KeyHook.hLowKeyBoardHook);
}
/************************************************************************/
/*程序退出                                                              */
/************************************************************************/
void CLoserWindowDlg::OnClose()
{
	UnloadHook();
	if (g_ExitFlag == 0)
	{
		g_ExitFlag = 1;
		Sleep(500);
	}
	CDialog::OnClose();
}
/************************************************************************/
/*设置热键                                                              */
/************************************************************************/
void CLoserWindowDlg::SetHook()
{
	m_KeyHook = SetHookEx(m_hWnd);
}

/************************************************************************/
/* 远程注入方式双击树结点                                               */
/************************************************************************/
int CLoserWindowDlg::DblClickItemNodeEx(HWND hwnd)
{
	CString strText = "";
	DWORD PID;
	HANDLE hProcess;
	HWND hParant = ::GetParent(hwnd);
	DWORD dwProcSize = 4096;
	GetWindowThreadProcessId(hParant, &PID);
	hProcess=OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	if (!hProcess)
	{
		ShowMessage("打开远程进程空间失败");
		return -1;
	}
	MyData data;
	ZeroMemory(&data, sizeof (MyData));
	HINSTANCE hUser = LoadLibrary("user32.dll");
	if (! hUser)
	{
		ShowMessage("加载user32.dll失败");
		return -1;
	}
	data.dwSendMsg = (DWORD)GetProcAddress(hUser, "SendMessageA");
	FreeLibrary(hUser);
	if (!data.dwSendMsg)
	{
		ShowMessage("获取SendMessage函数地址失败");
		return -1;
	}
	//构造WM_NOTIFY消息结构体
	ZeroMemory(&data.nmh, sizeof (NMHDR));
	data.nmh.code = NM_DBLCLK;
	data.nmh.hwndFrom = hwnd;
	data.nmh.idFrom = ::GetDlgCtrlID(hwnd);
	data.hwndParant = ::GetParent(hwnd);

	//分配空间
	void *pRemoteThread	= VirtualAllocEx(hProcess, 0,dwProcSize, MEM_COMMIT|MEM_RESERVE,PAGE_EXECUTE_READWRITE);
	if (!pRemoteThread)
	{
		ShowMessage("开辟远程函数地址空间失败");
		CloseHandle(hProcess);
		return -1;
	}
	if (!WriteProcessMemory(hProcess, pRemoteThread, &RemoteFunc,dwProcSize,0))
	{
		ShowMessage("写入远程函数地址失败");
		VirtualFreeEx(hProcess, pRemoteThread, dwProcSize, MEM_RELEASE);
		CloseHandle(hProcess);
		return -1;
	}
	MyData *pData = (MyData*)VirtualAllocEx(hProcess, 0,sizeof (MyData), MEM_COMMIT,PAGE_READWRITE);
	if (!pData)
	{
		VirtualFreeEx(hProcess, pRemoteThread, dwProcSize, MEM_RELEASE);
		CloseHandle(hProcess);
		ShowMessage("开辟远程参数空间失败");
		return -1;
	}
	if (! WriteProcessMemory(hProcess, pData, &data, sizeof (MyData), 0))
	{
		VirtualFreeEx(hProcess, pRemoteThread, dwProcSize, MEM_RELEASE);
		VirtualFreeEx(hProcess, pData, sizeof (MyData), MEM_RELEASE);
		CloseHandle(hProcess);
		ShowMessage("注入参数信息失败");
		return -1;
	}
	//创建远程线程
	HANDLE hThread = CreateRemoteThread(hProcess,0,0,(LPTHREAD_START_ROUTINE)pRemoteThread,(LPVOID)pData,0,0);
	if (!hThread)
	{
		VirtualFreeEx(hProcess, pRemoteThread, dwProcSize, MEM_RELEASE);
		VirtualFreeEx(hProcess, pData, sizeof (MyData), MEM_RELEASE);
		CloseHandle(hProcess);
		ShowMessage("创建远程线程失败");
		return -1;
	}
	WaitForSingleObject(hThread,1000);
	CloseHandle(hThread);
	VirtualFreeEx(hProcess, pRemoteThread, dwProcSize, MEM_RELEASE);
	VirtualFreeEx(hProcess, pData, sizeof (MyData), MEM_RELEASE);
	CloseHandle(hProcess);
	return 0;
}

/************************************************************************/
/*遍历树,查找第一个名称为给定值的节点,返回                              */
/************************************************************************/
HTREEITEM CLoserWindowDlg::FindItem(HWND hwnd,HTREEITEM hItem,CString strText)   
{   
	HTREEITEM hFind;
	if(hItem == NULL)   
	{
		return NULL;
	}
	CString strItemText = GetItemNodeText(hwnd,hItem);
	ShowMessage(strItemText);
	if(strItemText.Left(strText.GetLength()) == strText)//找到
	{		
		return hItem;
	}
	HTREEITEM hChild = TreeView_GetChild(hwnd,hItem);
	if(hChild)
	{
		hItem = hChild;   
		hFind = FindItem(hwnd,hItem,strText);
		if(hFind)   
		{   
			return hFind;
		}
		else   
		{			
			hItem= TreeView_GetNextItem(hwnd,TreeView_GetParent(hwnd,hItem),TVGN_NEXT);
			if (hItem)
			{
				return FindItem(hwnd,hItem,strText);
			}
		}
	}   
	else   
	{
		hItem= TreeView_GetNextItem(hwnd,hItem,TVGN_NEXT);
		if (!hItem)
		{
			return NULL;
		}
		hFind = FindItem(hwnd,hItem,strText);
		return hFind;		
	}
	return hFind;
}
/************************************************************************/
/* 速度调节                                                             */
/************************************************************************/
void CLoserWindowDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSliderCtrl* pSc2=(CSliderCtrl*)pScrollBar;
	g_iSpeed = 100 - pSc2->GetPos();
	if (g_iSpeed < 1)
	{
		g_iSpeed = 1;
	}
	else if (g_iSpeed >100)
	{
		g_iSpeed = 100;
	}
	CString strAppPath = "";
	CString strTemp = "";
	CUserFile *pFile = CUserFile::GetInstance();
	pFile->GetAppPath(strAppPath);
	pFile->WriteInitInfo(strAppPath +"set.ini","Fresh","speed","",g_iSpeed,1);
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CLoserWindowDlg::OnBnClickedCheckGmcheck()
{
	g_iIsCheckGm = m_CheckGM.GetCheck();
	CString strAppPath = "";
	CString strTemp = "";
	CUserFile *pFile = CUserFile::GetInstance();
	pFile->GetAppPath(strAppPath);
	pFile->WriteInitInfo(strAppPath +"set.ini","Fresh","checkgm","",g_iIsCheckGm,1);
	if (g_iIsCheckGm)
	{
		GetDlgItem(IDC_EDIT_STOPMINUTES)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_GMCHECKWAIT)->EnableWindow(TRUE);
		OnBnClickedCheckGmcheckwait();
	}
	else
	{
		GetDlgItem(IDC_EDIT_CHECKINTERVAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_GMCHECKWAIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_STOPMINUTES)->EnableWindow(FALSE);
	}
	GetDlgItem(IDC_EDIT_GMSTR)->EnableWindow(g_iIsCheckGm);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CLoserWindowDlg::OnBnClickedCheckHidewindow()
{
	g_iIsHidePlayWindow = m_HideWindow.GetCheck();
	CString strAppPath = "";
	CString strTemp = "";
	CUserFile *pFile = CUserFile::GetInstance();
	pFile->GetAppPath(strAppPath);
	pFile->WriteInitInfo(strAppPath +"set.ini","Fresh","hidewindow","",g_iIsHidePlayWindow,1);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CLoserWindowDlg::AddFreshResult(RegisterPack rgPack,int iFlag)
{
	CListCtrl *pList = &m_ListAcc;
	int nCount = pList->GetItemCount();
	for (int i=0;i<nCount;i++)
	{
		if (rgPack.strAcc == pList->GetItemText(i,1))
		{
			pList->SetItemText(i,3,g_strBalance);
			pList->SetItemText(i,4,iFlag==0?"成功":"失败");
			break;
		}
	}
	if (iFlag == 0)		//写入刷分完成的号码文件
	{
		CString strShow = "";
		strShow.Format("刷分完成: %s,%s",rgPack.strAcc,g_strBalance);
		WriteLog(strShow,1);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CLoserWindowDlg::ShutUpWindow()
{
	CWindowFind find;
	Sleep(150);
	HWND hwnd = ::FindWindow("#32770","提示信息");
	HWND hwndChild = find.GetChildWindow(hwnd,1);
	int iCount = 1;
	while (hwnd && hwndChild)
	{
		hwndChild = find.GetChildWindow(hwnd,iCount);
		CString strWinName = find.GetContrlWindowText(hwndChild);
		if ((strWinName.Find("确定") != -1 || strWinName.Find("是") != -1) && 
			find.GetContrlClassName(hwndChild) =="Button")
		{
			ShowMessage("强制退出游戏!");
			find.PostBtnClick(hwndChild);
		}
		iCount ++;
		if (iCount > 25)
		{
			break;
		}
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CLoserWindowDlg::GetPower()
{
	CInternetSession session;
	CString strTemp = "";
	try
	{
		CStdioFile *pFile = session.OpenURL("http://blog.163.com/shaojun_hnist/blog/static/1387171242011115387766/");
		if (pFile != NULL)
		{
			while(pFile->ReadString(strTemp))
			{
				strTemp.MakeLower();
				if (strTemp.Find("myownpower7pmi") !=-1)
				{
					pFile->Close();
					delete pFile;
					pFile = NULL;
					return 0;
				}
			}
			
			if (pFile == NULL)
			{
				pFile->Close();
				delete pFile;
				pFile = NULL;
			}
		}
	}
	catch (...)
	{
	}
	return -1;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CLoserWindowDlg::SetLastWaitCheckTime()
{
	m_iCurrTime = ::GetTickCount();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
bool CLoserWindowDlg::IsOverCheckTime(int iIntervalSeconds)
{
	if (g_iIsCheckGm == 0)
	{
		return false;
	}
	if (g_iCheckGmWait == 0)
	{
		return false;
	}
	DWORD t2 = 0;
	t2 = ::GetTickCount();
	DWORD tSpan = 0;
	tSpan = t2 - m_iCurrTime;
	if (tSpan > iIntervalSeconds * 1000) //iDelay 秒
	{
		return true;
	}
	else
	{
		return false;
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CLoserWindowDlg::OnBnClickedCheckGmcheckwait()
{
	g_iCheckGmWait = m_gmCheckWait.GetCheck();
	CString strAppPath = "";
	CString strTemp = "";
	CUserFile *pFile = CUserFile::GetInstance();
	pFile->GetAppPath(strAppPath);
	pFile->WriteInitInfo(strAppPath +"set.ini","Fresh","checkgmwait","",g_iCheckGmWait,1);
	if (g_iIsCheckGm)
	{
		GetDlgItem(IDC_EDIT_CHECKINTERVAL)->EnableWindow(g_iCheckGmWait);
	}
	else
	{
		GetDlgItem(IDC_EDIT_CHECKINTERVAL)->EnableWindow(FALSE);
	}
}
/************************************************************************/
/*  获取外部进程树的结点文字信息                                        */
/************************************************************************/
void CLoserWindowDlg::SetRemoteWindowText(HWND hwnd, CString strText)
{
	DWORD PID;
	HANDLE hProcess;
	DWORD dwProcSize = 4096;
	GetWindowThreadProcessId(hwnd, &PID);
	hProcess=OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	if (!hProcess)
	{
		ShowMessage("打开远程进程空间失败");
		return ;
	}
	char data[512] = {0};
	strcpy(data,strText);

	char *pData = (char*)VirtualAllocEx(hProcess,0,sizeof (char), MEM_COMMIT,PAGE_READWRITE);
	if (!pData)
	{
		VirtualFreeEx(hProcess, pData, dwProcSize, MEM_RELEASE);
		CloseHandle(hProcess);
		ShowMessage("开辟远程参数空间失败");
		return ;
	}
	if (! WriteProcessMemory(hProcess, pData, &data, sizeof (char), 0))
	{
		VirtualFreeEx(hProcess, pData, sizeof (char), MEM_RELEASE);
		CloseHandle(hProcess);
		ShowMessage("注入参数信息失败");
		return ;
	}
	::SetWindowText(hwnd,data);
	VirtualFreeEx(hProcess, pData, sizeof (pData), MEM_RELEASE);
	CloseHandle(hProcess);
	return ;
}