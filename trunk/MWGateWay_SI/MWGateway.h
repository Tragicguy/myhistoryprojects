// MWGateway.h
// 文件说明: 深圳梦网 双向网关 客服网关 动态库 函数头文件
//   版本号: 1.2.0.0
//     时间: 2006-03-01
#define USE_SMEIDLL_DLL_
#ifdef WIN32
  #pragma once
#else
  #define SOCKET int
  #define __stdcall
#endif


char* hexChrTosrcStr(const char* strMsg,char* retstr);
void srcStrTohexChr(const char* strMsg,char* retstr);
int __stdcall SetUserType(const char* Os,int SoftType,const char* Version,const char* sPwd);
//---------------------------------------------------------------------------
// 通用返回值说明
//   函数执行成功，返回值为0，或者正数，通常为1
//   函数执行失败，返回值为负数，
//   若返回值小于 -100 大于 -200，则表示非预期的错误，即发生了不应该产生的错误
//       发生该错误时，请关闭连接后，重新连接
//   若返回值小于 -10000，则其值的绝对值减去10000，即为平台的返回值

//---------------------------------------------------------------------------
// 发送信息说明
//   1.请注意，发送信息中有一些关键字被平台列出来，将无法提交成功
//   2.发送的信息，必须为多字节编码，信息内容最大长度为68个字，一个英文字符也
//     算一个字


//---------------------------------------------------------------------------
//                                ***连接***
// MongateConnect
// 参数说明：
//        pszIP: [in] 服务器IP，通常为"server1.montnets.com"或者为"61.242.89.115"
//        iPort: [in] 服务器端口，通常为 8018
//   pszAccount: [in] 登陆帐户名
//       pszPwd: [in] 登陆帐户密码
// 返回值：
//  正值: 登陆成功，该值为socket句柄
//    -1: const char*, char* 这样的参数中，包含有NULL值
//    -2: 非法长度的登陆帐号，或密码
//    -3: 获取远程主机信息失败,原因:IP解析失败或无效的IP,或网络不通
//    -4: socket创建失败
//    -5: connect失败
//    -6: connect成功,等待平台返回连接确认信息超时
//    -7: 接收平台返回登陆验证信息超时
//    -8: 错误的用户名或密码
//---------------------------------------------------------------------------
int __stdcall MongateConnect(const char* pszIP, int iPort
                             , const char* pszAccount, const char* pszPwd);

//---------------------------------------------------------------------------
//                            ***发送单向信息***
// MongateSendSms
// 参数说明：
//      sock: [in] 连接句柄，该句柄由MongateConnect返回
//   pszMobi: [in] 接收方手机号
//    pszMsg: [in] 信息内容
//   返回值:
//      1: 发送信息成功提交到平台
//     -1: const char*, char* 这样的参数中，包含有NULL值
//     -2: 非法手机号
//     -3: 等待发送超时
//     -4: 等待接收超时
//---------------------------------------------------------------------------
int __stdcall MongateSendSms(SOCKET sock, const char* pszMobi,
                             const char* pszMsg);
int __stdcall MongateSendSmsBeta(SOCKET sock, const char* pszMobi,
                             const char* pszMsg, char* pszSN);

//---------------------------------------------------------------------------
//                             ***测试连接***
// MongateTestConn
// 参数说明:
//   sock: [in] 连接句柄
//  返回值:
//     1: 该sock有效
//    -1: 等待发送超时
//    -2: 等待接收超时
//    -3: 该sock无效
//---------------------------------------------------------------------------
int __stdcall MongateTestConn(SOCKET sock);
int __stdcall MongateTestConnHttp(SOCKET sock);

//---------------------------------------------------------------------------
//                             ***查询余额***
// MongateQueryBalance
// 参数说明:
//   sock: [in] 连接句柄
// 返回值:
//    >=0: 为余额
//     -1: 等待发送超时
//     -2: 等待接收超时
//---------------------------------------------------------------------------
int __stdcall MongateQueryBalance(SOCKET sock);

//---------------------------------------------------------------------------
//                        ***查询已经使用条数***
// MongateQueryUsed
// 参数说明:
//   sock: [in] 连接句柄
// 返回值:
//    >=0: 为已经使用条数
//     -1: 等待发送超时
//     -2: 等待接收超时
//---------------------------------------------------------------------------
int __stdcall MongateQueryUsed(SOCKET sock);

