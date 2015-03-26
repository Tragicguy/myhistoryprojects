// BrowserHtml.cpp : ????
//

#include "stdafx.h"
#include "BrowserHtml.h"
//***add by panpeng 2008-10-20 for the MYWM_OPENVIDEODIR message
//end***

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CBrowserHtml

IMPLEMENT_DYNCREATE(CBrowserHtml, CWeb1800HtmlView)

CBrowserHtml::CBrowserHtml()
{
	m_strSupporterId = _T("");
	EnableAutomation();
	m_nCount = 0;
	m_bLoading = false;
}

CBrowserHtml::~CBrowserHtml()
{
	DeletePointers();
}

void CBrowserHtml::OnFinalRelease()
{
	CWeb1800HtmlView::OnFinalRelease();
}

BSTR CBrowserHtml::GetSupporterId()
{
	return m_strSupporterId.AllocSysString();
}

BOOL CBrowserHtml::OpenSubWebPage(LPCTSTR strWebAddress)
{
	m_strWebAddress = strWebAddress;
	if (m_pParent)
	{
		m_pParent->PostMessage(ENABLE_HTML);
	}

	return TRUE;
}

void CBrowserHtml::ShowStartupInfo(BOOL bLogined)
{

}

void CBrowserHtml::CloseStartupWindow()
{
	if (m_pParent != NULL)
	{
		m_pParent->PostMessage(WELCOMEPAGE_EXIT);
	}
}

void CBrowserHtml::SetParentWnd(CWnd *pParent)
{
	m_pParent = pParent;
}
BOOL CBrowserHtml::CreateHtmlView(UINT nID, CWnd *pParent)
{
	CStatic wndStatic;
	if (!wndStatic.SubclassDlgItem(nID, pParent)) return FALSE;
	
	m_pParent = pParent;
	wndStatic.DestroyWindow();
	return Create(NULL, NULL, WS_CHILD | WS_BORDER | WS_VISIBLE, CRect(0, 0, 0, 0), pParent, nID, NULL);
}

BEGIN_MESSAGE_MAP(CBrowserHtml, CWeb1800HtmlView)
	ON_WM_DESTROY()
	ON_WM_MOUSEACTIVATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CBrowserHtml, CWeb1800HtmlView)
	DISP_FUNCTION(CBrowserHtml, "GetSupporterId", GetSupporterId, VT_BSTR, VTS_NONE) 
	DISP_FUNCTION(CBrowserHtml, "OpenSubWebPage", OpenSubWebPage, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CBrowserHtml, "ShowStartupInfo", ShowStartupInfo, VTS_NONE, VTS_BOOL)
	DISP_FUNCTION(CBrowserHtml, "CloseStartupWindow", CloseStartupWindow , VTS_NONE, VTS_NONE)
	DISP_FUNCTION(CBrowserHtml, "Open_Page", Open_Page, VTS_NONE, VTS_BSTR)
	DISP_FUNCTION(CBrowserHtml, "OpenVideoDirectory", OpenVideoDirectory, VTS_NONE, VTS_BSTR)
	DISP_FUNCTION(CBrowserHtml, "OpenAudioDirectory", OpenAudioDirectory, VTS_NONE, VTS_BSTR)
	DISP_FUNCTION(CBrowserHtml, "GetSupporterStatus", GetSupporterStatus, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CBrowserHtml, "ShowScreenCapture", ShowScreenCapture, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CBrowserHtml, "SaveScreenCapture", SaveScreenCapture, VT_BSTR, VTS_BSTR)
END_DISPATCH_MAP()

void CBrowserHtml::OnCommandStateChange(long nCommand, BOOL bEnable)
{
	switch (nCommand)
	{
	case CSC_NAVIGATEFORWARD:
		m_pParent->PostMessage(ENABLE_FORWARD, (WPARAM)bEnable);
		break;

	case CSC_NAVIGATEBACK:
		m_pParent->PostMessage(ENABLE_BACK, (WPARAM)bEnable);
		break;

	default:
		break;
	}

	CWeb1800HtmlView::OnCommandStateChange(nCommand, bEnable);
}

