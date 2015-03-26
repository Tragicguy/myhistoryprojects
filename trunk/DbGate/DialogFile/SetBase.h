#pragma once
#include "OtherSet.h"
#include "OtherSetEx.h"
#include "StroeSet.h"
class CSetBase : public CDialog
{
	DECLARE_DYNAMIC(CSetBase)

public:
	CSetBase(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetBase();

// 对话框数据
	enum { IDD = IDD_DLG_SET_BASE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	BOOL OnInitDialog();
	void InitDialogs();
private:
	COtherSet otherSet;
	COtherSetEx otherSetEx;
	CStroeSet storeSet;
public:

	afx_msg void OnBnClickedRadioNormal();
	afx_msg void OnBnClickedRadioOwnset();
	afx_msg void OnBnClickedRadioDbstoreset();
	afx_msg void OnClose();
	void SaveParams();
	afx_msg void OnBnClickedCheckMainport();
	afx_msg void OnBnClickedCheckHistry();
	afx_msg void OnBnClickedCheckRecvtable();
};
