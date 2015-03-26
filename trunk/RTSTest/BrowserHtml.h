#pragma once
#include "Web1800HtmlView.h"

// message type
#define ENABLE_HTML				3000
#define ENABLE_BACK				3001
#define ENABLE_FORWARD			3002
#define WELCOMEPAGE_EXIT		4001
#define MYWM_PAGELOADED			WM_USER + 4002

#define MYWM_GETSUPPORTERSTATUS WM_USER + 5000

// CBrowserHtml Html View

class CBrowserHtml : public CWeb1800HtmlView
{
	DECLARE_DYNCREATE(CBrowserHtml)
	
public:
	CBrowserHtml();
	virtual ~CBrowserHtml();

public:
	enum CONTEXT_MENU_MODE		// 上下文菜单
	{
		NoContextMenu,			// 无菜单
		DefaultMenu,			// 默认菜单
		TextSelectionOnly,		// 仅文本选择菜单
		CustomMenu				// 自定义菜单
	};


public:
	void SetSupporterId(LPCTSTR strSupporterId) {
		m_strSupporterId = strSupporterId;
	};


	BOOL CreateHtmlView(UINT nID, CWnd* pParent);
	CString GetWebAddress() const { return m_strWebAddress; };

	// function for web server
public:
	BSTR GetSupporterId();
	BOOL OpenSubWebPage(LPCTSTR strWebAddress);
	void ShowStartupInfo(BOOL bLogined);
	void CloseStartupWindow();
	BOOL ShowScreenCapture(LPCTSTR strChatInfo);
	BSTR SaveScreenCapture(LPCTSTR strImageURL);
	//***add by pan peng for open video files direction.
	void OpenVideoDirectory(LPCTSTR sSubPath);
	void OpenAudioDirectory(LPCTSTR sSubPath);
	//end***
	BSTR GetSupporterStatus();
	void SetParentWnd(CWnd *pParent);
private:
	CWnd *m_pParent;
	CString m_strSupporterId;
	CString m_strWebAddress;

	long m_nCount;
	char m_strCurrentDirectory[MAX_PATH];
	bool m_bLoading;
protected:
	virtual void PostNcDestroy() {};

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_EVENTSINK_MAP()

protected:
	virtual void OnFinalRelease();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnCommandStateChange(long nCommand, BOOL bEnable);
	virtual void OnNewWindow2(LPDISPATCH* ppDisp, BOOL* Cancel);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
private:
	void MyVirtualEnterCtrlKey();
	virtual void OnDocumentComplete(LPCTSTR lpszURL);
	static void OnLoad(VARIANT* pVarResult, LONG_PTR lpUserData); 
	static void DeletePointers();
public:
	void Navigate(LPCTSTR URL, DWORD dwFlags = 0,
		LPCTSTR lpszTargetFrameName = NULL,
		LPCTSTR lpszHeaders = NULL, LPVOID lpvPostData = NULL,
		DWORD dwPostDataLen = 0);
	bool IsLoading(){ return m_bLoading; }
	void SetLoading(bool bLoading){ m_bLoading = bLoading; }
	void NewWindow3( IDispatch **ppDisp, VARIANT_BOOL *Cancel, DWORD dwFlags, LPCTSTR bstrUrlContext, LPCTSTR bstrUrl );
};

//typedef void (*PFN_DOM_EVENT_CALLBACK)(VARIANT* pVarResult, LONG_PTR lpUserData); 
//
//class CDOMEventHandler : public IDispatch 
//{ 
//public: 
//	CDOMEventHandler(PFN_DOM_EVENT_CALLBACK pfnCallback, LONG_PTR lpUserData); 
//	~CDOMEventHandler(void); 
//
//	HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObject); 
//	DWORD __stdcall AddRef(); 
//	DWORD __stdcall Release(); 
//
//	STDMETHOD(GetTypeInfoCount)(unsigned int FAR* pctinfo) { return E_NOTIMPL; } 
//	STDMETHOD(GetTypeInfo)(unsigned int iTInfo, LCID lcid, ITypeInfo FAR* FAR* ppTInfo) { return E_NOTIMPL; } 
//
//	STDMETHOD(GetIDsOfNames)(REFIID riid, OLECHAR FAR* FAR* rgszNames, unsigned int cNames, LCID lcid, DISPID FAR* rgDispId) { return S_OK; } 
//
//	STDMETHOD(Invoke)(DISPID dispIdMember 
//		, REFIID riid 
//		, LCID lcid 
//		, WORD wFlags 
//		, DISPPARAMS* pDispParams 
//		, VARIANT* pVarResult 
//		, EXCEPINFO * pExcepInfo 
//		, UINT * puArgErr 
//		); 
//
//	static LPDISPATCH CreateEventHandler(PFN_DOM_EVENT_CALLBACK pfnCallback, LONG_PTR lpUserData); 
//	static VARIANT CreateEventHandlerVariant(PFN_DOM_EVENT_CALLBACK pfnCallback, LONG_PTR lpUserData); 
//
//private: 
//	PFN_DOM_EVENT_CALLBACK m_pfCallback; 
//	long m_lRefCount; 
//	LONG_PTR m_lpUserData; 
//	
//}; 
//
//extern CArray <CDOMEventHandler *, CDOMEventHandler *> gl_Pointers;