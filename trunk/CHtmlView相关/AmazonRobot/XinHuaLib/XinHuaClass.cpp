#include "StdAfx.h"
#include "XinHuaClass.h"

XinHuaClass::XinHuaClass(void)
{
	CString strAppPath = "";
	GetAppPath(strAppPath);
	m_strPicPath.append((LPCTSTR)strAppPath);
	m_strPicPath.append("checkcode.bmp");
	m_strPicPath = "C:\\2.raw";
	m_strHeader = "Content-Type:application/x-www-form-urlencoded";
	m_strHeader.append("\r\nUser-Agent:Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.2; SV1; .NET CLR 1.1.4322; .NET CLR 2.0.50727; .NET CLR 3.0.04506.30; .NET CLR 3.0.04506.648)");
	m_strHeader.append("\r\nAccept: */*");
	m_strHeader.append("\r\nConnection: Keep-Alive");
	InitProvinceInfo();	
	m_strListPrice = "";
	m_strSalePrice = "";
	m_bClearCart = false;
}
inline BYTE toHex(const BYTE &x)
{
	return x > 9 ? x + 55: x + 48;
}
XinHuaClass::~XinHuaClass(void)
{
}
bool XinHuaClass::Login(CString strUser,CString strPwd)
{
	int iCount = 0;
	while(iCount++ < 4)
	{
		bool bRet = DoLogin(strUser,strPwd);
		if (bRet)
		{
			return true;
		}
	}
	return false;
}

bool XinHuaClass::DoLogin(CString strUser,CString strPwd)
{
	string strServer = "www.xinhuabookstore.com";
	string strHeader = m_strHeader;
	srand(time(NULL));
	int iRet  = -1;
	int iLoopCount = 0;
	CString strFailReason = "";
	CString strObject = "";
	strObject.Format("/customer/login.jsp");
	time_t t;
	time(&t);
	strHeader.append("\r\nReferer:http://www.xinhuabookstore.com/customer/login.jsp");
	strObject.Format("/customer/getValidateNumber.htm?d=%d000",(long)t);
	iRet = m_webbase.GetPage(strObject,strHeader.c_str(),strServer.c_str(),80,true,m_strPicPath.c_str());
	if (iRet < 0)
	{
		return false;
	}	
	strObject = "/customer/login.htm";
	string strCode = m_objCheck.CheckCode(m_strPicPath);
	CString strData = "";
	ConvertGBKToUtf8(strUser);
	strData.Format("registername=%s&password=%s&testcode=%s&x=0&y=0",strUser,strPwd,strCode.c_str());
	iRet = m_webbase.PostPage(strObject,strData,strHeader.c_str(),strServer.c_str());
	CString strRet = CString(m_webbase.GetContentInfo());
	if (strRet.Find("The URL has moved") >=0 )
	{
		time(&t);
		strObject.Format("/customer/shoppingcart/clear.htm?timestamp=%d000",(long)t);		
		if (m_bClearCart)
		{
			//清空购物车
			m_webbase.GetPage(strObject,strHeader.c_str(),strServer.c_str());			
		}
		return true;
	}	
	return false;
}

int XinHuaClass::Query(XinHuaOrderStr &orderStr)
{
	m_bClearCart = false;
	if (!Login(orderStr.strUserID,orderStr.strPwd))
	{
		return LOGIN_FAILED;
	}
	return DoQuery(orderStr);
}
int XinHuaClass::DoQuery(XinHuaOrderStr &orderStr)
{
	//http://www.xinhuabookstore.com/customer/order/list.htm?pagination.pageSize=2000&pagination.skipSize=10&pagination.currentPage=1
	string strServer = "www.xinhuabookstore.com";
	string strHeader = m_strHeader;
	srand(time(NULL));
	int iRet  = -1;
	CString strObject = "";	
	CString strData = "";
	time_t t;
	int iCount = 0;
	//strHeader.append("\r\nReferer:http://www.xinhuabookstore.com/customer/shoppingcart/shoppingcart.jsp");
	strObject = "/customer/order/list.htm?pagination.pageSize=10000&pagination.skipSize=10&pagination.currentPage=1";
	iRet = m_webbase.GetPage(strObject,strHeader.c_str(),strServer.c_str());
	if (iRet < 0)
	{
		return -1;
	}
	CString strRet = CString(m_webbase.GetContentInfo());
	ConvertUtf8ToGBK(strRet);
	if (strRet.Find("我的订单：") < 0)
	{
		return GETQUERY_ERROR;
	}
	//解析出所有的编号来
	CString strFlag = "view.htm?orderId=";
	CStringArray array;
	CString strNo = "";
	CString strContent = "";
	while(true)
	{
		if( strRet.Find(strFlag) < 0 )
		{
			break;
		}
		strNo = GetKeyString(strRet,strFlag,"\"");
		strObject.Format("/customer/order/view.htm?orderId=%s",strNo);
		int iCount = 0;
		while(iCount++<3)
		{
			iRet = 	m_webbase.GetPage(strObject,strHeader.c_str(),strServer.c_str());
			if (iRet == 0)
			{
				break;
			}
		}
		strContent = CString(m_webbase.GetContentInfo());
		ConvertUtf8ToGBK(strContent);
		AnanyzerGood(strNo, strContent);
		strRet = strRet.Mid(strRet.Find(strFlag)+strFlag.GetLength());
	}

	return 0;
}

