#include "stdafx.h"
#include "WebBase.h"
#include "log.h"
#include "zlib\gzdecompress.h"
#include "../GoodBase/Function.h"
#include <algorithm>
#pragma warning(disable:4267)
#define BASE_BUF_SIZE 10240	
//log g_objLog;
CWebBase::CWebBase()
{
	m_strErrorInfo.clear();
	m_strContent.clear();
	m_strServer.clear();
	m_nTimeOutSecond = 0;
	m_nHttpType = 1;	
	m_nPort = 80;
	m_nTimeOutSecond = 50;
	m_bNoCookie = false;
	m_bNoAutoRedirect = false;
	m_hInterSessin = InternetOpen("YuTone_Http_Post", 
		INTERNET_OPEN_TYPE_DIRECT, 
		NULL, 
		NULL, 
		INTERNET_FLAG_NO_CACHE_WRITE);
	if ( NULL == m_hInterSessin )
	{
		SetLastErrorInfo("InternetOpen Error:%d", GetLastError());
	}
	else
	{
		SetTimeOut();
	}	
	m_nNewTimes = 1;
	m_nBufLen = m_nNewTimes * BASE_BUF_SIZE;
	m_pDataBuf = NULL;
	m_pDataBuf=static_cast<char*>( malloc(m_nBufLen));
	m_nDataLen = 0;
	//g_objLog.SetLogPath("c:\\log.txt");
}

CWebBase::~CWebBase(void)
{
	InternetCloseHandle(m_hInterSessin);
	if (m_pDataBuf)
	{
		free(m_pDataBuf);
	}
}
/*
int CWebBase::DownLoadFile(const char* lpszUrl,const char* lpszSavePath)
{
	return 0;
}
*/
HINTERNET CWebBase::GetHttpConnectionEx(const char * lpszServer,int nPort)
{
	if (lpszServer == NULL)
	{
		lpszServer = m_strServer.c_str();
	}
	if (nPort == 0)
	{
		nPort = m_nPort;
	}
	SetProxy();
	return InternetConnect(m_hInterSessin,
		lpszServer,
		nPort,
		NULL,
		NULL,
		INTERNET_SERVICE_HTTP, 
		INTERNET_FLAG_NO_CACHE_WRITE,
		NULL);
}

