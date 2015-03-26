// AuToWebSiteDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "WindowFind.h"

// CAuToWebSiteDlg 对话框
class CAuToWebSiteDlg : public CDialog
{
// 构造
public:
	CAuToWebSiteDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_AUTOWEBSITE_DIALOG };

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
	afx_msg void OnBnClickedBtnBrowser();
	afx_msg void OnBnClickedBtnBrowser2();
	afx_msg void OnBnClickedBtnBrowser3();
	afx_msg void OnBnClickedBtnBrowser4();
	void GetFilePath(int nCtrlId);
	afx_msg void OnBnClickedBtnSave();
	void InitParams();
	int GetAppPath(CString &strAppPath);
	CComboBox m_comType;
	afx_msg void OnBnClickedBtnQuit();
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStop();
	void RunFunc();
	static void ThreadFunc(LPVOID lParam);
	void ShowMessage(CString strInfo);
public:
	afx_msg void OnBnClickedButton1();

	CWindowFind m_wndFind;
};
