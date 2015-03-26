// QRCodeTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "QRCodeTest.h"
#include "QRCodeTestDlg.h"
#include ".\qrcodetestdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
HMODULE g_hDll;
typedef bool (__stdcall *FnWMEnCode)(const char* szPath, const char* pszStr);
typedef bool (__stdcall *FnWMDeCode)(const char* szPath, char* pszStr);
FnWMEnCode WMEnCode;
FnWMDeCode WMDeCode;
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


// CQRCodeTestDlg 对话框



CQRCodeTestDlg::CQRCodeTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQRCodeTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQRCodeTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CQRCodeTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_ENCODE, OnBnClickedBtnEncode)
	ON_BN_CLICKED(IDC_BTN_DECODE, OnBnClickedBtnDecode)
	ON_BN_CLICKED(IDC_BTN_GETPATH, OnBnClickedBtnGetpath)
END_MESSAGE_MAP()


// CQRCodeTestDlg 消息处理程序

BOOL CQRCodeTestDlg::OnInitDialog()
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

	char path[255] = {0};
	::GetCurrentDirectory(255, path);
	strcat(path, "\\QR.dll");
	if (NULL == (g_hDll = ::LoadLibrary(path)))
	{
		AfxMessageBox("加载QR.dll失败！");
		exit(0);
	}
	//	MongateSetCmdType = (FnMongateTestConn)::GetProcAddress(g_hDll, "MongateSetCmdType");	
	WMEnCode = (FnWMEnCode)::GetProcAddress(g_hDll, "WMEnCode");
	WMDeCode = (FnWMDeCode)::GetProcAddress(g_hDll, "WMDeCode");	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CQRCodeTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CQRCodeTestDlg::OnPaint() 
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
HCURSOR CQRCodeTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CQRCodeTestDlg::OnBnClickedBtnEncode()
{
	char szPath[255] = {0};
	strcpy(szPath,"e:\\code.bmp");	
	char szStr[1000] = {0};
	CString strContent = "";
	GetDlgItemText(IDC_EDIT_CONTENT,strContent);
	if (strContent.Trim() == "" || strContent.GetLength() > 140)
	{
		MessageBox("输入不合法");
		return;
	}
	strcpy(szStr,strContent);
	if(!WMEnCode(szPath,szStr))
	{
		MessageBox("编码失败");
	}
	OnBnClickedBtnGetpath();
}

void CQRCodeTestDlg::OnBnClickedBtnDecode()
{
	char szPath[255] = {0};
	strcpy(szPath,"e:\\code.bmp");
	char szStr[1000] = {0};
	if(WMDeCode(szPath,szStr))
	{
		SetDlgItemText(IDC_EDIT_CONTENT,szStr);
	}
	else
	{
		MessageBox("解码失败");
	}
}


//显示位图数据
BOOL CQRCodeTestDlg::ShowBmpFile(HDC hdc, int nXDest, int nYDest,LPBYTE lpDIB)
{
	if( lpDIB == NULL)
	{
		return FALSE;
	}
	LPBITMAPINFOHEADER lpBMPInfoHeader=(LPBITMAPINFOHEADER) (lpDIB + sizeof(BITMAPFILEHEADER));
	BITMAPINFO * lpBMPInfo =(BITMAPINFO *)(lpDIB + sizeof(BITMAPFILEHEADER));

	SetDIBitsToDevice(hdc,nXDest,nYDest,lpBMPInfoHeader->biWidth,lpBMPInfoHeader->biHeight,
		0,0,0,lpBMPInfoHeader->biHeight,lpDIB+((BITMAPFILEHEADER *)lpDIB)->bfOffBits,lpBMPInfo,DIB_RGB_COLORS);

	return TRUE;
}

/***************************************************************************   
*   函数名称   
*   ReadBMPFile  
*   参数   
*        LPCTSTR strFileName         -   图像的路径   

*   返回值   
*         LPBYTE                     -返回图像二进制数据
*   说明   
*         该函数采用文件读取的方法读取图像二进制流   
***************************************************************************/ 
bool CQRCodeTestDlg::ReadBMPFile(LPCTSTR strFileName,LPBYTE &m_lpBMPFileData)
{

	CFile BitmapFile;
	BOOL blOpen=BitmapFile.Open(strFileName, CFile::modeRead,NULL);
	if( !blOpen)
	{
		//CString str = "读文件失败！";
		m_lpBMPFileData = NULL;
		return false;
	}

	unsigned long FileLength=BitmapFile.GetLength();
	m_lpBMPFileData = new BYTE[FileLength];

	ASSERT( m_lpBMPFileData!=NULL );
	BitmapFile.Read(m_lpBMPFileData,FileLength);

	if ( ((LPBITMAPFILEHEADER)m_lpBMPFileData)->bfType != 0x4D42 )
	{
		//"不支持除BMP之外的文件！";
		delete[] m_lpBMPFileData;
		m_lpBMPFileData = NULL;
		return false;;
	}
	return true;
}	

void CQRCodeTestDlg::OnBnClickedBtnGetpath()
{
	LPBYTE pBMPFileData;
	ReadBMPFile("e:\\code.bmp",pBMPFileData);
	ShowBmpFile(::GetDC(m_hWnd),50,150,pBMPFileData);
	if (pBMPFileData)
	{
		delete[] pBMPFileData;
		pBMPFileData = NULL;
	}	
}
