// BMPTestDlg.h : 头文件
//

#pragma once
#include "MyImage.h"
// CBMPTestDlg 对话框
class CBMPTestDlg : public CDialog
{
// 构造
public:
	CBMPTestDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_BMPTEST_DIALOG };

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
	int GetRows(CString fileName);
	void GenerateDataFile();
	afx_msg void OnBnClickedBtnDetect();
	afx_msg void OnBnClickedBtnCreattemp();
	afx_msg void OnBnClickedBtnCbat();
	afx_msg void OnBnClickedBtnCvec();
	afx_msg void OnBnClickedBtnTrain();
	afx_msg void OnBnClickedBtnCxml();
};
