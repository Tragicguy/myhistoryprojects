#ifndef  _CAMAZONCLASS_DEFINE__
#define  _CAMAZONCLASS_DEFINE__
#include "../GoodBase/DataDefine.h"
#include "../GoodBase/GoodOrderBase.h"
#include "../GoodBase/Function.h"
#include "AmazonEncode.h"
#define AMAZON_INVOICE_CONTENT "<option value='1' >图书</option><option value='2' >音像</option><option value='3' >游戏</option><option value='4' >软件</option><option value='5' >资料</option><option value='6' >办公用品</option><option value='7' >洗涤用品</option><option value='8' >化妆品</option><option value='9' >体育用品</option><option value='10' >劳保用品</option><option value='11' >服装</option><option value='12' >玩具</option><option value='13' >饰品</option><option value='14' >手机</option><option value='15' >家电</option><option value='16' >配件</option><option value='17' >数码产品</option><option value='18' >汽车用品</option><option value='19' >厨具</option><option value='20' >鞋靴</option><option value='21' >钟表</option><option value='22' >食品</option><option value='23' >教材</option>"
#define AMAZON_PAY_INFO "<option value='cash'>现金</option><option value='mpos'>POS机刷卡</option><option value='70'>支付宝</option><option value='80'>财付通账户</option><option value='53'>首信会员账户</option><option value='4'>用银行电汇支付您的订单</option><option value='11'>招商银行</option><option value='19'>中国工商银行</option><option value='30'>中国农业银行</option><option value='31'>中国建设银行</option><option value='32'>广东发展银行</option><option value='33'>中国银行</option><option value='34'>兴业银行</option><option value='35'>浦东发展银行</option><option value='37'>平安银行</option><option value='39'>民生银行</option><option value='40'>北京银行</option><option value='41'>华夏银行</option><option value='42'>中国交通银行</option><option value='43'>中国邮政</option><option value='44'>中信实业银行</option><option value='45'>深圳发展银行</option>"
class CAmazonClass : public CGoodOrderBase , protected CAmazonEncode
{
public:
	CAmazonClass(void);
public:
	~CAmazonClass(void);

	virtual bool Login(CString strUser, CString strPwd, CString strCheckCode = "");
	virtual bool AddToCard(CString strGoodNo, int nGoodNum);
	int DoOrder(GoodOrderStru* pGoodOrderStr);
	virtual CString GetRetString(int iCode);
private:
	bool AddAddress(GoodOrderStru* pGoodOrderStr, int& iRetCode);
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
#endif
