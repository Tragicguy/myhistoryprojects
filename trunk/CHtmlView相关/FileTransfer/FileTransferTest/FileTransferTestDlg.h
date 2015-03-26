// FileTransferTestDlg.h : 头文件
//

#pragma once
#include "FileFuncDelegate.h"
#include "afxwin.h"
#include "afxcmn.h"

// CFileTransferTestDlg 对话框
class CFileTransferTestDlg : public CDialog
{
// 构造
public:
	CFileTransferTestDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_FILETRANSFERTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CFileFuncDelegate m_File;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	CIPAddressCtrl m_ServerIP;
	CEdit m_ServerPort;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	CEdit m_ServerListenPort;
	afx_msg void OnClose();
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
