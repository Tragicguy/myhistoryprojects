// BrowserHtml.cpp : 实现文件
//

#include "stdafx.h"
#include "DocHostSite.h"
#include "Web1800HtmlView.h"
#include   <afxpriv2.h> 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CWeb1800HtmlView, CHtmlView)

CWeb1800HtmlView::CWeb1800HtmlView()
{
	EnableAutomation();
	m_DefaultMsgBoxTitle="";
	SetContextMenuMode (TextSelectionOnly);

	m_bAutoRefresh = false;
	m_nAutoRefreshTimeout = 60;
	m_strOldUrl = _T("");

	m_pDispDoc = NULL;
	m_pEventHandler = new SDocEvtHandler();
	m_pEventHandler->m_pOnClickCallback = &m_OnClickCallback;
	m_dwDocCookie = 0;
}

CWeb1800HtmlView::~CWeb1800HtmlView()
{
}

void CWeb1800HtmlView::SetContextMenuMode(CONTEXT_MENU_MODE MenuMode)
{
	m_ContextMenuMode=MenuMode;
}


HRESULT CWeb1800HtmlView::OnGetExternal(LPDISPATCH *lppDispatch) 
{ 
	*lppDispatch = GetIDispatch(TRUE);
	return S_OK; 
}

BOOL CWeb1800HtmlView::CreateHtmlView(UINT nID, CWnd *pParent)
{
	CStatic wndStatic;
	if (!wndStatic.SubclassDlgItem(nID, pParent)) return FALSE;
	
	m_pParent = pParent;
	wndStatic.DestroyWindow();	
	return Create(NULL, NULL, WS_CHILD | WS_BORDER | WS_VISIBLE, CRect(0, 0, 0, 0), pParent, nID, NULL);
}

void CWeb1800HtmlView::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWeb1800HtmlView, CHtmlView)
	ON_WM_DESTROY()
	ON_WM_MOUSEACTIVATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CWeb1800HtmlView, CHtmlView)
	DISP_FUNCTION(CWeb1800HtmlView, "Open_Page", Open_Page, VTS_NONE, VTS_BSTR)
END_DISPATCH_MAP()

//modify by shaojun at 2010-09-03 屏蔽此代码，解决浏览器卡死问题
BOOL CWeb1800HtmlView::CreateControlSite(COleControlContainer * pContainer, COleControlSite ** ppSite, UINT /*nID*/, REFCLSID /*clsid*/)
{
	*ppSite = new CDocHostSite(pContainer, this);// 创建自己的控制站点实例
	return (*ppSite) ? TRUE : FALSE;
}

void CWeb1800HtmlView::SetAlertTitle(WCHAR* strTitle)
{
	wcscpy(m_strAlertTitle, strTitle); 
}

