#include "StdAfx.h"
#include ".\51jobwebpost.h"
#include "UserMessagePump.h"
#include <io.h>
#include <direct.h>
#include "UserFile.h"

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
	m_strServerURL = "www.sdo.com";
	m_TimeInterval = 3;
	m_strHeader = "Content-Type: application/x-www-form-urlencoded";
	m_strHeader += "\r\nAccept:image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, application/x-silverlight, */*";
	//m_strHeader += "\r\nAccept:*/*";
	m_strHeader += "\r\nAccept-Language:zh-cn";
	m_strHeader += "\r\nAccept-Encoding:gzip, deflate";//
	//m_strHeader += "\r\nCache-Control:no-cache";
	m_strHeader += "\r\nUser-Agent:Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; Foxy/1; Foxy/1; .NET CLR 1.1.4322; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022)";
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
bool C51JobWebPost::UserRegister(RegisterPack &rgPack)
{
	CString g_strUserId = rgPack.strAcc;			//密码
	CString g_strUserpwd = rgPack.strPwd;			//账号
	CString g_strPhone = rgPack.strPhone;		//认证手机
	CString g_strReCommendMobile = rgPack.strIntroMan;	//推荐人手机
	rgPack.strMemo = "[开始登录]";
	try
	{
		CString strError = "";
		CString sendStr = "";
		CString strOjbStrings = "";
		ShowMessage("正在获取登录页面...");
		CString objectlink = "/loginPT.asp";
		CString strHeaders = m_strHeader;
		CString postStr = "";
		CString strShow = "";
		int iRet = -1;
		m_strServerURL = "www.sdo.com";
		m_iPostFlag = 0;
 		iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,
			strError);//"148.233.239.24",3128,"","",0);
		CString strCookies = m_strSetCookie;
		if ( iRet != 0 )
		{
			TRACE("获取页面出错");
			ShowMessage("获取登录页面失败");
			rgPack.iStatus = 0;
			DeleteObjectExEx();
			return false;
		}		
		int iPos = m_strWorkBuf.Find("location.href = \"http://www.sdo.com");
		if (iPos < 0)
		{
			ShowMessage("尝试获取登录页失败!");
			rgPack.iStatus = 0;
			return false;
		}
		CString strBefore11 = "location.href = \"http://www.sdo.com";
		CString strAfter11 = "\";";
		objectlink = GetKeyString(m_strWorkBuf,strBefore11,strAfter11);
		iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError,0);
		strCookies = m_strSetCookie;
		if (iRet != 0)
		{
			ShowMessage("获取参数信息失败");
			m_nErrorTime ++;
			DeleteObjectExEx();
			rgPack.iStatus = 0;
			return false;
		}
		CString strLt = "";
		CString strChallenge = "";
		//CString strError = "";
		strBefore11 = "<input type=\"hidden\" name=\"lt\" value=\"";
		strAfter11 = "\" />";
		strLt = GetKeyString(m_strWorkBuf,strBefore11,strAfter11);
		strBefore11 = "name=\"challenge\" value=\"";
		strAfter11 = "\" />";
		strChallenge = GetKeyString(m_strWorkBuf,strBefore11,strAfter11);
		if (strChallenge == "" || strLt == "")
		{
			ShowMessage("尝试获取登录页失败!");
			rgPack.iStatus = 0;
			DeleteObjectExEx();
			return false;
		}
		CString strTemp = "";
		strTemp = "http://"+m_strServerURL + objectlink;
		strHeaders += "\r\nReferer:"+strTemp;
		strLt.Replace(":","%3A");
		postStr.Format("lt=%s&service=&challenge=%s&cardno=&ekeytype=&request_QueryString=&ecardpos=&err=8",strLt,strChallenge);
		objectlink = "/loginPT.asp";
		iRet = RequestPostPageExExEx(m_strServerURL,objectlink,strHeaders,postStr,strError,0);
		strCookies = m_strSetCookie;
		strBefore11 = "action=\"https://cas.sdo.com/CAS/Login.Validate.Account?";
		strAfter11 = "\">";
		objectlink = GetKeyString(m_strWorkBuf,strBefore11,strAfter11);
		strBefore11 = "name=\"lt\" value=\"";
		strAfter11 = "\" />";
		URL_Encode(objectlink);
		objectlink = "/CAS/Login.Validate.Account?service=http://www.sdo.com/center/index.asp?cas=1";
		strLt = GetKeyString(m_strWorkBuf,strBefore11,strAfter11);
		//strLt.Replace(":","%3A");
		postStr.Format("warn=false&lt=%s&_eventId=submit&idtype=0&gamearea=0&gametype=0&username=%s&password=%s&ekey=&challenge=",strLt,g_strUserId
			,g_strUserpwd);
		ShowMessage("获取登录页成功,正在登录...");
		DeleteObjectExEx();
		strHeaders = m_strHeader;
		strHeaders += "\r\nReferer:http://www.sdo.com/loginPT.asp";
		m_strServerURL = "cas.sdo.com";
		SetHttpsType(1);		
		iRet = RequestPostPageExExEx(m_strServerURL,objectlink,strHeaders,postStr,strError,0);
		SetHttpsType(0);
		strHeaders = m_strHeader;
		strBefore11 = "location.href	=	\"http://www.sdo.com";
		strAfter11 = "\";";
		objectlink = GetKeyString(m_strWorkBuf,strBefore11,strAfter11);
		m_strServerURL = "www.sdo.com";
		DeleteObjectExEx();
		iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
		objectlink = "/Partner/MobileDeposit.aspx";
		if (m_strWorkBuf.Find("我的个人信息") > 0)
		{
			ShowMessage("登录成功,正在跳转到积分页...");
		}
		else
		{
			ShowMessage("登录失败,跳过此账号");
			rgPack.strMemo += ",[登录失败,跳过此账号]";
			rgPack.iStatus = 2;
			return false;
		}
		DeleteObjectExEx();
		m_strServerURL = "jf.sdo.com";
		strHeaders = m_strHeader;
		strHeaders += "\r\nReferer:http://jf.sdo.com/";		
		objectlink = "/LoginFrame.aspx";
		iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
		strBefore11 = "var path    =   \"http://jf.sdo.com";
		strAfter11 = "\";";
		objectlink = GetKeyString(m_strWorkBuf,strBefore11,strAfter11);
		iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
		if (m_strWorkBuf.Find("您的积分余额") > 0)
		{
			ShowMessage("成功跳转到积分页,正在加载手机认证页...");
		}
		objectlink = "/Partner/MobileDeposit.aspx";
    	iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
		CString strViewState = "";
		CString strValidDate = "";
		if (m_strWorkBuf.Find("您的盛大通行证账号") > 0)
		{
			ShowMessage("加载手机认证页成功，开始进行手机验证");
		}
		else if (m_strWorkBuf.Find("您已进行过手机验证") > 0)
		{
			ShowMessage("已经通过手机认证，跳转到资料修改页");
			rgPack.strMemo += ",[已经通过手机认证，跳转到资料修改页]";
			goto GETUSERDATAPAGE;
		}
		else
		{
			ShowMessage("加载手机认证页失败，跳转到资料修改页");
			rgPack.strMemo += ",[加载手机认证页失败]";
			goto GETUSERDATAPAGE;
		}		
		strBefore11 = "<input type=\"hidden\" name=\"__VIEWSTATE\" id=\"__VIEWSTATE\" value=\"";
		strAfter11 = "\" />";
		strViewState = GetKeyString(m_strWorkBuf,strBefore11,strAfter11);
		strBefore11 = "name=\"__EVENTVALIDATION\" id=\"__EVENTVALIDATION\" value=\"";
		strAfter11 = "\" />";
		strValidDate = GetKeyString(m_strWorkBuf,strBefore11,strAfter11);
		objectlink = "/csharpwrapper/Score.Web.AjaxMethod,JF.sdo.com.ashx?_method=SendMobileSms&_session=rw";
		strHeaders = m_strHeader;
		strHeaders += "\r\nReferer:http://jf.sdo.com/Partner/MobileDeposit.aspx";		
		postStr.Format("mobile=%s\nuserId=%s\nrecmobile=\n",g_strPhone,g_strUserId);
		m_iPostFlag = 1;
		iRet = RequestPostPageExExEx(m_strServerURL,objectlink,strHeaders,postStr,strError,0);
		m_iPostFlag = 0;
		if (m_strWorkBuf.Trim() == "0")
		{
			ShowMessage("验证信息已发送,请查收并尽快输入,跳过手机验证请输入字符'R'或点击按钮");
			g_strCheckCode = "";
			while(!g_ExitFlag)
			{
				if (g_strCheckCode != "")
				{
					break;
				}
				Sleep(100);
			}
			if (g_ExitFlag)		//控制退出
			{
				DeleteObjectExEx();
				rgPack.iStatus = 0;
				return false;
			}
			if (g_strCheckCode.MakeLower() == "r")
			{
				ShowMessage("手动跳过手机验证");
				rgPack.strMemo += ",[手动跳过手机验证]";
			}
			else
			{
				ShowMessage("手机验证码已经输入,正在验证...");
				//准备POST的数据
				URL_Encode(strViewState);
				URL_Encode(strValidDate);
				CString strPostPre = "__EVENTTARGET=&__EVENTARGUMENT=&__VIEWSTATE=%s&__EVENTVALIDATION=%s&_ctl0:ContentPlaceHolder1:mobileDeposit:TB_UserId=%s&_ctl0:ContentPlaceHolder1:mobileDeposit:TB_Mobile=%s&_ctl0:ContentPlaceHolder1:mobileDeposit:TB_SmsCode=%s&_ctl0:ContentPlaceHolder1:mobileDeposit:TB_ReCommendMobile=%s&_ctl0:ContentPlaceHolder1:mobileDeposit:btnSubmit.x=23&_ctl0:ContentPlaceHolder1:mobileDeposit:btnSubmit.y=9";
				objectlink = "/Partner/MobileDeposit.aspx";				
				postStr.Format(strPostPre,strViewState,strValidDate,g_strUserId,g_strPhone,g_strCheckCode,g_strReCommendMobile);
				postStr.Replace(":","%3A");
				//URL_Encode(postStr);
				strHeaders = m_strHeader;
				strHeaders += "\r\nReferer:http://jf.sdo.com/Partner/MobileDeposit.aspx";
				iRet = RequestPostPageExExEx(m_strServerURL,objectlink,strHeaders,postStr,strError,0);				
				if(iRet < 0)
				{
					ShowMessage("发送手机验证信息出错，跳过手机验证");	
					rgPack.strMemo += ",[发送手机验证信息出错，跳过手机验证]";
				}
				else
				{
					if (m_strWorkBuf.Find("验证手机成功") > 0)
					{
						ShowMessage("验证手机成功");
						rgPack.strMemo += ",[验证手机成功]";
					}
					else
					{
						strBefore11 = "id=\"_ctl0_ContentPlaceHolder1_mobileDeposit_lblCheckMobileMsg\" style=\"color:Red;\">";
						strAfter11 = "</span></td>";
						CString strErrorInfo = "";
						strErrorInfo.Format("[%s]",GetKeyString(m_strWorkBuf,strBefore11,strAfter11));
						ShowMessage(strErrorInfo);
						ShowMessage("跳过手机验证,进行资料填写");
						rgPack.strMemo += ",[" + strErrorInfo+"]";
					}
				}
			}
		}
		else
		{
			if (m_strWorkBuf.GetLength() > 10)
			{
				m_strWorkBuf = m_strWorkBuf.Left(10);
			}
			m_strWorkBuf.Trim();
			switch (atoi(m_strWorkBuf))
			{
			case -1:
				m_strWorkBuf = "('您的手机格式错误')";
				break;
			case -2:
				m_strWorkBuf = "('推荐人手机格式错误')";
				break;
			case -5:
				m_strWorkBuf = "('推荐人不能是自己')";
				break;
			case -3:
				m_strWorkBuf = "('您已超过在一小时内发送三次验证码的限制')";
				break;
			case -35001:
				m_strWorkBuf = "('该手机号或用户已送过积分')";
				break;
			case -35006:
				m_strWorkBuf = "('该推荐人已通过推荐方式送过积分')";
				break;
			case -35008:
				m_strWorkBuf = "('该推荐人手机未被验证')";
				break;
			default:
				m_strWorkBuf = "('系统异常，请稍候再试')";
					break;
			}
			ShowMessage(m_strWorkBuf);
			rgPack.strMemo += ",[" + m_strWorkBuf + "]";
			ShowMessage("跳过手机验证,进行资料填写");
		}
		
		//接下来进行资料填写
