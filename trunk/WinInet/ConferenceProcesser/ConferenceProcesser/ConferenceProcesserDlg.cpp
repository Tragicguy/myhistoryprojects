// ConferenceProcesserDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ConferenceProcesser.h"
#include "ConferenceProcesserDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#pragma warning(disable:4099)

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


// CConferenceProcesserDlg 对话框




CConferenceProcesserDlg::CConferenceProcesserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConferenceProcesserDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CConferenceProcesserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list_show);
}

BEGIN_MESSAGE_MAP(CConferenceProcesserDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START, &CConferenceProcesserDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BUTTON2, &CConferenceProcesserDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CConferenceProcesserDlg::OnBnClickedBtnExit)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CConferenceProcesserDlg 消息处理程序

BOOL CConferenceProcesserDlg::OnInitDialog()
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
	
	CWebBase::WebInit();
	InitListCtrl();
	SetWindowText("视频会话显示系统数据转换");
	m_ProgramConfig.SetConfigFileName("set.ini");
	
	CString strUrl = m_ProgramConfig.ReadStringConfigParameter("set", "url", "http://svcm.paic.com.cn/svcm/display/display.xml");
	int nInterval = m_ProgramConfig.ReadIntConfigParameter("set", "interval", 10000);
	SetDlgItemText(IDC_EDIT_PATH, strUrl);
	SetDlgItemInt(IDC_EDIT_INTERVAL, nInterval);
	GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
#ifdef DEBUG
	GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_SHOW);
#endif
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CConferenceProcesserDlg::InitListCtrl()
{
	DWORD dwSytle=::GetWindowLong(m_list_show.m_hWnd,GWL_STYLE);
	// 设置为报表形式
	SetWindowLong(m_list_show.m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=m_list_show.GetExtendedStyle();
	m_list_show.SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	::SendMessage(m_list_show.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	m_list_show.InsertColumn(0,_T("会议ID"),LVCFMT_LEFT,70);
	m_list_show.InsertColumn(1,_T("会议名称"),LVCFMT_LEFT,120);
	m_list_show.InsertColumn(2,_T("开始时间"),LVCFMT_LEFT,90);
	m_list_show.InsertColumn(3,_T("结束时间"),LVCFMT_LEFT,90);
	m_list_show.InsertColumn(4,_T("部门"),LVCFMT_LEFT,140);
	m_list_show.InsertColumn(5,_T("会议室名"),LVCFMT_LEFT,140);
	m_list_show.InsertColumn(6,_T("参与人数"),LVCFMT_LEFT,60);
	m_list_show.InsertColumn(7,_T("会议室状态"),LVCFMT_LEFT,70);
	m_list_show.InsertColumn(8,_T("类型"),LVCFMT_LEFT,70);
	m_list_show.MoveWindow(10, 60, 900, 420);
}
void CConferenceProcesserDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CConferenceProcesserDlg::OnPaint()
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
HCURSOR CConferenceProcesserDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CConferenceProcesserDlg::OnBnClickedBtnStart()
{
	CString strUrl = "";
	GetDlgItemText(IDC_EDIT_PATH, strUrl);
	int iInterval = GetDlgItemInt(IDC_EDIT_INTERVAL);
	if(!m_DataProcess.InitInfo(strUrl, iInterval, true, true))
	{
		AfxMessageBox("地址非法");
	}
	m_ProgramConfig.WriteStringConfigParameter("set", "url", strUrl);
	int nInterval = m_ProgramConfig.WriteIntConfigParameter("set", "interval", iInterval);
	m_DataProcess.SetShowHwnd(m_list_show.m_hWnd, ::GetDlgItem(m_hWnd, IDC_STATIC_NOTICE));
	m_DataProcess.StartWorking();
	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
}

void CConferenceProcesserDlg::OnBnClickedButton2()
{

	ConferenceInfo info;
	info.boxnos = "BOX00100";
	info.updatetime = "2011-07-01 13:00:00";
	info.version = "2.0";
	ConferenceRecord record;
	record.bookpeople = CFunction::ConvertGBKToUtf8Ex("邵军");
	record.chairman = CFunction::ConvertGBKToUtf8Ex("邵军");
	record.currentfloor = false;
	record.displayformat.ishighlight = true;
	record.endtime = "2011-07-01 17:00:00";
	record.starttime = "2011-07-01 11:00:00";
	record.roomid = "0001";
	record.roomname = CFunction::ConvertGBKToUtf8Ex("会议室A");
	record.topic = CFunction::ConvertGBKToUtf8Ex("讨论一下未来发展规划");
	record.peoples.push_back(CFunction::ConvertGBKToUtf8Ex("部门一"));
	record.peoples.push_back(CFunction::ConvertGBKToUtf8Ex("部门二"));
	record.peoples.push_back(CFunction::ConvertGBKToUtf8Ex("部门三"));
	info.record.push_back(record);
	
	info.ToXml("c:\\bbb.xml");
	return;
	//m_DataProcess.ToXml();
	TiXmlDocument xmlDoc;  
	
	TiXmlDeclaration *pDeclaration = new TiXmlDeclaration(_T("1.0"),_T("utf-8"),_T(""));
	if (!pDeclaration)	return;
	xmlDoc.LinkEndChild(pDeclaration);  
	// 生成一个根节点：MyApp  
	TiXmlElement *pRootEle = TinyXmlFunction::DocNewElement("MyApp", &xmlDoc);
	// 生成子节点：Messages  
	TiXmlElement *pMsg = TinyXmlFunction::NewElement("Messages", pRootEle);
	
	TinyXmlFunction::NewElement("Welcome", pMsg, "fuck you");
	TinyXmlFunction::NewElement("Farewell", pMsg, "Thank you for using MyApp");
	
	// 生成子节点：Windows  
	TiXmlElement *pWindows = TinyXmlFunction::NewElement("Windows", pRootEle);
	if (!pWindows)  return;

	// 生成子节点：Window  
	TiXmlElement *pWindow = TinyXmlFunction::NewElement("Window", pWindows);
	if (!pWindow)  return;

	// 设置节点Window的值  
	pWindow->SetAttribute(_T("name"),_T("MainFrame"));  
	pWindow->SetAttribute(_T("x"),_T("5"));
	pWindow->SetAttribute(_T("y"),_T("15"));
	pWindow->SetAttribute(_T("w"),_T("400"));
	pWindow->SetAttribute(_T("h"),_T("250"));

	// 生成子节点：Connection  
	TiXmlElement *pConnection  = TinyXmlFunction::NewElement("Connection", pRootEle);
	if (!pConnection)  return;
	// 设置节点Connection的值  
	pConnection->SetAttribute(_T("ip"),_T("192.168.0.1"));  
	pConnection->SetAttribute(_T("timeout"),_T("123.456000"));  
	xmlDoc.SaveFile("c:\\df.xml");  
	return ;  
}

void CConferenceProcesserDlg::OnBnClickedBtnExit()
{
	PostMessage(WM_CLOSE, 0, 0);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CConferenceProcesserDlg::PreTranslateMessage( MSG* pMsg )
{
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;      //屏蔽掉Esc键
	}
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		return TRUE;      //屏蔽掉Enter键
	}
	return CDialog::PreTranslateMessage(pMsg);
}
void CConferenceProcesserDlg::OnClose()
{
	m_DataProcess.StopWorking();
	CDialog::OnClose();
}
