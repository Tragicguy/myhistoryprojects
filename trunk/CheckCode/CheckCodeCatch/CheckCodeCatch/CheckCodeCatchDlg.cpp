// CheckCodeCatchDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CheckCodeCatch.h"
#include "CheckCodeCatchDlg.h"
#include "WebBase.h"
#include <io.h>
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


// CCheckCodeCatchDlg 对话框




CCheckCodeCatchDlg::CCheckCodeCatchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCheckCodeCatchDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);
}
CCheckCodeCatchDlg::~CCheckCodeCatchDlg()
{
	GdiplusShutdown(m_pGdiToken);
}

void CCheckCodeCatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCheckCodeCatchDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_BEGIN, &CCheckCodeCatchDlg::OnBnClickedBtnBegin)
END_MESSAGE_MAP()


// CCheckCodeCatchDlg 消息处理程序

BOOL CCheckCodeCatchDlg::OnInitDialog()
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
	m_strHeader = "Content-Type: application/x-www-form-urlencoded";
	m_strHeader += "\r\nAccept:image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, application/x-silverlight, */*";
	m_strHeader += "\r\nAccept-Language:zh-cn";
	m_strHeader += "\r\nAccept-Encoding:gzip, deflate";//
	m_strHeader += "\r\nCache-Control:no-cache";
	m_strHeader += "\r\nUser-Agent:Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 1.1.4322; .NET CLR 2.0.50727)";
	m_strHeader += "\r\nConnection:Keep-Alive";
	m_strDirect = "D:\\PIC\\";
	m_iGetCount = 1;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCheckCodeCatchDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCheckCodeCatchDlg::OnPaint()
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
HCURSOR CCheckCodeCatchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCheckCodeCatchDlg::OnBnClickedBtnBegin()
{
	m_ExitFlag = FALSE;
	m_iTotalCount = GetDlgItemInt(IDC_EDIT_GETNUM);
	if (m_iTotalCount < 1)
	{
		return;
	}
	AfxBeginThread((AFX_THREADPROC)ThreadFunc,(LPVOID)this);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCheckCodeCatchDlg::ThreadFunc(LPVOID lParam)
{
	//CoInitialize(NULL);
	CCheckCodeCatchDlg *pParam = ((CCheckCodeCatchDlg *)lParam);
	if ( pParam )
	{
		//pParam->RegisterFunc();
		//pParam->ShowMessage("线程退出");
		pParam->GetCheckCode();
	}
	//CoUninitialize();

}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCheckCodeCatchDlg::GetCheckCode()
{
	CWebBase webBase;
	CString strServer = "www.eku8.com";
	CString strError = "";
	CString strObject = "/JH_UserConfig/FundCode.aspx";
	CString strPath = "";
	CString strDestPath = "";
	int iRet = -1;
	CString strTime = CTime::GetCurrentTime().Format("%m%d%H%M%S");;
	srand(time(NULL));
	SetDlgItemText(IDC_INFOSHOW,"线程启动");
	while (!m_ExitFlag)
	{
		if (m_iGetCount > m_iTotalCount)
		{
			m_ExitFlag = TRUE;
			continue;
		}
		try
		{
			SetDlgItemText(IDC_INFOSHOW,"正在抓取...");
			SYSTEMTIME   t; 
			GetLocalTime(&t); 			
			strPath.Format("%s%d-%s.gif",m_strDirect,m_iGetCount,strTime);
			strDestPath.Format("%s%d-%s.bmp",m_strDirect,m_iGetCount,strTime);
			strObject.Format("/JH_UserConfig/FundCode.aspx?time=%d%d%d&rand=%d",strTime,t.wMinute,t.wSecond,t.wMilliseconds,rand()%10000);
			m_iGetCount++;
			iRet = webBase.RequestGetPageExExEx(strServer,strObject,m_strHeader,strError,1,0,0,"",80,"","",strPath);
			//转存一下
			TranstToBMP(strPath,strDestPath);
			CFile::Remove(strPath);
			//显示当前数量
			SetDlgItemInt(IDC_EDIT_GOTNUM,m_iGetCount-1);
			Sleep(10);
		}
		catch (...)
		{
			SetDlgItemText(IDC_INFOSHOW,"抓取出现异常");
		}		
	}	
	SetDlgItemText(IDC_INFOSHOW,"抓取线程退出");
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CCheckCodeCatchDlg::TranstToBMP(CString srcPath,CString destPath)
{
	WCHAR wCH[255] = {0};
	WCHAR wCH1[255] = {0};
	wcscpy(wCH,srcPath.AllocSysString());
	Image img(wCH);//这里的图片可以是其它格式	
	CLSID pngClsid;
	GetImageCLSID(L"image/bmp", &pngClsid);//这里的图片可以是其它格式，此处转化为BMP格式
	wcscpy(wCH1,   destPath.AllocSysString());
	img.Save(wCH1, &pngClsid, NULL);
	return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CCheckCodeCatchDlg::GetImageCLSID(const WCHAR* format, CLSID* pCLSID)
{
	UINT num = 0;
	UINT size = 0;
	ImageCodecInfo* pImageCodecInfo = NULL;
	GetImageEncodersSize(&num, &size);
	if(size == 0)
	{
		return FALSE;
	}
	pImageCodecInfo = (ImageCodecInfo *)malloc((size));
	if(pImageCodecInfo == NULL)
		return FALSE;
	GetImageEncoders(num, size, pImageCodecInfo);
	// Find for the support of format for image in the windows
	for(UINT i = 0; i < num; ++i)
	{
		//MimeType: Depiction for the program image
		if( wcscmp(pImageCodecInfo[i].MimeType, format) == 0)
		{
			*pCLSID = pImageCodecInfo[i].Clsid;
			free(pImageCodecInfo);
			return TRUE;
		}
	}
	free(pImageCodecInfo);
	return FALSE;
}