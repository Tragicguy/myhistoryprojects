#include "stdafx.h"
#include "AmazonClass.h"
#pragma warning(disable:4267)
#define AMAZON_HOST_FLAG "www.amazon.cn"

CAmazonClass::CAmazonClass(void)
{
	CFunction::InitInfo(AMAZON_INVOICE_CONTENT, m_mapInvoiceCategoryID);
	CFunction::InitInfo(AMAZON_PAY_INFO, m_mappaymentMethodId);	
	SetNotAutoRedirect(true);
	m_strHeader = "Content-Type:application/x-www-form-urlencoded";
	m_strHeader.append("\r\nUser-Agent:Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.2; SV1; .NET CLR 1.1.4322; .NET CLR 2.0.50727; .NET CLR 3.0.04506.30; .NET CLR 3.0.04506.648)");
	m_strHeader.append("\r\nAccept: */*");
	m_strHeader.append("\r\nConnection: Keep-Alive");
	m_strHeader.append("\r\nAccept-Encoding:gzip, deflate");
	m_strHeader.append("\r\nUA-CPU:x86");
	m_strHeader.append("\r\nAccept-Language:zh-cn");
	m_strHeader.append("\r\nConnection: Keep-Alive");
	m_strServer = "www.amazon.cn";
	SetDefaultServer(m_strServer.c_str());
	m_bNeedMoreInfo = false;
}

CAmazonClass::~CAmazonClass(void)
{
}



bool CAmazonClass::Login(CString strUser, CString strPwd, CString strCheckCode)
{
	string strHeader = m_strHeader;
	strHeader.append("\r\nReferer:");
	strHeader.append(m_strLastQueryUrl);

	CString strContent = GetContentInfo();
	int iPos = strContent.Find("修改购物车");
	if (iPos < 0)
		return false;
	strContent = strContent.Mid(iPos);
	CString strObject = CFunction::GetKeyString(strContent, "href=\"https://www.amazon.cn", "\"");
	m_strLastQueryUrl = "https://www.amazon.cn" + strObject;

	SetDefaultServer(m_strServer.c_str(), 443);
	SetHttpType(0);
	int iRet = GetPage(strObject, strHeader.c_str());
	if(iRet < 0)	return false;
	CString strRet = GetContentInfo();
	if (strRet.Find("登录") < 0)	return false;
	CString strassoc_handle = CFunction::GetKeyString(strRet, "name=\"openid.assoc_handle\" value=\"", "\"");
	CString stridentity = "ape:aHR0cDovL3NwZWNzLm9wZW5pZC5uZXQvYXV0aC8yLjAvaWRlbnRpZmllcl9zZWxlY3Q=";
	CString strsuppressSignInRadioButtons = "ape:MA==";
	CString strreturn_to = CFunction::GetKeyString(strRet, "name=\"openid.return_to\" value=\"", "\"");
	CString strpage = "ape:aHR0cDovL3NwZWNzLm9wZW5pZC5uZXQvZXh0ZW5zaW9ucy9wYXBlLzEuMA==";
	CString strmax_auth_age = "ape:MA==";
	CString strclaimedid = "ape:aHR0cDovL3NwZWNzLm9wZW5pZC5uZXQvYXV0aC8yLjAvaWRlbnRpZmllcl9zZWxlY3Q=";
	CString strMode = "ape:Y2hlY2tpZF9zZXR1cA==";
	CString strns = "ape:aHR0cDovL3NwZWNzLm9wZW5pZC5uZXQvYXV0aC8yLjA=";
	CString strPageId = "ape:YW1hem9uX2NoZWNrb3V0X2Nu";
	CString strsessionID = CFunction::GetKeyString(strRet, "name=\"sessionID\" value=\"", "\"");
	CString strmetadata1 = GenAuthKey();
	CString strData = "";
	strData.Format("openid.assoc_handle=%s&suppressSignInRadioButtons=%s&openid.identity=%s&openid.return_to=%s\
&openid.ns.pape=%s&openid.pape.max_auth_age=%s&openid.claimed_id=%s&openid.mode=%s&openid.ns=%s&pageId=%s&sessionID=%s&email=%s&create=0\
&password=%s&siteState=hasWorkingJavascript&metadata1=%s&x=28&y=11",
		strassoc_handle,
		strsuppressSignInRadioButtons,
		stridentity,
		strreturn_to,
		strpage,
		strmax_auth_age,
		strclaimedid,
		strMode,
		strns,
		strPageId,
		strsessionID,
		strUser,
		strPwd,
		strmetadata1);
	strObject = "/ap/signin?ie=UTF8";

	strHeader.append("\r\nReferer:");
	strHeader.append(m_strLastQueryUrl);
	iRet = PostPage(strObject, strData, strHeader.c_str());
	if(iRet < 0)	return false;
	
	m_strLastQueryUrl = GetLocationUrl();

	iPos = m_strLastQueryUrl.Find(AMAZON_HOST_FLAG);
	if(iPos < 0)	return false;
	strObject = m_strLastQueryUrl.Mid(iPos + strlen(AMAZON_HOST_FLAG));

	iRet = GetPage(strObject, strHeader.c_str());
	if(iRet < 0)	return false;
	strRet = GetContentInfo();
	if (strRet.Find("卓越亚马逊订单确认") < 0)	return false;
	return true;
}

