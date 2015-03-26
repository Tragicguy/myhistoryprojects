// FileTransferTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FileTransferTest.h"
#include "FileTransferTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFileTransferTestDlg 对话框


#define MY_SQUITMSG (WM_USER + 1000)
#define MY_CQUITMSG (WM_USER + 1001)

CFileTransferTestDlg::CFileTransferTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileTransferTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileTransferTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ServerIP);
	DDX_Control(pDX, IDC_EDIT3, m_ServerPort);
	DDX_Control(pDX, IDC_EDIT1, m_ServerListenPort);
}

BEGIN_MESSAGE_MAP(CFileTransferTestDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CFileTransferTestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CFileTransferTestDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CFileTransferTestDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CFileTransferTestDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CFileTransferTestDlg::OnBnClickedButton5)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CFileTransferTestDlg 消息处理程序

BOOL CFileTransferTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_ServerListenPort.SetWindowTextA("4300");
	m_ServerPort.SetWindowTextA("4300");
	m_ServerIP.SetAddress(127,0,0,1);

	OnBnClickedButton1();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFileTransferTestDlg::OnPaint()
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
HCURSOR CFileTransferTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFileTransferTestDlg::OnBnClickedButton1()
{
	if(!m_File.InitFileDLLFunction())
	{
		AfxMessageBox("初始化失败！");
	}
}

void CFileTransferTestDlg::OnBnClickedButton2()
{
	char buf[8];
	memset(buf, 0, 8);

	m_ServerListenPort.GetWindowText(buf, 8);
	int i = atoi(buf);

	if (!m_File.m_funcCreateFileServer(i, this->m_hWnd, MY_SQUITMSG, "1033"))
	{
		AfxMessageBox("CreateFileServer失败！");
	}
}

void CFileTransferTestDlg::OnBnClickedButton3()
{
	m_File.m_funcStopFileServer();
}

void CFileTransferTestDlg::OnBnClickedButton4()
{
	char buf[8];
	memset(buf, 0, 8);

	m_ServerPort.GetWindowText(buf, 8);
	int i = atoi(buf);

	char IPBuf[20];
	memset(IPBuf, 0, 20);

	m_ServerIP.GetWindowText(IPBuf, 20);

	if (!m_File.m_funcCreateFileClient(IPBuf, i, this->m_hWnd, MY_CQUITMSG, true))
	{
		AfxMessageBox("CreateFileClient失败！");
	}
}

void CFileTransferTestDlg::OnBnClickedButton5()
{
	m_File.m_funcStopFileClient();
}

void CFileTransferTestDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_File.m_funcStopFileServer();
	m_File.m_funcStopFileClient();
	CDialog::OnClose();
}

LRESULT CFileTransferTestDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (message)
	{
	case MY_SQUITMSG:
		if (1 == wParam)
		{
			CString str = (LPCTSTR) lParam;
			TRACE("Receive file %s success...\r\n", str);
		}
		break;

	case MY_CQUITMSG:
		if (1 == wParam)
		{
			CString str = (LPCTSTR) lParam;
			TRACE("Sent file %s success...\r\n", str);
		}
		break;

	default:
		break;
	}
	return CDialog::DefWindowProc(message, wParam, lParam);
}