BEGIN_EVENTSINK_MAP(CBrowserHtml, CWeb1800HtmlView)
   // {{AFX_EVENTSINK_MAP(CBrowserHtml)
		//ON_EVENT(CBrowserHtml, IDC_STATIC_HTMLVIEW, 505 /* CommandStateChange */, OnCommandStateChange, VTS_I4 VTS_BOOL)
		ON_EVENT(CBrowserHtml, AFX_IDW_PANE_FIRST, 273, NewWindow3, VTS_PDISPATCH VTS_PBOOL VTS_I4 VTS_BSTR VTS_BSTR)
	//}}AFX_EVENTSINK_MAP
	

END_EVENTSINK_MAP()

BOOL CBrowserHtml::PreCreateWindow(CREATESTRUCT& cs)
{
	//cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.style = (cs.style & ~WS_BORDER) | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	return CWeb1800HtmlView::PreCreateWindow(cs);
}
void CBrowserHtml::NewWindow3(   
				IDispatch **ppDisp,  
				VARIANT_BOOL *Cancel,  
				DWORD dwFlags,  
				LPCTSTR bstrUrlContext,  
				LPCTSTR bstrUrl  
				)
{
	AfxMessageBox("b");
	//_bstr_t t(bstrUrl, FALSE);
	//MessageBox(CString(t));
	//CString s = COLE2T(bstrUrl);
	//AfxMessageBox(s);
}


void CBrowserHtml::OnNewWindow2(LPDISPATCH* ppDisp, BOOL* Cancel)
{
	AfxMessageBox("a");
	*ppDisp = this->GetApplication();
	*Cancel = FALSE;
}

//***add by pan peng for open video files direction.
void CBrowserHtml::OpenVideoDirectory(LPCTSTR sSubPath)
{
	CWnd *pWnd = AfxGetApp()->GetMainWnd();
	if(NULL == pWnd)
		return;
	CString str = sSubPath;
	//str.Replace(_T(" "),_T(""));
	TCHAR *szPath = new TCHAR[MAX_PATH];
	memset(szPath, 0, MAX_PATH);
	memcpy(szPath, str.GetBuffer(), str.GetLength() + 1);
#ifndef UNICODE
	strcpy(szPath, sSubPath);
//	pWnd->PostMessage(MYWM_OPENVIDEODIR, (WPARAM)szPath, str.GetLength() + 1);
#else
	wcscpy(szPath, sSubPath);
	pWnd->PostMessage(MYWM_OPENVIDEODIR, (WPARAM)szPath, str.GetLength() + 1);
#endif
	
}

//***add by pan peng for open video files direction.
void CBrowserHtml::OpenAudioDirectory(LPCTSTR sSubPath)
{
	CWnd *pWnd = AfxGetApp()->GetMainWnd();
	if(NULL == pWnd)
		return;
	CString str = sSubPath;
	//str.Replace(_T(" "),_T(""));
	TCHAR *szPath = new TCHAR[MAX_PATH];
	memset(szPath, 0, MAX_PATH);
	memcpy(szPath, str.GetBuffer(), str.GetLength() + 1);
#ifndef UNICODE
	strcpy(szPath, sSubPath);
//	pWnd->PostMessage(MYWM_OPENAUDIODIR, (WPARAM)szPath, str.GetLength() + 1);
#else
	wcscpy(szPath, sSubPath);
	pWnd->PostMessage(MYWM_OPENAUDIODIR, (WPARAM)szPath, str.GetLength() + 1);
#endif
	
}

//Get Supporter Status, return list: -1, error; 0, offline; 1, online; 2, leave
BSTR CBrowserHtml::GetSupporterStatus()
{
	CString strStatus = _T("-1");
	CWnd *pWnd = AfxGetApp()->GetMainWnd();
	if(NULL == pWnd)
		strStatus.AllocSysString();

	int nStatus = (int)pWnd->PostMessage(MYWM_GETSUPPORTERSTATUS, 0, 0);
	
	if (nStatus == 0)
		strStatus = _T("0");
	else if (nStatus == 1)
		strStatus = _T("1");
	else if (nStatus == 2)
		strStatus = _T("2");
	
	return strStatus.AllocSysString();
}

