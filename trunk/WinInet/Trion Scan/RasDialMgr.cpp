#include "StdAfx.h"
#include "datatype.h"

CRasDialMgr::CRasDialMgr(void)
{
	m_hRscon = NULL;
}

CRasDialMgr::~CRasDialMgr(void)
{
}

int CRasDialMgr::DialRas(CString &strResult)
{
	DWORD iRet= RasDialA(NULL,NULL,&m_rdParams,0L,NULL,&m_hRscon);
	if (iRet == 0)
	{
		strResult = "拨号成功";
	}
	else
	{
		m_hRscon = NULL;
		strResult = "连接出错";
	}
	return iRet;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CRasDialMgr::StopRas(CString &strResult)
{
	//枚举连接
	DWORD iRet = 0;
	if (m_hRscon != NULL)
	{
		iRet= RasHangUp(m_hRscon);
		if (iRet==0)
		{
			strResult = "连接已断开";
			m_hRscon = NULL;
			return iRet;
		}
		else
		{
			strResult = "断开连接失败";
		}		
	}	
	DWORD iCount = sizeof(RASCONN);
	RASCONN rasConn;
	rasConn.dwSize = 0x19c;
	DWORD iLen = 0;
	iRet = RasEnumConnections(&rasConn,&iCount,&iLen);
	if (iRet == 0 && iLen > 0)
	{
		for (int i=0;i<iLen;i++)
		{
			iRet= RasHangUp(rasConn.hrasconn);
			if (iRet==0)
			{
				strResult = "连接已断开";
				rasConn.hrasconn = NULL;				
			}
			else
			{
				strResult = "断开连接出错";
			}
		}
	}
	else
	{
		strResult = "无连接";
	}
	m_hRscon = NULL;
	return iRet;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CRasDialMgr::SetParams(CString strAcc,CString strPwd,CString strConnName)
{
	m_rdParams.dwSize=sizeof(RASDIALPARAMSA);
	strcpy(m_rdParams.szEntryName,strConnName); //连接名称是你的拨号名称
	m_rdParams.szPhoneNumber[0]='\0';
	m_rdParams.szCallbackNumber[0]='\0';
	strcpy(m_rdParams.szUserName,strAcc);
	strcpy(m_rdParams.szPassword,strPwd);
	m_rdParams.szDomain[0]='\0';
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CRasDialMgr::GetGateWayIP(CString &strIP)
{
	RASPPPIP rip;
	ZeroMemory(&rip,sizeof(rip));
	//RASPROJECTION rasprj;
// 	RASPROJECTION
// 	{
// 		RASP_Amb = 0x10000,
// 			RASP_PppNbf = 0x803F,
// 			RASP_PppIpx = 0x802B,
// 			RASP_PppIp = 0x8021,
// #if (WINVER >= 0x500)
// 			RASP_PppCcp = 0x80FD,
// #endif
// 			RASP_PppLcp = 0xC021,
// 			RASP_Slip = 0x20000
// 	};
	DWORD size;
	size = 40;
	rip.dwSize = 40;
	if((RasGetProjectionInfo(m_hRscon,RASP_PppIp,&rip,&size)) != 0)		//获取拨号后的公网IP
	{
		strIP = "";
		return -1;
	}
	else
	{
		strIP.Format("%s",rip.szIpAddress);
	}
	return 0;
}




bool CRasDialMgr::CreateVPN(const LPCTSTR pszEntryName, const LPCTSTR pszServerName, 
    const LPCTSTR pszUserName, const LPCTSTR pszPassWord)
{
    RASENTRY rasEntry;
    DWORD rasEntrySize, dwResult;

    memset(&rasEntry, 0, sizeof(rasEntry));
    rasEntrySize = sizeof(rasEntry);
    rasEntry.dwCountryCode = 86;
    rasEntry.dwCountryID = 86;
    rasEntry.dwDialExtraPercent = 75;
    rasEntry.dwDialExtraSampleSeconds = 120;
    rasEntry.dwDialMode = RASEDM_DialAll;
    rasEntry.dwType = RASET_Vpn;
    rasEntry.dwRedialCount = 30;
    rasEntry.dwRedialPause = 60;
    rasEntry.dwSize = sizeof(rasEntry);
    rasEntry.dwfOptions = RASEO_SwCompression | RASEO_RequireEncryptedPw | RASEO_RequireDataEncryption | 
        RASEO_PreviewUserPw;
    rasEntry.dwFramingProtocol = RASFP_Ppp;
    rasEntry.dwVpnStrategy = VS_PptpOnly;
    rasEntry.dwfNetProtocols = RASNP_Ip;
    rasEntry.dwEncryptionType = ET_Optional;    //可选加密
    rasEntry.dwHangUpExtraPercent = 10;
    rasEntry.dwHangUpExtraSampleSeconds = 120;
    lstrcpy(rasEntry.szLocalPhoneNumber, pszServerName);
    lstrcpy(rasEntry.szDeviceType, RASDT_Vpn);
    lstrcpy(rasEntry.szDeviceName, TEXT("vpn"));

    dwResult = RasSetEntryProperties(NULL, pszEntryName, &rasEntry, sizeof(rasEntry), NULL, 0);
    if (dwResult != 0)
    {
        AfxMessageBox(_T("error: SetEntryProperties"));
        return false;
    }

    RASDIALPARAMS rdParams;
    ZeroMemory(&rdParams, sizeof(RASDIALPARAMS));
    rdParams.dwSize = sizeof(RASDIALPARAMS);
    lstrcpy(rdParams.szEntryName, pszEntryName);
    lstrcpy(rdParams.szUserName, pszUserName);
    lstrcpy(rdParams.szPassword, pszPassWord);
    DWORD dwRet = RasSetEntryDialParams(NULL, &rdParams, TRUE);

    /*
    RASCREDENTIALS  rc;
    rc.dwSize = sizeof(RASCREDENTIALS);
    rc.dwMask = 11;
    lstrcpy(rc.szUserName, pszUserName);
    lstrcpy(rc.szPassword, pszPassWord);
    dwRet = RasSetCredentials(NULL, pszEntryName, &rc, FALSE);*/
    
    if(dwRet == 0)
        return true;
    else
        return false;

}



bool CRasDialMgr::ConnectVPN(void)
{
    RASDIALPARAMS    RasDialParams;
    HRASCONN        hRasConn;
    DWORD            Ret;
	ZeroMemory(&RasDialParams, sizeof(RASDIALPARAMS));
    RasDialParams.dwSize = sizeof(RASDIALPARAMS);
    lstrcpy(RasDialParams.szEntryName, c_VPNDescription);
    lstrcpy(RasDialParams.szPhoneNumber, c_VPNServer);
    lstrcpy(RasDialParams.szUserName, m_strAcc);
    lstrcpy((LPSTR)RasDialParams.szPassword, m_strPwd);
    hRasConn = NULL;
    Ret = RasDial(NULL, NULL, &RasDialParams, 0, NULL, &hRasConn);
    if (Ret != 0)
    {
		//RasDeleteEntry(NULL, RasDialParams.szEntryName);
        return false;
    }
    return true;
}