bool CAmazonClass::AddToCard(CString strGoodNo, int nGoodNum)
{
	CString strObject = "";
	strObject.Format("/mn/detailApp?asin=%s&%s",strGoodNo,FLAG_STRING);
	int iRet = GetPage(strObject, m_strHeader.c_str());
	if(iRet < 0)
		return false;
	CString strSessionId = "";	
	CString strisMerchantExclusive = "";
	CString strmerchantID = "";
	CString strnodeID = "";
	CString strofferListingID = "";
	CString strsellingCustomerID = "";
	CString strsourceCustomerOrgListID = "";
	CString strsourceCustomerOrgListItemID = "";
	CString strqid = "";
	CString strsr = "";
	CString strstoreID = "";
	CString strtagActionCode = "";
	CString strviewID = "";
	CString strrsid = "";

	CString strRet = GetContentInfo();
	strSessionId = CFunction::GetKeyString(strRet, "name=\"session-id\" value=\"", "\"");
	strisMerchantExclusive = CFunction::GetKeyString(strRet, "name=\"isMerchantExclusive\" value=\"", "\"");
	strmerchantID = CFunction::GetKeyString(strRet, "name=\"merchantID\" value=\"", "\"");
	strnodeID = CFunction::GetKeyString(strRet, "name=\"nodeID\" value=\"", "\"");
	strofferListingID = CFunction::GetKeyString(strRet, "name=\"offerListingID\" value=\"", "\"");
	strsellingCustomerID = CFunction::GetKeyString(strRet, "name=\"sellingCustomerID\" value=\"", "\"");
	strsourceCustomerOrgListID = CFunction::GetKeyString(strRet, "name=\"sourceCustomerOrgListID\" value=\"", "\"");
	strsourceCustomerOrgListItemID = CFunction::GetKeyString(strRet, "name=\"sourceCustomerOrgListItemID\" value=\"", "\"");
	strqid = CFunction::GetKeyString(strRet, "name=\"qid\" value=\"", "\"");
	strsr = CFunction::GetKeyString(strRet, "name=\"sr\" value=\"", "\"");
	strstoreID = CFunction::GetKeyString(strRet, "name=\"storeID\" value=\"", "\"");
	strtagActionCode = CFunction::GetKeyString(strRet, "name=\"tagActionCode\" value=\"", "\"");
	strviewID = CFunction::GetKeyString(strRet, "name=\"viewID\" value=\"", "\"");
	strrsid = CFunction::GetKeyString(strRet, "name=\"rsid\" value=\"", "\"");
	
	CString strData = "";
	CString strFormat = "__mk_zh_CN=%s&session-id=%s&ASIN=%s&isMerchantExclusive=%s&merchantID=%s&nodeID=%s&offerListingID=%s";
	strFormat += "&sellingCustomerID=%s&sourceCustomerOrgListID=%s&sourceCustomerOrgListItemID=%s&qid=%s&sr=%s&storeID=%s";
	strFormat += "&tagActionCode=%s&viewID=%s&quantity=%d&rsid=%s&submit.add-to-cart.x=59&submit.add-to-cart.y=11";

	strData.Format(strFormat,
		AMAZON_TITILE,
		strSessionId,
		strGoodNo,
		strisMerchantExclusive,
		strmerchantID,
		strnodeID,
		strofferListingID,
		strsellingCustomerID,
		strsourceCustomerOrgListID,
		strsourceCustomerOrgListItemID,
		strqid,
		strsr,
		strstoreID,
		strtagActionCode,
		strviewID,		
		nGoodNum,
		strrsid);

	string strHeader = m_strHeader;
	strHeader.append("\r\nReferer:");
	strHeader.append("http://www.amazon.cn");
	strHeader.append(strObject);

	int iPos = strRet.Find("id=\"handleBuy\"");
	if (iPos < 0)
		return false;
	strRet = strRet.Mid(iPos);
	strObject = CFunction::GetKeyString(strRet, "action=\"http://www.amazon.cn", "\"");

	iRet = PostPage(strObject, strData, strHeader.c_str());
	if (iRet < 0)	return false;

	m_strLastQueryUrl = GetLocationUrl();
	iPos = m_strLastQueryUrl.Find(AMAZON_HOST_FLAG);
	if(iPos < 0)	return false;
	strObject = m_strLastQueryUrl.Mid(iPos + strlen(AMAZON_HOST_FLAG));
	iRet = GetPage(strObject, strHeader.c_str());
	strRet = GetContentInfo();
	if (strRet.Find("已将") < 0)
		return false;
	return true;
}

int CAmazonClass::DoOrder(GoodOrderStru* pGoodOrderStr)
{
	CFunction::ClearSiteCookie("amazon");
	ClearCard();
	m_bNeedMoreInfo = false;
	int iRetCode = 0;	
	map<CString, int>::iterator it;
	bool bRet = false;
	bRet = CheckConfigInfo(pGoodOrderStr, iRetCode);
	if (!bRet)
		return ADD_TOCARTERROR;

	for (it = pGoodOrderStr->mapGoodNos.begin(); it != pGoodOrderStr->mapGoodNos.end(); it++)
	{
		int iTryCount = 0;
		while (iTryCount++ < 3)
		{
			bRet = AddToCard(it->first, it->second);
			if (bRet)	break;
		}
		if (!bRet)
			return ADD_TOCARTERROR;
	}
	bRet = Login(pGoodOrderStr->strUserID, pGoodOrderStr->strPwd);
	if (!bRet)
		return LOGIN_FAILED;
	EncodeAddrInfo(pGoodOrderStr);
	//判断是否是提交订单的信息,如果有,则进入修复地址
	
	bRet = AddAddress(pGoodOrderStr, iRetCode);
	if (!bRet)
		return iRetCode;
	bRet = SelectShipType(pGoodOrderStr, iRetCode);
	if( !bRet ) return iRetCode;
	bRet = SelectPayType(pGoodOrderStr, iRetCode);
	if( !bRet ) return iRetCode;
	
	bRet = Submit(pGoodOrderStr, iRetCode);
	if( !bRet ) return iRetCode;	
	return ORDER_SUCESS;
}

