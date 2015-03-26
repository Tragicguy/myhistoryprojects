#pragma once
#include "dataType.h"
#include "TcpClient.h"
#define  USE_DLL___    //控制是否通过动态库发送
//定义错误类型
#define  MSG_ERROR_INIT			1
#define  MSG_ERROR_LOGIN		2
#define  MSG_ERROR_SEND			3
#define  MSG_ERROR_RECVMO		4
#define  MSG_ERROR_RECVREPORT	5
#define  MSG_ERROR_KEEPONLINE	6
#define  MSG_ERROR_MODIFYPWD	7
#define  MSG_ERROR_CHECKFEE		8

//操作类型
#define  OPTYPE_LOGIN			1
#define  OPTYPE_SEND			2
#define  OPTYPE_RECVMO			3
#define  OPTYPE_RECVREPORT		4
#define  OPTYPE_MODIFYPWD		5
#define  OPTYPE_CHECKFEE		6

#define MWGATE_DLL_NAME "EMWGateway.dll"	//动态库名
#define MWGATE_DLL_NAME_SMEIDLL "SMEIDll.dll"	//动态库名

#ifdef USE_DLL___
class CMWMessageSend
#else
class CMWMessageSend : CTcpClient
#endif
{
public:
	CMWMessageSend(void);
	~CMWMessageSend(void);
	int SendMtMessage(LPCTSTR sMsg, LPCTSTR sDestAddrs,int byUserNum,LPCTSTR sSubPort,unsigned char* byMsgID,CString &strError);//发送下行信息	
	BOOL InitInterface();	//初始化接口
	BOOL ExitInterface();	//释放接口
	void SetParams(CString strServerIP,int iPort,CString strUserID,CString strPwd);	//设置用户信息
	int ConnectAndLogin(CString &strError); //连接并登录服务器
	int GetMoMessges(DeliverPackList &recvPackList,CString &strError );  //接收上行
	int GetMsgReports(ReportPackList &reportPackList,CString &strError );//接状态报告
	int ChangePassword(CString strNewPwd,CString &strError);	//修改密码
	int GetFee(int &iUsed,int &iLeft,CString &strError);	//查询已用与剩余条数
	int KeepOnline(CString &strError);		//保持连接
	void Disconnect();						//断开连接
	void SetCmdTypeEx(int iCmdType,int iRptFlag);
private:
	int m_sock;		//与服务器通讯的SOCKET
	HMODULE m_handle;	//加载动态库的句柄
	HMODULE m_handle_smeidll;
	char m_sServerIP[20]; //服务器IP
	int m_iport;			//服务器端口
	char m_sUserID[32+1];	//用户ID
	char m_sPassWord[32+1];	//用户密码
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	CString GetServerErrorStr(int iErrorCode);	//返回服务器错误
	CString GetLoginErrorStr( int iErrorCode );	//返回登录错误字符串
	CString GetSendMsgErrorStr( int iErrorCode );
	CString GetMoReportErrorMsg(int iErrorCode);
	int AddRePortPack(ReportPackList &reportPackList, CString strMsg);
	int AddDeleverPack(DeliverPackList &recvPackList,CString strMsg);
	void TranMsgIdI64ToChar(unsigned char* MsgId, __int64 IMsgId);
	int TranMsgId(unsigned char *pSrc, char *MsgId);
	/************************************************************************/
	/* 以下为动态库函数                                                      */
	/************************************************************************/
#ifdef USE_DLL___
	typedef int (__stdcall *FnMongateConnect)(const char* pszIP, int iPort,
		const char* pszAccount, const char* pszPwd);
	//typedef int (__stdcall *FnMongateCsSendSms)(int sock, const char* pszMobis, const char* pszMsg,
	//	int iMobiCount, char* pszSN);
	typedef int (__stdcall *FnMongateCsSPSendSms)(int sock, const char* pszMobis,
		const char* pszMsg, int iMobiCount,const char* pszSubPort,char* pszSN);
	typedef void (__stdcall *FnMongateDisconnect)(int sock);
	//typedef int (__stdcall *FnMongateCsGetSms)(int sock, char strMsg[500][255]);
	typedef int (__stdcall *FnMongateCsGetSmsEx)(int sock, char *strMsg);//[1600*500]
	//typedef int (__stdcall *FnMongateSetCmdType)(int iCmdType);
	typedef int (__stdcall *FnMongateQueryBalance)(int sock);
	typedef int (__stdcall *FnMongateQueryUsed)(int sock);
	typedef int (__stdcall *FnMongateTestConn)(int sock);		//维持连接
	typedef int (__stdcall *FnMongateCsGetStatusReportEx)(int sock, char *strMsg);
	typedef int (__stdcall *FnMGhexChrTosrcStr)(const char* srcMsg,char* retMsg);
	typedef int (__stdcall *FnMongateChangePwd)(int sock, const char* pszAccount,
		const char* pszOldPwd, const char* pszNewPwd);
	typedef int (__stdcall *FnSetCmdType)(int iCmdType,int iRptFlag);
	
	FnMongateConnect MongateConnect;
	FnMongateCsSPSendSms MongateCsSPSendSms;
	FnMongateDisconnect MongateDisconnect;
	FnMongateCsGetSmsEx MongateCsGetSmsEx;
	FnMongateQueryBalance MongateQueryBalance;
	FnMongateQueryUsed MongateQueryUsed;
	FnMongateCsGetStatusReportEx MongateCsGetStatusReportEx;
	FnMGhexChrTosrcStr MGhexChrTosrcStr;
	FnMongateChangePwd MongateChangePwd;
	FnMongateTestConn MongateTestConn;
	FnSetCmdType	SetCmdType;
#else
#endif
	/////////////////////////////////////////////////////
	//动态库函数接口
	/////////////////////////////////////////////////////
// 	typedef int (FAR __stdcall *_IFInitInterface)(DWORD dwCodeProtocol, DWORD dwDriverProtocol, LPCTSTR pDriverParam);
// 	typedef int (FAR __stdcall *_IFExitInterface)();
// 	typedef int (FAR __stdcall *_Login_R)(LPCTSTR Account, LPCTSTR Password);
// 	typedef int (FAR __stdcall *_SendSms)
// 		(
// 		const char* sOrgAddr,		//源地址
// 		const char* sPhones,		//手机号，最多一次发送100个，用英文逗号分隔
// 		unsigned char ReportFlag,	//是否要状态报告,1需要，0不需要
// 		const char *sMsg,		    //短信内容
// 		unsigned char *sMsgID,		//返回的msgId
// 		unsigned char* FCS		    //返回的错误码
// 		);
// 
// 	typedef int (FAR __stdcall *_RecvSms)
// 		(
// 		char *sOrgAddr,			//源地址
// 		char *sPhone,				//手机号
// 		unsigned char *sMsgID,		//上行或状态报告的msgid
// 		unsigned char *ReportFlag, //信息标志 0:状态报告 1:上行
// 		char *sTimeStamp,			//接收到的时间 格式：2009-10-28 12:12:12	
// 		char *sSubmitdate,			//提交时间   200910281212
// 		char *sDonedate,			//受理时间200910281212
// 		char *sErrorCode,			//状报告错误码
// 		char *sMsg				    //上行信息
// 		);
// 	_IFInitInterface IFInitInterface = NULL;
// 	_IFExitInterface IFExitInterface = NULL;
// 	_Login_R         Login_R = NULL;
// 	_SendSms         SendSms = NULL;
// 	_RecvSms         RecvSms = NULL;

};