// ControlFile\DbGateAllSet.cpp : 实现文件
//

#include "stdafx.h"
#include "../DbGate.h"
#include "DbGateAllSet.h"
#include "../ControlFile/UserFile.h"
#include "../ControlFile/dataType.h"
#include ".\dbgateallset.h"
#include "../MainFrm.h"
#include "SetBase.h"
/************************************************************************/
/*                                                                      */
/************************************************************************/
IMPLEMENT_DYNAMIC(CDbGateAllSet, CDialog)
CDbGateAllSet::CDbGateAllSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDbGateAllSet::IDD, pParent)
{
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CDbGateAllSet::~CDbGateAllSet()
{
}

void CDbGateAllSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CDbGateAllSet::OnInitDialog()
{
	CDialog::OnInitDialog();
	HICON hIcon;
	hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(hIcon, TRUE);			// 设置大图标
	SetIcon(hIcon, FALSE);		// 设置小图标
	InitParams();		//初始化配置
	return TRUE;
}

BEGIN_MESSAGE_MAP(CDbGateAllSet, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_MORE, OnBnClickedBtnMore)
	ON_BN_CLICKED(IDC_CHECK_RECVMO, OnBnClickedCheckRecvmo)
	ON_BN_CLICKED(IDC_CHECK_RECVREPORT, OnBnClickedCheckRecvreport)
	ON_BN_CLICKED(IDC_CHECK_RECONN, OnBnClickedCheckReconn)
	ON_BN_CLICKED(IDC_CHECK_DIFFMOBILE, OnBnClickedCheckDiffmobile)
	ON_BN_CLICKED(IDC_RADIO_MSSQL, OnBnClickedRadioMssql)
	ON_BN_CLICKED(IDC_RADIO_ORACLE, OnBnClickedRadioOracle)
	ON_BN_CLICKED(IDC_RADIO_MYSQL, OnBnClickedRadioMysql)
	ON_BN_CLICKED(IDC_RADIO_ACCESS, OnBnClickedRadioAccess)
	ON_BN_CLICKED(IDC_RADIO_KEXINTONG, &CDbGateAllSet::OnBnClickedRadioKexintong)
	ON_BN_CLICKED(IDC_RADIO_SI, &CDbGateAllSet::OnBnClickedRadioSi)
END_MESSAGE_MAP()

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CDbGateAllSet::PreTranslateMessage( MSG* pMsg )
{
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;      //屏蔽掉Esc键
	}
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		return TRUE;      //屏蔽掉Enter键
	}
	return CDialog::PreTranslateMessage(pMsg);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//0正确，1错误
