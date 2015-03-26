// DbGate.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "DbGate.h"
#include "MainFrm.h"
#include "DbGateDoc.h"
#include "DbGateView.h"
#include "ControlFile/UserFile.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDbGateApp
CString g_strWindowName = "";
CString g_strVirsion = "";
BEGIN_MESSAGE_MAP(CDbGateApp, CWinApp)
	//{{AFX_MSG_MAP(CDbGateApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDbGateApp construction

CDbGateApp::CDbGateApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDbGateApp object

CDbGateApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDbGateApp initialization

BOOL CDbGateApp::InitInstance()
{
	// 检测是否已经创建Mutex
	// 如果已经创建，就终止进程的启动
// 	m_hMutex = CreateMutex(NULL, TRUE, "mw_dbgate_deploy_version");	
// 	if ((m_hMutex != NULL) && (GetLastError() == ERROR_ALREADY_EXISTS)) 
// 	{
// 		ReleaseMutex(m_hMutex);
// 		MessageBox(NULL, "该程序已经在运行中,请不要运行多个!", "严重警告", MB_OK);
// 		return FALSE;
// 	}
	
	CString strAppPath="";
	CUserFile *pFile = CUserFile::GetInstance();
	if ( pFile )
	{
		pFile->GetAppPath(strAppPath);
		if (strAppPath.GetLength() > 16)
		{
			strAppPath = strAppPath.Right(16);
		}
		strAppPath = pFile->StrEncode(strAppPath);
	}
	if (strAppPath == "")
	{
		return FALSE;
	}
	if (strAppPath.GetLength() > 32)
	{
		strAppPath = strAppPath.Right(32);
	}
	// 检测是否已经创建Mutex
	// 如果已经创建，就终止进程的启动
	m_hMutex = CreateMutex(NULL, TRUE, strAppPath);	
	if ((m_hMutex != NULL) && (GetLastError() == ERROR_ALREADY_EXISTS)) 
	{
		ReleaseMutex(m_hMutex);
		//MessageBox(NULL, "该程序已经在运行中,请不要运行多个!", "严重警告", MB_OK);
		return FALSE;
	}
	g_strServiceName.Format("MWGate %s",strAppPath);//.GetLength() > 20?strAppPath.Right(20):strAppPath);

	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("梦网数据库短信网关"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CDbGateDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CDbGateView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);	
	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;		
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	::CoInitialize(NULL);	
	m_pMainWnd->SetWindowText(g_strWindowName);
	return TRUE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CDbGateApp::ExitInstance(void)
{
	ReleaseMutex(m_hMutex);
	CloseHandle(m_hMutex);
	::CoUninitialize();
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetDlgItemText(IDC_WINDOWNAME,g_strWindowName);
	::SetWindowText(this->m_hWnd,"关于 "+g_strWindowName);
	return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CDbGateApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CDbGateApp message handlers