void XinHuaClass::AnanyzerGood(CString strNo,CString& strContent)
{
	strContent.Replace("\r\n"," ");
	strContent.Replace("\r"," ");
	strContent.Replace("\n"," ");
	strContent.MakeLower();
	strContent.Replace("\t"," ");
	strContent.Replace("      "," ");
	CString strFlag = "订单号";
	CString strRecordInfo = strNo;
	CString strTemp = strContent.Mid(strContent.Find(strFlag));
	CString strStatus = GetKeyString(strTemp,"(",")");		//状态
	strStatus.Trim();
	CString strOrderTime = GetKeyString(strContent,"下单时间：</strong>","<");
	strOrderTime.Trim();
	strRecordInfo += "\r\n" + strOrderTime;
	strRecordInfo += "\r\n" + strStatus;
	if (strStatus == "全部已发")
	{
		strFlag = "发货时间";
		strTemp = strContent.Mid(strContent.Find(strFlag));
		CString strSendTime = GetKeyString(strTemp,"</strong>","</td>");
		strSendTime.Trim();
		strRecordInfo += "\r\n" + strSendTime;
		//配送公司：</STRONG> 申通快递 		<STRONG>运单号：</STRONG> 368778164603 <
		strFlag = "配送公司";
		strTemp = strContent.Mid(strContent.Find(strFlag)+strFlag.GetLength());
		CString strCompany = GetKeyString(strTemp,"</strong>","<");
		strFlag = "运单号";
		strTemp = strTemp.Mid(strTemp.Find(strFlag)+strFlag.GetLength());
		CString strSendNo = GetKeyString(strTemp,">","<");
		strCompany.Trim();
		strSendNo.Trim();
		strRecordInfo += "\r\n" + strCompany;
		strRecordInfo += "\t" + strSendNo;
	}
	else if (strStatus == "已取消")
	{
		//取消时间: 2011-04-07 20:53:35  取消原因： 客户取消 
		strFlag = "取消时间";
		strTemp = strContent.Mid(strContent.Find(strFlag));
		CString strCancelTime = GetKeyString(strTemp,"</strong>","</td>");
		strCancelTime.Trim();
		strRecordInfo += "\r\n" + strCancelTime;
		strFlag = "取消原因";
		strTemp = strTemp.Mid(strTemp.Find(strFlag));
		CString strCancelReason = GetKeyString(strTemp,"</strong>","</td>");
		strCancelReason.Trim();
		strRecordInfo += "\r\n" + strCancelReason;
	}
	/*收 货 人： 
		杜佳艺 <BR>收货地址： 中国 北京市 朝阳区 亮马河南路14号 外交办公大楼2-102A ， 100600 <BR>联系电话： 
		13691040531 </TD>*/
	strFlag = "收 货 人";
	strTemp = strContent.Mid(strContent.Find(strFlag));
	strTemp = strTemp.Left(strTemp.Find("</td>"));
	strTemp.Replace("<br>","\r\n");
	strTemp.Replace("<br />","\r\n");
	strTemp.Trim();
	strRecordInfo += "\r\n" + strTemp;
	//送货方式：</STRONG> <BR>快递送货上门 . 	送货时间不限 </TD>
	strFlag = "送货方式：";
	strTemp = strContent.Mid(strContent.Find(strFlag));
	CString strSendType = GetKeyString(strTemp,"</strong>","</td>");
	strSendType.Trim();
	strSendType.Replace("<br>","\t");
	strSendType.Replace("<br />","\t");
	strRecordInfo += "\r\n" + strSendType;

	//付款方式：</STRONG> <BR>银行转帐
	strFlag = "付款方式：";
	strTemp = strContent.Mid(strContent.Find(strFlag));
	CString strPayType = GetKeyString(strTemp,"</strong>","</td>");
	strPayType.Trim();
	strPayType.Replace("<br>","");
	strPayType.Replace("<br />","");
	strRecordInfo += "\r\n" + strPayType;
	//缺货处理方式：</STRONG> 发送有的商品
	strFlag = "缺货处理方式";
	strTemp = strContent.Mid(strContent.Find(strFlag));
	CString strDeault = GetKeyString(strTemp,"</strong>","</td>");
	strDeault.Trim();
	strDeault.Replace("<br>","");
	strDeault.Replace("<br />","");
	strRecordInfo += "\r\n缺货处理方式: " + strDeault;
	//是否需要发票
	strFlag = "是否需要发票";
	strTemp = strContent.Mid(strContent.Find(strFlag));
	CString strBill = GetKeyString(strTemp,"</strong>","</td>");
	strBill.Trim();
	strBill.Replace("<br>","");
	strBill.Replace("<br />","");
	strRecordInfo += "\r\n是否需要发票: " + strBill;
	strRecordInfo += "\r\n/************************************************************************/";
	strFlag = "/product/";
	//strTemp开始
	while (true)
	{
		if (strTemp.Find(strFlag) < 0)
		{
			break;
		}
		CString strGoodInfo = GetKeyString(strTemp,strFlag,"</tr>");
		//名称
		CString strTempEx = GetKeyString(strGoodInfo,">","<");
		strTempEx.Trim();
		strRecordInfo +="\r\n" + strTempEx;
		//价格
		strTempEx = "<td";
		strGoodInfo = strGoodInfo.Mid(strGoodInfo.Find(strTempEx)+strTempEx.GetLength());
		strTempEx = GetKeyString(strGoodInfo,">","<");
		strTempEx.Trim();
		strRecordInfo +="\r\n" + strTempEx;
		//折扣
		strTempEx = "<td";
		strGoodInfo = strGoodInfo.Mid(strGoodInfo.Find(strTempEx)+strTempEx.GetLength());
		strTempEx = GetKeyString(strGoodInfo,">","<");
		strTempEx.Trim();
		strRecordInfo +="\r\n" + strTempEx;
		//数量
		strTempEx = "<td";
		strGoodInfo = strGoodInfo.Mid(strGoodInfo.Find(strTempEx)+strTempEx.GetLength());
		strTempEx = GetKeyString(strGoodInfo,">","<");
		strTempEx.Trim();
		strRecordInfo +="\r\n" + strTempEx;
		//小计
		strTempEx = "<td";
		strGoodInfo = strGoodInfo.Mid(strGoodInfo.Find(strTempEx)+strTempEx.GetLength());
		strTempEx = GetKeyString(strGoodInfo,">","<");
		strTempEx.Trim();
		strRecordInfo +="\r\n" + strTempEx;
		//
		strRecordInfo += "\r\n/************************************************************************/";
		strTemp = strTemp.Mid(strTemp.Find(strFlag) + strFlag.GetLength());
	}
	CString strFile = "";
	strFile.Format("C:\\test\\%s.txt",strNo);

	CFile file;
	file.Open(strFile,CFile::modeCreate | CFile::modeWrite);
	file.Write(strRecordInfo,strRecordInfo.GetLength());
	file.Close();


	//已取消
	//全部已发
	//已收到
	

}

