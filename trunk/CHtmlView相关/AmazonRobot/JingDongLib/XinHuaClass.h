#pragma once
#include "DataDefine.h"
#include "WebBase.h"
 
#include "URLEncode.h"
#include <map>
using namespace std;
class JinDongClass
{
public:
	JinDongClass(void);
	~JinDongClass(void);
	bool Login(CString strUser,CString strPwd);
	bool DoLogin(CString strUser,CString strPwd);
	int AddToCart(CString strBookNo,float fPrice);
	int AddToCart(CString strGoodNo,CString num);
	int Order(JingDongOrderStr &orderStr);
	int GetAppPath(CString &strAppPath);
	int DoOrder(JingDongOrderStr &orderStr);
	CString GetKeyString(CString &srcStr,CString strBefore,CString strAfter);
	void ConvertUtf8ToGBK(CString& strUtf8);
	void InitProvinceInfo();
	int Test(JingDongOrderStr &orderStr);
	int GetSubNo(CString strParantNo,CString strKey,CString orderNum,CString &strNo);
	int   GetSubCityNo(CString strParantNo,CString strKey,CString orderNum,CString &strNo);
	CString URLEncode(CString sIn);
	void ConvertGBKToUtf8(CString& strGBK);
	int DoQuery(JingDongOrderStr &orderStr);
	string Query(JingDongOrderStr &orderStr);
	void AnanyzerGood(CString strNo,CString& strContent);
	bool  writeOrderNum(CString file,CString id);
	string   getOrderHtml(JingDongOrderStr &orderStr);
private:
	CWebBase m_webbase;
 
	string m_strPicPath;
	string m_strHeader;
	map<CString,CString> m_mapProvince;
	CString m_strListPrice;
	CString m_strSalePrice;
	bool m_bClearCart;
	CString m_path;
};
