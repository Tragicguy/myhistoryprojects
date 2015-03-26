#pragma once
//状态报告结构体
#include <afxmt.h>
#include <afxtempl.h>
#include "ASCIIConver.h"
#define STATUS_RUN_NORMAL 0
#define STATUS_STOP_ERROR 1
#define STATUS_STOP_MANUAL   2
#define STATUS_STOP_OVERRUNTIME 3
#define STATUS_STOP_NORMAL 4

#define USERMSGSTART	 WM_USER + 500
#define USERWINDOWMSG_1  USERMSGSTART + 1
#define USERWINDOWMSG_2  USERMSGSTART + 2
#define USERWINDOWMSG_3  USERMSGSTART + 3
#define USERWINDOWMSG_4  USERMSGSTART + 4
#define USERWINDOWMSG_5  USERMSGSTART + 5
#define USERWINDOWMSG_6  USERMSGSTART + 6
#define USERWINDOWMSG_7  USERMSGSTART + 7
#define USERWINDOWMSG_8  USERMSGSTART + 8
#define USERWINDOWMSG_9  USERMSGSTART + 9
#define USERWINDOWMSG_10 USERMSGSTART + 10
#define USERWINDOWMSG_11 USERMSGSTART + 11
#define USERWINDOWMSG_12 USERMSGSTART + 12

#define USERMSGSHOWINFO USERMSGSTART + 13
#define USERMSGSHOWDISPINFO USERMSGSTART + 14
#define WM_USERMSGSEND		USERMSGSTART + 15
#define WM_USERMSGSTATUS		USERMSGSTART + 85	//状态栏消息宏
typedef struct ReportPack
{
	CString strDate;		//接收日期
	CString strTime;		//接收时间
	CString strMsgNo;		//信息编号
	CString strReserve;		//保留
	CString strMsgStatus;	//信息状态
	CString strErrorCode;	//信息错误码	
	ReportPack()
	{
		strDate.Empty();
		strTime.Empty();
		strMsgNo.Empty();
		strReserve.Empty();
		strMsgStatus.Empty();
		strErrorCode.Empty();
	}
}ReportPack;

//上行信息结构体
typedef struct DeliverPack
{
	CString strDate;		//接收日期
	CString strTime;		//接收时间
	CString strOrgAddr;		//手机号码
	CString strDestAddr;	//接收方号码
	CString strReserve;		//保留
	CString strMsg;			//信息内容
	DeliverPack()
	{
		strDate.Empty();
		strTime.Empty();
		strOrgAddr.Empty();
		strDestAddr.Empty();
		strReserve.Empty();
		strMsg.Empty();
	}
}DeliverPack;

