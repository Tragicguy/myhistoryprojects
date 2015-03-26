// WarHelperDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WarHelper.h"
#include "WarHelperDlg.h"
#include <io.h>
#include ".\warhelperdlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

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

/////////////////////////////////////////////////////////////////////////////
// CWarHelperDlg dialog

CWarHelperDlg::CWarHelperDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWarHelperDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWarHelperDlg)
	m_show = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWarHelperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWarHelperDlg)
	DDX_Text(pDX, IDC_SHOW, m_show);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWarHelperDlg, CDialog)
	//{{AFX_MSG_MAP(CWarHelperDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SET, OnSet)
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_MESSAGE(WM_SHOWTASK,OnShowTask)
	ON_MESSAGE(WM_SETTIMER3,OnSetTimer3)
	ON_MESSAGE(WM_SETTIMER4,OnSetTimer4)
	ON_MESSAGE(WM_KILLTIMER3,OnKillTimer3)
	ON_MESSAGE(WM_KILLTIMER4,OnKillTimer4)
	ON_COMMAND(IDR_CLOSE, OnClose)
	ON_COMMAND(IDR_SET, OnMenuSet)
	ON_COMMAND(IDR_ABOUT, OnAbout)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWarHelperDlg message handlers
_declspec(dllimport) HookStruct SetHook(HWND m_hwnd,HWND m_hwndHelper,UINT i,UINT j ,UINT k,UINT m,UINT n,UINT p,BOOL m_WinFlag);
BOOL CWarHelperDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
    
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
    PostMessage(WM_SIZE,0,0);
	SetToTray();
	::SetTimer(m_hWnd,1,1000,NULL);
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CWarHelperDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWarHelperDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWarHelperDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CWarHelperDlg::OnSet() 
{
	// TODO: Add your control notification handler code here
	CWarHelperSetDlg dlg;
	CStdioFile   file;
	CString   cstr;
    CFileException   fe;   
	
  if(file.Open("key.txt",CFile::modeCreate|CFile::modeNoTruncate|CFile::modeRead|CFile::shareDenyWrite,&fe))   
  { 
	  UpdateData(true);
	  file.SeekToBegin();
	  file.ReadString(cstr); 
	  GetSavedData(cstr);
	  dlg.InitData(cstr);
	  file.Close();
	  dlg.DoModal();
      
  }
  else { MessageBox("读取文件出错！");  ::KillTimer(m_hWnd,1);}
}

void CWarHelperDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	if(nIDEvent == 1)
	{
		
		
    CWnd *m_pWndHack = FindWindow("Warcraft III",NULL);
	m_hwndHack = m_pWndHack->GetSafeHwnd();		
    if(m_hwndHack == NULL)
		{
			m_show = "游戏未启动,请启动 War III!";
		}
		else
		{   
			CStdioFile   file;
	    CString   cstr;
        CFileException   fe;   
		if(_access("key.txt",0) != 0)
		{
			   if(file.Open("key.txt",CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::shareDenyWrite,&fe))   
			   { 
	  
	            file.SeekToBegin();
	            file.WriteString("1,41,0,0,0,0,0");
				SetFileAttributes("key.txt",FILE_ATTRIBUTE_HIDDEN);
                 file.Close();
			   }
  
               else { MessageBox("读取文件出错！");::KillTimer(m_hWnd,1);}
		}
  
if(file.Open("key.txt",CFile::modeCreate|CFile::modeNoTruncate|CFile::modeRead|CFile::shareDenyWrite,&fe))   
  { 
	  
	  file.SeekToBegin();
	  file.ReadString(cstr); 
	  GetSavedData(cstr);
      file.Close();
  }
  
    else { MessageBox("读取文件出错！"); ::KillTimer(m_hWnd,1);}


			
	        m_show = "已经对游戏进行控制!";
			
			UnloadHook();
    	  KeyHook = SetHook(m_hwndHack,m_hWnd,(int)(strtod(m_vk7.GetBuffer(m_vk7.GetLength()),&stopstring)),(int)(strtod(m_vk4.GetBuffer(m_vk4.GetLength()),&stopstring)),(int)(strtod(m_vk1.GetBuffer(m_vk1.GetLength()),&stopstring)),(int)(strtod(m_vk8.GetBuffer(m_vk8.GetLength()),&stopstring)),(int)(strtod(m_vk5.GetBuffer(m_vk5.GetLength()),&stopstring)),(int)(strtod(m_vk2.GetBuffer(m_vk2.GetLength()),&stopstring)),m_WinFlag);
	     ::SetTimer(m_hWnd,2,6000,NULL);
		 ::KillTimer(m_hWnd,1);
         }
	}
	if(nIDEvent == 2)
	{
       CWnd *m_pWndHack = FindWindow("Warcraft III",NULL);
	   m_hwndHack = m_pWndHack->GetSafeHwnd();
		if(m_hwndHack == NULL)
		{   
			m_show = "游戏退出!";
            UnloadHook();
    	    
			::KillTimer(m_hWnd,3);
			::KillTimer(m_hWnd,4);
			::SetTimer(m_hWnd,1,2000,NULL);
			::KillTimer(m_hWnd,2);
		}
	
	}

	if(nIDEvent == 3)
	{
   		if(m_hwndHack != NULL)
		{   
		 ::SendMessage(m_hwndHack,WM_KEYUP,VK_OEM_4,0);
		 ::SendMessage(m_hwndHack,WM_KEYDOWN,VK_OEM_4,0);
			 
		}
	
	}
    
	if(nIDEvent == 4)
	{
   		if(m_hwndHack != NULL)
		{   
		 ::SendMessage(m_hwndHack,WM_KEYUP,VK_OEM_6,0);
		 ::SendMessage(m_hwndHack,WM_KEYDOWN,VK_OEM_6,0);
				 
		 
		}
	
	}
	UpdateData(false);	
	CDialog::OnTimer(nIDEvent);
}

void CWarHelperDlg::GetData(CString vk7,CString vk4,CString vk1,CString vk8,CString vk5,CString vk2,CString winstr)
{
    m_vk7 = vk7;
	m_vk4 = vk4;
	m_vk1 = vk1;
	m_vk8 = vk8;
	m_vk5 = vk5;
	m_vk2 = vk2;
	if(winstr == "1")   m_WinFlag = TRUE;
	else  m_WinFlag = FALSE;

}

LRESULT CWarHelperDlg::OnSetTimer3(WPARAM wParam,LPARAM lParam)
{
  ::SetTimer(m_hWnd,3,1000,NULL);
  return 0;
}

LRESULT CWarHelperDlg::OnSetTimer4(WPARAM wParam,LPARAM lParam)
{
  ::SetTimer(m_hWnd,4,1000,NULL);
  return 0;
}

LRESULT CWarHelperDlg::OnKillTimer3(WPARAM wParam,LPARAM lParam)
{
  ::KillTimer(m_hWnd,3);
  return 0;
}
LRESULT CWarHelperDlg::OnKillTimer4(WPARAM wParam,LPARAM lParam)
{
  ::KillTimer(m_hWnd,4);
  return 0;
}
void CWarHelperDlg::Begin()
{
    CWnd *m_pWndHack = FindWindow("Warcraft III",NULL);
	HWND  m_hwndHack = m_pWndHack->GetSafeHwnd();
	if(m_hwndHack != NULL)
    {   
		UnloadHook();
      KeyHook = SetHook(m_hwndHack,m_hWnd,(int)(strtod(m_vk7.GetBuffer(m_vk7.GetLength()),&stopstring)),(int)(strtod(m_vk4.GetBuffer(m_vk4.GetLength()),&stopstring)),(int)(strtod(m_vk1.GetBuffer(m_vk1.GetLength()),&stopstring)),(int)(strtod(m_vk8.GetBuffer(m_vk8.GetLength()),&stopstring)),(int)(strtod(m_vk5.GetBuffer(m_vk5.GetLength()),&stopstring)),(int)(strtod(m_vk2.GetBuffer(m_vk2.GetLength()),&stopstring)),m_WinFlag);
	}
}