HRESULT CWeb1800HtmlView::OnShowMessage(HWND hwnd,LPOLESTR lpstrText,LPOLESTR lpstrCaption,DWORD dwType,LPOLESTR lpstrHelpFile,DWORD dwHelpContext,LRESULT * plResult)
{
    // 窗口标题"Microsoft Internet Explorer"的资源标识
	#define IDS_MESSAGE_BOX_TITLE 2213
    //载入Shdoclc.dll 和IE消息框标题字符串
    HINSTANCE hinstSHDOCLC = LoadLibrary(TEXT("SHDOCLC.DLL"));
    if (hinstSHDOCLC == NULL)
        return S_FALSE;

	CString strBuf,strCaption(lpstrCaption);
	strBuf.LoadString(hinstSHDOCLC, IDS_MESSAGE_BOX_TITLE);

    // 比较IE消息框标题字符串和lpstrCaption
    // 如果相同，用自定义标题替换
	if(strBuf==lpstrCaption)
        strCaption = m_DefaultMsgBoxTitle;

    // 创建自己的消息框并且显示
    //*plResult = MessageBox(CString(lpstrText), strCaption, dwType);
	*plResult = MessageBoxW (this->m_hWnd, lpstrText, L"提示", dwType); 

    //卸载Shdoclc.dll并且返回
    FreeLibrary(hinstSHDOCLC);

	return S_OK;
}
//通过添加命令来截获部分事件
BOOL CWeb1800HtmlView::OnCmdMsg(UINT   nID,   int   nCode,   void*   pExtra,   AFX_CMDHANDLERINFO*   pHandlerInfo)  
{
	if(nCode!=CN_EVENT   ||   pExtra==NULL)  
		return   CHtmlView::OnCmdMsg(nID,   nCode,   pExtra,   pHandlerInfo);
	AFX_EVENT*pae=(AFX_EVENT*)pExtra;

	switch(pae-> m_dispid)
	{
	case   270:   //文件下载,   因为此id没有正式文档,   所以下面判断特别小心.
		if(pae-> m_pDispParams!=NULL)
			if(pae-> m_pDispParams-> cArgs==2)
				if(pae-> m_pDispParams-> rgvarg-> vt==(VT_BOOL|VT_BYREF)   &&  
					(pae-> m_pDispParams-> rgvarg+1)-> vt==VT_BOOL)
				{
					VARIANT_BOOL   bePage=(pae-> m_pDispParams-> rgvarg+1)-> boolVal;
					if(bePage==VARIANT_FALSE)//如果不是打开页面
					{

// 						IHTMLDocument2*   pDocument ;//= (IHTMLDocument2*)this->GetDocument();
// 						//HRESULT   hr   =   GetDHtmlDocument(&pDocument);   
// 						m_pBrowserApp->get_Document((IDispatch **)&pDocument);
// 						IHTMLWindow2*   pWindow;
// 						pDocument->get_parentWindow(&pWindow);   
// 						VARIANT  ret;   
// 						CString strScript = "";	
// 
// 						
// 						strScript= "alert('aaaa');document.execCommand('saveas',);";
// 						strScript.Format("var winname = window.open('http://localhost/WebSite/svn.rar');");
// 						//strScript += "winname.document.execCommand('saveas','','');"; 
// 						//strScript += "winname.close();";
// 						BSTR bstrStr = strScript.AllocSysString();
// 						pWindow->execScript(bstrStr,L"javascript",&ret);
// 						::SysFreeString(bstrStr);
// 						pWindow->Release();
// 						pDocument->Release();
// 						*(pae-> m_pDispParams-> rgvarg-> pboolVal)=VARIANT_TRUE;//取消下载
// 						return(TRUE);
					}
				}
				break;
	case 273:
		AfxMessageBox("new window");
		break;
	default:
		break;
	}
	return   CHtmlView::OnCmdMsg(nID,   nCode,   pExtra,   pHandlerInfo);
} 

// In the .cpp file.
STDMETHODIMP CWeb1800HtmlView::Download(IMoniker* pmk,
								 IBindCtx* pbc,
								 DWORD dwBindVerb,
								 LONG grfBINDF,
								 BINDINFO* pBindInfo,
								 LPCOLESTR pszHeaders,
								 LPCOLESTR pszRedir,
								 UINT uiCP )
{
	::MessageBox(NULL,"Download","Download Manager",MB_OK);
	return S_OK; 
}


