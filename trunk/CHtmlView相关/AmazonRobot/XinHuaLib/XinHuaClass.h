#pragma once
#include "../AmazonRobot/DataDefine.h"
#include "WebBase.h"
#include "CodeCheck.h"
#include <map>
using namespace std;
class XinHuaClass
{
public:
	XinHuaClass(void);
	~XinHuaClass(void);
	bool Login(CString strUser,CString strPwd);
	bool DoLogin(CString strUser,CString strPwd);
	int AddToCart(CString strBookNo,float fPrice);
	int AddToCart(CString strGoodNo);
	int Order(XinHuaOrderStr &orderStr);
	int GetAppPath(CString &strAppPath);
	int DoOrder(XinHuaOrderStr &orderStr);
	CString GetKeyString(CString &srcStr,CString strBefore,CString strAfter);
	void ConvertUtf8ToGBK(CString& strUtf8);
	void InitProvinceInfo();
	int Test(XinHuaOrderStr &orderStr);
	int GetSubNo(CString strParantNo,CString strKey,CString &strNo);
	CString URLEncode(CString sIn);
	void ConvertGBKToUtf8(CString& strGBK);
	int DoQuery(XinHuaOrderStr &orderStr);
	int Query(XinHuaOrderStr &orderStr);
	void AnanyzerGood(CString strNo,CString& strContent);
private:
	CWebBase m_webbase;
	CCodeCheck m_objCheck;
	string m_strPicPath;
	string m_strHeader;
	map<CString,CString> m_mapProvince;
	CString m_strListPrice;
	CString m_strSalePrice;
	bool m_bClearCart;
};
