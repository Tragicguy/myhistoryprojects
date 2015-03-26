#include "StdAfx.h"
#include ".\mwmessagesendex.h"
#include "UserFile.h"
#include "../DialogFile/DbGateClose.h"
#include <io.h>
#include <direct.h>

CCriticalSection g_DisListLock;
int g_iIsNeedToLog = 0;
int g_iDebugLog = 0;
int g_iServerType = 0;	//接入平台选择，0为客馨通平台，1为SI平台
int g_iReportFlag = 0;
CASCIIConver g_Conver;
int g_iSendBufLen = 0;			//发送缓冲区大小
CString g_strServiceName = "";
/************************************************************************/
/*                                                                      */
/************************************************************************/
CMWMessageSendEx::CMWMessageSendEx(void)
{
	m_iIsSendThreadRun = 0;
	m_iIsRecvThreadRun = 0;
	m_iPhoneSplitLen = 1000;
	m_iIsRecvReport = 1;
	m_iIsRecvMo = 1;
	m_iDiffMobile = 0;
	SetRunStatus(STATUS_STOP_NORMAL);		//置运行状态为停止
	if(InitInterface()== FALSE)
	{
        AfxMessageBox("初始化接口失败,系统找不到 EMWGateway.dll");
		exit(0);
	}
	m_iTestConInterval = 0;
	m_iActiveThreadNum = 0;
	int i = 0;
	for (i=0;i<5;i++)
	{
		m_AddResultThreadContrl[i].ThreadExit = TRUE;
	}
	//初始化连续相同单发处理数据
	m_strLastSingleMsg = "";
	m_iSameSingleSendTime = 0;
	m_iMostSameSingleSendTime = 15;
	InitParams();
	//初始化账户信息
	SetParams(m_strServerIP,m_iServerPort,m_strAccount,m_strAccPwd);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CMWMessageSendEx::~CMWMessageSendEx(void)
{
	ExitInterface();
	CUserFile *pFile = NULL;
	pFile = CUserFile::GetInstance();
	if (pFile)
	{
		delete pFile;
		pFile = NULL;
	}
// 	CConsolePrinter *pConsole = CConsolePrinter::Instance();
// 	if (pConsole)
// 	{
// 		delete pConsole;
// 		pConsole = NULL;
// 	}
	Sleep(20);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
bool CMWMessageSendEx::IsNeedToRecvMo(unsigned int iDelay)
{
	DWORD t2 = 0;
	t2 = ::GetTickCount();
	DWORD tSpan = 0;
	m_LastRecvMoTimeLock.Lock();
	tSpan = t2 - m_LastRecvMoTime;
	m_LastRecvMoTimeLock.Unlock();
	if (tSpan > iDelay * 1000) //iDelay 秒
	{
		return true;
	}
	else
	{
		return false;
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMWMessageSendEx::SetLastRecvMoTime()
{
	m_LastRecvMoTimeLock.Lock();
	m_LastRecvMoTime = ::GetTickCount();
	m_LastRecvMoTimeLock.Unlock();
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
bool CMWMessageSendEx::IsNeedToRecvReport(unsigned int iDelay)
{
	DWORD t2 = 0;
	t2 = ::GetTickCount();
	DWORD tSpan = 0;
	m_LastRecvReportTimeLock.Lock();
	tSpan = t2 - m_LastRecvReportTime;
	m_LastRecvReportTimeLock.Unlock();
	if (tSpan > iDelay * 1000) //iDelay 秒
	{
		return true;
	}
	else
	{
		return false;
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMWMessageSendEx::SetLastRecvReportTime()
{
	m_LastRecvReportTimeLock.Lock();
	m_LastRecvReportTime = ::GetTickCount();
	m_LastRecvReportTimeLock.Unlock();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMWMessageSendEx::SetLastActiveTime( )
{
	m_LastActiveTimeLock.Lock();
	m_LastActiveTime = ::GetTickCount();
	m_LastActiveTimeLock.Unlock();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//Delay为间隔秒数,判断是否需要激活连接
bool CMWMessageSendEx::IsNeedReConn(unsigned int iDelay)
{
	try
	{
		DWORD t2 = 0;
		t2 = ::GetTickCount();
		DWORD tSpan = 0;

		m_LastActiveTimeLock.Lock();
		tSpan = t2 - m_LastActiveTime;
		m_LastActiveTimeLock.Unlock();

		if (tSpan > iDelay * 1000 ) //1分钟
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	catch (...)
	{
	}
	return true;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CMWMessageSendEx::MessageOperate(int iOptype,int &iErrorType,CString &strError,KfSubmitPack &mtPack,DeliverPackList &recvList,ReportPackList &reportList)
{
	int iRet = -99;
	try
	{
		iRet = -99;
		switch (iOptype)
		{
		case OPTYPE_LOGIN:
			{
				iErrorType = MSG_ERROR_KEEPONLINE;
				if (IsNeedReConn(m_iTestConInterval))		//1分钟激活连接一次
				{
					iRet = KeepOnline(strError);
					if (iRet < 0)
					{
						Disconnect();
						m_LastActiveTime = ::GetTickCount() - 10000000;
						iErrorType = MSG_ERROR_LOGIN;
						iRet = ConnectAndLogin(strError);
					}
					else
					{
						strError = "激活连接成功";
						//m_CheckFeeFlag = true;
						SetLastActiveTime();
					}
				}
				else
				{
					strError = "无需激活连接";
					iRet = 1;
				}
				break;
			}
		case OPTYPE_SEND:
			{
				iErrorType = MSG_ERROR_SEND;
				unsigned char szMsgNO[50]= {0};		
				CString strDestAddr = mtPack.m_strUserNumber;
				strDestAddr.Trim();
				strDestAddr.Replace(' ',',');
				if ( m_iUseSubPort == 0 )	//不使用子端口
				{
					mtPack.m_strSubPort = "*";
				}
				iRet = SendMtMessage(mtPack.m_strMsg,strDestAddr,mtPack.m_iCount,mtPack.m_strSubPort,szMsgNO,strError);
				if (iRet == 0)	//发送成功
				{
					mtPack.m_strSpMsgId.Format("%s",szMsgNO);		
				}
				break;
			}
		case OPTYPE_RECVMO:
			{
				iErrorType = MSG_ERROR_RECVMO;
				if ( IsNeedToRecvMo( m_iRecvMoInterval ))
				{
					iRet = GetMoMessges(recvList,strError);
					if (iRet == -300)
					{
						iRet = 10000;
					}
					else
					{
						SetLastRecvMoTime();
					}	
				}
				else
				{
					strError = "接收上行信息未超过接收间隔 -- 本地验证";
					iRet = 10000;
				}				
				break;
			}
		case OPTYPE_RECVREPORT:
			{
				iErrorType = MSG_ERROR_RECVREPORT;
				if ( IsNeedToRecvReport( m_iRecvReportInterval ))
				{
					iRet = GetMsgReports(reportList,strError);
					if (iRet == -300)
					{
						iRet = 10000;
					}
					else
					{
						SetLastRecvReportTime();
					}					
				}
				else
				{
					strError = "接收状态报告未超过接收间隔 -- 本地验证";
					iRet = 10000;
				}				
				break;
			}
		case OPTYPE_CHECKFEE:
			{
				iErrorType = MSG_ERROR_RECVREPORT;
				int iUsed,iLeft;
				iRet = GetFee(iUsed,iLeft,strError);
				break;
			}
		}	
		return iRet;
	}
	catch (...)
	{
		WriteLog("MessageOperate异常",3);
		iErrorType = 10000;	//未知异常
		return -99;
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//回存线程函数
void CMWMessageSendEx::InsertBackThreadProc(LPVOID lpParam)
{
	CMWMessageSendEx *pThis = (CMWMessageSendEx *)lpParam;
	::CoInitialize(NULL);
	pThis->InsertBackProc(); //执行线程函数体
	::CoUninitialize();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//回存线程执行函数
void CMWMessageSendEx::InsertBackProc()
{
	AddToDispListShow("程序将在写历史完成后退出",1);	
	Sleep(300);
	int iRet = 0;
	KfSubmitPack mt;
	int iRunFlag = 1;	
	try
	{
		while (true)
		{
			m_IsSendThreadRunLock.Lock();
			iRunFlag = m_iIsSendThreadRun;
			m_IsSendThreadRunLock.Unlock();
			if (iRunFlag==0)
			{
				break;
			}
			else
			{
				Sleep(500);
			}
		}
		while(true)
		{
			iRet = GetRealSendPack(mt);
			if (iRet == 0)		//将未发送的记录存入文件
			{
				AddToDispListShow("将待发缓冲列表信息写入文件");
				WriteBufToFile(mt);
			}
			else
			{
				break;
			}
		}
	}
	catch (...)
	{		
	}
	AddToDispListShow("写文件线程退出");
	OnActiveThreadExit(2);
	return;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//写历史线程函数
void CMWMessageSendEx::AddSendResultThread(LPVOID lpParam)
{
	ThreadParam *pParam = (ThreadParam*)lpParam;
	CMWMessageSendEx *pThis = (CMWMessageSendEx *)pParam->pThis;
	int iThreadNo = pParam->iThreadNO;
	::CoInitialize(NULL);
	pThis->AddSendResultProc(iThreadNo); //执行线程函数体
	::CoUninitialize();
	//AfxMessageBox("aa");
	delete pParam;
	pParam = NULL;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//写历史表线程执行函数
void CMWMessageSendEx::AddSendResultProc(int iThreadNO)
{
	KfSubmitPack mt;
	CDataBaseCtrl dataBaseCtrl;	//数据库操作对象
 	dataBaseCtrl.InitParams(m_dbSQLServer,m_dbSQLName,m_dbSQLUser,m_dbSQLPwd,m_iDbType,m_iDbPort);
	CString strTemp="";
	int iRet = 0;
	bool bExitFlag = false;
	try
	{	
		Sleep(100*(iThreadNO+1));
		strTemp.Format("写历史表线程%d启动",iThreadNO);
		AddToDispListShow(strTemp);
		while(true)
		{	
			if (m_iAddHistInterval >= 5)
			{
				Delay(m_iAddHistInterval,&m_AddResultThreadContrl[iThreadNO].ThreadExit);
			}			
			if (m_AddResultThreadContrl[iThreadNO].ThreadExit && iRet < 0)
			{
				if(bExitFlag==true)	//此线程退出
				{
					break;
				}
				int iRunFlag = 1;
				m_IsSendThreadRunLock.Lock();
				iRunFlag = m_iIsSendThreadRun;
				m_IsSendThreadRunLock.Unlock();
				if (iRunFlag == 0)	//如果发送线程已经退出
				{
					bExitFlag=true;
				}
				else
				{
					AddToDispListShow("等待发送线程退出...",1);
					Sleep(500);
				}
			}
			m_SendHistoryLock.Lock();
			try
			{				
				int iSize = m_SendHistoryList.GetSize();
				SetStatusInfoEx(4,iSize);//待存
				if (iSize <= 0)	//缓冲区无数据
				{
					iRet = -1;
				}
				else
				{
					iRet = 0;
					mt = m_SendHistoryList.GetAt(0);
					//WriteLogEx("保存发送历史开始",50 + iThreadNO);
					if (AddSendResult(dataBaseCtrl,mt,mt.m_iSendResultFlag)<0)//保存发送结果失败
					{
						if (mt.m_iTryTimes > 0)
						{
							strTemp.Format("第%d次保存发送结果失败[%d]",4-mt.m_iTryTimes,mt.m_iLocalID);
							AddToDispListShow(strTemp);
							m_SendHistoryList.GetAt(0).m_iTryTimes--;
						}
						else
						{
							AddToDispListShow("保存发送结果失败");
							m_SendHistoryList.RemoveAt(0);
						}
					}
					else
					{
						m_SendHistoryList.RemoveAt(0);
					}
					//WriteLogEx("保存发送历史结束",50 + iThreadNO);
				}		
			}
			catch(...)
			{
				iRet = -1;
				WriteLog("GetSendHistory异常",3);
			}
			m_SendHistoryLock.Unlock();
			if (iRet == -1)
			{
				Delay(m_iAddHistNullWait,&m_AddResultThreadContrl[iThreadNO].ThreadExit);
			}
		}
	}
	catch (...)
	{
		WriteLog("AddSendResultProc异常",3);
	}
	strTemp.Format("写历史表线程%d退出",iThreadNO);
	AddToDispListShow(strTemp);
	OnActiveThreadExit(1);	
	return;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMWMessageSendEx::MessageSendThreadProc(LPVOID lpParam)
{
	CMWMessageSendEx *pThis = (CMWMessageSendEx *)lpParam;
	::CoInitialize(NULL);
	pThis->MessageSendProc(); //执行线程函数体
	::CoUninitialize();
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//发送下行信息与接收上行状态报告的线程执行函数
void CMWMessageSendEx::MessageSendProc()
{
	CString strTemp = "";
	CString strSend = "";			
	CDataBaseCtrl dataBaseCtrl;	//数据库操作对象	
	DeliverPackList recvList;	//上行信息列表
	ReportPackList reportList;	//状态报告列表
	int iNextRet = -99;		//下一步操作返回值
	int iiRet = -99;		//发送与接收返回值
	int iErrorType = -99;	//记录返回的错误类型
	CString strError = "";	//记录返回的错误信息
	CStringList strIllegalList;	//非法号码列表
	int iPos = 0;				//账户余额处查询位置
	CString strBalance = "";	//账户余额
	int iRet = 0;	//获取待发结构体的返回值
	int i=0;		//循环控制变量
	int iHistLen = 0;//历史表记录数
	KfSubmitPack mt;			//下发结构体
	ReportPack report;
	DeliverPack mo;
	//Delay(100,&m_SendThreadContrl.ThreadExit);
	try
	{	
		m_IsSendThreadRunLock.Lock();
		m_iIsSendThreadRun = 1;
		m_IsSendThreadRunLock.Unlock();
		AddToDispListShow("发送线程启动");
		dataBaseCtrl.InitParams(m_dbSQLServer,m_dbSQLName,m_dbSQLUser,m_dbSQLPwd,m_iDbType,m_iDbPort);
		LoadKeys();	  //加载非法关键词列表
		if (m_iIsReconn==1)
		{
			strTemp.Format("启用了定时自动重连,重连间隔:%d秒",m_iReconnInterval);
			AddToDispListShow(strTemp);
		}
		//strTemp.Format("连续单发最大次数为%d次,超过即会在信息末尾加'。'后再发",m_iMostSameSingleSendTime);
		//AddToDispListShow(strTemp);
		//Disconnect();
		m_LastActiveTime = ::GetTickCount() - 10000000;
		m_iLastErrorTime = ::GetTickCount() - 1000000;
		m_iLastReconnTime = ::GetTickCount();
		//加载未发送完的记录
		ReadBufFromFile();
		int iRunFlag = 0;
		int iSendInterval = 0;
		SetCmdTypeEx(g_iServerType,g_iReportFlag);
		while (true)
		{
			if (m_SendThreadContrl.ThreadExit)
			{
				m_IsRecvThreadRunLock.Lock();
				iRunFlag = m_iIsRecvThreadRun;
				m_IsRecvThreadRunLock.Unlock();
				if (iRunFlag==0)
				{
					break;
				}
				else
				{
					AddToDispListShow("等待接收线程退出",1);
					Sleep(500);
					continue;
				}
			}
			if (m_iSendInterval >= 10)
			{
				Delay(m_iSendInterval,&m_SendThreadContrl.ThreadExit);//发送线程执行间隔
			}
			if (m_iIsReconn==1)//定时重连
			{
				if(IsOverLastReconnTime(m_iReconnInterval)==true)	//重连时间到
				{
					AddToDispListShow("重连时间到");
					Disconnect();
					m_iLastReconnTime = ::GetTickCount();
					m_LastActiveTime = ::GetTickCount() - 10000000;
					continue;
				}
			}
			/************************************************************************/
			//登录
			iiRet = MessageOperate(OPTYPE_LOGIN,iErrorType,strError,mt,recvList,reportList);
			iNextRet = NextOptJudge(iiRet,iErrorType);
			if (iNextRet != 0) //发生错误
			{
				strTemp.Format("登录失败 [%s]",strError);
				AddToDispListShow(strTemp);
				m_LastActiveTime = ::GetTickCount() - 10000000;
				SetStatusInfoEx(3,-2);//账户余额:未查询
				Delay(10000,&m_SendThreadContrl.ThreadExit);	//登录失败,暂停10秒
				continue;	
			}
			else if (iErrorType == MSG_ERROR_LOGIN)
			{
				SetLastActiveTime();
				AddToDispListShow("登录成功");
				AddToDispListShow("等待查询余额,请稍后...");
				m_CheckFeeFlag = true;
				SetStatusInfoEx(1,1);	//已登录
			}
			/************************************************************************/
			//查询余额
			if (m_CheckFeeFlag == true && m_SendThreadContrl.ThreadExit == FALSE)
			{
				iiRet = MessageOperate(OPTYPE_CHECKFEE,iErrorType,strError,mt,recvList,reportList);
				AddToDispListShow(strError);
				m_CheckFeeFlag = false;
				iNextRet = NextOptJudge(iiRet,iErrorType);
				if (iNextRet < 0)
				{
					//设置断开连接，需要重新登录
					Disconnect();
					m_LastActiveTime = ::GetTickCount() - 10000000;
					SetStatusInfoEx(1,0);	//未登录
					SetStatusInfoEx(3,-2);//账户余额:未查询
					m_CheckFeeFlag = true;
					continue;
				}
				g_iBalance = iiRet;
				SetStatusInfoEx(3,iiRet);//账户余额
				SetLastActiveTime();
			}
			//取待发信息
			if (!ReadWaitMessage(dataBaseCtrl))//从数据库中读待发信息	
			{
				Delay(500,&m_SendThreadContrl.ThreadExit);
				continue;
			}				
			if (IsOverLastErrorTime(m_iErrorStopTime))	//如果超过了发送出错时间
			{
				/************************************************************************/
				//格式化信息到真正缓冲中				
				m_RealSendLock.Lock();
				iRet = m_RealSendList.GetSize();
				m_RealSendLock.Unlock();
				if (iRet <= 0)	//真正链表为空时才执行此操作
				{
					//合并相同单发信息,用一个函数操作
					UnionSameSingleMessage(dataBaseCtrl);
					iRet = GetKfSubmitPack(mt);
					if (iRet == 0)
					{
						mt.m_iTryTimes = 3;	//总共三次重发						
						iiRet = dataBaseCtrl.ConvertSendMessage(mt);//对号码及信息进行初步合法验证
 						if (iiRet <= 0)
 						{
							AddToDispListShow("发送失败["+mt.m_strFailReason+"]");
							AddToSentListShow("88888888",mt.m_strUserNumber,mt.m_strMsg,g_Conver.GetMsgULen(mt.m_strMsg),mt.m_iCount,6,mt.m_strFailReason,m_iDiffMobile,mt.m_strOperatorID);
							AddtoSendHistory(dataBaseCtrl,mt,1,mt.m_strFailReason);
							continue;
 						}
						//检查非法关键词
						if(m_iCheckKey != 0)	//需要查询关键词
						{
							iiRet = m_keyWord.KeyWordCheck(mt.m_strMsg,strTemp);
							if (iiRet==0)	//存在非法关键词
							{
								strError.Format("存在非法关键词[%s]",strTemp);
								strTemp.Format("发送失败[%s],内容[%s]",strError,mt.m_strMsg);
								AddToDispListShow(strTemp);
								AddToSentListShow("88888888",mt.m_strUserNumber,mt.m_strMsg,g_Conver.GetMsgULen(mt.m_strMsg),mt.m_iCount,6,strError,m_iDiffMobile,mt.m_strOperatorID);
								AddtoSendHistory(dataBaseCtrl,mt,1,strError);
								continue;
							}
						}
						//将合法的信息加入真正的发送缓冲
						ConvertPhoneAddMessage(mt,0,strIllegalList);
						//将不在号段内的信息保存失败结果
						int iErrorCount = 0;
						mt.m_strUserNumber = "";
						for (POSITION Pos_ = strIllegalList.GetHeadPosition(); Pos_ != NULL;)
						{
							mt.m_strUserNumber += strIllegalList.GetNext(Pos_)+" ";
							iErrorCount++;
						}
						mt.m_strUserNumber.Trim();
						if (mt.m_strUserNumber != "")
						{
							AddToDispListShow(mt.m_strUserNumber+"不在号段表内");
							AddToSentListShow("88888888",mt.m_strUserNumber,mt.m_strMsg,g_Conver.GetMsgULen(mt.m_strMsg),iErrorCount,5,"失败[不在号段表内]",m_iDiffMobile,mt.m_strOperatorID);
							AddtoSendHistory(dataBaseCtrl,mt,1,"号码不在号段表内");
						}	
					}
					else if (iRet == -1)
					{
						AddToDispListShow("发送缓冲队列无信息",1);
						Delay(m_iBufNullWaitTime,&m_SendThreadContrl.ThreadExit);
					}
					else
					{
						AddToDispListShow("读缓冲队列出错");
						Delay(100,&m_SendThreadContrl.ThreadExit);		
					}
				}
				/************************************************************************/
				//发送信息				
				iNextRet = -99;
				iiRet = -99;
				//int iNum = 0;
				while(true)		//把一个包中的发送完
				{
					m_SendHistoryLock.Lock();
					iHistLen = m_SendHistoryList.GetSize();
					m_SendHistoryLock.Unlock();
					if (iHistLen>m_iNumSendStop)	//如果历史表中的记录数大于一定数量,暂停发送
					{
						break;
					}
					iRet = GetRealSendPack(mt);
					if (iRet == 0)	//取到等待发送信息
					{
						iiRet = MessageOperate(OPTYPE_SEND,iErrorType,strError,mt,recvList,reportList);
						iErrorType = MSG_ERROR_SEND;
						iNextRet = NextOptJudge(iiRet,iErrorType);
						if (iNextRet == 0)
						{
							SetLastActiveTime();				
							//减账户余额
							g_iBalance -= mt.m_iCount;
							SetStatusInfoEx(3,g_iBalance);//账户余额
							AddToSentListShow(mt.m_strSpMsgId,mt.m_strUserNumber,mt.m_strMsg,mt.m_iMsgLen,mt.m_iCount,mt.m_iPhoneType,"成功",m_iDiffMobile,mt.m_strOperatorID);
							strTemp.Format("发送信息成功,编号[%s],内容[%s]",mt.m_strSpMsgId,mt.m_strMsg);
							AddToDispListShow(strTemp);
							AddtoSendHistory(dataBaseCtrl,mt,0,"");
						}
						else	//发送出问题了
						{
							if (iNextRet == -1)
							{
								//断开,需要重发
								Disconnect();
								m_LastActiveTime = ::GetTickCount() - 10000000;
								SetStatusInfoEx(1,0);//登录状态:断开
								SetStatusInfoEx(3,-2);//"账户余额:未查询"
								//重发三次
								if (mt.m_iTryTimes > 0)
								{
									strTemp.Format("第%d次发送失败,等待重发",4-mt.m_iTryTimes);
									AddToDispListShow(strTemp);
									mt.m_iTryTimes--;
									AddRealSendPack(mt);
								}
								else
								{
									strSend.Format("失败[%s]",strError);					
									AddToSentListShow(mt.m_strSpMsgId,mt.m_strUserNumber,mt.m_strMsg,mt.m_iMsgLen,mt.m_iCount,mt.m_iPhoneType,strSend,m_iDiffMobile,mt.m_strOperatorID);
									strTemp.Format("发送信息失败[%s][超过重发次数]",strError);
									AddToDispListShow(strTemp);
									//添加发送失败结果，错误处理
									AddtoSendHistory(dataBaseCtrl,mt,1,strError);
								}
								break;
							}
							else if (iNextRet == -2)
							{
								Disconnect();
								m_LastActiveTime = ::GetTickCount() - 10000000;
								SetStatusInfoEx(1,0);//登录状态:断开
								SetStatusInfoEx(3,-2);//"账户余额:未查询"
								SetLastErrorTime();	//设置最后出错时间
							}
							//-3不重连

							//添加发送失败结果，错误处理
							AddtoSendHistory(dataBaseCtrl,mt,1,strError);
							strSend.Format("失败[%s]",strError);		
							AddToSentListShow(mt.m_strSpMsgId,mt.m_strUserNumber,mt.m_strMsg,mt.m_iMsgLen,mt.m_iCount,mt.m_iPhoneType,strSend,m_iDiffMobile,mt.m_strOperatorID);
							strTemp.Format("发送信息失败[%s]",strError);
							AddToDispListShow(strTemp);
							break;
						}
					}
					else		//无信息等待发送
					{
						break;
					}
				}
				//如果要断开连接,至下一次循环,本次不接收上行和状态报告
				if (iNextRet == -1 || iNextRet == -2)
				{
					continue;
				}
			}
			else		//发送出错暂停间隔
			{
				Delay(500,&m_SendThreadContrl.ThreadExit);	//暂停100MS
			}
			if (g_iServerType != 0)		//如果是走SI平台，直接返回，接收交由另外一个线程执行
			{
				continue;
			}
			/************************************************************************/		
			//接收上行信息
			if (m_iIsRecvMo == 1 && m_SendThreadContrl.ThreadExit == FALSE)	//是否接收上行信息标志
			{
				iNextRet = -99;
				iiRet = -99;
				recvList.RemoveAll();
				iiRet = MessageOperate(OPTYPE_RECVMO,iErrorType,strError,mt,recvList,reportList);
				iNextRet = NextOptJudge(iiRet,iErrorType);
				if (iNextRet < 0)
				{
					//设置断开连接，需要重新登录
					AddToDispListShow(strError);
					Disconnect();					
					m_LastActiveTime = ::GetTickCount() - 10000000;
					SetStatusInfoEx(1,0);//登录状态:断开
					SetStatusInfoEx(3,-2);//"账户余额:未查询"
					continue;
				}
				else		//处理接收到的上行信息
				{
					if (iiRet > 0 && iiRet != 10000)	//有上行信息
					{
						//strTemp.Format("收到%d条上行信息",recvList.GetSize());
						//AddToDispListShow(strTemp);
						//将接收的信息加入数据库
						//WriteLogEx("保存上行信息开始",2);
						for ( i=0;i<recvList.GetSize();i++)
						{
							mo = recvList.GetAt(i);
							mo.strTime = mo.strDate +" "+ mo.strTime;
							AddToRecvListShow(mo.strTime,mo.strOrgAddr,mo.strMsg,mo.strDestAddr);
							strTemp.Format("上行信息:[%s,%s,%s,%s]",mo.strTime,mo.strOrgAddr,mo.strDestAddr,mo.strMsg);
							AddToDispListShow(strTemp,0);
							dataBaseCtrl.AddMoMessage(mo);
						}
						//WriteLogEx("保存上行信息结束",2);
						SetLastActiveTime();
					}
					else if (iiRet != 10000) //无上行信息 = 0
					{
						AddToDispListShow("无上行信息",1);
					}					
				}
			}			
			/************************************************************************/
			//接收状态报告
			if (m_iIsRecvReport == 1 && m_SendThreadContrl.ThreadExit == FALSE)	//是否接收上行信息标志
			{
				iNextRet = -99;
				iiRet = -99;
				reportList.RemoveAll();
				iiRet = MessageOperate(OPTYPE_RECVREPORT,iErrorType,strError,mt,recvList,reportList);				
				iNextRet = NextOptJudge(iiRet,iErrorType);
				if (iNextRet < 0)
				{
					//设置断开连接，需要重新登录
					AddToDispListShow(strError);
					Disconnect();
					m_LastActiveTime = ::GetTickCount() - 10000000;
					SetStatusInfoEx(1,0);//登录状态:断开
					SetStatusInfoEx(3,-2);//"账户余额:未查询"
					continue;
				}
				else		//处理接收到的状态报告
				{
					if (iiRet > 0 && iiRet != 10000)	//有状态报告
					{
						strTemp.Format("收到%d条状态报告",reportList.GetSize());
						AddToDispListShow(strTemp);
						//WriteLogEx("处理状态报告开始",2);
						for ( i=0;i<reportList.GetSize();i++)
						{
							report = reportList.GetAt(i);
							if ( report.strMsgStatus == "0")
							{
								strSend = "成功";
							}
							else if (report.strMsgStatus == "1")
							{
								strSend.Format("发送暂缓[%s]",report.strErrorCode);
							}
							else if (report.strMsgStatus == "2")
							{
								strSend.Format("失败[%s]",report.strErrorCode);
							}
							else
							{
								strSend.Format("失败[%s]",report.strErrorCode);
							}
							report.strTime = report.strDate +" "+ report.strTime;
							AddToRePortListShow(report.strTime,report.strMsgNo,strSend);
							dataBaseCtrl.UpdateSendResult(report);
						}
						g_ReportList.PostMessage(WM_VSCROLL,SB_BOTTOM,0);
						//WriteLogEx("处理状态报告结束",2);
						SetLastActiveTime();
					}
					else if (iiRet != 10000)//无状态报告 = 0
					{
						AddToDispListShow("无状态报告",1);
					}					
				}
			}
		}		
	}
	catch (...)
	{
		AddToDispListShow("发送线程异常");
		WriteLog("MessageSendProc异常",3);
	}
	Disconnect();
	m_LastActiveTime = ::GetTickCount() - 10000000;
	SetStatusInfoEx(1,0);//登录状态:断开
	SetStatusInfoEx(3,-2);//"账户余额:未查询"
	AddToDispListShow("发送线程退出");
	m_IsSendThreadRunLock.Lock();
	m_iIsSendThreadRun = 0;
	m_IsSendThreadRunLock.Unlock();
	//添加一个判断是否所有线程都已退出的函数,执行操作	
	OnActiveThreadExit(0);
	return;
}
/************************************************************************/
/*接收线程函数                                                          */
/************************************************************************/
void CMWMessageSendEx::RecvThreadProc(LPVOID lpParam)
{
	CMWMessageSendEx *pThis = (CMWMessageSendEx *)lpParam;
	::CoInitialize(NULL);
	pThis->RecvProc(); //执行线程函数体
	::CoUninitialize();
}
/************************************************************************/
/*接收线程执行函数                                                      */
/************************************************************************/
void CMWMessageSendEx::RecvProc()
{
	CString strTemp = "";
	CString strSend = "";			
	CDataBaseCtrl dataBaseCtrl;	//数据库操作对象	
	DeliverPackList recvList;	//上行信息列表
	ReportPackList reportList;	//状态报告列表
	int iNextRet = -99;		//下一步操作返回值
	int iiRet = -99;		//发送与接收返回值
	int iErrorType = -99;	//记录返回的错误类型
	CString strError = "";	//记录返回的错误信息
	int iRet = 0;	//获取待发结构体的返回值
	int i=0;		//循环控制变量
	int iHistLen = 0;//历史表记录数
	KfSubmitPack mt;
	ReportPack report;
	DeliverPack mo;
	try
	{	
		m_IsRecvThreadRunLock.Lock();
		m_iIsRecvThreadRun = 1;
		m_IsRecvThreadRunLock.Unlock();
		AddToDispListShow("接收线程启动");
		dataBaseCtrl.InitParams(m_dbSQLServer,m_dbSQLName,m_dbSQLUser,m_dbSQLPwd,m_iDbType,m_iDbPort);		
		SetCmdTypeEx(g_iServerType,g_iReportFlag);
		bool bRecvMoStop = false;
		bool bRecvRptStop = false;
		while (!m_RecvThreadContrl.ThreadExit)
		{	
			/************************************************************************/		
			//接收上行信息
			iNextRet = -99;
			iiRet = -99;
			bRecvMoStop = false;
			bRecvRptStop = false;
			recvList.RemoveAll();
			iiRet = MessageOperate(OPTYPE_RECVMO,iErrorType,strError,mt,recvList,reportList);
			iNextRet = NextOptJudge(iiRet,iErrorType);
			if (iiRet > 0 && iiRet != 10000)	//有上行信息
			{
				strTemp.Format("收到%d条上行信息",recvList.GetSize());
				AddToDispListShow(strTemp);
				//将接收的信息加入数据库
				for ( i=0;i<recvList.GetSize();i++)
				{
					mo = recvList.GetAt(i);
					mo.strTime = mo.strDate +" "+ mo.strTime;
					AddToRecvListShow(mo.strTime,mo.strOrgAddr,mo.strMsg,mo.strDestAddr);
					dataBaseCtrl.AddMoMessage(mo);
				}
			}
			else if (iiRet != 10000) //无上行信息 = 0
			{
				AddToDispListShow("无上行信息",1);
			}				
			else
			{
				bRecvMoStop = true;
			}

			/************************************************************************/
			//接收状态报告
			iNextRet = -99;
			iiRet = -99;
			reportList.RemoveAll();
			iiRet = MessageOperate(OPTYPE_RECVREPORT,iErrorType,strError,mt,recvList,reportList);				
			iNextRet = NextOptJudge(iiRet,iErrorType);
			if (iiRet > 0 && iiRet != 10000)	//有状态报告
			{
				strTemp.Format("收到%d条状态报告",reportList.GetSize());
				AddToDispListShow(strTemp);
				for ( i=0;i<reportList.GetSize();i++)
				{
					report = reportList.GetAt(i);
					if (report.strMsgStatus == "0")
					{
						strSend = "成功";
					}
					else if (report.strMsgStatus == "1")
					{
						strSend.Format("发送暂缓[%s]",report.strErrorCode);
					}
					else if (report.strMsgStatus == "2")
					{
						strSend.Format("失败[%s]",report.strErrorCode);
					}
					else
					{
						strSend.Format("失败[%s]",report.strErrorCode);
					}
					report.strTime = report.strDate +" "+ report.strTime;
					AddToRePortListShow(report.strTime,report.strMsgNo,strSend);
					dataBaseCtrl.UpdateSendResult(report);
				}
				g_ReportList.PostMessage(WM_VSCROLL,SB_BOTTOM,0);
			}
			else if (iiRet != 10000)//无状态报告 = 0
			{
				AddToDispListShow("无状态报告",1);
			}
			else
			{
				bRecvRptStop = true;
			}
			if (bRecvMoStop && bRecvRptStop)		//如果接收上行和状态报告的时间都没到，暂停500MS
			{
				Delay(500,&m_RecvThreadContrl.ThreadExit);
			}
		}
	}
	catch (...)
	{
		AddToDispListShow("接收线程异常");
		WriteLog("RecvProc异常",3);
	}
	AddToDispListShow("接收线程退出");
	m_IsRecvThreadRunLock.Lock();
	m_iIsRecvThreadRun = 0;
	m_IsRecvThreadRunLock.Unlock();
	//添加一个判断是否所有线程都已退出的函数,执行操作	
	OnActiveThreadExit(0);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//线程中控制延时的函数
void CMWMessageSendEx::Delay(int iMiniSecond,BOOL *bExit)
{
	for (int i=0;i<iMiniSecond;i+=5)
	{
		if (*bExit)
		{
			break;
		}
		Sleep( 5 );
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//从数据库中读取待发短信到发送缓冲中
BOOL CMWMessageSendEx::ReadWaitMessage(CDataBaseCtrl &dataBaseCtrl)
{
	try
	{		
		int iBufLen = 0;
		int iHistLen = 0;
		int iRealLen = 0;
		//真正发送缓冲
		m_RealSendLock.Lock();
		iRealLen = m_RealSendList.GetSize();
		m_RealSendLock.Unlock();
		if(iRealLen>0)
		{
			return TRUE;
		}
		//发送缓冲
		m_SendListLock.Lock();
		iBufLen = m_SubmitPackList.GetSize();
		m_SendListLock.Unlock();
		if(iBufLen>0)
		{
			return TRUE;
		}
		//发送历史
		m_SendHistoryLock.Lock();
		iHistLen = m_SendHistoryList.GetSize();
		m_SendHistoryLock.Unlock();
		if (iHistLen > 0)
		{
			return TRUE;
		}
		//读等待发送信息
		int iRet = -3;
		//WriteLogEx("从数据库读取待发信息开始",2);
		iRet = dataBaseCtrl.ReadWaitMessage(m_SubmitPackList,m_SendListLock);
		//WriteLogEx("从数据库读取待发信息结束",2);
		if (iRet == 0)	//无信息
		{
			Delay(m_iAddNullWaitTime,&m_SendThreadContrl.ThreadExit);
		}
		else if (iRet == -3)	//连接服务器失败
		{
			return FALSE;
		}
	}
	catch (...)
	{
		AddToDispListShow("读取数据异常");
		WriteLog("CMWMessageSendEx::ReadWaitMessage异常",3);
	}
	return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//添加发送缓存
void CMWMessageSendEx::AddKfSubmitPack(KfSubmitPack &submitPack)
{
	m_SendListLock.Lock();
	try
	{	
		m_SubmitPackList.Add(submitPack);
	}
	catch(...)
	{
		WriteLog("AddKfSubmitPack异常",3);
	}
	m_SendListLock.Unlock();
	return;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//添加发送缓存
void CMWMessageSendEx::AddKfSubmitPackEx(KfSubmitPack &submitPack)
{
	m_SendListLock.Lock();
	try
	{	
		m_SubmitPackList.InsertAt(0,submitPack);
	}
	catch(...)
	{
		WriteLog("AddKfSubmitPackEx异常",3);
	}
	m_SendListLock.Unlock();
	return;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//获取发送数据,返回0表示正常,-1表示无数据,-2表示出错
int CMWMessageSendEx::GetKfSubmitPack(KfSubmitPack &submitPack)
{
	int iRet = 0;
	m_SendListLock.Lock();
	try
	{
		int iSize = m_SubmitPackList.GetSize();
		if (iSize == 0)	//缓冲区无数据
		{
			iRet = -1;
		}
		else
		{
			submitPack = m_SubmitPackList.GetAt( 0 );
			m_SubmitPackList.RemoveAt( 0 );
			SetStatusInfoEx(2,iSize-1);//缓冲
			iRet = 0;	//正常获取到数据
		}		
	}
	catch(...)
	{
		iRet = -2;		//出错
		WriteLog("GetKfSubmitPack异常",3);
	}
	m_SendListLock.Unlock();
	return iRet;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//获取等待发送信息进行发送
int CMWMessageSendEx::GetRealSendPack(KfSubmitPack &sendPack)
{
	int iRet = 0;
	m_RealSendLock.Lock();
	try
	{		
		int iSize = m_RealSendList.GetSize();
		if (iSize == 0)	//缓冲区无数据
		{
			iRet = -1;
		}
		else
		{
			sendPack = m_RealSendList.GetAt( 0 );
			m_RealSendList.RemoveAt( 0 );
			iRet = 0;	//正常获取到数据
		}		
	}
	catch(...)
	{
		iRet = -2;		//出错
		WriteLog("GetRealSendPack异常",3);
	}
	m_RealSendLock.Unlock();
	return iRet;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//添加等待发送缓冲数据
void CMWMessageSendEx::AddRealSendPack(KfSubmitPack &sendPack)
{
	m_RealSendLock.Lock();
	try
	{		
		m_RealSendList.Add(sendPack);
	}
	catch(...)
	{
		WriteLog("AddRealSendPack异常",3);
	}
	m_RealSendLock.Unlock();
	return;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//当所有线程都退出时的操作,0表示发送线程，1表示写历史表线程
void CMWMessageSendEx::OnActiveThreadExit(int iFlag)
{
	try
	{
		Sleep(200);
		//m_activeNumLock.Lock();
		m_iActiveThreadNum--;
		//m_activeNumLock.Unlock();
		
		//当活动线程全部停止后,更新工具栏显示
		if (m_iActiveThreadNum == 0)
		{
			int iStatus = GetRunStatus();
			switch (iStatus)
			{
			case STATUS_RUN_NORMAL:
				{
					AddToDispListShow("发送线程正常运行");
					break;
				}
			case STATUS_STOP_ERROR:
				{
					AddToDispListShow("连接数据库出错,发送线程自动停止,请填入正确的数据库信息");
					break;
				}
			case STATUS_STOP_MANUAL:
				{
					//AddToDispListShow("发送线程停止");
					break;
				}
			case STATUS_STOP_NORMAL:		//配置更改,线程重启
				{
					AddToDispListShow("发送线程重启");
					break;
				}
			case STATUS_STOP_OVERRUNTIME:
				{
					AddToDispListShow("超过可执行时间段,线程自动停止");
					break;
				}
			default:
				{
					AddToDispListShow("线程自动停止");
					break;
				}
			}
			if (m_hWnd!=NULL)
			{
				::PostMessage(m_hWnd,WM_USERMSGSEND,0,iStatus);
			}
		}		
		Sleep(200);
		if (g_hwndClose != NULL)
		{
			::PostMessage(g_hwndClose,USERWINDOWMSG_1,0,0);
		}
	}
	catch (...)
	{
		WriteLog("OnActiveThreadExit异常",3);
	}	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//拆分号码及信息,根据号码打包添加到真正发送队列中,flag用于区分是放入真正发送表0,还是放入匹配表1
void CMWMessageSendEx::ConvertPhoneAddMessage(KfSubmitPack &mt,int iFlag,CStringList &strIllegalPhones)
{
	CString strTemp = "";
	int iIndex = 0;
	CString strPhoneTemp = "";
	CString MobilePhoneList = "";	//移动号码列
	CString UnicomPhoneList = "";	//联通号码列
	CString TelcomPhoneList = "";	//小灵通号码列
	int iMobilePhoneNum = 0;
	int iUnicomPhoneNum = 0;
	int iTelcomPhoneNum = 0;
	CString strTelComList = "";	//电信号码列
	int iTelComNumEx = 0;	//电信号码个数
	int iPhoneType = -1;
	int idx1 = 0;
	int idx2 = 0;
	strIllegalPhones.RemoveAll();
	CString strTempMesg = "";
	CString strOut = "";
	CString strMsg = "";
	//WriteLogEx("拆分号码及信息函数开始",3);
	try
	{		
		CString strPhoneList = mt.m_strUserNumber;
		strPhoneList.Trim();
		strPhoneList += " ";
		idx2 = strPhoneList.Find(' ', idx1);  //找第一个记录
		while(idx2 != -1)						//如果找到
		{
			strPhoneTemp = strPhoneList.Mid(idx1,idx2 - idx1); //取到一个记录	
			strPhoneTemp.Trim();
			if ( strPhoneTemp == "" )
			{
				//strIllegalPhones.AddTail(strPhoneTemp);
				idx1 = idx2 + 1;
				idx2 = strPhoneList.Find(' ', idx1);  //找下一个记录
				continue;
			}
			iPhoneType = -1;			
			iPhoneType = g_Conver.MobileType(strPhoneTemp);			
			if ( m_iDiffMobile == 0 && iPhoneType >= 0)
			{
// 				if ( iPhoneType == 1 || iPhoneType == 3)	//把联通与电信号段置为移动
// 				{
// 					iPhoneType = 0;
// 				}
				iPhoneType = 0;			//如果不区分号段,则所有号段按移动号段的配置统一处理
			}
			if ( iPhoneType == 0 )	//移动号码
			{
				iMobilePhoneNum++;
				MobilePhoneList += strPhoneTemp + " ";
			}
			else if ( iPhoneType == 1 ) //联通号码 
			{
				iUnicomPhoneNum++;
				UnicomPhoneList += strPhoneTemp + " ";
			}
			else if (iPhoneType == 2)		//小灵通号码 
			{
				iTelcomPhoneNum++;
				TelcomPhoneList += strPhoneTemp + " ";
			}
			else if (iPhoneType == 3)	//电信号码
			{
				iTelComNumEx++;
				strTelComList += strPhoneTemp + " ";
			}
			else		//号码类型出错
			{
				strIllegalPhones.AddTail(strPhoneTemp);
			}
			idx1 = idx2 + 1;
			idx2 = strPhoneList.Find(' ', idx1);  //找下一个记录
		}

		//号码已经拆分开，开始拆分信息
		strTempMesg = mt.m_strMsg;
		strOut = "";
		strMsg = strTempMesg;
		int iMsgLen = g_Conver.GetMsgULen(strMsg);
		if (iMobilePhoneNum > 0)	//拆分移动
		{
			if ( iMsgLen <= m_iSplitLength )	//无需拆分
			{
				mt.m_iMsgLen = iMsgLen;
				if ( iMobilePhoneNum > 0 )
				{
					mt.m_iPhoneType = 0;
					AddSendPackList( MobilePhoneList,mt,iMobilePhoneNum,m_iPhoneSplitLen );
				}				
			}
			else	//需要拆分
			{
				int iSplitLength = m_iSplitLength - 3;
				int nMsgNum = 0;
				nMsgNum = iMsgLen / iSplitLength + 1;
				if ( iMsgLen % iSplitLength == 0 )
				{
					nMsgNum--;
				}
				int i = 0;
				for (i=0;i<nMsgNum;)
				{
					i++;
					int iSplitRet = g_Conver.SplitMsg(strMsg,iSplitLength,strOut);
					if (iSplitRet>=0)
					{
						mt.m_strMsg.Format("%d/%d%s",i,nMsgNum,strOut);
						mt.m_iMsgLen = g_Conver.GetMsgULen(mt.m_strMsg);
						if ( iMobilePhoneNum > 0 )
						{
							mt.m_iPhoneType = 0;
							AddSendPackList( MobilePhoneList,mt,iMobilePhoneNum,m_iPhoneSplitLen );
						}						
					}
				}
			}
		}
		strOut = "";
		strMsg = strTempMesg;
		if (iUnicomPhoneNum > 0)	//拆分联通电信
		{
			if ( iMsgLen <= m_iSplitLengthUnicom )	//无需拆分
			{
				mt.m_iMsgLen = iMsgLen;
				mt.m_strMsg = strTempMesg;
				if ( iUnicomPhoneNum > 0 )
				{
					mt.m_iPhoneType = 1;
					AddSendPackList( UnicomPhoneList,mt,iUnicomPhoneNum,m_iPhoneSplitLen );
				}				
			}
			else	//需要拆分
			{
				int iSplitLength = m_iSplitLengthUnicom - 3;
				int nMsgNum = 0;
				nMsgNum = iMsgLen / iSplitLength + 1;
				if ( iMsgLen % iSplitLength == 0 )
				{
					nMsgNum--;
				}
				int i = 0;
				for (i=0;i<nMsgNum;)
				{
					i++;
					int iSplitRet = g_Conver.SplitMsg(strMsg,iSplitLength,strOut);
					if (iSplitRet>=0)
					{
						mt.m_strMsg.Format("%d/%d%s",i,nMsgNum,strOut);
						mt.m_iMsgLen = g_Conver.GetMsgULen(mt.m_strMsg);					
						if ( iUnicomPhoneNum > 0 )
						{
							mt.m_iPhoneType = 1;
							AddSendPackList( UnicomPhoneList,mt,iUnicomPhoneNum,m_iPhoneSplitLen );
						}
						
					}
				}
			}
		}
		strOut = "";
		strMsg = strTempMesg;
		if (iTelComNumEx > 0)	//拆分电信
		{
			if ( iMsgLen <= m_iSplitLengthChinaCom )	//无需拆分
			{
				mt.m_iMsgLen = iMsgLen;
				if ( iTelComNumEx > 0 )
				{
					mt.m_iPhoneType = 3;
					AddSendPackList(strTelComList,mt,iTelComNumEx,m_iPhoneSplitLen);
				}			
			}
			else	//需要拆分
			{
				int iSplitLength = m_iSplitLengthChinaCom - 3;
				int nMsgNum = 0;
				nMsgNum = iMsgLen / iSplitLength + 1;
				if ( iMsgLen % iSplitLength == 0 )
				{
					nMsgNum--;
				}
				int i = 0;
				for (i=0;i<nMsgNum;)
				{
					i++;
					int iSplitRet = g_Conver.SplitMsg(strMsg,iSplitLength,strOut);
					if (iSplitRet>=0)
					{
						mt.m_strMsg.Format("%d/%d%s",i,nMsgNum,strOut);
						mt.m_iMsgLen = g_Conver.GetMsgULen(mt.m_strMsg);
						if ( iTelComNumEx > 0 )
						{
							mt.m_iPhoneType = 3;
							AddSendPackList( strTelComList,mt,iTelComNumEx,m_iPhoneSplitLen );
						}					
					}
				}
			}
		}
		//拆分小灵通
		strOut = "";
		strMsg = strTempMesg;
		if ( iTelcomPhoneNum > 0 )
		{
			if (iMsgLen <= m_iSplitLengthChina)	//无需拆分
			{
				mt.m_iMsgLen = iMsgLen;
				mt.m_strMsg = strTempMesg;
				if (iTelcomPhoneNum > 0)
				{
					mt.m_iPhoneType = 2;
					AddSendPackList(TelcomPhoneList,mt,iTelcomPhoneNum,m_iPhoneSplitLen);
				}				
			}
			else	//需要拆分
			{
				int iSplitLength = m_iSplitLengthChina - 3;
				int nMsgNum = 0;
				nMsgNum = iMsgLen / iSplitLength + 1;
				if ( iMsgLen % iSplitLength == 0 )
				{
					nMsgNum--;
				}
				int i = 0;
				for (i=0;i<nMsgNum;)
				{
					i++;
					int iSplitRet = g_Conver.SplitMsg(strMsg,iSplitLength,strOut);
					if (iSplitRet>=0)
					{
						mt.m_strMsg.Format("%d/%d%s",i,nMsgNum,strOut);
						mt.m_iMsgLen = g_Conver.GetMsgULen(mt.m_strMsg);
						if ( iTelcomPhoneNum > 0 )
						{
							mt.m_iPhoneType = 2;
							AddSendPackList(TelcomPhoneList,mt,iTelcomPhoneNum,m_iPhoneSplitLen);
						}					
					}
				}
			}
		}		
	}
	catch (...)
	{
		WriteLog("ConvertPhoneAddMessage异常",3);
	}
	//WriteLogEx("拆分号码及信息函数结束",3);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//针对号码打包,可以根据传入参数控制每包最大号码数
void CMWMessageSendEx::AddSendPackList( CString &strPhoneList,KfSubmitPack &mt,int iCount,int iSplitLen)
{
	try
	{
		CString strDestAddr = strPhoneList;
		strDestAddr.Trim();
		strDestAddr += " ";
		//CString temp = "";
		int iCalCount = 0;
		if ( iCount > iSplitLen )		//号码需要打包
		{
			//找空格
			int idx1 = 0;
			int idx2 = 0;
			int i = 0;
			while(true)						//如果找到
			{ 
				if ( idx2 != -1 )
				{
					idx2 = strDestAddr.Find(' ', idx1);  //找下一个记录
					if ( idx2 == -1 )
					{
						continue;
					}
					idx1 = idx2 + 1;
					i ++;						
					if ( i == iSplitLen )	//刚好1000的时候
					{
						//目标手机号码列
						mt.m_strUserNumber = strDestAddr.Left(idx2);//15913218097
						//手机号码个数
						mt.m_iCount = iSplitLen;
						iCalCount += iSplitLen;
						AddRealSendPack( mt );
						strDestAddr = strDestAddr.Mid(idx2+1);
						i = 0;
						idx1 = 0;
						idx2 = 0;
					}
				}
				else
				{
					strDestAddr.Trim();
					//目标手机号码列
					if ( strDestAddr != "" )
					{
						mt.m_strUserNumber = strDestAddr;
						//手机号码个数
						mt.m_iCount = i;
						iCalCount += i;
						AddRealSendPack( mt );
					}				
					break;
				}
			}
		}
		else if (iCount > 0 && iCount <= iSplitLen)	//号码无需打包
		{
			//目标手机号码列
			mt.m_strUserNumber = strDestAddr; 
			//手机号码个数
			mt.m_iCount = iCount;
			AddRealSendPack( mt );
		}
		else		//无号码
		{

		}
	}
	catch (...)
	{
		WriteLog("AddSendPackList异常",3);
	}	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//读取配置文件,初始化配置
void CMWMessageSendEx::InitParams()
{
	try
	{
		CString strTemp = "";
		int iTemp = 0;
		CUserFile *pFile = NULL;
		pFile = CUserFile::GetInstance();
		if (pFile)
		{	
			int iRet = -1;
			CString strAppPath = "";
			pFile->GetAppPath(strAppPath);
			strAppPath += MWGATEINITFILE;
			//数据库信息
			iRet = pFile->ReadInitInfo(strAppPath,"database","dbserver",m_dbSQLServer,iTemp,0);
			if (iRet < 0)
			{
				m_dbSQLServer = "127.0.0.1";
			}
			iRet = pFile->ReadInitInfo(strAppPath,"database","dbname",m_dbSQLName,iTemp,0);
			if (iRet < 0)
			{
				m_dbSQLName = "test";
			}
			iRet = pFile->ReadInitInfo(strAppPath,"database","dbuser",m_dbSQLUser,iTemp,0);
			if (iRet < 0)
			{
				m_dbSQLUser = "test";
			}
			iRet = pFile->ReadInitInfo(strAppPath,"database","dbpwd",m_dbSQLPwd,iTemp,0);
			if (iRet < 0)
			{
				m_dbSQLPwd = "123456";
			}
			pFile->ReadInitInfo(strAppPath,"database","dbtype",strTemp,m_iDbType,1);			
			pFile->CheckInitValue(m_iDbType,1,4);
			pFile->ReadInitInfo(strAppPath,"database","dbport",strTemp,m_iDbPort,1);			
			pFile->CheckInitValue(m_iDbPort,0,1000000000);
			//用户配置信息
			iRet = pFile->ReadInitInfo(strAppPath,"Userinfo","Account",m_strAccount,iTemp,0);
			if (iRet < 0)
			{
				m_strAccount = "13600000412";
			}
			iRet = pFile->ReadInitInfo(strAppPath,"Userinfo","Pwd",m_strAccPwd,iTemp,0);
			if (iRet < 0)
			{
				m_strAccPwd = "123456";
			}
			//平台信息
			iRet = pFile->ReadInitInfo(strAppPath,"Server","Ip",m_strServerIP,iTemp,0);
			if (iRet < 0)
			{
				m_strServerIP = "192.169.1.128";
			}
			iRet = pFile->ReadInitInfo(strAppPath,"Server","Port",strTemp,m_iServerPort,1);
			if (iRet < 0)
			{
				m_iServerPort = 8018;
			}
			//号段
			iRet = pFile->ReadInitInfo(strAppPath,"NumberHead","Mobile",m_strMobileList,iTemp,0);
			if (iRet < 0)
			{
				m_strMobileList = "134,135,136,137,138,139,150,151,154,155,157,158,159,188";
			}
			iRet = pFile->ReadInitInfo(strAppPath,"NumberHead","Unicom",m_strUnicomList,iTemp,0);
			if (iRet < 0)
			{
				m_strUnicomList = "130,131,132,133,152,153,156";
			}
			iRet = pFile->ReadInitInfo(strAppPath,"NumberHead","Telecom",m_strTelecomList,iTemp,0);
			if (iRet < 0)
			{
				m_strTelecomList = "116,118,189";
			}
			iRet = pFile->ReadInitInfo(strAppPath,"NumberHead","Differ",strTemp,m_iDiffMobile,1);
			if (iRet < 0)
			{
				m_iDiffMobile = 0;
			}
			//发送线程配置
			iRet = pFile->ReadInitInfo(strAppPath,"Sendthread","Interval",strTemp,m_iSendInterval,1);
			if (iRet < 0)
			{
				m_iSendInterval = 500;
			}
			pFile->CheckInitValue(m_iSendInterval,5,2000);
			iRet = pFile->ReadInitInfo(strAppPath,"Sendthread","Bufnullwait",strTemp,m_iBufNullWaitTime,1);
			if (iRet < 0)
			{
				m_iBufNullWaitTime = 500;
			}
			pFile->CheckInitValue(m_iBufNullWaitTime,5,5000);
			iRet = pFile->ReadInitInfo(strAppPath,"Sendthread","Buflen",strTemp,g_iSendBufLen,1);
			pFile->CheckInitValue(g_iSendBufLen,100,1000);
			iRet = pFile->ReadInitInfo(strAppPath,"Sendthread","Splitlength",strTemp,m_iSplitLength,1);
			if (iRet < 0)
			{
				m_iSplitLength = 62;	//移动
			}
			pFile->CheckInitValue(m_iSplitLength,30,1000);
			iRet = pFile->ReadInitInfo(strAppPath,"Sendthread","SplitlengthUnicom",strTemp,m_iSplitLengthUnicom,1);
			if (iRet < 0)
			{
				m_iSplitLength =70;		//联通
			}
			pFile->CheckInitValue(m_iSplitLength,30,1000);
			iRet = pFile->ReadInitInfo(strAppPath,"Sendthread","SplitlengthChinacom",strTemp,m_iSplitLengthChinaCom,1);
			if (iRet < 0)
			{
				m_iSplitLength = 70;	//电信
			}
			pFile->CheckInitValue(m_iSplitLength,30,1000);
			iRet = pFile->ReadInitInfo(strAppPath,"Sendthread","SplitlengthChinaNum",strTemp,m_iSplitLengthChina,1);
			if (iRet < 0)
			{
				m_iSplitLength = 56;	//小灵通
			}
			pFile->CheckInitValue(m_iSplitLength,30,1000);

			//添加缓冲配置
			iRet = pFile->ReadInitInfo(strAppPath,"Addbuf","NoMessage",strTemp,m_iAddNullWaitTime,1);
			pFile->CheckInitValue(m_iAddNullWaitTime,500,5000);

			//出错停止时间
			iRet = pFile->ReadInitInfo(strAppPath,"StopTime","Error",strTemp,m_iErrorStopTime,1);
			if (iRet < 0)
			{
				m_iErrorStopTime = 0;
			}
			pFile->CheckInitValue(m_iErrorStopTime,1,5000);

			//子端口
			iRet = pFile->ReadInitInfo(strAppPath,"SubPort","type",strTemp,m_iUseSubPort,1);
			if (iRet < 0)
			{
				m_iUseSubPort = 1;
			}
			pFile->CheckInitValue(m_iUseSubPort,0,1);

			//接收配置
			iRet = pFile->ReadInitInfo(strAppPath,"Recv","RecvMo",strTemp,m_iIsRecvMo,1);
			if (iRet < 0)
			{
				m_iIsRecvMo = 1;
			}
			pFile->CheckInitValue(m_iIsRecvMo,0,1);
			iRet = pFile->ReadInitInfo(strAppPath,"Recv","RecvReport",strTemp,m_iIsRecvReport,1);
			if (iRet < 0)
			{
				m_iIsRecvReport = 1;
			}
			pFile->CheckInitValue(m_iIsRecvReport,0,1);

			//号码打包数量
			iRet = pFile->ReadInitInfo(strAppPath,"PhoneSplit","PhoneSplit",strTemp,m_iPhoneSplitLen,1);
			if (iRet < 0)
			{
				m_iPhoneSplitLen = 500;
			}
			pFile->CheckInitValue(m_iPhoneSplitLen,1,1000);

			//运行时间段
			iRet = pFile->ReadInitInfo(strAppPath,"runtime","time",m_strRunTime,iTemp,0);
			if ( iRet < 0)
			{
				m_strRunTime = "08:00--21:00";
			}
			//客户端检测非法关键词
			iRet = pFile->ReadInitInfo(strAppPath,"keycheck","type",strTemp,m_iCheckKey,1);
			if (iRet == 0)
			{
				m_iCheckKey = 0;
			}
			//接收间隔
			pFile->ReadInitInfo(strAppPath,"Recv","RecvMoInterval",strTemp,m_iRecvMoInterval,1);
			pFile->ReadInitInfo(strAppPath,"Recv","RecvReportInterval",strTemp,m_iRecvReportInterval,1);
			pFile->CheckInitValue(m_iRecvMoInterval,1,100);
			pFile->CheckInitValue(m_iRecvReportInterval,1,100);
			//定时重连
			pFile->ReadInitInfo(strAppPath,"Reconn","type",strTemp,m_iIsReconn,1);
			pFile->ReadInitInfo(strAppPath,"Reconn","interval",strTemp,m_iReconnInterval,1);
			pFile->CheckInitValue(m_iIsReconn,0,1);
			pFile->CheckInitValue(m_iReconnInterval,1,100000);

			//写历史表线程
			pFile->ReadInitInfo(strAppPath,"SaveHistThread","threadnum",strTemp,m_iAddHistThreadNum,1);
			pFile->CheckInitValue(m_iAddHistThreadNum,1,5);
			pFile->ReadInitInfo(strAppPath,"SaveHistThread","interval",strTemp,m_iAddHistInterval,1);
			pFile->CheckInitValue(m_iAddHistInterval,0,5);
			pFile->ReadInitInfo(strAppPath,"SaveHistThread","nullwait",strTemp,m_iAddHistNullWait,1);
			pFile->CheckInitValue(m_iAddHistNullWait,300,1000);
			pFile->ReadInitInfo(strAppPath,"SaveHistThread","numstopsend",strTemp,m_iNumSendStop,1);
			pFile->CheckInitValue(m_iNumSendStop,500,1500);
			//激活连接间隔			
			pFile->ReadInitInfo(strAppPath,"Testconn","interval",strTemp,m_iTestConInterval,1);
			pFile->CheckInitValue(m_iTestConInterval,60,150);		
			
			//连续单发最大次数
			pFile->ReadInitInfo(strAppPath,"samesingle","times",strTemp,m_iMostSameSingleSendTime,1);
			pFile->CheckInitValue(m_iMostSameSingleSendTime,5,1000);	
			m_strAccount = pFile->StrDecode(m_strAccount);
			m_strAccPwd = pFile->StrDecode(m_strAccPwd);
			m_dbSQLUser = pFile->StrDecode(m_dbSQLUser);
			m_dbSQLPwd = pFile->StrDecode(m_dbSQLPwd);
			
			pFile->ReadInitInfo(strAppPath,"Server","type",strTemp,g_iServerType,1);
			pFile->ReadInitInfo(strAppPath,"Report","flag",strTemp,g_iReportFlag,1);
			pFile->CheckInitValue(g_iServerType,0,1);
			pFile->CheckInitValue(g_iReportFlag,0,1);
			//初始化号段信息
			g_Conver.InitNumbers(m_strMobileList,m_strUnicomList,m_strTelecomList);
			int iCmdType = -99;
			pFile->ReadInitInfo(strAppPath,"acctype","cmdtype",strTemp,iCmdType,1);
			if (iCmdType == -99)
			{
				iCmdType = 25;
			}
			pFile->CheckInitValue(iCmdType,10,150);
		}
		else
		{
			AddToDispListShow("获取配置文件类指针出错,程序退出");
			Sleep(200);
			exit(0);
		}
	}
	catch (...)
	{
		WriteLog("InitParams异常",3);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//重新加载配置文件,做相应处理
void CMWMessageSendEx::ReInitParams()
{
	try
	{
		CString stroldAccount = m_strAccount;
		CString stroldAccPwd = m_strAccPwd;
		CString stroldIp = m_strServerIP;
		int ioldPort = m_iServerPort;
		CString stroldDbServer = m_dbSQLServer;
		CString stroldDbName = m_dbSQLName;
		CString stroldDbUser = m_dbSQLUser;
		CString stroldDbPwd = m_dbSQLPwd;
		int ioldServerType = g_iServerType;
		InitParams( );
		SetCmdTypeEx(g_iServerType,g_iReportFlag);
		if ( //数据库信息
			stroldDbServer != m_dbSQLServer||
			stroldDbName != m_dbSQLName||
			stroldDbUser != m_dbSQLUser||
			stroldDbPwd != m_dbSQLPwd||
			//服务器信息
			stroldAccount != m_strAccount||
			stroldAccPwd != m_strAccPwd||
			stroldIp != m_strServerIP||
			ioldPort != m_iServerPort||
			ioldServerType != g_iServerType)	//如果这两项信息有任何更改,重启线程
		{
			SetRunStatus(STATUS_STOP_NORMAL);	//正常停止状态
			Stop();
		}
	}	
	catch (...)
	{
		WriteLog("ReInitParams异常",3);
	}	
}
/************************************************************************/
/* 根据与服务器的交互,返回下一步的操作标志                              */
/************************************************************************/
//返回0表示成功,-1断开重连[重发]，-2暂停发送[不重发]，-3发送失败（不重发)
int CMWMessageSendEx::NextOptJudge(int iErrorCode,int iErrorType)
{
	if ( iErrorCode >= 0 )
	{
		return 0;
	}
	else
	{
		switch ( iErrorType )
		{
			case MSG_ERROR_KEEPONLINE:
				{
					return -1;	//断开重连
				}
			case MSG_ERROR_LOGIN:
				{
					return -1;	//断开重连
				}
			case MSG_ERROR_SEND:		
				{
					switch ( iErrorCode )
					{
					case -101:						
					case -102:					
					case -103:
					case -104:
					case -105:
					case -198:
					case -200:
						{							
							return -1;	//断开重连,重发
						}
					case -10015:
						{
							//strRet = "服务器繁忙";
							return -1;
						}				
					case -10099:
						{
							//strRet = "其它错误";
							return -1;
						}
					case -10009:
						{
							//strRet = "其它错误";
							return -1;
						}
					case -10003:
						{
							//strRet = "用户余额不足";
							return -2;	//暂停发送,不重发
						}
					case -10037:
						{
							//strRet = "单发相同的信息超过流量暂停";
							return -2;
						}					
					case -10044:
						{
							//strRet = "该帐号没有扩展子端口权限";
							return -2;
						}					
					case -10029:
						{
							//strRet = "此用户没有权限从此通道发送信息";
							return -2;
						}
					case -1:
						{
							//strError = "参数中包含有NULL值";
							return -3;
							break;
						}
					case -2:
						{
							//strError = "电话号码个数超过1000";
							return -3;
							break;
						}
					case -10:
						{
							//strError = "申请缓存空间失败";
							return -3;
							break;
						}			
					case -11:
						{
							//strError = "电话号码中有非数字字符";
							return -3;
							break;
						}
					case -12:
						{
							//strError = "有异常电话号码";
							return -3;
							break;
						}
					case -13:
						{
							//strError = "电话号码个数与实际个数不相等";
							return -3;
							break;
						}
					case -14:
						{
							//strError = "实际号码个数超过1000";
							return -3;
							break;
						}
					case -10017:
						{
							//strRet = "此时间段不允许发送";
							return -3;
						}
					case -10002:
						{
							//strRet = "提交格式不正确";
							return -3;
						}
					case -10004:
						{
							//strRet = "手机号码不正确";
							return -3;
						}
					case -10008:
						{
							//strRet = "账号类型不支持该功能";
							return -3;
						}					
					case -10011:
						{
							//strRet = "信息内容超长";
							return -3;
						}
					case -10012:
						{
							//strRet = "不能发送联通号码";
							return -3;
						}					
					case -10019:
						{
							//strRet = "手机列表或规则错误";
							return -3;
						}					
					case -10030:
						{
							//strRet = "不能发送移动号码";
							return -3;
						}
					case -10031:
						{
							//strRet = "手机号码(段)非法";
							return -3;
						}
					case -10033:
						{
							//strRet = "非法关键词";
							return -3;
						}
					case -10035:
						{
							//strRet = "不能发送多个手机";
							return -3;
						}
					case -10036:
						{
							//strRet = "不能发送单个手机";
							return -3;
						}
					case -10051:
						{
							//strRet = "只能发送移动号码";
							return -3;
						}
					case -10052:
						{
							//strRet = "只能发送联通号码";
							return -3;
							break;
						}
					case -10053:
						{
							//strRet = "只能发送电信小灵通号码";
							return -3;
						}
					default:
						{
							return -3;
						}
					}
					break;
				}
			case MSG_ERROR_RECVMO:
				{
					switch (iErrorCode)
					{
					case -101:						
					case -102:					
					case -103:
					case -104:
					case -105:
					case -198:
					case -200:
						{							
							return -1;
						}
					case -10015:
						{
							//strRet = "服务器繁忙";
							return -1;
						}					
					case -10099:
						{
							//strRet = "其它错误";
							return -1;
						}
					case -10009:
						{
							//strRet = "其它错误";
							return -1;
						}
					case -300:
						{
							break;
						}
					case -10041:
						{
							//strRet = "读取上行MO信息太快";
							break;
						}
					case -10042:
						{
							//strRet = "读取状态报告太快";
							break;
						}
					default:
						{
							return -1;
						}
					}
					break;
				}
			case MSG_ERROR_RECVREPORT:
				{
					switch (iErrorCode)
					{
					case -101:						
					case -102:					
					case -103:
					case -104:
					case -105:
					case -198:
					case -200:
						{							
							return -1;
						}
					case -10015:
						{
							//strRet = "服务器繁忙";
							return -1;
						}					
					case -10099:
						{
							//strRet = "其它错误";
							return -1;
						}
					case -10009:
						{
							//strRet = "其它错误";
							return -1;
							break;
						}
					case -300:
						{
							break;
						}
					case -10041:
						{
							//strRet = "读取上行MO信息太快";
							break;
						}
					case -10042:
						{
							//strRet = "读取状态报告太快";
							break;
						}
					default:
						{
							return -1;
						}
					}
					break;
				}
			case MSG_ERROR_CHECKFEE:
				{
					switch (iErrorCode)
					{
					case -101:						
					case -102:
					case -103:
					case -104:
					case -105:
					case -198:
					case -200:
						{
							return -1;
						}
					case -10015:
						{
							//strRet = "服务器繁忙";
							return -1;
						}					
					case -10099:
						{
							//strRet = "其它错误";
							return -1;
						}
					case -10009:
						{
							//strRet = "其它错误";
							return -1;
						}
					case -10041:
						{
							//strRet = "读取上行MO信息太快";
							break;
						}
					case -10042:
						{
							//strRet = "读取状态报告太快";
							break;
						}
					default:
						{
							return -1;
						}
					}
					break;
				}
			default:
				{
					break;
				}
		}
	}
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//起动线程
void CMWMessageSendEx::Run()
{
	try
	{
		int i = 0;
		InitParams();
		//初始化账户信息
		SetParams(m_strServerIP,m_iServerPort,m_strAccount,m_strAccPwd);
		//创建发送线程
		m_SendThreadContrl.ThreadExit = FALSE;
		m_SendThreadContrl.pThread = AfxBeginThread((AFX_THREADPROC)MessageSendThreadProc,(LPVOID)this);
		m_iActiveThreadNum++;
		if (g_iServerType == 1)
		{
			//创建接收线程
			m_RecvThreadContrl.ThreadExit = FALSE;
			m_RecvThreadContrl.pThread = AfxBeginThread((AFX_THREADPROC)RecvThreadProc,(LPVOID)this);
			m_iActiveThreadNum++;
		}		
		if(m_iAddHistThreadNum>5)
		{
			m_iAddHistThreadNum = 5;
		}
		for (i=0;i<m_iAddHistThreadNum;i++)
		{
			if (m_AddResultThreadContrl[i].ThreadExit == TRUE)
			{
				ThreadParam *lpParam = new ThreadParam;
				lpParam->pThis = this;
				lpParam->iThreadNO = i;
				m_AddResultThreadContrl[i].ThreadExit = FALSE;
				m_AddResultThreadContrl[i].pThread = AfxBeginThread((AFX_THREADPROC)AddSendResultThread,(LPVOID)lpParam);
				m_iActiveThreadNum++;
			}
		}
		SetRunStatus(STATUS_RUN_NORMAL);
	}
	catch (...)
	{
		WriteLog("Run异常",3);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//停止发送线程
void CMWMessageSendEx::Stop()
{
	try
	{
		m_RecvThreadContrl.ThreadExit = TRUE;
		m_SendThreadContrl.ThreadExit = TRUE;
		int i=0;
		for (i=0;i<5;i++)
		{
			m_AddResultThreadContrl[i].ThreadExit = TRUE;
		}
	}
	catch (...)
	{
		WriteLog("Stop异常",3);
	}	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//应用程序退出时的处理
void CMWMessageSendEx::AppExit()
{
	try
	{
		CString strAppPath="";
		CUserFile *pFile = CUserFile::GetInstance();
		if ( pFile )
		{
			pFile->GetAppPath(strAppPath);
			pFile->WriteInitInfo(strAppPath+MWGATEINITFILE,"window","runstatus","",1,1);
		}
		HWND hwnd  = FindWindow(NULL,"网关辅助程序_"+g_strWindowName);
		if (hwnd)
		{
			::PostMessage(hwnd,WM_CLOSE,0,0);
		}
		Sleep(200);
		//新建立一个将数据重新存入数据库的线程
		m_InsertBackContrl.pThread = AfxBeginThread((AFX_THREADPROC)InsertBackThreadProc, (LPVOID)this);
		m_InsertBackContrl.ThreadExit = TRUE;
		m_iActiveThreadNum++;
		if (m_iActiveThreadNum > 0)
		{
			SetRunStatus(STATUS_STOP_MANUAL);
			Stop();		//手动终止线程
			CDbGateClose dbClose;
			dbClose.m_ThreadNum = m_iActiveThreadNum;
			dbClose.DoModal();
			Sleep(100);
		}
	}
	catch (...)
	{
		WriteLog("AppExit异常",3);
	}	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//查询余额函数
void CMWMessageSendEx::CheckFee()
{
	try
	{
		AddToDispListShow("等待查询余额,请稍后...");
		if (GetRunStatus() == STATUS_RUN_NORMAL)
		{
			m_CheckFeeFlag = true;
		}
		else
		{
			KfSubmitPack mt;
			DeliverPackList recvList;
			ReportPackList reportList;

			int iNextRet = -99;		//下一步操作返回值
			int iiRet = -99;		//发送与接收返回值
			int iErrorType = -99;	//记录返回的错误类型
			CString strError = "";	//记录返回的错误信息
			//InitParams();
			iiRet = MessageOperate( OPTYPE_LOGIN,iErrorType,strError,mt,recvList,reportList);	
			iNextRet = NextOptJudge(iiRet,iErrorType);	
			if ( iNextRet != 0 ) //发生错误
			{
				AddToDispListShow(strError);
				Disconnect();			
				m_LastActiveTime = ::GetTickCount() - 10000000;	
				SetStatusInfoEx(1,0);	//断开
				return;
			}
			iiRet = MessageOperate( OPTYPE_CHECKFEE,iErrorType,strError,mt,recvList,reportList);
			iNextRet = NextOptJudge(iiRet,iErrorType);
			if ( iNextRet != 0 ) //发生错误
			{
				Disconnect();
				m_LastActiveTime = ::GetTickCount() - 10000000;
				SetStatusInfoEx(1,0);	//断开
				AddToDispListShow(strError);
				return;
			}
			AddToDispListShow(strError);			
			SetStatusInfoEx(3,iiRet);			
			m_CheckFeeFlag = false;			
		}
	}
	catch (...)
	{
		WriteLog("CheckFee异常",3);
	}	
}

//全局函数
/************************************************************************/
/*                                                                      */
/************************************************************************/
//加入显示列表
void AddToDispListShow(CString showStr,int iFlag)
{
	//加入日志
	if (iFlag == 0)
	{
		WriteLog(showStr,1);
	}
	showStr.Trim();
	g_DisListLock.Lock();
	try
	{		
		int iCount = g_DispList.GetItemCount();
		if ( iCount >= 1000 )		//只保留1000条记录
		{
			g_DispList.DeleteAllItems();
			iCount = 0;
		}
		if ( iCount > 0 )
		{
			if ( g_DispList.GetItemText(iCount-1,1) == showStr)
			{
				g_DispList.SetItemText(iCount-1 , 0, CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S") );
			}
			else
			{
				g_DispList.InsertItem(iCount,"");
				g_DispList.SetItemText(iCount , 0, CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"));
				g_DispList.SetItemText(iCount , 1, showStr);
				g_DispList.PostMessage(WM_VSCROLL,SB_BOTTOM,0);
			}
		}
		else
		{
			g_DispList.InsertItem(iCount,"");
			g_DispList.SetItemText(iCount , 0, CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"));
			g_DispList.SetItemText(iCount , 1, showStr);
			g_DispList.PostMessage(WM_VSCROLL,SB_BOTTOM,0);
		}		
	}	
	catch (...)
	{
		CString tempStr = "";
		tempStr.Format("AddToDispListShow异常,参数:%s",showStr);
		WriteLog(tempStr,3);
	}
	g_DisListLock.Unlock();
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void WriteConsoleInfo(CString srcStr)
{
	//CConsolePrinter::Instance()->print(srcStr);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//写日志
void WriteLog(CString srcStr,int iFlag)
{
	//WriteConsoleInfo(srcStr);
	if (g_iIsNeedToLog == 1)
	{
		CTime currTime = CTime::GetCurrentTime();
		try
		{
			CString filepath = "";
			CString destStr = "";
			CString folder = "";
			CString filename = "";
			CString strAppPath = "";
			if (iFlag == 0)			//发送日志
			{
				folder = "log";
				filename.Format("%s_sendlog.txt",currTime.Format("%Y%m%d"));
			}
			else if(iFlag == 1)		//信息日志
			{
				folder = "log";
				filename.Format("%s_log.txt",currTime.Format("%Y%m%d"));
			}
			else if ( iFlag == 2 )	//缓冲日志
			{
				folder = "log";
				filename.Format("%s_bufinfo.txt",currTime.Format("%Y%m%d"));
			}
			else			//异常日志 (3)
			{
				folder = "exception";
				filename.Format("%s_exception.txt",currTime.Format("%Y%m%d"));
			}

			CUserFile *pFile = CUserFile::GetInstance();
			if ( pFile )
			{
				pFile->GetAppPath(strAppPath);
			}
			CString folderName = "";
			folderName.Format("%s%s",strAppPath,folder);
			if(_access(folderName,0)!=0)
			{
				if( mkdir(folderName) != 0 )//创建目录
				{
					return;
				}
			}					
			destStr.Format("%s	%s",currTime.Format("%Y-%m-%d %H:%M:%S"),srcStr);
			filepath.Format("%s\\%s",folderName,filename);
			FILE *fp;
			char szfile[1000]={0};
			sprintf(szfile,filepath);
			fp=fopen(szfile,"a+");
			fprintf(fp,"%s\r\n",destStr);
			fclose(fp);
		}
		catch (...)
		{
		}
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//写调试日志
void WriteLogEx(CString srcStr,int iFlag)
{
	if (g_iDebugLog == 1)		//写调试日志标志为真时,写调试日志
	{
		CTime currTime = CTime::GetCurrentTime();
		try
		{
			CString filepath = "";
			CString destStr = "";
			CString folder = "";
			CString filename = "";
			CString strAppPath = "";
			
			if(iFlag == 1)		//网络,收,发,击活,等日志
			{
				folder = "debuglog";
				filename.Format("%s_netdebug.txt",currTime.Format("%Y%m%d"));
			}
			else if ( iFlag == 2 )	//数据库操作相关日志
			{
				folder = "debuglog";
				filename.Format("%s_dbdebug.txt",currTime.Format("%Y%m%d"));
			}
			else if ( iFlag == 5 )	//数据库操作相关日志
			{
				folder = "netlog";
				filename.Format("%s_netlog.txt",currTime.Format("%Y%m%d"));
			}
			else if ( iFlag > 45 )		//保存发送历史的记录
			{
				folder = "debuglog";
				filename.Format("%s_dbdebug_thread%d.txt",currTime.Format("%Y%m%d"),iFlag);
			}
			else			//其他日志 (3)
			{
				folder = "debuglog";
				filename.Format("%s_elsedebug.txt",currTime.Format("%Y%m%d"));
			}

			CUserFile *pFile = CUserFile::GetInstance();
			if ( pFile )
			{
				pFile->GetAppPath(strAppPath);
			}
			CString folderName = "";
			folderName.Format("%s%s",strAppPath,folder);
			if(_access(folderName,0)!=0)
			{
				if( mkdir(folderName) != 0 )//创建目录
				{
					return;
				}
			}					
			destStr.Format("%s	%s",currTime.Format("%Y-%m-%d %H:%M:%S"),srcStr);
			filepath.Format("%s\\%s",folderName,filename);
			FILE *fp;
			char szfile[1000]={0};
			sprintf(szfile,filepath);
			fp=fopen(szfile,"a+");
			fprintf(fp,"%s\r\n",destStr);
			fclose(fp);
		}
		catch (...)
		{
		}
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMWMessageSendEx::SetRunStatus( int iFlag )
{
	m_iRunStatus = iFlag;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CMWMessageSendEx::GetRunStatus()
{
	return m_iRunStatus;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//判断是否在正确的运行时间范围内,0正确，1错误
int CMWMessageSendEx::IsRightSendTime(CString strSendTime,CString strStartTime,CString strEndTime)
{
	if ( strStartTime == strEndTime )
	{
		return 0;
	}
	int iRet = 1;
	try
	{
		CTime currTime = CTime::GetCurrentTime();
		int iYear,iMonth,iDay;
		iYear = currTime.GetYear();
		iMonth = currTime.GetMonth();
		iDay = currTime.GetDay();
		int iHour,iMinite;
		int iStartHour,iStartMinite;
		int iEndHour,iEndMinite;

		CString strTemp = "";
		strTemp = strSendTime.Right(8);
		iHour = atoi(strTemp.Left(2));
		strTemp = strTemp.Right(5);
		iMinite = atoi(strTemp.Left(2));
		iStartHour = atoi(strStartTime.Left(2));
		iStartMinite = atoi(strStartTime.Right(2));
		iEndHour = atoi(strEndTime.Left(2));
		iEndMinite = atoi(strEndTime.Right(2));
		CTime sendTime,startTime,endTime;
		sendTime = CTime(iYear,iMonth,iDay,iHour,iMinite,0,0);
		startTime = CTime(iYear,iMonth,iDay,iStartHour,iStartMinite,0,0);
		endTime = CTime(iYear,iMonth,iDay,iEndHour,iEndMinite,0,0);
		CTimeSpan timeSpan1;
		CTimeSpan timeSpan2;
		CTimeSpan timeSpan;

		timeSpan1 = sendTime - startTime;		
		timeSpan2 = endTime - sendTime;

		timeSpan = endTime - startTime;
		if ( timeSpan < 0 )			//结束时间小于开始时间
		{
			if (timeSpan1 >= 0 || timeSpan2 > 0)
			{
				return 0;
			}
			else
			{
				return 1;
			}			
		}
		else if ( timeSpan > 0 )			//结束时间大于开始时间
		{
			if (timeSpan1 >= 0 && timeSpan2 > 0)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else								//结束时间等于开始时间
		{
			return 0;
		}	
		iRet = 0;
	}
	catch (...)
	{
		iRet = 1;
		WriteLog("IsRightSendTime异常",3);
	}	
	return iRet;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//0需要，1已经不在可执行时间段,2设置不需要 [暂时未使用]
int CMWMessageSendEx::IsNeedToReRunOnError()
{
	try
	{
		int iRet = IsRightSendTime(CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"),m_strRunTime.Left(5),m_strRunTime.Right(5));
		if ( iRet == 0 )
		{
			if ( m_iAutoReRun == 1 )		//如果设置为需要自动运行
			{
				if ( GetRunStatus() == STATUS_STOP_ERROR )
				{
					return 0;
				}			
			}
			else
			{
				return 2;
			}
		}
		else
		{
			return 1;
		}		
	}
	catch (...)
	{
		WriteLog("IsNeedToReRunOnError异常",3);
	}
	return 0;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//自动重运行配置
int CMWMessageSendEx::IsNeedToReRunOnTime()
{
	return IsRightSendTime(CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"),m_strRunTime.Left(5),m_strRunTime.Right(5));
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
//设置最后出错时间
void CMWMessageSendEx::SetLastErrorTime()
{
	m_iLastErrorTime = ::GetTickCount();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//判断是否已经超过发送出错等待时间
bool CMWMessageSendEx::IsOverLastErrorTime(unsigned int iErrorStopTime )
{
	DWORD t2 = 0;
	t2 = ::GetTickCount();
	DWORD tSpan = 0;
	tSpan = t2 - m_iLastErrorTime;
	if (tSpan > iErrorStopTime * 1000) //iDelay 秒
	{
		return true;
	}
	else
	{
		AddToDispListShow("发送出错等待");
		return false;
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//判断是否已经达到重连时间
bool CMWMessageSendEx::IsOverLastReconnTime(unsigned int iReconnInterval)
{
	DWORD t2 = 0;
	t2 = ::GetTickCount();
	DWORD tSpan = 0;
	tSpan = t2 - m_iLastReconnTime;
	if (tSpan > iReconnInterval * 1000) //iDelay 秒
	{
		return true;
	}
	else
	{
		return false;
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//合并单发信息,返回合并了多少条记录
int CMWMessageSendEx::UnionSameSingleMessage(CDataBaseCtrl &dataBaseCtrl)
{
	int iRet = 0;	
	m_SendMatchList.clear();
	//WriteLogEx("合并单发信息函数开始",3);
	m_SendListLock.Lock();
	try
	{
		int iSize = m_SubmitPackList.GetSize();
		if (iSize == 0)	//缓冲区无数据
		{
			iRet = -1;
		}
		else
		{
			int i = 0;
			KfSubmitPack mt;
			KfSubmitPack mtSend;
			CString lastMsg = "";
			int iLastPhoneType = -100; //上个号段类型
			int iUnionCount = 0;
			CString strTempID = "";
			for (i=0;i<iSize;i++)
			{
				mt = m_SubmitPackList.GetAt(i);
				if (lastMsg == "" && iLastPhoneType == -100)	//第一条信息
				{
					lastMsg = mt.m_strMsg;
					//求号段类型
					iLastPhoneType = 1;
					if (dataBaseCtrl.IsSingleSend(mt.m_strUserNumber))	//是单发
					{
						iLastPhoneType = ComputePhoneType(mt.m_strUserNumber);
						iUnionCount++;
						mt.m_strUserNumber.Trim();
						mtSend = mt;
						m_SendMatchList[mt.m_iLocalID] = mt;
						strTempID.Format("%d",mt.m_iLocalID);
						mtSend.m_strIdList += strTempID;											
					}
				}
				else
				{
					if (mt.m_strMsg == lastMsg && dataBaseCtrl.IsSingleSend(mt.m_strUserNumber)==true)
					{
						if (iLastPhoneType != ComputePhoneType(mt.m_strUserNumber))//求号段类型
						{
							break;
						}
						iUnionCount++;
						mt.m_strUserNumber.Trim();
						mtSend.m_strUserNumber += " "+mt.m_strUserNumber;
						m_SendMatchList[mt.m_iLocalID] = mt;
						strTempID.Format("%d",mt.m_iLocalID);
						mtSend.m_strIdList += " "+strTempID;
						if (iUnionCount >= m_iPhoneSplitLen)
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
			if (iUnionCount > 1)	//大于1表示为组合信息
			{
				for (i=0;i<iUnionCount;i++)
				{
					m_SubmitPackList.RemoveAt(0);
				}
				mtSend.m_iSendType = 100;	//100表示是组合后的信息,以便发送后进行匹配
				m_SubmitPackList.InsertAt(0,mtSend);
			}
		}
	}
	catch( ... )
	{
		iRet = -2;		//出错
		WriteLog("UnionSameSingleMessage异常",3);
	}
	m_SendListLock.Unlock();
	//WriteLogEx("合并单发信息函数结束",3);
	return iRet;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//将发送后的信息加入历史列表
void CMWMessageSendEx::AddtoSendHistory(CDataBaseCtrl &dataCtrl,KfSubmitPack &mt,int iFlag,CString strError)
{
	int i=0;
	CString strPhoneArray[1001] = {""};
	CString strIdArray[1001] = {""};
	CString strMsgID = "";
	CString tempLeft = "";
	CString tempRight = "";
	CString tempStr = "";
	int iRight = 0;
	int tempLen = 0;
	KfSubmitPack mtSave;
	mt.m_strUserNumber.Trim();
	mt.m_strIdList.Trim();
	int iPhoneNum = 0;	
	try
	{
		iPhoneNum= StringSplit(mt.m_strUserNumber+" ",strPhoneArray,' ');
		StringSplit(mt.m_strIdList+" ",strIdArray,' ');
		tempStr = mt.m_strSpMsgId;
		tempStr.Trim();
		tempLen = tempStr.GetLength();
		tempLeft = tempStr.Left(tempLen - 6);
		tempRight = tempStr.Right(6);
		iRight = atoi(tempRight);
		mt.m_iTryTimes = 3;
		mt.m_strFailReason = strError;
		mt.m_strFailReason.Replace("'","''");
		mtSave = mt;
		m_SendHistoryLock.Lock();
		try
		{
			for (i=0;i<iPhoneNum;i++)
			{
				if (iFlag != 0)	//如果失败
				{
					strMsgID = "88888888";
				}
				else
				{
					//iRight += i;
					strMsgID.Format("%06d",iRight+i);
					strMsgID = tempLeft + strMsgID;
				}
				if (mt.m_iSendType==100)	//如果是组合后的信息
				{
					GetMatchMtPack(mtSave,atoi(strIdArray[i]));
					if (mtSave.m_iLocalID <= 0)	//没有获取到
					{
						mtSave = mt;
					}
				}
				mtSave.m_strFailReason = strError;
				mtSave.m_iSendResultFlag = iFlag;
				mtSave.m_strUserNumber = strPhoneArray[i];
				mtSave.m_strMsg = mt.m_strMsg;
				mtSave.m_strSpMsgId = strMsgID;
				mtSave.m_iCount = 1;
				m_SendHistoryList.Add(mtSave);
			}
		}
		catch (...)
		{
		}	
		m_SendHistoryLock.Unlock();
	}
	catch (...)
	{
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//添加发送历史表
int CMWMessageSendEx::AddSendResult(CDataBaseCtrl &dataCtrl,KfSubmitPack &mt,int iFlag)
{
	return dataCtrl.AddSendResult(mt,iFlag);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//此处用于拆分号码及匹配ID号
int CMWMessageSendEx::StringSplit(CString srcStr,CString strArray[],char splitChar)
{
	try
	{
		int idx1 = 0;
		int idx2 = 0;
		int iCount = 0;
		idx2 = srcStr.Find(splitChar, idx1);  //找第一个记录
		while(idx2 != -1)						//如果找到
		{ 
			strArray[iCount] = srcStr.Mid(idx1,idx2 - idx1); //取到一个记录
			idx1 = idx2 + 1; 
			idx2 = srcStr.Find(splitChar, idx1);  //找下一个记录
			iCount++;
		} 
		return iCount;
	}
	catch (...)
	{
	}
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//查找匹配的单发信息
void CMWMessageSendEx::GetMatchMtPack(KfSubmitPack &mt,int iLocalID)
{
	mt = m_SendMatchList[iLocalID];
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//加载关键词
void CMWMessageSendEx::LoadKeys()
{
	CString strTemp = "";
	CString strAppPath = "";
	CUserFile *pFile = CUserFile::GetInstance();
	if (pFile != NULL)
	{
		pFile->GetAppPath(strAppPath);
		strAppPath += KEYWORDFILE;
		//加载非法关键词列表
		m_keyWord.SetDBInfo(strAppPath);
		int iCount = 0;
		int iRet = m_keyWord.LoadAllKeyWords(iCount);
		if ( iRet == 0 )
		{
			strTemp.Format("成功加载%d个非法关键词",iCount);
			AddToDispListShow(strTemp);
		}
		else
		{
			switch(iRet)
			{
			case -3:
				AddToDispListShow("加载非法关键词列表异常");
				break;
			default:
				AddToDispListShow("加载非法关键词列表失败");
				break;
			}			
		}
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//保存未发记录
void CMWMessageSendEx::WriteBufToFile(KfSubmitPack &mt)
{
	CString strBuf = "";
	mt.m_strMsg.Replace("'","");
	try
	{
		strBuf.Format("%d'%d'%d'%d'%d'%d'%d'%d'%d'%d'%d'%d'%d'%d'%s'%s'%s'%s'%s'%s'%s'%s'%s'%s'%s'%s'%s'%s'%s'%s'%s'%s'%s'%s'%s'%s",
			mt.m_iSendType,				//发送类型
			mt.m_iCount,				//接收号码数
			mt.m_iCountA,				//临时接收号码数
			mt.m_iPhoneType,			//所属号段
			mt.m_iTryTimes,				//重试次数
			mt.m_iMsgLen,				//信息长度
			mt.m_iPrecedence,			//短信优先级
			mt.m_iLocalID,				//本地ID(INT型)
			mt.m_iReverse1,				//保留字段1
			mt.m_iReverse2,				//保留字段2
			mt.m_iReverse3,				//保留字段3
			mt.m_iReverse4,				//保留字段4
			mt.m_iReverse5,				//保留字段5
			mt.m_iSendResultFlag,		//发送结果，0成功，1失败
			mt.m_strSendType,			//发送类型
			mt.m_strUserNumber,			//接收方号码列表
			mt.m_strUserNumberA,		//临时接收方号码列表
			mt.m_strPtMsgId,			//消息本地标识
			mt.m_strSpMsgId,			//消息ID(发送成功后的信息编号)
			mt.m_strMsg,				//短信内容
			mt.m_strCreatTime,			//信息生成时间
			mt.m_strSubTime,			//信息提交到数据库时间
			mt.m_strOperatorID,			//操作员ID
			mt.m_strServiceID,			//服务ID
			mt.m_strTaskName,			//任务名
			mt.m_strLocalID,			//信息本地ID
			mt.m_strSubOprationType,	//子操作类型
			mt.m_strSubPort,			//子端口
			mt.m_strUserId,				//保留字段
			mt.m_strMsgStatus,			//保留字段1
			mt.m_strRecvStatus,			//保留字段2
			mt.m_strMainPort,			//保留字段3
			mt.m_strFailReason,			//保留字段4
			mt.m_strReverse5,			//保留字段5
			mt.m_strReverse6,			//保留字段6
			mt.m_strIdList);			//用于保存合并后的信息ID集合)
		FILE *fp;
		char szfile[1000]={0};
		CString strAppPath = "";
		CUserFile *pFile = CUserFile::GetInstance();
		if ( pFile )
		{
			pFile->GetAppPath(strAppPath);
			sprintf(szfile,strAppPath+"\\"+BUFSAVEFILE);
			fp=fopen(szfile,"a+");
			fprintf(fp,"%s\r\n",pFile->StrEncode(strBuf));
			fclose(fp);
		}		
	}
	catch (...)
	{
	}	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//将未发送的记录加载
void CMWMessageSendEx::ReadBufFromFile()
{
	CString strBuf = "'";
	CString m_strFileName = "";
	KfSubmitPack mt;
	CString strArray[100] = {""};
	try
	{
		CString strAppPath = "";
		CUserFile *pFile = CUserFile::GetInstance();
		if ( pFile )
		{
			pFile->GetAppPath(strAppPath);			
		}
		else
		{
			return;
		}
		m_strFileName = strAppPath+"\\"+BUFSAVEFILE;
		if(_access(m_strFileName,0)!=0)
		{
			return;
		}
		
		CStdioFile   file;
		CString   cstr ="";
		CFileException   fe;
		if(file.Open(m_strFileName,CFile::modeRead,&fe))   
		{ 
			file.SeekToBegin();
			while (file.ReadString(cstr))
			{
				cstr.Trim();
				if (cstr != "")
				{
					cstr = pFile->StrDecode(cstr);
					StringSplit(cstr,strArray,'\'');
					mt.m_iSendType=atoi(strArray[0]);			//发送类型
					mt.m_iCount=atoi(strArray[1]);				//接收号码数
					mt.m_iCountA=atoi(strArray[2]);				//临时接收号码数
					mt.m_iPhoneType=atoi(strArray[3]);			//所属号段
					mt.m_iTryTimes=atoi(strArray[4]);			//重试次数
					mt.m_iMsgLen=atoi(strArray[5]);				//信息长度
					mt.m_iPrecedence=atoi(strArray[6]);			//短信优先级
					mt.m_iLocalID=atoi(strArray[7]);			//本地ID(INT型)
					mt.m_iReverse1=atoi(strArray[8]);			//保留字段1
					mt.m_iReverse2=atoi(strArray[9]);			//保留字段2
					mt.m_iReverse3=atoi(strArray[10]);			//保留字段3
					mt.m_iReverse4=atoi(strArray[11]);			//保留字段4
					mt.m_iReverse5=atoi(strArray[12]);			//保留字段5
					mt.m_iSendResultFlag=atoi(strArray[13]);	//发送结果，0成功，1失败
					mt.m_strSendType=strArray[14];			//发送类型
					mt.m_strUserNumber=strArray[15];		//接收方号码列表
					mt.m_strUserNumberA=strArray[16];		//临时接收方号码列表
					mt.m_strPtMsgId=strArray[17];			//消息本地标识
					mt.m_strSpMsgId=strArray[18];			//消息ID(发送成功后的信息编号)
					mt.m_strMsg=strArray[19];				//短信内容
					mt.m_strCreatTime=strArray[20];			//信息生成时间
					mt.m_strSubTime=strArray[21];			//信息提交到数据库时间
					mt.m_strOperatorID=strArray[22];		//操作员ID
					mt.m_strServiceID=strArray[23];			//服务ID
					mt.m_strTaskName=strArray[24];			//任务名
					mt.m_strLocalID=strArray[25];			//信息本地ID
					mt.m_strSubOprationType=strArray[26];	//子操作类型
					mt.m_strSubPort=strArray[27];			//子端口
					mt.m_strUserId=strArray[28];			//保留字段
					mt.m_strMsgStatus=strArray[29];			//保留字段1
					mt.m_strRecvStatus=strArray[30];		//保留字段2
					mt.m_strMainPort=strArray[31];			//保留字段3
					mt.m_strFailReason=strArray[32];		//保留字段4
					mt.m_strReverse5=strArray[33];			//保留字段5
					mt.m_strReverse6=strArray[34];			//保留字段6
					mt.m_strIdList=strArray[35];
					mt.m_timeSendTime = COleDateTime::GetCurrentTime();
					if (mt.m_iCount > 0)
					{
						AddRealSendPack(mt);
					}					
				}
			}
			file.Close();
		}
		file.Remove(m_strFileName);
		AddToDispListShow("成功从文件加载到待发信息");
	}
	catch (...)
	{
	}	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMWMessageSendEx::SetFrameHwnd(HWND hwnd)
{
	m_hWnd = hwnd;
}

/************************************************************************/
/*计算单发信息的所属号段                                                  */
/************************************************************************/
int CMWMessageSendEx::ComputePhoneType(CString &strPhone)
{
	int iPhoneType = -1;
	strPhone.Trim();
	iPhoneType = g_Conver.MobileType(strPhone);			
	if (m_iDiffMobile == 0)
	{
		if ( iPhoneType == 1 || iPhoneType == 3)	//把联通与电信号段置为移动
		{
			iPhoneType = 0;
		}
	}
	return iPhoneType;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMWMessageSendEx::AddSingleSend(CString strPhone,CString strContent,CString strSubport)
{
	try
	{
		KfSubmitPack mt;
		mt.m_iCount = 1;
		mt.m_strUserNumber = strPhone;
		mt.m_strMsg = strContent;
		COleDateTime currTime = COleDateTime::GetCurrentTime();
		mt.m_strSubTime = currTime.Format("%y-%m-%d %H:%M:%S");
		mt.m_strCreatTime = mt.m_strSubTime;
		mt.m_timeSendTime = currTime;
		mt.m_strSubPort = strSubport;
		mt.m_strPtMsgId = "88888888";
		mt.m_strSpMsgId = "88888888";
		mt.m_iLocalID = 0;
		m_SubmitPackList.Add(mt);
	}
	catch (...)
	{
	}	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//通过窗口消息设置状态栏信息
void CMWMessageSendEx::SetStatusInfoEx(int iIndex,int iCount)
{
	try
	{
		if (m_hWnd != NULL)
		{
			::PostMessage(m_hWnd,WM_USERMSGSTATUS,iIndex,iCount);
		}
		else
		{
			WriteLog("设置状态栏信息时句柄为空",3);
		}
	}
	catch (...)
	{
		WriteLog("SetStatusInfoEx异常",3);
	}	
}