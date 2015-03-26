// TrayTestDlg.h : 头文件
//

#pragma once
#include "SystemTray.h"


// CTrayTestDlg 对话框
class CTrayTestDlg : public CDialog
{
// 构造
public:
	CTrayTestDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TRAYTEST_DIALOG };

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
	afx_msg void OnBnClickedButton1();
	~CTrayTestDlg();
	CSystemTray *m_pTray;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnClose();
	afx_msg void OnTrayMenu();
	afx_msg void OnMenuexit();
	afx_msg void OnTestmenu();
	afx_msg void OnMenushow();
};