HRESULT CWeb1800HtmlView::ExecJavascript(CString   strJsFunctionName_In,   const   CArray <VARIANT,   VARIANT& >   &pArrVARIANT_Params) 
{ 
	//前提是你已经取得了IHTMLDocument2 
	//(1)Get   Script
	/*IHTMLDocument2*   pDocument;

	GetDHtmlDocument(&pDocument);
	CComPtr <IDispatch >   pScript; 
	HRESULT hResult;
	hResult  =pDocument->get_Script(&pScript); 
	if(FAILED(hResult)) 
	{ 
		return   FALSE; 
	}
	//(2)Get   Javascript   Function 
	CComBSTR   pCComBSTR(strJsFunctionName_In); 
	DISPID   pDISPID; 
	hResult   =   pScript->GetIDsOfNames(IID_NULL,   &pCComBSTR,   1,   LOCALE_SYSTEM_DEFAULT,   &pDISPID); 
	if(FAILED(hResult)) 
	{ 
		return   FALSE; 
	}
	//(3)Add   Parameters 
	DISPPARAMS   pDISPPARAMS; 
	memset(&pDISPPARAMS,0,sizeof(pDISPPARAMS));
	int   intArgsCount_In   =   pArrVARIANT_Params.GetSize();
	VARIANT*   vParams   =   new   VARIANT[intArgsCount_In];
	for(int i = 0;i < intArgsCount_In; i++)
	{
		vParams[i]   =   pArrVARIANT_Params.GetAt(i);
	}
	//Params   Count 
	pDISPPARAMS.cArgs   =   intArgsCount_In; 
	//Params   Array 
	pDISPPARAMS.rgvarg   =   vParams; 
	//Name   Args   Count 
	pDISPPARAMS.cNamedArgs   =   0; 

	//(4)Invoke   Javascript   Method 
	EXCEPINFO   pEXCEPINFO; 
	memset(&pEXCEPINFO,0,sizeof(pEXCEPINFO));
	CComVariant pCComVariant;
	//Initialize   to   invalid   arg 
	UINT   nintArgErr   =   (UINT)-1;
	hResult = pScript->Invoke(pDISPID,IID_NULL,0,DISPATCH_METHOD,&pDISPPARAMS,&pCComVariant,&pEXCEPINFO,   &nintArgErr); 
	delete[]   pDISPPARAMS.rgvarg; 
	pScript.Release();
	CString strTemp = CString(pCComVariant.bstrVal);
	AfxMessageBox(strTemp);
	if(FAILED(hResult)) 
	{ 
		return FALSE; 
	}*/
	return TRUE; 
} 
HRESULT CWeb1800HtmlView::OnShowContextMenu(DWORD dwID, LPPOINT ppt,LPUNKNOWN pcmdtReserved, LPDISPATCH pdispReserved)
{
	HRESULT result = S_FALSE;

	switch(m_ContextMenuMode)
	{
	case NoContextMenu:				// 无菜单
		result=S_OK;
		break;
	case DefaultMenu:				// 默认菜单
		break;
	case TextSelectionOnly:			// 仅文本选择菜单
		if(!(dwID == CONTEXT_MENU_TEXTSELECT || dwID == CONTEXT_MENU_CONTROL))
			result=S_OK;
		break;
	case CustomMenu:				// 自定义菜单
		//if(dwID!=CONTEXT_MENU_TEXTSELECT)
		//result=OnShowCustomContextMenu(ppt,pcmdtReserved,pdispReserved);
		break;
	}

	return result;
}


BEGIN_EVENTSINK_MAP(CWeb1800HtmlView, CHtmlView)
    //{{AFX_EVENTSINK_MAP(CWeb1800HtmlView)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CWeb1800HtmlView::PreCreateWindow(CREATESTRUCT& cs)
{
	//cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.style = (cs.style & ~WS_BORDER) | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	
	return CHtmlView::PreCreateWindow(cs);
}

void CWeb1800HtmlView::OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel)
{
	UninstallEventHandler();

	if ( GetAsyncKeyState(VK_BACK) & 0x8000 )
		*pbCancel = TRUE;
	else if (m_OnBeforeNavigateCallback.IsEnableCallback())
	{
		*pbCancel = m_OnBeforeNavigateCallback.call(CString(lpszURL));
		if (!(*pbCancel))
		{
			CHtmlView::OnBeforeNavigate2(lpszURL, nFlags, lpszTargetFrameName, baPostedData, lpszHeaders, pbCancel);
		}
	}
	else
		CHtmlView::OnBeforeNavigate2(lpszURL, nFlags, lpszTargetFrameName, baPostedData, lpszHeaders, pbCancel);
}

void CWeb1800HtmlView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy); 
	if (::IsWindow(m_wndBrowser.m_hWnd))
	{
		CRect rect;
		GetClientRect(rect);

		::AdjustWindowRectEx(rect, GetStyle(), FALSE, WS_EX_CLIENTEDGE);
		m_wndBrowser.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	}
}