//---------------------------------------------------------------------------
//                            ***充值***
// MongateRecharge
// 参数说明:
//        sock: [in] 连接句柄
//   pszCardNo: [in] 充值卡卡号
//  pszCardPwd: [in] 充值卡密码
// 返回值:
//   >0: 为帐户剩余条数
//   -1: const char*, char* 这样的参数中，包含有NULL值
//   -2: 充值卡卡号或密码长度无效
//   -3: 等待发送超时
//   -4: 等待接收超时
//---------------------------------------------------------------------------
int __stdcall MongateRecharge(SOCKET sock, const char* pszCardNo,
                              const char* pszCardPwd);

//---------------------------------------------------------------------------
//                            ***修改密码***
// MongateChangePwd
// 参数说明:
//         sock: [in] 连接句柄
//   pszAccount: [in] 帐号
//    pszOldPwd: [in] 帐号当前密码
//    pszNewPwd: [in] 帐号新密码
// 返回值:
//    1: 成功
//   -1: const char*, char* 这样的参数中，包含有NULL值
//   -2: 用户名或密码长度非法
//   -3: 等待发送超时
//   -4: 等待接收超时
//---------------------------------------------------------------------------
int __stdcall MongateChangePwd(SOCKET sock, const char* pszAccount,
                               const char* pszOldPwd, const char* pszNewPwd);

//---------------------------------------------------------------------------
//                            ***断开连接***
// MongateDisconnect
// 参数说明:
//    sock: [in] 连接句柄
// 返回值: 无
//---------------------------------------------------------------------------
void __stdcall MongateDisconnect(SOCKET sock);

//---------------------------------------------------------------------------
//                         ***增值网关，获取上行信息***
// MongateVasGetSms
// 参数说明:
//     sock: [in]  连接句柄
//   strMsg: [out] 用来存组信息的数组
// 返回值:
//    0-10: 为从平台返回
//      -1: strMsg为NULL
//      -2: 等待发送超时
//      -3: 等待接收超时
//---------------------------------------------------------------------------
int __stdcall MongateVasGetSms(SOCKET sock, char strMsg[500][255]);


//---------------------------------------------------------------------------
//                      ***增值网关，发送信息***
// MongateVasSendSms
// 参数说明:
//          sock: [in] 连接句柄
//    pszFeeCode: [in] 费率代码
//  pszServiceNo: [in] 服务号码,即下行时的端口号
//   pszOperCode: [in] 参数组(又叫操作码)
//     pszLinkID: [in] linkid
//      pszMobis: [in] 接收方手机号组 如:"13500000000,13600000000"
//        pszMsg: [in] 下行内容
//    iMobiCount: [in] 接收手机号码组中的手机个数
// 返回值:
//    1: 成功
//   -1: const char*, char* 这样的参数中，包含有NULL值
//   -2: pszMobis手机号码检查 不合格
//   -3: 等待发送超时
//   -4: 等待接收超时
//  -10: 分配堆空间失败
//---------------------------------------------------------------------------
int __stdcall MongateVasSendSms(SOCKET sock, const char* pszFeeCode,
                                 const char* pszServiceNo,
                                 const char* pszOperCode,
                                 const char* pszLinkID,
                                 const char* pszMobis,
                                 const char* pszMsg, int iMobiCount);								 
int __stdcall MongateVasSendSmsBeta(SOCKET sock, const char* pszFeeCode,
                                 const char* pszServiceNo,
                                 const char* pszOperCode,
                                 const char* pszLinkID,
                                 const char* pszMobis,
                                 const char* pszMsg, int iMobiCount,
								 char* pszSN); 
//---------------------------------------------------------------------------
//                         ***客服网关，获取上行信息***
// MongateGetCsSms
// 参数说明:
//     sock: [in]  连接句柄
//   strMsg: [out] 用来存组信息的数组
// 返回值:
//    0-10: 为从平台返回
//      -1: strMsg为NULL
//      -2: 等待发送超时
//      -3: 等待接收超时
//---------------------------------------------------------------------------
int __stdcall MongateCsGetSms(SOCKET sock, char strMsg[500][255]);
int __stdcall MongateCsGetSmsEx(SOCKET sock, char *strMsg);


