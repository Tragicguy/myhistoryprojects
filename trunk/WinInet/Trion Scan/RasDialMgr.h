#pragma once
#include <windows.h>
#include <stdio.h>
#include <Ras.h>
#pragma comment(lib,"rasapi32.lib")
class CRasDialMgr
{
public:
	CRasDialMgr(void);
	~CRasDialMgr(void);
	int DialRas(CString &strResult);
	int StopRas(CString &strResult);
	void SetParams(CString strAcc,CString strPwd,CString strConnName);
	int GetGateWayIP(CString &strIP);
	void SetVpnInfo(CString strServer,CString strInfo,CString strUser,CString strPwd){
		c_VPNServer = strServer;
		c_VPNDescription = strInfo;
		m_strAcc = strUser;
		m_strPwd = strPwd;
		c_VPNDescription = m_rdParams.szEntryName;
	}
	bool CreateVPN(const LPCTSTR pszEntryName, const LPCTSTR pszServerName, const LPCTSTR pszUserName, const LPCTSTR pszPassWord);
	bool ConnectVPN(void);
	HRASCONN m_hRscon;
	CString m_strAcc;
	CString m_strPwd;
	CString c_VPNServer;
	CString c_VPNDescription;
	RASDIALPARAMSA m_rdParams;
};