int CDbGateAllSet::CheckSendTime(CString strTime1,CString strTime2)
{
	if(strTime1.GetLength() != 5 || strTime1.GetLength() != 5)
	{
		return 1;
	}
	if (atoi(strTime1.Left(2))<0||atoi(strTime1.Left(2))>23 || atoi(strTime1.Right(2))<0 || atoi(strTime1.Right(2))>59)
	{
		return 1;
	}
	if (atoi(strTime2.Left(2))<0||atoi(strTime2.Left(2))>23 || atoi(strTime2.Right(2))<0 || atoi(strTime2.Right(2))>59)
	{
		return 1;
	}
	return 0;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//显示配置信息
void CDbGateAllSet::InitParams()
{
	CString strTemp = "";
	CString strAppPathEx = "";
	int iTemp = 0;
	CUserFile *pFile = NULL;
	pFile = CUserFile::GetInstance();
	if (pFile)
	{	
		int iRet = -1;
		CString strAppPath = "";
		pFile->GetAppPath(strAppPath);
		strAppPathEx =strAppPath;
		strAppPath += MWGATEINITFILE;
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
		iRet = pFile->ReadInitInfo(strAppPath,"Sendthread","Buflen",strTemp,m_iSendBufLen,1);
		if (iRet < 0)
		{
			m_iSendBufLen = 100;
		}
		pFile->CheckInitValue(m_iSendBufLen,100,1000);
		iRet = pFile->ReadInitInfo(strAppPath,"Sendthread","Splitlength",strTemp,m_iSplitLength,1);
		if (iRet < 0)
		{
			m_iSplitLength = 70;
		}
		pFile->CheckInitValue(m_iSplitLength,1,1000);
		int SplitlengthUnicom = 0;	//联通
		int iSplitChinaNum = 0;	//小灵通
		int iSplitChinacom = 0;	//电信手机
		//联通
		iRet = pFile->ReadInitInfo(strAppPath,"Sendthread","SplitlengthUnicom",strTemp,SplitlengthUnicom,1);
		if (iRet < 0)
		{
			SplitlengthUnicom = 70;
		}
		pFile->CheckInitValue(SplitlengthUnicom,1,1000);
		SetDlgItemInt(IDC_EDIT_SPLITELEN_UNICOM,SplitlengthUnicom);
		//小灵通
		iRet = pFile->ReadInitInfo(strAppPath,"Sendthread","SplitlengthChinaNum",strTemp,iSplitChinaNum,1);
		if (iRet < 0)
		{
			iSplitChinaNum = 70;
		}
		pFile->CheckInitValue(iSplitChinaNum,1,1000);
		SetDlgItemInt(IDC_EDIT_SPLITELEN_CHINANUM,iSplitChinaNum);
		//电信手机
		iRet = pFile->ReadInitInfo(strAppPath,"Sendthread","SplitlengthChinacom",strTemp,iSplitChinacom,1);
		if (iRet < 0)
		{
			iSplitChinacom = 70;
		}
		pFile->CheckInitValue(iSplitChinacom,1,1000);
		SetDlgItemInt(IDC_EDIT_SPLITELEN_CHINACOM,iSplitChinacom);
		//添加缓冲线程配置
		iRet = pFile->ReadInitInfo(strAppPath,"Addbuf","NoMessage",strTemp,m_iAddNullWaitTime,1);
		if (iRet < 0)
		{
			m_iAddNullWaitTime = 2000;
		}
		pFile->CheckInitValue(m_iAddNullWaitTime,500,5000);
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
			m_dbSQLUser = "sa";
		}
		iRet = pFile->ReadInitInfo(strAppPath,"database","dbpwd",m_dbSQLPwd,iTemp,0);
		if (iRet < 0)
		{
			m_dbSQLPwd = "123456";
		}
		//自动运行
		int iAutoRun = 0;
		iRet = pFile->ReadInitInfo(strAppPath,"AutoRun","AutoRun",strTemp,iAutoRun,1);
		if ( iRet < 0 )
		{
			iAutoRun = 0;
		}
		pFile->CheckInitValue(iAutoRun,0,1);
		
		//子端口
		iRet = pFile->ReadInitInfo(strAppPath,"SubPort","type",strTemp,m_iUseSubPort,1);
		if (iRet < 0)
		{
			m_iUseSubPort = 0;
		}
		pFile->CheckInitValue(m_iUseSubPort,0,1);

        //出错停止时间
		int iErrorStopTime = 0;
		iRet = pFile->ReadInitInfo(strAppPath,"StopTime","Error",strTemp,iErrorStopTime,1);
		if (iRet < 0)
		{
			iErrorStopTime = 0;
		}
		pFile->CheckInitValue(iErrorStopTime,1,5000);

		//接收上行
		iRet = pFile->ReadInitInfo(strAppPath,"Recv","RecvMo",strTemp,m_iIsRecvMo,1);
		if (iRet < 0)
		{
			m_iIsRecvMo = 1;
		}
		pFile->CheckInitValue(m_iIsRecvMo,0,1);
		//状态报告
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
		//激活连接间隔
		int iTestConInterval = 0;
		pFile->ReadInitInfo(strAppPath,"Testconn","interval",strTemp,iTestConInterval,1);
		pFile->CheckInitValue(iTestConInterval,60,150);
		SetDlgItemInt(IDC_EDIT_TESTCONNINTERVAL,iTestConInterval);

		m_dbSQLUser = pFile->StrDecode(m_dbSQLUser);
		m_dbSQLPwd = pFile->StrDecode(m_dbSQLPwd);
		//数据库信息
		SetDlgItemText(IDC_EDIT_DBASESERVER,m_dbSQLServer);
		SetDlgItemText(IDC_EDIT_DBNAME,m_dbSQLName);
		SetDlgItemText(IDC_EDIT_DBASEUID,m_dbSQLUser);
		SetDlgItemText(IDC_EDIT_DBASEPWD,m_dbSQLPwd);

		m_strAccount = pFile->StrDecode(m_strAccount);
		m_strAccPwd = pFile->StrDecode(m_strAccPwd);
		//用户账户
		SetDlgItemText(IDC_EDIT_ACCOUNT,m_strAccount);
		SetDlgItemText(IDC_EDIT_ACCPWD,m_strAccPwd);

		//缓冲区
		SetDlgItemInt(IDC_EDIT_BUFFLEN,m_iSendBufLen);

		//发送线程
		SetDlgItemInt(IDC_EDIT_SENDINTERVAL,m_iSendInterval);
		SetDlgItemInt(IDC_EDIT_SENDNULLWAIT,m_iBufNullWaitTime);
		SetDlgItemInt(IDC_EDIT_SPLITELEN,m_iSplitLength);
		//添加缓冲线程
//		SetDlgItemInt(IDC_EDIT_ADDINTERVAL,m_iAddBufInterval);
		SetDlgItemInt(IDC_EDIT_ADDNULLWAIT,m_iAddNullWaitTime);

		//平台信息
		SetDlgItemText(IDC_EDIT_SERVERIP,m_strServerIP);
		SetDlgItemInt(IDC_EDIT_SERVERPORT,m_iServerPort);

		//号段
		SetDlgItemText(IDC_EDIT_MOBILE,m_strMobileList);
		SetDlgItemText(IDC_EDIT_UNICOM,m_strUnicomList);
		SetDlgItemText(IDC_EDIT_TELECOM,m_strTelecomList);		
		
		((CButton *)GetDlgItem(IDC_CHECK_AUTORUN))->SetCheck(iAutoRun);
		
		((CButton *)GetDlgItem(IDC_CHECK_USESUBPORT))->SetCheck(m_iUseSubPort);
		((CButton *)GetDlgItem(IDC_CHECK_RECVMO))->SetCheck(m_iIsRecvMo);
		((CButton *)GetDlgItem(IDC_CHECK_RECVREPORT))->SetCheck(m_iIsRecvReport);
		OnBnClickedCheckRecvreport();
		OnBnClickedCheckRecvmo();		
		((CButton *)GetDlgItem(IDC_CHECK_LOG))->SetCheck(g_iIsNeedToLog);	

		SetDlgItemInt(IDC_EDIT_ERRORSTOPTIME,iErrorStopTime);
		SetDlgItemInt(IDC_EDIT_PHONESPLITELEN,m_iPhoneSplitLen);
		//运行时间段
		SetDlgItemText(IDC_EDIT_TIMEREADSTART,m_strRunTime.Left(5));
		SetDlgItemText(IDC_EDIT_TIMEREADEND,m_strRunTime.Right(5));
		int iDiffMobile = 0;
		pFile->ReadInitInfo(strAppPath,"NumberHead","Differ",strTemp,iDiffMobile,1);
		((CButton *)GetDlgItem(IDC_CHECK_DIFFMOBILE))->SetCheck(iDiffMobile);
		int iCheckKey = 0;
		pFile->ReadInitInfo(strAppPath,"keycheck","type",strTemp,iCheckKey,1);
		((CButton *)GetDlgItem(IDC_CHECK_KEYCHECK))->SetCheck(iCheckKey);
		int iRecvmoInterval = 0;
		int iRecvreportInterval = 0;
		pFile->ReadInitInfo(strAppPath,"Recv","RecvMoInterval",strTemp,iRecvmoInterval,1);
		pFile->ReadInitInfo(strAppPath,"Recv","RecvReportInterval",strTemp,iRecvreportInterval,1);
		pFile->CheckInitValue(iRecvmoInterval,1,100);
		pFile->CheckInitValue(iRecvreportInterval,1,100);
		SetDlgItemInt(IDC_EDIT_RECVMOINTERVAL,iRecvmoInterval);
		SetDlgItemInt(IDC_EDIT_RECVREPORTINTERVAL,iRecvreportInterval);
		CString strPhoneFilePath = "";
		pFile->ReadInitInfo(strAppPath,"phonefile","path",strPhoneFilePath,iTemp,0);
		SetDlgItemText(IDC_EDIT_PHONEFILEPATH,strPhoneFilePath);
		CString strWindowName = "";
		pFile->ReadInitInfo(strAppPath,"window","name",strWindowName,iTemp,0);
		if (strWindowName=="")
		{
			strWindowName = "梦网短信网关";
		}
		m_strOldName = strWindowName;
		SetDlgItemText(IDC_EDIT_WINDOWNAME,strWindowName);
		//自动定时重连
		int iIsReconn = 0;
		int iReconnInterval = 0;
		pFile->ReadInitInfo(strAppPath,"Reconn","type",strTemp,iIsReconn,1);
		pFile->ReadInitInfo(strAppPath,"Reconn","interval",strTemp,iReconnInterval,1);
		pFile->CheckInitValue(iIsReconn,0,1);
		pFile->CheckInitValue(iReconnInterval,100,100000);
		((CButton *)GetDlgItem(IDC_CHECK_RECONN))->SetCheck(iIsReconn);
		SetDlgItemInt(IDC_EDIT_RECONNINTERVAL,iReconnInterval);
		int iDbType = 1;
		pFile->ReadInitInfo(strAppPath,"database","dbtype",strTemp,iDbType,1);
		pFile->CheckInitValue(iDbType,1,4);
		switch (iDbType)
		{
		case 1:
			{
				OnBnClickedRadioMssql();
				break;
			}
		case 2:
			{
				OnBnClickedRadioOracle();
				break;
			}
		case 3:
			{
				OnBnClickedRadioMysql();
				break;
			}
		case 4:
			{
				OnBnClickedRadioAccess();
				break;
			}
		}
		//字符集
		CString strCharSet = "";
		pFile->ReadInitInfo(strAppPath,"database","charset",strCharSet,iTemp,0);
		SetDlgItemText(IDC_EDIT_DBCHARSET,strCharSet);

		int iDbPort = 1433;
		pFile->ReadInitInfo(strAppPath,"database","dbport",strTemp,iDbPort,1);
		pFile->CheckInitValue(iDbPort,0,100000000);
		SetDlgItemInt(IDC_EDIT_DBASEPORT,iDbPort);
		OnBnClickedCheckReconn();
		OnBnClickedCheckDiffmobile();
		int iServerType = 0;
		int iReportFlag = 0;
		pFile->ReadInitInfo(strAppPath,"Server","type",strTemp,iServerType,1);
		pFile->CheckInitValue(iServerType,0,1);
		pFile->ReadInitInfo(strAppPath,"Report","flag",strTemp,iReportFlag,1);
		pFile->CheckInitValue(iReportFlag,0,1);
		if (iServerType == 0)	//kexintong
		{
			OnBnClickedRadioKexintong();
		}
		else					//si
		{
			OnBnClickedRadioSi();
		}
		((CButton*)GetDlgItem(IDC_CHECK_NEEDRPT))->SetCheck(iReportFlag);

		//读取通道号
		strAppPath = strAppPathEx + SMEIDLL_DLL;
		CString strSpgate = "";
		pFile->ReadInitInfo(strAppPath,"Port","num",strSpgate,iTemp,0);
		SetDlgItemText(IDC_EDIT_SPGATE,strSpgate);
		if(pFile->IsAutoRun(g_strServiceName))
		{
			((CButton*)GetDlgItem(IDC_CHECK_SYSRUN))->SetCheck(TRUE);
		}
		else
		{
			((CButton*)GetDlgItem(IDC_CHECK_SYSRUN))->SetCheck(FALSE);
		}
		
	}
	else
	{
		//AddToDispListShow("获取配置文件类指针出错");
	}	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//更新配置
void CDbGateAllSet::OnBnClickedOk()
{
	CString dbServer="";
	CString dbName="";
	CString dbUser="";
	CString dbPwd="";

	CString strAccount="";
	CString strAccPwd="";

	int iSendBufLen;
	int iSendInterval;
	int iBufNullWaitTime;

	int iAddNullWaitTime;

	CString strServerIP="";
	int iServerPort;

	int iSpliterLen;
	int iUseSubPort = 0;
	CString strMobileList="";
	CString strUnicomList="";
	CString strTelecomList="";
	//数据库信息
	GetDlgItemText(IDC_EDIT_DBASESERVER,dbServer);
	GetDlgItemText(IDC_EDIT_DBNAME,dbName);
	GetDlgItemText(IDC_EDIT_DBASEUID,dbUser);
	GetDlgItemText(IDC_EDIT_DBASEPWD,dbPwd);

	//用户账户
	GetDlgItemText(IDC_EDIT_ACCOUNT,strAccount);
	GetDlgItemText(IDC_EDIT_ACCPWD,strAccPwd);

	//缓冲区
	iSendBufLen = GetDlgItemInt(IDC_EDIT_BUFFLEN);

	//发送线程
	iSendInterval = GetDlgItemInt(IDC_EDIT_SENDINTERVAL);
	iBufNullWaitTime = GetDlgItemInt(IDC_EDIT_SENDNULLWAIT);

	//添加缓冲线程
//	iAddBufInterval = GetDlgItemInt(IDC_EDIT_ADDINTERVAL);
	iAddNullWaitTime = GetDlgItemInt(IDC_EDIT_ADDNULLWAIT);

	//平台IP及端口
	GetDlgItemText(IDC_EDIT_SERVERIP,strServerIP);
	iServerPort = GetDlgItemInt(IDC_EDIT_SERVERPORT);

	//拆分长度
	iSpliterLen = GetDlgItemInt(IDC_EDIT_SPLITELEN);	
	
	GetDlgItemText(IDC_EDIT_MOBILE,strMobileList);
	GetDlgItemText(IDC_EDIT_UNICOM,strUnicomList);
	GetDlgItemText(IDC_EDIT_TELECOM,strTelecomList);

	//添加验证
	if ( dbServer == "" || dbName == "" || dbUser =="" || dbPwd == "")
	{
		MessageBox( "请填写完整的数据库信息","警告" );
		return;
	}

	if ( strAccount == "" || strAccPwd == "" )
	{
		MessageBox( "请填写完整的用户信息","警告" );
		return;
	}
	if ( iSpliterLen > 1000 || iSpliterLen < 1 )
	{
		MessageBox( "请正确配置信息拆分长度","警告" );
		return;		
	}
	int iAutoRun = 0;
	int iErrorStop = 0;
	iAutoRun = ((CButton *)GetDlgItem(IDC_CHECK_AUTORUN))->GetCheck();
	m_iUseSubPort = ((CButton *)GetDlgItem(IDC_CHECK_USESUBPORT))->GetCheck();
	m_iIsRecvMo = ((CButton *)GetDlgItem(IDC_CHECK_RECVMO))->GetCheck();
	m_iIsRecvReport = ((CButton *)GetDlgItem(IDC_CHECK_RECVREPORT))->GetCheck();
	iErrorStop = GetDlgItemInt(IDC_EDIT_ERRORSTOPTIME);		
	if ( iErrorStop < 1 )
	{
		iErrorStop = 1;
	}
	if ( iErrorStop > 5000 )
	{
		iErrorStop = 5000;
	}	
	m_iPhoneSplitLen = GetDlgItemInt(IDC_EDIT_PHONESPLITELEN);
	if ( m_iPhoneSplitLen > 1000 || m_iPhoneSplitLen < 1)
	{
		MessageBox("每包号码个数不正确(1000以内)","警告");
		return;
	}

	CString strStart="";
	CString strEnd="";
	GetDlgItemText(IDC_EDIT_TIMEREADSTART,strStart);
	GetDlgItemText(IDC_EDIT_TIMEREADEND,strEnd);
	if ( CheckSendTime(strStart,strEnd ) == 1 )
	{
		MessageBox("运行时间段设置格式错误","警告");
		return;
	}
	int iRecvmoInterval = 0;
	int iRecvreportInterval = 0;
	iRecvmoInterval = GetDlgItemInt(IDC_EDIT_RECVMOINTERVAL);
	iRecvreportInterval = GetDlgItemInt(IDC_EDIT_RECVREPORTINTERVAL);
	if (iRecvmoInterval == 0)
	{
		iRecvmoInterval = 11;
	}
	if (iRecvreportInterval==0)
	{
		iRecvreportInterval = 11;
	}
	g_iIsNeedToLog = ((CButton *)GetDlgItem(IDC_CHECK_LOG))->GetCheck();
	int iDiffMobile = ((CButton *)GetDlgItem(IDC_CHECK_DIFFMOBILE))->GetCheck();
	int iCheckKey = ((CButton *)GetDlgItem(IDC_CHECK_KEYCHECK))->GetCheck();
	
	CString strPhoneFilePath = "";
	GetDlgItemText(IDC_EDIT_PHONEFILEPATH,strPhoneFilePath);
	CString strWindowName = "";
	GetDlgItemText(IDC_EDIT_WINDOWNAME,strWindowName);

    int iIsReconn = ((CButton *)GetDlgItem(IDC_CHECK_RECONN))->GetCheck();
	int iReconnInterval = GetDlgItemInt(IDC_EDIT_RECONNINTERVAL);
	if (iIsReconn != 0 && iIsReconn != 1)
	{
		iIsReconn = 0;
	}
	if (iReconnInterval < 100)
	{
		iReconnInterval=100;
	}
	int iDbType = 0;//数据库类型
	if(((CButton*)GetDlgItem(IDC_RADIO_MSSQL))->GetCheck()==1)
	{
		iDbType = 1;
	}
	else if(((CButton*)GetDlgItem(IDC_RADIO_ORACLE))->GetCheck()==1)
	{
		iDbType = 2;
	}
	else if(((CButton*)GetDlgItem(IDC_RADIO_MYSQL))->GetCheck()==1)
	{
		iDbType = 3;
	}
	else
	{
		iDbType = 4;
	}
	int iDbPort = GetDlgItemInt(IDC_EDIT_DBASEPORT);
	int iTestConInterval = GetDlgItemInt(IDC_EDIT_TESTCONNINTERVAL);
	CString strCharSet="";
	GetDlgItemText(IDC_EDIT_DBCHARSET,strCharSet);
	int iSplitlengthUnicom = GetDlgItemInt(IDC_EDIT_SPLITELEN_UNICOM);	//联通
	int iSplitChinaNum = GetDlgItemInt(IDC_EDIT_SPLITELEN_CHINANUM);	//小灵通
	int iSplitChinacom = GetDlgItemInt(IDC_EDIT_SPLITELEN_CHINACOM);	//电信手机
	
	CString strAppPathEx = "";
	CUserFile *pFile = NULL;
	pFile = CUserFile::GetInstance();
	if (pFile)
	{
		CString strAppPath = "";
		pFile->GetAppPath(strAppPath);
		strAppPathEx = strAppPath;
		strAppPath += MWGATEINITFILE;
		dbUser = pFile->StrEncode(dbUser);
		dbPwd = pFile->StrEncode(dbPwd);
		//数据库配置
		pFile->WriteInitInfo(strAppPath,"database","dbtype","",iDbType,1);	
		pFile->WriteInitInfo(strAppPath,"database","dbserver",dbServer,0,0);
		pFile->WriteInitInfo(strAppPath,"database","dbname",dbName,0,0);
		pFile->WriteInitInfo(strAppPath,"database","dbuser",dbUser,0,0);
		pFile->WriteInitInfo(strAppPath,"database","dbpwd",dbPwd,0,0);
		pFile->WriteInitInfo(strAppPath,"database","dbport","",iDbPort,1);	
		strAccount = pFile->StrEncode(strAccount);
		strAccPwd = pFile->StrEncode(strAccPwd);
		//用户信息
		pFile->WriteInitInfo(strAppPath,"Userinfo","Account",strAccount,0,0);
		pFile->WriteInitInfo(strAppPath,"Userinfo","Pwd",strAccPwd,0,0);
		//服务器设置
		pFile->WriteInitInfo(strAppPath,"Server","Ip",strServerIP,0,0);
		pFile->WriteInitInfo(strAppPath,"Server","Port","",iServerPort,1);
		//号段
		pFile->WriteInitInfo(strAppPath,"NumberHead","Mobile",strMobileList,0,0);
		pFile->WriteInitInfo(strAppPath,"NumberHead","Unicom",strUnicomList,0,0);
		pFile->WriteInitInfo(strAppPath,"NumberHead","Telecom",strTelecomList,0,0);
		pFile->WriteInitInfo(strAppPath,"NumberHead","Differ","",iDiffMobile,1);		
		//发送线程配置
		pFile->WriteInitInfo(strAppPath,"Sendthread","Interval","",iSendInterval,1);
		pFile->WriteInitInfo(strAppPath,"Sendthread","Bufnullwait","",iBufNullWaitTime,1);
		pFile->WriteInitInfo(strAppPath,"Sendthread","Buflen","",iSendBufLen,1);
		pFile->WriteInitInfo(strAppPath,"Sendthread","Splitlength","",iSpliterLen,1);
		pFile->WriteInitInfo(strAppPath,"Sendthread","Splitlength","",iSpliterLen,1);
		pFile->WriteInitInfo(strAppPath,"Sendthread","SplitlengthUnicom","",iSplitlengthUnicom,1);
		pFile->WriteInitInfo(strAppPath,"Sendthread","SplitlengthChinaNum","",iSplitChinaNum,1);
		pFile->WriteInitInfo(strAppPath,"Sendthread","SplitlengthChinaCom","",iSplitChinacom,1);

		//添加缓冲区配置
		pFile->WriteInitInfo(strAppPath,"Addbuf","NoMessage","",iAddNullWaitTime,1);
	
		//自动运行
		pFile->WriteInitInfo(strAppPath,"AutoRun","AutoRun","",iAutoRun,1);
		pFile->WriteInitInfo(strAppPath,"StopTime","Error","",iErrorStop,1);

		//接收上行和状态报告配置
		pFile->WriteInitInfo(strAppPath,"Recv","RecvMo","",m_iIsRecvMo,1);
		pFile->WriteInitInfo(strAppPath,"Recv","RecvReport","",m_iIsRecvReport,1);
		pFile->WriteInitInfo(strAppPath,"Recv","RecvMoInterval","",iRecvmoInterval,1);
		pFile->WriteInitInfo(strAppPath,"Recv","RecvReportInterval","",iRecvreportInterval,1);
		//子端口
		pFile->WriteInitInfo(strAppPath,"SubPort","type","",m_iUseSubPort,1);
		//号码打包数量
		pFile->WriteInitInfo(strAppPath,"PhoneSplit","PhoneSplit","",m_iPhoneSplitLen,1);
		//运行时间段
		pFile->WriteInitInfo(strAppPath,"runtime","time",strStart+"--"+strEnd,0,0);
		//日志
		pFile->WriteInitInfo(strAppPath,"log","type","",g_iIsNeedToLog,1);
		//客户端检测非法关键词
		pFile->WriteInitInfo(strAppPath,"keycheck","type","",iCheckKey,1);
		//号码文件路径
		pFile->WriteInitInfo(strAppPath,"phonefile","path",strPhoneFilePath,0,0);
		//窗口标题
		pFile->WriteInitInfo(strAppPath,"window","name",strWindowName,0,0);
		//定时重连
        pFile->WriteInitInfo(strAppPath,"Reconn","type","",iIsReconn,1);
		pFile->WriteInitInfo(strAppPath,"Reconn","interval","",iReconnInterval,1);	
		pFile->CheckInitValue(iTestConInterval,60,150);
		pFile->WriteInitInfo(strAppPath,"Testconn","interval","",iTestConInterval,1);
		pFile->WriteInitInfo(strAppPath,"database","charset",strCharSet,0,0);
		
		CString strSpgate = "";
		int iRptFlag = 0;
		int iServerType = 0;
		GetDlgItemText(IDC_EDIT_SPGATE,strSpgate);
		if(((CButton*)GetDlgItem(IDC_RADIO_SI))->GetCheck()==1)
		{
			iServerType = 1;
		}
		if(((CButton*)GetDlgItem(IDC_CHECK_NEEDRPT))->GetCheck()==1)
		{
			iRptFlag = 1;
		}
		pFile->WriteInitInfo(strAppPath,"Server","type","",iServerType,1);
		pFile->WriteInitInfo(strAppPath,"Report","flag","",iRptFlag,1);	
		strAppPath = strAppPathEx + SMEIDLL_DLL;
		pFile->WriteInitInfo(strAppPath,"Port","num",strSpgate,0,0);	

		//处理系统自动启动项
		if(((CButton*)GetDlgItem(IDC_CHECK_SYSRUN))->GetCheck()==1)
		{		
			pFile->SetAutoRun(g_strServiceName);	
		}
		else
		{	
			pFile->DeleteAutoRun(g_strServiceName);
		}
		//如果窗口名变化了
		if (m_strOldName != strWindowName)
		{
			AfxGetMainWnd()->SetWindowText(strWindowName+" "+g_strVirsion);
		}
	}
	((CMainFrame*)GetParent())->ReSetParams();	
	OnOK();
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//数据库交互配置对话框显示
void CDbGateAllSet::OnBnClickedBtnMore()
{
	CSetBase setBase;
	setBase.DoModal();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CDbGateAllSet::OnBnClickedCheckRecvmo()
{
	if(((CButton *)GetDlgItem(IDC_CHECK_RECVMO))->GetCheck()==1)
	{
		GetDlgItem(IDC_EDIT_RECVMOINTERVAL)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_RECVMOINTERVAL)->EnableWindow(FALSE);
	}
	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CDbGateAllSet::OnBnClickedCheckRecvreport()
{
	if(((CButton *)GetDlgItem(IDC_CHECK_RECVREPORT))->GetCheck()==1)
	{
		GetDlgItem(IDC_EDIT_RECVREPORTINTERVAL)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_RECVREPORTINTERVAL)->EnableWindow(FALSE);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CDbGateAllSet::OnBnClickedCheckReconn()
{
	if(((CButton *)GetDlgItem(IDC_CHECK_RECONN))->GetCheck()==1)
	{
		GetDlgItem(IDC_EDIT_RECONNINTERVAL)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_RECONNINTERVAL)->EnableWindow(FALSE);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//是否区分号段的复选框按钮点击
void CDbGateAllSet::OnBnClickedCheckDiffmobile()
{
	if (((CButton*)GetDlgItem(IDC_CHECK_DIFFMOBILE))->GetCheck()==1)
	{
		GetDlgItem(IDC_EDIT_MOBILE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TELECOM)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_UNICOM)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_SPLITELEN_CHINACOM)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SPLITELEN_CHINANUM)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SPLITELEN_UNICOM)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_MOBILE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TELECOM)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_UNICOM)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_SPLITELEN_CHINACOM)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SPLITELEN_CHINANUM)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SPLITELEN_UNICOM)->EnableWindow(FALSE);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CDbGateAllSet::OnBnClickedRadioMssql()
{
	((CButton*)GetDlgItem(IDC_RADIO_ACCESS))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_ORACLE))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_MYSQL))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_MSSQL))->SetCheck(TRUE);
	GetDlgItem(IDC_EDIT_DBASEUID)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_DBNAME)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_DBASEPORT)->EnableWindow(FALSE);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CDbGateAllSet::OnBnClickedRadioOracle()
{
	((CButton*)GetDlgItem(IDC_RADIO_ACCESS))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_ORACLE))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_MYSQL))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_MSSQL))->SetCheck(FALSE);
	GetDlgItem(IDC_EDIT_DBASEUID)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_DBNAME)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_DBASEPORT)->EnableWindow(FALSE);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CDbGateAllSet::OnBnClickedRadioMysql()
{
	((CButton*)GetDlgItem(IDC_RADIO_ACCESS))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_ORACLE))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_MYSQL))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_MSSQL))->SetCheck(FALSE);
	GetDlgItem(IDC_EDIT_DBASEUID)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_DBNAME)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_DBASEPORT)->EnableWindow(TRUE);
	//GetDlgItem(IDC_EDIT_DBCHARSET)->ShowWindow(SW_SHOW);
	//GetDlgItem(IDC_STATIC_DBCHARSET)->ShowWindow(SW_SHOW);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CDbGateAllSet::OnBnClickedRadioAccess()
{
	((CButton*)GetDlgItem(IDC_RADIO_ACCESS))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_ORACLE))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_MYSQL))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_MSSQL))->SetCheck(FALSE);
	GetDlgItem(IDC_EDIT_DBASEUID)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_DBNAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_DBASEPORT)->EnableWindow(FALSE);
	//GetDlgItem(IDC_EDIT_DBCHARSET)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_STATIC_DBCHARSET)->ShowWindow(SW_HIDE);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CDbGateAllSet::OnBnClickedRadioKexintong()
{
	GetDlgItem(IDC_EDIT_SPGATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_NEEDRPT)->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_SI))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_KEXINTONG))->SetCheck(TRUE);

	((CButton*)GetDlgItem(IDC_CHECK_RECONN))->SetCheck(TRUE);
	GetDlgItem(IDC_CHECK_RECONN)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_RECONNINTERVAL)->EnableWindow(TRUE);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CDbGateAllSet::OnBnClickedRadioSi()
{
	GetDlgItem(IDC_EDIT_SPGATE)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK_NEEDRPT)->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_SI))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_KEXINTONG))->SetCheck(FALSE);

	((CButton*)GetDlgItem(IDC_CHECK_RECONN))->SetCheck(FALSE);
	GetDlgItem(IDC_CHECK_RECONN)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_RECONNINTERVAL)->EnableWindow(FALSE);
}