bool CAmazonClass::AddAddressEx(CString& strRet, GoodOrderStru* pGoodOrderStr, int& iRetCode)
{
	if (strRet.Find("建议地址") < 0)
		return false;
	CString addressValidationUseCase = CFunction::GetKeyString(strRet, "addressValidationUseCase\" value=\"", "\"");
	CString addressValidationClientName = CFunction::GetKeyString(strRet, "addressValidationClientName\" value=\"", "\"");
	CString addressValidationFormType = CFunction::GetKeyString(strRet, "addressValidationFormType\" value=\"", "\"");
	CString useAddressWidget = CFunction::GetKeyString(strRet, "useAddressWidget\" value=\"", "\"");
	CString ID = CFunction::GetKeyString(strRet, "ID\" value=\"", "\"");
	CString addr_1zip = CFunction::GetKeyString(strRet, "addr_1zip\" value=\"", "\"");
	CString addr_1validationStrategy = CFunction::GetKeyString(strRet, "addr_1validationStrategy\" value=\"", "\"");
	CString addr_1suggestionId = CFunction::GetKeyString(strRet, "addr_1suggestionId\" value=\"", "\"");
	CString avClientComponent_0 = CFunction::GetKeyString(strRet, "avClientComponent_0\" value=\"", "\"");
	CString hasWorkingJavascript = CFunction::GetKeyString(strRet, "hasWorkingJavascript\" value=\"", "\"");
	CString fingerprint = CFunction::GetKeyString(strRet, "fingerprint\" value=\"", "\"");
	if ("CNPostcodeValidator" != addr_1suggestionId)
		return false;
	CString strData = "";
	strData.Format("__mk_zh_CN=%s&addressValidationUseCase=%s&addressValidationClientName=%s&addressValidationFormType=%s&useAddressWidget=%s&ID=%s\
&addr_0name=%s&addr_0address1=%s&addr_0address2=&addr_0address3=&addr_0city=%s\
&addr_0county=%s&addr_0state=%s&addr_0zip=%s&addr_0country=China&addr_0voice=%s&addr_0countryCode=CN&addr=addr_1\
&addr_1name=%s&addr_1address1=%s&addr_1address2=&addr_1address3=&addr_1city=%s&addr_1county=%s&addr_1state=%s\
&addr_1zip=%s&addr_1country=CN&addr_1voice=%s&addr_1validationStrategy=%s&addr_1suggestionId=%s&addr_1countryCode=CN\
&useSelectedAddress.x=41&useSelectedAddress.y=9&avClientComponent_0=%s&hasWorkingJavascript=%s&fingerprint=%s",
				AMAZON_TITILE,
				addressValidationUseCase,
				addressValidationClientName,
				addressValidationFormType,
				useAddressWidget,
				ID,
				pGoodOrderStr->strRecver,
				pGoodOrderStr->strAddr,
				pGoodOrderStr->strCity,
				pGoodOrderStr->strTown,
				pGoodOrderStr->strProvince,
				pGoodOrderStr->strPostCode,
				pGoodOrderStr->strPhone,
				pGoodOrderStr->strRecver,
				pGoodOrderStr->strAddr,
				pGoodOrderStr->strCity,
				pGoodOrderStr->strTown,
				pGoodOrderStr->strProvince,
				addr_1zip,
				pGoodOrderStr->strPhone,
				addr_1validationStrategy,
				addr_1suggestionId,
				avClientComponent_0,
				hasWorkingJavascript,
				fingerprint	);
	CString strObject = "/gp/buy/addressvalidation/handlers/continue.html";
	string strHeader = m_strHeader;
	strHeader.append("\r\nReferer:");
	strHeader.append(m_strLastQueryUrl);
	m_strLastQueryUrl = "https://www.amazon.cn" + strObject;
	int iRet = PostPage(strObject, strData, strHeader.c_str());
	if(iRet < 0) return false;
	strRet = GetContentInfo();
	if (strRet.Find("抱歉。此商品的购买数量有限") > 0)
	{
		iRetCode = NOT_ENUGH_GOOD;
		return false;
	}
	if (strRet.Find("配送至") < 0)
		return false;
	return true;
}

bool CAmazonClass::AddAddress(GoodOrderStru* pGoodOrderStr, int& iRetCode)
{	
	iRetCode = SETADDR_ERROR;
	CString strContent = GetContentInfo();
	CString strObject = "";
	int iRet = -1;
	if (strContent.Find("id=\"needChinaInvoice\"") > 0)	//需要进入添加地址页面
	{
		iRetCode = BE_SET_QUICK_ORDER;
		return false;
	}
	DeleteAddress();
	CString strnumberOfDistinctItems = CFunction::GetKeyString(strContent, "numberOfDistinctItems\" value=\"", "\"");
	CString strenterAddressIsDomestic = CFunction::GetKeyString(strContent, "enterAddressIsDomestic\" value=\"", "\"");
	CString strhasWorkingJavascript = CFunction::GetKeyString(strContent, "hasWorkingJavascript\" value=\"", "\"");
	CString strfingerprint = CFunction::GetKeyString(strContent, "fingerprint\" value=\"", "\"");
	if (strnumberOfDistinctItems == "")	strnumberOfDistinctItems = "1";
	if (strenterAddressIsDomestic == "")	strenterAddressIsDomestic = "1";
	if (strhasWorkingJavascript == "")	strhasWorkingJavascript = "1";
	if (strfingerprint == "")	strfingerprint = "1";
	CString strData = "";
	strData.Format("__mk_zh_CN=%s&numberOfDistinctItems=%s&enterAddressFullName=%s&enterAddressStateOrRegion=%s&enterAddressCity=%s\
&enterAddressDistrictOrCounty=%s&enterAddressAddressLine1=%s&enterAddressPostalCode=%s&enterAddressPhoneNumber=%s&enterAddressIsDomestic=%s\
&enterAddressCountryCode=CN&hasWorkingJavascript=%s&fingerprint=%s&shipToThisAddress.x=40&shipToThisAddress.y=19",
		AMAZON_TITILE,
		strnumberOfDistinctItems,
		pGoodOrderStr->strRecver,
		pGoodOrderStr->strProvince,
		pGoodOrderStr->strCity,
		pGoodOrderStr->strTown,
		pGoodOrderStr->strAddr,
		pGoodOrderStr->strPostCode,
		pGoodOrderStr->strPhone,
		strenterAddressIsDomestic,
		strhasWorkingJavascript,
		strfingerprint);
	string strHeader = m_strHeader;
	strHeader.append("\r\nReferer:");
	strHeader.append(m_strLastQueryUrl);
	
	strObject = "/gp/buy/shipaddressselect/handlers/continue.html/ref=ox_shipaddress_add_new_addr";
	m_strLastQueryUrl = "https://www.amazon.cn" + strObject;
	iRet = PostPage(strObject, strData, strHeader.c_str());
	if(iRet < 0)	return false;
	CString strRet = GetContentInfo();
	if (strRet.Find("抱歉。此商品的购买数量有限") > 0)
	{
		iRetCode = NOT_ENUGH_GOOD;
		return false;
	}
	if (strRet.Find("订单有一些小问题") > 0)
		return false;
	if (strRet.Find("配送至") > 0)
		return true;
	return AddAddressEx(strRet, pGoodOrderStr, iRetCode);
}

