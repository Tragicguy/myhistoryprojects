// LibCurlTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LibCurlTest.h"
#include "LibCurlTestDlg.h"

#include <string>
#include "../libcurl/include/curl.h"
#pragma comment(lib, "../libcurl/lib/libcurl_imp.lib")

using namespace std;
static char errorBuffer[CURL_ERROR_SIZE];
static int writer(char *, size_t, size_t, string *);
static bool init(CURL *&, char *,string *);


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


// CLibCurlTestDlg 对话框




CLibCurlTestDlg::CLibCurlTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLibCurlTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLibCurlTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLibCurlTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CLibCurlTestDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CLibCurlTestDlg 消息处理程序

BOOL CLibCurlTestDlg::OnInitDialog()
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

void CLibCurlTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CLibCurlTestDlg::OnPaint()
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
HCURSOR CLibCurlTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CLibCurlTestDlg::OnBnClickedButton1()
{
	CURL *conn = NULL;
	CURLcode code;
	string buffer;

	curl_global_init(CURL_GLOBAL_ALL);
	//char* url="https://www.amazon.cn";
	char* url="http://www.baidu.com";
	if (!init(conn,url,&buffer ))
	{
		fprintf(stderr, "Connection initializion failed\n");
		exit(EXIT_FAILURE);
	}
	code = curl_easy_perform(conn);

	if (code != CURLE_OK)
	{
		
		AfxMessageBox(errorBuffer);
		exit(EXIT_FAILURE);
	}
	curl_easy_cleanup(conn);
	if (buffer.size() > 1000)
	{
		AfxMessageBox(buffer.substr(0, 1000).c_str());return;
	}
	AfxMessageBox(buffer.c_str());
	return ;

}

static bool init(CURL *&conn, char *url,string *p_buffer)
{
	CURLcode code;
	conn = curl_easy_init();
	if (conn == NULL)
	{
		fprintf(stderr, "Failed to create CURL connection\n");
		exit(EXIT_FAILURE);
	}
	code = curl_easy_setopt(conn, CURLOPT_ERRORBUFFER, errorBuffer);
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to set error buffer [%d]\n", code);
		return false;
	}
	code = curl_easy_setopt(conn, CURLOPT_URL, url);
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to set URL [%s]\n", errorBuffer);
		return false;
	}
	code = curl_easy_setopt(conn, CURLOPT_FOLLOWLOCATION, 1);
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to set redirect option [%s]\n", errorBuffer);
		return false;
	}
	//curl_easy_setopt(conn,CURLOPT_CAINFO,"c:\\ca.crt");
	curl_easy_setopt(conn, CURLOPT_SSL_VERIFYPEER,  FALSE);
	curl_easy_setopt(conn, CURLOPT_SSL_VERIFYHOST,  FALSE); 

	code = curl_easy_setopt(conn, CURLOPT_WRITEFUNCTION, writer);
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to set writer [%s]\n", errorBuffer);
		return false;
	}
	code = curl_easy_setopt(conn, CURLOPT_WRITEDATA, p_buffer);
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to set write data [%s]\n", errorBuffer);
		return false;
	}
	return true;
}

static int writer(char *data, size_t size, size_t nmemb, string *writerData)
{
	unsigned long sizes = size * nmemb;
	if (writerData == NULL) return 0;
	writerData->append(data, sizes);
	return sizes;
}
