// VSHackNewDlg.h : 头文件
//

#pragma once


// CVSHackNewDlg 对话框
class CVSHackNewDlg : public CDialog
{
// 构造
public:
	CVSHackNewDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_VSHACKNEW_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	
	static DWORD WINAPI CloseWarnProc(LPVOID lpParameter);

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	void CloseWarnWindow();
};