bool CAmazonClass::SelectShipType(GoodOrderStru* pGoodOrderStr, int& iRetCode)
{
	CString strRet = GetContentInfo();
	bool bSpeedSend = false;
	int iPos = strRet.Find("ShippingSpeed_std-cn-postal");
	if (iPos < 0)
	{
		iRetCode = SEND_TYPE_ERROR;
		return false;
	}
	
	CString strContent = strRet.Left(iPos);
	iPos = strContent.ReverseFind('=');
	if (iPos < 0)
	{
		iRetCode = SEND_TYPE_ERROR;
		return false;
	}
	
	strContent = strContent.Mid(iPos);
	CString strOrderNo = CFunction::GetKeyString(strContent, "_", "_");	
	CString strShipValue[6] = {""};	
	CString strDataEx = "";	
	if (!GetShipData(strRet, strShipValue[Normal], "ShippingSpeed_std-cn-postal", strDataEx, strOrderNo))
	{
		if (pGoodOrderStr->eShipType == Normal)
		{
			iRetCode = NONORMAL;
			return false;
		}
	}

	if (!GetShipData(strRet, strShipValue[SuperExpress], "ShippingSpeed_exp-cn-d2d", strDataEx, strOrderNo))
	{
		if (pGoodOrderStr->eShipType == SuperExpress)
			pGoodOrderStr->eShipType = Express;
	}

	if (!GetShipData(strRet, strShipValue[Ems], "ShippingSpeed_exp-cn-ems", strDataEx, strOrderNo))
	{
		if (pGoodOrderStr->eShipType == Ems)
		{
			if (!pGoodOrderStr->bCanNormalSend){
				iRetCode = NOEMS;
				return false;
			}
			pGoodOrderStr->eShipType = Normal;
		}
		
	}
	if (!GetShipData(strRet, strShipValue[Express], "ShippingSpeed_std-cn-d2d", strDataEx, strOrderNo))
	{
		if (pGoodOrderStr->eShipType == Express)
		{
			if (!pGoodOrderStr->bCanNormalSend)
			{
				iRetCode = NOEXPRESS;
				return false;
			}
			pGoodOrderStr->eShipType = Normal;
		}
	}	
	if (!GetShipData(strRet, strShipValue[Scheduled], "ShippingSpeed_scheduled", strDataEx, strOrderNo))
	{
		if (pGoodOrderStr->eShipType == Scheduled)
		{
			if (!pGoodOrderStr->bCanNormalSend)
			{
				iRetCode = NOEXPRESS;
				return false;
			}
			pGoodOrderStr->eShipType = Normal;
		}
	}
	else
	{
		CString strTemp = "";
		CString strOrderNoEx = strOrderNo;
		strOrderNoEx.Replace("-", ":");
		//478:100814124136", "jhqmlrhpqpms,jhqmlrhpqpqs"
		CString strItemList = CFunction::GetKeyString(strRet, strOrderNoEx + "\", \"", "\"");
		strTemp.Format("&SD-ResID-0=&SD-StartTime-0=&SD-EndTime-0=&SD-WindowTZ-0=&SD-CutoffTime-0=&SD-CutoffTZ-0=&SD-Date-0=&SD-Cutoff-0=&SD-OrderID-0=%s&SD-ItemList-0=%s&D_MinShipDtUTC-0=&D_MaxShipDtUTC-0=&makeReservation=0",
		strOrderNoEx,
		strItemList);
		strDataEx += strTemp;
		m_bNeedMoreInfo = true;
		m_strOrderNoEx = strOrderNoEx;
		m_strItemList = strItemList;
	}
	
	CString strShipType = "shipWhenComplete";
	if( !pGoodOrderStr->bShipWaitType)
		strShipType = "shipWhenever";

	CString strData = "";
	strData.Format("returnpipedata=1&isClientTimeBased=1&order_%s_ShippingSpeed=%s&continue=set shipping option&returnjson=1",
		strOrderNo,
		strShipValue[pGoodOrderStr->eShipType]);
	CString strData2 = "";
	GetAllHiddenValue(strRet, strData2);
	strData += strData2;
	
	CString strObject = "/gp/buy/shipoptionselect/handlers/continue.html/ref=ox_shipoption_page_continue_0";
	string strHeader = m_strHeader;
	strHeader.append("\r\nReferer:");
	strHeader.append(m_strLastQueryUrl);	
	m_strLastQueryUrl = "https://www.amazon.cn" + strObject;
	int iRet = PostPage(strObject, strData, strHeader.c_str());
	if (iRet < 0)
	{
		iRetCode = SEND_TYPE_ERROR;
		return false;
	}
	strRet = GetContentInfo();
	if (strRet.Find("只能选择\"等待所有商品到货一起发货") > 0)
	{
		iRetCode = ONLY_ALL_COMPLETE;
		return false;
	}
	if (strRet.Find("{\"error\":\"") >= 0)
	{
		iRetCode = SEND_TYPE_ERROR;
		return false;
	}
	
	iPos = strRet.Find("订单总计");
	if (iPos < 0)
	{
		iRetCode = SEND_TYPE_ERROR;
		return false;
	}
	if (pGoodOrderStr->bUseGiftCard)
	{
		if (strRet.Find("\"giftcard\":null") > 0)
		{
			iRetCode = NO_GIFT_CARD;
			return false;
		}
		//取礼品卡余额
		float fGiftBalance = atof(CFunction::GetKeyString(strRet, "\"giftcard\":\"￥ ", "\""));
		strRet = strRet.Left(iPos);
		iPos = strRet.ReverseFind('{');
		if (iPos < 0){
			iRetCode = SEND_TYPE_ERROR;
			return false;
		}
		strRet = strRet.Mid(iPos);
		float fPrice = atof(CFunction::GetKeyString(strRet, "value\":\"￥ ", "\""));
		if (fPrice > fGiftBalance){
			iRetCode = LOW_GIFTCARD_BALANCE;
			return false;
		}
	}
	return true;
}

