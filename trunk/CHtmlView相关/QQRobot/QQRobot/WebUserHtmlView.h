#pragma once
#include <afxhtml.h>
//#include "Callback.h"
//#include "SDocEvtHandler.h"
//#include "MSGDefines.H"
//#include "FixHtmlView.h"

#define NAVIGATE_TIMERID 1

#ifdef _WIN32_IE
#undef _WIN32_IE
#endif

#define _WIN32_IE 0x0600	// Change this to the appropriate value to target IE 6.0 or later.
// message type
// CUserHtmlView Html View
class CUserHtmlView : public CHtmlView
{
	DECLARE_DYNCREATE(CUserHtmlView)

public:
	CUserHtmlView();
	virtual ~CUserHtmlView();

public:
	BOOL CreateHtmlView(UINT nID, CWnd* pParent);

public:
	enum CONTEXT_MENU_MODE		// 上下文菜单
	{
		NoContextMenu,			// 无菜单
		DefaultMenu,			// 默认菜单
		TextSelectionOnly,		// 仅文本选择菜单
		CustomMenu				// 自定义菜单
	};

	// function for web server
public:
	void Open_Page(LPCTSTR strUrl);
	void OpenWebWindow(LPCTSTR strUrl);
	void FlashMainWindow();
	BSTR GetComputerHardId();
	//void StartReportClientStatus();
	// MesseBox
	virtual HRESULT OnShowMessage(HWND hwnd,LPOLESTR lpstrText,LPOLESTR lpstrCaption,DWORD dwType,LPOLESTR lpstrHelpFile,DWORD dwHelpContext,LRESULT * plResult);
	virtual HRESULT OnShowContextMenu(DWORD dwID, LPPOINT ppt,LPUNKNOWN pcmdtReserved, LPDISPATCH pdispReserved);
	//virtual BOOL PreTranslateMessage(LPMSG lpMsg);
	// 设置上下文菜单模式
	void SetContextMenuMode(CONTEXT_MENU_MODE MenuMode);
	virtual HRESULT OnGetExternal(LPDISPATCH *lppDispatch);
	void SetAlertTitle(WCHAR *strTitle);

	//When 60 seconds time out, Auto refresh
	void SetAutoRefresh(bool bAutoRefresh, int nTimeout = 60){ m_bAutoRefresh = bAutoRefresh, m_nAutoRefreshTimeout = nTimeout; }
	
	void Show(){ShowWindow(SW_SHOW); EnableWindow(TRUE);}
	void Hide(){ShowWindow(SW_HIDE); EnableWindow(FALSE);}

protected:
	WCHAR m_strAlertTitle[MAX_PATH]; 
	CString m_DefaultMsgBoxTitle;					// 默认MessageBox标题
	CONTEXT_MENU_MODE m_ContextMenuMode;			// 上下文菜单模式

protected:
	CWnd *m_pParent;
	char m_strCurrentDirectory[MAX_PATH];
	bool	m_bAutoRefresh;
	int		m_nAutoRefreshTimeout;
	CString m_strOldUrl;
	
//	SDocEvtHandler*	m_pEventHandler;
	DWORD			m_dwDocCookie;		// 用于卸载事件响应函数
	IDispatch*		m_pDispDoc;			// 用于卸载事件响应函数


public:
// 	Callback<BOOL, CString>						m_OnBeforeNavigateCallback;
// 	Callback<BOOL, MSHTML::IHTMLEventObjPtr>	m_OnClickCallback;
// 	Callback<BOOL, CString>						m_OnDocumentCompleteCallback;
// 	Callback<BOOL, CString>						m_OnNewWindow3Callback;
// 	Callback<void,LONG>							m_OnNewReceived;
// 	Callback<LONG,LONG>							m_OnGetMessageID;
// 	Callback<void,bool>							m_OnFlashTrayIcon;
// 	Callback<CString,int>						m_OnGetVisibleGroup;
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
	
	//virtual BOOL CreateControlSite(COleControlContainer * pContainer, COleControlSite ** ppSite, UINT nID, REFCLSID clsid);
	virtual void PostNcDestroy() {};
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV Support

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_EVENTSINK_MAP()	

	virtual void OnDocumentComplete(LPCTSTR lpszURL);
	afx_msg void OnTimer(UINT_PTR nIDEvent);


	
public:
	void Navigate(LPCTSTR URL, DWORD dwFlags = 0,
		LPCTSTR lpszTargetFrameName = NULL,
		LPCTSTR lpszHeaders = NULL, LPVOID lpvPostData = NULL,
		DWORD dwPostDataLen = 0);
	void OnNewWindow3( IDispatch *ppDisp, VARIANT_BOOL *Cancel, DWORD dwFlags, LPCTSTR bstrUrlContext, LPCTSTR bstrUrl );
	BOOL OnAmbientProperty(COleControlSite* pSite, DISPID dispid, VARIANT* pvar);
};