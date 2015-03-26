#include "stdafx.h"
#include "GoodOrderBase.h"
#include "Function.h"
#define MYWM_DANGDANG
CGoodOrderBase::CGoodOrderBase(void)
{
	m_nRetCode = ORDER_UNKNOWN_STATE;
	m_pGoodOrderStr = NULL;
}

CGoodOrderBase::~CGoodOrderBase(void)
{
}

int CGoodOrderBase::Order(GoodOrderStru* pGoodOrderStr, int nTimeOut)
{
#ifdef MYWM_DANGDANG
	return DoOrder(pGoodOrderStr);
#endif
	m_pGoodOrderStr = pGoodOrderStr;
	
	bool bRet = StartWorking();
	DWORD dwReturn = ::WaitForSingleObject(m_eQueryInfoEvent.m_hObject, nTimeOut);
	if (dwReturn == WAIT_TIMEOUT)
	{
		StopWorking();
		return ORDER_TIMEOUT;
	}
	return m_nRetCode;
}

void CGoodOrderBase::ThreadProc()
{
	MarkTheadToStop();
	if (m_pGoodOrderStr)
		m_nRetCode = DoOrder(m_pGoodOrderStr);
	else 
		m_nRetCode = PARAM_EXCEPTION;
	m_eQueryInfoEvent.SetEvent();
}

bool CGoodOrderBase::GetAllHiddenValue(CString strSrc, CString& strRet)
{
	CString strFlag = "type=\"hidden\"";
	char cLastFlag = '<';
	CString strNameBefore = "name=\"";
	CString strValueBefore = "value=\"";
	CString strAfter = "\"";
	int iPos = strSrc.Find(strFlag);
	CString strTemp = "";
	CString strName = "";
	CString strValue = "";
	while(iPos >=0 )
	{
		strTemp = strSrc.Left(iPos);
		int iPos1 = strTemp.ReverseFind(cLastFlag);
		if (iPos1 < 0)
			break;
		strSrc = strSrc.Mid(iPos + strFlag.GetLength());
		strName = CFunction::GetKeyString(strSrc, strNameBefore, strAfter);
		strValue = CFunction::GetKeyString(strSrc, strValueBefore, strAfter);
		if (strName != "")
		{
			CFunction::ConvertGBKToUtf8(strValue);			
			strTemp.Format("&%s=%s", strName, CFunction::URLEncode(strValue));
			strRet += strTemp;
		}
		iPos = strSrc.Find(strFlag);
	}
	return true;
}