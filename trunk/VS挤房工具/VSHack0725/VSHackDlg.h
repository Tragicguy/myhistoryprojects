// VSHackDlg.h : 头文件
//

#pragma once


// CVSHackDlg 对话框
class CVSHackDlg : public CDialog
{
// 构造
public:
	CVSHackDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_VSHACK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	HWND hwnd;
	int oldItem;
	RECT  Rect;
	bool hitFlag;
	bool dblhitFlag;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnTimer(UINT nIDEvent);
	BOOL PreTranslateMessage( MSG* pMsg);
	static DWORD WINAPI TryIntoRoomProc(LPVOID lpParameter);
	static DWORD WINAPI CloseWarnProc(LPVOID lpParameter);
	void TryIntoRoom();
	void CloseWarnWindow();
	void DblHitItem();
	afx_msg void OnClose();
	DWORD m_dwProcessID;
	HWND m_hMainHwnd;
public:
	afx_msg void OnBnClickedButton1();
	HWND GetProcessMainWnd(HWND hwndList) /*将DLL窗口的句柄传入，以便识别找到的句柄?是DLL窗口的句柄 */;
	void MyFindWindow(CString strClassName,CString strWindowName,HWND hParant);
	void GenBatFile();
	void GenBatFileEx();
	CString GetFileVersion(CString strFileName);
	CString GetKeyString(CString &srcStr,CString &strBefore,CString &strAfter);
	void ConvertUtf8ToGBK(CString& strUtf8);
	void CheckNewVersion();
	static DWORD WINAPI CheckNewVersionProc(LPVOID lpParameter);
	LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
