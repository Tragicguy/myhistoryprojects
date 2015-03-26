#include "StdAfx.h"
#include ".\51jobwebpost.h"
#include "UserMessagePump.h"
#include <io.h>
#include <direct.h>
#include "UserFile.h"
#include "datatype.h"
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
	m_strServerURL = "www.wanlitong.com";
	m_TimeInterval = 3;
	//m_strHeader = "Content-Type: application/x-www-form-urlencoded";
	//m_strHeader += "Accept:image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, application/x-silverlight, */*";
	m_strHeader += "Accept:*/*";
	m_strHeader += "\r\nAccept-Language:zh-cn";
	//m_strHeader += "\r\nAccept-Encoding:gzip, deflate";//
	//m_strHeader += "\r\nCache-Control:no-cache";
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
		CString strError = "";
		CString sendStr = "";
		ShowMessage("正在获取登录页面...");
		CString objectlink = "/eloyalty_chs/start.swe?SWECmd=Login&SWECM=S&SWEHo=www.wanlitong.com";
		CString strHeaders = m_strHeader;
	//	CString strRefferLink = "https://www.wanlitong.com/index.jsp";
		//strHeaders += "\r\nReferer:"+strRefferLink;
		//int iRet = RequestGetPage(m_Session,m_pConnection,m_strServerURL,objectlink,strHeaders);
		int iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,
			strError);//"148.233.239.24",3128,"","",0);
		if ( iRet != 0 )
		{
			TRACE("获取页面出错");
			ShowMessage("获取登录页面失败");
			m_nErrorTime ++;
			DeleteObjectExEx();
			return false;
		}
		//m_strWorkBuf.MakeLower();
		CFile file;
		file.Open("C:\\testss.txt",CFile::modeCreate | CFile::modeWrite);
		file.Write(m_strWorkBuf,m_strWorkBuf.GetLength());
		file.Close();
		//AfxMessageBox(m_strWorkBuf);
		//检查是否打开简历管理页
		CString strBefore1 = "<img src='/eloyalty_chs/start.swe?";
		CString strBefore2 = "' alt='图像' Id=ImageFile>";
		CString strCheckCodeUrl = "";
		int iPos = m_strWorkBuf.Find("会员登录");