BOOL CBrowserHtml::PreTranslateMessage(MSG* pMsg)
{
	//if(IsDialogMessage(pMsg))
	//{

	//	if(pMsg->message == WM_KEYDOWN)
	//	{
	//		int nVirtKey = (int)pMsg->wParam; 
	//		if(nVirtKey == VK_RETURN)   
	//		{
	//			if(m_nCount % 2 != 0)
	//			{
	//				m_nCount ++;
	//				return 0;
	//			}
	//			m_nCount ++;
	//			SHORT key = GetKeyState(VK_CONTROL);

	//			if(key >= 0)//not press Ctrol
	//			{
	//				//
	//				{
	//					MyVirtualEnterCtrlKey();
	//				}
	//				TRACE("MyVirtualEnterCtrlKey\n");
	//				return 0;
	//			}
	//		}
	//	}
	//}
	return CWeb1800HtmlView::PreTranslateMessage(pMsg);
}

void CBrowserHtml::MyVirtualEnterCtrlKey()
{
	//VCÄ£Äâ¼üÅÌ²Ù×÷ 
	//INPUT input[4];
	//
	//

	//memset(input, 0, sizeof(input));

	////ÉèÖÃÄ£Äâ¼üÅÌÊäÈë

	//input[0].type = input[1].type = input[2].type = input[3].type = INPUT_KEYBOARD;
	//input[0].ki.wVk  = input[2].ki.wVk = VK_CONTROL;
	//input[1].ki.wVk  = input[3].ki.wVk = VK_RETURN;

	//// ÊÍ·Å°´¼ü
	//input[2].ki.dwFlags = input[3].ki.dwFlags = KEYEVENTF_KEYUP;


	//SendInput(4, input, sizeof(INPUT)); 
}
//end***

void CBrowserHtml::OnDocumentComplete(LPCTSTR lpszURL)
{
	if (GetParent())
		GetParent()->PostMessage(MYWM_PAGELOADED, 0, 0);

	/*IHTMLDocument * pDoc = (IHTMLDocument *)GetHtmlDocument(); 
	CComQIPtr<IHTMLDocument2> pDoc2(pDoc); 

	if (pDoc2) 
	{ 
		CComPtr<IHTMLWindow2> pWnd2; 
		pDoc2->get_parentWindow(&pWnd2); 
		if (pWnd2) 
		{ 
			VARIANT vEvent = CDOMEventHandler::CreateEventHandlerVariant(&CBrowserHtml::OnLoad, (LONG_PTR)this); 
			pWnd2->put_onload(vEvent); 
		} 
	}*/
	m_bLoading = false;
	CWeb1800HtmlView::OnDocumentComplete(lpszURL);
}

void CBrowserHtml::OnLoad(VARIANT* pVarResult, LONG_PTR lpUserData) 
{ 
	CBrowserHtml * pThis = reinterpret_cast<CBrowserHtml*>(lpUserData); 
	//::MessageBox( NULL, _T("OnLoad"), NULL, MB_OK);
	if (pThis->GetParent())
		pThis->GetParent()->PostMessage(MYWM_PAGELOADED, 0, 0);
	pThis->ShowWindow(SW_SHOW);

	pThis->SetLoading(false);
}


void CBrowserHtml::Navigate(LPCTSTR URL, DWORD dwFlags/* = 0*/,
		LPCTSTR lpszTargetFrameName/* = NULL*/,
		LPCTSTR lpszHeaders/* = NULL*/, LPVOID lpvPostData/* = NULL*/,
		DWORD dwPostDataLen/* = 0*/)
{
	m_bLoading = true;
	CWeb1800HtmlView::Navigate(URL, dwFlags, lpszTargetFrameName, lpszHeaders, lpvPostData, dwPostDataLen);
}

void CBrowserHtml::DeletePointers()
{
	try
	{
		/*while(gl_Pointers.GetSize() > 0)
		{
			CDOMEventHandler * pHandler = gl_Pointers.GetAt(0);
			if (pHandler)
			{
				delete pHandler;
				pHandler = NULL;
			}
			gl_Pointers.RemoveAt(0);
		}*/
	}
	catch(...){}
}