bool CAmazonClass::GetShipData(CString &strSrc, CString& strValue, CString strFlag, CString& strRet, CString strOrderNo)
{
	int iPos = strSrc.Find(strFlag);
	if(iPos > 0)
	{
		CString strTemp = strSrc.Mid(iPos);
		CString strData = "";
		strData.Format("SSS_order_%s_%s%s", strOrderNo, strFlag, CFunction::GetKeyString(strSrc, strFlag, "\""));
		strValue = CFunction::GetKeyString(strTemp, "value=\"", "\"");
		strRet += "&" + strData + "=0";
		return true;
	}
	return false;
}

bool CAmazonClass::SelectPayType(GoodOrderStru* pGoodOrderStr, int& iRetCode)
{
	//如果支付方式是现金或者POS刷卡,则判断是否有权限
	if ((pGoodOrderStr->eShipType == Normal || pGoodOrderStr->eShipType == Ems) &&
		(pGoodOrderStr->strPayType == "cash" || pGoodOrderStr->strPayType == "mpos") )
	{
		iRetCode = NOSUCHPAYTYPE;
		return false;
	}
	CString strData = "";
	CString strObject = "/gp/buy/pipeline/payselect.html";
	int iRet = -1;
	CString strRet = "";
	if (pGoodOrderStr->strPayType == "cash" || pGoodOrderStr->strPayType == "mpos")
	{
		iRet = PostPage(strObject, strData, m_strHeader.c_str());
		if (iRet < 0)
		{
			iRetCode = PAY_TYPE_ERROR;
			return false;
		}
		strRet = GetContentInfo();
		strRet.MakeLower();
		CString strFlag = "";
		if (pGoodOrderStr->strPayType == "cash")
			strFlag = "<strong>现金</strong>";
		else
			strFlag = "<strong>pos机刷卡</strong>";
		int iPos  = strRet.Find(strFlag);
		if(iPos < 0)
		{
			iRetCode = PAY_TYPE_ERROR;
			return false;
		}
		strRet = strRet.Mid(iPos);
		CString strTemp = CFunction::GetKeyString(strRet, strFlag, "</span>");
		if(strTemp.Find("不可用") >= 0)
		{			
			iRetCode = NOSUCHPAYTYPE;
			return false;
		}
	}

	pGoodOrderStr->strPayTypeEx = GetPayType(pGoodOrderStr->strPayType);
	strData.Format("paymentMethodId=%s&paymentMethodType=%s&useGCBalance=%s&usePromoBalance=true&currentlyUsesPromoBalance=1&\
fpsPaymentMethodId=&purchaseOrderNumber=&purchasingCardNumber=&rewardAccountId=&rewardAccountAmount=&rewardAccountRate=&\
fingerprint=&hasWorkingJavascript=1&returnjson=0&canTryTFX=undefined",
pGoodOrderStr->strPayType,
pGoodOrderStr->strPayTypeEx,
pGoodOrderStr->bUseGiftCard ? "true" : "false");

	strObject = "/gp/buy/payselect/handlers/continue.html/ref=ox_pay_page_continue_top";
	string strHeader = m_strHeader;
	strHeader.append("\r\nReferer:");
	strHeader.append(m_strLastQueryUrl);
	m_strLastQueryUrl = "https://www.amazon.cn" + strObject;
	iRet = PostPage(strObject, strData, strHeader.c_str());
	if (iRet < 0)
	{
		iRetCode = PAY_TYPE_ERROR;
		return false;
	}
	strRet = GetContentInfo();
	if (strRet.Find("id=\"needChinaInvoice\"") < 0 && strRet.Find("china-invoice-required") < 0)
	{
		iRetCode = PAY_TYPE_ERROR;
		return false;
	}
	return true;
}
/*
bool CAmazonClass::GetSlotData()
{
	CString strData = "";
	CString strAsin = "";
	strData.Format("merchant=A1AJ19PSB66TGU&marketplace=AAHKV2X7AFYLW&asin=%s&sku=&FMID=1000005578&quantity=1&offerListingID=%s&address=jpmimrnttq&itemListId=0&shipoption=1",
		strAsin,
		"a2avVBKOW5h5oSAFO90lyu%2FsizSwTtP%2FF2Bl51E7tnr6eH3u6gf57nkNWAtgGsXY%2BH7iLWyy8p9AAK%2Bm8FP0%2F92lnNqaYfsx4eT8Zy8ioys8u7VK0PFwWkUO8a6wDg54");
	//merchant=A1AJ19PSB66TGU&marketplace=AAHKV2X7AFYLW&asin=%s&sku=&FMID=1000005578&quantity=1&offerListingID=%s&address=jpmimrnttq&itemListId=0&shipoption=1

	//
}*/
bool CAmazonClass::Submit(GoodOrderStru* pGoodOrderStr, int& iRetCode)
{
	CString strRet = GetContentInfo();	
	CString strOrderId = CFunction::GetKeyString(strRet, "orderid=\"", "\"");
	if (m_bNeedMoreInfo)
	{
		strOrderId = m_strOrderNoEx;
	}
	if (strOrderId == "")
	{
		iRetCode = ORDER_ERROR;
		return false;
	}
	CString previousShipSplitPreference = "previousShipSplitPreference.::";
	CString orderLineItemIds = "orderLineItemIds.::";
	CString previousShippingSpeed = "previousShippingSpeed.::";
	CString orderdeliveryTimePreference = "order.::.deliveryTimePreference";
	CString orderdeliveryInstructionRemark = "order.::.deliveryInstructionRemark";
	previousShipSplitPreference.Replace("::", strOrderId);
	orderLineItemIds.Replace("::", strOrderId);
	previousShippingSpeed.Replace("::", strOrderId);
	orderdeliveryTimePreference.Replace("::", strOrderId);
	orderdeliveryInstructionRemark.Replace("::", strOrderId);
	
	previousShipSplitPreference = CFunction::GetKeyString(strRet, previousShipSplitPreference + "\" value=\"", "\"");
	orderLineItemIds = CFunction::GetKeyString(strRet, orderLineItemIds + "\" value=\"", "\"");
	previousShippingSpeed = CFunction::GetKeyString(strRet, previousShippingSpeed + "\" value=\"", "\"");

	CString strorderingCustomerID = CFunction::GetKeyString(strRet, "orderingCustomerID\" value=\"", "\""); 
	CString strpayingCustomerID = CFunction::GetKeyString(strRet, "payingCustomerID\" value=\"", "\""); 
		
	CString purchaseTotal = CFunction::GetKeyString(strRet, "purchaseTotal\" value=\"", "\"");
	CString purchaseTotalCurrency = CFunction::GetKeyString(strRet, "purchaseTotalCurrency\" value=\"", "\"");
	CString strcanUseGC = CFunction::GetKeyString(strRet, "canUseGC\" value=\"", "\"");

	CString strisCAMPSAvailable = CFunction::GetKeyString(strRet, "isCAMPSAvailable\" value=\"", "\"");
	CString strpaymentMethod = CFunction::GetKeyString(strRet, "paymentMethod\" value=\"", "\"");
	CString strcustName = CFunction::GetKeyString(strRet, "custName\" value=\"", "\"");
	CFunction::ConvertGBKToUtf8(strcustName);
	strcustName = CFunction::URLEncode(strcustName);

	CString strorderingPrefs = CFunction::GetKeyString(strRet, "orderingPrefs\" value=\"", "\"");

	CFunction::ConvertGBKToUtf8(pGoodOrderStr->strSendRemark);
	pGoodOrderStr->strSendRemark = CFunction::URLEncode(pGoodOrderStr->strSendRemark);
	CString chinaInvoiceCategoryID = pGoodOrderStr->strBillContent;	
	if (pGoodOrderStr->bBill)
	{
		CFunction::ConvertGBKToUtf8(pGoodOrderStr->strBillHeader);
		pGoodOrderStr->strBillHeader = CFunction::URLEncode(pGoodOrderStr->strBillHeader);		
		
	}
	else
	{
		pGoodOrderStr->strBillHeader = "";
		chinaInvoiceCategoryID = "";
	}
	
	CString strData = "";
	/*strData.Format("__mk_zh_CN=%s&purchaseTotal=%s&purchaseTotalCurrency=%s&claimCode=%s&previousShipSplitPreference.%s=%s&orderLineItemIds.%s=%s\
&previousShippingSpeed.%s=%s&needChinaInvoice=%d&chinaInvoiceTitle=%s&chinaInvoiceCategoryID=%s&hasWorkingJavascript=1&placeYourOrder.x=18&placeYourOrder.y=9",
		AMAZON_TITILE,
		purchaseTotal,
		purchaseTotalCurrency,
		pGoodOrderStr->strClaimCode,
		strOrderId,
		previousShipSplitPreference,
		strOrderId,
		orderLineItemIds,
		strOrderId,
		previousShippingSpeed,
		pGoodOrderStr->bBill,
		pGoodOrderStr->strBillHeader,
		chinaInvoiceCategoryID);
	*/
	CString strDataEx = "";
	if (pGoodOrderStr->eSendTime != 0 && pGoodOrderStr->eShipType != Normal && pGoodOrderStr->eShipType != Ems)
	{
		strDataEx.Format("&order.%s.deliveryTimePreference=%s&order.%s.deliveryInstructionRemark=%s",
			strOrderId,
			GetWaitType(pGoodOrderStr->eSendTime),
			strOrderId,
			pGoodOrderStr->strSendRemark);
		strData += strDataEx;
	}
	
	CString strObject = CFunction::GetKeyString(strRet, "action=\"", "\"");

	CString strData2 = "";
	GetAllHiddenValue(strRet, strData2);
	strData.Format("claimCode=%s&needChinaInvoice=%d&chinaInvoiceTitle=%s&chinaInvoiceCategoryID=%s&hasWorkingJavascript=1&placeYourOrder.x=18&placeYourOrder.y=9",
		pGoodOrderStr->strClaimCode,
		pGoodOrderStr->bBill,
		pGoodOrderStr->strBillHeader,
		chinaInvoiceCategoryID);
	strData += strDataEx;
	strData += strData2;

	//"/gp/buy/spc/handlers/static-submit.html";
	if (strRet.Find("/gp/buy/spc/handlers/static-submit-decoupled.html") > 0)
	{
		strObject = "/gp/buy/spc/handlers/static-submit-decoupled.html";
		
	}

	if (m_bNeedMoreInfo)
	{
		time_t timer; 
		time(&timer);
		long lStart = timer;
		int iTimeTail = rand() % 1000;
		CString strTempLog = "";
		strTempLog.Format("log0=%ld%03d,client-side-validation chinaInvoiceTitle,&log1=%ld%03d,client-side-validation chinaInvoiceCategoryID,",
			lStart,iTimeTail,lStart,iTimeTail);
		strTempLog = CFunction::URLEncode(strTempLog);
		CString strDataExEx1 = "";
		strDataExEx1.Format("&chinaInvoiceLogEvents=%s&orderingCustomerID=%s&payingCustomerID=%s&canUseGC=%s&isCAMPSAvailable=%s&paymentMethod=%s&custName=%s&orderingPrefs=%s",
			strTempLog,
			strorderingCustomerID,
			strpayingCustomerID,
			strcanUseGC,
			strisCAMPSAvailable,
			strpaymentMethod,
			strcustName,
			strorderingPrefs);
		
		//log0=1313227667,client-side-validation chinaInvoiceTitle,&log1=1313227667,client-side-validation chinaInvoiceCategoryID,
		//chinaInvoiceLogEvents=log0%3D1313227667%2Cclient-side-validation+chinaInvoiceTitle%2C%26log1%3D1313227667%2Cclient-side-validation+chinaInvoiceCategoryID%2C&
		
		CString strDataExEx2 = "";
		strDataExEx2.Format("&SD-ResID-0=%s&SD-StartTime-0=1313312400&SD-EndTime-0=1313344800&SD-WindowTZ-0=CTT&SD-CutoffTime-0=&SD-CutoffTZ-0=CTT&SD-Date-0=&SD-Cutoff-0=&SD-OrderID-0=%s&SD-ItemList-0=%s&D_MinShipDtUTC-0=1313251200&D_MaxShipDtUTC-0=1313337599&sduiDate=14&",
			"A392QOQTK7L7Q5%3BJOYO%3B0%5E0%5ECNY%3B",
			m_strOrderNoEx,
			m_strItemList);
		strData += strDataExEx1;
		strData += strDataExEx2;
		strObject = "/gp/checkout/confirm/select.html/ref=ox_spc_place_order_top";
	}
	
	string strHeader = m_strHeader;
	strHeader.append("\r\nReferer:");
	strHeader.append(m_strLastQueryUrl);
	m_strLastQueryUrl = "https://www.amazon.cn" + strObject;
	int iRet = PostPage(strObject, strData, strHeader.c_str());
	if (iRet < 0)
	{
		iRetCode = ORDER_ERROR;
		return false;
	}
	strRet = GetContentInfo();
	if (strRet.Find("感谢") < 0)
	{
		iRetCode = ORDER_ERROR;
		return false;
	}
	return true;
}

