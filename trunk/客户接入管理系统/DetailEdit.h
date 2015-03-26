#pragma once


// CDetailEdit 对话框

class CDetailEdit : public CDialog
{
	DECLARE_DYNAMIC(CDetailEdit)

public:
	CDetailEdit(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDetailEdit();

// 对话框数据
	enum { IDD = IDD_DLG_DETAIL_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnAdd();
public:
	afx_msg void OnBnClickedBtnSave();
public:
	afx_msg void OnBnClickedBtnExit();
	void SetParams(CString strdbName,CString strdbServer,CString strdbUser,CString strPwd,CString strID,int iFlag);
	BOOL OnInitDialog();
	void InitData();
	BOOL PreTranslateMessage( MSG* pMsg );
	CString m_strID;
	CString m_dbServer;		//数据库服务器
	CString m_dbName;		//数据库名
	CString m_dbUser;		//登录账户
	CString m_dbPwd;		//登录密码
	int m_iFlag;
};