int XinHuaClass::Order(XinHuaOrderStr &orderStr)
{
	m_bClearCart = true;
	if (!Login(orderStr.strUserID,orderStr.strPwd))
	{
		return LOGIN_FAILED;
	}
	CArray<BookInfo,BookInfo>& bookInfo = orderStr.bookInfo;
	bool bRet = false;
	int iRet = -1;
	for (int i=0;i<bookInfo.GetCount();i++)
	{
		int iCount = 0;
		if (orderStr.iType == 0)	
		{
			iRet = -1;
			while(iRet < 0 && iCount ++ < 3)
			{
				iRet = AddToCart(bookInfo.GetAt(i).strNo,bookInfo.GetAt(i).fPrice);
			}
			
		}
		else	//通过商品编号下单
		{
			iRet = -1;
			while(iRet < 0 && iCount ++ < 3)
			{
				iRet = AddToCart(bookInfo.GetAt(i).strNo);
			}
			
		}
		if (iRet != 0)
		{
			return iRet;
		}
	}
	//接着进行确认订单
    return DoOrder(orderStr);
}

int XinHuaClass::DoOrder(XinHuaOrderStr &orderStr)
{
	string strServer = "www.xinhuabookstore.com";
	string strHeader = m_strHeader;
	srand(time(NULL));
	int iRet  = -1;
	CString strObject = "";	
	CString strData = "";
	time_t t;
	int iCount = 0;
	strHeader.append("\r\nReferer:http://www.xinhuabookstore.com/customer/shoppingcart/shoppingcart.jsp");
	strObject = "/customer/checkout/consignee.htm";
	iRet = m_webbase.PostPage(strObject,"",strHeader.c_str(),strServer.c_str());
	if (iRet < 0)
	{
		return -1;
	}
	//<input type="hidden" value="-449216821" id="randomCode" name="randomCode"/>
	CString strRet = CString(m_webbase.GetContentInfo());
	int iPos = strRet.Find("randomCode");
	CString strTemp = strRet.Left(iPos);
	iPos = strTemp.ReverseFind('<');
	strTemp = strRet.Mid(iPos);
	CString strRandomCode = GetKeyString(strTemp,"value=\"","\"");
	//http://www.xinhuabookstore.com/customer/shoppingcart/shoppingcart.jsp
	//进入购物中心
	/************************************************************************/
	//收货人信息
	map<CString,CString>::iterator it = m_mapProvince.find(orderStr.strProvince);
	if (it == m_mapProvince.end())
	{
		return ADDR_ERROR;
	}
	CString strCountryNo = "23";
	CString strProvinceNo = it->second;
	CString strCityNo = "";
	CString strAreaNo = "";
	iRet = GetSubNo(strProvinceNo,orderStr.strCity,strCityNo);
	if (iRet != 0)
	{
		return iRet;
	}
	iRet = GetSubNo(strCityNo,orderStr.strTown,strAreaNo);
	if (iRet != 0)
	{
		return iRet;
	}
	time(&t);
	//POST提交，获取地址代码
	ConvertGBKToUtf8(orderStr.strRecver);
	ConvertGBKToUtf8(orderStr.strAddr);
	
	strData.Format("orderTemplate.id=&orderTemplate.consignee=%s&orderTemplate.country.id=%s&orderTemplate.province.id=%s&orderTemplate.city.id=%s&orderTemplate.district.id=%s&orderTemplate.address=%s&orderTemplate.zipCode=%s&orderTemplate.mail=%s&orderTemplate.phone=%s&timestamp=%d000",
		orderStr.strRecver,
		strCountryNo,strProvinceNo,strCityNo,strAreaNo,
		orderStr.strAddr,
		orderStr.strPostCode,
		orderStr.strEmail,
		orderStr.strPhone,(long)t);
	strObject = "/customer/checkout/opearateConsignee.htm";
	strHeader = m_strHeader;
	strHeader.append("\r\nReferer:http://www.xinhuabookstore.com/customer/checkout/consignee.htm");
	//strHeader.append("\r\nx-requested-with:XMLHttpRequest");
	//strHeader.append("\r\nUA-CPU:x86");
	//strHeader.append("\r\nAccept-Language:zh-cn");
	iRet = m_webbase.PostPage(strObject,strData,strHeader.c_str(),strServer.c_str());
	strRet = CString(m_webbase.GetContentInfo());
	ConvertUtf8ToGBK(strRet);
	if (strRet.Find("成功") < 0)
	{
		return ADDR_ERROR;
	}	
	//判断，是否正确，并且拿到地址编号进行保存
	CString strAddrNo = GetKeyString(strRet,":",",");

	//发送方式
	/************************************************************************/
	time(&t);
	strObject.Format("/customer/checkout/deliverySetting.htm?areaId=%s&listPrice=%s&salePrice=%s&timestamp=%d000",
		strAreaNo,
		m_strListPrice,
		m_strSalePrice,
		(long)t);
	iRet = m_webbase.GetPage(strObject,strHeader.c_str(),strServer.c_str());
	strRet = CString(m_webbase.GetContentInfo());
	ConvertUtf8ToGBK(strRet);
	CString strdeliveryTypeId = "";
	CString strFee = "";	
	int iSendType = (int)orderStr.eSendType;
	if (iSendType > 2)
	{
		iSendType = 0;
	}
	time(&t);
	if (!orderStr.bCanNormalSend && strRet.Find("快递送货上门") < 0 )
	{
		return NOSPEEDSEND;
	}
	if (strRet.Find("快递送货上门") > 0)
	{		
		CString strTemp = strRet.Mid(strRet.Find("}"));
		strFee = GetKeyString(strTemp,"fee\":",",");
		strdeliveryTypeId = GetKeyString(strTemp,"id\":",",");
		strObject.Format("/customer/checkout/paymentSetting.htm?areaId=%s&orderTemplateId=%s&deliveryTypeId=%s&deliveryFee=%s&deliveryTime=%d&timestamp=%d000",
			strAreaNo,strAddrNo,strdeliveryTypeId,strFee,iSendType,(long)t);
	}
	else
	{
		CString strTemp = strRet.Mid(strRet.Find("}"));
		strFee = GetKeyString(strTemp,"fee\":",",");
		strdeliveryTypeId = GetKeyString(strTemp,"id\":",",");
		strObject.Format("/customer/checkout/paymentSetting.htm?areaId=%s&orderTemplateId=%s&deliveryTypeId=%s&deliveryFee=%s&deliveryTime=2&timestamp=%d000",
			strAreaNo,strAddrNo,strdeliveryTypeId,strFee,(long)t);
	}
	iRet = m_webbase.GetPage(strObject,strHeader.c_str(),strServer.c_str());
	if (iRet < 0)
	{
		return -1;
	}
	strRet = CString(m_webbase.GetContentInfo());
	ConvertUtf8ToGBK(strRet);
	if( strRet.Find("国内顾客可以通过全国任何一家银行") < 0 )
	{
		return ORDER_ERROR;
	}
	//付款方式
	/************************************************************************/
	CString strPayType = "15"; //先用银行汇票转账方式
	//付款方式只取一种
	/*
	[{"description":"请您在邮局汇款单中的\"汇款方式\"中选择\"现金\"，\"收款人姓名\"中填写：四川文轩在线电子商务有限公司 ，在\"入帐汇款：汇入帐户\"中填写：100434268700010001，同时要填写汇款的金额。您订购的商品会在汇款到达文轩网帐户后发出。注意事项：办理邮局汇款时，请您务必在邮局汇款单的用途栏内注明订单号。<a href=\"\/help\/281\/411\" target=\"_blank\">查看邮局汇款使用帮助<\/a>","id":3,"index":0,"logo":null,"name":"邮局汇款","type":0},
	{"description":"国内顾客可以通过全国任何一家银行，向四川文轩在线电子商务有限公司的建设银行开立的账户进行银行电汇，到款时间一般为自办理汇款手续之日起2-7个工作日，您订购的商品会在汇款到达文轩网帐户后发出。户名：四川文轩在线电子商务有限公司：建设银行成都新华支行帐号：51001870836051517375 注意事项：办理银行电汇时，请您务必在电汇单的用途栏内注明订单号。","id":4,"index":0,"logo":null,"name":"银行转帐","type":0},
	{"description":"","id":6,"index":0,"logo":"\/customer\/checkout\/images\/bank\/bank_004-2.gif","name":"建设银行","type":1},
	{"description":"","id":9,"index":0,"logo":"\/customer\/checkout\/images\/bank\/y_zfb.gif","name":"支付宝","type":1},
	{"description":null,"id":22,"index":0,"logo":null,"name":"网上支付","type":1},
	{"description":null,"id":26,"index":0,"logo":null,"name":"支付宝网银","type":1},
	{"description":null,"id":33,"index":0,"logo":"\/customer eckout\/images\/bank\/y_cft.gif","name":"财付通","type":1}]
	*/
	time(&t);
	strObject.Format("/customer/checkout/payment_save_json.htm?orderTemplateId=%s&refundmentMeans=1&paymentId=%s&timestamp=%d000",strAddrNo,strPayType,(long)t);
	iRet = m_webbase.GetPage(strObject,strHeader.c_str(),strServer.c_str());
	if (iRet < 0)
	{
		return -1;
	}
	strRet = CString(m_webbase.GetContentInfo());
	ConvertUtf8ToGBK(strRet);
	if (strRet.Find("orderTemplateId") < 0)
	{
		return ORDER_ERROR;
	}
	CString strPayNo = GetKeyString(strRet,"orderTemplateId\":","}");
	CString strResHead = CString(m_webbase.GetCookie().c_str());
	
	strData.Format("orderTemplateId=%s&useAdvanceAccount=true&excludeUnion=0&randomCode=%s&presentId=&x=55&y=24",strPayNo,strRandomCode);
	strObject = "/customer/checkout/submit.htm";

	iRet = m_webbase.PostPage(strObject,strData,strHeader.c_str(),strServer.c_str());
	strRet = CString(m_webbase.GetContentInfo());
	ConvertUtf8ToGBK(strRet);
	if(strRet.Find("?orderId=") < 0)
	{
		return ORDER_ERROR;
	}
	return 0;
}