CString CAmazonClass::GetLocationUrl()
{
	CString strCHeader = GetHeader().c_str();
	CString strUrl = CFunction::GetKeyString(strCHeader, "Location:", "\r");
	strUrl.Trim();
	return strUrl;
}

void CAmazonClass::EncodeAddrInfo(GoodOrderStru *pGoodOrderStr)
{
	CFunction::ConvertGBKToUtf8(pGoodOrderStr->strRecver);
	CFunction::ConvertGBKToUtf8(pGoodOrderStr->strProvince);
	CFunction::ConvertGBKToUtf8(pGoodOrderStr->strCity);
	CFunction::ConvertGBKToUtf8(pGoodOrderStr->strTown);
	CFunction::ConvertGBKToUtf8(pGoodOrderStr->strAddr);
	pGoodOrderStr->strRecver = CFunction::URLEncode(pGoodOrderStr->strRecver);
	pGoodOrderStr->strProvince = CFunction::URLEncode(pGoodOrderStr->strProvince);
	pGoodOrderStr->strCity = CFunction::URLEncode(pGoodOrderStr->strCity);
	pGoodOrderStr->strTown = CFunction::URLEncode(pGoodOrderStr->strTown);
	pGoodOrderStr->strAddr = CFunction::URLEncode(pGoodOrderStr->strAddr);
}

