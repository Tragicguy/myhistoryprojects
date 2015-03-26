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
	HRASCONN m_hRscon;
	CString m_strAcc;
	CString m_strPwd;
	RASDIALPARAMSA m_rdParams;
};
