// VSHackDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VSHack.h"
#include "VSHackDlg.h"
#include ".\vshackdlg.h"
#include <io.h> 
#include <vector>
#include "WindowFind.h"
#include "WebBase.h"
#pragma comment(lib, "VERSION.LIB")
using std::vector;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_NEWFILEVERSION WM_USER + 1212
#define BAT_FILE_NAME "AutoUpdate.bat"
#define BAT_FILE_NAMEEX "del.bat"
#define UPDATE_FILE_NAME "RoomHelperNew.exe"
HWND gl_MainHwnd = NULL;
DWORD gl_ProcessID = 0;
vector<HWND> gl_hwndList;
CWindowFind gl_WndFind;
CString gl_strFileDownUrl = "";
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
CString gl_strFilePath = "";
CString gl_strExeFilePath = "";
CString gl_strExeName = "";
CString gl_strBatPath = "";
CString gl_strBatPathEx = "";
CString GetAppPath()
{
	char szName[255] = {0};
	GetModuleFileName(NULL,szName,255);
	gl_strExeFilePath = CString(szName);
	char *p = strrchr(szName,'\\');
	*p = '\0';
	gl_strExeName = CString(p+1);
	return CString(szName) + "\\";
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


// CVSHackDlg 对话框



CVSHackDlg::CVSHackDlg(CWnd* pParent /*=NULL*/)
: CDialog(CVSHackDlg::IDD, pParent)
{
		m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
		m_dwProcessID = 0;
}

void CVSHackDlg::DoDataExchange(CDataExchange* pDX)
{
		CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVSHackDlg, CDialog)
		ON_WM_SYSCOMMAND()
		ON_WM_PAINT()
		ON_WM_QUERYDRAGICON()
		//}}AFX_MSG_MAP

		ON_BN_CLICKED(IDC_BTN_START, OnBnClickedBtnStart)
		ON_BN_CLICKED(IDC_BTN_STOP, OnBnClickedBtnStop)
		ON_WM_TIMER()
		ON_WM_CLOSE()
		ON_BN_CLICKED(IDC_BUTTON1, &CVSHackDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CVSHackDlg 消息处理程序

BOOL CVSHackDlg::PreTranslateMessage( MSG* pMsg)
{
		if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		{
				return TRUE;      //屏蔽掉Esc键
		}
		if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		{
				return TRUE;      //屏蔽掉Enter键
		}

		return CDialog::PreTranslateMessage( pMsg );
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CVSHackDlg::OnInitDialog()
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
		CString strShow = "";
		strShow.Format("说明\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s",
						"1. 使用时您只需要启动VS游戏竞技平台，再打开挤房间工具，点击开始，在您想要挤的房间上点一下即可。",
						"2. 本程序使用简单，挤房间时您可以进行其他任何操作，让您挤房娱乐两不误。如果想换房间挤，只需要在挤房间隔中点击您想要挤的房间即可更换,不用中断挤房工具。",
						"3. 本程序为绿色挤房，不能进入人满房间，更不能进入高于您等级的房间，但是它能循环挤房，保证您在最短的时间内进入您想进的房间。",
						"4. 现在的版本没有绕开时间的限制,如果需要快速挤房，可以使用加速器[使用后不保证账号安全]。",
						"5. 也由于是绿色挤房，所以您不用担心被VS封号，我也是一名VS玩家，自己用了一年多，账号安然无恙。",
						"备注: 如果您对本软件有任何意见或者建议,可以发送邮件到shaojun.hnist@gmail.com[对此软件进行改造的朋友,请一定Email联系]，由于时间关系，如无需要，将不再对此软件进行升级。\r\n");
		SetDlgItemText(IDC_EDIT_NOTICE,strShow);
		gl_strFilePath = GetAppPath();

		//建立线程检查是否有新版本
		HANDLE hThread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)CheckNewVersionProc,
			(LPVOID)this,0,NULL);
		CloseHandle(hThread);

		
		
		//GenBatFile();
		
		//ShellExecute(NULL,"open",gl_strBatPath,NULL,NULL,0);
		//exit(0);
	//	GenBatFileEx();
		//ShellExecute(NULL,"open",gl_strBatPathEx,NULL,NULL,0);
		// TODO: 在此添加额外的初始化代码
		oldItem = -2;
		GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
		return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CVSHackDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CVSHackDlg::OnPaint() 
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
HCURSOR CVSHackDlg::OnQueryDragIcon()
{
		return static_cast<HCURSOR>(m_hIcon);
}


void CVSHackDlg::OnBnClickedBtnStart()
{
		hitFlag = true;
		dblhitFlag = true;
		SetDlgItemText(IDC_EDIT_ROOM,"查找房间列表开始,请将鼠标移动至房间列表上");
		::SetTimer(m_hWnd,1,500,NULL);  //开始查找房间列表
		GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
}

void CVSHackDlg::OnBnClickedBtnStop()
{
		// TODO: 在此添加控件通知处理程序代码
		::KillTimer(m_hWnd,1);
		hitFlag = false;
		::KillTimer(m_hWnd,2);
		::KillTimer(m_hWnd,3);
		GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
}


void CVSHackDlg::OnTimer(UINT nIDEvent)
{
		if (nIDEvent == 1)
		{
				POINT pnt;
				::GetCursorPos(&pnt);                //取得鼠标坐标
				HWND tempHWND = ::WindowFromPoint(pnt) ;    //取得鼠标指针处窗口句柄
				TCHAR   szClassName[256];     
				TCHAR   szWindowText[256];   
				GetClassName(tempHWND,   szClassName,256);     
				::GetWindowText(tempHWND,(LPTSTR)&szWindowText,256);
				//HWND hwndParant = ::GetParent(tempHWND);
				if ("SysListView32" == CString(szClassName) && "List1" == CString(szWindowText))
				{//找到房间列表
						hwnd = tempHWND;
						::SetTimer(m_hWnd,3,80,NULL); //开始执行关闭警告框循环
						::SetTimer(m_hWnd,2,400,NULL);//开始执行挤房循环
						::KillTimer(m_hWnd,1);
						SetDlgItemText(IDC_EDIT_ROOM,"找到房间列表,请点击您要挤的房间");
						m_hMainHwnd = GetProcessMainWnd(hwnd);
						gl_MainHwnd = m_hMainHwnd;
				}
		}
		if (nIDEvent == 2)
		{
				::KillTimer(m_hWnd,2);
				if (hitFlag)
				{
						//建立挤房线程
						HANDLE hThread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)TryIntoRoomProc,
								(LPVOID)this,0,NULL);
						CloseHandle(hThread);
				}		
		}

		if (nIDEvent == 3)
		{
				//建立关闭警告框线程
				HANDLE hThread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)CloseWarnProc,
						(LPVOID)this,0,NULL);
				CloseHandle(hThread);
		}
		CDialog::OnTimer(nIDEvent);
}

