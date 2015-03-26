// BrowserHtml.cpp : 实现文件
//

#include "stdafx.h"
#include "WebUserHtmlView.h"
#include <MsHtmdid.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CUserHtmlView, CHtmlView)

CUserHtmlView::CUserHtmlView()
{
	EnableAutomation();
	m_DefaultMsgBoxTitle="";
	SetContextMenuMode (TextSelectionOnly);

	m_bAutoRefresh = false;
	m_nAutoRefreshTimeout = 60;
	m_strOldUrl = _T("");

	m_pDispDoc = NULL;
	m_dwDocCookie = 0;
	m_pParent = NULL;
}

CUserHtmlView::~CUserHtmlView()
{

}

void CUserHtmlView::SetContextMenuMode(CONTEXT_MENU_MODE MenuMode)
{
	m_ContextMenuMode=MenuMode;
}


HRESULT CUserHtmlView::OnGetExternal(LPDISPATCH *lppDispatch) 
{ 
	*lppDispatch = GetIDispatch(TRUE);
	return S_OK; 
}

BOOL CUserHtmlView::CreateHtmlView(UINT nID, CWnd *pParent)
{
	CStatic wndStatic;
	if (!wndStatic.SubclassDlgItem(nID, pParent)) return FALSE;
	
	m_pParent = pParent;
	wndStatic.DestroyWindow();

	return Create(NULL, NULL, WS_CHILD | WS_BORDER | WS_VISIBLE, CRect(0, 0, 0, 0), pParent, nID, NULL);
}

void CUserHtmlView::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUserHtmlView, CHtmlView)
	ON_WM_DESTROY()
	ON_WM_MOUSEACTIVATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CUserHtmlView, CHtmlView)
	DISP_FUNCTION(CUserHtmlView, "Open_Page", Open_Page, VTS_NONE, VTS_BSTR)
	DISP_FUNCTION(CUserHtmlView, "OpenWebWindow", OpenWebWindow, VTS_NONE, VTS_BSTR)
	DISP_FUNCTION(CUserHtmlView, "FlashMainWindow", FlashMainWindow, VTS_NONE, VTS_NONE)
END_DISPATCH_MAP()
BEGIN_EVENTSINK_MAP(CUserHtmlView, CHtmlView)
	ON_EVENT(CUserHtmlView, AFX_IDW_PANE_FIRST,273,OnNewWindow3,VTS_PDISPATCH VTS_PBOOL VTS_I4 VTS_BSTR VTS_BSTR)
END_EVENTSINK_MAP()
//modify by shaojun at 2010-09-03 屏蔽此代码，解决浏览器卡死问题
// BOOL CUserHtmlView::CreateControlSite(COleControlContainer * pContainer, COleControlSite ** ppSite, UINT /*nID*/, REFCLSID /*clsid*/)
// {
// 	*ppSite = new CDocHostSite(pContainer, this);// 创建自己的控制站点实例
// 	return (*ppSite) ? TRUE : FALSE;
// }

void CUserHtmlView::OnNewWindow3(      
				IDispatch *ppDisp,
				VARIANT_BOOL *Cancel,
				DWORD dwFlags,
				LPCTSTR bstrUrlContext,
				LPCTSTR bstrUrl
				)
{
	int iii = 0;
// 	if (m_OnNewWindow3Callback.IsEnableCallback())
// 	{
// 		m_OnNewWindow3Callback.call(bstrUrl);
// 	}
}
void CUserHtmlView::SetAlertTitle(WCHAR* strTitle)
{
	wcscpy(m_strAlertTitle, strTitle); 
}

HRESULT CUserHtmlView::OnShowMessage(HWND hwnd,LPOLESTR lpstrText,LPOLESTR lpstrCaption,DWORD dwType,LPOLESTR lpstrHelpFile,DWORD dwHelpContext,LRESULT * plResult)
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
	*plResult = ::MessageBoxW (this->m_hWnd, lpstrText, L"提示", dwType); 

    //卸载Shdoclc.dll并且返回
    FreeLibrary(hinstSHDOCLC);

	return S_OK;
}

HRESULT CUserHtmlView::OnShowContextMenu(DWORD dwID, LPPOINT ppt,LPUNKNOWN pcmdtReserved, LPDISPATCH pdispReserved)
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

BOOL CUserHtmlView::PreCreateWindow(CREATESTRUCT& cs)
{
	//cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.style = (cs.style & ~WS_BORDER) | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	return CHtmlView::PreCreateWindow(cs);
}

