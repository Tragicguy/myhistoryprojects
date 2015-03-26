#include "StdAfx.h"
#include "DangdangClass.h"

CDangdangClass::CDangdangClass(void)
{
	m_strHeader = "Content-Type:application/x-www-form-urlencoded";
	m_strHeader.append("\r\nUser-Agent:Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.2; SV1; .NET CLR 1.1.4322; .NET CLR 2.0.50727; .NET CLR 3.0.04506.30; .NET CLR 3.0.04506.648)");
	m_strHeader.append("\r\nAccept: */*");
	m_strHeader.append("\r\nConnection: Keep-Alive");
	m_strHeader.append("\r\nAccept-Encoding:gzip, deflate");
	m_strHeader.append("\r\nUA-CPU:x86");
	m_strHeader.append("\r\nAccept-Language:zh-cn");
	m_strHeader.append("\r\nConnection: Keep-Alive");
	m_strServer = "shopping.dangdang.com";
	SetDefaultServer(m_strServer.c_str());
	//SetNoCookie(true);
	SetNotAutoRedirect(true);
}

CDangdangClass::~CDangdangClass(void)
{
}

bool CDangdangClass::Login(CString strUser, CString strPwd, CString strCheckCode)
{
	CString strLoginServer = "login.dangdang.com";
	CString strObject = "";
	strObject.Format("/signin.aspx");
	int iRet = GetPage(strObject, m_strHeader.c_str(), (LPCTSTR)strLoginServer);
	if(iRet < 0)		
		return false;
	CString strRet = GetContentInfo();
	CString strViewState = CFunction::GetKeyString(strRet, "__VIEWSTATE\" value=\"", "\"");
	strViewState = CFunction::URLEncode(strViewState);

	string strHeader = m_strHeader;
	strHeader.append("\r\nReferer:http://login.dangdang.com/signin.aspx");
	CString strData = "";
	strData.Format("__VIEWSTATE=%s&txtUsername=%s&txtPassword=%s&login_type=0&btnSign=",
		strViewState, strUser, strPwd);
	iRet = PostPage(strObject, (LPCTSTR)strData, strHeader.c_str(), (LPCTSTR)strLoginServer);
	if(iRet < 0)
		return false;
	strRet = GetContentInfo();

	strObject = "/checkout.aspx?shop_id=0";

	iRet = GetPage(strObject, m_strHeader.c_str(), "checkout.dangdang.com");
	if(iRet < 0)
		return false;
	if (strRet.Find("window.location.replace") > 0)
	{
		return true;
	}
	return false;
}

bool CDangdangClass::AddToCard(CString strGoodNo, int nGoodNum)
{
	RemoveGood(strGoodNo);
	CString strObject = "";
	strObject.Format("/product.aspx?product_id=%s", strGoodNo);
	int iRet = GetPage(strObject, m_strHeader.c_str(), "product.dangdang.com");
	if(iRet < 0)		
		return false;
	CString strUrl = "http://product.dangdang.com" + strObject;
	strObject.Format("/shoppingcart/shopping_cart_add.aspx?product_ids=%s.%d&dd_refer=%s", strGoodNo, nGoodNum, CFunction::URLEncode(strUrl));
	iRet = GetPage(strObject, m_strHeader.c_str());
	if(iRet < 0)		
		return false;
	strObject = "/shoppingcart/show.php";
	iRet = PostPage(strObject, NULL, m_strHeader.c_str());
	if(iRet < 0)
		return false;
	ClearOthersInCard(strGoodNo);
	return true;
}

int CDangdangClass::DoOrder(GoodOrderStru* pGoodOrderStr)
{
	return 0;
}

CString CDangdangClass::GetRetString(int iCode)
{
	return "";
}

bool CDangdangClass::ClearOthersInCard(CString strGoodNo)
{
	CString strRet = GetContentInfo();	
	CString strFlag = "\"products\":";
	int iPos = strRet.Find(strFlag);
	while (iPos >= 0)
	{
		strRet = strRet.Mid(strFlag.GetLength() + iPos);
		CString strNo = CFunction::GetKeyString(strRet, "\"", "\":");
		if (strGoodNo != strNo)
		{
			RemoveGood(strNo);
		}		
		iPos = strRet.Find(strFlag);
	}
	return true;
}

void CDangdangClass::RemoveGood(CString strGoodNos)
{
	CString strObject = "";
	strObject.Format("/shoppingcart/remove.php", strGoodNos);
	CString strData = "";
	strData.Format("product_id=%s", strGoodNos);
	int iRet = PostPage(strObject, (LPCTSTR)strData, m_strHeader.c_str());
	if(iRet < 0)
		return;
}

//
bool CDangdangClass::AddAddress(GoodOrderStru* pGoodOrderStr, int& iRetCode)
{
	CString strData = "";
	CFunction::ConvertGBKToUtf8(pGoodOrderStr->strRecver);
	CFunction::ConvertGBKToUtf8(pGoodOrderStr->strAddr);
	strData.Format("ship_name=%s&country_id=9000&province_id=%s&city_id=%s&town_id=%s&ship_zip=%s&ship_mb=%s&ship_tel=&ship_address=%s&addr_id=&is_valid_address=1&town_id_in_book=&ship_name_in_book=&ship_address_in_book=&cur_edit_area=0&shop_id=0",
		pGoodOrderStr->strRecver,
		pGoodOrderStr->strProvince,
		pGoodOrderStr->strCity,
		pGoodOrderStr->strTown,
		pGoodOrderStr->strPostCode,
		pGoodOrderStr->strPhone,
		pGoodOrderStr->strAddr);
	CString strObject = "/consignee.aspx";
	CString strServer = "checkout.dangdang.com";
	string strHeader = m_strHeader;
	strHeader.append("\r\nReferer: http://checkout.dangdang.com/checkout.aspx?shop_id=0");
	int iRet = PostPage(strObject, (LPCTSTR)strData, strHeader.c_str(), (LPCTSTR)strServer);


	strData = "ship_type=1&ship_date_type=1&cur_edit_area=1&shop_id=0";
	strObject = "/shipment.aspx";
	iRet = PostPage(strObject, (LPCTSTR)strData, strHeader.c_str(), (LPCTSTR)strServer);

	strData = "pay_type=0&pay_id=1&shop_id=0";
	strObject = "/payment.aspx";
	iRet = PostPage(strObject, (LPCTSTR)strData, strHeader.c_str(), (LPCTSTR)strServer);

	strObject = "/show_vcode.aspx?t=1352061703038";
	iRet = GetPage(strObject, strHeader.c_str(), (LPCTSTR)strServer, 80, true, "c:\\test.bmp");


	int iResult = 9;

	CFunction::ConvertGBKToUtf8(pGoodOrderStr->strBillHeader);
	CFunction::ConvertGBKToUtf8(pGoodOrderStr->strBillContent);
	strData.Format("invoice_title=%s&invoice_content=%s&shop_id=0", 
		pGoodOrderStr->strBillHeader, 
		pGoodOrderStr->strBillContent);
	strObject = "/invoice_submit.aspx";
	iRet = PostPage(strObject, (LPCTSTR)strData, strHeader.c_str(), (LPCTSTR)strServer);

	strData.Format("AYH=%d&shop_id=0&need_send_order_sms=0", iResult);
	strObject = "/order_flow_submit.aspx";
	iRet = PostPage(strObject, (LPCTSTR)strData, strHeader.c_str(), (LPCTSTR)strServer);

	return true;

}