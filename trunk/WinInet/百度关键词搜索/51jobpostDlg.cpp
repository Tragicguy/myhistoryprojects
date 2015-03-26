#include "stdafx.h"
#include "51jobpost.h"
#include "51jobpostDlg.h"
#include ".\51jobpostdlg.h"
#include "CountGen.h"
#include "datatype.h"
#include "resource.h"
#include "UserFile.h"
#include "WindowFind.h"
#include <afxdb.h>
#include <odbcinst.h>
// #include <cv.h>
// #include <cxcore.h>
// #include <highgui.h>
// #pragma comment(lib,"cxcore.lib")
// #pragma comment(lib,"cv.lib")
// #pragma comment(lib,"highgui.lib")
//#pragma comment(lib,"cvaux.lib")
//#pragma comment(lib,"cvcam.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define  USE_DLL_DLL 000
#define WM_SHOWTASK WM_USER + 1000
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
CListCtrl g_AccList;
map<CString,int> g_keyMap;
typedef int (FAR __stdcall *FnMatchCheckCode)(char *szPath, char *szCode,int iFlag);

FnMatchCheckCode MatchCheckCode = NULL;
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
/************************************************************************/
/*                                                                      */
/************************************************************************/
CjobpostDlg::CjobpostDlg(CWnd* pParent /*=NULL*/)
: CDialog(CjobpostDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);
	m_pByte = NULL;
	CUserFile *pFile = CUserFile::GetInstance();
	CString strAppPath = "";
	pFile->GetAppPath(strAppPath);


}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CjobpostDlg::~CjobpostDlg()
{
	CUserMessagePump *pPump = CUserMessagePump::GetInstance();
	if (pPump)
	{
		delete pPump;
		pPump = NULL;
	}
	if (m_pByte != NULL)
	{
		delete[] m_pByte;
		m_pByte = NULL;
	}
	GdiplusShutdown(m_pGdiToken);
	if(m_handle)
	{
		FreeLibrary(m_handle);
	}

}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CjobpostDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_message);
	DDX_Control(pDX, IDC_LIST_COUNT, g_AccList);
	DDX_Control(pDX, IDC_BTN_CHECKCODE, m_btn);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BEGIN_MESSAGE_MAP(CjobpostDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START, OnBnClickedBtnStart)
	ON_MESSAGE(USERWINDOWMSG_1,AddToList)
	ON_BN_CLICKED(IDC_BTN_SET, OnBnClickedBtnSet)
	ON_BN_CLICKED(IDC_BTN_CHECKCODE, OnBnClickedBtnCheckcode)
	ON_BN_CLICKED(IDC_BTN_Registe, OnBnClickedBtnRegiste)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_COUNT, OnNMRclickListCount)
	ON_COMMAND(ID_MENU_REGISTER, OnMenuRegister)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBnClickedBtnStop)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_COMMAND(IDR_CLOSE, OnExitClose)
	ON_MESSAGE(WM_SHOWTASK,OnShowTask)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
END_MESSAGE_MAP()
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CjobpostDlg::OnInitDialog()
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

#ifndef DEBUG
	if (GetPower() != 0)
	{
		exit(0);
	}