void CUserHtmlView::OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel)
{
	//UninstallEventHandler();

	if ( GetAsyncKeyState(VK_BACK) & 0x8000 )
		*pbCancel = TRUE;
// 	else if (m_OnBeforeNavigateCallback.IsEnableCallback())
// 	{
// 		*pbCancel = m_OnBeforeNavigateCallback.call(CString(lpszURL));
// 		if (!(*pbCancel))
// 		{
// 			CHtmlView::OnBeforeNavigate2(lpszURL, nFlags, lpszTargetFrameName, baPostedData, lpszHeaders, pbCancel);
// 		}
// 	}
 	else
		CHtmlView::OnBeforeNavigate2(lpszURL, nFlags, lpszTargetFrameName, baPostedData, lpszHeaders, pbCancel);
}

void CUserHtmlView::OnSize(UINT nType, int cx, int cy)
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
/*
void WriteLog(char * szInfo)
{
	CTime currTime = CTime::GetCurrentTime();
	try
	{
		CString filepath = "";
		CString destStr = "";
		CString folder = "";
		CString filename = "test.dat";
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

}*/
/************************************************************************/
/*                                                                      */
/************************************************************************/
//modify by shaojun at 2010-08-06 保证能传递比较长的URL地址
void CUserHtmlView::Open_Page(LPCTSTR strUrl)
{
	if (strUrl != NULL)
	{
		ShellExecute(NULL, _T("open"), strUrl, NULL, 0, SW_SHOWMAXIMIZED);		
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			ShellExecute(NULL, _T("open"), _T("IEXPLORE"), strUrl, 0, SW_SHOWMAXIMIZED);
		}
	}	
}


void CUserHtmlView::OpenWebWindow(LPCTSTR strUrl)
{
	if (strUrl != NULL)
	{
		CWnd* pParent = this->GetParent();
		if (pParent && IsWindow(pParent->GetSafeHwnd()))
		{
			//父窗口处理是记住delete CString
			//pParent->PostMessage(MYWM_OPENWEBWINDOW, 0, (LPARAM)new CString(strUrl));
		}
	}
}

void CUserHtmlView::FlashMainWindow()
{
	CWnd* pParent = this->GetParent();
	if (pParent && IsWindow(pParent->GetSafeHwnd()))
		pParent->FlashWindow(FALSE);
}

void CUserHtmlView::OnDocumentComplete(LPCTSTR lpszURL)
{
	if (m_bAutoRefresh)
		KillTimer(NAVIGATE_TIMERID);
	
	//InstallEventHandler();
	CHtmlView::OnDocumentComplete(lpszURL);
}

void CUserHtmlView::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == NAVIGATE_TIMERID)
	{
		if (!m_strOldUrl.IsEmpty())
			this->Navigate(m_strOldUrl);
	}
	
	CHtmlView::OnTimer(nIDEvent);
}

void CUserHtmlView::Navigate(LPCTSTR URL, DWORD dwFlags/* = 0*/,
		LPCTSTR lpszTargetFrameName/* = NULL*/,
		LPCTSTR lpszHeaders/* = NULL*/, LPVOID lpvPostData/* = NULL*/,
		DWORD dwPostDataLen/* = 0*/)
{
#ifdef DISABLE_HTML_NAVIGATE
	return;
#endif
	if (m_bAutoRefresh)
	{
		KillTimer(NAVIGATE_TIMERID);
		SetTimer(NAVIGATE_TIMERID, m_nAutoRefreshTimeout * 1000, NULL);

		m_strOldUrl = URL;
	}

	CHtmlView::Navigate(URL, dwFlags, lpszTargetFrameName, lpszHeaders, lpvPostData, dwPostDataLen);
}


void CUserHtmlView::OnDestroy()
{
	//UninstallEventHandler();
	CHtmlView::OnDestroy();
}

BOOL CUserHtmlView::OnAmbientProperty(COleControlSite* pSite, DISPID dispid, VARIANT* pvar)
{ 
	// TODO: Add your specialized code here and/or call the base class 
	if (dispid == DISPID_AMBIENT_DLCONTROL) 
	{
		pvar->vt = VT_I4; 
		pvar->lVal = pvar->lVal & ~DLCTL_DLIMAGES; 
		return TRUE;
	} 
	return CHtmlView::OnAmbientProperty(pSite, dispid, pvar); 
} 