DWORD   WINAPI   CVSHackDlg::CheckNewVersionProc(LPVOID lpParameter)
{   
	((CVSHackDlg *)lpParameter )->CheckNewVersion();   
	return 0;
}
void CVSHackDlg::CheckNewVersion()
{
		CString strVersion = GetFileVersion(gl_strExeFilePath);
		//检查是否有新版本,有则提示完事儿
		CString strFormat = "/TragicGuy/archive/2011/01/15/6142456.aspx";

		bool bNeedNotice = false;
		CWebBase objWeb(50);
		string strServer = "blog.csdn.net";
		string strHeader = "";
		strHeader.append("\r\nUser-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.2.13) Gecko/20101203 Firefox/3.6.13");
		strHeader.append("\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
		strHeader.append("\r\nKeep-Alive: 115");
		//Accept-Charset:GB2312,utf-8;q=0.7,*;q=0.7

		strHeader.append("\r\nConnection: Keep-Alive");
		int iRet = objWeb.GetPage(strFormat,strHeader.c_str(),strServer.c_str());
		CString strRet = CString(objWeb.GetContentInfo());
		//ConvertUtf8ToGBK(strRet);
	//	AfxMessageBox(strRet);
		CString strBefore = "Latest Version:";
		CString strAfter = ";";
		CString strLatestVesion = GetKeyString(strRet,strBefore,strAfter);
		strLatestVesion.Trim();
		strVersion.Trim();
		if (strLatestVesion != "" && strVersion != "" && strLatestVesion != strVersion)
		{
			//有新版本
			strBefore = "Down Url:";
			CString strDownUrl = GetKeyString(strRet,strBefore,strAfter);
			strDownUrl.Trim();
			if(strDownUrl != "")
			{
				gl_strFileDownUrl = strDownUrl;
				PostMessage(WM_NEWFILEVERSION,0,0);				
			}
		}
}
//挤房线程函数
DWORD   WINAPI   CVSHackDlg::TryIntoRoomProc(LPVOID lpParameter)
{   
		((CVSHackDlg *)lpParameter )->TryIntoRoom();   
		return 0;
}

LRESULT CVSHackDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
	if (message == WM_NEWFILEVERSION)
	{
		if (::MessageBox(m_hWnd,"检测到有新版本的挤房器,是否下载更新版本?","提示",MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			::ShellExecute(NULL, _T("open"), "IEXPLORE", gl_strFileDownUrl, NULL, SW_SHOWMAXIMIZED);
		}
		return 0;
	}
	return CDialog::DefWindowProc(message, wParam, lParam);
}
//关闭警告框线程函数
DWORD WINAPI CVSHackDlg::CloseWarnProc(LPVOID lpParameter)
{
	Sleep(1000);
		//((CVSHackDlg *)lpParameter )->CloseWarnWindow();
		return 0;
}
//关闭警告框
void CVSHackDlg::CloseWarnWindow()
{
	MyFindWindow("","",NULL);
	int iSize = gl_hwndList.size();
	vector<HWND>::iterator it;
	for (it = gl_hwndList.begin();it != gl_hwndList.end(); it++)
	{
		HWND hwndTemp = (HWND)*it;
		HWND hwndChild = gl_WndFind.GetChildWindow(hwndTemp,1);
		if (hwndChild)
		{
			CString strWindowName = gl_WndFind.GetContrlWindowText(hwndChild);
			if(strWindowName.Find("休息一下,请稍后再试")>=0 || strWindowName.Find("当前网络连接不可用")>=0)
			{
				hwndChild = gl_WndFind.GetChildWindow(hwndTemp,3);
				if(hwndChild)
				{
					gl_WndFind.PostBtnClick(hwndChild);
				}
			}
		}
		::PostMessage(hwndTemp,WM_CLOSE,0,0);
	}

	dblhitFlag = true;
	Sleep(200);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//挤房并获取房间名
void CVSHackDlg::TryIntoRoom()
{
		CWnd *pHack = CWnd::FromHandle(hwnd);
		CListCtrl *listHack = (CListCtrl *)pHack;	

		POSITION pos = listHack->GetFirstSelectedItemPosition();
		int iItem = listHack->GetNextSelectedItem(pos); //房间索引号
		if(iItem == -1)
		{			
			POINT Pos;
			::GetCursorPos(&Pos);
			int wX = Pos.x;
			int wY = Pos.y;
			::ClientToScreen(hwnd, &Pos);
			::SetCursorPos(Pos.x, Pos.y);
			::PostMessage(hwnd, WM_LBUTTONDOWN,   MK_LBUTTON, ((DWORD)((WORD)(wX)) | (((DWORD)(WORD)(wY))<<16)));
			::PostMessage(hwnd, WM_LBUTTONUP,     MK_LBUTTON, ((DWORD)((WORD)(wX)) | (((DWORD)(WORD)(wY))<<16)));	
			return;
		}
		if (iItem != oldItem)  //如果房间索引号变了，则重新获取新的房间名，并重新获取新的房间项区域
		{
				oldItem = iItem;   //重置原房间索引
				RECT *lpRect;
				LVITEM lvitem, *plvitem;
				char ItemBuf[512],*pItem;
				DWORD PID;
				HANDLE hProcess;

				GetWindowThreadProcessId(hwnd, &PID);
				m_dwProcessID = PID;
				gl_ProcessID = m_dwProcessID;
				hProcess=OpenProcess(PROCESS_ALL_ACCESS,false,PID);
				if (!hProcess)
				{
						//获取进程句柄操作失败
				}
				else
				{
						plvitem=(LVITEM*)VirtualAllocEx(hProcess, NULL, sizeof(LVITEM), MEM_COMMIT, PAGE_READWRITE);
						pItem=(char*)VirtualAllocEx(hProcess, NULL, 512, MEM_COMMIT, PAGE_READWRITE);

						if ((!plvitem)||(!pItem))
						{
								//无法分配内存
						}
						else    //获取房间名
						{
								lvitem.cchTextMax=512;
								lvitem.iSubItem=0; //ProcessName
								lvitem.pszText=pItem;
								WriteProcessMemory(hProcess, plvitem, &lvitem, sizeof(LVITEM), NULL);
								::SendMessage(hwnd,LVM_GETITEMTEXT, (WPARAM)iItem,(LPARAM)plvitem);
								ReadProcessMemory(hProcess, pItem, ItemBuf, 512, NULL);
						}
						//CloseHandle(hProcess);
						VirtualFreeEx(hProcess, plvitem, 0, MEM_RELEASE);
						VirtualFreeEx(hProcess, pItem, 0, MEM_RELEASE);
						
						CPoint pt;
						GetCursorPos(&pt);
						Rect.top = pt.y;
						Rect.left = pt.x;
						DWORD dRet = 0;
						lpRect  =  (RECT*)VirtualAllocEx(hProcess,  NULL,  sizeof(RECT),  MEM_COMMIT,  PAGE_READWRITE);
						if (lpRect == 0)
						{
							dRet = GetLastError();
						}
						if (!lpRect)
						{
								//无法分配内存
						}
						else
						{
								//获取房间项区域
								ZeroMemory(&Rect,sizeof(Rect));
								WriteProcessMemory(hProcess,  lpRect,  &Rect,  sizeof(RECT),  NULL);
								::SendMessage(hwnd,  LVM_GETITEMRECT,  (WPARAM)iItem,  (LPARAM)lpRect);
								ReadProcessMemory(hProcess,  lpRect,  &Rect,  sizeof(RECT),  NULL);
								VirtualFreeEx(hProcess, lpRect, 0, MEM_RELEASE);
						}
						CloseHandle(hProcess);
				}
				SetDlgItemText(IDC_EDIT_ROOM,ItemBuf);//将房间名赋值到文本框
		}

		DblHitItem(); //双击，挤房
		if (hitFlag)
		{
				::SetTimer(m_hWnd,2,400,NULL);//重新开启计时器，循环挤房
		}
}


//发送双击消息
void CVSHackDlg::DblHitItem()
{
		CWnd *pWnd1 = NULL;
		CWnd *pWnd2 = NULL;
		pWnd2 = FindWindow(_T("#32770"), _T("VSClient"));
		pWnd1 = FindWindow(_T("#32770"), _T("VS竞技游戏平台"));
		//dblhitFlag = true;
		CloseWarnWindow();
		//Sleep(500);
		
		if(!pWnd1 && !pWnd2 && dblhitFlag && ::IsWindowVisible(hwnd))  //如果没有警告框且挤房标志为真
		{
				CPoint pt;
				GetCursorPos(&pt);
				dblhitFlag = false;
				int wX = Rect.left;
				int wY = Rect.top;
				POINT Pos;
				Pos.x = wX;
				Pos.y = wY;
				::ClientToScreen(hwnd, &Pos);
				::SetCursorPos(Pos.x+2, Pos.y+2);
				::PostMessage(hwnd, WM_LBUTTONDOWN,   MK_LBUTTON, ((DWORD)((WORD)(wX)) | (((DWORD)(WORD)(wY))<<16)));
				::PostMessage(hwnd, WM_LBUTTONUP,     MK_LBUTTON, ((DWORD)((WORD)(wX)) | (((DWORD)(WORD)(wY))<<16)));
				::PostMessage(hwnd, WM_LBUTTONDBLCLK, MK_LBUTTON, ((DWORD)((WORD)(wX)) | (((DWORD)(WORD)(wY))<<16)));
				Sleep(20);
				::SetCursorPos(pt.x, pt.y);
		}

}

void CVSHackDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnClose();
}

void CVSHackDlg::OnBnClickedButton1()
{
	::PostMessage((HWND)2950430,WM_CLOSE,0,0);
}

HWND CVSHackDlg::GetProcessMainWnd(HWND hwndList)
//将DLL窗口的句柄传入，以便识别找到的句柄不是DLL窗口的句柄
{
	while(::GetParent(hwndList) != NULL )
	{
		hwndList = ::GetParent( hwndList );
	}
	return hwndList;
} 


BOOL CALLBACK EnumChildProcExVS(HWND  hwndTemp, LPARAM lParam)
{   	
	if (::GetParent(hwndTemp) == gl_MainHwnd )
	{
		//DWORD PID;
		//GetWindowThreadProcessId(hwndTemp, &PID);
	//	if (PID == gl_ProcessID)
	//	{
			char strClassName[255];
			//char strWindowName[255];
			memset(strClassName,0,sizeof(strClassName));   
			//memset(strWindowName,0,sizeof(strWindowName));  
			GetClassName(hwndTemp,strClassName,sizeof(strClassName));
			if (CString(strClassName) == "#32770")
			{
				gl_hwndList.insert(gl_hwndList.end(),hwndTemp);
			}
	//	}
		
	}
	EnumChildWindows(hwndTemp,EnumChildProcExVS,lParam);
	
	return   TRUE;   
}

void CVSHackDlg::MyFindWindow(CString strClassName,CString strWindowName,HWND hParant)
{
	gl_hwndList.clear();
	EnumChildWindows(NULL,EnumChildProcExVS,NULL);
}

void CVSHackDlg::GenBatFile()
{
	//AutoUpdate
	CString strBatContent = "";
	CString strBatPath = "";
	strBatPath.Format("%s%s",gl_strFilePath,BAT_FILE_NAME);
	gl_strBatPath = strBatPath;

	strBatContent.Format("del %s\r\n@Copy \"%s%s\" \"%s\"\r\n%s\r\ndel %s%s\r\ndel %s",gl_strExeFilePath,gl_strFilePath,UPDATE_FILE_NAME,gl_strExeFilePath,gl_strExeFilePath,gl_strFilePath,UPDATE_FILE_NAME,strBatPath);
	CFile file;
	
	file.Open(strBatPath,CFile::modeCreate | CFile::modeWrite);
	file.Write(strBatContent,strBatContent.GetLength());
	file.Close();
}

void CVSHackDlg::GenBatFileEx()
{
	//AutoUpdate
	CString strBatContent = "";
	CString strBatPath = "";
	strBatPath.Format("%s%s",gl_strFilePath,BAT_FILE_NAMEEX);
	gl_strBatPathEx = strBatPath;

	strBatContent.Format("del %s%s\r\ndel %s\r\ndel %s",gl_strFilePath,UPDATE_FILE_NAME,gl_strBatPath,gl_strBatPathEx);
	CFile file;

	file.Open(strBatPath,CFile::modeCreate | CFile::modeWrite);
	file.Write(strBatContent,strBatContent.GetLength());
	file.Close();
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CVSHackDlg::GetFileVersion(CString strFileName)   
{      
	int iVerInfoSize;   
	char *pBuf;   
	CString asVer="";   
	VS_FIXEDFILEINFO *pVsInfo;   
	unsigned int iFileInfoSize = sizeof(VS_FIXEDFILEINFO);
	iVerInfoSize   =   GetFileVersionInfoSize(strFileName,NULL);
	if(iVerInfoSize!= 0)   
	{      
		pBuf = new char[iVerInfoSize];   
		if(GetFileVersionInfo(strFileName,0,iVerInfoSize,pBuf))
		{      
			if(VerQueryValue(pBuf,"\\",(void**)&pVsInfo,&iFileInfoSize))      
			{      
				asVer.Format("%d.%d.%d.%d",HIWORD(pVsInfo->dwFileVersionMS),LOWORD(pVsInfo->dwFileVersionMS),HIWORD(pVsInfo->dwFileVersionLS),LOWORD(pVsInfo->dwFileVersionLS));   
			}      
		}      
		delete[] pBuf;
		pBuf = NULL;
	}
	return "v"+asVer;
}

CString CVSHackDlg::GetKeyString(CString &srcStr,CString &strBefore,CString &strAfter)
{
	int iPosBefore = 0;
	int iPosAfter = 0;
	iPosBefore = srcStr.Find(strBefore) + strBefore.GetLength();
	CString strTest = srcStr.Left(iPosBefore);
	iPosAfter = srcStr.Find(strAfter,iPosBefore);
	if (iPosBefore < 0 || iPosAfter < 0 || iPosAfter - iPosBefore < 0 )
	{
		return "";
	}
	else
	{
		return srcStr.Mid(iPosBefore,iPosAfter - iPosBefore);
	}
}

void CVSHackDlg::ConvertUtf8ToGBK(CString& strUtf8) 
{
	int len = 0;
	unsigned short * wszGBK = NULL;
	char * szGBKt = NULL;
	char * szGBKs = NULL;

	len = MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, NULL,0);
	wszGBK = new unsigned short[len+1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, (LPWSTR)wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL); 
	szGBKt  =new char[len + 1];
	memset(szGBKt, 0, len + 1);
	WideCharToMultiByte (CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBKt, len, NULL,NULL);

	szGBKs  =new char[len + 1];
	memset(szGBKs, 0, len + 1);
	//转换Gb码繁体到Gb码简体，使用API函数LCMapString 
	LCMapString( 0x0804, LCMAP_SIMPLIFIED_CHINESE, szGBKt, -1, szGBKs, len);
	strUtf8 = szGBKs;
	delete[] szGBKs;
	delete[] szGBKt;
	delete[] wszGBK;
}