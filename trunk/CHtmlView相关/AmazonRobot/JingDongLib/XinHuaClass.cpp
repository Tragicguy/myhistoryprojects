#include "StdAfx.h"
#include "XinHuaClass.h"

JinDongClass::JinDongClass(void)
{
	CString strAppPath = "";
	GetAppPath(strAppPath);
	char *path=new char[200];
	GetCurrentDirectory(200,path);
	//m_path=path;//得到逐鹿进
	m_path=path;//得到目录

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
JinDongClass::~JinDongClass(void)
{
}
bool JinDongClass::Login(CString strUser,CString strPwd)
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

bool JinDongClass::DoLogin(CString strUser,CString strPwd)
{
	string strServer = "passport.360buy.com";
	string strHeader = m_strHeader;
	srand(time(NULL));
	int iRet  = -1;
	int iLoopCount = 0;
	CString strFailReason = "";
	CString strObject = "";
	strObject="/new/LoginService.aspx?uuid=5fd78a5a-9dae-4f1e-8087-341c7851043e&ReturnUrl=http%3A%2F%2Fwww.360buy.com%2F&r=0.8358589050791421";
	time_t t;
	time(&t);
	strHeader.append("\r\nReferer:https://passport.360buy.com/new/login.aspx?ReturnUrl=http%3A%2F%2Fwww.360buy.com%2F");
	//strObject.Format("/customer/getValidateNumber.htm?d=%d000",(long)t);
	/*iRet = m_webbase.GetPage(strObject,strHeader.c_str(),strServer.c_str(),80,true,m_strPicPath.c_str());
	if (iRet < 0)
	{
	return false;
	}	*/
	//	strObject = "/customer/login.htm";
	//	string strCode = m_objCheck.CheckCode(m_strPicPath);
	CString strData = "";
	ConvertGBKToUtf8(strUser);

	strData.Format("loginname=%s&loginpwd=%s&authcode=&chkRememberUsername=onGET /new/JDVerification.aspx?&uid=5fd78a5a-9dae-4f1e-8087-341c7851043e&yys=1305422697625",strUser,strPwd);
	iRet = m_webbase.PostPage(strObject,strData,strHeader.c_str(),strServer.c_str());
	CString strRet = CString(m_webbase.GetContentInfo());
	ConvertUtf8ToGBK(strRet);
	if (strRet.Find("su") >=0 )
	{
		time(&t);
		strObject.Format("/purchase/ajaxServer/ForShoppingCart_pop.aspx?roid=0.7632127128916124&action=clearCart");	
		strHeader = m_strHeader;

		strHeader.append("\r\nReferer:http://jd2008.360buy.com/purchase/shoppingcart_pop.aspx?backurl=http://www.360buy.com/product/1000340060.html");
		strServer = "jd2008.360buy.com";
		if (m_bClearCart)
		{
			//清空购物车
			m_webbase.PostPage(strObject,"",strHeader.c_str(),strServer.c_str());			
		}
		CString strRet3 = CString(m_webbase.GetContentInfo());
		//ConvertUtf8ToGBK(strRet3);
		return true;
	}	
	return false;
}

string JinDongClass::Query(JingDongOrderStr &orderStr)
{
	m_bClearCart = false;
	if (!Login(orderStr.strUserID,orderStr.strPwd))
	{
		return "登陆失败";
	}

	return getOrderHtml(orderStr);
}
string JinDongClass::getOrderHtml(JingDongOrderStr &orderStr)
{
	//http://www.xinhuabookstore.com/customer/order/list.htm?pagination.pageSize=2000&pagination.skipSize=10&pagination.currentPage=1
	string strServer = "member.winxuan.com";
	string strHeader = m_strHeader;
	srand(time(NULL));
	int iRet  = -1;
	CString strObject = "";	
	CString strData = "";
	time_t t;
	int iCount = 0;
	strHeader.append("\r\nReferer:http://member.winxuan.com/member/orderDetailAction_list.action");
	strObject = "/member/orderDetailAction_get.action";
	CString data ;
	data="orderDetail.sub_order_id="+orderStr.xinhuaOrderNum+"&orderDetail.pay_model=&pager_normal_actionValue=http%3A%2F%2Fmember.winxuan.com%2Fmember%2ForderDetailAction_list.action&pager_normal_formName=subOrder&pager_normal_pageCurr=1&pager_normal_pageMethod=&pager_normal_pageNum=&pager_cancel_actionValue=http%3A%2F%2Fmember.winxuan.com%2Fmember%2ForderDetailAction_list.action&pager_cancel_formName=subOrder&pager_cancel_pageCurr=1&pager_cancel_pageMethod=&pager_cancel_pageNum=";
	iRet = m_webbase.PostPage(strObject,data.GetBuffer(),strHeader.c_str(),strServer.c_str());
	CString strRet = CString(m_webbase.GetContentInfo());
	ConvertUtf8ToGBK(strRet);
	return strRet.GetBuffer();
}
int JinDongClass::DoQuery(JingDongOrderStr &orderStr)
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

void JinDongClass::AnanyzerGood(CString strNo,CString& strContent)
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

int JinDongClass::Order(JingDongOrderStr &orderStr)
{
	m_bClearCart = true;
	if (!Login(orderStr.strUserID,orderStr.strPwd))
	{
		return LOGIN_FAILED;
	}
	CArray<BookInfo,BookInfo>& bookInfo = orderStr.bookInfo;
	bool bRet = false;
	int iRet = -1;
	int count = bookInfo.GetCount();
	bool isQueHuoAll = true;
	bool isQueHuo = false;
	for (int i=0;i<count;i++)
	{
		int iCount = 0;
		if (orderStr.iType[i] == 0)	
		{
			iRet = -1;
			/*while(iRet < 0 && iCount ++ < 3)
			{
			iRet = AddToCart(bookInfo.GetAt(i).strNo,bookInfo.GetAt(i).fPrice);
			}*/

		}
		else	//通过商品编号下单
		{
			iRet = -1;
			/*	while(iRet < 0 && iCount ++ < 3)
			{*/
			iRet = AddToCart(bookInfo.GetAt(i).strNo,orderStr.num[i]);
			//}

		}
		if (iRet != 0)
		{
			orderStr.res.push_back("缺货");
			isQueHuo = true;
			//return iRet;
		}
		else 
		{
			orderStr.res.push_back("有货");
			isQueHuoAll = false;
		}
	}
	if(isQueHuo)//有缺货的 
	{
		return ADD_TOCARTERROR ;
	}
	if(isQueHuoAll)///都缺货
	{
		return ADD_TOCARTERROR ;
	}
	//接着进行确认订单
	return DoOrder(orderStr);
}

int JinDongClass::DoOrder(JingDongOrderStr &orderStr)
{
	//string strServer = "order.winxuan.com";
	string strServer = "jd2008.360buy.com";
	string strHeader = m_strHeader;
	srand(time(NULL));
	int iRet  = -1;
	CString strObject = "";	
	CString strData = "";
	time_t t;
	int iCount = 0;

	strObject = "/purchase/shoppingcart_pop.aspx";
	iRet = m_webbase.GetPage(strObject,strHeader.c_str(),strServer.c_str());
	CString strRet = CString(m_webbase.GetContentInfo());
	// strHeader.append("\r\nReferer:http://www.xinhuabookstore.com/customer/shoppingcart/shoppingcart.jsp");
	strObject = "/purchase/orderinfo_pop_main.aspx";
	iRet = m_webbase.GetPage(strObject,strHeader.c_str(),strServer.c_str());
	if (iRet < 0)
	{
		return -1;
	}
	//<input type="hidden" value="-449216821" id="randomCode" name="randomCode"/>
	  strRet = CString(m_webbase.GetContentInfo());
	 int iPos = strRet.Find("randomCode");
	CString strTemp = strRet.Left(iPos);
	iPos = strTemp.ReverseFind('<');
	strTemp = strRet.Mid(iPos);
	CString strRandomCode = GetKeyString(strTemp,"subOrderId=","\"");
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
	bool isTeShu = false;
	 

	strObject = "/purchase/ajaxServer/ForOrderInfo_pop_main.aspx?roid=&action=showForm_consignee";
	iRet = m_webbase.GetPage(strObject,strHeader.c_str(),strServer.c_str());
strRet = CString(m_webbase.GetContentInfo());
	if( orderStr.strCity.Find(orderStr.strProvince)==-1)
	{

		iRet = GetSubNo(strProvinceNo,orderStr.strCity,strRandomCode,strCityNo);
		if (iRet != 0)
		{
			return iRet;
		}
		iRet = GetSubCityNo(strProvinceNo  ,orderStr.strTown,strCityNo,strAreaNo);
		if (iRet != 0)
		{
			return iRet;
		}
	}
	else//特殊地区
	{
		iRet = GetSubNo(strProvinceNo,orderStr.strTown,strRandomCode,strCityNo);
		if (iRet != 0)
		{
			return iRet;
		}
		isTeShu = true;
	}
	time(&t);
	//POST提交，获取地址代码
	ConvertGBKToUtf8(orderStr.strRecver);
	ConvertGBKToUtf8(orderStr.strAddr);
	orderStr.strTown = orderStr.strProvince+orderStr.strCity+orderStr.strTown;	//这里需要是三个地址相加
	ConvertGBKToUtf8(orderStr.strTown);

	//这里面还有坐机和邮箱你需要自己再加上
	strData.Format("<table><addressName><![CDATA[%s]]></addressName><address><![CDATA[%s]]></address><message><![CDATA[%s]]></message><phone><![CDATA[0730-5880186]]></phone><email><![CDATA[58357601@qq.com]]></email><postcode><![CDATA[%s]]></postcode><province><![CDATA[%s]]></province><city><![CDATA[%s]]></city><county><![CDATA[%s]]></county><addressAll><![CDATA[%s%s]]></addressAll><isEasyBuyAddress>false</isEasyBuyAddress><easyBuyAddressId>0</easyBuyAddressId></table>",
		orderStr.strRecver,
		orderStr.strAddr,
		orderStr.strPhone,
		orderStr.strPostCode,          
		strProvinceNo,		
		strCityNo,
		strAreaNo,
		orderStr.strTown,
		orderStr.strAddr);
	strObject.Format("/purchase/ajaxServer/ForOrderInfo_pop_main.aspx?roid=0.630056%d&action=savePart_consignee&isAddAddress=1",time);
	strHeader = "";
	strHeader.append("\r\nUser-Agent:Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.2; SV1; .NET CLR 1.1.4322; .NET CLR 2.0.50727; .NET CLR 3.0.04506.30; .NET CLR 3.0.04506.648)");
	strHeader.append("\r\nAccept: */*");
	strHeader.append("\r\nConnection: Keep-Alive");
	strHeader.append("\r\nReferer:http://jd2008.360buy.com/purchase/orderinfo_pop_main.aspx");
	strHeader.append("\r\nUA-CPU:x86");	//注意这一项尽量加上
	strHeader.append("\r\nAccept-Language:zh-cn");
	iRet = m_webbase.PostPage(strObject,strData,strHeader.c_str(),strServer.c_str());
	strRet = CString(m_webbase.GetContentInfo());	
	//此处看结果

	//支付方式开始
	strData = "<table><IdPaymentType><![CDATA[4]]></IdPaymentType><IdShipmentType><![CDATA[70]]></IdShipmentType><IdPickSite><![CDATA[0]]></IdPickSite><CODTime><![CDATA[1]]></CODTime><PaymentWay><![CDATA[0]]></PaymentWay><IdAgent><![CDATA[]]></IdAgent><IsCodInform><![CDATA[0]]></IsCodInform><ShipTime><![CDATA[0]]></ShipTime></table>";
	strData = "";
	strObject = "/purchase/ajaxServer/ForOrderInfo_pop_main.aspx?roid=0.8882640386713443&action=savePayTypeAndShipType";
	strHeader = m_strHeader;
	strHeader.append("\r\nReferer:http://jd2008.360buy.com/purchase/orderinfo_pop_main.aspx");

	iRet = m_webbase.PostPage(strObject,strData,strHeader.c_str(),strServer.c_str());
	strRet = CString(m_webbase.GetContentInfo());
//	ConvertUtf8ToGBK(strRet);
	/*if (strRet.Find("0") != -1)
	{
		return ADDR_ERROR;
	}	*/
	CString dataMid ="subOrderId="+ strRandomCode;
	/*if( orderStr.strTown.Find("其他")!=-1)
	{
		return NOSPEEDSEND;
	}
	if( orderStr.strTown.Find("其它")!=-1)
	{
		return NOSPEEDSEND;
	}*/
	/*strObject = "/main/order/Order_stockGoods.do";
	iRet = m_webbase.PostPage(strObject,dataMid,strHeader.c_str(),strServer.c_str());
	strRet = CString(m_webbase.GetContentInfo());*/
	//dataMid = "orderConsignee.name=%E8%BE%9C%E9%B9%A4&orderConsignee.country=23&orderConsignee.province=175&orderConsignee.city=1507&orderConsignee.district=1528&orderConsignee.address=%E5%8F%8C%E6%A1%A5%E8%B7%AF158%E6%A0%8B2%E5%8D%95%E5%85%83702&orderConsignee.zip=100000&orderConsignee.mobile=18980889086&orderConsignee.phone=&updateFlagOrInsert=&orderConsignee.consigneeId=313273&addressfalg=false&useAddress=313273&oname=&country=23&province=&city=&district=&address=&zip=&mobile=&phone=&=&falgAd=&subOrder.distributionModel=1&orderConsignee.deliveryDate=%E5%91%A8%E4%B8%80%E8%87%B3%E5%91%A8%E4%BA%94&payPlatformStr=6_2&couponId=0&bookValue=0&advance=Y&advanceTextValue=11064.60&advanceTextValue=25.00&subOrder.sellingAmount=20.00&subOrder.subOrderId="+strRandomCode+"&subOrder.freight=5&subOrder.receivableAmount=0.00&subOrder.discountAmount=12.80&subOrder.issueInvoice=N&headContent=";
	/*dataMid.Format("orderConsignee.name=%s&orderConsignee.country=%s&orderConsignee.province=%s&orderConsignee.city=%s&orderConsignee.district=%s&orderConsignee.address=%s&orderConsignee.zip=%s&orderConsignee.mobile=%s&orderConsignee.phone=&updateFlagOrInsert=true&orderConsignee.consigneeId=&addressfalg=false&useAddress=%s&oname=%s&country=%s&province=%s&city=%s&district=%s&address=%s&zip=%s&mobile=%s&phone=&=&falgAd=false&subOrder.distributionModel=3&orderConsignee.deliveryDate=%s&payPlatformStr=6_2&couponId=0&bookValue=0&advance=Y&&subOrder.subOrderId=%s&subOrder.freight=5&subOrder.receivableAmount=0.00&subOrder.discountAmount=12.80&subOrder.issueInvoice=N&headContent=",
		orderStr.strRecver,
		strCountryNo,
		strProvinceNo,
		strCityNo,
		strAreaNo,
		orderStr.strAddr,
		orderStr.strPostCode,
		orderStr.strPhone,
		"%E5%8D%95%E9%80%89",
		orderStr.strRecver,
		strCountryNo,
		strProvinceNo,
		strCityNo,
		strAreaNo,
		orderStr.strAddr,
		orderStr.strPostCode,
		orderStr.strPhone,
		"%E5%91%A8%E4%B8%80%E8%87%B3%E5%91%A8%E4%BA%94",strRandomCode
		);*/
	strObject = "/purchase/ajaxServer/ForOrderInfo_pop_main.aspx?roid=0.7413889057125769&action=submitOrder&isReSubmit=0&passWord=&ck=";
	iRet = m_webbase.GetPage(strObject,strHeader.c_str(),strServer.c_str());
	strRet = CString(m_webbase.GetContentInfo());
//	ConvertUtf8ToGBK(strRet);
	int tt = 0;
	CString midRes =GetKeyString(strRet ,"<span class=\"orderMainText\">","<");
	//if(midRes.Find("支付成功")==-1)
	//{
	//	return ORDER_ERROR;
	//}
	//else
	//{
	orderStr.xinhuaOrderNum = GetKeyString(strRet,"您的订单号为：","的");
	if(orderStr.xinhuaOrderNum.IsEmpty())
	{
		orderStr.xinhuaOrderNum = strRandomCode;//订单号
	}
	//}
	//div class="verticalMiddle"><img src="../../images/success1.gif" />

	//	<span class="orderMainText">您的订单号为：20110519002395的订单已经支付成功！</span>


	//	<a href="javascript:toDetail(20110519002395);" class="f12blue" style="margin-left:35px;">查看订单状态&gt;&gt;</a></div>  
	//判断，是否正确，并且拿到地址编号进行保存
	//CString strAddrNo = GetKeyString(strRet,":",",");
	//      if(isTeShu)//特殊地质
	//   {
	//        strAreaNo = strCityNo;
	//   }
	////发送方式
	///************************************************************************/
	//time(&t);
	//strObject.Format("/customer/checkout/deliverySetting.htm?areaId=%s&listPrice=%s&salePrice=%s&timestamp=%d000",
	//	strAreaNo,
	//	m_strListPrice,
	//	m_strSalePrice,
	//	(long)t);
	//iRet = m_webbase.GetPage(strObject,strHeader.c_str(),strServer.c_str());
	//strRet = CString(m_webbase.GetContentInfo());
	//ConvertUtf8ToGBK(strRet);
	//CString strdeliveryTypeId = "";
	//CString strFee = "";	
	////int iSendType = (int)orderStr.eSendType;
	//int iSendType = 0;
	//if (iSendType > 2)
	//{
	//	iSendType = 0;
	//}
	//time(&t);
	//if (!orderStr.bCanNormalSend && strRet.Find("快递送货上门") < 0 )
	//{
	//	return NOSPEEDSEND;
	//}
	//if (strRet.Find("快递送货上门") > 0)
	//{		
	//	CString strTemp = strRet.Mid(strRet.Find("}"));
	//	strFee = GetKeyString(strTemp,"fee\":",",");
	//	strdeliveryTypeId = GetKeyString(strTemp,"id\":",",");
	//	strObject.Format("/customer/checkout/paymentSetting.htm?areaId=%s&orderTemplateId=%s&deliveryTypeId=%s&deliveryFee=%s&deliveryTime=%d&timestamp=%d000",
	//		strAreaNo,strAddrNo,strdeliveryTypeId,strFee,iSendType,(long)t);
	//}
	//else
	//{
	//	CString strTemp = strRet.Mid(strRet.Find("}"));
	//	strFee = GetKeyString(strTemp,"fee\":",",");
	//	strdeliveryTypeId = GetKeyString(strTemp,"id\":",",");
	//	strObject.Format("/customer/checkout/paymentSetting.htm?areaId=%s&orderTemplateId=%s&deliveryTypeId=%s&deliveryFee=%s&deliveryTime=2&timestamp=%d000",
	//		strAreaNo,strAddrNo,strdeliveryTypeId,strFee,(long)t);
	//}
	//iRet = m_webbase.GetPage(strObject,strHeader.c_str(),strServer.c_str());
	//if (iRet < 0)
	//{
	//	return -1;
	//}
	//strRet = CString(m_webbase.GetContentInfo());
	//ConvertUtf8ToGBK(strRet);
	//if( strRet.Find("国内顾客可以通过全国任何一家银行") < 0 )
	//{
	//	return ORDER_ERROR;
	//}
	////付款方式
	///************************************************************************/
	//CString strPayType = "15"; //先用银行汇票转账方式
	////付款方式只取一种
	///*
	//[{"description":"请您在邮局汇款单中的\"汇款方式\"中选择\"现金\"，\"收款人姓名\"中填写：四川文轩在线电子商务有限公司 ，在\"入帐汇款：汇入帐户\"中填写：100434268700010001，同时要填写汇款的金额。您订购的商品会在汇款到达文轩网帐户后发出。注意事项：办理邮局汇款时，请您务必在邮局汇款单的用途栏内注明订单号。<a href=\"\/help\/281\/411\" target=\"_blank\">查看邮局汇款使用帮助<\/a>","id":3,"index":0,"logo":null,"name":"邮局汇款","type":0},
	//{"description":"国内顾客可以通过全国任何一家银行，向四川文轩在线电子商务有限公司的建设银行开立的账户进行银行电汇，到款时间一般为自办理汇款手续之日起2-7个工作日，您订购的商品会在汇款到达文轩网帐户后发出。户名：四川文轩在线电子商务有限公司：建设银行成都新华支行帐号：51001870836051517375 注意事项：办理银行电汇时，请您务必在电汇单的用途栏内注明订单号。","id":4,"index":0,"logo":null,"name":"银行转帐","type":0},
	//{"description":"","id":6,"index":0,"logo":"\/customer\/checkout\/images\/bank\/bank_004-2.gif","name":"建设银行","type":1},
	//{"description":"","id":9,"index":0,"logo":"\/customer\/checkout\/images\/bank\/y_zfb.gif","name":"支付宝","type":1},
	//{"description":null,"id":22,"index":0,"logo":null,"name":"网上支付","type":1},
	//{"description":null,"id":26,"index":0,"logo":null,"name":"支付宝网银","type":1},
	//{"description":null,"id":33,"index":0,"logo":"\/customer eckout\/images\/bank\/y_cft.gif","name":"财付通","type":1}]
	//*/
	//time(&t);
	//strObject.Format("/customer/checkout/payment_save_json.htm?orderTemplateId=%s&refundmentMeans=1&paymentId=%s&timestamp=%d000",strAddrNo,strPayType,(long)t);
	//iRet = m_webbase.GetPage(strObject,strHeader.c_str(),strServer.c_str());
	//if (iRet < 0)
	//{
	//	return -1;
	//}
	//strRet = CString(m_webbase.GetContentInfo());
	//ConvertUtf8ToGBK(strRet);
	//if (strRet.Find("orderTemplateId") < 0)
	//{
	//	return ORDER_ERROR;
	//}
	//CString strPayNo = GetKeyString(strRet,"orderTemplateId\":","}");
	//CString strResHead = CString(m_webbase.GetCookie().c_str());
	//
	//strData.Format("orderTemplateId=%s&useAdvanceAccount=true&excludeUnion=0&randomCode=%s&presentId=&x=55&y=24",strPayNo,strRandomCode);
	//strObject = "/customer/checkout/submit.htm";

	//iRet = m_webbase.PostPage(strObject,strData,strHeader.c_str(),strServer.c_str());
	//strRet = CString(m_webbase.GetContentInfo());
	//ConvertUtf8ToGBK(strRet);
	//CString midt = GetKeyString(strRet,"?orderId=","\"" );
	//writeOrderNum(m_path+"\\res.txt",midt);
	// orderStr.xinhuaOrderNum = midt;
	//if(strRet.Find("?orderId=") < 0)
	//{
	//	return ORDER_ERROR;
	//}
	return 0;
}

int JinDongClass::GetSubNo(CString strParantNo,CString strKey,CString orderNum,CString &strNo)
{
	strNo = "";
	CString data = "pid="+strParantNo;
	string strServer = "jd2008.360buy.com";
	string strHeader = m_strHeader;
	srand(time(NULL));
	int iRet  = -1;
	CString strObject = "";	
	time_t t;
	int iCount = 0;	
	time(&t);
	// /main/order/gangedCty_listCounty.do
	strObject.Format("/purchase/ajaxServer/ForOrderInfo_pop_main.aspx?roid=0.3783642613355343&action=changeArea_province&areaId=%s",strParantNo);
	strHeader.append("\r\nReferer:http://jd2008.360buy.com/purchase/orderinfo_pop_main.aspx");
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
	//ConvertUtf8ToGBK(strRet);
	if (strRet.Find(strKey) < 0)
	{
		return ADDR_ERROR;
	}
	CString strTemp = strRet.Left(strRet.Find(strKey));
	strTemp = strTemp.Mid(strTemp.ReverseFind('='));
	strNo = GetKeyString(strTemp,"'","'");
	if (strNo == "")
	{
		return ADDR_ERROR;
	}
	return 0;
}
int JinDongClass::GetSubCityNo(CString strParantNo,CString strKey,CString strCityId,CString &strNo)
{
	strNo = "";
	CString data = "pid="+strParantNo;
	string strServer = "jd2008.360buy.com";
	string strHeader = m_strHeader;
	srand(time(NULL));
	int iRet  = -1;
	CString strObject = "";	
	time_t t;
	int iCount = 0;	
	time(&t);
	// 
	strObject.Format("/purchase/ajaxServer/ForOrderInfo_pop_main.aspx?roid=0.060631474600115864&action=changeArea_city&provinceId=%s&cityId=%s",strParantNo,strCityId);
	//strHeader.append("\r\nReferer:http://order.winxuan.com/main/order/Order_confOrderInfo.do?subOrderId="+orderNum);
	iCount = 0;
	iRet  = -1;
	while( iRet < 0 && iCount++ < 3 )
	{
		iRet = m_webbase.GetPage(strObject ,strHeader.c_str(),strServer.c_str());		
	}
	if (iRet < 0)
	{
		return -1;
	}
	CString strRet = CString(m_webbase.GetContentInfo());
 
	if (strRet.Find(strKey) < 0)
	{
		return ADDR_ERROR;
	}
	CString strTemp = strRet.Left(strRet.Find(strKey));
	strTemp = strTemp.Mid(strTemp.ReverseFind('='));
	strNo = GetKeyString(strTemp,"'","'");
	if (strNo == "")
	{
		return ADDR_ERROR;
	}
	return 0;
}
int JinDongClass::AddToCart(CString strGoodNo,CString num)
{
	string strServer = "jd2008.360buy.com";
	string strHeader = m_strHeader;
	srand(time(NULL));
	int iRet  = -1;
	int iLoopCount = 0;
	CString strFailReason = "";
	CString strObject = "";	
	time_t t;
	time(&t);
	strObject.Format("/purchase/InitCart.aspx?pid=%s&pcount=%s&ptype=1",strGoodNo,num);
	//添加商品进购物车
	iRet = m_webbase.GetPage(strObject,strHeader.c_str(),strServer.c_str());
	if (iRet < 0)
	{
		return -1;
	}
	CString strRet = CString(m_webbase.GetContentInfo());
	 
	//CString res = GetKeyString(strRet,"http://order.winxuan.com/main/order/ShopCart_listCart.do?msg=","\"");
	if (strRet.Find("成功")!=-1)
	{
		//得到listPrice和realPrice
		strRet = strRet.Mid(strRet.ReverseFind(']'));
		m_strListPrice = GetKeyString(strRet,"\"listPrice\":",",");
		m_strSalePrice = GetKeyString(strRet,"\"salePrice\":",",");
		return 0;
	}
	return -1;
}

int JinDongClass::AddToCart(CString strBookNo,float fPrice)
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

int JinDongClass::GetAppPath(CString &strAppPath)
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
CString JinDongClass::GetKeyString(CString &srcStr,CString strBefore,CString strAfter)
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
void JinDongClass::ConvertUtf8ToGBK(CString& strUtf8) 
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
void JinDongClass::ConvertGBKToUtf8(CString& strGBK) 
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
void JinDongClass::InitProvinceInfo()
{
	CString strData = "<option value='1'>北京</option><option value='2'>上海</option><option value='3'>天津</option><option value='4'>重庆</option><option value='5'>河北</option><option value='6'>山西</option><option value='7'>河南</option><option value='8'>辽宁</option><option value='9'>吉林</option><option value='10' selected>黑龙江</option><option value='11'>内蒙古</option><option value='12'>江苏</option><option value='13'>山东</option><option value='14'>安徽</option><option value='15'>浙江</option><option value='16'>福建</option><option value='17'>湖北</option><option value='18'>湖南</option><option value='19'>广东</option><option value='20'>广西</option><option value='21'>江西</option><option value='22'>四川</option><option value='23'>海南</option><option value='24'>贵州</option><option value='25'>云南</option><option value='26'>西藏</option><option value='27'>陕西</option><option value='28'>甘肃</option><option value='29'>青海</option><option value='30'>宁夏</option><option value='31'>新疆</option><option value='32'>台湾</option><option value='42'>香港</option><option value='43'>澳门</option><option value='84'>钓鱼岛</option>";
	CString strProvince = "";
	CString strNo = "";
	while(true)
	{
	    CString mid  = GetKeyString(strData,"<option","option>");
		//strData = strData.Mid(strData.Find("}") + 1);
		strProvince = GetKeyString(mid,">","<");
		strNo = GetKeyString(mid,"value='","'");
		if(strNo.IsEmpty())
		{
			break;
		}
		m_mapProvince[strProvince] = strNo;
		strData.Replace("<option"+mid+"option>","");
	}	
}

int JinDongClass::Test(JingDongOrderStr &orderStr)
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
CString JinDongClass::URLEncode(CString sIn)
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

bool JinDongClass::writeOrderNum(CString fileName,CString id)
{
	CFile file;
	bool  is = file.Open(fileName,CFile::modeCreate|CFile::modeWrite);
	file.Write(id,id.GetLength());
	file.Close();
	return true;
}