// 		CFile file;
// 		file.Open("c:\\test.txt",CFile::modeCreate|CFile::modeWrite);
// 		file.Write(m_strWorkBuf,m_strWorkBuf.GetLength());
// 		file.Close();
		CString strUserIDStr = "";		//用户名输入框ID
		CString strPwdIDStr = "";		//密码框ID
		CString strCodeIDStr = "";		//验证码输入框ID
		CString strLoginIDStr = "";		//登录按钮ID
		CString strFormID = "";			//SWEForm2 //FormID
		CString strSWETSValue = "";		// SWETS   value
		CString strUserID = "junbrave";
		CString strPwd = "850518";
		if (iPos >= 0 && m_strWorkBuf.Find("用户名") >= 0)
		{
			ShowMessage("成功获取登录页,正在获取验证码...");
			strCheckCodeUrl = GetKeyString(m_strWorkBuf,strBefore1,strBefore2);
			AfxMessageBox(strCheckCodeUrl);
			//获取特征值
			//用户名输入框
			strBefore1 = "ctrlLookupMap[\"Login Applet.Login Name\"] = \"";
			strBefore2 = "\";";
			strUserIDStr = GetKeyString(m_strWorkBuf,strBefore1,strBefore2);
			//密码框
			strBefore1 = "ctrlLookupMap[\"Login Applet.Password\"] = \"";
			strBefore2 = "\";";
			strPwdIDStr = GetKeyString(m_strWorkBuf,strBefore1,strBefore2);
			//验证码输入框
			strBefore1 = "ctrlLookupMap[\"Login Applet.VerifyCode\"] = \"";
			strBefore2 = "\";";
			strCodeIDStr = GetKeyString(m_strWorkBuf,strBefore1,strBefore2);
			//SWETS   value
			strBefore1 = "if (typeof(Top().SWEHtmlPopupName) == 'undefined' || Top().SWEHtmlPopupName == null) Top().SWEHtmlPopupName = '_swe";
			strBefore2 = "';";
			strSWETSValue = GetKeyString(m_strWorkBuf,strBefore1,strBefore2);
			//登录按钮
			strBefore1 = "onclick='Edit__0__Control__Login__onclick(null, \"";
			strBefore2 = "\")";
			strLoginIDStr = GetKeyString(m_strWorkBuf,strBefore1,strBefore2);
			//
			strFormID = "SWEForm2_0";
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
					objectlink.Format("/eloyalty_chs/start.swe?%s",strCheckCodeUrl);
					//m_strHeader += "Accept:*/*";
					//strHeaders += "\r\nAccept-Language:zh-cn";
					strHeaders = "Accept:image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, application/x-silverlight, */*";
					strHeaders += "\r\nAccept-Encoding:gzip, deflate";
					//m_strHeader += "\r\nAccept-Encoding:gzip, deflate";//
					//m_strHeader += "\r\nCache-Control:no-cache";
					strHeaders += "\r\nUser-Agent:Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 1.1.4322; .NET CLR 2.0.50727)";
					strHeaders += "\r\nConnection:Keep-Alive";
					strHeaders = "\r\nReferer:https://www.wanlitong.com/eloyalty_chs/start.swe?SWECmd=GotoView&amp;SWEView=Login+View";
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
				objectlink = "/eloyalty_chs/start.swe";
				strHeaders = m_strHeader + "\r\nReferer:https://www.wanlitong.com/eloyalty_chs/start.swe";
				//strHeaders += "\r\nx-requested-with: XMLHttpRequest";
				strHeaders += "\r\nContent-Type: application/x-www-form-urlencoded";
				CString strTime = CTime::GetCurrentTime().Format("%m%d%H%M%S");
				strCheckCode = NativeToAscii(strCheckCode);
				postStr.Format("%s=%s&%s=%s&%s=%s&SWEFo=%s&SWEField=%s&SWENeedContext=true&SWENoHttpRedir=true&W=t&SWECmd=InvokeMethod&SWEMethod=AppletLogin&SWERowIds=&SWESP=false&SWEVI=&SWESPNR=&SWEPOC=&SWESPNH=&SWEH=&SWETargetView=&SWEDIC=false&SWEReqRowId=0&SWEView=Login+View&SWEC=6&SWERowId=VRId-0&SWETVI=&SWEW=&SWEBID=-1&SWEM=&SWESPa=&SWETS=%s&SWEContainer=&SWEWN=&SWEKeepContext=0&SWEApplet=Login+Applet&SWETA=",
								strUserIDStr,
								strUserID,
								strPwdIDStr,
								strPwd,
								strCodeIDStr,
								strCheckCode,
								strLoginIDStr,
								strFormID,
								strSWETSValue);
				iRet = RequestPostPageExExEx(m_strServerURL,objectlink,strHeaders,postStr,strError,0);//
				CFile file;
				file.Open("c:\\test22.txt",CFile::modeCreate|CFile::modeWrite);
				file.Write(m_strWorkBuf,m_strWorkBuf.GetLength());
				file.Close();
				//AfxMessageBox(m_strWorkBuf);
				if ( iRet != 0 )
				{
					TRACE("POST出错");
					m_nErrorTime ++;
					DeleteObjectExEx();
					return false;
				}
				m_strWorkBuf.MakeLower();
				if (m_strWorkBuf.Find("您目前可用积分") != -1)
				{
					ShowMessage("登录成功");
					rgPack.iStatus = 1;
					rgPack.strMemo = "成功";
					strShow.Format("%s,%s,%s,%s,%s",rgPack.strAcc,rgPack.strPwd,rgPack.strNickName,rgPack.strIntroMan,rgPack.strMail);
					WriteLog(strShow,0);
					m_iIpSuccNum++;
					if (m_iIpSuccNum >= g_iIpRegNum)
					{
						if (g_iAotuChangeIp)
						{
							ShowMessage("单一IP注册数已达上限,正在切换IP...");
						}						
						m_iNeedChageIP = 1;
						m_iIpSuccNum = 0;					
					}
				}
				else
				{//登录失败					
					strFailReason = m_strWorkBuf;
					ShowMessage("登录失败:	"+strFailReason);
					rgPack.iStatus = 2;
					rgPack.strMemo = strFailReason;
				}
				AddToShowList(rgPack);
				if (g_iDeleteFlag == 1)
				{
					if (strFailReason.Find("请输入正确的验证码")<0)
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
		if (g_RegisterList.GetSize() <= 0)
		{
			Sleep(500);
			continue;
		}
		m_iIpSuccNum = 0;
		m_iNeedChageIP = 1;
		int iErrorCount = 0;
		int iSameIpCount = 0;
		while (m_iNeedChageIP && !g_ExitFlag && g_iAotuChangeIp == 1)	//如果需要换IP
		{
			m_iNeedChageIP = 1;
			g_RasMgr.StopRas(sendStr);
			ShowMessage(sendStr);
			Sleep(300);
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
		m_iNeedChageIP = 0;
		if (!g_ExitFlag)
		{
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
	CInternetSession session;
	CString strTemp = "";
	char temp[1001] = {0};
	CStdioFile *pFile = session.OpenURL("http://www.ip138.com/ip2city.asp");
	if (pFile != NULL)
	{
		long len = pFile->GetLength();
		if (len > 1000)
		{
			len = 1000;
		}		
		pFile->SeekToBegin();
		pFile->Read(temp,len);
		temp[len] = '\0';
		strTemp.Format("%s",temp);
		strTemp.MakeLower();
		if (strTemp.Find("您的ip地址是：[") !=-1)
		{
			CString strBefore = "您的ip地址是：[";
			CString strAfter = "]";
			strIP = GetKeyString(strTemp,strBefore,strAfter);
			pFile->Close();
			delete pFile;
			pFile = NULL;
			return 0;
		}
	}
	return -1;
}