void CWarHelperDlg::GetSavedData(CString str)
{
    
	int pos;
    pos =  str.Find(',')+1;
	m_vk7 = str.Mid(0,pos-1);
	str = str.Mid(pos,str.GetLength());
	
    pos =  str.Find(',')+1;
	m_vk4 = str.Mid(0,pos-1);
	str = str.Mid(pos,str.GetLength());

	pos =  str.Find(',')+1;
	m_vk1 = str.Mid(0,pos-1);
	str = str.Mid(pos,str.GetLength());

	pos =  str.Find(',')+1;
	m_vk8 = str.Mid(0,pos-1);
	str = str.Mid(pos,str.GetLength());

	pos =  str.Find(',')+1;
	m_vk5 = str.Mid(0,pos-1);
	str = str.Mid(pos,str.GetLength());
	
	pos =  str.Find(',')+1;
	m_vk2 = str.Mid(0,pos-1);
	
    CString wintemp = str.Mid(pos,str.GetLength());
    if(wintemp == "1")  m_WinFlag = TRUE;
	else m_WinFlag = FALSE;

	
}





void CWarHelperDlg::SetToTray()
{
   
    nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA);
    nid.hWnd=this->m_hWnd;
    nid.uID=IDR_MAINFRAME;
    nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP ;
    nid.uCallbackMessage = WM_SHOWTASK;//自定义的消息名称
    nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
    strcpy(nid.szTip,"魔兽按键替换助手");
    Shell_NotifyIcon(NIM_ADD,&nid);//在托盘区添加图标    
}

LRESULT CWarHelperDlg::OnShowTask(WPARAM wParam,LPARAM lParam)
{
 if(wParam!=IDR_MAINFRAME)
    return 1;
    switch(lParam)
    {
        case WM_RBUTTONUP:
			{      CMenu menu; 
                   menu.LoadMenu(IDR_POPUPMENU); //载入事先定义的选单 
                   CMenu *pmenu = menu.GetSubMenu(0); 
                   CPoint pos; 
                   GetCursorPos(&pos);
				   SetForegroundWindow();
                   pmenu->TrackPopupMenu(TPM_CENTERALIGN,pos.x,pos.y,AfxGetMainWnd());
			}		
              break;
      case WM_LBUTTONDOWN:
		{
		Shell_NotifyIcon(NIM_DELETE,&nid);
        this->ShowWindow(SW_SHOW);
		}
        break;
   }
    return 0;

}

void CWarHelperDlg::OnSize(UINT nType, int cx, int cy) 
{
	
	ShowWindow(SW_HIDE);
}

void CWarHelperDlg::OnCancel()
{
   Shell_NotifyIcon(NIM_ADD,&nid);
	ShowWindow(SW_HIDE);
   
}

void CWarHelperDlg::OnClose() 
{
	// TODO: Add your command handler code here
   UnloadHook();
   Shell_NotifyIcon(NIM_DELETE,&nid);
	CDialog::OnCancel();
	
}

void CWarHelperDlg::OnMenuSet() 
{
	// TODO: Add your command handler code here
	OnSet();
}

void CWarHelperDlg::OnAbout() 
{
	// TODO: Add your command handler code here
	CAboutDlg dlg;
	dlg.DoModal();
}

void CWarHelperDlg::OnButtonExit()
{
	// TODO: Add your control notification handler code here
	OnClose();
}

void CWarHelperDlg::UnloadHook(void)
{
	if(KeyHook.hKeyBoardHook  != NULL)
		UnhookWindowsHookEx(KeyHook.hKeyBoardHook);
  if(KeyHook.hLowKeyBoardHook != NULL)
		UnhookWindowsHookEx(KeyHook.hLowKeyBoardHook);
}
