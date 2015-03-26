#pragma once
#include "mwmessagesend.h"
#include "ADOFile\AdoRecordSet.h"
#include "ASCIIConver.h"
#include "DataBaseCtrl.h"
#include "KeyWord.h"

class CMWMessageSendEx :public CMWMessageSend
{
	typedef struct tagThreadControl
	{
		CWinThread* pThread;
		BOOL ThreadExit;
		int iThreadNO;
	} ThreadControl;
	typedef struct tagThreadParam
	{
		CMWMessageSendEx* pThis;
		int iThreadNO;
	} ThreadParam;

public:
	CMWMessageSendEx(void);
	~CMWMessageSendEx(void);
public:
	void Run();
	void Stop( );
	void AppExit();
	int GetRunStatus();
	void SetRunStatus( int iFlag);
	void CheckFee();
	int IsNeedToReRunOnError();
	int IsNeedToReRunOnTime();
	void ReInitParams();
	void InitParams();
	void SetFrameHwnd(HWND hwnd);
	void AddSingleSend(CString strPhone,CString strContent,CString strSubport);
private:
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	bool IsNeedToRecv(unsigned int iDelay);		//是否可以读数据了
	void SetLastActiveTime( );					//上次连接正常的时间
	bool IsNeedReConn(unsigned int iDelay);		//是否需要重连	
	bool IsNeedToRecvReport(unsigned int iDelay);
	bool IsNeedToRecvMo(unsigned int iDelay);
	void SetLastRecvReportTime();
	void SetLastRecvMoTime();

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	int MessageOperate(int iOptype,int &iErrorType,CString &strError,KfSubmitPack &mtPack,DeliverPackList &recvList,ReportPackList &reportList);	//操作发送与接收

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	static void AddSendResultThread(LPVOID lpParam);	//添加发送结果线程
	void AddSendResultProc(int iThreadNO);//添加发送结果执行函数
	static void MessageSendThreadProc(LPVOID lpParam);		//发送线程函数
	void MessageSendProc();			//发送线程执行函数

	static void InsertBackThreadProc(LPVOID lpParam);		//回存线程函数
	void InsertBackProc();			//回存线程执行函数
	
	static void RecvThreadProc(LPVOID lpParam);		//接收线程函数
	void RecvProc();			//接收线程执行函数

	void Delay(int iMiniSecond,BOOL *bExit);
	void OnActiveThreadExit(int iFlag);
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	void AddKfSubmitPack(KfSubmitPack &submitPack);
	void AddKfSubmitPackEx(KfSubmitPack &submitPack);	//插到表头处
	int GetKfSubmitPack(KfSubmitPack &submitPack);
	void AddRealSendPack(KfSubmitPack &sendPack);
	int GetRealSendPack(KfSubmitPack &sendPack);
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	void ConvertPhoneAddMessage(KfSubmitPack &mt,int iFlag,CStringList &strIllegalPhones);
	void AddSendPackList( CString &strPhoneList,KfSubmitPack &mt,int iCount,int iSplitLen);
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	BOOL ReadWaitMessage(CDataBaseCtrl &dataBaseCtrl);
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/		
	int NextOptJudge(int iErrorCode,int iErrorType);	
	int IsRightSendTime(CString strSendTime,CString strStartTime,CString strEndTime);	
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	bool IsOverLastErrorTime(unsigned int iErrorStopTime );
	void SetLastErrorTime();
	int UnionSameSingleMessage(CDataBaseCtrl &dataBaseCtrl);
	int AddSendResult(CDataBaseCtrl &dataCtrl,KfSubmitPack &mt,int iFlag);
	int StringSplit(CString srcStr,CString strArray[],char splitChar);
	void GetMatchMtPack(KfSubmitPack &mt,int iLocalID);
	void LoadKeys();
	bool IsOverLastReconnTime(unsigned int iReconnInterval);
	void AddtoSendHistory(CDataBaseCtrl &dataCtrl,KfSubmitPack &mt,int iFlag,CString strError);
	void WriteBufToFile(KfSubmitPack &mt);
	void ReadBufFromFile();	
	int ComputePhoneType(CString &strPhone);
	void SetStatusInfoEx(int iIndex,int iCount);
private:
	long m_LastRecvMoTime;
	CCriticalSection m_LastRecvMoTimeLock;