// void CWebBase::CloseHttpConnection(HINTERNET &hConnect)
// {
// 	try
// 	{
// 		if (hConnect)
// 		{
// 			InternetCloseHandle(hConnect);
// 		}
// 	}
// 	catch (...)
// 	{
// 	}
// }
/************************************************************************/
/* 发送数据至WEB服务器                                                  */
/************************************************************************/
BOOL CWebBase::SendRequestEx(HINTERNET hRequest,const char* lpszHeader,const char* lpszPostData,bool bPostFlag)
{
	BOOL bRet = FALSE;
	char *pData = NULL;
	if (bPostFlag)
	{
		pData = (char *)lpszPostData;
	}
	return HttpSendRequest(hRequest,lpszHeader,strlen(lpszHeader),pData,pData == NULL?0:strlen(pData));
}
/************************************************************************/
/* HTTP通信                                                             */
/************************************************************************/
int CWebBase::GetData(const char* lpszObject,const char* lpszHeader,const char* lpszServer,int nPort,const char * lpszVerb,const char* lpszData,bool bIsFile,const char* szFilePath)
{
	time_t t = time(NULL);
	//获取HTTP连接
	HINTERNET hConnect = GetHttpConnectionEx(lpszServer,nPort);
	if (hConnect == NULL)
	{
		SetLastErrorInfo("InternetConnect Error:%d,TimeSpan:%d", GetLastError(),time(NULL)-t);
		return WM_CONNECT_FAILED;
	}
	DWORD dwFlag = GenFlags();
	t = time(NULL);
	//发送HTTP请求
	HINTERNET hRequest = HttpOpenRequest(hConnect,lpszVerb,lpszObject,NULL,NULL,NULL,dwFlag,NULL);
	if (NULL == hRequest)
	{		
		SetLastErrorInfo("HttpOpenRequest %s Error:%d,TimeSpan:%d",lpszVerb, GetLastError(),time(NULL)-t);
		InternetCloseHandle(hConnect);
		return WM_OPENREQUEST_FAILED;
	}
	t = time(NULL);
	DWORD dwFlagsEx;
	DWORD dwBuffLen = sizeof(hRequest);
	InternetQueryOptionA(hRequest, INTERNET_OPTION_SECURITY_FLAGS, (LPVOID)&dwFlagsEx, &dwBuffLen);
	dwFlagsEx |= SECURITY_IGNORE_ERROR_MASK;
	InternetSetOptionA(hRequest, INTERNET_OPTION_SECURITY_FLAGS, &dwFlagsEx, sizeof(dwFlagsEx) );

	string strHeader = string(lpszHeader);
	AddCookie(strHeader);
	BOOL bRet = SendRequestEx(hRequest, strHeader.c_str(), lpszData, lpszData!=NULL);
	if (!bRet)
	{
		SetLastErrorInfo("HttpSendRequest Error:%d,TimeSpan:%d", GetLastError(),time(NULL)-t);
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		return WM_SENDREQUEST_FAILED;
	}
	t = time(NULL);
	DWORD dwStatus = 0;
	DWORD dwStatusSize = sizeof(dwStatus);
	bRet = HttpQueryInfo(hRequest,HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,&dwStatus,&dwStatusSize,NULL);
	if (!bRet)
	{
		SetLastErrorInfo("HttpQueryInfo Error:%d,TimeSpan:%d", GetLastError(),time(NULL)-t);
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		return WM_QUERYINFO_FAILED;
	}
	if ( !(dwStatus == HTTP_STATUS_OK ||dwStatus == HTTP_STATUS_MOVED ||
		dwStatus == HTTP_STATUS_REDIRECT ||	dwStatus == HTTP_STATUS_REDIRECT_METHOD) )
	{
		SetLastErrorInfo("RetStatusCode Error,dwStatus:%d,LastError:%d,TimeSpan:%d",dwStatus,GetLastError(),time(NULL)-t);
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		return WM_RETCODE_ERROR;
	}
	char szHttpHeader[10240] = {0};
	DWORD dHeaderLen = sizeof(szHttpHeader);
	bRet = HttpQueryInfo(hRequest,HTTP_QUERY_RAW_HEADERS_CRLF,szHttpHeader,&dHeaderLen,NULL);
	if (!bRet)
	{
		SetLastErrorInfo("HttpQueryInfo Error:%d,TimeSpan:%d", GetLastError(),time(NULL)-t);
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		return WM_QUERYINFO_FAILED;
	}

	m_strHttpHeader = string(szHttpHeader);
	SetSessionCookie();
	t = time(NULL);
	m_strContent.clear();
	DWORD dwReadLen = 0;
	DWORD dwCurrLen = 0;	
	char szBuf[1024] = {0};
	if (szFilePath != NULL && _access(szFilePath,0) == 0)
	{
		remove(szFilePath);
	}
	memset(m_pDataBuf, 0, m_nBufLen);
	m_nDataLen = 0;
	while (true)
	{
		if (bIsFile)
		{
			bRet = InternetReadFile(hRequest,szBuf,1,&dwCurrLen);
			if ( !bRet || dwCurrLen <= 0)
			{
				break;
			}
			fstream fsFile(szFilePath, ios_base::out | ios_base::app | ios_base::binary);
			fsFile << szBuf[0];
			fsFile.flush();
			fsFile.close();
		}
		else
		{
			ZeroMemory(szBuf,1024);
			bRet = InternetReadFile(hRequest,szBuf,1023,&dwCurrLen);
			if (!bRet)
			{
				SetLastErrorInfo("InternetReadFile Error:%d,TimeSpan:%d", GetLastError(),time(NULL)-t);
				InternetCloseHandle(hRequest);
				InternetCloseHandle(hConnect);
				return WM_READFILE_ERROR;
			}
			if (dwCurrLen == 0)
			{
				break;
			}
			if (m_nDataLen + dwCurrLen > m_nBufLen -1)
			{
				char *pTemp = m_pDataBuf;
				m_nNewTimes ++;
				m_nBufLen = m_nNewTimes * BASE_BUF_SIZE;
				m_pDataBuf = NULL;
				m_pDataBuf=static_cast<char*>( malloc(m_nBufLen));
				if (m_pDataBuf == NULL)
				{
					free(pTemp);
					SetLastErrorInfo("WM_GENNEWBUFF_ERROR Error:%d,TimeSpan:%d", GetLastError(),time(NULL)-t);
					return WM_GENNEWBUFF_ERROR;
				}
				ZeroMemory(m_pDataBuf, m_nBufLen);
				memcpy(m_pDataBuf, pTemp, m_nDataLen);
				free(pTemp);
			}
			memcpy(m_pDataBuf+m_nDataLen, szBuf, dwCurrLen);
			m_nDataLen += dwCurrLen;
		}		
	}
	return ConvertData(m_strHttpHeader, m_strErrorInfo, m_pDataBuf, m_nDataLen);
}
/************************************************************************/
/* GET方式获取数据                                                      */
/************************************************************************/
int CWebBase::GetPage(const char* lpszObject,const char* lpszHeader,const char* lpszServer,int nPort,bool bIsFile,const char* szFilePath)
{
	return GetData(lpszObject,lpszHeader,lpszServer,nPort,"GET",NULL,bIsFile,szFilePath);
}

/************************************************************************/
/* POST方式获取数据                                                     */
/************************************************************************/
int CWebBase::PostPage(const char* lpszObject,const char* lpszData,const char* lpszHeader,const char* lpszServer,int nPort,bool bIsFile,const char* szFilePath)
{
	return GetData(lpszObject,lpszHeader,lpszServer,nPort,"POST",lpszData,bIsFile,szFilePath);
}

