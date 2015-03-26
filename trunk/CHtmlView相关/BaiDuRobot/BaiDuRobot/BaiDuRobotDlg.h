// BaiDuRobotDlg.h : 头文件
//

#pragma once
#include <mshtml.h>
#include <oleacc.h>
#include <comutil.h>
#include <afxhtml.h>
#include <comdef.h>
#include <ExDispid.h>
#include "afxcmn.h"
#include <map>
using namespace std;
// CBaiDuRobotDlg 对话框
class CBaiDuRobotDlg : public CDialog
{
// 构造
public:
	CBaiDuRobotDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_BAIDUROBOT_DIALOG };

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
	void AddToList(CString strInfo);
	void GetElementCollection(CString tagName,IHTMLElementCollection** pSpecialColl,IHTMLDocument2* pDoc);
	HRESULT GetElement(LPCTSTR szElementId, IDispatch **ppdisp, BOOL *pbCollection /*= NULL*/,CComPtr<IHTMLDocument2> sphtmlDoc);
	HRESULT GetElement(LPCTSTR szElementId, IHTMLElement **pphtmlElement,IHTMLDocument2* pDoc);
	void AuToOpenPage(CString strKey, CString strUrl, bool *pbExitFlag);
	CString GetInnerText(IHTMLDocument2 *pDoc,CString strId);
	CString GetInnerText(IHTMLElement *pElement);
	bool PostWebClick(IHTMLDocument2 *pDoc,CString strId);
	HRESULT GetElementInterface(LPCTSTR szElementId, REFIID riid, void **ppvObj,IHTMLDocument2* pDoc);
	HWND GetExplorerHWND();
	HWND GetExplorerHWND(HWND hwnd);
	void RunFunc();
private:
	bool m_bExitFlag;
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStop();
	static void ThreadFunc(LPVOID lParam);
	void InitListCtrl();
	int GetAppPath(CString &strAppPath);
	void WriteLog(CString srcStr,int iFlag);
	CListCtrl m_list_show;

	CString m_strAppPath;
	afx_msg void OnBnClickedButton1();
	IHTMLDocument2* GetDocInterface(HWND hWnd);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnClose();
	CString URLEncode(CString sIn);
	bool HTMLElementRect(IHTMLElement* pElement, RECT& rect, int level);
	bool StartExplorer(CString strUrl, STARTUPINFO &vStartupInfo, PROCESS_INFORMATION& vProcessInfo);
	HWND GetMainHwnd(DWORD dwProcessId);
	CString GetInnerUrl(CString strText, int iFlag);
	CString GetKeyString(CString &srcStr,CString strBefore,CString strAfter);
	CString GetInnerHtml(IHTMLElement *pElement);
	int StringSplit(CString srcStr,map<CString, int>& mapInfo,char splitChar);
};