#endif
	SetToTray();
	// TODO: 在此添加额外的初始化代码
	//此处加验证

	InitListCtrl();
	SetTableStyle();
	//m_btn.SubclassDlgItem(IDC_BTN_CHECKCODE,this);
	m_btn.DrawBorder(FALSE);
	//m_btn.DrawTransparent(TRUE);
	m_btn.SetTooltipText("点击重新获取验证码");
	m_btn.SetDefaultColors(RGB(255,0,0));
	m_btn.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(0x7c,0xc1,0xf1));
	m_btn.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0x8c,0x71,0xf1));//变颜色
	m_btn.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(0x7c,0xc1,0xf1));
	//m_btn.DrawBorder(FALSE);
	m_btn.SetBtnCursor(IDC_CURSOR1);
	m_handle = 0x0;
	CUserMessagePump *pPump = CUserMessagePump::GetInstance();
	if (pPump)
	{
		pPump->SetHwnd(USERWINDOWMSG_1,m_hWnd);
	}
	CString strAppPath = "";
	CString strTemp = "";
	CUserFile *pFile = CUserFile::GetInstance();
	pFile->GetAppPath(strAppPath);
	pFile->ReadInitInfo(strAppPath+"set.ini","Register","freshdelay",strTemp,g_iFreshDelay,1);
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CjobpostDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
/************************************************************************/
/*                                                                      */
/************************************************************************/
// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void CjobpostDlg::OnPaint() 
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
/************************************************************************/
/*                                                                      */
/************************************************************************/
//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CjobpostDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CjobpostDlg::OnBnClickedBtnStart()
{
	g_ExitFlag = false;
	g_AccList.DeleteAllItems();
	g_keyMap.clear();
	g_RegisterList.RemoveAll();
    m_JobPost.BeginRegister();
	CString strKeyWord = "";
	GetDlgItemText(IDC_EDIT_KEYWORD,strKeyWord);
	strKeyWord.Trim();
	if (strKeyWord == "")
	{
		MessageBox("请输入关键词","警告");
		return;
	}
	g_keyMap[strKeyWord] = 1;
	RegisterPack rgPack;
	rgPack.strAcc = strKeyWord;
	rgPack.iStatus = 0;
	g_RegisterList.Add(rgPack);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CjobpostDlg::SetTableStyle()
{
	CListCtrl* conList = &g_AccList;
	DWORD dwSytle=::GetWindowLong(conList->m_hWnd,GWL_STYLE);
	// 设置为报表形式
	SetWindowLong(conList->m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=conList->GetExtendedStyle();
	conList->SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CImageList image;
	image.Create(1,16, ILC_COLOR, 0, 0);
	conList->SetImageList(&image, LVSIL_STATE);
	conList->MoveWindow(7,5,780,310);
	conList->InsertColumn(0,"序号",LVCFMT_CENTER,50);
	conList->InsertColumn(1,"关键词",LVCFMT_LEFT,110);
	conList->InsertColumn(2,"层次",LVCFMT_LEFT,110);	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CjobpostDlg::InitListCtrl()
{
	DWORD dwSytle=::GetWindowLong(m_message.m_hWnd,GWL_STYLE);
	// 设置为报表形式
	SetWindowLong(m_message.m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=m_message.GetExtendedStyle();
	m_message.SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	::SendMessage(m_message.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	m_message.InsertColumn(0,_T("时间"),LVCFMT_LEFT,125);
	m_message.InsertColumn(1,_T("信息"),LVCFMT_LEFT,390);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
LRESULT CjobpostDlg::AddToList(WPARAM wParam,LPARAM lParam)
{
	CUserMessagePump *pMsgPump = NULL;
	pMsgPump = CUserMessagePump::GetInstance();
	if (!pMsgPump)
	{
		return 0;
	}
	CString msgBuf;
	pMsgPump->GetPumpMessage(USERWINDOWMSG_1,m_hWnd,msgBuf);
	if ( msgBuf == "" )
	{
		return 0;
	}

	int nCount = m_message.GetItemCount();
	if ( nCount >= 14 )
	{
		m_message.DeleteItem( 0 );
		m_message.DeleteItem( 0 );
		nCount -= 2;
	}
	m_message.InsertItem( nCount, "");
	if ( nCount % 2 == 0 )
	{
		m_message.SetRowSelectedBgColor(nCount, RGB(0,0,0) , false);
		m_message.SetRowTxtColor(nCount , RGB(255,0,0) , false);

	}
	else
	{
		m_message.SetRowTxtColor(nCount , RGB(0,0,255) , false);
	}
	m_message.SetRowBgColor(nCount, RGB(230,190,220), false );
	m_message.SetRowStyle(nCount,  LIS_TXTCOLOR | LIS_BGCOLOR);

	m_message.SetItemText( nCount , 0, CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S") );
	m_message.SetItemText( nCount , 1, msgBuf );
	::PostMessage(m_message.m_hWnd,WM_VSCROLL,SB_BOTTOM,0);
	if (msgBuf == "线程退出")
	{
		GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
	}
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CjobpostDlg::OnBnClickedBtnSet()
{
	CCountGen coutGen;
	coutGen.m_pList = &g_AccList;
	coutGen.DoModal();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CjobpostDlg::OnBnClickedBtnCheckcode()
{
	//GetDlgItem(IDC_BTN_Registe)->EnableWindow(FALSE);
	SetDlgItemText(IDC_EDIT_CODE,"");
	g_strCheckCode = "r";
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
bool CjobpostDlg::ReadBMPFile(LPCTSTR strFileName,LPBYTE &m_lpBMPFileData,long& fileLength)
{
	CFile BitmapFile;
	BOOL blOpen=BitmapFile.Open(strFileName, CFile::modeRead,NULL);
	if( !blOpen )
	{
		//CString str = "读文件失败！";
		m_lpBMPFileData = NULL;
		return false;
	}
	unsigned long FileLength=BitmapFile.GetLength();
	fileLength = FileLength;
	if (m_lpBMPFileData != NULL)
	{
		delete[] m_lpBMPFileData;
		m_lpBMPFileData = NULL;
	}
	m_lpBMPFileData = new BYTE[FileLength];
	ASSERT( m_lpBMPFileData!=NULL );
	BitmapFile.Read(m_lpBMPFileData,FileLength);

	if ( ((LPBITMAPFILEHEADER)m_lpBMPFileData)->bfType != 0x4D42 )
	{
		//CString str = "不支持除BMP之外的文件！";
		delete[] m_lpBMPFileData;
		m_lpBMPFileData = NULL;
		return false;;
	}
	return true;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//显示位图数据
BOOL CjobpostDlg::ShowBmpFile(HDC hdc, int nXDest, int nYDest,LPBYTE lpDIB,CRect rect)
{
	if( lpDIB == NULL)
	{
		return FALSE;
	}
	LPBITMAPINFOHEADER lpBMPInfoHeader=(LPBITMAPINFOHEADER) (lpDIB + sizeof(BITMAPFILEHEADER));
	BITMAPINFO * lpBMPInfo =(BITMAPINFO *)(lpDIB + sizeof(BITMAPFILEHEADER));
	int iWidth =0;
	int iHeight = 0;
	if ( lpBMPInfoHeader->biWidth > rect.Width())
	{
		iWidth = rect.Width();
	}
	else
	{
		iWidth = lpBMPInfoHeader->biWidth;
	}
	if ( lpBMPInfoHeader->biHeight > rect.Height())
	{
		iHeight = rect.Height();
	}
	else
	{
		iHeight = lpBMPInfoHeader->biHeight;
	}
	SetDIBitsToDevice(hdc,nXDest,nYDest,iWidth,iHeight,
		0,0,0,lpBMPInfoHeader->biHeight,lpDIB+((BITMAPFILEHEADER *)lpDIB)->bfOffBits,lpBMPInfo,DIB_RGB_COLORS);
	return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CjobpostDlg::ShowCheckCode()
{
 	m_btn.m_strSrc = g_strDirect+g_strFileName;
	ShowBmp(g_strDirect+g_strFileName,::GetDC(GetDlgItem(IDC_BTN_CHECKCODE)->GetSafeHwnd()));
	if (g_iAotuCheckCode)
	{
		TranstToBMP(g_strDirect+g_strFileName,g_strDirect+BMPFILE_PATH);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CjobpostDlg::GetImageCLSID(const WCHAR* format, CLSID* pCLSID)
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
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CjobpostDlg::TranstToBMP(CString srcPath,CString destPath)
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
void CjobpostDlg::OnBnClickedBtnRegiste()
{
	GetDlgItemText(IDC_EDIT_CODE,g_strCheckCode);
	g_strCheckCode.Trim();
	if (g_strCheckCode == "")
	{
		MessageBox("验证码不能为空","错误");
		return;
	}
	SetDlgItemText(IDC_EDIT_CODE,"");
	CString strShow  = "";
	strShow.Format("验证码已输入:%s,正在注册...",g_strCheckCode);
	CUserMessagePump *pMsgPump = NULL;
	pMsgPump = CUserMessagePump::GetInstance();
	if (pMsgPump)
	{
		pMsgPump->AddToMessagePump(USERWINDOWMSG_1,strShow);
	}
	//GetDlgItem(IDC_BTN_Registe)->EnableWindow(FALSE);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CjobpostDlg::PreTranslateMessage( MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;      //屏蔽掉Esc键
	}
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		HWND hWnd=::GetFocus();
		int iID=::GetDlgCtrlID(hWnd);
		if(iID==IDC_EDIT_CODE)
		{
			OnBnClickedBtnRegiste();
		}
		return TRUE;      //屏蔽掉Enter键
	}
	return CDialog::PreTranslateMessage( pMsg );
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CjobpostDlg::OnNMRclickListCount(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION pos = g_AccList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int nItem = g_AccList.GetNextSelectedItem(pos);
		if (g_AccList.GetItemText(nItem,5) != "√" && g_AccList.GetItemText(nItem,5) != "")
		{
			CMenu menu; 
			menu.LoadMenu(IDR_MENU1); //载入事先定义的选单 
			CMenu *pmenu = menu.GetSubMenu(0); 
			CPoint pos; 
			GetCursorPos(&pos);
			SetForegroundWindow();
			pmenu->TrackPopupMenu(TPM_LEFTALIGN,pos.x,pos.y,this);
		}		
	}
	*pResult = 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CjobpostDlg::OnMenuRegister()
{
	POSITION pos = g_AccList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int nItem = g_AccList.GetNextSelectedItem(pos);
		RegisterPack rgPack;
		rgPack.strAcc = g_AccList.GetItemText(nItem,1);
		rgPack.strPwd = g_AccList.GetItemText(nItem,2);
		rgPack.strMail = g_AccList.GetItemText(nItem,3);
		//AfxMessageBox(rgPack.strAcc);
		AddRegisterPackEx(rgPack);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CjobpostDlg::OnBnClickedBtnStop()
{
	g_ExitFlag = true;
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
	m_btn.m_strSrc = "";
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CjobpostDlg::OnClose()
{
	ShowWindow(SW_HIDE);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void ShowBmp(CString srcPath,HDC hdc)
{
	WCHAR wCH[255] = {0};
	wcscpy(wCH,srcPath.AllocSysString());
	Image img(wCH);//这里的图片可以是其它格式
	Graphics mygraphics(hdc);//
	int iWidth=img.GetWidth() * g_iShowWidth / 100;
	int iHeight=img.GetHeight() * g_iShowHeight / 100;
	mygraphics.DrawImage(&img,2,2,iWidth,iHeight);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CjobpostDlg::OnBnClickedButton2()
{
	g_keyMap.clear();
	g_RegisterList.RemoveAll();
	if (g_ExitFlag == false)
	{
		g_ExitFlag = true;
		Sleep(800);
	}
	Shell_NotifyIcon(NIM_DELETE,&nid);
	exit(0);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CjobpostDlg::GetRetString(CString strSrc)
{
	char num = 0;
	CString strRet = "";
	CString strTemp = "";
	for (int i= 0;i<strSrc.GetLength();i++)
	{
		num = strSrc.GetAt(i);
		if ((num >= 48 && num <=57) ||
			(num >= 65 && num <=90) ||
			(num >= 97 && num <=122))
		{
			strTemp.Format("%c",num);
			strRet += strTemp;
		}
	}
	return strRet;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CjobpostDlg::SetToTray()
{
	nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd=this->m_hWnd;
	nid.uID=IDR_MAINFRAME;
	nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP ;
	nid.uCallbackMessage = WM_SHOWTASK;//自定义的消息名称
	nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	strcpy(nid.szTip,"PhpWind顶帖器");
	Shell_NotifyIcon(NIM_ADD,&nid);//在托盘区添加图标    
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
LRESULT CjobpostDlg::OnShowTask(WPARAM wParam,LPARAM lParam)
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
			this->ShowWindow(!IsWindowVisible());
		}
		break;
	}
	return 0;

}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CjobpostDlg::OnExitClose()
{
	OnBnClickedButton2();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CjobpostDlg::GetPower()
{
	CInternetSession session;
	CString strTemp = "";
	try
	{
		CStdioFile *pFile = session.OpenURL("http://blog.csdn.net/TragicGuy/archive/2010/01/08/5157814.aspx");
		if (pFile != NULL)
		{
			while(pFile->ReadString(strTemp))
			{
				strTemp.MakeLower();
				if (strTemp.Find("myownpower333") !=-1)
				{
					pFile->Close();
					delete pFile;
					pFile = NULL;
					return 0;
				}
			}

			if (pFile == NULL)
			{
				pFile->Close();
				delete pFile;
				pFile = NULL;
			}
		}
	}
	catch (...)
	{
	}
	return -1;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CjobpostDlg::OnBnClickedButton3()
{
	CString strAcc="";
	CString strPwd="";
	CString strPhone = "";
	CString strRecommandPhone = "";
	int   lines   =   0;   
	CString strFilePath = "";
// 	char szFilter[]="Text files(*.txt)|*.txt";
// 	CFileDialog dlg(1,NULL,NULL,NULL,szFilter);
// 	if(dlg.DoModal()==IDOK) 
// 	{
// 		strFilePath = dlg.GetPathName();
// 	}
// 	if (strFilePath=="")
// 	{
// 		return;
// 	}
	GetDefaultTxtFileName(strFilePath);
	CFile file;
	file.Open(strFilePath,CFile::modeCreate|CFile::modeWrite);
	int iCount = g_AccList.GetItemCount();
	for (int i=0;i<iCount;i++)
	{
		strAcc = g_AccList.GetItemText(i,1)+"\n";
		file.Write(strAcc,strAcc.GetLength());
	}
	file.Close();
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
//获得默认的文件名
BOOL CjobpostDlg::GetDefaultXlsFileName(CString& sExcelFile)
{
	///默认文件名：yyyymmddhhmmss.xls
	CString timeStr = "";
	timeStr = CTime::GetCurrentTime().Format("%Y%m%d%H%M%S");
	timeStr = "关键词导出["+timeStr+"]";

	sExcelFile =  timeStr + ".xls";
	// prompt the user (with all document templates)
	CFileDialog dlgFile(FALSE,".xls",sExcelFile);
	CString title;
	CString strFilter;

	title = "导出";
	strFilter = "Excel文件(*.xls)";
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.xls");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;
	dlgFile.m_ofn.nFilterIndex = 1;
	// append the "*.*" all files filter
	CString allFilter;
	VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.*");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;

	if (dlgFile.DoModal()==IDCANCEL)
	{
		return FALSE; // open cancelled
	}	
	sExcelFile.ReleaseBuffer();
	sExcelFile = dlgFile.GetPathName();
	sExcelFile.MakeLower();
	if (sExcelFile.Right(4) != ".xls")
	{
		sExcelFile+=".xls";
	}
	if (MakeSurePathExists(sExcelFile,true)) 
	{
		if(!DeleteFile(sExcelFile)) 
		{    // delete the file
			AfxMessageBox("覆盖文件时出错！");
			return FALSE;
		}
	}
	return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CjobpostDlg::GetDefaultTxtFileName(CString& sExcelFile)
{
	///默认文件名：yyyymmddhhmmss.xls
	CString timeStr = "";
	timeStr = CTime::GetCurrentTime().Format("%Y%m%d%H%M%S");
	timeStr = "关键词导出["+timeStr+"]";

	sExcelFile =  timeStr + ".txt";
	// prompt the user (with all document templates)
	CFileDialog dlgFile(FALSE,".txt",sExcelFile);
	CString title;
	CString strFilter;

	title = "导出";
	strFilter = "Excel文件(*.txt)";
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.txt");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;
	dlgFile.m_ofn.nFilterIndex = 1;
	// append the "*.*" all files filter
	CString allFilter;
	VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.*");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;

	if (dlgFile.DoModal()==IDCANCEL)
	{
		return FALSE; // open cancelled
	}	
	sExcelFile.ReleaseBuffer();
	sExcelFile = dlgFile.GetPathName();
	sExcelFile.MakeLower();
	if (sExcelFile.Right(4) != ".txt")
	{
		sExcelFile+=".txt";
	}
	if (MakeSurePathExists(sExcelFile,true)) 
	{
		if(!DeleteFile(sExcelFile)) 
		{    // delete the file
			AfxMessageBox("覆盖文件时出错！");
			return FALSE;
		}
	}
	return TRUE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CjobpostDlg::MakeSurePathExists( CString &Path,bool FilenameIncluded)
{
	int Pos=0;
	while((Pos=Path.Find('\\',Pos+1))!=-1)
	{
		CreateDirectory(Path.Left(Pos),NULL);
	}
	if(!FilenameIncluded)
	{
		CreateDirectory(Path,NULL);
	}
	return !_access(Path,0);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CjobpostDlg::GetExcelDriver()
{
	char szBuf[2001];
	WORD cbBufMax = 2000;
	WORD cbBufOut;
	char *pszBuf = szBuf;
	CString sDriver;

	// 获取已安装驱动的名称(涵数在odbcinst.h里)
	if (!SQLGetInstalledDrivers(szBuf, cbBufMax, &cbBufOut))
		return "";

	// 检索已安装的驱动是否有Excel...
	do
	{
		if (strstr(pszBuf, "Excel") != 0)
		{
			//发现 !
			sDriver = CString(pszBuf);
			break;
		}
		pszBuf = strchr(pszBuf, '\0') + 1;
	}
	while (pszBuf[1] != '\0');

	return sDriver;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CjobpostDlg::OnBnClickedButton4()
{
	try
	{
		CDatabase database;
		CString sDriver;
		CString sExcelFile; 
		CString sSql;
		CString tableName = "导出结果";
		// 检索是否安装有Excel驱动 "Microsoft Excel Driver (*.xls)" 
		sDriver = GetExcelDriver();
		if (sDriver.IsEmpty())
		{
			// 没有发现Excel驱动
			ShowMessage("请先安装Excel软件才能使用导出功能");
			return;
		}
		CString strSQL;				
		///默认文件名
		if (!GetDefaultXlsFileName(sExcelFile))
		{
			return;
		}
		ShowMessage("正在进行导出,请稍后...");
		// 创建进行存取的字符串
		sSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, sExcelFile);
		// 创建数据库 (既Excel表格文件)
		if( database.OpenEx(sSql,CDatabase::noOdbcDialog) )
		{
			CString strTemp;
			strTemp.Format("关键词 TEXT");
			// 创建表结构			
			sSql = "CREATE TABLE " + tableName + " ( " + strTemp +  " ) ";
			int i = 0;
			database.ExecuteSQL(sSql);			
		}
		else
		{
			ShowMessage("创建EXCEL文件出错,导出未完成");
			return;
		}
		int iCount = g_AccList.GetItemCount();
		for (int i=0;i<iCount;i++)
		{
			sSql.Format("INSERT INTO %s (关键词)values('%s')",tableName,g_AccList.GetItemText(i,1));
			database.ExecuteSQL(sSql);
		}
		database.Close();
		ShowMessage("导出完成");		
	}
	catch(...)
	{
		ShowMessage("执行异常,导出未完成");
	}
}