int XinHuaClass::GetSubNo(CString strParantNo,CString strKey,CString &strNo)
{
	strNo = "";
	string strServer = "www.xinhuabookstore.com";
	string strHeader = m_strHeader;
	srand(time(NULL));
	int iRet  = -1;
	CString strObject = "";	
	time_t t;
	int iCount = 0;	
	time(&t);
	strObject.Format("/js/area/area_json.jsp?id=%s",strParantNo);
	strHeader.append("\r\nReferer:http://www.xinhuabookstore.com/customer/checkout/consignee.htm");
	iCount = 0;
	iRet  = -1;
	while( iRet < 0 && iCount++ < 3 )
	{
		iRet = m_webbase.GetPage(strObject,strHeader.c_str(),strServer.c_str());		
	}
	if (iRet < 0)
	{
		return -1;
	}
	CString strRet = CString(m_webbase.GetContentInfo());
	ConvertUtf8ToGBK(strRet);
	if (strRet.Find(strKey) < 0)
	{
		return ADDR_ERROR;
	}
	CString strTemp = strRet.Left(strRet.Find(strKey));
	strTemp = strTemp.Mid(strTemp.ReverseFind('{'));
	strNo = GetKeyString(strTemp,":",",");
	if (strNo == "")
	{
		return ADDR_ERROR;
	}
	return 0;
}

