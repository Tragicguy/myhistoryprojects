#pragma once
#include <comutil.h>
#include <afxhtml.h>
#include <comdef.h>
#include <ExDispid.h>
#include "resource.h"
#define	CDHTMLDIALOGEXPOINTERTYPE	1		//对话框以指针形式在堆中生成
#define	CDHTMLDIALOGEXOBJECTTYPE	2		//对话框以对象方式在栈中生成

// CDHtmlDialogEx 对话框

class CDHtmlDialogEx : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CDHtmlDialogEx)

public:
	CDHtmlDialogEx(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDHtmlDialogEx();
	BOOL IsExternalDispatchSafe();
// 对话框数据
	enum { IDD = IDD_DLG_HTMLDLG, IDH = 0 };
	void BickerWindow();
	void TestFunction(LPCTSTR szStr);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	CString GetAppPath();		//获得应用程序存放路径
	CString GetItemValue(CString itemID);	//获取网页对应ITEM的值（主要针对INPUT框）
	BOOL PreTranslateMessage( MSG* pMsg);
	void PostNcDestroy();
	void GetSpecialElement(CString tagName,CString innerText,IHTMLElement** pElemRet);
	void GetElementCollection(CString tagName,IHTMLElementCollection** pSpecialColl,IHTMLDocument2* pDoc);
	void GetFrameDoc(int index,IHTMLDocument2* pDoc,IHTMLDocument2 **pFrameDocRet);
	void OnNavigateComplete(LPDISPATCH pDisp, LPCTSTR szUrl);
	virtual void OnNewWindow3(
		LPDISPATCH* ppDisp, 
		BOOL* Cancel, 
		unsigned long dwFlags,
		LPCTSTR bstrUrlContext,
		LPCTSTR bstrUrl
		);
	virtual void OnBeforeNavigate(LPDISPATCH pDisp,	LPCTSTR szUrl);
	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags,
		LPCTSTR lpszTargetFrameName, CByteArray& baPostedData,
		LPCTSTR lpszHeaders, BOOL* pbCancel);
	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
	DECLARE_DISPATCH_MAP() // 构建dispatch映射表以暴露方法或属性
public:
	IHTMLDocument2 *pDoc3;
	afx_msg void OnBnClickedBtnLogin();
	afx_msg void OnBnClickedBtnGetuserinfo();
	afx_msg void OnClose();
	int m_DlgCreatType;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	HRESULT ExecJavascript(CString strJsFunctionName_In, const CArray <VARIANT, VARIANT& > &pArrVARIANT_Params);
	BOOL GetUrlPath();
	afx_msg void OnBnClickedButton4();
	STDMETHODIMP CopyImage(VARIANT vImageID);
	void OnPasteAsFile();
	void SetItemValue(CString strID,CString strValue);
	bool SendClickEvent(CString strID);
	afx_msg LRESULT AcceptMessage(WPARAM wParam,LPARAM lParam);
	int StringSplit(CString srcStr,CString strArray[],char splitChar);
	void GetRegisterPack(CString strSrc);
	afx_msg void OnTimer(UINT nIDEvent);
	void OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl);
	CString GetHtmlContent();
	CString GetHtmlContentEx();
	CString GetHtmlContentExEx();
	CComQIPtr<IWebBrowser2> HtmlWindowToHtmlWebBrowser(CComQIPtr<IHTMLWindow2> spWindow);
	void GetDocInterface(HWND hWnd,IHTMLDocument2* pDocOut);
	int GetHtmlCtrl(IHTMLDocument2 *pDoc2);
	int ClickSpecial();
	int ClickSpecialEx(IHTMLDocument2 *pDoc2);
	int SetFrameSrc();
	HWND m_hSubHwnd;
	afx_msg void OnBnClicked1008();
	int ClickSpecialExEx(IHTMLDocument2 *pDoc2);
	int AddEmailAddr(IHTMLDocument2 *pDoc2);
	CString GetKeyString(CString &srcStr,CString &strBefore,CString &strAfter);
	BOOL OnAmbientProperty(COleControlSite* pSite, DISPID dispid, VARIANT* pvar);
};
