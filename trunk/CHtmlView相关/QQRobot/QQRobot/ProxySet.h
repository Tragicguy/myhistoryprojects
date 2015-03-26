#pragma once
#include <string>
using namespace std;
class CProxySet
{
	enum ProxyAuthType {
		proxy_auth_unknown = 0, 
		proxy_auth_basic, 
		proxy_auth_ntlm_auto, 
		proxy_auth_ntlm, 
		proxy_auth_none, 
		proxy_auth_error, 
	};

public:
	CProxySet(void)
	{
		m_bProxyEnable = false;
		m_bBasicAuthSupported = false;
		m_bNtlmAuthSupported = false;
		m_strProxyServerIP.clear();
		m_nProxyServerPort = 0;
		m_strProxyUser.clear();
		m_strProxyPassword.clear();
		m_strDomainName.clear();
	}
	~CProxySet(void){}
	void EnableProxy(bool bEnable = true){m_bProxyEnable = bEnable;}
	bool IsEnableProxy(){return m_bProxyEnable;}
	void GenProxyParam(INTERNET_PROXY_INFO &proxyParam)
	{		
		sprintf_s(m_strProxyList, "%s:%d", m_strProxyServerIP.c_str(), m_nProxyServerPort);
		proxyParam.dwAccessType = INTERNET_OPEN_TYPE_PROXY;
		proxyParam.lpszProxy = (LPCWSTR)m_strProxyList;
		proxyParam.lpszProxyBypass = NULL;
	}
	void SetProxyInfo(const char* lpszServerIP = NULL,int nProxyPort = 0,const char* lpszUser = NULL,const char* lpszPwd = NULL,const char* lpszDomain = NULL)
	{
		m_strProxyServerIP = lpszServerIP == NULL?"":string(lpszServerIP);
		m_nProxyServerPort = nProxyPort;
		m_strProxyUser = lpszUser == NULL?"":string(lpszUser);
		m_strProxyPassword = lpszPwd == NULL?"":string(lpszPwd);
		m_strDomainName = lpszDomain == NULL?"":string(lpszDomain);
	}
private:
	bool m_bProxyEnable;
	ProxyAuthType m_ProxyAuthType; 
	bool m_bBasicAuthSupported; 
	bool m_bNtlmAuthSupported; 
	string m_strProxyServerIP;
	int m_nProxyServerPort;
	string m_strProxyUser;
	string m_strProxyPassword;
	string m_strDomainName;
	char m_strProxyList[MAX_PATH];
};
