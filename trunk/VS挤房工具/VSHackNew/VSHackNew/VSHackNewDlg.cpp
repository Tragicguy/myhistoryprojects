// VSHackNewDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VSHackNew.h"
#include "VSHackNewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

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


// CVSHackNewDlg 对话框




CVSHackNewDlg::CVSHackNewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVSHackNewDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVSHackNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVSHackNewDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CVSHackNewDlg 消息处理程序

BOOL CVSHackNewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

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

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CVSHackNewDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CVSHackNewDlg::OnPaint()
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
//
HCURSOR CVSHackNewDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//关闭警告框线程函数
DWORD WINAPI CVSHackNewDlg::CloseWarnProc(LPVOID lpParameter)
{
	((CVSHackNewDlg *)lpParameter )->CloseWarnWindow();
	return 0;
}
//关闭警告框
void CVSHackNewDlg::CloseWarnWindow()
{
	HWND hwndWarn = NULL;
	hwndWarn = ::FindWindow(_T("#32770"), _T("VS竞技游戏平台")); //满员警告框
	if(hwndWarn != NULL)
	{
		//将系统时间减少30秒,避开客户端验证
		SYSTEMTIME systime;
		GetLocalTime(&systime); 
		union 
		{
			FILETIME ft;
			ULONGLONG ut;
		}t;
		DWORD dwMiniSeconds=30;
		SystemTimeToFileTime(&systime, &t.ft);
		t.ut-=UInt32x32To64(dwMiniSeconds,10000000);//减30秒
		FileTimeToSystemTime(&t.ft, &systime);
		SetLocalTime(&systime);

		::SendMessage(hwndWarn,WM_CLOSE,0,0);

		Sleep(200);		//SLEEP 100 毫秒

		SystemTimeToFileTime(&systime, &t.ft);
		t.ut+=UInt32x32To64(dwMiniSeconds,10000000);//加30秒
		FileTimeToSystemTime(&t.ft, &systime);
		SetLocalTime(&systime);
		//将正确的系统时间还原
		//dblhitFlag = true;
	}
	hwndWarn = NULL;
	hwndWarn = ::FindWindow(_T("#32770"), _T("VSClient"));//不能重复进入房间警告框
	if(hwndWarn != NULL)
	{
		::SendMessage(hwndWarn,WM_CLOSE,0,0);
		Sleep(200);		//SLEEP 100 毫秒
		SYSTEMTIME systime;
		GetLocalTime(&systime);
		union 
		{
			FILETIME ft;
			ULONGLONG ut;
		}t;
		DWORD dwMiniSeconds=10;
		SystemTimeToFileTime(&systime, &t.ft);
		t.ut+=UInt32x32To64(dwMiniSeconds,10000000);//加10秒
		FileTimeToSystemTime(&t.ft, &systime);
		SetLocalTime(&systime);
		//dblhitFlag = true;
	}
}