void WriteLog(char * szInfo)
{
	CTime currTime = CTime::GetCurrentTime();
	try
	{
		CString filepath = "";
		CString destStr = "";
		CString folder = "";
		CString filename = "test.log";
		CString strAppPath = "";		
		CString folderName = "";
		folderName.Format("%s","c:\\");
		char szDestInfo[3000] = {0};
		sprintf_s(szDestInfo,"%s	%s",currTime.Format("%Y-%m-%d %H:%M:%S"),szInfo);
		//destStr.Format("%s	%s",currTime.Format("%Y-%m-%d %H:%M:%S"),szInfo);
		filepath.Format("%s%s",folderName,filename);
		FILE *fp;
		char szfile[1000]={0};
		sprintf(szfile,filepath);
		fp=fopen(szfile,"a+");
		fprintf(fp,"%s\r\n",szDestInfo);
		fclose(fp);
	}
	catch (...)
	{
	}

}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//modify by shaojun at 2010-08-06 保证能传递比较长的URL地址
void CWeb1800HtmlView::Open_Page(LPCTSTR strUrl)
{
	if (strUrl != NULL)
	{
		ShellExecute(NULL, "open", strUrl, NULL, 0, SW_SHOWMAXIMIZED);		
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			ShellExecute(NULL, "open", "IEXPLORE", strUrl, 0, SW_SHOWMAXIMIZED);
		}
	}	
}

void CWeb1800HtmlView::OnDocumentComplete(LPCTSTR lpszURL)
{
	if (m_bAutoRefresh)
		KillTimer(NAVIGATE_TIMERID);

	if (m_OnDocumentCompleteCallback.IsEnableCallback())
		m_OnDocumentCompleteCallback.call(CString(lpszURL));

	InstallEventHandler();
	CHtmlView::OnDocumentComplete(lpszURL);
}

void CWeb1800HtmlView::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == NAVIGATE_TIMERID)
	{
		if (!m_strOldUrl.IsEmpty())
			this->Navigate(m_strOldUrl);
	}
	
	CHtmlView::OnTimer(nIDEvent);
}

void CWeb1800HtmlView::Navigate(LPCTSTR URL, DWORD dwFlags/* = 0*/,
		LPCTSTR lpszTargetFrameName/* = NULL*/,
		LPCTSTR lpszHeaders/* = NULL*/, LPVOID lpvPostData/* = NULL*/,
		DWORD dwPostDataLen/* = 0*/)
{
	if (m_bAutoRefresh)
	{
		KillTimer(NAVIGATE_TIMERID);
		SetTimer(NAVIGATE_TIMERID, m_nAutoRefreshTimeout * 1000, NULL);

		m_strOldUrl = URL;
	}

	CHtmlView::Navigate(URL, dwFlags, lpszTargetFrameName, lpszHeaders, lpvPostData, dwPostDataLen);
}


// 安装响应函数。省略了一些失败判断以突出主要步骤
void CWeb1800HtmlView::InstallEventHandler()
{
	try
	{
		if(m_dwDocCookie)   // 已安装，卸载先。最后一次安装的才有效
			UninstallEventHandler();

		m_pDispDoc = GetHtmlDocument();
		IConnectionPointContainerPtr pCPC = m_pDispDoc;
		IConnectionPointPtr pCP;

		// 找到安装点
		pCPC->FindConnectionPoint(DIID_HTMLDocumentEvents2, &pCP);
		IUnknown* pUnk = m_pEventHandler->GetInterface(&IID_IUnknown);

		//安装
		HRESULT hr = pCP->Advise(pUnk, &m_dwDocCookie);

		if(!SUCCEEDED(hr))  // 安装失败
			m_dwDocCookie = 0;
	}
	catch(...){}
}

// 卸载响应函数。省略了一些失败判断以突出主要步骤
void CWeb1800HtmlView::UninstallEventHandler()
{
	try
	{
		if(0 == m_dwDocCookie) return;

		IConnectionPointContainerPtr pCPC = m_pDispDoc;
		IConnectionPointPtr pCP;
		pCPC->FindConnectionPoint(DIID_HTMLDocumentEvents2, &pCP);
		HRESULT hr = pCP->Unadvise(m_dwDocCookie);
	}
	catch(...){}
}

void CWeb1800HtmlView::OnDestroy()
{
	UninstallEventHandler();
	CHtmlView::OnDestroy();
}