//---------------------------------------------------------------------------
//                       ***客服网关，发送信息***
// MongateCsSendSms
// 参数说明:
//        sock: [in] 连接句柄
//    pszMobis: [in] 手机号码组 如:"13500000000,13600000000"
//      pszMsg: [in] 信息内容
//  iMobiCount: [in] 手机号码组中,手机号个数
// 返回值:
//    1: 成功
//   -1: const char*, char* 这样的参数中，包含有NULL值
//   -2: pszMobis手机号码检查 不合格
//   -3: 等待发送超时
//   -4: 等待接收超时
//  -10: 分配堆空间失败
//---------------------------------------------------------------------------
int __stdcall MongateKxtSend(SOCKET sock, const char* pszMobis,
							   const char* pszMsg, int iMobiCount, char* pszSN);
int __stdcall MongateCsSendSms(SOCKET sock, const char* pszMobis,
                            const char* pszMsg, int iMobiCount, char* pszSN);

//---------------------------------------------------------------------------
//                         ***客服网关，取状态报告***
// MongateCsGetStatusReport
// 参数说明:
//     sock: [in]  连接句柄
//   strMsg: [out] 用于存放状态信息
// 返回值:
//     1: 成功
//    -1: strMsg为NULL
//    -2: 等待发送超时
//    -3: 等待接收超时
//---------------------------------------------------------------------------
int __stdcall MongateCsGetStatusReport(SOCKET sock, char strMsg[500][255]);

int __stdcall MongateCsGetStatusReportEx(SOCKET sock, char *strMsg);


//---------------------------------------------------------------------------
//                       ***获取动态库版本***
// MongateGetVer
// win版本库,请查看dll属性中的版本号即可
//---------------------------------------------------------------------------
const char* __stdcall MongateGetVer();
void TestMsg(const char* mystr);


//---------------------------------------------------------------------------
//                       ***客服网关，子端口发送信息***
// MongateCsSPSendSms
// 参数说明:
//        sock: [in] 连接句柄
//    pszMobis: [in] 手机号码组 如:"13500000000,13600000000"
//      pszMsg: [in] 信息内容
//  iMobiCount: [in] 手机号码组中,手机号个数
//   psSubPort: [in] 客服子端口编号
//     msgPkid: [in] 用户自定义信息包编号，默认为(*)暂不开放
//       pszSN: [out] 信息编号
// 返回值:
//    1: 成功
//   -1: const char*, char* 这样的参数中，包含有NULL值
//   -2: pszMobis手机号码检查 不合格
//   -3: 等待发送超时
//   -4: 等待接收超时
//  -10: 分配堆空间失败
//---------------------------------------------------------------------------
int __stdcall MongateCsSPSendSms(SOCKET sock, const char* pszMobis,
   const char* pszMsg, int iMobiCount, const char* pszSubPort,char* pszSN);

int __stdcall MongateCsSPSendSmsHttp(SOCKET sock, const char* pszMobis,
	const char* pszMsg, int iMobiCount, const char* pszSubPort,char* pszSN);
   //int msgPkid,char* pszSN);
//---------------


/*-------------------------------------20070326 fq---------------------------------------------*/

//---------------------------------------------------------------------------
//                      ***定时信息发送***
//参数说明:
//	sock:[in] 连接句柄
//  iType:[in] 定时类型,0 表示'只发一次',1表示'每天发送',2'表示每周发送',5表示'每月发送'
//	pBeginDate: [in]开始日期,定时开始执行的日期。格式为2006-01-01
//	pEndDate：[in]结束日期，定时结束执行的日期。格式为2006-01-01
//  iFlag：[in]定时参数, 动态调整：
//       “只发一次”定时：为定时日期:格式20060101
//       “每天发送”定时：为0
//       “每周发送”定时：为4字节的周组合数字,从右到左每bit代表一天, 1为选中
//                         发送,0为不发送.如“0011111”为周一到周五发送，周六
//                         日不发送
//       “每月发送”定时：为4字节数字转换的字符串的日组合字符串,从右到左每bit
//                         位代表一天,1为选中发送,0为不发送
//                         如二进制“01111111111111111111111111111111”,转换十
//                         进制位2的31次方，31天都发送，转换值0为月末发送。
//  Time：[in]定时时间: 格式为15:30:01
//  iMobiCount：[in] 手机号码组中,手机号个数
//  pszMsg：[in] 信息内容
//  pTelNos：[in] 接收方手机号组 如:"13500000000,13600000000"
//---------------------------------------------------------------------------
int __stdcall MongateCsSendTime(SOCKET sock,
								int iType,
                                const char* pBeginDate,
                                const char* pEndDate,
                                int  iFlag,
                                const char* Time, 
								int iMobiCount,
								const char* pszMsg,
								const char* pTelNos);	

