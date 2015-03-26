#pragma once
#include "afxcmn.h"


// CServiceDetail 对话框

class CServiceDetail : public CDialog
{
	DECLARE_DYNAMIC(CServiceDetail)

public:
	CServiceDetail(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CServiceDetail();

// 对话框数据
	enum { IDD = IDD_DLG_DETAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	void InitCtrList();
	BOOL OnInitDialog();


public:
	void InitData();
	void SetParams(CString strdbName,CString strdbServer,CString strdbUser,CString strPwd,CString strID);
	BOOL PreTranslateMessage( MSG* pMsg );
	CListCtrl m_resultList;
	CString m_strID;
	CString m_dbServer;		//数据库服务器
	CString m_dbName;		//数据库名
	CString m_dbUser;		//登录账户
	CString m_dbPwd;		//登录密码
public:
	afx_msg void OnDetailAdd();
public:
	afx_msg void OnDetailEdit();
public:
	afx_msg void OnDetailDelete();
public:
	afx_msg void OnNMRclickListShow(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnNMDblclkListShow(NMHDR *pNMHDR, LRESULT *pResult);
};
