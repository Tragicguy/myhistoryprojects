// Web1800AutoTesterDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"

#include <mshtml.h>
//#include <afxhtml.h>
//#include <atlbase.h>
#include <oleacc.h>
#include <comutil.h>
#include <afxhtml.h>
#include <comdef.h>
#include <ExDispid.h>
// CWeb1800AutoTesterDlg 对话框
class CWeb1800AutoTesterDlg : public CDialog
{
// 构造
public:
	CWeb1800AutoTesterDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_WEB1800AUTOTESTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStarttimer();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnStarttest();
	void InitListCtrl();
	void WriteLog(CString srcStr,int iFlag);
	int AddToList(CString msgBuf);
	int GetAppPath(CString &strAppPath);
	IHTMLDocument2* GetDocInterface(HWND hWnd);
	CListCtrl m_message;
	afx_msg void OnBnClickedButton1();
	CString GetHtmlContentEx(IHTMLDocument2* pDoc);
	CString GetKeyString(CString &srcStr,CString &strBefore,CString &strAfter);
	HWND GetExplorerHWND();
	void GetElementCollection(CString tagName,IHTMLElementCollection** pSpecialColl,IHTMLDocument2* pDoc);
	HRESULT GetElement(LPCTSTR szElementId, IDispatch **ppdisp, BOOL *pbCollection /*= NULL*/,CComPtr<IHTMLDocument2> sphtmlDoc);
	
	HRESULT GetElement(LPCTSTR szElementId, IHTMLElement **pphtmlElement,IHTMLDocument2* pDoc);
	HRESULT GetElementInterface(LPCTSTR szElementId, REFIID riid, void **ppvObj,IHTMLDocument2* pDoc);
	bool SendRequest();
	static void ThreadFunc(LPVOID lParam);
	void RunFunc();

	static void ThreadFuncSp(LPVOID lParam);
	void RunFuncSp();
	afx_msg void OnBnClickedButton2();
	bool AcceptRemote(IHTMLDocument2 *pDoc);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	void ClearFiles();
	bool RegistOcx(LPCTSTR strFileName, bool bInstall);
	bool PostWebClick(IHTMLDocument2 *pDoc,CString strId);
	CString GetInnerText(IHTMLDocument2 *pDoc,CString strId);
	bool StopSession(IHTMLDocument2 *pDoc);
	void ClickOcxInstall();
	int AddToListSp(CString msgBuf);
	CListCtrl m_spMessage;
	afx_msg void OnBnClickedBtnStartsp();
	afx_msg void OnBnClickedBtnStopclient();
	afx_msg void OnBnClickedBtnStopsp();
};
