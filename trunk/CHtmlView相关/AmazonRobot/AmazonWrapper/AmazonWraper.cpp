#include "StdAfx.h"
#include "AmazonWraper.h"

CAmazonWraper::CAmazonWraper(void)
{
	GetAppPath(m_strAppPath);
}

CAmazonWraper::~CAmazonWraper(void)
{
}

int CAmazonWraper::GetAppPath(CString &strAppPath)
{
	int iRet = -3;
	try
	{
		TCHAR szBuff[_MAX_PATH];
		VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szBuff, _MAX_PATH));
		strAppPath.Format("%s",szBuff);
		int pos=strAppPath.ReverseFind('\\');
		strAppPath=strAppPath.Left(pos+1);		
		iRet = 0;
	}
	catch (...)
	{
		
	}
	return iRet;	
}
void CAmazonWraper::WriteParams(CString strPath,AmazonOrderStru &objStr)
{
	CString strNode = "set";
	::WritePrivateProfileString(strNode,"strGoodNos", objStr.strGoodNos, strPath);
	::WritePrivateProfileString(strNode,"strUserID", objStr.strUserID, strPath);
	::WritePrivateProfileString(strNode,"strPwd", objStr.strPwd, strPath);
	::WritePrivateProfileString(strNode,"strProvince", objStr.strProvince, strPath);
	::WritePrivateProfileString(strNode,"strCity", objStr.strCity, strPath);
	::WritePrivateProfileString(strNode,"strTown", objStr.strTown, strPath);
	::WritePrivateProfileString(strNode,"strRecver", objStr.strRecver, strPath);
	::WritePrivateProfileString(strNode,"strAddr", objStr.strAddr, strPath);
	::WritePrivateProfileString(strNode,"strPostCode", objStr.strPostCode, strPath);
	::WritePrivateProfileString(strNode,"strPhone", objStr.strPhone, strPath);
	::WritePrivateProfileString(strNode,"strBillHeader", objStr.strBillHeader, strPath);
	::WritePrivateProfileString(strNode,"strBillContent", objStr.strBillContent, strPath);
	::WritePrivateProfileString(strNode,"strOrderNo", objStr.strOrderNo, strPath);
	CString strTemp = "";
	strTemp.Format("%d",objStr.eSendType);
	::WritePrivateProfileString(strNode,"eSendType", strTemp, strPath);
	strTemp.Format("%d",objStr.bBill);
	::WritePrivateProfileString(strNode,"bBill", strTemp, strPath);
	strTemp.Format("%d",objStr.iShowFlag);
	::WritePrivateProfileString(strNode,"iShowFlag", strTemp, strPath);
	strTemp.Format("%d",objStr.iDebug);
	::WritePrivateProfileString(strNode,"iDebug", strTemp, strPath);
	strTemp.Format("%d",objStr.iOpFlag);
	::WritePrivateProfileString(strNode,"iOpFlag", strTemp, strPath);
	strTemp.Format("%d",objStr.eQueryType );
	::WritePrivateProfileString(strNode,"eQueryType", strTemp, strPath);	
	strTemp.Format("%d",objStr.bCanNormalSend );
	::WritePrivateProfileString(strNode,"bCanNormalSend", strTemp, strPath);	
}

int CAmazonWraper::Order(AmazonOrderStru& objStr)
{
	objStr.iOpFlag = 2;
	return Operate(objStr);
}

int CAmazonWraper::Query(AmazonOrderStru& objStr)
{
	objStr.iOpFlag = 1;
	return Operate(objStr);
}

int CAmazonWraper::ChangePayType(AmazonOrderStru& objStr)
{
	objStr.iOpFlag = 3;
	return Operate(objStr);
}


int CAmazonWraper::Operate(AmazonOrderStru& objStr)
{
	//得到本程序所在路径
	CString strIniPath = m_strAppPath + AMAZON_CONFIG_FILE;
	WriteParams(strIniPath,objStr);
	//启动下单程序
	STARTUPINFO vStartupInfo;
	memset(&vStartupInfo, 0, sizeof(vStartupInfo));
	vStartupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES; //Enable the wShowWindow
	vStartupInfo.wShowWindow = SW_HIDE; //Hide the consol window
	vStartupInfo.cb = sizeof(STARTUPINFO);
	char commandBuf[512];
	sprintf(commandBuf, "\"%s%s\" %s", m_strAppPath,AMAZON_ROBOTNAME,strIniPath);
	PROCESS_INFORMATION vProcessInfo;	
	if (::CreateProcess(NULL,
		commandBuf,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		m_strAppPath,
		&vStartupInfo,
		&vProcessInfo))
	{

		CloseHandle(vProcessInfo.hThread);
		int iCount = 0;
		while ( iCount++ < 360 )	//两分钟超时时间
		{
			DWORD dwExitCode = 0;
			if (vProcessInfo.hProcess != NULL)
			{
				if (GetExitCodeProcess(vProcessInfo.hProcess, &dwExitCode))
				{
					if (dwExitCode != STILL_ACTIVE)
					{
						TerminateProcess(vProcessInfo.hProcess, 0 );
						return dwExitCode;
					}
				}
			}
			Sleep(500);
		}
		TerminateProcess(vProcessInfo.hProcess, 0 );
	}
	return USER_TIMEOUT;	//成功
}