bool CAmazonClass::CheckConfigInfo(GoodOrderStru* pGoodStr, int& iRetCode)
{
	iRetCode = CONFIG_ERROR;
	if (pGoodStr->mapGoodNos.size() < 1)
		return false;
	if (pGoodStr->mapGoodNos.size() == 1)
		pGoodStr->bShipWaitType = TRUE;

	if (m_mappaymentMethodId.find(pGoodStr->strPayType) == m_mappaymentMethodId.end())
	{
		iRetCode = PAYTYPE_ERROR;
		return false;
	}
	pGoodStr->strPayType = m_mappaymentMethodId[pGoodStr->strPayType];

	if (pGoodStr->bBill == true && m_mapInvoiceCategoryID.find(pGoodStr->strBillContent) == m_mapInvoiceCategoryID.end())
	{
		iRetCode = BILL_CONTENT_ERROR;
		return false;
	}
	pGoodStr->strBillContent = m_mapInvoiceCategoryID[pGoodStr->strBillContent];
	
	if (pGoodStr->eShipType < 1 || pGoodStr->eShipType > 4)
	{
		iRetCode = NOSUCHPAYTYPE;
		return false;
	}
	if ((pGoodStr->eShipType == Normal || pGoodStr->eShipType == Ems) &&
		(pGoodStr->strPayType == "cash" || pGoodStr->strPayType == "mpos") )
	{
		iRetCode = NOSUCHPAYTYPE;
		return false;
	}
	return true;
}

