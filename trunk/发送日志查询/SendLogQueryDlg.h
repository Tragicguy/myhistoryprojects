// SendLogQueryDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxdtctl.h"
#include "ADOFile/AdoRecordSet.h"
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
	void OperateProc(int iFlag);
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

	CString m_strStartDate,m_strStartTime;
	CString m_strEndDate,m_strEndTime;
public:
	afx_msg void OnBnClickedBtnQuery();
	void InitCtrList();
	void QuerySendLog(CString strWhere,int iFlag,BOOL *bExit,int iCurrPage,BOOL bDetail);
	CListCtrl m_resultList;
	afx_msg void OnBnClickedBtnFirst();
	afx_msg void OnBnClickedBtnPre();
	afx_msg void OnBnClickedBtnNext();
	afx_msg void OnBnClickedBtnLast();
	afx_msg void OnBnClickedRadioSenddelay();
	afx_msg void OnBnClickedRadioSendnow();
	afx_msg void OnBnClickedRadioNowordelay();
	afx_msg void OnBnClickedRadioSuccess();
	afx_msg void OnBnClickedRadioFail();
	afx_msg void OnBnClickedRadioSucorfial();
	afx_msg void OnBnClickedBtnStop();
	CTime GetCTimeFromString(CString strTime);
	CDateTimeCtrl m_TimeCtrl;
	afx_msg void OnClose();
	int GetAppPath(CString &strAppPath);
	CDateTimeCtrl m_TimeEndCtrl;
	afx_msg void OnBnClickedBtnOutport();
	CString GetExcelDriver();
	void ExportToExcel(CString strTitle,CString strWhere,int iFlag,BOOL *bExit,int iCurrPage,BOOL bDetail);
	BOOL GetDefaultXlsFileName(CString& sExcelFile);
	BOOL MakeSurePathExists( CString &Path,bool FilenameIncluded);
	void GetDaySucCount(CString strStartDay,CString strEndDay,CAdoConnection &adoConn,CAdoRecordSet &adoRst);
	void ExportToExcelEx(CString strTitle);
	CListCtrl m_showDay;
};