int XinHuaClass::AddToCart(CString strGoodNo)
{
	string strServer = "www.xinhuabookstore.com";
	string strHeader = m_strHeader;
	srand(time(NULL));
	int iRet  = -1;
	int iLoopCount = 0;
	CString strFailReason = "";
	CString strObject = "";	
	time_t t;
	time(&t);
	strObject.Format("/customer/shoppingcart/add.htm?commodityId=%s&quantity=1&timestamp=%d000&flag=true",strGoodNo,(long)t);
	//添加商品进购物车
	iRet = m_webbase.GetPage(strObject,strHeader.c_str(),strServer.c_str());
	if (iRet < 0)
	{
		return -1;
	}
	CString strRet = CString(m_webbase.GetContentInfo());
	ConvertUtf8ToGBK(strRet);
	if (strRet.Find("\"listPrice\":") >=0 )
	{
		//得到listPrice和realPrice
		strRet = strRet.Mid(strRet.ReverseFind(']'));
		m_strListPrice = GetKeyString(strRet,"\"listPrice\":",",");
		m_strSalePrice = GetKeyString(strRet,"\"salePrice\":",",");
		return 0;
	}
	return -1;
}

int XinHuaClass::AddToCart(CString strBookNo,float fPrice)
{
	//根据书号和价格找到商品编号,对比得到价格一致的商品编号,添加进购物车
	//http://www.xinhuabookstore.com/search/?keyword=9787530963852
	string strServer = "www.xinhuabookstore.com";
	string strHeader = m_strHeader;
	srand(time(NULL));
	int iRet  = -1;
	int iLoopCount = 0;
	CString strFailReason = "";
	CString strObject = "";	
	strObject.Format("/search/?keyword=%s",strBookNo);		
	iRet = m_webbase.GetPage(strObject,strHeader.c_str(),strServer.c_str());
	if (iRet < 0)
	{
		return -1;
	}
	CString strRet = CString(m_webbase.GetContentInfo());
	//解析出价格，得到相等的再得到商品编号
	//定价：23.00元
	ConvertUtf8ToGBK(strRet);
	CString strTemp = strRet;
	CString strPrice = "";
	CString strFlag = "定价";
	int iPos = -1;
	CString strNo = "";
	while(true)
	{
		iPos = strTemp.Find(strFlag);
		if (iPos < 0)
		{
			//未找到商品
			return BOOKNOTFOUND;
		}		
		strPrice = GetKeyString(strRet,"定价：","元");
		if (atof(strPrice) == fPrice)
		{
			//取得商品编号add_to_cart(1724439, 
			strNo = GetKeyString(strRet,"add_to_cart(",",");
			break;
		}
		strTemp = strTemp.Mid(iPos + strFlag.GetLength());
	}
	if(strNo == "")
	{
		//未找到价格匹配的商品
		return NOMATCHPRICEBOOK;
	}
	time_t t;
	time(&t);
	strObject.Format("/customer/shoppingcart/add.htm?commodityId=%s&quantity=1&timestamp=%d000&flag=true",strNo,(long)t);
	//添加商品进购物车
	iRet = m_webbase.GetPage(strObject,strHeader.c_str(),strServer.c_str());
	if (iRet < 0)
	{
		return -1;
	}
	strRet = CString(m_webbase.GetContentInfo());
	ConvertUtf8ToGBK(strRet);
	if (strRet.Find("\"listPrice\":") >=0 )
	{
		//得到listPrice和realPrice
		strRet = strRet.Mid(strRet.ReverseFind(']'));
		m_strListPrice = GetKeyString(strRet,"\"listPrice\":",",");
		m_strSalePrice = GetKeyString(strRet,"\"salePrice\":",",");
		return 0;
	}
	return -1;
}

