// ConferenceProcesserDlg.h : 头文件
//

#pragma once
#include "DataProcess.h"
#include "tinyxml/tinyxml.h"
#include "afxcmn.h"
#include "../Util/ProgramConfig.h"
// CConferenceProcesserDlg 对话框
class CConferenceProcesserDlg : public CDialog
{
// 构造
public:
	CConferenceProcesserDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CONFERENCEPROCESSER_DIALOG };

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
	CDataProcess m_DataProcess;
public:
	afx_msg void OnBnClickedButton2();
	void InitListCtrl();
public:
	CListCtrl m_list_show;
	ProgramConfig m_ProgramConfig;
public:
	afx_msg void OnBnClickedBtnExit();
	BOOL PreTranslateMessage( MSG* pMsg );
public:
	afx_msg void OnClose();
};
