// TESTTESTDlg.h : 头文件
//

#pragma once


// CTESTTESTDlg 对话框
class CTESTTESTDlg : public CDialog
{
// 构造
public:
	CTESTTESTDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TESTTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	HINSTANCE m_hModule;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	~CTESTTESTDlg();
	void ThreadProc(int iIndex);
	static void ThreadRun(LPVOID lpParam);
	void ShowMessage(CString strMsg);
};
