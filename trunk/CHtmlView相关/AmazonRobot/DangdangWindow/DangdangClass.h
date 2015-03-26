#pragma once
#include "../GoodBase/DataDefine.h"
#include "../GoodBase/GoodOrderBase.h"
#include "../GoodBase/Function.h"
class CDangdangClass : public CWebBase, public GoodOrderStru
{
public:
	CDangdangClass(void);
	~CDangdangClass(void);
	virtual bool Login(CString strUser, CString strPwd, CString strCheckCode = "");
	virtual bool AddToCard(CString strGoodNo, int nGoodNum);
	int DoOrder(GoodOrderStru* pGoodOrderStr);
	virtual CString GetRetString(int iCode);

	bool AddAddress(GoodOrderStru* pGoodOrderStr, int& iRetCode);
	private:
	CString GetLocationUrl();
	void EncodeAddrInfo(GoodOrderStru *pGoodOrderStr);
	bool AddAddressEx(CString& strRet, GoodOrderStru* pGoodOrderStr, int& iRetCode);
	bool SelectShipType(GoodOrderStru* pGoodOrderStr, int& iRetCode);
	bool SelectPayType(GoodOrderStru* pGoodOrderStr, int& iRetCode);
	bool Submit(GoodOrderStru* pGoodOrderStr, int& iRetCode);

	bool CheckConfigInfo(GoodOrderStru* pGoodStr, int& iRetCode);
	CString GetPayType(CString strPayId);
	CString GetWaitType(_SendTime eType);	
	void DeleteAddress();	
	bool GetShipData(CString &strSrc, CString& strValue, CString strFlag, CString& strRet, CString strOrderNo);
	void ClearCard();
	bool GetAllHiddenValue(CString strSrc, CString& strRet);
	bool ClearOthersInCard(CString strGoodNo);
	void RemoveGood(CString strGoodNos);
	string m_strHeader;
	string m_strServer;
	CString m_strLastQueryUrl;
	map<CString, CString> m_mapInvoiceCategoryID;
	map<CString, CString> m_mappaymentMethodId;
	CString m_strBillContent;
	CString m_strPayType;
	CString m_strpaymentMethodType;

	bool m_bNeedMoreInfo;
	CString m_strOrderNoEx;
	CString m_strItemList;
};
