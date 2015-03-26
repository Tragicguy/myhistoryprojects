#pragma once


// CDbGateAllSet 对话框

class CDbGateAllSet : public CDialog
{
	DECLARE_DYNAMIC(CDbGateAllSet)

public:
	CDbGateAllSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDbGateAllSet();

// 对话框数据
	enum { IDD = IDD_DLG_ALLSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	int CheckSendTime(CString strTime1,CString strTime2);
	BOOL PreTranslateMessage( MSG* pMsg );
	void InitParams();
	BOOL OnInitDialog();
private:
	//sql
	CString m_dbSQLServer;		//数据库服务器
	CString m_dbSQLName;		//数据库名
	CString m_dbSQLUser;		//登录账户
	CString m_dbSQLPwd;			//登录密码

	// 	//access
	// 	CString m_dbAccessPath;		//数据库文件路径
	// 	CString m_dbAccessPwd;		//数据库密码
	int m_iSplitLength;		//字符串拆分长度
	CString m_strAccount;		//登录账户
	CString m_strAccPwd;		//账户密码

	CString m_strServerIP;			//服务器IP
	int		m_iServerPort;		//服务器端口

	int m_iSendInterval;		//发送线程执行间隔
	int m_iBufNullWaitTime;		//缓冲区为空暂停时间
	int m_iSendBufLen;			//发送缓冲区大小

	//int m_iAddBufInterval;		//加入缓冲区线程执行间隔
	int m_iAddNullWaitTime;		//读数据库为空时暂停时间

	int m_iUseSubPort;			//是否使用子端口标志

	CString m_strMobileList;		//号段
	CString m_strUnicomList;
	CString m_strTelecomList;

	int m_iIsRecvReport;
	int m_iIsRecvMo;
	int m_iPhoneSplitLen;		//号码每包最大条数
	CString m_strRunTime;		//运行时间段配置

	CString m_strOldName;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnMore();
	afx_msg void OnBnClickedCheckRecvmo();
	afx_msg void OnBnClickedCheckRecvreport();
	afx_msg void OnBnClickedCheckReconn();
	afx_msg void OnBnClickedCheckDiffmobile();
	afx_msg void OnBnClickedRadioMssql();
	afx_msg void OnBnClickedRadioOracle();
	afx_msg void OnBnClickedRadioMysql();
	afx_msg void OnBnClickedRadioAccess();
public:
	afx_msg void OnBnClickedRadioKexintong();
public:
	afx_msg void OnBnClickedRadioSi();
};
