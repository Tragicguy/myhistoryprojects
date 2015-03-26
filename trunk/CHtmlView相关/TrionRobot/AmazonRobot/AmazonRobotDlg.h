// AmazonRobotDlg.h : 头文件
//

#pragma once
#include <comutil.h>
#include <afxhtml.h>
#include <comdef.h>
#include <ExDispid.h>
#include "DataDefine.h"
#include <map>
using namespace std;
extern AmazonOrderStru gl_AmazonStr;
extern int GetAppPath(CString &strAppPath);
// CAmazonRobotDlg 对话框
class CAmazonRobotDlg : public CDHtmlDialog
{
// 构造
public:
	CAmazonRobotDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_AMAZONROBOT_DIALOG, IDH = NULL };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	


	virtual void _OnBeforeNavigate2(LPDISPATCH pDisp, VARIANT FAR* URL, VARIANT FAR* Flags, VARIANT FAR* TargetFrameName, VARIANT FAR* PostData, VARIANT FAR* Headers, BOOL FAR* Cancel);


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_EVENTSINK_MAP()
	CString GetKeyString(CString &srcStr,CString strBefore,CString strAfter);
	BOOL OnAmbientProperty(COleControlSite* pSite, DISPID dispid, VARIANT* pvar);
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	
	bool AddGoodsTocart(CString strGoodNo);
	void OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl);
	void WaitForComplete();
	void NavigateEx(CString strUrl);

	static void ThreadFunc(LPVOID lParam);
	void RunFunc();
	void ExecJavascript(CString strJs);
	LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void OnClickMessage(WPARAM wParam, LPARAM lParam);
	void ClickElement(CString strID);
	void OnRunJavascript(WPARAM wParam, LPARAM lParam);
	void RunJavascript(CString strJS);
	CString GetHtmlContent();
	void OnGetHtmlContent(WPARAM wParam, LPARAM lParam);
	void GetContent();
	void OnClickMessageEx(WPARAM wParam, LPARAM lParam);
	void ClickElementEx(IHTMLElement* pElement);
	void OnGetHtmlContentEx(WPARAM wParam, LPARAM lParam);
	void GetContentEx(IHTMLElement* pElement);
	void OnTimer(UINT_PTR nIDEvent);
	
	void OnSetValue(WPARAM wParam, LPARAM lParam);
	void SetItemValue(CString strID,CString strValue);
	void ConvertGBKToUtf8(CString& strGBK);
	void ConvertUtf8ToGBK(CString& strUtf8);
	void OnSetSendTypeAndBill(WPARAM wParam, LPARAM lParam);
	void SetSendTypeAndBillInfo(AmazonOrderStru &objInfo);
	void GetElementCollection(CString tagName,IHTMLElementCollection** pSpecialColl,IHTMLDocument2* pDoc);
	void SetSendType(_SendType type);
	void InitBillType();
	BOOL PreTranslateMessage( MSG* pMsg );
	int StringSplit(CString srcStr,CString strArray[],char splitChar);
	int GetPower();
	afx_msg void OnClose();
//	void GetElementCollectionEx(CString tagName,IHTMLElementCollection** pSpecialColl,IHTMLElement* pElement);
	map<CString,CString> m_mapBillType;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();

	void Order();
	bool DoOrder(AmazonOrderStru &objStr);
	void Query();	
	bool DoQuery(AmazonOrderStru &objStr);
	void GetElementValue(CString strId);
	CString GetItemValue(IHTMLElement *pElement);
	void SetValue(LPCTSTR strValue);
	void ChangePayType();
	bool DoChange(AmazonOrderStru &objStr);
	int m_iStarted;
};
