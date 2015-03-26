// SendLogQueryDlg.h : 头文件
//

#pragma once

// CSendLogQueryDlg 对话框
class CSendLogQueryDlg : public CDialog
{
	//线程参数结构
	typedef struct tagThreadParams
	{
		CSendLogQueryDlg* pWnd;
		int  m_iNo;
	} ThreadParams;

	//线程控制结构体
	typedef struct tagThreadControl
	{
		CWinThread* pThread;
		BOOL ThreadExit;		
	} ThreadControl;
// 构造
public:
	CSendLogQueryDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SENDLOGQUERY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	static void ThreadProc(LPVOID lpParam);

	afx_msg void OnTimer(UINT nIDEvent);
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	ThreadControl m_ThreadContrl;
	int m_Counter;
	CProgressCtrl m_ProgressCtrl;
	int m_iRowOnePage;			//每页显示记录数
	int m_iTotalPage;			//总页数
	int m_iCurrPage;			//当前页
	int m_iTotalRecordCount;	//查出的总记录数
	int m_iTotalSucRecord;
	int m_iTotalFailRecord;

	int m_iTotalPhoneCount;		//查出的总的发送信息条数
	int m_iTotalSucPhoneCount;
	int m_iTotalFailPhoneCount;

	int m_iSendFlag;			//发送状态标志
	CString m_strWhere;			//查询条件
	CString m_strWhereOther;			//查询条件
	BOOL m_IsShowDetail;		//是否显示详细信息

	CAdoConnection m_adoConn;
	CAdoRecordSet m_adoRecrd;

	CString m_dbServer;		//数据库服务器
	CString m_dbName;		//数据库名
	CString m_dbUser;		//登录账户
	CString m_dbPwd;		//登录密码
	//CString m_dbPath;		//数据库路径
	CString m_strStartDate;
	CString m_strEndDate;
public:
	void OperateProc(int iFlag);
	afx_msg void OnBnClickedBtnQuery();
	void InitCtrList();
	void QuerySendLog(CString strWhere,int iFlag,BOOL *bExit,int iCurrPage,BOOL bDetail);
	CListCtrl m_resultList;
	afx_msg void OnBnClickedBtnStop();
	CTime GetCTimeFromString(CString strTime);
	afx_msg void OnClose();
	int GetAppPath(CString &strAppPath);
	afx_msg void OnBnClickedBtnOutport();
	CString GetExcelDriver();
	void ExportToExcel(CString strTitle,CString strWhere,int iFlag,BOOL *bExit,int iCurrPage,BOOL bDetail);
	BOOL GetDefaultXlsFileName(CString& sExcelFile);
	BOOL MakeSurePathExists( CString &Path,bool FilenameIncluded);
	CListCtrl m_listtype;
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnNMDblclkListShow3(NMHDR *pNMHDR, LRESULT *pResult);
	void QueryTypeInfo(CString strWhere,int iFlag,BOOL *bExit,int iCurrPage,BOOL bDetail);
public:
	afx_msg void OnBnClickedBtnFresh();
	void QueTypeAndLanguage();
	BOOL PreTranslateMessage( MSG* pMsg );
public:
	CComboBox m_comboType;
	CComboBox m_combo_language;
	map<CString, int>  m_typeMap;
	map<CString, int>  m_languageMap;
public:
	afx_msg void OnNMRclickListShow(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnMenuDelete();
public:
	afx_msg void OnMenuEdit();
public:
	afx_msg void OnNMDblclkListShow(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeComboAcctype();
};
