#pragma once
#include "afxcmn.h"

#include "../resource.h"
// CDbGateClose 对话框

class CDbGateClose : public CDialog
{
	DECLARE_DYNAMIC(CDbGateClose)

public:
	CDbGateClose(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDbGateClose();
	int m_ThreadNum;
	int m_iFlag;
	BOOL OnInitDialog();
// 对话框数据
	enum { IDD = IDD_DLG_CLOSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT ThreadClose(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	int m_Counter;	
public:
	CProgressCtrl m_ProgressCtrl;
	afx_msg void OnTimer(UINT nIDEvent);
	BOOL PreTranslateMessage( MSG* pMsg );
};