int XinHuaClass::GetAppPath(CString &strAppPath)
{
	int iRet = -3;
	try
	{
		TCHAR szBuff[_MAX_PATH];
		VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szBuff, _MAX_PATH));
		strAppPath.Format("%s",szBuff);
		int pos=strAppPath.ReverseFind('\\');
		strAppPath=strAppPath.Left(pos+1);		
		iRet = 0;
	}
	catch (...)
	{

	}
	return iRet;	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CString XinHuaClass::GetKeyString(CString &srcStr,CString strBefore,CString strAfter)
{
	int iPosBefore = 0;
	int iPosAfter = 0;
	iPosBefore = srcStr.Find(strBefore) + strBefore.GetLength();
	if (iPosBefore < strBefore.GetLength())
	{
		return "";
	}
	iPosAfter = srcStr.Find(strAfter,iPosBefore);
	if (iPosBefore < 0 || iPosAfter < 0 || iPosAfter - iPosBefore < 0 )
	{
		return "";
	}
	else
	{
		return srcStr.Mid(iPosBefore,iPosAfter - iPosBefore);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void XinHuaClass::ConvertUtf8ToGBK(CString& strUtf8) 
{
	int len = 0;
	unsigned short * wszGBK = NULL;
	char * szGBKt = NULL;
	char * szGBKs = NULL;

	len = MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, NULL,0);
	wszGBK = new unsigned short[len+1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, (LPWSTR)wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL); 
	szGBKt  =new char[len + 1];
	memset(szGBKt, 0, len + 1);
	WideCharToMultiByte (CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBKt, len, NULL,NULL);

	szGBKs  =new char[len + 1];
	memset(szGBKs, 0, len + 1);
	//转换Gb码繁体到Gb码简体，使用API函数LCMapString 
	LCMapString( 0x0804, LCMAP_SIMPLIFIED_CHINESE, szGBKt, -1, szGBKs, len);
	strUtf8 = szGBKs;
	delete[] szGBKs;
	delete[] szGBKt;
	delete[] wszGBK;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void XinHuaClass::ConvertGBKToUtf8(CString& strGBK) 
{
	int len=MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, NULL,0);
	unsigned short * wszUtf8 = new unsigned short[len+1];
	memset(wszUtf8, 0, len * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, (LPWSTR)wszUtf8, len);

	len = WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)wszUtf8, -1, NULL, 0, NULL, NULL); 
	char *szUtf8=new char[len + 1];
	memset(szUtf8, 0, len + 1);
	WideCharToMultiByte (CP_UTF8, 0, (LPWSTR)wszUtf8, -1, szUtf8, len, NULL,NULL);

	strGBK = szUtf8;
	delete[] szUtf8;
	delete[] wszUtf8;
	return ;
}
void XinHuaClass::InitProvinceInfo()
{
	CString strData = "{'id':152,'name':'北京市'},{'id':177,'name':'天津市'},{'id':174,'name':'上海市'},{'id':183,'name':'重庆市'},	{'id':165,'name':'江苏省'},{'id':182,'name':'浙江省'},{'id':155,'name':'广东省'},{'id':156,'name':'广西'},{'id':153,'name':'福建省'},{'id':162,'name':'湖北省'},{'id':163,'name':'湖南省'},{'id':175,'name':'四川省'},{'id':171,'name':'山东省'},{'id':150,'name':'安徽省'},{'id':160,'name':'河南省'},{'id':166,'name':'江西省'},{'id':159,'name':'河北省'},{'id':172,'name':'山西省'},{'id':157,'name':'贵州省'},{'id':181,'name':'云南省'},{'id':154,'name':'甘肃省'},{'id':180,'name':'新疆'},{'id':161,'name':'黑龙江省'},{'id':169,'name':'宁夏'},{'id':173,'name':'陕西省'},{'id':178,'name':'西藏'},{'id':168,'name':'内蒙古'},{'id':164,'name':'吉林省'},{'id':170,'name':'青海省'},{'id':167,'name':'辽宁省'},{'id':158,'name':'海南省'},{'id':179,'name':'香港'},{'id':151,'name':'澳门'},{'id':176,'name':'台湾'},";
	CString strProvince = "";
	CString strNo = "";
	while(true)
	{
		CString strTemp = strData.Left(strData.Find("}"));		
		if (strTemp.GetLength() < 3)
		{
			break;
		}
		strData = strData.Mid(strData.Find("}") + 1);
		strProvince = GetKeyString(strTemp,"name':'","'");
		strNo = GetKeyString(strTemp,"'id':",",");
		m_mapProvince[strProvince] = strNo;
	}	
}