int __stdcall MongateCsSendTimeHttp(SOCKET sock,
								int iType,
                                const char* pBeginDate,
                                const char* pEndDate,
                                int  iFlag,
                                const char* Time, 
								int iMobiCount,
								const char* pszMsg,
								const char* pTelNos);	

//---------------------------------------------------------------------------
//                      ***更新定时信息发送***
//参数说明:
//	sock:[in] 连接句柄
//  iNumber:[in] 定时编号
//  iType:[in] 定时类型,0 表示'只发一次',1表示'每天发送',2'表示每周发送',5表示'每月发送'
//	pBeginDate: [in]开始日期,定时开始执行的日期。格式为2006-01-01
//	pEndDate：[in]结束日期，定时结束执行的日期。格式为2006-01-01
//  iFlag：[in]定时参数, 动态调整：
//       “只发一次”定时：为定时日期:格式20060101
//       “每天发送”定时：为0
//       “每周发送”定时：为4字节的周组合数字,从右到左每bit代表一天, 1为选中
//                         发送,0为不发送.如“0011111”为周一到周五发送，周六
//                         日不发送
//       “每月发送”定时：为4字节数字转换的字符串的日组合字符串,从右到左每bit
//                         位代表一天,1为选中发送,0为不发送
//                         如二进制“01111111111111111111111111111111”,转换十
//                         进制位2的31次方，31天都发送，转换值0为月末发送。
//  Time：[in]定时时间: 格式为15:30:01
//  iMobiCount：[in] 手机号码组中,手机号个数
//  pszMsg：[in] 信息内容
//  pTelNos：[in] 接收方手机号组 如:"13500000000,13600000000"
//---------------------------------------------------------------------------
int __stdcall MongateCsUpdateSendTime(SOCKET sock,
									  int iNumber,
								      int iType,
                                      const char* pBeginDate,
                                      const char* pEndDate,
                                      int  iFlag,
                                      const char* Time, 
								      int iMobiCount,
								      const char* pszMsg,
								      const char* pTelNos);

int __stdcall MongateCsUpdateSendTimeHttp(SOCKET sock,
									  int iNumber,
								      int iType,
                                      const char* pBeginDate,
                                      const char* pEndDate,
                                      int  iFlag,
                                      const char* Time, 
								      int iMobiCount,
								      const char* pszMsg,
								      const char* pTelNos);



//---------------------------------------------------------------------------
//                      ***删除指定定时信息***
//参数说明:
//	sock:[in] 连接句柄
//  iNum:[in] 定时编号
//---------------------------------------------------------------------------
int __stdcall MongateCsDelSendTime(SOCKET sock,
								   int iNum);

int __stdcall MongateCsDelSendTimeHttp(SOCKET sock,
								   int iNum);
	
//---------------------------------------------------------------------------
//                      ***删除所有指定定时信息***
//参数说明:
//	sock:[in] 连接句柄
//---------------------------------------------------------------------------
int __stdcall MongateCsDelAllSendTime(SOCKET sock);

int __stdcall MongateCsDelAllSendTimeHttp(SOCKET sock);

//---------------------------------------------------------------------------
//                      ***读取定时基本资料***
//参数说明:
//	sock:[in] 连接句柄
//  iNumber:[in] 定时编号
//  iType:[in] 读取类型: 为读取类型，1为等于该定时编号，2为大于该定时编号,服务器可能一
//	        次只返回部分定时基本资料
//  pStrMessage:[out] 读取服务器返回信息
//---------------------------------------------------------------------------
int __stdcall MongateCsReadSendTime(SOCKET sock,
									int iNumber,
								    int iType,
									char strMsg[10][1024]
									);

int __stdcall MongateCsReadSendTimeEx(SOCKET sock,int iNumber,int iType,char *strMsg);

int __stdcall MongateCsReadSendTimeHttp(SOCKET sock,
									int iNumber,
								    int iType,
									char strMsg[10][1024]
									);