/************************************************************************/
/* 对InternetSession设置超时                                            */
/************************************************************************/
void CWebBase::SetTimeOut( )
{
	if (m_nTimeOutSecond < 10)
	{
		m_nTimeOutSecond = 10;
	}
	DWORD dwTime = 3000;//3秒的连接超时
	InternetSetOption(m_hInterSessin,INTERNET_OPTION_CONNECT_TIMEOUT,(LPVOID)&dwTime,sizeof(dwTime));
	//3秒的发送超时
	InternetSetOption(m_hInterSessin,INTERNET_OPTION_SEND_TIMEOUT,(LPVOID)&dwTime,sizeof(dwTime));
	dwTime = m_nTimeOutSecond * 1500;	//接收超时
	InternetSetOption(m_hInterSessin,INTERNET_OPTION_RECEIVE_TIMEOUT,(LPVOID)&dwTime,sizeof(dwTime));
	dwTime = 3000;//3秒的数据发送超时
	InternetSetOption(m_hInterSessin,INTERNET_OPTION_DATA_SEND_TIMEOUT,(LPVOID)&dwTime,sizeof(dwTime));
	dwTime = m_nTimeOutSecond * 1500;	//接收超时
	InternetSetOption(m_hInterSessin,INTERNET_OPTION_DATA_RECEIVE_TIMEOUT,(LPVOID)&dwTime,sizeof(dwTime));
	dwTime = 2;//重试次数
	InternetSetOption(m_hInterSessin,INTERNET_OPTION_CONNECT_RETRIES,(LPVOID)&dwTime,sizeof(dwTime));
}



/************************************************************************/
/*                                                                      */
/************************************************************************/
DWORD CWebBase::GenFlags()
{
	DWORD dwFlags = 0;
	dwFlags = INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_RELOAD;
	if (m_bNoAutoRedirect)
	{
		dwFlags |= INTERNET_FLAG_NO_AUTO_REDIRECT;
	}
	if (m_nHttpType == 0)			//如果是HTTPS协议
	{
		dwFlags = dwFlags |
			INTERNET_FLAG_SECURE |
			INTERNET_FLAG_IGNORE_CERT_CN_INVALID |
			INTERNET_FLAG_IGNORE_CERT_DATE_INVALID | SECURITY_IGNORE_ERROR_MASK ;
	}
	if(m_bNoCookie)
	{
		dwFlags |= INTERNET_FLAG_NO_COOKIES;
	}
	return dwFlags;
}
/************************************************************************/
/* 设置错误信息                                                         */
/************************************************************************/
void CWebBase::SetLastErrorInfo(const char* szFormat, ...) 
{ 		
	char szBuf[2048] = {0};
	va_list arg_list;
	va_start (arg_list, szFormat);
	::vsnprintf_s(szBuf, sizeof(szBuf) - 1, _TRUNCATE, szFormat, arg_list);
	m_strErrorInfo = string(szBuf); 
	//g_objLog.loginfo(szBuf);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CWebBase::SetSessionCookie()
{
	if(!m_bNoCookie) return TRUE;
	string strBefore = "Set-Cookie:";
	int iPos1 = m_strHttpHeader.find(strBefore);
	while(iPos1 >= 0)
	{
		int iPos2 = m_strHttpHeader.find("\r\n",iPos1);
		if (iPos2 < iPos1)
		{
			return FALSE;
		}
		string strTest = m_strHttpHeader.substr(iPos1+strBefore.length() + 1, iPos2 - iPos1 - strBefore.length() - 1);
		m_strCookie.append(strTest);
		m_strCookie.append("; ");
		iPos1 = m_strHttpHeader.find(strBefore, iPos2);
	}
	//m_strCookie.replace("\r\n", "");
	return TRUE;
}

int CWebBase::ConvertData(string strReponseHead,string &strError,char* szData, int nLen)
{
	if (nLen < 1)
		return 0;
	if(strReponseHead.find("gzip" ) != -1) 
	{
		if( gzuncompress(szData, nLen, m_strContent ) != 0 )
		{
			strError = "解压失败";
			return 5;
		}
	}
	else
	{
		m_strContent = szData;
	}	
	//transform(strReponseHead.begin(), strReponseHead.end(), strReponseHead.begin(), ::tolower); //将大写的都转换成小写
	//if( strReponseHead.find("utf-8") != -1 && strReponseHead.find("jpeg" ) == -1 )
	{                        
		CFunction::ConvertUtf8ToGBK( m_strContent );
	}
	return 0;
}

void CWebBase::AddCookie(string &strHeader)
{
	if (!m_bNoCookie) return;
	if (m_strCookie.size() < 1) return;
	strHeader.append("\r\nCookie:" + m_strCookie);
}