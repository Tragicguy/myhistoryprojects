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

CString g_strCurrIP = "";
CString g_strDirect = "";		//图像保存目录
CString g_strFileName = "";
int g_iRegWebFlag = 1;
int g_iIsChangeMac =0;

int g_iTimeStart = 0;
int g_iTimeEnd = 0;
int g_iTimeDelay = 0;
int g_iFreshDelay = 0;
int g_iHelpDelay = 0;	//顶帖间隔，两帖之间的间隔

int g_iIsNeedLogin = 1;
int g_iUtfFlag = 0;
/************************************************************************/
/*                                                                      */
/************************************************************************/
//添加发送缓存
/************************************************************************/
/*                                                                      */
/************************************************************************/
void ShowMessage(CString strMsg)
{
	CUserMessagePump *pMsgPump = NULL;
	pMsgPump = CUserMessagePump::GetInstance();
	if (pMsgPump)
	{
		pMsgPump->AddToMessagePump(USERWINDOWMSG_1,strMsg);
	}
}

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
	int nCount = g_AccList.GetItemCount();
	bool isExist = false;
	for ( int i = 0;i<nCount;i++)
	{
		if (g_AccList.GetItemText(i,1) == rgPack.strAcc && g_AccList.GetItemText(i,4) == rgPack.strMail)
		{
			CString strNum = g_AccList.GetItemText(i,3);
			strNum.Format("%d",atoi(strNum) + 1);
			g_AccList.SetItemText(i,3,strNum);
			break;
		}		
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
		if (iFlag == 0)
		{
			destStr = srcStr;
		}
		else
		{
			destStr.Format("%s	%s",srcStr,currTime.Format("%Y-%m-%d %H:%M:%S"));
		}		
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
	//m_strHeader += "\r\nAccept:image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, application/x-silverlight, */*";
	m_strHeader += "\r\nAccept:*/*";
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
	m_iID = 0;
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
	return true;     
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void C51JobWebPost::ThreadFunc(LPVOID lParam)
{
	CoInitialize(NULL);
	C51JobWebPost *pParam = ((C51JobWebPost *)lParam);
	if ( pParam )
	{
		pParam->RegisterFunc();
		ShowMessage("线程退出");
	}
	CoUninitialize();
     
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void C51JobWebPost::RegisterFunc()
{
	CString strOldAcc = "";
	g_iIsNeedLogin = 1;
	m_iNeedChageIP = 0;
	m_iIpSuccNum = 0;
	CString sendStr = "";
	CString strError = "";
	ShowMessage("初始化成功");
	int iTime = rand() % ((g_iTimeEnd - g_iTimeStart)==0?30:(g_iTimeEnd - g_iTimeStart));
	g_iTimeDelay = g_iTimeStart + iTime;
	if (g_iTimeDelay < 1)
	{
		g_iTimeDelay = 1;
	}
	sendStr.Format("顶帖时间间隔:%d秒",g_iTimeDelay);
	ShowMessage(sendStr);
	if (g_iFreshDelay < 0)
	{
		g_iFreshDelay = 0;
	}
	sendStr.Format("页面刷新时间间隔:%d毫秒",g_iTimeDelay);
	ShowMessage(sendStr);
	srand(time(NULL));
	long lLastHelpTime = -10000;
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
		if (strOldAcc != rgPack.strAcc)
		{
			g_iIsNeedLogin = 1;
		}
// 		if (!IsOverLastTime(g_iTimeDelay,rgPack.iLastTime))
// 		{
// 			DWORD t2 = 0;
// 			t2 = ::GetTickCount();
// 			DWORD tSpan = 0;
// 			tSpan = t2 - rgPack.iLastTime;
// 			sendStr.Format("%d秒后开始顶帖",g_iTimeDelay - int(tSpan / 1000) );
// 			ShowMessage(sendStr);
// 			Sleep(1000);
// 			continue;
// 		}
		//rgPack.iLastTime = ::GetTickCount();
		if (!IsOverLastTime(g_iTimeDelay,lLastHelpTime))
		{
			DWORD t2 = 0;
			t2 = ::GetTickCount();
			DWORD tSpan = 0;
			tSpan = t2 - lLastHelpTime;
			sendStr.Format("%d秒后开始顶帖",g_iTimeDelay - int(tSpan / 1000) );
			ShowMessage(sendStr);
			Sleep(1000);
			continue;
		}
		//判断刷新时间是否到了
		CString strContent = RandContent();
		ShowMessage("随机内容:" + strContent);
		iRet = HelpSubject(rgPack.strAcc,rgPack.strPwd,rgPack.strMail,strContent,g_iFreshDelay,strError);
		switch (iRet)
		{
		case  -2:			
			//DeleteRegisterPack(rgPack);
			break;
		case -1:
			break;
		case -3:
			DeleteRegisterPack(rgPack);
			lLastHelpTime = ::GetTickCount();
			break;
		case 0:
			DeleteRegisterPack(rgPack);
			//rgPack.iLastTime = ::GetTickCount();
			lLastHelpTime = ::GetTickCount();
			AddRegisterPack(rgPack);
			AddToShowList(rgPack);
			break;
		default:
			break;
		}
		strOldAcc = rgPack.strAcc;
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
CString C51JobWebPost::GetKeyStringEx(CString srcStr,CString &strBefore,CString &strAfter)
{
	int iPosBefore = 0;
	int iPosAfter = 0;
	iPosAfter = srcStr.Find(strAfter);
	srcStr = srcStr.Left(iPosAfter);
	iPosBefore = srcStr.ReverseFind('"');
	if (iPosBefore < 0 || iPosAfter < 0)
	{
		return "";
	}
	else
	{
		return srcStr.Mid(iPosBefore+1);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString C51JobWebPost::GetKeyStringExEx(CString srcStr,CString &strBefore,CString &strAfter)
{
	int iPosBefore = 0;
	int iPosAfter = 0;
	iPosAfter = srcStr.Find(strAfter);
	srcStr = srcStr.Left(iPosAfter);
	iPosBefore = srcStr.ReverseFind('=');
	if (iPosBefore < 0 || iPosAfter < 0)
	{
		return "";
	}
	else
	{
		return srcStr.Mid(iPosBefore+1);
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
					if (strID == "machineserial")		//email name repassword password
					{
						m_strMac = GetElementValue(pElement);
						ShowMessage(m_strMac);
					}
					else if (strID == "transactionid")
					{
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
	Sleep(1500);
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
			HWND hwndChind = find.GetChildWindow(hwnd,6);	//账号选择框
			Sleep(40);
			hwndChind = find.GetChildWindow(hwndChind,1);//账号输入编辑框
			//注册按钮
			hwndChind = find.GetChildWindow(hwnd,9);
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

/************************************************************************/
/*                                                                      */
/************************************************************************/
int C51JobWebPost::GetFirstUsefulKey()
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
			if ( !adoConn.ConnectAccess(strAppPath+"machineInfo.mdb",""))
			{
				ShowMessage("连接数据库失败");
				return -1; 
			}
			adoRst.SetAdoConnection(&adoConn);
			adoRst.SetCursorLocation(adUseClient);
		}
		CString strSQL;		
		strSQL.Format("select top 1 * from [machine] where datediff('d',lastusetime,now()) > 0 order by [ID] asc");
		if(!adoRst.Open(strSQL,adCmdText))
		{
			iRet = -1;
			ShowMessage("读取键值信息失败");
		}
		else
		{
			if (!adoRst.IsEOF())
			{
				adoRst.GetCollect("ID",m_iID);
				adoRst.GetCollect("machineserial",m_strMac);
				adoRst.GetCollect("transactionid",m_strTrans);
				adoRst.GetCollect("verify",m_strVerify);
				iRet = 0;
			}
			else
			{
				ShowMessage("当天已经没有可用的键值信息，请调用生成程序");
				iRet = -1;
			}
		}
		adoRst.Close();
		adoConn.Close();
		return iRet;
	}
	catch (...)
	{
		adoRst.Close();
		adoConn.Close();
		ShowMessage("读取键值信息异常");
	}
	return -1;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int C51JobWebPost::DisableKey(int iKeyNum)
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
			if ( !adoConn.ConnectAccess(strAppPath+"machineInfo.mdb",""))
			{
				ShowMessage("连接数据库失败");
				return -1; 
			}
			adoRst.SetAdoConnection(&adoConn);
			adoRst.SetCursorLocation(adUseClient);
		}
		CString strSQL;		
		strSQL.Format("update [machine] set [lastusetime]=now() where [ID]=%d",iKeyNum);
		if(!adoRst.Open(strSQL,adCmdText))
		{
			iRet = -1;
			ShowMessage("删除机器信息失败");
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
		ShowMessage("删除机器信息异常");
	}
	return -1;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//-1会循环,-2不循环
int C51JobWebPost::HelpSubject(CString strUserName,CString strPwd,CString strUrl,CString strSubContent,int iDelay,CString &strError)
{
	try
	{
		CString postStr = "";
		CString strShow = "";
		int iRet = -1;
		INTERNET_PORT iPort = 80;
		DWORD dType = AFX_INET_SERVICE_HTTP;
		CString strSubUrl = "";
		BOOL iiRet = AfxParseURL(strUrl,dType,m_strServerURL,strSubUrl,iPort);
		if (iiRet == 0)
		{
			ShowMessage("解析URL失败,退出");
			return -3;
		}
		CString strCmpUrl = "";
		strCmpUrl = "bbs.0595bbs.cn";		//泉州论坛
		strCmpUrl = "bbs.etaicang.com";		//太仓论坛
		strCmpUrl = "www.18lou.net";		//18楼
		strCmpUrl = "www.chinayk.com";		//中国营口论坛
		strCmpUrl = "www.0574bbs.com";		//余姚论坛
		strCmpUrl = "bbs.zxip.com";			//慈溪论坛
		strCmpUrl = m_strServerURL;
		//strCmpUrl = "bbs.0595bbs.cn";		//泉州论坛
		if (strCmpUrl == "www.chinayk.com")
		{
			g_iUtfFlag = 1;
		}
		else
		{
			g_iUtfFlag = 0;
		}
		if (strCmpUrl != m_strServerURL)
		{
			ShowMessage("论坛地址错误,退出");
			return -3;
		}
		CString strFailReason = "";
		CString strFlag = "";
		CString sendStr = "";
		CString objectlink = "/login.php";
		CString strHeaders = m_strHeader;
		if (g_iIsNeedLogin == 1)
		{
			ShowMessage("获取登录页面...");			
			iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
			if (iRet < 0)
			{
				ShowMessage("获取登录页面失败");
				m_nErrorTime ++;
				DeleteObjectExEx();
				return -1;
			}
			int iPos = m_strWorkBuf.Find("Cookie 有效期:");			
			if (iPos >= 0)
			{
				ShowMessage("成功获取登录页...");
			}
			else if (m_strWorkBuf.Find("您已经为会员身份,请不要重复登录") > 0)	//需要退出登录
			{
				objectlink = "/index.php";
				Sleep(iDelay);
				iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
				CString strBefore = "";
				CString strAfter = "\" hidefocus=\"true\">退出</a>";
				objectlink = GetKeyStringEx(m_strWorkBuf,strBefore,strAfter);
				Sleep(iDelay);
				iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
				return -1;
			}
			else 
			{
				ShowMessage("获取登录页失败");
				return -2;
			}
			m_iCheckFlag = 0;
			g_strCheckCode = "";				
			if (g_ExitFlag)
			{
				DeleteObjectExEx();
				return -2;
			}			
			CString strCheckCode = g_strCheckCode;
			CString struuidCode = "";
			CString strAppId = "";
			objectlink = "/login.php?";
			strHeaders = m_strHeader;
			CString strReffers = "";
			strReffers.Format("\r\nReferer:http://%s/login.php",m_strServerURL);
			strHeaders += strReffers;
			postStr.Format("forward=&jumpurl=&step=2&lgt=0&pwuser=%s&pwpwd=%s&hideid=0&cktime=0&submit=%B5%C7%C2%BC",
				strUserName,
				strPwd
				);
			Sleep(iDelay);
			iRet = RequestPostPageExExEx(m_strServerURL,objectlink,strHeaders,postStr,strError,0);//
			if ( iRet != 0 )
			{
				m_nErrorTime ++;
				DeleteObjectExEx();
				return -2;
			}
			m_strWorkBuf.MakeLower();
			CFile file;
			file.Open("c:\\sdfd.txt",CFile::modeCreate|CFile::modeWrite);
			file.Write(m_strWorkBuf,m_strWorkBuf.GetLength());
			file.Close();
			iPos = m_strWorkBuf.Find("您已经顺利登录");
			int iPos11 = m_strWorkBuf.Find(strUserName);
			int iPos2 = m_strWorkBuf.Find("登陆成功");
			if ( iPos > 0 || iPos2 > 0 || iPos11 > 0)
			{
			}
			else if (m_strWorkBuf.Find("您已经为会员身份") > 0)	//需要退出登录
			{
				objectlink = "/index.php";
				Sleep(iDelay);
				iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
				CString strBefore = "";
				CString strAfter = "\" hidefocus=\"true\">退出</a>";
				objectlink = GetKeyStringEx(m_strWorkBuf,strBefore,strAfter);
				Sleep(iDelay);
				iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
				return -1;
			}
			else
			{
				CString strBefore = "";
				CString strAfter = "\" hidefocus=\"true\">退出</a>";

				ShowMessage("登录论坛失败");
				strBefore = "<span class=\"f14\">";
				if (m_strWorkBuf.Find(strBefore) >=0)
				{
					strAfter = "</span><br />";
					strError = GetKeyString(m_strWorkBuf,strBefore,strAfter);
					ShowMessage(strError);
				}
				DeleteObjectExEx();
				if (m_strWorkBuf.Find("密码错误") >=0 || m_strWorkBuf.Find("不存在") >= 0) 
				{
					return -3;
				}
				return -2;
			}				
			ShowMessage("登录论坛成功");
			g_iIsNeedLogin = 0;
		}
		ShowMessage("开始获取顶帖页面...");
		CString strTime = CTime::GetCurrentTime().Format("%m%d%H%M%S");
		try
		{				
			//加载进入需要顶的帖子
			objectlink = strSubUrl;
			Sleep(iDelay);
			iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
			if (iRet < 0)
			{
				ShowMessage("获取顶帖页面失败");
				m_nErrorTime ++;
				LogOut();
				DeleteObjectExEx();
				return -1;
			}
			CFile file1;
			file1.Open("c:\\333333331.txt",CFile::modeCreate|CFile::modeWrite);
			file1.Write(m_strWorkBuf,m_strWorkBuf.GetLength());
			//file1.Write(m_szWorkBuf,m_iszWorkBufLen);
			file1.Close();
			int iPos = strSubUrl.ReverseFind('/');
			strSubUrl = strSubUrl.Mid(iPos+1);
			if (m_strWorkBuf.Find(strSubUrl) < 0)
			{
				ShowMessage("获取顶帖页面失败");
				m_nErrorTime ++;
				LogOut();
				DeleteObjectExEx();
				return -2;
			}
			ShowMessage("获取顶帖页面成功");
			
			//AfxMessageBox(m_strWorkBuf.Right(100));
			int iiiFlag = 0;
			objectlink = "/post.php?";
			CString strTid = "";
			CString strVerify = "";
			CString strHexie = "";
			CString strTitle = "";
			CString strContent = "";
			CString strFid = "";
			CString strBefore = "";
			CString strAfter = "\" name=\"tid\" />";
			strAfter = "\" name=\"_hexie";
			int iiiiPos = m_strWorkBuf.Find("_hexie");
			if ( iiiiPos< 0)
			{
				ShowMessage("获取参数方式有变");
				iiiFlag = 1;
			}
			if (iiiFlag == 0)
			{
				strAfter = "\" name=\"tid\" />";\
				strTid = GetKeyStringEx(m_strWorkBuf,strBefore,strAfter);
				strAfter = "\" name=\"_hexie";
				strHexie = GetKeyStringEx(m_strWorkBuf,strBefore,strAfter);
				strAfter = "\" name=\"fid\" />";
				strFid = GetKeyStringEx(m_strWorkBuf,strBefore,strAfter);
				strBefore = "name=\"verify\" value=\"";
				strAfter = "\" />";
				strVerify = GetKeyString(m_strWorkBuf,strBefore,strAfter);
				strBefore = "name=\"atc_title\" value=\"";
				strAfter = "\" ";
				strTitle = GetKeyString(m_strWorkBuf,strBefore,strAfter);
			}
			else
			{
				strAfter = " name=tid";
				strTid = GetKeyStringExEx(m_strWorkBuf,strBefore,strAfter);
				strAfter = " name=_hexie";
				strHexie = GetKeyStringExEx(m_strWorkBuf,strBefore,strAfter);
				strAfter = " name=fid";
				strFid = GetKeyStringExEx(m_strWorkBuf,strBefore,strAfter);
				strBefore = "name=tid> <INPUT type=hidden value=";
				strAfter = " ";
				strVerify = GetKeyString(m_strWorkBuf,strBefore,strAfter);
				strBefore = "name=\"atc_title\" value=\"";
				strAfter = " name=atc_title";
				strTitle = GetKeyStringExEx(m_strWorkBuf,strBefore,strAfter);
			}
			if (strHexie == "" || strFid == "")
			{
				ShowMessage("找不到参数");
				g_iIsNeedLogin = 1;
				DeleteObjectExEx();
				return -1;
			}
			/*
			<input type="hidden" value="192" name="fid" />
			<input type="hidden" value="167729" name="tid" />
			<input type="hidden" name="verify" value="eb4ee27e" />
			<input type="hidden" value="cn0zz" name="_hexie" />
			*/
			
			
			strSubContent = URLEncode(strSubContent);
			strTitle = URLEncode(strTitle);
			postStr.Format("atc_usesign=1&atc_convert=1&atc_autourl=1&step=2&action=reply&fid=%s&tid=%s&verify=%s&_hexie=%s&atc_title=%s&atc_content=%s&attachment_1=&atc_desc1=",
				strFid,
				strTid,
				strVerify,
				strHexie,
				strTitle,
				strSubContent);
			Sleep(iDelay);
			CString strRefer = "";
			strRefer.Format("\r\nReferer:%s",strUrl);
			strHeaders += strRefer;
			iRet = RequestPostPageExExEx(m_strServerURL,objectlink,strHeaders,postStr,strError,0);//
			if ( iRet != 0 )
			{
				m_nErrorTime ++;
				LogOut();
				DeleteObjectExEx();
				return -1;
			}
			CFile file;
			file.Open("c:\\sdfd111.txt",CFile::modeCreate|CFile::modeWrite);
			file.Write(m_strWorkBuf,m_strWorkBuf.GetLength());
			file.Close();
			if (m_iStatusCode != 302)
			{
				if (m_strWorkBuf.Find("发帖完毕点击进入主题列表") > 0)
				{
					ShowMessage("顶帖成功");
					LogOut();
					DeleteObjectExEx();
					return 0;
				}
				strBefore = "<span class=\"f14\">";
				if (m_strWorkBuf.Find(strBefore) >=0)
				{
					strAfter = "</span><br />";
					strError = GetKeyString(m_strWorkBuf,strBefore,strAfter);
					ShowMessage(strError);
					LogOut();
					DeleteObjectExEx();
					return -2;
				}
// 				ShowMessage("顶帖失败");
// 				DeleteObjectExEx();
// 				return -2;
			}			
			ShowMessage("顶帖成功");
			LogOut();
			DeleteObjectExEx();
			return 0;
		}
		catch (...)
		{
			LogOut();
			DeleteObjectExEx();
			return -1;
		}			
	}
	catch (...)
	{
	}
	LogOut();
	DeleteObjectExEx();
	return -1;    
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString C51JobWebPost::RandContent()
{
	int iCount = g_ContentList.GetSize();
	if (iCount <= 0)
	{
		return "未导入数据,给你的默认数据";
	}
	int iValue = rand() % iCount;
	while (iValue >= iCount)
	{
		iValue --;
	}
	if (iValue < 0)
	{
		return "未导入数据,给你的默认数据";
	}
	return g_ContentList.GetAt(iValue);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//判断是否已经超过发送出错等待时间
bool C51JobWebPost::IsOverLastTime(unsigned int iErrorStopTime,long lastTime)
{
	DWORD t2 = 0;
	t2 = ::GetTickCount();
	DWORD tSpan = 0;
	tSpan = t2 - lastTime;
	if (tSpan > iErrorStopTime * 1000) //iDelay 秒
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
void C51JobWebPost::LogOut()
{	
// 	CString strHeaders = m_strHeader;
// 	CString postStr = "";
// 	CString strError = "";
// 	CString strTid = "";
// 	CString strVerify = "";
// 	CString strBefore = "";
// 	CString strAfter = "";
// 	CString objectlink  = "";
// 	objectlink = "/index.php";
// 	int iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
// 	strHeaders += "\r\nReferer:http://bbs.0595bbs.cn/index.php";
// 	strBefore = "name=\"verify\" value=\"";
// 	strAfter = "\" />";
// 	strVerify = GetKeyString(m_strWorkBuf,strBefore,strAfter);	
// 	objectlink.Format("/login.php?action=quit&verify=%s",strVerify);
// 	iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
}