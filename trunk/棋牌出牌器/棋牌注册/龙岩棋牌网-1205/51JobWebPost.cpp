#include "StdAfx.h"
#include ".\51jobwebpost.h"
#include "UserMessagePump.h"
#include <io.h>
#include <odbcinst.h>
#include <afxdb.h>
#include <direct.h>
#include "UserFile.h"
#include "ADOFile/AdoRecordSet.h"
#include "WindowFind.h"
#include <Iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib")
HANDLE  g_hEvent = NULL;			//事件句柄
CString g_strCheckCode = "";
bool g_ExitFlag = true;
RegisterPackList g_RegisterList;
int g_iDeleteFlag = 0;
int g_iShowWidth = 0;
int g_iShowHeight = 0;
int g_iAotuChangeIp = 0;
int g_iAotuCheckCode = 0;
int g_iIpRegNum = 0;
int g_iIpChangeType = 0;		//IP切换方式
int g_iCheckIpFlag = 0;			//0不检测IP是否变化，1通过RAS检测IP变化，2通过POST方式检测
int g_iReCheckIpCount = 0;		//在一次拨号成功后重新检测IP是否变化的总次数
int g_iTimeOut = 0;				//超时
int g_iCheckCodeType = 0;		//验证码自动检测配置，是否为测试
int g_iCodeErrorNum = 0;		//验证码验证错误重试次数
CRasDialMgr g_RasMgr;
CString g_strCurrIP = "";
CString g_strDirect = "";		//图像保存目录
CString g_strFileName = "";
int g_iRegWebFlag = 1;
int g_iIsChangeMac =0;
/************************************************************************/
/*                                                                      */
/************************************************************************/
//添加发送缓存
void AddRegisterPack(RegisterPack &submitPack)
{
	try
	{	
		g_RegisterList.Add(submitPack);
	}
	catch(...)
	{
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//添加发送缓存
void AddRegisterPackEx(RegisterPack &submitPack)
{
	try
	{	
		g_RegisterList.InsertAt(0,submitPack);
	}
	catch(...)
	{
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//获取发送数据,返回0表示正常,-1表示无数据,-2表示出错
int GetRegisterPack(RegisterPack &submitPack)
{
	int iRet = 0;
	try
	{
		int iSize = g_RegisterList.GetSize();
		if (iSize == 0)	//缓冲区无数据
		{
			iRet = -1;
		}
		else
		{
			submitPack = g_RegisterList.GetAt( 0 );			
			iRet = 0;	//正常获取到数据
		}		
	}
	catch(...)
	{
		iRet = -2;		//出错
	}
	return iRet;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void DeleteRegisterPack(RegisterPack &rgPack)
{
	for (int i=0;i<g_RegisterList.GetSize();i++)
	{
		if (g_RegisterList.GetAt(i).strAcc == rgPack.strAcc)
		{
			g_RegisterList.RemoveAt(i);
			return;
		}
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void AddToShowList(RegisterPack rgPack)
{
	CString strStauts = "";
	switch (rgPack.iStatus)
	{
	case 0:
		break;
	case 1:
		strStauts = "√";
		break;
	case 2:
		strStauts = "X";
		break;
	default:
		break;
	}
	int nCount = g_AccList.GetItemCount();
	bool isExist = false;
	for ( int i = 0;i<nCount;i++)
	{
		if (g_AccList.GetItemText(i,1) == rgPack.strAcc&&g_AccList.GetItemText(i,5) != "√")
		{
			isExist = true;
			g_AccList.SetItemText(i,5,strStauts);
			g_AccList.SetItemText(i,6,rgPack.strMemo);
			g_AccList.SetItemText(i,8,rgPack.strIp);
			break;
		}		
	}
	if (!isExist)
	{
		CString listIndex = "";
		listIndex.Format("%d",nCount);
		g_AccList.InsertItem(nCount,"",0);
		g_AccList.SetItemText(nCount,0,listIndex);
		g_AccList.SetItemText(nCount,1,rgPack.strAcc);
		g_AccList.SetItemText(nCount,2,rgPack.strPwd);
		g_AccList.SetItemText(nCount,3,rgPack.strMail);
		g_AccList.SetItemText(nCount,4,rgPack.strIntroMan);
		g_AccList.SetItemText(nCount,5,strStauts);
		g_AccList.SetItemText(nCount,6,rgPack.strMemo);
		g_AccList.SetItemText(nCount,7,rgPack.strNickName);
		g_AccList.SetItemText(nCount,8,rgPack.strIp);
	}	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//写日志
void WriteLog(CString srcStr,int iFlag)
{
	CTime currTime = CTime::GetCurrentTime();
	try
	{
		CString filepath = "";
		CString destStr = "";
		CString folder = "";
		CString filename = "";
		CString strAppPath = "";
		if (iFlag == 0)			//注册成功列表
		{
			folder = "log";
			filename.Format("%s_registerList.txt",currTime.Format("%Y%m%d"));
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
		else if ( iFlag == 3 )	//缓冲日志
		{
			folder = "log";
			filename.Format("%s_insertErrorAcc.txt",currTime.Format("%Y%m%d"));
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
		destStr.Format("%s	%s",srcStr,currTime.Format("%Y-%m-%d %H:%M:%S"));
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
/************************************************************************/
/*                                                                      */
/************************************************************************/
C51JobWebPost::C51JobWebPost()
{
    //初始化信息
    m_nErrorTime = 0;
	m_LoginFlag = false;
	m_OpenFreshPageFlag = false;
	m_ExceptionNum = 0;
	m_FreshTimes = 0;
	switch (g_iRegWebFlag)
	{
	case 0:
		m_strServerURL = "www.7ple.com";
		break;
	case 1:
		m_strServerURL = "www.7pmi.com";
		break;
	case 2:
	default:
		m_strServerURL = "www.7psw.com";
		break;
	}	
	m_TimeInterval = 3;
	m_strHeader = "Content-Type: application/x-www-form-urlencoded";
	m_strHeader += "\r\nAccept:image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, application/x-silverlight, */*";
	m_strHeader += "\r\nAccept-Language:zh-cn";
	m_strHeader += "\r\nAccept-Encoding:gzip, deflate";//
	m_strHeader += "\r\nCache-Control:no-cache";
	m_strHeader += "\r\nUser-Agent:Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 1.1.4322; .NET CLR 2.0.50727)";
	m_strHeader += "\r\nConnection:Keep-Alive";
	m_iCheckFlag = 0;
	g_hEvent = CreateEvent(NULL, FALSE, TRUE, NULL);	//创建事件对象
	CString strAppPath = "";
	CString strTemp = "";
	g_iDeleteFlag = 0;
	CUserFile *pFile = CUserFile::GetInstance();
	if (pFile)
	{
		pFile->GetAppPath(strAppPath);
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","type",strTemp,g_iDeleteFlag,1);
		pFile->CheckInitValue(g_iDeleteFlag,0,1);
	}
	m_iNeedChageIP = 0;
	m_iIpSuccNum = 0;
	m_strOldIP = "";
	m_iCodeErrorNum = 0;
	m_iIsNeedChangeMac = 0;
// 	CInternetSession session;
// 	strTemp = "";
// 	char temp[10001] = {0};
// 	CStdioFile *pFile1 = session.OpenURL("http://blog.csdn.net/TragicGuy/archive/2009/11/26/4881708.aspx");
// 	if (pFile != NULL)
// 	{
// 		long len = pFile1->GetLength();
// 		if (len > 10000)
// 		{
// 			len = 10000;
// 		}		
// 		pFile1->SeekToBegin();
// 		pFile1->Read(temp,len);
// 		temp[len] = '\0';
// 		strTemp.Format("%s",temp);
// 		strTemp.MakeLower();
// 		if (strTemp.Find("myownpower") !=-1)
// 		{
// 			g_iRegWebFlag = 1;
// 		}
// 		else
// 		{
// 			g_iRegWebFlag = 0;
// 		}
// 	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
C51JobWebPost::~C51JobWebPost()
{
	CUserFile *pFile = CUserFile::GetInstance();
	if (pFile)
	{
		delete pFile;
		pFile = NULL;
	}
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
//用户登录
bool C51JobWebPost::UserRegister()
{
	try
	{
		switch (g_iRegWebFlag)
		{
		case 0:
			m_strServerURL = "www.7ple.com";
			break;
		case 1:
			m_strServerURL = "www.7pmi.com";
			break;
		case 2:
		default:
			m_strServerURL = "www.7psw.com";
			break;
		}
		CString strError = "";
		CString sendStr = "";
		ShowMessage("正在获取注册页面...");
		CString objectlink = "";
		switch (g_iRegWebFlag)
		{
		case 0:
			objectlink = "/reg.jhtml?siteID=11";
			break;
		case 1:
			objectlink = "/reg.jhtml?siteID=11";
			break;
		case 2:
		default:
			objectlink = "/reg.jhtml?siteID=18";
			break;
		}
		CString strHeaders = m_strHeader;
		//int iRet = RequestGetPage(m_Session,m_pConnection,m_strServerURL,objectlink,strHeaders);
		int iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,
			strError);//"148.233.239.24",3128,"","",0);
		if ( iRet != 0 )
		{
			TRACE("获取页面出错");
			ShowMessage("获取注册页面失败");
			m_nErrorTime ++;
			DeleteObjectExEx();
			return false;
		}
		m_strWorkBuf.MakeLower();
		//AfxMessageBox(m_strWorkBuf.Right(m_strWorkBuf.GetLength() - 200));
		//检查是否打开简历管理页
		int iPos = m_strWorkBuf.Find("找回密码用");
		if (iPos >= 0)
		{
			ShowMessage("成功获取注册页,正在获取验证码...");
		}
		else
		{
			return false;
		}
		CString postStr = "";
		CString strShow = "";		
		while(!g_ExitFlag)
		{
			try
			{
				RegisterPack rgPack;
				int iRet = GetRegisterPack(rgPack);
				if (iRet == -1)
				{
					Sleep(700);
					continue;
				}
				else if (iRet == -2)
				{
					Sleep(2000);
					continue;
				}
				/************************************************************************/
				/* 此处先获取注册页面的信息,以备填充                                    */
				/************************************************************************/
				//HWND hwnd = ::FindWindow("IEFrame","棋牌乐 - 注册帐号 - Microsoft Internet Explorer");
				m_hwnd = ::FindWindow("#32770","帐号注册");
				while(!m_hwnd)
				{
					m_hwnd = ::FindWindow("#32770","帐号注册");
					ShowMessage("请打开客户端注册窗口");
				}
				char str[128] = {0};
				HWND hwnd = NULL;
				CWindowFind find;
				//hwnd = find.MyFindWindow("Internet Explorer_Server","",hwnd);
				hwnd = find.GetChildWindow(m_hwnd,1);
				hwnd = find.GetChildWindow(hwnd,1);
				hwnd = find.GetChildWindow(hwnd,1);
				hwnd = find.GetChildWindow(hwnd,1);
				GetClassName(hwnd,str,sizeof(str));//获取类名 
				if(strcmp(str,"Internet Explorer_Server")==0)
				{
					IHTMLDocument2 *pDoc = GetDocInterface(hwnd);
					GetHtmlCtrl(pDoc);
				}
				rgPack.strIp = g_strCurrIP;
				m_iCheckFlag = 0;
				g_strCheckCode = "";
				if (m_iCheckFlag == 0)
				{
					//删除旧的验证码
					if (_access(g_strDirect+g_strFileName,0) == 0)
					{
						CFile::Remove(g_strDirect+g_strFileName);
					}
					ShowMessage("正在获取验证码...");
					m_iCheckFlag = 0;
					objectlink = "/RegRandomCodeCtrl";
					//strHeaders = m_strHeader + "\r\nReferer:http://www.dajiale365.com/reg.shtml";
					iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError,1);
					if (iRet != 0)
					{
						ShowMessage("获取验证码失败");
						m_nErrorTime ++;
						DeleteObjectExEx();
						return false;
					}
					else
					{
						ShowMessage("获取验证码成功");	
						strShow.Format("正在注册:%s %s",rgPack.strAcc,rgPack.strPwd);
						ShowMessage(strShow);
					}
				}		
				while(!g_ExitFlag)
				{
					if ( g_strCheckCode != "")
					{
						break;
					}
					Sleep(100);
				}
				if (g_ExitFlag)
				{
					DeleteObjectExEx();
					return false;
				}
				CString strFailReason = "";
				CString strCheckCode = g_strCheckCode;
				if (strCheckCode == "r")
				{
					continue;
				}
				objectlink = "/regAction.jhtml";

				switch (g_iRegWebFlag)
				{
				case 0:
					strHeaders = m_strHeader + "\r\nReferer:http://www.7ple.com/reg.jhtml?siteID=11";
					break;
				case 1:
					strHeaders = m_strHeader + "\r\nReferer:http://www.7pmi.com/reg.jhtml?siteID=11";
					break;
				case 2:
				default:
					strHeaders = m_strHeader + "\r\nReferer:http://www.7psw.com/reg.jhtml?siteID=18";
					break;
				}	
				
				//strHeaders += "\r\nx-requested-with: XMLHttpRequest";
				//strHeaders += "\r\nContent-Type: application/x-www-form-urlencoded";
				CString strTime = CTime::GetCurrentTime().Format("%m%d%H%M%S");
				//strCheckCode = NativeToAscii(strCheckCode);
				//strCheckCode = "确学装衣";
				//ConvertGBKToUtf8(strCheckCode);
				m_strMac.MakeUpper();
				switch (g_iRegWebFlag)
				{
				case 0:
					ConvertGBKToUtf8(strCheckCode);
					strCheckCode = URLEncode(strCheckCode);
					postStr.Format("username=%s&password=%s&repassword=%s&sex=1&myhead=1&email=%s&siteID=11&7pmiReferrerUrl=&7pmiLocalUrl=&selfinfo=&machineserial=%s&transactionid=%s&verify=%s&ptvodaccounts=&introducer=%s&checkcode=%s&agree=on",rgPack.strAcc,rgPack.strPwd,rgPack.strPwd,rgPack.strMail,m_strMac,m_strTrans,m_strVerify,rgPack.strNickName,strCheckCode);
					break;
				case 1:
					postStr.Format("username=%s&password=%s&repassword=%s&sex=1&myhead=1&email=%s&siteID=11&7pmiReferrerUrl=&7pmiLocalUrl=&selfinfo=&machineserial=%s&transactionid=%s&verify=%s&ptvodaccounts=&introducer=%s&checkcode=%s&agree=on",rgPack.strAcc,rgPack.strPwd,rgPack.strPwd,rgPack.strMail,m_strMac,m_strTrans,m_strVerify,rgPack.strNickName,strCheckCode);
					break;
				case 2:
				default:
					ConvertGBKToUtf8(strCheckCode);
					strCheckCode = URLEncode(strCheckCode);
					postStr.Format("username=%s&password=%s&repassword=%s&sex=1&myhead=1&email=%s&siteID=18&7pmiReferrerUrl=&7pmiLocalUrl=&selfinfo=&machineserial=%s&transactionid=%s&verify=%s&ptvodaccounts=&introducer=%s&checkcode=%s&agree=on",rgPack.strAcc,rgPack.strPwd,rgPack.strPwd,rgPack.strMail,m_strMac,m_strTrans,m_strVerify,rgPack.strNickName,strCheckCode);
					break;
				}
				iRet = RequestPostPageExExEx(m_strServerURL,objectlink,strHeaders,postStr,strError,0);//
				if ( iRet != 0 )
				{
					TRACE("POST出错");
					m_nErrorTime ++;
					DeleteObjectExEx();
					return false;
				}
				m_strWorkBuf.MakeLower();				
				iPos = m_strWorkBuf.Find("注册成功");
				if ( iPos >= 0)
				{
                    ShowMessage("注册成功");
					rgPack.iStatus = 1;
					rgPack.strMemo = "成功";
					strShow.Format("%s,%s,",rgPack.strAcc,rgPack.strPwd);
					WriteLog(strShow,0);
					//保存注册成功的账号进数据库
// 					iRet = WriteToBase(rgPack,g_iRegWebFlag);
// 					if (iRet != 0)
// 					{
// 						WriteLog(strShow,3);
// 					}
					m_iIpSuccNum++;
					if (m_iIpSuccNum >= g_iIpRegNum)
					{
						if (g_iAotuChangeIp)
						{
							ShowMessage("单一IP注册数已达上限,正在切换IP...");
						}
						if (g_iIsChangeMac)
						{
							ShowMessage("单一MAC地址注册数已达上限,正在切换MAC地址...");
						}
						m_iNeedChageIP = 1;
						m_iIpSuccNum = 0;
						m_iIsNeedChangeMac = 1;
					}
				}
				else
				{//注册失败						
					CString strBefore = "<div align=\"center\"><font color='red'>";
					CString strAfter = "</font></div>";
					strFailReason = GetKeyString(m_strWorkBuf,strBefore,strAfter);
					ShowMessage("注册失败:	"+strFailReason);
					rgPack.iStatus = 2;
					rgPack.strMemo = strFailReason;
					if (strFailReason.Find("棋牌迷日注册量已达到当日限额") != -1)
					{
						if (g_iIsChangeMac)
						{
							ShowMessage("单一MAC地址注册数已达上限,正在切换MAC地址...");
						}
						m_iIsNeedChangeMac = 1;
					}
				}
				AddToShowList(rgPack);
				if (g_iDeleteFlag == 1)
				{
					if (strFailReason.Find("验证码输入错误")<0)
					{
						DeleteRegisterPack(rgPack);												
					}
					else
					{
						m_iCodeErrorNum ++;
						if (m_iCodeErrorNum >= g_iCodeErrorNum)
						{
							ShowMessage("验证码错误次数到达上限,跳过此账号的注册");
							DeleteRegisterPack(rgPack);
							m_iCodeErrorNum = 0;
						}
					}
				}
				else
				{
					DeleteRegisterPack(rgPack);
				}
#ifdef DEBUG
				m_iNeedChageIP = 1;
#endif
				if (m_iNeedChageIP == 1)		//换IP
				{
					DeleteObjectExEx();
					return false;
				}
				if (m_iIsNeedChangeMac == 1)		//换MAC地址
				{
					DeleteObjectExEx();
					return false;
				}
			}
			catch (...)
			{
				DeleteObjectExEx();
				return false;
			}			
		}
		return false;  
	}
	catch (...)
	{
	}
	DeleteObjectExEx();
	return false;            
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void C51JobWebPost::ThreadFunc(LPVOID lParam)
{
     C51JobWebPost *pParam = ((C51JobWebPost *)lParam);
     if ( pParam )
     {
        pParam->RegisterFunc();
		pParam->ShowMessage("线程退出");
     }
     
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void C51JobWebPost::RegisterFunc()
{
	m_iNeedChageIP = 0;
	m_iIpSuccNum = 0;
	CString sendStr = "";
	ShowMessage("初始化成功");
	if (g_iAotuChangeIp == 1)
	{
		sendStr.Format("自动换IP,单一IP注册数:%d",g_iIpRegNum);
		ShowMessage(sendStr);
	}	
	if (g_iAotuCheckCode==1)
	{
		ShowMessage("自动识别验证码");
	}
	m_strOldIP = "";
	while (!g_ExitFlag)
	{
		m_hwnd = ::FindWindow("#32770","帐号注册");
		while(!m_hwnd)
		{
			m_hwnd = ::FindWindow("#32770","帐号注册");
			ShowMessage("请打开客户端注册窗口");
			Sleep(1000);
		}
		char str[128] = {0};
		HWND hwnd = NULL;
		CWindowFind find;
		//hwnd = find.MyFindWindow("Internet Explorer_Server","",hwnd);
		hwnd = find.GetChildWindow(m_hwnd,1);
		hwnd = find.GetChildWindow(hwnd,1);
		hwnd = find.GetChildWindow(hwnd,1);
		hwnd = find.GetChildWindow(hwnd,1);
		GetClassName(hwnd,str,sizeof(str));//获取类名 
		if(strcmp(str,"Internet Explorer_Server")==0)
		{
			IHTMLDocument2 *pDoc = GetDocInterface(hwnd);
			GetHtmlCtrl(pDoc);
		}
		Sleep(1000);
		continue;
		if (g_RegisterList.GetSize() <= 0)
		{
			Sleep(500);
			continue;
		}
		
		m_iIpSuccNum = 0;
		m_iNeedChageIP = 1;
		int iErrorCount = 0;
		int iSameIpCount = 0;
		if (!g_iAotuChangeIp)//不需要自动换IP
		{
			//更换MAC地址
			if (m_iIsNeedChangeMac)
			{
				m_iIsNeedChangeMac = ChangeMacAddr();
				if (m_iIsNeedChangeMac == 1)
				{
					ShowMessage("修改MAC地址失败");
				}
				else
				{
					m_iIsNeedChangeMac = 0;
				}
			}
			else
			{
				ShowMessage("已经更换MAC地址");				
			}
		}
		CString strTest = "";
		while(GetGateWayIP(strTest) != 0)
		{
			Sleep(1000);
		}
		ShowMessage("网络连接成功");	
		DeleteObjectExEx();
		while (m_iNeedChageIP && !g_ExitFlag && g_iAotuChangeIp == 1)	//如果需要换IP
		{
			m_iNeedChageIP = 1;
			g_RasMgr.StopRas(sendStr);
			ShowMessage(sendStr);
			//更换MAC地址
			if (m_iIsNeedChangeMac == 1)
			{
				 m_iIsNeedChangeMac = ChangeMacAddr();
				 if (m_iIsNeedChangeMac == 1)
				 {
					 ShowMessage("修改MAC地址失败");
				 }
				 else
				 {
					 m_iIsNeedChangeMac = 0;
				 }
			}
			else
			{
				 ShowMessage("已经更换MAC地址");
			}
			int iRet =  g_RasMgr.DialRas(sendStr);			
			ShowMessage(sendStr);
			if (iRet == 0)		//重新拨号成功，获取新的IP
			{
				if (g_iCheckIpFlag == 2)////POST检测
				{
					int iRet = -1;
					int iErrorNum = 0;
					while (iRet == -1)
					{
						iRet = GetGateWayIP(g_strCurrIP);
						if (iRet == -1)
						{
							iErrorNum++;
							sendStr.Format("获取本机Ip失败 %d 次,重试...",iErrorNum);
							ShowMessage(sendStr);							
							if (iErrorNum >= 3)
							{
								ShowMessage("获取本机Ip失败次数超过限制,跳过此操作...");
								g_strCurrIP = m_strOldIP;
								break;
							}
							Sleep(300);
						}
						else
						{
							ShowMessage("检测到本机Ip: "+g_strCurrIP);
						}
					}
					if (g_strCurrIP == m_strOldIP)
					{
						m_iNeedChageIP = 1;
						iSameIpCount ++;
						if (iSameIpCount < g_iReCheckIpCount)
						{
							sendStr.Format("Ip未更换,重复拨号[%d]...",iSameIpCount);
							ShowMessage(sendStr);
							Sleep(300);
							continue;
						}
						sendStr.Format("重复拨号次数[%d]超过限制仍未更换Ip,跳过此操作...",g_iReCheckIpCount);
						ShowMessage(sendStr);
						iSameIpCount = 0;
					}
					else	//IP更换成功
					{
						m_strOldIP = g_strCurrIP;
					}
				}
				else if (g_iCheckIpFlag == 1)//RAS检测
				{
					int iRet = -1;
					int iErrorNum = 0;
					while (iRet == -1)
					{
						iRet = g_RasMgr.GetGateWayIP(g_strCurrIP);
						if (iRet == -1)
						{
							iErrorNum++;
							sendStr.Format("获取本机Ip失败 %d 次,重试...",iErrorNum);
							ShowMessage(sendStr);
							if (iErrorNum >= 3)
							{
								ShowMessage("获取本机Ip失败次数超过限制,跳过此操作...");
								g_strCurrIP = m_strOldIP;
								break;
							}
							Sleep(300);
						}
						else
						{
							ShowMessage("检测到本机Ip: "+g_strCurrIP);
						}
					}
					if (g_strCurrIP == m_strOldIP)
					{
						m_iNeedChageIP = 1;
						iSameIpCount ++;
						if (iSameIpCount < g_iReCheckIpCount)
						{
							sendStr.Format("Ip未更换,重复拨号[%d]...",iSameIpCount);
							ShowMessage(sendStr);
							Sleep(500);
							continue;
						}
						sendStr.Format("重复拨号次数[%d]超过限制仍未更换Ip,跳过此操作...",g_iReCheckIpCount);
						ShowMessage(sendStr);
						iSameIpCount = 0;
					}
					else	//IP更换成功
					{
						m_strOldIP = g_strCurrIP;
					}
				}
				else{}//不检测
				iSameIpCount = 0;
				m_iNeedChageIP = 0;
				iErrorCount = 0;
				break;
			}
			iErrorCount ++;
			if (iErrorCount>=10)
			{
				ShowMessage("拨号连接失败次数过多,请确认您是使用ADSL拨号并且配置正确");
				return;
			}
			Sleep(500);
		}
		m_iIsNeedChangeMac = 0;
		m_iNeedChageIP = 0;
		if (!g_ExitFlag)
		{
			ReOpenRegWindow();
			UserRegister();
		}		
		if (g_ExitFlag)
		{
			return;
		}
	}   
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CString C51JobWebPost::GetKeyString(CString &srcStr,CString &strBefore,CString &strAfter)
{
        int iPosBefore = 0;
        int iPosAfter = 0;
        iPosBefore = srcStr.Find(strBefore) + strBefore.GetLength();
        iPosAfter = srcStr.Find(strAfter,iPosBefore);
        if (iPosBefore < 0 || iPosAfter < 0 || iPosAfter - iPosBefore < 0 )
        {
                return "";
        }
        else
        {
                return srcStr.Mid(iPosBefore,iPosAfter - iPosBefore);
        }
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void C51JobWebPost::BeginRegister()
{
	AfxBeginThread((AFX_THREADPROC)ThreadFunc,(LPVOID)this);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void C51JobWebPost::ShowMessage(CString strMsg)
{
	CUserMessagePump *pMsgPump = NULL;
	pMsgPump = CUserMessagePump::GetInstance();
	if (pMsgPump)
	{
		pMsgPump->AddToMessagePump(USERWINDOWMSG_1,strMsg);
	}
	WriteLog(strMsg,5);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void C51JobWebPost::ShowMessageEx(CString strMsg)
{
	CUserMessagePump *pMsgPump = NULL;
	pMsgPump = CUserMessagePump::GetInstance();
	if (pMsgPump)
	{
		pMsgPump->AddToMessagePump(USERWINDOWMSG_1,strMsg);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString C51JobWebPost::NativeToAscii(CString strSrc)
{
	return m_Conver.NativeToAscii(strSrc);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int C51JobWebPost::GetGateWayIP(CString &strIP)
{
	DeleteObjectExEx();
	CString strError = "";
	CString strUrl = "www.ip138.com";
	CString objectlink = "/ip2city.asp";
	int iRet = RequestGetPageExExEx(strUrl,objectlink,m_strHeader,
		strError);//"148.233.239.24",3128,"","",0);
	if ( iRet != 0 )
	{
		TRACE("获取页面出错");
		ShowMessage("获取页面失败");
		m_nErrorTime ++;
		DeleteObjectExEx();
		return -1;
	}
	m_strWorkBuf.MakeLower();
	if (m_strWorkBuf.Find("您的ip地址是：[") !=-1)
	{
		CString strBefore = "您的ip地址是：[";
		CString strAfter = "]";
		strIP = GetKeyString(m_strWorkBuf,strBefore,strAfter);
		return 0;
	}
	return -1;
 }
// 	CInternetSession session;
// 	CString strTemp = "";
// 	char temp[1001] = {0};
// 	CStdioFile *pFile = session.OpenURL("http://www.ip138.com/ip2city.asp");
// 	if (pFile != NULL)
// 	{
// 		long len = pFile->GetLength();
// 		if (len > 1000)
// 		{
// 			len = 1000;
// 		}		
// 		pFile->SeekToBegin();
// 		pFile->Read(temp,len);
// 		temp[len] = '\0';
// 		strTemp.Format("%s",temp);
// 		strTemp.MakeLower();
// 		if (strTemp.Find("您的ip地址是：[") !=-1)
// 		{
// 			CString strBefore = "您的ip地址是：[";
// 			CString strAfter = "]";
// 			strIP = GetKeyString(strTemp,strBefore,strAfter);
// 			pFile->Close();
// 			delete pFile;
// 			pFile = NULL;
// 			return 0;
// 		}
// 		if (pFile == NULL)
// 		{
// 			pFile->Close();
// 			delete pFile;
// 			pFile = NULL;
// 		}
// 		
// 	}
// 	return -1;
/*}*/


/************************************************************************/
/*                                                                      */
/************************************************************************/
void C51JobWebPost::SaveToExcel(RegisterPack rgPack)
{
	CAdoConnection adoConn;
	CString strError;
	//CreatExcelDb("c:\\aaa.xls","hello",strError);
// 	CAdoConnection adoConn;
// 	CAdoRecordSet adoRst;
// 	try
// 	{
// 		if (!adoConn.IsOpen())
// 		{
// 			if(adoConn.ConnectExcel("c:\\aa.xml"))
// 			{
// 				adoRst.SetAdoConnection(&adoConn);
// 				adoRst.SetCursorLocation(adUseClient);
// 			}
// 			else
// 			{
// 				//创建它
// 				
// 				ShowMessage("打开Excel文件失败"+adoConn.GetLastErrorText());
// 				return;
// 			}
// 		}
// 		CString strSQL
}

/************************************************************************/
/*       创建Excel数据库                                                */
/************************************************************************/
BOOL C51JobWebPost::CreatExcelDb(CString dbpath,CString tableName,CString &strError)
{
	if (_access(dbpath,0)==0)	//存在
	{
		strError = "数据库已存在";
		return TRUE;
	}
	CDatabase database;
	CString sDriver;
	CString sExcelFile; 
	CString sSql;
	//检索是否安装有Excel驱动 "Microsoft Excel Driver (*.xls)" 
	sDriver = GetExcelDriver();
	if (sDriver.IsEmpty())
	{
		// 没有发现Excel驱动
		strError = "请先安装Excel软件才能使用导出功能";
		return FALSE;
	}
	// 创建进行存取的字符串
	sSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, dbpath);
	// 创建数据库 (既Excel表格文件)
	if( database.OpenEx(sSql,CDatabase::noOdbcDialog) )
	{
		CString strTemp;
		strTemp.Format("账号 TEXT,密码 TEXT,推荐人 TEXT,邮箱 TEXT,网址 TEXT,时间 TEXT");
		// 创建表结构			
		sSql = "CREATE TABLE " + tableName + " ( " + strTemp +  " ) ";
		database.ExecuteSQL(sSql);
		return TRUE;
	}
	else
	{
		strError = "创建EXCEL数据库失败";
		return FALSE;
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CString C51JobWebPost::GetExcelDriver()
{
	char szBuf[2001];
	WORD cbBufMax = 2000;
	WORD cbBufOut;
	char *pszBuf = szBuf;
	CString sDriver;

	// 获取已安装驱动的名称(涵数在odbcinst.h里)
	if (!SQLGetInstalledDrivers(szBuf, cbBufMax, &cbBufOut))
		return "";

	// 检索已安装的驱动是否有Excel...
	do
	{
		if (strstr(pszBuf, "Excel") != 0)
		{
			//发现 !
			sDriver = CString(pszBuf);
			break;
		}
		pszBuf = strchr(pszBuf, '\0') + 1;
	}
	while (pszBuf[1] != '\0');

	return sDriver;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int C51JobWebPost::WriteToBase(RegisterPack rgPack,int iFlag)
{
	int iRet = -1;
	CAdoConnection adoConn;
	CAdoRecordSet adoRst;
	try
	{
		if ( !adoConn.IsOpen() )		//如果未连接上服务器
		{
			CString strAppPath = "";
			CUserFile::GetInstance()->GetAppPath(strAppPath);
			if ( !adoConn.ConnectAccess(strAppPath+"7pRegister.mdb",""))
			{
				ShowMessage("连接数据库失败");
				return -1; 
			}
			adoRst.SetAdoConnection(&adoConn);
			adoRst.SetCursorLocation(adUseClient);
		}
		CString strSQL;		
		strSQL.Format("insert into [Users] ([userid],[userpwd],flag)values('%s','%s',%d,)",rgPack.strAcc,rgPack.strPwd,iFlag);
		if( !adoRst.Open(strSQL,adCmdText))
		{
			iRet = -1;
			ShowMessage("保存账号信息失败");
		}
		else
		{
			iRet = 0;
		}
		adoRst.Close();
		adoConn.Close();
		return iRet;
	}
	catch (...)
	{
		adoRst.Close();
		adoConn.Close();
		ShowMessage("保存账号信息异常");
	}
	return -1;
}

IHTMLDocument2* C51JobWebPost::GetDocInterface(HWND hWnd) 
{
	// 我们需要显示地装载OLEACC.DLL,这样我们才知道有没有安装MSAA
	CoInitialize(NULL);
	HINSTANCE hInst = ::LoadLibrary( _T("OLEACC.DLL") );
	IHTMLDocument2* pDoc2=NULL;
	if ( hInst != NULL ){
		if ( hWnd != NULL ){
			CComPtr<IHTMLDocument> spDoc=NULL;
			LRESULT lRes;

			UINT nMsg = ::RegisterWindowMessage( _T("WM_HTML_GETOBJECT") );
			::SendMessageTimeout( hWnd, nMsg, 0L, 0L, SMTO_ABORTIFHUNG, 1000, (DWORD*)&lRes );

			LPFNOBJECTFROMLRESULT pfObjectFromLresult = (LPFNOBJECTFROMLRESULT)::GetProcAddress( hInst, ("ObjectFromLresult") );
			if ( pfObjectFromLresult != NULL ){
				HRESULT hr;
				hr=pfObjectFromLresult(lRes,IID_IHTMLDocument,0,(void**)&spDoc);
				if ( SUCCEEDED(hr) ){
					CComPtr<IDispatch> spDisp;
					CComQIPtr<IHTMLWindow2> spWin;
					spDoc->get_Script( &spDisp );
					spWin = spDisp;
					spWin->get_document( &pDoc2 );
				}
			}
		}
		::FreeLibrary(hInst);
	} 
	else{//如果没有安装MSAA
		AfxMessageBox(_T("请您安装Microsoft Active Accessibility"));
	}
	//CoUninitialize();
	return pDoc2;
}
/************************************************************************/
/* 得到MAC地址等信息                                                    */
/************************************************************************/
int C51JobWebPost::GetHtmlCtrl(IHTMLDocument2 *pDoc2)
{
	if(pDoc2==NULL)
	{
		return 0;
	}
	IHTMLElementCollection   *pColl=NULL;
	HRESULT hr=pDoc2->get_all(&pColl);
	if(SUCCEEDED(hr)   &&   pColl!=NULL)   
	{   
		long   lcount   =   0;   
		pColl->get_length(&lcount);
		CString strName = "name";
		BSTR bstrText = strName.AllocSysString();
		//CString strValue = "onfocus";
		//BSTR bstrValue = strName.AllocSysString();
		int iFindCount = 0;
		for(int i=0;i<lcount;i++)   
		{
			if (iFindCount >= 8)
			{
				//break;
			}
			CComVariant   index;   
			index.vt=VT_I4;   
			index.intVal=i;   
			CComPtr<IDispatch>   disp;   
			pColl->item(index,index,&disp);   
			if(disp==NULL)   
				hr=E_FAIL;   
			else   
			{
				CComPtr<IHTMLElement>   pElement;
				//CComPtr<IHTMLImgElement> pImg;
				hr=disp->QueryInterface(IID_IHTMLElement,
					(void**)&pElement);
				if(SUCCEEDED(hr))   
				{   
					BSTR   bstrID;					
					pElement->get_id(&bstrID);
					CString strID(bstrID);
					VARIANT  varValue;
					pElement->getAttribute(bstrText,0,&varValue);
					strID.MakeLower();
					//pElement->
// 					CString strValue = "";
// 					if (varValue.vt == VT_BSTR)
// 					{
// 						strValue.Format ("%s",(const char*)(_bstr_t)(_variant_t)varValue);
// 					}
// 					strValue.MakeLower();
// 					ShowMessage(strValue);
// 					if (strID == "imgReg")
// 					{
// 						ShowMessage("找到注册按钮");
// 						//m_pRegBtn = pElement;
// 						iFindCount ++;
// 						//pElement->click();
// 					}
// 					else if (strID == "introducer")
// 					{
// 						ShowMessage("找到推荐人输入框");
// 						//pElement->setAttribute(bstrValue,"aa",0);
// 						//pElement->
// 						//pUserIntroman = (CComPtr<IHTMLInputTextElement>)pElement;
// 						iFindCount ++;
// 					}
					if (strID == "machineserial")		//email name repassword password
					{
						//ShowMessage("找到MAC地址隐藏域");
						m_strMac = GetElementValue(pElement);
						ShowMessage(m_strMac);
						//iFindCount ++;
					}
// 					else if (strID == "username")		
// 					{
// 						ShowMessage("找到USERNAME");
// 						iFindCount ++;
// 					}
// 					else if (strID == "userpwd")
// 					{
// 						ShowMessage("找到密码");
// 						iFindCount ++;
// 					}
// 					else if (strID == "repassword" || strValue == "repassword")
// 					{
// 						ShowMessage("找到密码确认");
// 						iFindCount ++;
// 					}
// 					else if (strID == "checkcode" || strValue == "checkcode")
// 					{
// 						ShowMessage("找到验证码");
// 						iFindCount ++;
// 					}
					else if (strID == "transactionid")
					{
						//找到TRANS
						//ShowMessage("找到TRANS");
						m_strTrans = GetElementValue(pElement);
						ShowMessage(m_strTrans);
					}
					else if (strID == "verify")
					{
						//找到VERIFY
						//ShowMessage("找到verify");
						m_strVerify = GetElementValue(pElement);
						ShowMessage(m_strVerify);
						break;
					}
				}
			}
		}
		::SysFreeString(bstrText);
		pColl->Release();   
	}   
	pDoc2->Release();
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString C51JobWebPost::GetElementValue(CComPtr<IHTMLElement>   pElement)
{
	CString strRet = "";
	CString strName = "value";
	BSTR bstrText = strName.AllocSysString();
	VARIANT  varValue;
	pElement->getAttribute(bstrText,0,&varValue);
	if (varValue.vt == VT_BSTR)
	{
		strRet.Format ("%s",(const char*)(_bstr_t)(_variant_t)varValue);
	}
	strRet.MakeLower();
	::SysFreeString(bstrText);
	return strRet;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int C51JobWebPost::ChangeMacAddr()
{
	//目标窗口已经关闭，更新MAC地址
	//SetIpNetEntry()	
	CWindowFind find;
	HWND hwnd = NULL;
	int iCount = 0;
	while (!hwnd && iCount < 5)
	{
        hwnd =::FindWindow("TNMForm","Mac MakeUp ver. 1.95d (c) 2003-2006 H&C Works");
		Sleep(50);
		if (!hwnd)
		{
			ShowMessage("MAC地址修改器未启动！");
		}
		iCount++;
	}
	if (!hwnd)
	{
		ShowMessage("MAC地址修改器未启动！");
		return 1;
	}
	srand(time(NULL));
    CString strMac = MacAddrRand();
	ShowMessage(strMac);
	hwnd = find.GetChildWindow(hwnd,1);
	//HWND hwndRand =  find.GetChildWindow(hwnd,1);
	HWND hwndModify =  find.GetChildWindow(hwnd,3);
	hwnd = find.GetChildWindow(hwnd,5);
	HWND hwndMac = find.GetChildWindow(hwnd,1);
	find.SetEditText(hwndMac,strMac);
	Sleep(50);
	find.PostBtnClick(hwndModify);
	Sleep(7500);
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString C51JobWebPost::MacAddrRand()
{
	CString strRet = "";
	CString strTemp = "";
	
	for (int i=0;i<10;i++)
	{
		int iGen= rand() % 16;
		if (iGen <= 9)
		{
			strTemp.Format("%d",iGen);
		}
		else
		{
			strTemp.Format("%c",iGen-10+65);
		}
		strRet += strTemp;
	}
	strRet = "00" + strRet;
	return strRet;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int C51JobWebPost::ReOpenRegWindow()
{
	//close window
	//重新打开注册窗口
	CWindowFind find;
	m_hwnd = NULL;
	m_hwnd = ::FindWindow("#32770","帐号注册");
	while(m_hwnd)
	{
		::SendMessage(m_hwnd,WM_CLOSE,0,0);
		Sleep(50);
		m_hwnd = ::FindWindow("#32770","帐号注册");
		if (m_hwnd == NULL)
		{
			break;
		}		
	}
	//关闭后
	HWND hwnd = find.MyFindWindow("Static","游戏帐号：");
	if (hwnd)	//登录框的子窗口
	{
		hwnd = ::GetParent(hwnd);
		if (hwnd)
		{
			//hwnd = pParantWnd->GetSafeHwnd();
			//找账号框
			HWND hwndChind = NULL;
			//注册按钮
			hwndChind = find.GetChildWindow(hwnd,2);
			Sleep(40);
			find.PostBtnClick(hwndChind);
		}
	}
	Sleep(100);
	m_hwnd = NULL;
	m_hwnd = ::FindWindow("#32770","帐号注册");
	//判断程序是否已经启动
	return 0;
}