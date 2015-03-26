// XlsCtrolDlg.h : 头文件
//

#pragma once

#include "XlsCtrolClass.h"
#include "BasicExcel.hpp"
using namespace YExcel;
// CXlsCtrolDlg 对话框
class CXlsCtrolDlg : public CDialog
{
// 构造
public:
	CXlsCtrolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_XLSCTROL_DIALOG };

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
	afx_msg void OnBnClickedButton3();
	
	vector<ExpressStr> m_vecExpressInfo;
	afx_msg void OnBnClickedButton2();
	CXlsCtrolClass m_XlsCtrol;

	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	void GenData(vector<ExpressStr> &vecStr);
	afx_msg void OnBnClickedButton6();
};