BOOL CBrowserHtml::ShowScreenCapture(LPCTSTR strChatInfo)
{
	if (m_pParent != NULL)
	{
//		m_pParent->SendMessage(MYWM_SHOWSCREENCAPTURE, (WPARAM)strChatInfo, NULL);	
	}

	return TRUE;
}

BSTR CBrowserHtml::SaveScreenCapture(LPCTSTR strImageURL)
{
	char strCurrentImageURL[MAX_PATH];
	lstrcpy(strCurrentImageURL, m_strCurrentDirectory);
	lstrcat(strCurrentImageURL, _T("ScreenShot\\EUScreenShot"));
	lstrcat(strCurrentImageURL, CTime::GetCurrentTime().Format("%Y%m%d%H%M%S"));
	lstrcat(strCurrentImageURL, _T(".jpg"));

	if (m_pParent != NULL)
	{
//		m_pParent->SendMessage(MYWM_SAVESCREENCAPTURE, (WPARAM)strImageURL, (LPARAM)strCurrentImageURL);	
	}

	CString strLocalImageURL(strCurrentImageURL);
	return strLocalImageURL.AllocSysString();
}


////------------------------------------------------------------------------- 
//CArray <CDOMEventHandler *, CDOMEventHandler *> gl_Pointers;
//
//CDOMEventHandler::CDOMEventHandler(PFN_DOM_EVENT_CALLBACK pfnCallback, LONG_PTR lpUserData) 
//: m_lRefCount(0) 
//, m_pfCallback(pfnCallback) 
//, m_lpUserData(lpUserData) 
//{ 
//} 
//
//CDOMEventHandler::~CDOMEventHandler(void) 
//{ 
//	
//} 
//
//
//HRESULT __stdcall CDOMEventHandler::QueryInterface(REFIID riid, void** ppvObject) 
//{ 
//	*ppvObject = NULL; 
//
//	if (IsEqualGUID(riid, IID_IUnknown)) 
//		*ppvObject = reinterpret_cast<void**>(this); 
//
//	if (IsEqualGUID(riid, IID_IDispatch)) 
//		*ppvObject = reinterpret_cast<void**>(this); 
//
//	if (*ppvObject) 
//	{ 
//		((IUnknown*)*ppvObject)->AddRef(); 
//		return S_OK; 
//	} 
//	else 
//	{ 
//		return E_NOINTERFACE; 
//	} 
//} 
//
//DWORD __stdcall CDOMEventHandler::AddRef() 
//{ 
//	return InterlockedIncrement(&m_lRefCount); 
//} 
//
//DWORD __stdcall CDOMEventHandler::Release() 
//{ 
//	if (InterlockedDecrement(&m_lRefCount) == 0) 
//	{ 
//		delete this; 
//		return 0; 
//	} 
//	return m_lRefCount; 
//} 
//
//HRESULT CDOMEventHandler::Invoke(DISPID dispIdMember, 
//								 REFIID riid, 
//								 LCID lcid, 
//								 WORD wFlags, 
//								 DISPPARAMS* pDispParams, 
//								 VARIANT* pVarResult, 
//								 EXCEPINFO * pExcepInfo, 
//								 UINT * puArgErr) 
//{ 
//	if (dispIdMember == DISPID_VALUE) 
//	{ 
//		if( m_pfCallback ) 
//			(*m_pfCallback)(pVarResult, m_lpUserData); 
//	} 
//
//	return S_OK; 
//} 
//
//LPDISPATCH CDOMEventHandler::CreateEventHandler(PFN_DOM_EVENT_CALLBACK pfnCallback, LONG_PTR lpUserData) 
//{ 
//	CDOMEventHandler * pHandler = new CDOMEventHandler( pfnCallback, lpUserData); 
//	gl_Pointers.Add(pHandler);
//	return reinterpret_cast<LPDISPATCH>(pHandler); 
//} 
//
//VARIANT CDOMEventHandler::CreateEventHandlerVariant(PFN_DOM_EVENT_CALLBACK pfnCallback, LONG_PTR lpUserData) 
//{ 
//	VARIANT variant; 
//	variant.vt = VT_DISPATCH; 
//	variant.pdispVal = CDOMEventHandler::CreateEventHandler( pfnCallback, lpUserData); 
//	return variant; 
//} 