GETUSERDATAPAGE:
		if (g_ExitFlag)		//控制退出
		{
			DeleteObjectExEx();
			rgPack.iStatus = 0;
			return false;
		}
		ShowMessage("正在获取资料修改页...");
		objectlink = "/Partner/UpdateUserInfoDeposit.aspx";
		strHeaders = m_strHeader;
		//strHeaders += "\r\nReferer:http://jf.sdo.com/Partner/MobileDeposit.aspx";		
		//postStr.Format("mobile=%s\nuserId=%s\nrecmobile=","15219212360","junbrave22");
		iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
		if (m_strWorkBuf.Find("再完善您的资料") > 0)
		{
			ShowMessage("手机验证未通过,跳过资料修改");
			rgPack.strMemo += ",[手机验证未通过,跳过资料修改]";
		}
		else if (m_strWorkBuf.Find("完善了个人信息") > 0)
		{
			ShowMessage("已经添加过资料,跳过资料修改");
			rgPack.strMemo += ",[已经添加过资料]";
		}
		else if (m_strWorkBuf.Find("恭喜您，验证手机成功") > 0)	//加载资料页面成功，开始填写资料
		{
			//此处进行资料修改
			strHeaders += "\r\nReferer:http://jf.sdo.com/Partner/UpdateUserInfoDeposit.aspx";
			ShowMessage("加载资料修改页成功,正在提交资料...");
			CString strPhone = "021-50504740";
			CString strEmail = rgPack.strMail;
			CString strUserName = "";
			GenUserName(strUserName);
			CString strIdentity = rgPack.strIdentity;
			CString strProvince = "";
			CString strCity = "";
			CString strMailCode = "";
			CString strAddress = "";
			strBefore11 = "name=\"__VIEWSTATE\" id=\"__VIEWSTATE\" value=\"";
			strAfter11 = "\" />";
			strViewState = GetKeyString(m_strWorkBuf,strBefore11,strAfter11);
			strBefore11 = "name=\"__EVENTVALIDATION\" id=\"__EVENTVALIDATION\" value=\"";
			strAfter11 = "\" />";
			strValidDate = GetKeyString(m_strWorkBuf,strBefore11,strAfter11);			
			GenAddress(strProvince,strAddress,strMailCode,strCity);
			strViewState.Trim();
			strValidDate.Trim();
			strViewState = URLEncode(strViewState);
			strValidDate = URLEncode(strValidDate);
			strProvince = URLEncode(strProvince);
			strUserName = URLEncode(strUserName);
			strCity = URLEncode(strCity);
			strAddress = URLEncode(strAddress);
			CString strPostPre = "__EVENTTARGET=&__EVENTARGUMENT=&__VIEWSTATE=%s&__EVENTVALIDATION=%s&_ctl0:ContentPlaceHolder1:userDeposit:TB_UserName=%s&_ctl0:ContentPlaceHolder1:userDeposit:TB_UserId=%s&_ctl0:ContentPlaceHolder1:userDeposit:TB_FromIndex=fromMall&_ctl0:ContentPlaceHolder1:userDeposit:TB_IdentityCard=%s&_ctl0:ContentPlaceHolder1:userDeposit:TB_Mobile=%s&_ctl0:ContentPlaceHolder1:userDeposit:TB_Telephone=%s&_ctl0:ContentPlaceHolder1:userDeposit:TB_LinkEmail=%s&_ctl0:ContentPlaceHolder1:userDeposit:DDL_Province=%s&_ctl0:ContentPlaceHolder1:userDeposit:TB_City=%s&_ctl0:ContentPlaceHolder1:userDeposit:TB_Address=%s&_ctl0:ContentPlaceHolder1:userDeposit:TB_Zip=%s&_ctl0:ContentPlaceHolder1:userDeposit:btnConfirm.x=15&_ctl0:ContentPlaceHolder1:userDeposit:btnConfirm.y=5";
			postStr.Format(strPostPre,strViewState,strValidDate,
				strUserName,g_strUserId,strIdentity,
				g_strPhone,	strPhone,strEmail,
				strProvince,strCity,strAddress,
				strMailCode);			
			postStr.Replace(":","%3A");
			if (g_ExitFlag)		//控制退出
			{
				DeleteObjectExEx();
				rgPack.iStatus = 0;
				return false;
			}
			iRet = RequestPostPageExExEx(m_strServerURL,objectlink,strHeaders,postStr,strError,0);
			strBefore11 = "name=\"__VIEWSTATE\" id=\"__VIEWSTATE\" value=\"";
			strAfter11 = "\" />";
			strViewState = GetKeyString(m_strWorkBuf,strBefore11,strAfter11);
			strBefore11 = "name=\"__EVENTVALIDATION\" id=\"__EVENTVALIDATION\" value=\"";
			strAfter11 = "\" />";
			strValidDate = GetKeyString(m_strWorkBuf,strBefore11,strAfter11);
			strViewState = URLEncode(strViewState);
			strValidDate = URLEncode(strValidDate);
			strPostPre = "__EVENTTARGET=&__EVENTARGUMENT=&__VIEWSTATE=%s&__EVENTVALIDATION=%s&_ctl0%3AContentPlaceHolder1%3AuserDeposit%3AbtnSubmit.x=56&_ctl0%3AContentPlaceHolder1%3AuserDeposit%3AbtnSubmit.y=11";
			strPostPre.Replace("%3A",":");
			postStr.Format(strPostPre,strViewState,strValidDate);
			postStr.Replace(":","%3A");
			if (g_ExitFlag)		//控制退出
			{
				DeleteObjectExEx();
				rgPack.iStatus = 0;
				return false;
			}
			iRet = RequestPostPageExExEx(m_strServerURL,objectlink,strHeaders,postStr,strError,0);
			if (m_strWorkBuf.Find("完善个人资料成功") > 0)
			{
				ShowMessage("完善个人资料成功,开始加载升级推广员页面...");
				rgPack.strMemo += ",[完善个人资料成功]";
			}
			else
			{
				ShowMessage("完善个人资料失败,跳过修改资料页,开始加载升级推广员页面...");
				rgPack.strMemo += ",[完善个人资料失败]";
			}
			//需要的参数已经全部生成
		}
		else	//加载资料修改页失败
		{
			ShowMessage("加载资料修改页失败,跳过资料修改");	
		}
		if (g_ExitFlag)		//控制退出
		{
			DeleteObjectExEx();
			rgPack.iStatus = 0;
			return false;
		}
		//升级推广员
		m_iCodeErrorNum = 0;
		ShowMessage("正在获取升级推广员页面...");
		objectlink = "/Activity/FenhongLevelUp.aspx";
		strHeaders = m_strHeader;
		strHeaders += "\r\nReferer:http://jf.sdo.com/FreeScoreIndex.aspx";
		iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
		CFile file1;
		file1.Open("c:\\44444.txt",CFile::modeCreate|CFile::modeWrite);
		file1.Write(m_strWorkBuf,m_strWorkBuf.GetLength());
		file1.Close();
		
		if (m_strWorkBuf.Find("您已经是盛大推广员") > 0)
		{
			ShowMessage("此账号已经是推广员,退出账号...");
			//退出账号
			rgPack.strMemo += ",[已经是推广员]";
			rgPack.iStatus = 1;
			DeleteObjectExEx();
			return false;
		}
		else if (m_strWorkBuf.Find("请输入验证码：") > 0)
		{
			strBefore11 = "name=\"__VIEWSTATE\" id=\"__VIEWSTATE\" value=\"";
			strAfter11 = "\" />";
			strViewState = GetKeyString(m_strWorkBuf,strBefore11,strAfter11);
			strBefore11 = "name=\"__EVENTVALIDATION\" id=\"__EVENTVALIDATION\" value=\"";
			strAfter11 = "\" />";
			strValidDate = GetKeyString(m_strWorkBuf,strBefore11,strAfter11);
			ShowMessage("获取升级推广员页面成功,正在加载验证码...");
			while(!g_ExitFlag)
			{
				//删除旧的验证码
				if (_access(g_strDirect+g_strFileName,0) == 0)
				{
					CFile::Remove(g_strDirect+g_strFileName);
				}
				ShowMessage("正在获取验证码...");
				g_strCheckCode = "";
				strHeaders = m_strHeader;
				strHeaders += "\r\nReferer:http://jf.sdo.com/Activity/FenhongLevelUp.aspx";
				objectlink.Format("/Activity/newvalidatecode1.aspx?0.410849410%s",CTime::GetCurrentTime().Format("%H%M%S"));
				if (g_ExitFlag)		//控制退出
				{
					DeleteObjectExEx();
					rgPack.iStatus = 0;
					return false;
				}
				iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError,1);
				if (iRet != 0)
				{
					ShowMessage("获取验证码失败,跳过");
					rgPack.strMemo += ",[获取验证码失败]";
					return false;
				}
				else
				{
					ShowMessage("获取验证码成功");
					ShowMessage("请输入验证码");
					while(!g_ExitFlag)
					{
						if (g_strCheckCode != "")
						{
							break;
						}
						Sleep(100);
					}
					if (g_ExitFlag)		//控制退出
					{
						DeleteObjectExEx();
						rgPack.iStatus = 0;
						return false;
					}
					CString strFailReason = "";
					CString strCheckCode = g_strCheckCode;
					if (strCheckCode == "r")
					{
						continue;
					}										
					strViewState.Trim();
					strValidDate.Trim();
					strViewState = URLEncode(strViewState);
					strValidDate = URLEncode(strValidDate);
					postStr.Format("__EVENTTARGET=&__EVENTARGUMENT=&__VIEWSTATE=%s&__EVENTVALIDATION=%s&ValidTxt=%s",strViewState,strValidDate,strCheckCode);
					postStr += "&btnLevelUp=%CE%D2%D2%D1%D4%C4%B6%C1%B2%A2%CD%AC%D2%E2%C9%CF%CA%F6%D0%AD%D2%E9%A3%AC%C8%B7%C8%CF%C9%FD%BC%B6%CE%AA%CA%A2%B4%F3%CD%C6%B9%E3%D4%B1";
					objectlink = "/Activity/FenhongLevelUp.aspx";
					if (g_ExitFlag)		//控制退出
					{
						DeleteObjectExEx();
						rgPack.iStatus = 0;
						return false;
					}
					iRet = RequestPostPageExExEx(m_strServerURL,objectlink,strHeaders,postStr,strError,0);//
					CFile file;
					file.Open("c:\\test9999.txt",CFile::modeCreate|CFile::modeWrite);
					file.Write(m_strWorkBuf,m_strWorkBuf.GetLength());
					file.Close();
					if (iRet != 0)
					{
						ShowMessage("传递验证码数据失败,跳过");
						DeleteObjectExEx();
						rgPack.iStatus = 2;
						rgPack.strMemo += ",[传递验证码数据失败]";
						return false;
					}
					if (m_strWorkBuf.Find("您成功升级为盛大推广员") > 0)
					{
						ShowMessage("升级推广员成功");
						rgPack.iStatus = 1;
						rgPack.strMemo += ",[升级推广员成功]";
					}
					else
					{//注册失败
						//<script type='text/javascript'>alert('输入验证码错误！');
						strBefore11 = "<script type='text/javascript'>alert('";
						strAfter11 = "');";
						strFailReason = GetKeyString(m_strWorkBuf,strBefore11,strAfter11);
						ShowMessage("升级推广员失败:	"+strFailReason);
						rgPack.iStatus = 2;
						rgPack.strMemo += ","+strFailReason;
					}
					if (g_iDeleteFlag == 1)
					{
						if (strFailReason.Find("输入验证码错误")>0)
						{
							m_iCodeErrorNum ++;
							if (m_iCodeErrorNum >= g_iCodeErrorNum)
							{
								ShowMessage("验证码错误次数到达上限,跳过此账号");
								m_iCodeErrorNum = 0;
								break;
							}
						}
						else
						{
							DeleteObjectExEx();
							return false;
						}
					}
					else
					{
						DeleteObjectExEx();
						return false;
					}
				}
			}
		}
		else
		{
			ShowMessage("获取升级推广员页面失败,退出此账号...");
			rgPack.strMemo += ",[获取升级推广员页面失败]";
		}
		//退出账号
		DeleteObjectExEx();
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
		RegisterPack rgPack;
		int iRet = GetRegisterPack(rgPack);
		if (iRet == -1)
		{
			ShowMessage("请导入账号...");
			Sleep(700);
			continue;
		}
		else if (iRet == -2)
		{
			Sleep(2000);
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
			SetHttpsType(0);
			UserRegister(rgPack);
			switch (rgPack.iStatus)
			{
			case 0:			//循环
				break;
			case 1:			//失败
				AddToShowList(rgPack);
				DeleteRegisterPack(rgPack);
				break;
			case 2:			//成功
				AddToShowList(rgPack);
				DeleteRegisterPack(rgPack);
				break;
			default:
				break;
			}
			//退出登录
			CString strError = "";
			CString objectlink = "/jfindex.aspx?logout=true";
			CString strHeaders = m_strHeader;
			CString postStr = "";
			CString strShow = "";
			int iRet = -1;
			m_iPostFlag = 0;
			DeleteObjectExEx();
			iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
			DeleteObjectExEx();
			m_strServerURL = "cas.sdo.com:8443";
			SetHttpsType(1);
			objectlink = "/cas/logout?url=http://jf.sdo.com";
			iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
			DeleteObjectExEx();
			m_strServerURL = "www.sdo.com";
			SetHttpsType(0);
			objectlink = "/logout_iframe.asp?url=http%3a%2f%2fjf.sdo.com";
			iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
			DeleteObjectExEx();
			//处理返回值,添加信息显示
			strShow.Format("%s,%s,",rgPack.strAcc,rgPack.strPwd,rgPack.strNickName,rgPack.strIntroMan);
			WriteLog(strShow,0);
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

/************************************************************************/
/*                                                                      */
/************************************************************************/
int C51JobWebPost::GetQQCheckCodeBMP(CString strUrl)
{
	try
	{
		CString strTemp = "";
		char cBuffer[1024] = {0};
		CStdioFile *pFile = m_Session.OpenURL(strUrl);
		int iReadLen = 0;
		if (pFile != NULL)
		{
			CStdioFile out;
			out.Open(g_strDirect+g_strFileName, CFile::modeCreate | CFile::modeWrite |CFile::typeBinary);
			while(true)
			{
				iReadLen = pFile->Read( cBuffer ,1 );
				if ( iReadLen <= 0)
				{
					break;
				}
				out.Write(cBuffer,1); 
			}
			out.Close();
			pFile->Close();
			delete pFile;
			pFile = NULL;
			return 0;
		}
	}
	catch (CInternetException* e)
	{
		int i = 0;
	}
	
	return -1;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int C51JobWebPost::StringSplit(CString srcStr,CString strArray[],char splitChar)
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
void C51JobWebPost::GenUserName(CString &strName)
{
	CString strFamilyName[78] = {"谢","宋","唐","许","韩","冯","邓","曹","彭","曾","肖","田","董","袁","潘","于","蒋","蔡","余","杜","叶","程","苏","魏","吕","丁","任","沈","姚","卢","姜","崔","钟","谭","陆","汪","范","金","石","廖","贾","夏","韦","付","方","白","邹","孟","熊","秦","邱","江","尹","薛","闫","段","雷","侯","龙","史","陶","黎","贺","顾","毛","郝","龚","邵","万","钱","严","覃","武","戴","莫","孔","向","汤"};
	CString strLastName[78] = {"谢","宋","唐","许","韩","冯","邓","曹","彭","曾","肖","田","董","袁","潘","于","蒋","蔡","余","杜","叶","程","苏","魏","吕","丁","任","沈","姚","卢","姜","崔","钟","谭","陆","汪","范","金","石","廖","贾","夏","韦","付","方","白","邹","孟","熊","秦","邱","江","尹","薛","闫","段","雷","侯","龙","史","陶","黎","贺","顾","毛","郝","龚","邵","万","钱","严","覃","武","戴","莫","孔","向","汤"};;
	int iFamilyName = 0;
	int iLastName = 0;
	srand(time(NULL));
	iFamilyName = rand() % 78;
	iLastName = rand() % 78;
	strName.Format("%s%s",strFamilyName[iFamilyName],strLastName[iLastName]);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void C51JobWebPost::GenAddress(CString &strProvince,CString &strAddress,CString &strMailCode,CString &strCity)
{
	CString strProvinceArray[15] = {"安徽省","澳门","北京市",
		"福建省","甘肃省","广东省",
		"广西","贵州省","海南省",
		"河北省","河南省","黑龙江省",
		"湖北省","湖南省","吉林省"};
	CString strAddressArray[15] = {"安徽省合肥市桂东路11号",
						"澳门抚平路1057号",
						"北京市桂东路1001号",
						"福建省福州闯东路101号",
						"甘肃省兰州市兰桂路88号",
						"广东省深圳市南山大冲614栋301号",
						"广西桂林市桂高路35号",
						"贵州省贵阳市场解放路28号",
						"海南省海口市旅游一路99号",
						"河北省石家庄市永和路55号",
						"河南省郑州市郑明路81号",
						"黑龙江省哈尔滨市哈东路98号",
						"湖北省武昌市武平路87号",
						"湖南省岳阳市岳阳楼区1024号",
						"吉林省吉林市吉昌路34号"
								};
	CString strCityArray[15] = {"合肥市",
		"澳门",
		"北京市",
		"福州",
		"兰州市",
		"深圳市",
		"桂林市",
		"贵阳市",
		"海口市",
		"石家庄市",
		"郑州市",
		"哈尔滨市",
		"武昌市",
		"岳阳市",
		"吉林市"
	};
	CString strCodeArray[15] = {"315004",
		"564824",
		"524654",
		"112547",
		"256548",
		"518000",
		"619222",
		"651248",
		"201547",
		"616584",
		"232564",
		"313658",
		"424565",
		"414005",
		"515007"
	};
	srand(time(NULL));
	int iNum = rand() % 15;
	strProvince.Format("%s",strProvinceArray[iNum]);
	strCity.Format("%s",strCityArray[iNum]);
	strAddress.Format("%s",strAddressArray[iNum]);
	strMailCode.Format("%s",strCodeArray[iNum]);
}