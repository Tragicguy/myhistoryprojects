#ifndef  _CWEBBASE_DEFINE__
#define  _CWEBBASE_DEFINE__
#include <windows.h>
#include <wininet.h>
#include <string>
#include "log.h"
#include "ProxySet.h"
#include <io.h>
using namespace std;
#pragma comment(lib,"wininet.lib")
#pragma comment(lib,"../WebBase/zlib/ZLibD.lib")

//extern log objLog;

//定义返回的宏
#define WM_CONNECT_FAILED -1
#define WM_OPENREQUEST_FAILED -2
#define WM_SENDREQUEST_FAILED -3
#define WM_QUERYINFO_FAILED -4
#define WM_RETCODE_ERROR -5
#define WM_READFILE_ERROR -6
#define WM_GENNEWBUFF_ERROR -7
//异常错误
#ifdef ERROR_INTERNET_CANNOT_CONNECT
#endif

#define SECURITY_IGNORE_ERROR_MASK  (INTERNET_FLAG_IGNORE_CERT_CN_INVALID   |  \
	INTERNET_FLAG_IGNORE_CERT_DATE_INVALID |  \
	SECURITY_FLAG_IGNORE_UNKNOWN_CA        |  \
	SECURITY_FLAG_IGNORE_REVOCATION    )

class CWebBase : public CProxySet
{
public:	
	CWebBase();
	~CWebBase(void);

	static void WebInit()
	{
		unsigned uParam = 1000;
		InternetSetOption(NULL, INTERNET_OPTION_MAX_CONNS_PER_1_0_SERVER, &uParam, sizeof(unsigned int));
		InternetSetOption(NULL, INTERNET_OPTION_MAX_CONNS_PER_SERVER, &uParam, sizeof(unsigned int));
	}
	int GetPage(const char* lpszObject,const char* lpszHeader = NULL,const char* lpszServer = NULL,int nPort = 0,bool bIsFile=false,const char* szFilePath=NULL);
	int PostPage(const char* lpszObject,const char* lpszData,const char* lpszHeader = NULL,const char* lpszServer = NULL,int nPort = 0,bool bIsFile=false,const char* szFilePath=NULL);
	int GetData(const char* lpszObject,const char* lpszHeader,const char* lpszServer,int nPort,const char * lpszVerb = "GET",const char* lpszData=NULL,bool bIsFile=false,const char* szFilePath=NULL);
	/************************************************************************/
	/* 设置、获取错误信息                                                   */
	/************************************************************************/
	const char * GetLastErrorInfo() { return m_strErrorInfo.c_str(); }
	char * GetContentInfo() { return (char *)m_strContent.c_str(); }
	//设置超时时间值 
	void SetTimeOutSeconnd( int nTimeOutSecond )  {	m_nTimeOutSecond = nTimeOutSecond; }
	//设置是HTTPS还是HTTP类型
	void SetHttpType( int nType = 1 ){ m_nHttpType = nType; }
	//设置默认的服务器地址
	void SetDefaultServer(const char *lpszServer,const int nPort = 80){ m_strServer = string(lpszServer); m_nPort = nPort;}	
	void SetNoCookie(bool bCookie){m_bNoCookie = bCookie;}
	void SetNotAutoRedirect(bool bNoRedirect){ m_bNoAutoRedirect = bNoRedirect; }
	string GetCookie(){return m_strCookie;}
	inline DWORD GenFlags();
	string GetHeader(){ return m_strHttpHeader; }
private:
	//设置代理
	BOOL SetProxy()
	{
		if (!IsEnableProxy()){	return TRUE; }
		GenProxyParam(m_ProxyParam);
		BOOL bRet = InternetSetOption(m_hInterSessin,INTERNET_OPTION_PROXY,(LPVOID)&m_ProxyParam,sizeof(m_ProxyParam));
		if (!bRet)
		{
			SetLastErrorInfo("INTERNET_OPTION_PROXY Error:%d",GetLastError());
		}
		return bRet;
	}
	void SetLastErrorInfo(const char* szFormat, ...);
	void SetTimeOut( );
	HINTERNET GetHttpConnectionEx(const char * lpszServer,int nPort);
	
	BOOL SendRequestEx(HINTERNET hRequest,const char* lpszHeader,const char* lpszPostData = NULL,bool bPostFlag = false);
	BOOL SetSessionCookie();
	int ConvertData(string strReponseHead,string &strError,char* szData, int nLen);
	void AddCookie(string &strHeader);
private:
	HINTERNET m_hInterSessin;
	string m_strErrorInfo;
	string m_strContent;
	string m_strHttpHeader;
	string m_strCookie;
	int m_nTimeOutSecond;
	int m_nHttpType;	//0表示是HTTPS，1表示HTTP
	string m_strServer;	//默认的服务器地址
	int m_nPort;	//默认端口
	INTERNET_PROXY_INFO m_ProxyParam;
	bool m_bNoCookie;
	bool m_bNoAutoRedirect;
	char* m_pDataBuf;
	int m_nDataLen;
	int m_nBufLen;
	int m_nNewTimes;

};

#endif
