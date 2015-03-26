// DetectDLLTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DetectDLLTest.h"
#include "DetectDLLTestDlg.h"
#include ".\detectdlltestdlg.h"
#include "LoadDllFile.h"
#include "OpenCVCheck.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

extern "C" _declspec(dllimport) void GetBinaryData(LPBYTE m_pSourse,LPBYTE &m_pNewbmp);
extern "C" _declspec(dllimport) CPoint GetCenterPoint(CString picPath,CString xmlPath);
extern "C" _declspec(dllimport) int GetVal(int a,int b,int iFlag);

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


// CDetectDLLTestDlg 对话框



CDetectDLLTestDlg::CDetectDLLTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDetectDLLTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDetectDLLTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDetectDLLTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
END_MESSAGE_MAP()


// CDetectDLLTestDlg 消息处理程序

BOOL CDetectDLLTestDlg::OnInitDialog()
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

	// TODO: 在此添加额外的初始化代码
	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CDetectDLLTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDetectDLLTestDlg::OnPaint() 
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
HCURSOR CDetectDLLTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDetectDLLTestDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnOK();

	CString sFolderPath;
	BROWSEINFO bi;
	char Buffer[MAX_PATH]; //初始化入口参数bi开始
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer; //此参数如为NULL则不能显示对话框
	bi.lpszTitle = "请选择图片所在目录";
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.iImage = 0;   //初始化入口参数bi结束
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//调用显示选择对话框
	if(pIDList)
	{
		SHGetPathFromIDList(pIDList, Buffer);
		//取得文件夹路径到Buffer里
		sFolderPath = Buffer;//将路径保存在一个CString对象里
	}
	CFileFind finder;
	CString findName = sFolderPath + "\\*.jpg";
	BOOL findFlag = finder.FindFile(findName);
	while (findFlag)
	{
		findFlag = finder.FindNextFile();
		if (finder.IsDots())
			continue;

		CString sFileName = finder.GetFileName();
		HINSTANCE hinstLib;
		LoadDllFile loadDll;
		hinstLib = loadDll.GetDllInstance("Dll\\DetectDll.dll");
		MYPROC GetCenterPoint = (MYPROC)loadDll.GetFuncProc(hinstLib,"GetCenterPoint");
		if (GetCenterPoint)
		{
			CPoint detectPoint =GetCenterPoint(sFolderPath+"\\"+sFileName,"c:\\OpenCVTrain\\data.xml");
			CString aa;
			aa.Format("%d,%d",detectPoint.x,detectPoint.y);
			MessageBox(aa);
			FreeLibrary(hinstLib);
		}
		
		
	}
	finder.Close();
}

void CDetectDLLTestDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	LPBYTE pSourse;
	LPBYTE pNewbmp;

	COpenCVCheck mycheck;
	mycheck.ReadBMPFile("C:\\aa.bmp",pSourse);

	GetBinaryData(pSourse,pNewbmp);

	delete pSourse;
	pSourse = NULL;

	CFile file;
	file.Open("E:\\aa.bmp",CFile::modeCreate|CFile::modeReadWrite);
	file.Write(pNewbmp,mycheck.GetPicLength(pNewbmp));
}

void CDetectDLLTestDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString testStr;
	int result = GetVal(3,5,1);
	testStr.Format("%d-%d=%d",3,5,result);
	MessageBox(testStr);
}