CString CAmazonClass::GetPayType(CString strPayId)
{
	if ("cash" == strPayId)
		return "cashOnDeliveryCash";
	if ("mpos" == strPayId)
		return "cashOnDeliveryMobilePointOfSale";
	return "externalPayments";
}

CString CAmazonClass::GetWaitType(_SendTime eType)
{
	switch(eType)
	{
	case WorkDayOnly:
		return "weekdays_cn_71874";
	case HolidayOnly:
		return "weekends_cn_71875";
	case NightOrOther:
		return "evenings_cn_71876";
	case AddDays:
		return "anyday_cn_71877";
	}
	return "weekdays_cn_71874";	
}

CString CAmazonClass::GetRetString(int iCode)
{

	switch(iCode)
	{
	case LOGIN_FAILED:
		return "登录失败";
	case ORDER_SUCESS:
		return "成功";
	case ORDER_ERROR:
		return "下单失败";
	case ADD_TOCARTERROR:
		return "添加到购物车失败";
	case SETADDR_ERROR:
		return "填写地址失败";
	case NOEMS:
		return "无EMS";
	case NOEXPRESS:
		return "无快递";
	case NOSUPEREXPRESS:
		return "无紧急快递";
	case CONFIG_ERROR:
		return "参数错误";
	case PAYTYPE_ERROR:
		return "支付类型无效";
	case BILL_CONTENT_ERROR:
		return "发票类型错";
	case NOSUCHPAYTYPE:
		return "您的配送地址不在MPOS及现金可配送范围内.您选择的送货方式是平邮或国内特快专递（EMS）";
	case ADDR_ERROR:
		return "地址错误";
	case SEND_TYPE_ERROR:
		return "设置发送方式出错";
	case ONLY_ALL_COMPLETE:
		return "只能选择等待所有商品到货一起发货";
	case NOT_ENUGH_GOOD:
		return "此商品的购买数量有限";
	case BE_SET_QUICK_ORDER:
		return "设置了一键下单,请先取消";
	case PAY_TYPE_ERROR:
		return "支付方式错误";
	}
	CString strTemp = "";
	strTemp.Format("未知:%d", iCode);
	return strTemp;
}

void CAmazonClass::DeleteAddress()
{
	CString strRet = GetContentInfo();
	CString strSessionId = CFunction::GetKeyString(strRet, "sessionID\" value=\"", "\"");
	CString strAddressId = "";
	CString strObjecgt = "";
	CString strAddressNo = "";
	while (true)
	{
		int iPos = strRet.Find("ox_shipaddress_edit_addr_");
		if (iPos < 0)
			break;
		strRet = strRet.Mid(iPos + 1);
		if (strRet == "")
		{
			break;
		}
		strAddressNo = CFunction::GetKeyString(strRet, "shipaddress_edit_addr_", "\"");
		strAddressId = CFunction::GetKeyString(strRet, "id=\"edit-", "\"");
		strObjecgt.Format("/gp/buy/address/handlers/delete-address.html?ref=ox_shipaddress_delete_addr_%s&addressID=%s&requestSessionId=%s",
			strAddressNo,
			strAddressId,
			strSessionId);
		GetPage(strObjecgt, m_strHeader.c_str());
	}	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CAmazonClass::ClearCard()
{
	int iCount = 0;
	while(iCount ++ < 5)
	{
		CString strObject = "/gp/cart/view.html/ref=gno_cart";
		int iRet = GetPage(strObject, m_strHeader.c_str());
		if (iRet < 0)	continue;
		CString strRet = GetContentInfo();
		if (strRet.Find("购物车为空") >= 0)	return;
		int iItemCount = atoi(CFunction::GetKeyString(strRet, "activeItemCount\" value=\"", "\"")) + 1;
		if (iItemCount < 2)	return;
		CString strData = "";
		for (int i=1; i<iItemCount; i++)
		{
			CString strTemp = "";
			strTemp.Format("name=\"itemID.%d\" value=\"", i);
			CString strItemId = CFunction::GetKeyString(strRet, strTemp, "\"");
			strTemp.Format("quantity.%d", i);
			int iPos = strRet.Find(strTemp);
			if (iPos < 1)	continue;
			strTemp = strRet.Mid(iPos);
			CString strQuantity = CFunction::GetKeyString(strRet, strTemp, "\"");
			strTemp.Format("isToBeGiftWrapped.%d=&delete.%d=&itemID.%d=%s&quantity.%d=%s",
				i, i, i, strItemId, i, strQuantity);
			strData += strTemp + "&";
		}
		strObject = "/gp/cart/view.html/ref=ox_sc_cart_delete_1";
		string strHeader = m_strHeader;
		strHeader.append("\r\nReferer:http://www.amazon.cn/gp/cart/view.html/ref=gno_cart");
		iRet = PostPage(strObject, strData, strHeader.c_str());
		if (iRet < 0)	continue;
		strRet = GetContentInfo();
		if (strRet.Find("购物车为空") >= 0)	return;
	}
}


bool CAmazonClass::GetAllHiddenValue(CString strSrc, CString& strRet)
{

	/*
	<input type="hidden" name="__mk_zh_CN" value="亚马逊网站" />
	<input type="hidden" name="purchaseTotal" value="46" />
	<input type="hidden" name="purchaseTotalCurrency" value="CNY" />
	*/
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