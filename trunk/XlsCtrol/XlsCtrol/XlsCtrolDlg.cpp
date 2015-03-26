// XlsCtrolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "XlsCtrol.h"
#include "XlsCtrolDlg.h"

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


// CXlsCtrolDlg 对话框




CXlsCtrolDlg::CXlsCtrolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXlsCtrolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CXlsCtrolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CXlsCtrolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CXlsCtrolDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CXlsCtrolDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CXlsCtrolDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CXlsCtrolDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CXlsCtrolDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CXlsCtrolDlg::OnBnClickedButton6)
END_MESSAGE_MAP()


// CXlsCtrolDlg 消息处理程序

BOOL CXlsCtrolDlg::OnInitDialog()
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

	CoInitialize(NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CXlsCtrolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CXlsCtrolDlg::OnPaint()
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
HCURSOR CXlsCtrolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CXlsCtrolDlg::OnBnClickedButton1()
{
	
	m_XlsCtrol.Import("c:\\test.xls",m_vecExpressInfo);
	int iSize = m_vecExpressInfo.size();
	int i=0;
}


void CXlsCtrolDlg::GenData(vector<ExpressStr> &vecStr)
{
	ExpressStr expressStr;
	BookStr bookStr;
	CString strTemp = "";
	CString strKeyNo = CTime::GetCurrentTime().Format("%Y%m%d%H%M%S");
	for (int i=0;i<10;i++)
	{
		expressStr.vecBookInfo.clear();
		strTemp.Format("%s%d",strKeyNo,i);
		expressStr.strOrderNo = strTemp;
		expressStr.strAddr = "测试地址 " + strTemp;
		expressStr.strRecver = "张三 "+ strTemp;
		expressStr.strPayTime = "2011-03-09 11:15";
		expressStr.strExpressCompany = "圆通 " + strTemp;
		expressStr.strExpressNo = strTemp;
		expressStr.strPhone = "15076787654";
		expressStr.strMemo = "无";
		for (int j=0;j<2;j++)
		{
			bookStr.strISBN = strTemp;
			bookStr.strName = "测试书 ";
			bookStr.strNum = "2";
			bookStr.strPress = "中华";
			bookStr.strPrice = "25";
			bookStr.strSellPrice = "30";
			expressStr.vecBookInfo.push_back(bookStr);
		}
		vecStr.push_back(expressStr);
	}
	int ii = 0;
}

void CXlsCtrolDlg::OnBnClickedButton3()
{
	vector<ExpressStr>::iterator it;
	for (it = m_vecExpressInfo.begin();it!= m_vecExpressInfo.end();it++)
	{
		it->vecBookInfo.clear();
	}
	m_vecExpressInfo.clear();
	ExpressStr expressStr;
	BookStr bookStr;
	CString strTemp = "";
	CString strKeyNo = CTime::GetCurrentTime().Format("%Y%m%d%H%M%S");
	for (int i=0;i<10;i++)
	{
		strTemp.Format("%s%d",strKeyNo,i);
		expressStr.strOrderNo = strTemp;
		expressStr.strAddr = "测试地址 " + strTemp;
		expressStr.strRecver = "张三 "+ strTemp;
		expressStr.strPayTime = "2011-03-09 11:15";
		expressStr.strExpressCompany = "圆通 " + strTemp;
		expressStr.strExpressNo = strTemp;
		expressStr.strPhone = "15076787654";
		expressStr.strMemo = "无";
		for (int j=0;j<2;j++)
		{
			bookStr.strISBN = strTemp;
			bookStr.strName = "测试书 ";
			bookStr.strNum = "2";
			bookStr.strPress = "中华";
			bookStr.strPrice = "25";
			bookStr.strSellPrice = "30";
			expressStr.vecBookInfo.push_back(bookStr);
		}
		m_vecExpressInfo.push_back(expressStr);
	}
	int ii = 0;
}


void CXlsCtrolDlg::OnBnClickedButton2()
{
	vector<ExpressStr> vecStr;
	GenData(vecStr);
	m_XlsCtrol.Export("c:\\test.xls",vecStr,false);
}

void CXlsCtrolDlg::OnBnClickedButton4()
{
	ExpressStr expressStr;
	BookStr bookStr;
	CString strTemp = "TEST ISBN";
	expressStr.strOrderNo = "201103101512358";
	expressStr.strAddr = "11111111111111 " + strTemp;
	expressStr.strRecver = "11111111111 "+ strTemp;
	expressStr.strPayTime = "2011-03-09 11:15";
	expressStr.strExpressCompany = "11111111 " + strTemp;
	expressStr.strExpressNo = "11111111111111";
	expressStr.strPhone = "111111111111";
	expressStr.strMemo = "111111111111111";
	strTemp = "TEST ISBN";
	for (int j=0;j<2;j++)
	{
		bookStr.strISBN = strTemp;
		bookStr.strName = "测试书 ";
		bookStr.strNum = "2";
		bookStr.strPress = "中华";
		bookStr.strPrice = "25";
		bookStr.strSellPrice = "30";
		expressStr.vecBookInfo.push_back(bookStr);
	}
	m_XlsCtrol.Modify("c:\\test.xls",expressStr);
}

void CXlsCtrolDlg::OnBnClickedButton5()
{
	vector<ExpressStr> vecStr;
	GenData(vecStr);
	m_XlsCtrol.Add("c:\\test.xls",vecStr);
}

void CXlsCtrolDlg::OnBnClickedButton6()
{
	BasicExcel e;
	e.New(1);
	BasicExcelWorksheet* sheet = e.GetWorksheet("Sheet1");
	BasicExcelCell* cell;
	if (sheet)
	{
		for (size_t c=0; c<4; ++c)
		{
			cell = sheet->Cell(0,c);
			cell->Set((int)c);
		}
		cell = sheet->Cell(1,3);
		cell->SetDouble(3.141592654);

		sheet->Cell(1,4)->SetString("Test str1");
		sheet->Cell(2,0)->SetString("Test str2");
		sheet->Cell(2,5)->SetString("Test str1");

		sheet->Cell(4,0)->SetDouble(1.1);
		sheet->Cell(4,1)->SetDouble(2.2);
		sheet->Cell(4,2)->SetDouble(3.3);
		sheet->Cell(4,3)->SetDouble(4.4);
		sheet->Cell(4,4)->SetDouble(5.5);

		sheet->Cell(4,4)->EraseContents();
	}
	e.SaveAs("c:\\example3.xls");
	//e
}
