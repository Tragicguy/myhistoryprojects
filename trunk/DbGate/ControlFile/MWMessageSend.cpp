#include "stdafx.h"
#include ".\mwmessagesend.h"

/************************************************************************/
/*                                                                      */
/************************************************************************/
CMWMessageSend::CMWMessageSend(void)
{
	m_sock=-1;//-1 未初始化 初始化 0 登陆 >0
	m_handle = NULL;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CMWMessageSend::~CMWMessageSend(void)
{
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//发送下行信息
int CMWMessageSend::SendMtMessage(LPCTSTR sMsg, LPCTSTR sDestAddrs,int byUserNum,LPCTSTR sSubPort,unsigned char* byMsgID,CString &strError)
{
	char pszSN[50]={0};
	int iret = -99;
	const char *pszMobis;
	const char *pszMsg;
	char pszSubPort[30]="*";
	WriteLogEx("发送下行信息函数开始",1);
	try
	{
		if( m_sock < 0 )
		{
			strError = "接口尚未初始化";
			return -999;	//未初始化
		}
		if (strlen(sSubPort) > 0)
		{
			strncpy(pszSubPort,sSubPort,30);
		}
		pszMobis = sDestAddrs;
		pszMsg = sMsg;
		WriteLogEx("动态库发送信息开始",1);
		try
		{
			iret = MongateCsSPSendSms(m_sock,pszMobis,pszMsg,byUserNum,pszSubPort,pszSN);	
			if(iret > 0)
			{
				strError = "发送成功";
				CString strNewID = "";
				strNewID.Format("%s",pszSN);
				if (strNewID == "")
				{
					strcpy((char*)byMsgID,pszSN);
				}
				else if (strNewID.Left(1) == "-")
				{
					unsigned char szOldNo[50] = {0};
					char szNewNo[50] = {0};
					__int64 iiii =_atoi64(pszSN);
					TranMsgIdI64ToChar(szOldNo,iiii);
					TranMsgId(szOldNo,szNewNo);
					//strNewID.Format("%s",szNewNo);
					strcpy((char*)byMsgID,szNewNo);
				}
				else
				{
					strcpy((char*)byMsgID,pszSN);
				}
				iret =  0;		//成功
			}
			else
			{
				strError = GetSendMsgErrorStr(iret);
			}
		}
		catch(...)
		{
			strError = "动态库发送信息异常";
			iret = -99;
		}		
		WriteLogEx("动态库发送信息结束"+strError,1);
	}
	catch(...)
	{
		WriteLog("SendMtMessage异常",3);
		strError = "发送信息异常错误";
		iret = -99;		//异常
	}
	WriteLogEx("发送下行信息函数结束,"+strError,1);
	return iret;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//释放接口
BOOL CMWMessageSend::ExitInterface()
{	
	try
	{
		if(m_sock < 0)
		{
			return TRUE;
		}
		MongateDisconnect(m_sock);
		if (m_handle)
		{
			FreeLibrary(m_handle);
			m_handle = NULL;
		}
		m_sock = -1;
		return TRUE;
	}	
	catch (...)
	{
		WriteLog("CMWMessageSend::ExitInterface异常",3);
	}
	return FALSE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//设置参数
void CMWMessageSend::SetParams(CString strServerIP,int iPort,CString strUserID,CString strPwd)
{
	memset(m_sServerIP,0,sizeof(m_sServerIP));
	m_iport = 8018;
	memset(m_sUserID,0,sizeof(m_sUserID));
	memset(m_sPassWord,0,sizeof(m_sPassWord));
	strncpy(m_sServerIP,strServerIP,20);
	m_iport = iPort;
	strncpy(m_sUserID,strUserID,33);
	strncpy(m_sPassWord,strPwd,33);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//初始化接口
BOOL CMWMessageSend::InitInterface()
{
#ifdef USE_DLL___
	WriteLogEx("初始化动态库函数开始",1);
	if(!m_handle)
	{
		TCHAR szBuff[_MAX_PATH];
		VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szBuff, _MAX_PATH));
		CString strAppPath="";
		strAppPath.Format("%s",szBuff);
		int pos=strAppPath.ReverseFind('\\');
		strAppPath=strAppPath.Left(pos+1);

		m_handle = LoadLibrary(strAppPath + MWGATE_DLL_NAME);
		if(!m_handle)
		{
			return FALSE;
		}
		MongateConnect = (FnMongateConnect)GetProcAddress(m_handle, "MongateConnect");
		MongateCsSPSendSms  = (FnMongateCsSPSendSms)GetProcAddress(m_handle, "MongateCsSPSendSms");	
		MongateCsGetSmsEx  = (FnMongateCsGetSmsEx)GetProcAddress(m_handle, "MongateCsGetSmsEx");
		MongateDisconnect  = (FnMongateDisconnect)GetProcAddress(m_handle, "MongateDisconnect");
		MongateQueryBalance = (FnMongateQueryBalance)GetProcAddress(m_handle,"MongateQueryBalance");
		MongateQueryUsed	= (FnMongateQueryUsed)GetProcAddress(m_handle,"MongateQueryUsed");
		MGhexChrTosrcStr = (FnMGhexChrTosrcStr)GetProcAddress(m_handle,"MGhexChrTosrcStr");
		MongateCsGetStatusReportEx = (FnMongateCsGetStatusReportEx)GetProcAddress(m_handle,"MongateCsGetStatusReportEx");
		MongateChangePwd = (FnMongateChangePwd)GetProcAddress(m_handle,"MongateChangePwd");
		MongateTestConn = (FnMongateTestConn)GetProcAddress(m_handle,"MongateTestConn");
		SetCmdType = (FnSetCmdType)GetProcAddress(m_handle,"SetServerType");
		
	}
	m_sock = 0;	//初始化接口后将SOCKET置为0以标识
	WriteLogEx("初始化动态库函数结束",1);
#endif
// 	m_handle_smeidll = LoadLibrary(strAppPath + MWGATE_DLL_NAME_SMEIDLL);
// 	if(m_handle_smeidll == 0x0)	
// 	{
// 		return FALSE;
// 	}
// 	IFInitInterface = (_IFInitInterface)GetProcAddress(m_handle_smeidll,"IFInitInterface");
// 	IFExitInterface = (_IFExitInterface)GetProcAddress(m_handle_smeidll,"IFExitInterface");
// 	Login_R = (_Login_R)GetProcAddress(m_handle_smeidll,"Login_R");
// 	SendSms = (_SendSms)GetProcAddress(m_handle_smeidll,"SendSms");
// 	RecvSms = (_RecvSms)GetProcAddress(m_handle_smeidll,"RecvSms");
// 	m_sock = 0;
	return TRUE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//连接并登录服务器
int CMWMessageSend::ConnectAndLogin(CString &strError)
{
	WriteLogEx("连接并登录服务器函数开始",1);
	int iret = -99;
	try
	{
		if(m_sock < 0)
		{
			strError = "接口尚未初始化";
			return -999;	//未初始化
		}
		if (CString(m_sServerIP) == "" || CString(m_sUserID) == "")
		{
			strError = "用户信息尚未设置";
			return -2;
		}
		iret = MongateConnect(m_sServerIP, m_iport,m_sUserID, m_sPassWord);
		m_sock = iret;
		if(iret < 1) //登陆失败
		{
			strError = GetLoginErrorStr(iret);
			m_sock = 0;
		}
		else
		{
			strError = "登录成功";
		}		
	}
	catch (...)
	{
		WriteLog("CMWMessageSend::ConnectAndLogin异常",3);
		strError = "登录异常";
		iret = -99;		
	}
	WriteLogEx("连接并登录服务器函数结束,"+strError,1);
	return iret;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//根据错误号返回错误信息
CString CMWMessageSend::GetServerErrorStr(int iErrorCode)
{
	CString strRet = "";
	switch ( iErrorCode )
	{
	case -10001:
		{
			strRet = "用户登陆不成功";
			break;
		}
	case -10002:
		{
			strRet = "提交格式不正确";
			break;
		}
	case -10003:
		{
			strRet = "用户余额不足";
			break;
		}
	case -10004:
		{
			strRet = "手机号码不正确";
			break;
		}
	case -10005:
		{
			strRet = "计费用户帐号错误";
			break;
		}
	case -10006:
		{
			strRet = "计费用户密码错";
			break;
		}
	case -10007:
		{
			strRet = "账号已经被停用";
			break;
		}
	case -10008:
		{
			strRet = "账号类型不支持该功能";
			break;
		}
	case -10009:
		{
			strRet = "其它错误";
			break;
		}
	case -10010:
		{
			strRet = "企业代码不正确";
			break;
		}
	case -10011:
		{
			strRet = "信息内容超长";
			break;
		}
	case -10012:
		{
			strRet = "不能发送联通号码";
			break;
		}
	case -10013:
		{
			strRet = "操作员权限不够";
			break;
		}
	case -10014:
		{
			strRet = "费率代码不正确";
			break;
		}
	case -10015:
		{
			strRet = "服务器繁忙";
			break;
		}
	case -10016:
		{
			strRet = "企业权限不够";
			break;
		}
	case -10017:
		{
			strRet = "此时间段不允许发送";
			break;
		}
	case -10018:
		{
			strRet = "经销商用户名或密码错";
			break;
		}
	case -10019:
		{
			strRet = "手机列表或规则错误";
			break;
		}
	case -10021:
		{
			strRet = "没有开停户权限";
			break;
		}
	case -10022:
		{
			strRet = "没有转换用户类型的权限";
			break;
		}
	case -10023:
		{
			strRet = "没有修改用户所属经销商的权限";
			break;
		}
	case -10024:
		{
			strRet = "经销商用户名或密码错";
			break;
		}
	case -10025:
		{
			strRet = "操作员登陆名或密码错误";
			break;
		}
	case -10026:
		{
			strRet = "操作员所充值的用户不存在";
			break;
		}
	case -10027:
		{
			strRet = "操作员没有充值商务版的权限";
			break;
		}
	case -10028:
		{
			strRet = "该用户没有转正不能充值";
			break;
		}
	case -10029:
		{
			strRet = "此用户没有权限从此通道发送信息";
			break;
		}
	case -10030:
		{
			strRet = "不能发送移动号码";
			break;
		}
	case -10031:
		{
			strRet = "手机号码(段)非法";
			break;
		}
	case -10032:
		{
			strRet = "用户使用的费率代码错误";
			break;
		}
	case -10033:
		{
			strRet = "非法关键词";
			break;
		}
	case -10035:
		{
			strRet = "不能发送多个手机";
			break;
		}
	case -10036:
		{
			strRet = "不能发送单个手机";
			break;
		}
	case -10037:
		{
			strRet = "单发相同的信息超过流量暂停";
			break;
		}
	case -10038:
		{
			strRet = "经销商权限不够";
			break;
		}
	case -10040:
		{
			strRet = "重复注册";
			break;
		}
	case -10041:
		{
			strRet = "读取上行MO信息太快";
			break;
		}
	case -10042:
		{
			strRet = "读取状态报告太快";
			break;
		}
	case -10043:
		{
			strRet = "超过单个帐号的最大登陆数";
			break;
		}
	case -10044:
		{
			strRet = "该帐号没有扩展子端口权限";
			break;
		}
	case -10045:
		{
			strRet = "信息包编号小于旧编号错误";
			break;
		}
	case -10046:
		{
			strRet = "信息包编号大于旧编号错误";
			break;
		}
	case -10047:
		{
			strRet = "修改密码错误";
			break;
		}
	case -10048:
		{
			strRet = "卡号或密码错误";
			break;
		}
	case -10051:
		{
			strRet = "只能发送移动号码";
			break;
		}
	case -10052:
		{
			strRet = "只能发送联通号码";
			break;
		}
	case -10053:
		{
			strRet = "只能发送电信小灵通号码";
			break;
		}
	case -10054:
		{
			strRet = "定时编号不存在";
			break;
		}
	case -10099:
		{
			strRet = "其它错误";
			break;
		}
	default:
		{
			strRet = "";
			break;
		}
	}
	return strRet;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//返回登录错误
CString CMWMessageSend::GetLoginErrorStr( int iErrorCode )
{
	CString strError = "";
	switch ( iErrorCode )
	{
	case -1:
		{
			strError = "参数中包含有NULL值";
			break;
		}
	case -2:
		{
			strError = "非法的登陆帐号或密码";
			break;
		}
	case -199:
		{
			strError = "获取远程主机信息失败";
			break;
		}			
	case -198:
		{
			strError = "创建socket失败";
			break;
		}
	case -197:
		{
			strError = "连接失败";
			break;
		}
	case -196:
		{
			strError = "等待平台返回连接确认信息超时";
			break;
		}
	case -195:
		{
			strError = "等待服务器握手失败";
			break;
		}
	default:
		{
			strError = "其他连接登录错误";
			break;
		}
	}
	if ( iErrorCode < -10000 )	//服务器返回错误
	{
		strError = GetServerErrorStr( iErrorCode );
	}
	return strError;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//返回发送信息错误信息
CString CMWMessageSend::GetSendMsgErrorStr( int iErrorCode )
{
	CString strError = "";
	switch ( iErrorCode )
	{
	case -1:
		{
			strError = "参数中包含有NULL值";
			break;
		}
	case -2:
		{
			strError = "电话号码个数超过1000";
			break;
		}
	case -10:
		{
			strError = "申请缓存空间失败";
			break;
		}			
	case -11:
		{
			strError = "电话号码中有非数字字符";
			break;
		}
	case -12:
		{
			strError = "有异常电话号码";
			break;
		}
	case -13:
		{
			strError = "电话号码个数与实际个数不相等";
			break;
		}
	case -14:
		{
			strError = "实际号码个数超过1000";
			break;
		}
	case -101:
		{
			strError = "发送消息等待超时";
			break;
		}
	case -102:
		{
			strError = "发送或接收消息失败";
			break;
		}
	case -103:
		{
			strError = "接收消息超时";
			break;
		}
	case -104:
		{
			strError = "接收消息失败";
			break;
		}
	case -105:
		{
			strError = "发送信息时接收消息失败[-105],缓冲区溢出";
		}
	case -198:
		{
			strError = "连接网络失败";
			break;
		}
	case -200:
		{
			strError = "其他错误";
			break;
		}
	default:
		{
			strError = "";
			break;
		}
	}
	if ( iErrorCode < -10000 )	//服务器返回错误
	{
		strError = GetServerErrorStr( iErrorCode);
	}
	return strError;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//返回读取上行或状态报告错误信息
CString CMWMessageSend::GetMoReportErrorMsg(int iErrorCode)
{
	CString strError = "";
	switch ( iErrorCode )
	{
	case -1:
		{
			strError = "参数中包含有NULL值";
			break;
		}
	case -101:
		{
			strError = "发送消息等待超时";
			break;
		}
	case -102:
		{
			strError = "发送消息失败";
			break;
		}
	case -103:
		{
			strError = "接收消息超时";
			break;
		}
	case -104:
		{
			strError = "接收消息失败";
			break;
		}
	case -105:
		{
			strError = "接收消息失败[-105],缓冲区溢出";			
			break;
		}
	case -198:
		{
			strError = "连接网络失败";
			break;
		}
	case -200:
		{
			strError = "其他错误";
			break;
		}
	case -300:
		{
			strError = "读取时间间隔不够";
			break;
		}
	default:
		{
			strError = "";
			break;
		}
	}
	if ( iErrorCode < -10000 )	//服务器返回错误
	{
		strError = GetServerErrorStr( iErrorCode);
	}
	return strError;
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
//接收上行信息
int CMWMessageSend::GetMoMessges(DeliverPackList &recvPackList,CString &strError )
{
	WriteLogEx("读取上行信息函数开始",1);
	int iRet = -99;
	CString strTemp = "";
	try
	{
		if( m_sock < 0 )
		{
			strError = "接口尚未初始化";
			return -999;	//未初始化
		}
		char szMsg[30000]={0};	//
		char tokens[]=";";	//条与条之间的分隔符
		char *szDeliverMsg = NULL; 		
		//读状态报告
		WriteLogEx("动态库接收上行信息函数开始",1);
		iRet = MongateCsGetSmsEx(m_sock,szMsg);
		strTemp.Format("动态库接收上行信息函数结束,返回值:%d",iRet);
		WriteLogEx(strTemp,1);
		if ( iRet == 0 )
		{
			strError = "无上行信息";
			//return 0; //表示无上行信息
		}
		else if (iRet > 0)	//有上行信息,返回的是上行信息条数
		{
			//进行处理
			szDeliverMsg=strtok(szMsg,tokens);
			while (szDeliverMsg!=NULL)
			{
				//AddRePortPack(reportPackList,szReport);				
				strTemp.Format("%s",szDeliverMsg);
				AddDeleverPack(recvPackList,strTemp);
				szDeliverMsg = strtok(NULL,tokens);
			}
			strError.Format("读取到%d条上行信息",iRet);
		}
		else if ( iRet < 0 )	//接收出错,返回错误信息
		{
			strError = "接收上行时出错:"+GetMoReportErrorMsg(iRet);
			WriteLogEx(strError,1);
		}		
	}
	catch (...)
	{
		WriteLog("CMWMessageSend::GetMoMessges异常",3);
		strError = "读取上行信息异常";
		iRet = -99;		
	}
	WriteLogEx("读取上行信息函数结束",1);
	return iRet;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//接收状态报告
int CMWMessageSend::GetMsgReports(ReportPackList &reportPackList,CString &strError )
{
	WriteLogEx("读取状态报告函数开始",1);
	int iRet = -99;
	CString strTemp = "";
	try
	{
		if( m_sock < 0 )
		{
			strError = "接口尚未初始化";
			return -999;	//未初始化
		}
		char szMsg[30000]={0};	//
		char tokens[]=";";	//条与条之间的分隔符
		char *szReport=NULL; 
		
		//读状态报告
		WriteLogEx("动态库接收状态报告函数开始",1);
		iRet = MongateCsGetStatusReportEx(m_sock,szMsg);
		strTemp.Format("动态库接收状态报告函数结束,返回值:%d",iRet);
		WriteLogEx(strTemp,1);
		if (iRet == 0)
		{
			strError = "无状态报告";
			//return 0; //表示无状态报告
		}
		else if (iRet > 0)	//有状态报告,返回的是状态报告条数
		{
			//进行处理
			
			szReport=strtok(szMsg,tokens);
			while (szReport!=NULL)
			{
				strTemp.Format("%s",szReport);
				AddRePortPack(reportPackList,strTemp);
				szReport = strtok(NULL,tokens);
			}
			strError.Format("读取到%d条状态报告",iRet);
		}
		else if (iRet < 0)	//接收出错,返回错误信息
		{
			strError = "接收状态报告时出错:"+GetMoReportErrorMsg(iRet);
			WriteLogEx(strError,1);
		}		
	}
	catch (...)
	{
		WriteLog("CMWMessageSend::GetMsgReports异常",3);
		strError = "读取状态报告异常";
		iRet = -99;		
	}
	WriteLogEx("读取状态报告函数结束",1);
	return iRet;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CMWMessageSend::AddRePortPack(ReportPackList &reportPackList, CString strMsg)
{
	/* 2009-05-20,17:06:05,05201706054587,*,0,DILEVRD */
	CString strMessage = strMsg+",";
	CString szstrMsg = "";
	int iCount=0;
	ReportPack tempPack;
	try
	{
		//szstrMsg=strtok(strTempMsg,tokens);
		int index = 0;
		index = strMessage.Find(',');
		while (index != -1)
		{
			szstrMsg = strMessage.Left(index);
			strMessage = strMessage.Mid(index + 1);
            iCount++;
			switch ( iCount )
			{
			case 1:		//日期
				{
					tempPack.strDate = szstrMsg;
					break;
				}
			case 2:		//时间
				{
					tempPack.strTime = szstrMsg;
					break;
				}
			case 3:		//编号
				{
					if(szstrMsg.GetLength() == 0)
					{
						tempPack.strMsgNo = "";
						break;
					}
					if (szstrMsg.Left(1) == "-")
					{
						unsigned char szOldNo[50] = {0};
						char szNewNo[50] = {0};
						__int64 iiii =_atoi64(szstrMsg);
						TranMsgIdI64ToChar(szOldNo,iiii);
						TranMsgId(szOldNo,szNewNo);
						tempPack.strMsgNo.Format("%s",szNewNo);
					}
					else
					{
						//strcpy((char*)byMsgID,pszSN);
						tempPack.strMsgNo = szstrMsg;
					}
					break;
				}
			case 4:		//备用
				{
					tempPack.strReserve = szstrMsg;
					break;
				}
			case 5:		//状态
				{
					tempPack.strMsgStatus = szstrMsg;
					break;
				}
			case 6:		//错误码
				{
					tempPack.strErrorCode = szstrMsg;
					break;
				}
			default:
				{
					break;
				}
			}
			index = strMessage.Find(',');
		}
		reportPackList.Add(tempPack);
		return 0;
	}
	catch (...)
	{
		WriteLog("CMWMessageSend::AddRePortPack异常",3);
		return -1;
	}	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CMWMessageSend::AddDeleverPack(DeliverPackList &recvPackList,CString strMsg)
{
	/* 2009-05-20,17:06:05,05201706054587,*,0,DILEVRD */
	CString strMessage = strMsg+",";
	CString szstrMsg = "";
	int iCount=0;
	DeliverPack tempPack;
	char szSrc[1000] = {0};
	char szDst[1000] = {0};
	try
	{
		//szstrMsg=strtok(strTempMsg,tokens);
		int index = 0;
		index = strMessage.Find(',');
		while (index != -1)
		{
			szstrMsg = strMessage.Left(index);
			strMessage = strMessage.Mid(index + 1);
			iCount++;
			switch (iCount)
			{
			case 1:		//日期
				{
					tempPack.strDate = szstrMsg;
					break;
				}
			case 2:		//时间
				{
					tempPack.strTime = szstrMsg;
					break;
				}
			case 3:		//发送手机号码
				{
					tempPack.strOrgAddr = szstrMsg;
					break;
				}
			case 4:		//接收号码
				{
					tempPack.strDestAddr = szstrMsg;
					break;
				}
			case 5:		//日期
				{
					tempPack.strReserve = szstrMsg;
					break;
				}
			case 6:
				{
					memset(szSrc,0,1000);
					memset(szDst,0,1000);
					strncpy(szSrc,szstrMsg,1000);
					MGhexChrTosrcStr(szSrc,szDst);
					szstrMsg.Format("%s",szDst);
					tempPack.strMsg.Format("%s",szDst);
					break;
				}
			default:
				{
					break;
				}
			}
			index = strMessage.Find(',');
		}
		recvPackList.Add(tempPack);
		return 0;
	}
	catch (...)
	{
		WriteLog("CMWMessageSend::AddDeleverPack异常",3);
		return -1;
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//修改密码
int CMWMessageSend::ChangePassword(CString strNewPwd,CString &strError)
{
	if (strNewPwd.GetLength() < 1)
	{
		strError = "新密码不能为空";
		return -203;
	}
	if( m_sock < 0 )
	{
		strError = "接口尚未初始化";
		return -999;	//未初始化
	}
	int iRet = -99;
	char szNewPassWord[32+1] = {0};	//用户密码
	try
	{
		strncpy(szNewPassWord,strNewPwd,33);
		//iRet = MongateChangePwd(m_sock,m_sUserID,m_sPassWord,szNewPassWord);
		if ( iRet > 0)	//修改成功
		{
			strError.Format("修改密码成功%s",strNewPwd);
		}
		else 
		{
			int iErrorCode = iRet;
			switch ( iErrorCode )
			{
			case -1:
				{
					strError = "参数中包含有NULL值";
					break;
				}
			case -101:
				{
					strError = "发送消息等待超时";
					break;
				}
			case -102:
				{
					strError = "发送消息失败";
					break;
				}
			case -103:
				{
					strError = "接收消息超时";
					break;
				}
			case -104:
				{
					strError = "接收消息失败";
					break;
				}
			case -105:
				{
					strError = "修改密码时接收消息失败[-105],缓冲区溢出";
					break;
				}
			case -198:
				{
					strError = "连接网络失败";
					break;
				}
			case -200:
				{
					strError = "其他错误";
					break;
				}
			default:
				{
					strError = "";
					break;
				}
			}
			if ( iErrorCode < -10000 )	//服务器返回错误
			{
				strError = GetServerErrorStr( iErrorCode );
			}
		}
		return iRet;
	}
	catch (...)
	{
		WriteLog("CMWMessageSend::ChangePassword异常",3);
		strError = "修改密码异常";
		return -99;
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//查询已用与剩余条数
int CMWMessageSend::GetFee(int &iUsed,int &iLeft,CString &strError)
{
	WriteLogEx("查询余额函数开始",1);
	int iRet1 = -99;
	int iRet2 = -99;
	CString strShow = "";
	if( m_sock < 0 )
	{
		strError = "接口尚未初始化";
		WriteLogEx("查询余额函数结束,"+strError,1);
		return -999;	//未初始化
	}
	try
	{
		WriteLogEx("动态库查询余额函数开始",1);
		iRet1 = MongateQueryBalance(m_sock);		
		strShow.Format("%d",iRet1);
		WriteLogEx("动态库查询余额函数结束,返回值:"+strShow,1);
		if (iRet1 >= 0)		//只有当余额查询成功,才去查询已经使用的条数
		{
			iRet2 = MongateQueryUsed(m_sock);
		}
		if (iRet1 >= 0)	//查询成功
		{
			iLeft = iRet1;
			iUsed = iRet2;
			strError.Format("已用%d条,剩余%d条",iUsed,iLeft);
		}
		else	//查询失败
		{
			strError ="查询余额出错:";
			if (iRet1 < -10000)
			{
				strError +=GetServerErrorStr(iRet1);				
			}
			else
			{
				switch (iRet1)
				{
				case -1:
					{
						strError += "参数中包含有NULL值";
						break;
					}
				case -101:
					{
						strError += "发送消息等待超时";
						break;
					}
				case -102:
					{
						strError += "发送消息失败";
						break;
					}
				case -103:
					{
						strError += "接收消息超时";
						break;
					}
				case -104:
					{
						strError += "接收消息失败";
						break;
					}
				case -105:
					{
						strError = "查余额函数接收消息失败[-105],缓冲区溢出";
						break;
					}
				case -198:
					{
						strError += "连接网络失败";
						break;
					}
				case -200:
					{
						strError += "其他错误";
						break;
					}
				default:
					{
						strError += "其他错误";
						break;
					}
				}				
			}			
		}			
	}
	catch (...)
	{
		WriteLog("CMWMessageSend::GetFee异常",3);
		strError = "查询已用与剩余条数异常";
		iRet1 =  -99;
	}
	WriteLogEx("查询余额函数结束,"+strError,1);
	return iRet1;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CMWMessageSend::KeepOnline(CString &strError)
{
	WriteLogEx("维持连接函数开始",1);
	int iRet = -99;
	if( m_sock < 0 )
	{
		strError = "接口尚未初始化";
		WriteLogEx("维持连接函数结束,"+strError,1);
		return -999;	//未初始化
	}
	try
	{
		iRet = MongateTestConn(m_sock);
		if (iRet > 0)
		{
			strError = "连接正常";
		}
		else
		{
			switch (iRet)
			{
			case -1:
				{
					strError = "参数中包含有NULL值";
					break;
				}
			case -101:
				{
					strError = "发送消息等待超时";
					break;
				}
			case -102:
				{
					strError = "发送消息失败";
					break;
				}
			case -103:
				{
					strError = "接收消息超时";
					break;
				}
			case -104:
				{
					strError = "接收消息失败";
					break;
				}
			case -105:
				{
					strError = "维持连接函数接收消息失败[-105],缓冲区溢出";
					break;
				}
			case -198:
				{
					strError = "连接网络失败";
					break;
				}
			case -200:
				{
					strError = "测试连接出错";
					break;
				}
			case -300:
				{
					strError = "读取时间间隔不够";
					break;
				}
			default:
				{
					strError = GetServerErrorStr(iRet);
					break;
				}
			}	
		}
	}
	catch (...)
	{
		WriteLog("CMWMessageSend::KeepOnline异常",3);
		strError = "测试连接异常";
		iRet = -99;
	}
	WriteLogEx("维持连接函数结束,"+strError,1);
	return iRet;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMWMessageSend::Disconnect()
{
	if (g_iServerType==0)
	{
		if (m_sock<=0)
		{
			return;
		}
	}	
	WriteLogEx("断开连接函数开始",1);
	try
	{
		MongateDisconnect(m_sock);
		m_sock = 0;		
	}
	catch (...)
	{
		WriteLog("CMWMessageSend::Disconnect异常",3);
	}
	WriteLogEx("断开连接函数结束",1);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMWMessageSend::SetCmdTypeEx(int iCmdType,int iRptFlag)
{
#ifndef USE_DLL___
	SetCmdType(iCmdType);
#else
	SetCmdType(iCmdType,iRptFlag);
#endif
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
int CMWMessageSend::TranMsgId(unsigned char *pSrc, char *MsgId)
{
	unsigned int nMon	= 0;
	unsigned int nDay	= 0;
	unsigned int nHour	= 0;
	unsigned int nMin	= 0;
	unsigned int nSec	= 0;
	unsigned int nGate	= 0;
	unsigned int nNo	= 0;
	unsigned char *p	= NULL;

	//先转为64bit整形再移位
	__int64 j = 0;
	p = pSrc;
	j |= (__int64)(*(p))	<< 56;
	j |= (__int64)(*(++p))	<< 48;
	j |= (__int64)(*(++p))	<< 40;
	j |= (__int64)(*(++p))	<< 32;
	j |= (__int64)(*(++p))	<< 24;
	j |= (__int64)(*(++p))	<< 16;
	j |= (__int64)(*(++p))	<< 8;
	j |= (__int64)(*(++p));

	nMon  = (unsigned int)(j >> 60) & 0x0f;            //4  bit
	nDay  = (unsigned int)(j >> 55) & 0x1f;           //5  bit
	nHour = (unsigned int)(j >> 50) & 0x1f;           //5  bit
	nMin  = (unsigned int)(j >> 44) & 0x3f;           //6  bit
	nSec  = (unsigned int)(j >> 38) & 0x3f;           //6  bit
	nGate = (unsigned int)(j >> 20) & 0x03ffff;       //18 bit,最大值9999
	nNo   = (unsigned int)(j)		& 0x0fffff;       //20 bit,最大值999999
	sprintf(MsgId, "%02d%02d%02d%02d%02d%06d%06d", nMon, nDay, nHour, nMin, nSec, nGate, nNo);
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMWMessageSend::TranMsgIdI64ToChar(unsigned char* MsgId, __int64 IMsgId)
{
	/*
	__int64 j = IMsgId;
	MsgId[0] = (unsigned char)((j >> 56) & 0xff);
	MsgId[1] = (unsigned char)((j >> 48) & 0xff);
	MsgId[2] = (unsigned char)((j >> 40) & 0xff);
	MsgId[3] = (unsigned char)((j >> 32) & 0xff);
	MsgId[4] = (unsigned char)((j >> 24) & 0xff);
	MsgId[5] = (unsigned char)((j >> 16) & 0xff);
	MsgId[6] = (unsigned char)((j >> 8 ) & 0xff);
	MsgId[7] = (unsigned char)((j) & 0xff);
	return;
	*/
	__int64 j = IMsgId;
	MsgId[0] = (unsigned char)((j >> 56) & 0xff);
	MsgId[1] = (unsigned char)((j >> 48) & 0xff);
	MsgId[2] = (unsigned char)((j >> 40) & 0xff);
	MsgId[3] = (unsigned char)((j >> 32) & 0xff);
	MsgId[4] = (unsigned char)((j >> 24) & 0xff);
	MsgId[5] = (unsigned char)((j >> 16) & 0xff);
	MsgId[6] = (unsigned char)((j >>  8) & 0xff);
	MsgId[7] = (unsigned char)(j & 0xff);
	return;
}