// ReStartAppDlg.h : 头文件
//

#pragma once


// CReStartAppDlg 对话框
class CReStartAppDlg : public CDialog
{
// 构造
public:
	CReStartAppDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_RESTARTAPP_DIALOG };

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
	afx_msg void OnBnClickedBtnStart();
	int GetAppPath(CString &strAppPath);
	afx_msg void OnTimer(UINT nIDEvent);
	int ReStartApp(int iFlag);
	afx_msg void OnBnClickedBtnExit();
	void InitParams();
	afx_msg LRESULT OnReInitparam(WPARAM wParam,LPARAM lParam);
	int m_iCheckInterval;
	CString m_strWindowName;
	CString m_strStrAppName;
	int m_iRunStatus;
	int m_iFailTime;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void CloseErrorWindow();
	static void CloseProcFun(LPVOID lpParam);	//
	afx_msg void OnClose();
	void CloseProc();
};
