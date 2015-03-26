// Web1800AutoTesterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Web1800AutoTester.h"
#include "Web1800AutoTesterDlg.h"
#include "WindowFind.h"
#include <io.h>
#include <direct.h>
#include "FileUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
bool g_ExitFlagClient = false;
bool g_ExitFlagServer = false;
bool g_bClientRequest = false; //表示客户端的会话窗口已经打开
CString g_strSpName = "";
CWindowFind g_find;
HWND g_hwndSpWindow = NULL;
HWND g_hwndSessionWindow = NULL;	//会话窗口句柄

HWND g_hwndWarn = NULL;
int g_iInterval = 4000;


BOOL CALLBACK EnumChildProcWarn(HWND   hwnd, LPARAM lParam)
{   
	if(g_find.GetContrlClassName(hwnd)=="Static") 
	{   
		if(g_find.GetContrlWindowText(hwnd)=="客户已经退出。") 
		{
			g_hwndWarn  = hwnd;
			return FALSE;
		}
	}
	EnumChildWindows(hwnd,EnumChildProcWarn,lParam);
	return   TRUE;   
}

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


// CWeb1800AutoTesterDlg 对话框




CWeb1800AutoTesterDlg::CWeb1800AutoTesterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWeb1800AutoTesterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWeb1800AutoTesterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_message);
	DDX_Control(pDX, IDC_LIST_MSGSP, m_spMessage);
}

BEGIN_MESSAGE_MAP(CWeb1800AutoTesterDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_STARTTIMER, &CWeb1800AutoTesterDlg::OnBnClickedBtnStarttimer)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_STARTTEST, &CWeb1800AutoTesterDlg::OnBnClickedBtnStarttest)
	ON_BN_CLICKED(IDC_BUTTON1, &CWeb1800AutoTesterDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CWeb1800AutoTesterDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CWeb1800AutoTesterDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CWeb1800AutoTesterDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BTN_STARTSP, &CWeb1800AutoTesterDlg::OnBnClickedBtnStartsp)
	ON_BN_CLICKED(IDC_BTN_STOPCLIENT, &CWeb1800AutoTesterDlg::OnBnClickedBtnStopclient)
	ON_BN_CLICKED(IDC_BTN_STOPSP, &CWeb1800AutoTesterDlg::OnBnClickedBtnStopsp)
END_MESSAGE_MAP()


// CWeb1800AutoTesterDlg 消息处理程序