	long m_LastRecvReportTime;
	CCriticalSection m_LastRecvReportTimeLock;

	DWORD m_LastActiveTime;		//上次连接时间
	CCriticalSection m_LastActiveTimeLock;	//上次连接时间锁

	KfSubmitPackList m_SubmitPackList;		//发送缓冲
	CCriticalSection m_SendListLock;		//发送链表锁

	KfSubmitPackList m_RealSendList;		//真正发送链表
	CCriticalSection m_RealSendLock;		//真正发送链表锁
	
	KfSubmitPackList m_SendHistoryList;		//发送历史
	CCriticalSection m_SendHistoryLock;		//发送历史锁

	ThreadControl m_SendThreadContrl;	//发送线程
	ThreadControl m_AddResultThreadContrl[5];	//保存发送结果线程
	ThreadControl m_RecvThreadContrl;	//发送线程
	ThreadControl m_InsertBackContrl;	//回存线程
	int m_iActiveThreadNum;
	CCriticalSection m_activeNumLock;
	bool m_CheckFeeFlag;
	int m_iRunStatus;
	long m_iLastErrorTime;		//最后一次发送出错时间
	int m_iIsSendThreadRun;
	int m_iIsRecvThreadRun;
	CCriticalSection m_IsSendThreadRunLock;
	CCriticalSection m_IsRecvThreadRunLock;
	/************************************************************************/
	//通用配置	
	int m_iSplitLength;			//字符串拆分长度[移动]
	int m_iSplitLengthUnicom;			//字符串拆分长度[联通]
	int m_iSplitLengthChinaCom;			//字符串拆分长度[电信]
	int m_iSplitLengthChina;			//字符串拆分长度[小灵通]
	int m_iPhoneSplitLen;		//号码每包最大条数

	CString m_dbSQLServer;		//数据库服务器
	CString m_dbSQLName;		//数据库名
	CString m_dbSQLUser;		//登录账户
	CString m_dbSQLPwd;			//登录密码

// 	CString m_dbAccessPath;		//数据库文件路径
// 	CString m_dbAccessPwd;		//数据库密码
	
	CString m_strAccount;		//登录账户
	CString m_strAccPwd;		//账户密码

	CString m_strServerIP;		//服务器IP
	int		m_iServerPort;		//服务器端口

	int m_iSendInterval;		//发送线程执行间隔
	int m_iBufNullWaitTime;		//缓冲区为空暂停时间
	
	int m_iAddNullWaitTime;		//读数据库为空时暂停时间
	int m_iUseSubPort;			//是否使用子端口标志

	CString m_strMobileList;		//号段
	CString m_strUnicomList;
	CString m_strTelecomList;
	int		m_iDiffMobile;		//区分移动与联通号段

	//是否接收状态报告及上行的标志
	int m_iIsRecvReport;
	int m_iIsRecvMo;
	int m_iRecvMoInterval;		//接收上行时间间隔
	int m_iRecvReportInterval;	//接收状态报告时间间隔

	int m_iAutoReRun;			//自动重运行(根据可运行时间段)
	CString m_strRunTime;		//运行时间段配置
	int m_iErrorStopTime;		//出错停止时间
	int m_iCheckKey;			//检测非法关键词

	int m_iIsReconn;			//是否自动重连
	int m_iReconnInterval;		//自动重连间隔
	long m_iLastReconnTime;		//上次自动重连时间
	int m_iTestConInterval;		//激活连接间隔
	//添加发送记录线程
	int m_iAddHistThreadNum;	//添加历史线程数
	int m_iAddHistInterval;		//执行间隔
	int m_iAddHistNullWait;		//无历史需要写入时暂停
	int m_iNumSendStop;			//当历史表记录超过*条时暂停发送
	
	int m_iDbType;			//数据库类型
	int m_iDbPort;			//数据库端口号,只在MYSQL或ORACLE时需要
	CKeyWord m_keyWord;		//非法关键词检测对象
	map<int,KfSubmitPack>  m_SendMatchList;//信息合并链表(匹配)

	HWND m_hWnd;			//主窗口句柄

	CString m_strLastSingleMsg;	//上一次单发的信息内容
	int m_iSameSingleSendTime;	//连续相同信息单发次数
	int m_iMostSameSingleSendTime;	//允许的最大连续相同信息单发次数
};
