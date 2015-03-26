// TestEncodeDlg.h : 头文件
//

#pragma once


// CTestEncodeDlg 对话框
class CTestEncodeDlg : public CDialog
{
// 构造
public:
	CTestEncodeDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TESTENCODE_DIALOG };

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
	CString EncodeC(CString U);
	CString EncodeR(CString ab);
public:
	afx_msg void OnBnClickedButton2();
};
