#pragma once
#include "afxwin.h"
#include "afxdtctl.h"

// CInfoShow 对话框

class CInfoShow : public CDialog
{
	DECLARE_DYNAMIC(CInfoShow)

public:
	CInfoShow(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInfoShow();

// 对话框数据
	enum { IDD = IDD_DIALOG_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnDeletetype();
	BOOL PreTranslateMessage( MSG* pMsg );
	BOOL OnInitDialog();
	int m_iFlag;		//标志位，1为添加，2为修改，其他为NOTHING退出
	int m_iCustomerID;
public:
	afx_msg void OnBnClickedBtnCancle();
	void QueTypeAndLanguage();
	void SetParams(CString strdbName,CString strdbServer,CString strdbUser,CString strPwd,int iFlag,int iCustomerID);
	void InitControls();
public:
	CComboBox m_comboType;
	CComboBox m_combo_language;
	map<CString, int>  m_typeMap;
	map<CString, int>  m_languageMap;
	//CString m_dbPath;
	CString m_dbServer;		//数据库服务器
	CString m_dbName;		//数据库名
	CString m_dbUser;		//登录账户
	CString m_dbPwd;		//登录密码
public:
	afx_msg void OnBnClickedBtnAddtype();
public:
	afx_msg void OnBnClickedBtnSave();
public:
	afx_msg void OnBnClickedBtnAdd();
public:
	afx_msg void OnBnClickedBtnAddlang();
public:
	afx_msg void OnBnClickedBtnDeletelang();
	int GetComboIndex(int iValue,int iFlag);
	void QueType();
	void QueLanguage();
public:
	CDateTimeCtrl m_datePicker;
};