int __stdcall MongateCsReadSendTimeHttpEx(SOCKET sock,int iNumber,int iType,char *strMsg);
//---------------------------------------------------------------------------
//                      ***更新定时号码列表***
//参数说明:
//	sock:[in] 连接句柄
//  iNumber:[in] 定时编号
//  iNum:[in] 为号码列表中号码的实际个数
//  pTelNos:[in] 号码列表: 1~1000个号码(符合号码规则),中间用英文逗号分割
//---------------------------------------------------------------------------
int __stdcall MongateCsUpdateTelListSendTime(SOCKET sock,
									int iNumber,
								    int iNum,
									const char* pTelNos);

int __stdcall MongateCsUpdateTelListSendTimeHttp(SOCKET sock,
									int iNumber,
								    int iNum,
									const char* pTelNos);

//---------------------------------------------------------------------------
//                      ***读取定时号码列表***
//参数说明:
//	sock:[in] 连接句柄
//  iNumber:[in] 定时编号
//  pStrMessage:[out] 读取服务器返回信息
//---------------------------------------------------------------------------
int __stdcall MongateCsReadTelListSendTime(SOCKET sock,
									      int iNumber,
										  char* pStrMessage);

int __stdcall MongateCsReadTelListSendTimeHttp(SOCKET sock,
									      int iNumber,
										  char* pStrMessage);

/*-----------------------------------20070326 fq-------------------------------------------*/
int __stdcall MongateTestMsg(SOCKET sock, const char* pStrMessage);
int __stdcall MongateTestMsgEx(SOCKET sock,const char* pStrMessage,char *keywords);
int __stdcall MongateTestMsgHttp(SOCKET sock,const char* pStrMessage);
int __stdcall MongateTestMsgExHttp(SOCKET sock,const char* pStrMessage,char *keyword);
int __stdcall MGhexChrTosrcStr(const char* srcMsg,char* retMsg);
int __stdcall MGQueryAccount(SOCKET sock ,const char * pReserve,char* pReStr);
/*******************************************************************************************/

int __stdcall MongateConnectHttp(const char* pszIP, int iPort,
   const char* pszAccount, const char* pszPwd);
int __stdcall MongateQueryUsedHttp(SOCKET sock);
int __stdcall MongateQueryBalanceHttp(SOCKET sock);
int __stdcall MongateCsSendSmsHttp(SOCKET sock, const char* pszMobis,
	const char* pszMsg, int iMobiCount, char* pszSN);
int __stdcall MongateKxtSendHttp(SOCKET sock, const char* pszMobis,
	const char* pszMsg, int iMobiCount, char* pszSN);
int __stdcall MongateCsGetSmsHttp(SOCKET sock, char strMsg[500][255]);
int __stdcall MongateCsGetSmsHttpEx(SOCKET sock, char *strMsg);
int __stdcall MongateChangePwdHttp(SOCKET sock, const char* pszAccount,
  const char* pszOldPwd, const char* pszNewPwd);
int __stdcall MongateCsGetStatusReportHttp(SOCKET sock, char strMsg[500][255]);
int __stdcall MongateCsGetStatusReportHttpEx(SOCKET sock, char *strMsg);
int __stdcall MongateRechargeHttp(SOCKET sock, const char* pszCardNo,
  const char* pszCardPwd);
//int __stdcall MongateTestConn(SOCKET sock);
int __stdcall MongateConnectSocks4Proxy(const char* pszProxyIP,int iProxyPort,
  const char* pszIP,int iPort,const char* pszAccount,const char* pszPwd);
int __stdcall MongateConnectSocks5Proxy( const char* pszProxyIP,
  int iProxyPort,const char* pszProxyAccount,const char* pszProxyPwd,
  const char* pszIP,int iPort,const char* pszAccount,const char* pszPwd);
int __stdcall MongateConnectHttpProxy(const char* pszProxyIP,
  int iProxyPort,const char* pszProxyAccount,const char* pszProxyPwd,
  const char* pszIP, int iPort, const char* pszAccount, const char* pszPwd);
int __stdcall MongateSetCmdType(int iCmdType); //设置登陆类型
int __stdcall MongateSetOverTime(int iOverTime); //设置网络超时时间

int __stdcall MongateCsReadInform(SOCKET sock,
								  int iType,
								  const char* sNumber,
								  const char* reserve1,
								  const char * reserve2,
								  const char * reserve3,
								  char *strMsg);
void TranMsgIdI64ToChar(unsigned char* MsgId, __int64 IMsgId);
int TranMsgId(unsigned char *pSrc, char *MsgId);
void TranSmitMsgID(unsigned char *pSrc, char *MsgId);