BOOL CWeb1800AutoTesterDlg::OnInitDialog()
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

	SetDlgItemText(IDC_EDIT_STARFFID,"02f9cf57-31fe-44b1-ba69-a134ed37b959");
	SetDlgItemText(IDC_EDIT_URL,"http://1800cisvr30/8000000");
	CoInitialize(NULL);
	InitListCtrl();
	GetDlgItem(IDC_BTN_STOPSP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STOPCLIENT)->EnableWindow(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CWeb1800AutoTesterDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CWeb1800AutoTesterDlg::OnPaint()
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
HCURSOR CWeb1800AutoTesterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CALLBACK EnumChildProcExExEx(HWND hwnd, LPARAM lParam)
{   
	char strClassName[255];
	char strWindowName[255];
	memset(strClassName,0,sizeof(strClassName));   
	memset(strWindowName,0,sizeof(strWindowName));  
	if(g_find.GetContrlWindowText(hwnd) == "网上客服中心 Ver 3.0")   
	{   
		if (g_find.GetContrlClassName(hwnd) == "#32770")
		{
			HWND hwndTemp = g_find.GetChildWindow(hwnd,3);
			if (g_find.GetContrlWindowText(hwndTemp) == g_strSpName)
			{
				g_hwndSpWindow  = hwnd;
				return FALSE;
			}			
		}
		else
		{
			EnumChildWindows(hwnd,EnumChildProcExExEx,lParam); 
			return TRUE;
		}
	}
	EnumChildWindows(hwnd,EnumChildProcExExEx,lParam);
	return TRUE;
}
BOOL CWeb1800AutoTesterDlg::PreTranslateMessage(MSG* pMsg)
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

void CWeb1800AutoTesterDlg::OnBnClickedBtnStarttimer()
{
	GetDlgItemText(IDC_EDIT_SPNAME,g_strSpName);
	g_hwndSpWindow = NULL;
	g_iInterval = GetDlgItemInt(IDC_EDIT_INTERVAL);
	EnumChildWindows(NULL,EnumChildProcExExEx,(LPARAM)NULL);
	if (g_hwndSpWindow == NULL)
	{
		AfxMessageBox("程序未启动");
		return;
	}
	SetTimer(1,g_iInterval,NULL);
	GetDlgItem(IDC_BTN_STARTTIMER)->EnableWindow(FALSE);
}

void CWeb1800AutoTesterDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		SetTimer(2,g_iInterval,NULL);
		KillTimer(1);
		if (!::IsWindow(g_hwndSpWindow))
		{
			AfxMessageBox("目标程序已退出");
			return;
		}
		HWND hwnd = g_find.GetChildWindow(g_hwndSpWindow,18);
		g_find.PostBtnClick(hwnd);
	}
	else if (nIDEvent == 2)
	{
		SetTimer(1,g_iInterval,NULL);
		KillTimer(2);
		HWND hwnd = ::FindWindow("#32770","  汉维测试 网上客服中心 ");
		if (hwnd)
		{
			::PostMessage(hwnd,WM_CLOSE,0,0);
		}
	}
	else if (nIDEvent == 3)
	{
		HWND hwnd = ::FindWindow("#32770","Microsoft Internet Explorer");
		if (hwnd)
		{
			HWND hwndTemp = g_find.GetChildWindow(hwnd,4);
			CString strText = g_find.GetContrlWindowText(hwndTemp);
			if (strText.Find("关闭窗口将退出整个服务")>=0)
			{
				hwndTemp = g_find.GetChildWindow(hwnd,1);
				g_find.PostBtnClick(hwndTemp);
			}
			else
			{
				::PostMessage(hwnd,WM_CLOSE,0,0);
			}
		}
		hwnd = ::FindWindow("Internet Explorer_TridentDlgFrame","Internet Explorer 脚本错误");
		if (hwnd)
		{
			::PostMessage(hwnd,WM_CLOSE,0,0);
		}
		hwnd = ::FindWindow("Internet Explorer_TridentDlgFrame","系统错误 -- 网页对话框");
		if (hwnd)
		{
			::PostMessage(hwnd,WM_CLOSE,0,0);
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CWeb1800AutoTesterDlg::InitListCtrl()
{
	DWORD dwSytle=::GetWindowLong(m_message.m_hWnd,GWL_STYLE);
	// 设置为报表形式
	SetWindowLong(m_message.m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=m_message.GetExtendedStyle();
	m_message.SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	::SendMessage(m_message.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	m_message.MoveWindow(7,250,370,270);
	m_message.InsertColumn(0,_T("时间"),LVCFMT_LEFT,90);
	m_message.InsertColumn(1,_T("信息"),LVCFMT_LEFT,250);

	dwSytle=::GetWindowLong(m_spMessage.m_hWnd,GWL_STYLE);
	// 设置为报表形式
	SetWindowLong(m_spMessage.m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	ExStyle=m_spMessage.GetExtendedStyle();
	m_spMessage.SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	::SendMessage(m_spMessage.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	m_spMessage.MoveWindow(377,250,382,270);
	m_spMessage.InsertColumn(0,_T("时间"),LVCFMT_LEFT,90);
	m_spMessage.InsertColumn(1,_T("信息"),LVCFMT_LEFT,250);


}

void CWeb1800AutoTesterDlg::OnBnClickedBtnStarttest()
{
	SetTimer(3,400,NULL);
	GetDlgItem(IDC_BTN_STARTTEST)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STOPCLIENT)->EnableWindow(TRUE);
	AfxBeginThread((AFX_THREADPROC)ThreadFunc,(LPVOID)this);
}

void CWeb1800AutoTesterDlg::OnBnClickedBtnStartsp()
{
	GetDlgItem(IDC_BTN_STARTSP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STOPSP)->EnableWindow(TRUE);
	AfxBeginThread((AFX_THREADPROC)ThreadFuncSp,(LPVOID)this);
}

int CWeb1800AutoTesterDlg::AddToList(CString msgBuf)
{
	if ( msgBuf == "" )
	{
		return 0;
	}
	int nCount = m_message.GetItemCount();
	if ( nCount >= 50 )
	{
		m_message.DeleteItem( 0 );
		m_message.DeleteItem( 0 );
		m_message.DeleteItem( 0 );
		nCount -= 3;
	}
	nCount = m_message.GetItemCount();
	CString strShow = m_message.GetItemText(nCount-1,1);
	if (msgBuf == strShow)
	{
		m_message.SetItemText( nCount-1 , 0, CTime::GetCurrentTime().Format("%H:%M:%S") );
		::PostMessage(m_message.m_hWnd,WM_VSCROLL,SB_BOTTOM,0);
		return 0;
	}
	m_message.InsertItem( nCount, "");	
	m_message.SetItemText( nCount , 0, CTime::GetCurrentTime().Format("%H:%M:%S") );
	m_message.SetItemText( nCount , 1, msgBuf );
	::PostMessage(m_message.m_hWnd,WM_VSCROLL,SB_BOTTOM,0);
	WriteLog(msgBuf,1);
	return 0;
}


int CWeb1800AutoTesterDlg::AddToListSp(CString msgBuf)
{
	if ( msgBuf == "" )
	{
		return 0;
	}
	int nCount = m_spMessage.GetItemCount();
	if ( nCount >= 50 )
	{
		m_spMessage.DeleteItem( 0 );
		m_spMessage.DeleteItem( 0 );
		m_spMessage.DeleteItem( 0 );
		nCount -= 3;
	}
	nCount = m_spMessage.GetItemCount();
	CString strShow = m_spMessage.GetItemText(nCount-1,1);
	if (msgBuf == strShow)
	{
		m_spMessage.SetItemText( nCount-1 , 0, CTime::GetCurrentTime().Format("%H:%M:%S") );
		::PostMessage(m_spMessage.m_hWnd,WM_VSCROLL,SB_BOTTOM,0);
		return 0;
	}
	m_spMessage.InsertItem( nCount, "");	
	m_spMessage.SetItemText( nCount , 0, CTime::GetCurrentTime().Format("%H:%M:%S") );
	m_spMessage.SetItemText( nCount , 1, msgBuf );
	::PostMessage(m_spMessage.m_hWnd,WM_VSCROLL,SB_BOTTOM,0);
	WriteLog(msgBuf,2);
	return 0;
}

void CWeb1800AutoTesterDlg::WriteLog(CString srcStr,int iFlag)
{
	CTime currTime = CTime::GetCurrentTime();
	try
	{
		CString filepath = "";
		CString destStr = "";
		CString folder = "";
		CString filename = "";
		CString strAppPath = "";
		if (iFlag == 0)			//注册成功列表
		{
			folder = "log";
			filename.Format("%s_freshList.txt",currTime.Format("%Y%m%d"));
		}
		else if(iFlag == 1)		//信息日志
		{
			folder = "log";
			filename.Format("%s_log.txt",currTime.Format("%Y%m%d"));
		}
		else if ( iFlag == 2 )	//缓冲日志
		{
			folder = "logSp";
			filename.Format("%s_log.txt",currTime.Format("%Y%m%d"));
		}
		else			//异常日志 (3)
		{
			folder = "exception";
			filename.Format("%s_exception.txt",currTime.Format("%Y%m%d"));
		}

		GetAppPath(strAppPath);
		CString folderName = "";
		folderName.Format("%s%s",strAppPath,folder);
		if(_access(folderName,0)!=0)
		{
			if( mkdir(folderName) != 0 )//创建目录
			{
				return;
			}
		}					
		destStr.Format("%s	%s",currTime.Format("%Y-%m-%d %H:%M:%S"),srcStr);
		filepath.Format("%s\\%s",folderName,filename);
		FILE *fp;
		char szfile[1000]={0};
		sprintf(szfile,filepath);
		fp=fopen(szfile,"a+");
		fprintf(fp,"%s\r\n",destStr);
		fclose(fp);
	}
	catch (...)
	{
	}
}


int CWeb1800AutoTesterDlg::GetAppPath(CString &strAppPath)
{
	int iRet = -3;
	try
	{
		TCHAR szBuff[_MAX_PATH];
		VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szBuff, _MAX_PATH));
		strAppPath.Format("%s",szBuff);
		int pos=strAppPath.ReverseFind('\\');
		strAppPath=strAppPath.Left(pos+1);		
		iRet = 0;
	}
	catch (...)
	{
		
	}
	return iRet;	
}
IHTMLDocument2* CWeb1800AutoTesterDlg::GetDocInterface(HWND hWnd) 
{
	// 我们需要显示地装载OLEACC.DLL,这样我们才知道有没有安装MSAA
	
	HINSTANCE hInst = ::LoadLibrary( _T("OLEACC.DLL") );
	IHTMLDocument2* pDoc2=NULL;
	if ( hInst != NULL ){
		if ( hWnd != NULL ){
			CComPtr<IHTMLDocument> spDoc=NULL;
			LRESULT lRes;

			UINT nMsg = ::RegisterWindowMessage( _T("WM_HTML_GETOBJECT") );
			::SendMessageTimeout( hWnd, nMsg, 0L, 0L, SMTO_ABORTIFHUNG, 1000, (DWORD*)&lRes );

			LPFNOBJECTFROMLRESULT pfObjectFromLresult = (LPFNOBJECTFROMLRESULT)::GetProcAddress( hInst, ("ObjectFromLresult") );
			if ( pfObjectFromLresult != NULL ){
				HRESULT hr;
				hr=pfObjectFromLresult(lRes,IID_IHTMLDocument,0,(void**)&spDoc);
				if ( SUCCEEDED(hr) ){
					CComPtr<IDispatch> spDisp;
					CComQIPtr<IHTMLWindow2> spWin;
					spDoc->get_Script( &spDisp );
					spWin = spDisp;
					spWin->get_document( &pDoc2 );
				}
			}
		}
		::FreeLibrary(hInst);
	} 
	else{//如果没有安装MSAA
		AfxMessageBox(_T("请您安装Microsoft Active Accessibility"));
	}
	//CoUninitialize();
	return pDoc2;
}


bool CWeb1800AutoTesterDlg::SendRequest()
{
	CString strStaffId = "";
	AddToList("正在查找支持中心窗口");
	GetDlgItemText(IDC_EDIT_STARFFID,strStaffId);
	HWND hwnd = GetExplorerHWND();
	if (!hwnd)
	{
		AddToList("获取句柄失败");
		return false;
	}
	IHTMLDocument2* pDoc = GetDocInterface(hwnd);
	if (!pDoc)
	{
		AddToList("获取HTML控制失败");
		return false;
	}
	strStaffId = "St_" + strStaffId;
	IHTMLElement *pElement = NULL;
	GetElement(strStaffId,&pElement,pDoc);
	if (pElement == NULL)
	{
		AddToList("支持人员正忙或不在线，请稍后再请求");
		pDoc->Release();
		return false;
	}
	IHTMLElement *pElementParent = NULL;
	pElement->get_parentElement(&pElementParent);
	if (!pElementParent)
	{
		AddToList("查找请求入口失败");
		pElement->Release();
		pDoc->Release();
		return false;
	}
	BSTR bstrStr;
	pElementParent->get_tagName(&bstrStr);
	CString str(bstrStr);
	::SysFreeString(bstrStr);
	str.MakeLower();
	if (str == "a")
	{
		pElementParent->click();
		pElementParent->Release();
		pElement->Release();
		pDoc->Release();
		AddToList("成功发送会话请求");
		return true;
	}
	else
	{
		pElementParent->click();
		pElementParent->Release();
		pElement->Release();
		pDoc->Release();
		AddToList("支持人员正忙或不在线，请稍后再请求");
		return false;
	}
}

void CWeb1800AutoTesterDlg::OnBnClickedButton1()
{
	SendRequest();
	return;
	CString strStaffId = "";
	GetDlgItemText(IDC_EDIT_STARFFID,strStaffId);	
	HWND hwnd = GetExplorerHWND();
	if (!hwnd)
	{
		AfxMessageBox("获取句柄失败");
		return;
	}
	IHTMLDocument2* pDoc = GetDocInterface(hwnd);
	if (!pDoc)
	{
		//pDoc->Get
	}
	strStaffId = "St_" + strStaffId;
/*
	CString strJsString = "";
	strJsString.Format("var tTable=document.getElementById('%s'); alert(tTable.childNodes.length);",strStaffId);
	strJsString = "alert('aaaaaaa');";
	IHTMLWindow2*   pWindow;
	pDoc->get_parentWindow(&pWindow);   
	VARIANT  ret;   
	BSTR bstrStrEx = strJsString.AllocSysString();
	pWindow->execScript(bstrStrEx,L"javascript",&ret);
	::SysFreeString(bstrStrEx);
	pWindow->Release();*/

	//return;
	IHTMLElement *pElement = NULL;
	GetElement(strStaffId,&pElement,pDoc);
	if (pElement == NULL)
	{
		return;
	}
	IHTMLElement *pElementParent = NULL;
	pElement->get_parentElement(&pElementParent);
	if (!pElementParent)
	{
		pElement->Release();
		return;
	}
	BSTR bstrStr;
	pElementParent->get_tagName(&bstrStr);
	CString str(bstrStr);
	::SysFreeString(bstrStr);
	str.MakeLower();
	if (str == "a")
	{
		pElementParent->click();
		pElementParent->Release();
		pElement->Release();
	}
	else
	{
		AfxMessageBox("当前不能请求客服服务");
		return;
	}
}

HRESULT CWeb1800AutoTesterDlg::GetElement(LPCTSTR szElementId, IHTMLElement **pphtmlElement,IHTMLDocument2* pDoc)
{
	return GetElementInterface(szElementId, __uuidof(IHTMLElement), (void **) pphtmlElement,pDoc);
}

HRESULT CWeb1800AutoTesterDlg::GetElementInterface(LPCTSTR szElementId, REFIID riid, void **ppvObj,IHTMLDocument2* pDoc)
{
	ENSURE_ARG(ppvObj!=NULL);
	HRESULT hr = E_NOINTERFACE;
	*ppvObj = NULL;
	CComPtr<IDispatch> spdispElem;
	BOOL bCollect = FALSE;
	hr = GetElement(szElementId, &spdispElem,&bCollect,pDoc);

	if (spdispElem)
		hr = spdispElem->QueryInterface(riid, ppvObj);
	return hr;
}

HRESULT CWeb1800AutoTesterDlg::GetElement(LPCTSTR szElementId, IDispatch **ppdisp, BOOL *pbCollection /*= NULL*/,CComPtr<IHTMLDocument2> sphtmlDoc)
{
	ENSURE_ARG(ppdisp!=NULL);
	CComPtr<IHTMLElementCollection> sphtmlAll;
	CComPtr<IHTMLElementCollection> sphtmlColl;
	CComPtr<IDispatch> spdispElem;
	CComVariant varName;
	CComVariant varIndex;
	HRESULT hr = S_OK;
	//CComPtr<IHTMLDocument2> sphtmlDoc;

	*ppdisp = NULL;

	if(szElementId == NULL)
		return E_INVALIDARG;

	const CString strElementId(szElementId);
	varName.vt = VT_BSTR;
	varName.bstrVal = strElementId.AllocSysString();

#ifndef _UNICODE	
	if (!varName.bstrVal)
	{
		hr = E_OUTOFMEMORY;
		goto Error;
	}
#endif

	hr = sphtmlDoc->get_all(&sphtmlAll);
	if (sphtmlAll == NULL)
		goto Error;
	hr = sphtmlAll->item(varName, varIndex, &spdispElem);
	if (spdispElem == NULL)
	{
		hr = E_NOINTERFACE;
		goto Error;
	}

	spdispElem->QueryInterface(__uuidof(IHTMLElementCollection), (void **) &sphtmlColl);
	if (sphtmlColl)
	{
		if (pbCollection)
			*pbCollection = TRUE;
#ifdef _DEBUG
		else
		{
			TRACE(traceHtml, 0, "Warning: duplicate IDs or NAMEs.\n");
			ATLASSERT(FALSE);
		}
#endif

	}
Error:
	if (SUCCEEDED(hr))
	{
		*ppdisp = spdispElem;
		if (spdispElem)
			(*ppdisp)->AddRef();
	}
	return hr;
}

HWND CWeb1800AutoTesterDlg::GetExplorerHWND()
{
	HWND hwnd = ::FindWindow("IEFrame","网上客服中心 - Microsoft Internet Explorer");
	if (!hwnd)
	{
		AddToList("支持中心页面未找到");
		return NULL;
	}
	hwnd = g_find.GetChildWindow(hwnd,5);
	hwnd = g_find.GetChildWindow(hwnd,1);
	CString strShow = g_find.GetContrlClassName(hwnd);
	if (g_find.GetContrlClassName(hwnd) == "Internet Explorer_Server")
	{
		return hwnd;
	}
	return NULL;
}

void CWeb1800AutoTesterDlg::GetElementCollection(CString tagName,IHTMLElementCollection** pSpecialColl,IHTMLDocument2* pDoc)
{
	IHTMLElementCollection* pElemColl = NULL;
	IDispatch* pElemDisp = NULL;
	IHTMLElement* pElem=NULL;
	if (pDoc != NULL)
	{
		pDoc->get_all(&pElemColl);
		if (pElemColl != NULL)
		{
			_variant_t var(tagName); 
			pElemColl->tags(var,&pElemDisp);	//指定TAG集合
			if (pElemDisp != NULL)
			{
				*pSpecialColl = (IHTMLElementCollection*)pElemDisp;
				pElemColl->Release();
				return;
			}
			pElemColl->Release();
		}
	}
}
CString CWeb1800AutoTesterDlg::GetHtmlContentEx(IHTMLDocument2* pDoc)
{
	IHTMLElement* pelmBody = NULL;
	if (!pDoc)
	{
		return "";
	}
	pDoc->get_body(&pelmBody);
	if (!pelmBody)
	{
		pDoc->Release();
		return "";
	}
	BSTR bstrStr;
	pelmBody->get_innerHTML(&bstrStr);
	CString str(bstrStr);
	::SysFreeString(bstrStr);
	CFile file;
	file.Open("c:\\tessssss.txt",CFile::modeCreate|CFile::modeWrite);
	file.Write(str,str.GetLength());
	file.Close();	
	pelmBody->Release();		
	return str;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CWeb1800AutoTesterDlg::GetKeyString(CString &srcStr,CString &strBefore,CString &strAfter)
{
	int iPosBefore = 0;
	int iPosAfter = 0;
	iPosBefore = srcStr.Find(strBefore) + strBefore.GetLength();
	iPosAfter = srcStr.Find(strAfter,iPosBefore);
	if (iPosBefore < 0 || iPosAfter < 0 || iPosAfter - iPosBefore < 0 )
	{
		return "";
	}
	else
	{
		return srcStr.Mid(iPosBefore,iPosAfter - iPosBefore);
	}
}

void CWeb1800AutoTesterDlg::ThreadFunc(LPVOID lParam)
{
	CoInitialize(NULL);
	CWeb1800AutoTesterDlg *pParam = ((CWeb1800AutoTesterDlg *)lParam);
	if ( pParam )
	{
		pParam->RunFunc();
		pParam->AddToList("客户端线程退出");
		g_ExitFlagClient = true;
	}
	::CoUninitialize();
	pParam->GetDlgItem(IDC_BTN_STARTTEST)->EnableWindow(TRUE);
	pParam->GetDlgItem(IDC_BTN_STOPCLIENT)->EnableWindow(FALSE);
}

void CWeb1800AutoTesterDlg::ThreadFuncSp(LPVOID lParam)
{
	CoInitialize(NULL);
	CWeb1800AutoTesterDlg *pParam = ((CWeb1800AutoTesterDlg *)lParam);
	if ( pParam )
	{
		pParam->RunFuncSp();
		pParam->AddToListSp("支持端线程退出");
		g_ExitFlagServer = true;
	}
	::CoUninitialize();
	pParam->GetDlgItem(IDC_BTN_STARTSP)->EnableWindow(TRUE);
	pParam->GetDlgItem(IDC_BTN_STOPSP)->EnableWindow(FALSE);
}
void CWeb1800AutoTesterDlg::RunFuncSp()
{
	HWND hwnd = NULL;
	HWND hwndSession = NULL;
	CString strCustomerName = "";
	CString strTitle = "";
	bool bAccepted = false;
	AddToListSp("支持端线程启动");
	while(!g_ExitFlagServer)
	{
		if (::FindWindow("#32770","网上客服中心 Ver 3.0") == NULL)
		{
			AddToListSp("支持端程序未启动");
			Sleep(2000);
			continue;
		}
		strCustomerName = "";
		hwndSession = NULL;
		bAccepted = false;
		//查找是否有请求进来，如果有，则接受
		hwnd = ::FindWindow("#32770","服务请求");
		if (hwnd && ::IsWindow(hwnd) && ::IsWindowVisible(hwnd))
		{
			AddToListSp("有客户请求，接受");
			HWND hwndTemp = g_find.GetChildWindow(hwnd,6);
			CString strText = g_find.GetContrlWindowText(hwndTemp);
			if (strText.Find("请求得到远程服务")>0)
			{
				strCustomerName = strText.Left(strText.Find("请求得到远程服务"));
				AddToListSp("客户名称： "+strCustomerName);
			}			
			hwnd = g_find.GetChildWindow(hwnd,1);
			g_find.PostBtnClick(hwnd);
			::PostMessage(hwnd,WM_MOUSEACTIVATE,0,WM_LBUTTONDOWN);
			
 			::PostMessage(hwnd,WM_LBUTTONDOWN,MK_LBUTTON,MAKELONG(32,12));
 			Sleep(10);
 			::PostMessage(hwnd,WM_LBUTTONUP,0,MAKELONG(32,12));
			bAccepted = true;
			Sleep(1000);
		}
		if (!bAccepted)
		{
			AddToListSp("等待客户端发送请求");
			Sleep(1000);
			continue;
		}
		AddToListSp("等待5秒");
		Sleep(5000);
		int iCount = 0;
		strTitle.Format("%s - 网上客服中心 服务窗口",strCustomerName);
		while(!g_ExitFlagServer && hwndSession==NULL && iCount++ < 20)
		{
			hwndSession = ::FindWindow("#32770",strTitle);
			Sleep(500);
		}
		if(g_ExitFlagServer)
		{
			return;
		}
		if (!hwndSession)
		{
			Sleep(500);
			continue;
		}		
		AddToListSp("找到会话窗口");
		Sleep(500);
		//初始化按钮
		
		//客户已经退出。
		//如果提示客户已经退出
		
		
		HWND hwndCtrol = g_find.GetChildWindow(hwndSession,13);		
		if (::IsWindowEnabled(hwndCtrol))
		{
			g_find.PostBtnClick(hwndCtrol);
		}
		else
		{
			g_hwndWarn = NULL;
			EnumChildWindows(NULL,EnumChildProcWarn,NULL);
			if (g_hwndWarn && ::IsWindow(g_hwndWarn) && ::IsWindowVisible(g_hwndWarn))
			{
				::ShowWindow(::GetParent(g_hwndWarn),SW_HIDE);				
			}
			//关闭会话窗口
			::PostMessage(hwndSession,WM_CLOSE,0,0);
		}
		AddToListSp("发送远程控制请求");
		while(!g_ExitFlagServer)
		{
			//客户已经退出对话框
			//客户已经退出。
			//网上客服中心
			hwnd = ::FindWindow("#32770","提示信息");
			::PostMessage(hwnd,WM_CLOSE,0,0);	
			hwnd = ::FindWindow("#32770","网上客服中心");
			::PostMessage(hwnd,WM_CLOSE,0,0);
			//查找当前是否弹出了支持记录窗口，如果弹出，说明会话结束，退出
			hwnd = ::FindWindow("#32770","支持记录");
			if (hwnd && ::IsWindow(hwnd) && ::IsWindowVisible(hwnd))
			{
				AddToListSp("客户端关闭了会话，退出");
				hwnd = g_find.GetChildWindow(hwnd,15);
				g_find.PostBtnClick(hwnd);				
				break;
			}
			Sleep(1000);
		}

		Sleep(2000);
	}
}

void CWeb1800AutoTesterDlg::RunFunc()
{
	bool bRet = false;
	int iErrorCount = 0;
	int iErrorCountEx = 0;
	IHTMLDocument2 *pDoc = NULL;
	CString strUrl = "";
	GetDlgItemText(IDC_EDIT_URL,strUrl);
	while(!g_ExitFlagClient)
	{
		Sleep(500);
		if (g_hwndSessionWindow == NULL || !::IsWindow(g_hwndSessionWindow))
		{
			//结束进程
			//CFileUtil::
			//关闭所有的网页，再来
			HWND hwnd = ::FindWindow("IEFrame","网上客服中心 - Microsoft Internet Explorer");
			if (hwnd)
			{
				AddToList("关闭支持中心页面");
				::PostMessage(hwnd,WM_CLOSE,0,0);
				Sleep(1000);
			}
			//清空临时文件
			AddToList("卸载控件，清空临时文件");
			ClearFiles();
			AddToList("尝试打开支持中心页面");
			
			ShellExecute(NULL, "open", "IEXPLORE", strUrl, 0, SW_SHOWMAXIMIZED);
			//再打开支持中心页面
			iErrorCount = 0;
			while (!g_ExitFlagClient && iErrorCountEx++<20 && ::FindWindow("IEFrame","网上客服中心 - Microsoft Internet Explorer") == NULL)
			{
				Sleep(1000);
			}
			if (g_ExitFlagClient)
			{
				return;
			}
			Sleep(500);
			bRet = SendRequest();
			if (!bRet)
			{
				AddToList("暂停5秒");
				Sleep(5000);
				continue;
			}
			//查找会话窗口
			g_hwndSessionWindow = NULL;
			int iCount = 0;
			AddToList("开始查找会话窗口");
			while (!g_ExitFlagClient && !g_hwndSessionWindow && iCount++ < 30)
			{
				g_hwndSessionWindow = ::FindWindow("IEFrame","网上客服中心-服务窗口 - Microsoft Internet Explorer");
				Sleep(500);
			}
			if (g_ExitFlagClient)
			{
				return;
			}
			if (g_hwndSessionWindow == NULL)
			{
				AddToList("会话未启动");
				Sleep(500);
				continue;
			}
			AddToList("会话窗口启动成功");
			pDoc = NULL;
		}

		//会话已经进入，开始进行控制
		if (pDoc == NULL)
		{
			HWND hwnd = g_find.GetChildWindow(g_hwndSessionWindow,5);
			hwnd = g_find.GetChildWindow(hwnd,1);
			pDoc = GetDocInterface(hwnd);
			if (!pDoc)
			{
				iErrorCount++;
				if (iErrorCount > 5)
				{
					iErrorCount = 0;
					::PostMessage(g_hwndSessionWindow,WM_CLOSE,0,0);
					g_hwndSessionWindow = NULL;
					Sleep(500);
				}
				Sleep(1000);
				continue;;
			}
		}		
		//判断是否有接受框，如果有，则接受
		bRet = AcceptRemote(pDoc);
		if (bRet == false)
		{
			//未有接受对话框，等待继续
			AddToList("等待支持端发出请求");
			Sleep(1000);			
			continue;
		}
		Sleep(500);
		//接受后等待判断N秒后远程是否成功
		int iCount = 0;
		bool bStartSuc = false;
		CString strMsg = "";
		while (!g_ExitFlagClient && ::IsWindow(g_hwndSessionWindow) && iCount++ < 30)
		{
			ClickOcxInstall();
			HWND hwnd = ::FindWindow("#32770","提示信息");
			::PostMessage(hwnd,WM_CLOSE,0,0);	
			hwnd = ::FindWindow("#32770","网上客服中心");
			::PostMessage(hwnd,WM_CLOSE,0,0);
			//通过PDOC获取内容，比对标志字符串
			//得到会话内容，如果包含启动成功的字符串，则表示成功
			strMsg = GetInnerText(pDoc,"Layer1");
			if (strMsg.Find("启动成功")>0)
			{
				bStartSuc = true;
				break;
			}
			else if (strMsg.Find("失败") > 0)
			{
				bStartSuc = false;
				break;
			}
			Sleep(1000);
		}
		HWND hwnd = ::FindWindow("#32770","提示信息");
		::PostMessage(hwnd,WM_CLOSE,0,0);	
		hwnd = ::FindWindow("#32770","网上客服中心");
		::PostMessage(hwnd,WM_CLOSE,0,0);
		if (g_ExitFlagClient)
		{
			return;
		}
		if (!::IsWindow(g_hwndSessionWindow))
		{
			AddToList("会话已关闭，重置");
			g_hwndSessionWindow = NULL;
		}
		//如果远程启动成功，则关闭
		if (bStartSuc)
		{
			AddToList("远程启动成功，退出会话");
		}
		else
		{
			//记录日志信息
			AddToList("远程启动失败，退出会话");
			AddToList(strMsg);			
			//记录信息
		}
		//退出会话
		StopSession(pDoc);
		//检测弹出的警告框，点击“是”
		//等待一段时间
		Sleep(3000);
		//关闭会话窗口，如果存在的话
		if (::IsWindow(g_hwndSessionWindow))
		{
			AddToList("关闭会话窗口");
			::PostMessage(g_hwndSessionWindow,WM_CLOSE,0,0);
			g_hwndSessionWindow = NULL;
		}
		Sleep(3000);
	}
}

CString CWeb1800AutoTesterDlg::GetInnerText(IHTMLDocument2 *pDoc,CString strId)
{
	if (pDoc == NULL)
	{
		return "";
	}
	IHTMLElement *pElement = NULL;
	GetElement(strId,&pElement,pDoc);
	if (pElement == NULL)
	{
		return "";
	}

	BSTR bstrStr;	
	pElement->get_innerText(&bstrStr);	
	CString str((char*)(_bstr_t)bstrStr);
	::SysFreeString(bstrStr);
	pElement->Release();	
	return str;
}

bool CWeb1800AutoTesterDlg::PostWebClick(IHTMLDocument2 *pDoc,CString strId)
{
	if (pDoc == NULL)
	{
		return false;
	}
	IHTMLElement *pElement = NULL;
	GetElement(strId,&pElement,pDoc);
	if (pElement == NULL)
	{
		return false;
	}	
	pElement->click();
	pElement->Release();
	return true;
}

bool CWeb1800AutoTesterDlg::StopSession(IHTMLDocument2 *pDoc)
{
	bool bRet = PostWebClick(pDoc,"close");
	if (bRet)
	{
		AddToList("停止会话");
	}
	return bRet;
}
bool CWeb1800AutoTesterDlg::AcceptRemote(IHTMLDocument2 *pDoc)
{
	bool bRet = PostWebClick(pDoc,"accept");
	if (bRet)
	{
		AddToList("接收支持端请求");
	}
	return bRet;
}
void CWeb1800AutoTesterDlg::OnBnClickedButton2()
{
	if (g_hwndSessionWindow == NULL)
	{
		g_hwndSessionWindow = ::FindWindow("IEFrame","网上客服中心-服务窗口 - Microsoft Internet Explorer");		
	}
	HWND hwnd = g_find.GetChildWindow(g_hwndSessionWindow,5);
	hwnd = g_find.GetChildWindow(hwnd,1);
	IHTMLDocument2 *pDoc = GetDocInterface(hwnd);
	AcceptRemote(pDoc);
	pDoc->Release();
	

}

void CWeb1800AutoTesterDlg::ClearFiles()
{
	CString strpath = CFileUtil::GetUserPath();
	strpath = strpath;
	//strpath = "c:\\test\\test.txt\0";
	char szPath[256] = {0};
	//卸载控件
	//
	RegistOcx("C:\\WINDOWS\\system32\\WebI800.dll",false);
	RegistOcx(strpath+"\\WebI800\\WebI800.dll",false);
	Sleep(50);
	CFileFind fileFinder;
	//strpath += "\\*.*";

	//清除临时文件
	BOOL bRes = fileFinder.FindFile(strpath + "\\*.*");
	while(bRes)
	{
		bRes = fileFinder.FindNextFile();
		CString strPath = fileFinder.GetFileName();
		if(fileFinder.IsDirectory() && !fileFinder.IsDots())
		{			
			if ( strPath == "FirstFolder" || strPath.Find("Web1800V")>=0 || strPath == "WebI800" || strPath == "SPScreenShot" || strPath == "EUScreenShot")
			{
				try
				{
					memset(szPath,0,256);
					sprintf(szPath,"%s\\%s",strpath,strPath);
					SHFILEOPSTRUCT FileOp={0};
					FileOp.fFlags = FOF_ALLOWUNDO |   //允许放回回收站
					FOF_NOCONFIRMATION; //不出现确认对话框
					FileOp.pFrom = szPath;
					FileOp.pTo = NULL;      //一定要是NULL
					FileOp.wFunc = FO_DELETE;//删除操作
					SHFileOperation(&FileOp);
				}				
				catch (...)
				{
				}
			}
		}
	}
	
}
void CWeb1800AutoTesterDlg::OnBnClickedButton3()
{
	ClearFiles();
}

void CWeb1800AutoTesterDlg::OnBnClickedButton4()
{
}

bool CWeb1800AutoTesterDlg::RegistOcx(LPCTSTR strFileName, bool bInstall)
{
	try
	{
		if(strFileName == NULL)
			return false;

		HINSTANCE hInstance = ::LoadLibrary(strFileName);
		if(hInstance < (HINSTANCE)HINSTANCE_ERROR)
		{
			return false;
		}
		

		FARPROC lpDllEntryPoint;
		if(bInstall)
			lpDllEntryPoint = GetProcAddress(hInstance, _T("DllRegisterServer"));
		else
			lpDllEntryPoint = GetProcAddress(hInstance, _T("DllUnregisterServer"));

		if(lpDllEntryPoint != NULL)   
		{   
			if(FAILED((*lpDllEntryPoint)()))   
			{
				FreeLibrary(hInstance);
				return false;
			}
			FreeLibrary(hInstance);
			return true;
		}
		else
		{
			return false;
		}
	}
	catch(...)
	{
		return false;
	}
	return true;
}
void CWeb1800AutoTesterDlg::ClickOcxInstall()
{
	HWND hwnd = ::FindWindow("#32770","Internet Explorer - 安全警告");
	hwnd = g_find.GetChildWindow(hwnd,16);
	if (hwnd)
	{
		g_find.PostBtnClick(hwnd);
	}
	
}

void CWeb1800AutoTesterDlg::OnBnClickedBtnStopclient()
{
	g_ExitFlagClient = true;
	GetDlgItem(IDC_BTN_STOPCLIENT)->EnableWindow(FALSE);
}

void CWeb1800AutoTesterDlg::OnBnClickedBtnStopsp()
{
	g_ExitFlagServer = true;
	GetDlgItem(IDC_BTN_STOPSP)->EnableWindow(FALSE);
}