int XinHuaClass::Test(XinHuaOrderStr &orderStr)
{
	string strServer = "www.xinhuabookstore.com";
	string strHeader = m_strHeader;
	srand(time(NULL));
	int iRet  = -1;
	int iLoopCount = 0;
	CString strFailReason = "";
	CString strObject = "";	
	time_t t;
	time(&t);
	strObject.Format("/js/area/area_json.jsp?id=23");
	//添加商品进购物车
	iRet = m_webbase.GetPage(strObject,strHeader.c_str(),strServer.c_str());
	if (iRet < 0)
	{
		return -1;
	}
	CString strRet = CString(m_webbase.GetContentInfo());
	ConvertUtf8ToGBK(strRet);
	if (strRet.Find("\"listPrice\":") >=0 )
	{
		return 0;
	}
	return -1;	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString XinHuaClass::URLEncode(CString sIn)
{
	CString sOut;
	const int nLen = sIn.GetLength() + 1;
	register LPBYTE pOutTmp = NULL;
	LPBYTE pOutBuf = NULL;
	register LPBYTE pInTmp = NULL;
	LPBYTE pInBuf =(LPBYTE)sIn.GetBuffer(nLen);
	//alloc out buffer
	pOutBuf = (LPBYTE)sOut.GetBuffer(nLen*3);

	if(pOutBuf)
	{
		pInTmp   = pInBuf;
		pOutTmp = pOutBuf;
		// do encoding
		while (*pInTmp)
		{
			if(isalnum(*pInTmp) || '-'==*pInTmp || '_'==*pInTmp || '.'==*pInTmp)
				*pOutTmp++ = *pInTmp;
			else if(isspace(*pInTmp))
				*pOutTmp++ = '+';
			else
			{
				*pOutTmp++ = '%';
				*pOutTmp++ = toHex(*pInTmp>>4);//高4位
				*pOutTmp++ = toHex(*pInTmp%16);//低4位
			}
			pInTmp++;
		}
		*pOutTmp = '\0';
		sOut.ReleaseBuffer();
	}
	sIn.ReleaseBuffer();

	return sOut;
}