typedef struct KfSubmitPack
{
	int m_iSendType;				//发送类型     
	int m_iCount;					//接收号码数
	int m_iCountA;					//临时接收号码数
	int m_iPhoneType;				//所属号段
	int m_iTryTimes;				//重试次数
	int m_iMsgLen;					//信息长度
	int m_iPrecedence;				//短信优先级

	int m_iLocalID;					//本地ID(INT型)
	int m_iReverse1;				//保留字段1
	int m_iReverse2;				//保留字段2
	int m_iReverse3;				//保留字段3
	int m_iReverse4;				//保留字段4
	int m_iReverse5;				//保留字段5
	int m_iSendResultFlag;			//发送结果，0成功，1失败
	CString m_strSendType;			//发送类型		
	CString m_strUserNumber;		//接收方号码列表
	CString m_strUserNumberA;		//临时接收方号码列表
	CString m_strPtMsgId;			//消息本地标识
	CString m_strSpMsgId;			//消息ID(发送成功后的信息编号)
	CString m_strMsg;				//短信内容
	CString m_strCreatTime;		//信息生成时间
	CString m_strSubTime;			//信息提交到数据库时间
	COleDateTime m_timeSendTime;			//发送时间

	CString m_strOperatorID;		//操作员ID
	CString m_strServiceID;			//服务ID
	CString m_strTaskName;			//任务名
	CString m_strLocalID;			//信息本地ID
	CString m_strSubOprationType;	//子操作类型
	CString m_strSubPort;			//子端口

	CString m_strUserId;			//保留字段
	CString m_strMsgStatus;			//保留字段1
	CString m_strRecvStatus;			//保留字段2
	CString m_strMainPort;			//保留字段3
	CString m_strFailReason;			//保留字段4
	CString m_strReverse5;			//保留字段5
	CString m_strReverse6;			//保留字段6
	CString m_strIdList;			//用于保存合并后的信息ID集合
	KfSubmitPack()
	{
		m_iSendType = 0;	
		m_iCount = 0;					//接收号码数
		m_iCountA = 0;					//临时接收号码数
		m_iPhoneType = 0;				//所属号段
		m_iTryTimes = 0;				//重试次数
		m_iMsgLen = 0;					//信息长度
		m_iPrecedence = 0;				//短信优先级

		m_iLocalID = 0;				//保留字段6
		m_iReverse1 = 0;				//保留字段1
		m_iReverse2 = 0;				//保留字段2
		m_iReverse3 = 0;				//保留字段3
		m_iReverse4 = 0;				//保留字段4
		m_iReverse5 = 0;				//保留字段5		
		m_iSendResultFlag = 0;
		m_strSendType.Empty();			//发送类型
		m_strUserId.Empty();			
		m_strUserNumber.Empty();		//接收方号码列表
		m_strUserNumberA.Empty();		//临时接收方号码列表
		m_strPtMsgId.Empty();			//消息本地标识
		m_strSpMsgId.Empty();			//消息标识2  //消息ID(发送成功后的信息编号)
		m_strMsg.Empty();				//短信内容
		m_strCreatTime.Empty();			//信息生成时间
		m_strSubTime.Empty();			//信息提交到数据库时间
		//m_timeSendTime.Empty();			//发送时间

		m_strOperatorID.Empty();		//操作员ID
		m_strServiceID.Empty();			//服务ID
		m_strTaskName.Empty();			//任务名	
		m_strLocalID.Empty();			//信息本地ID	
		m_strSubOprationType.Empty();	//子操作类型	
		m_strSubPort.Empty();			//子端口

		m_strMsgStatus.Empty();			//保留字段1
		m_strRecvStatus.Empty();			//保留字段2
		m_strMainPort.Empty();			//保留字段3
		m_strFailReason.Empty();			//保留字段4
		m_strReverse5.Empty();			//保留字段5
		m_strReverse6.Empty();			//保留字段6
		m_strIdList.Empty();
	}

}KfSubmitPack;

typedef CArray<DeliverPack, DeliverPack> DeliverPackList;
typedef CArray<ReportPack, ReportPack> ReportPackList;
typedef CArray<KfSubmitPack, KfSubmitPack> KfSubmitPackList;
extern CCriticalSection g_DisListLock;
extern CListCtrl g_DispList;
extern CListCtrl g_RecvList;
extern CListCtrl g_SentList;
extern CListCtrl g_ReportList;
extern int g_iSendBufLen;
extern int g_iIsNeedToLog;
extern int g_iDebugLog;		//写调试日志标志
extern int g_iServerType;	//接入平台选择，0为客馨通平台，1为SI平台
extern int g_iReportFlag;
extern CASCIIConver g_Conver;
extern int g_iBalance;	//余额
extern CString g_strWindowName;
extern CString g_strVirsion;
extern CString g_strServiceName;
extern HWND g_hwndClose;		//关闭窗口句柄
extern void WriteConsoleInfo(CString srcStr);	//写控制台信息
extern void WriteLog(CString srcStr,int iFlag);
extern void WriteLogEx(CString srcStr,int iFlag);//写调试日志
extern void AddToDispListShow(CString showStr,int iFlag=0);		//显示信息
extern void AddToRecvListShow(CString strTime,CString orgAddr,CString strContent,CString strDestAddr );
extern void AddToSentListShow(CString strMsgID,CString destAddr,CString strContent,int iMsgLen,int iUserNum,int iPhoneType,CString strStatus,int iFlag,CString strOptId );
extern void AddToRePortListShow( CString strTime,CString strMsgID,CString strState );
