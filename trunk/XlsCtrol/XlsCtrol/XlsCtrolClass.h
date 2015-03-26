#pragma once
#include "ADOFile/AdoRecordSet.h"
#include <odbcinst.h>
#include <afxdb.h>
#include <io.h>
#include <vector>
using namespace std;
typedef struct _BookStr
{
	CString strISBN;
	CString strName;
	CString strPress;
	CString strPrice;
	CString strNum;
	CString strSellPrice;
	_BookStr()
	{
		strISBN.Empty();
		strName.Empty();
		strPress.Empty();
		strPrice.Empty();
		strNum.Empty();
		strSellPrice.Empty();
	}
}BookStr;
typedef struct _ExpressStr
{
	CString strOrderNo;
	CString strAddr;
	CString strRecver;
	CString strPhone;
	CString strMemo;
	CString strPayTime;
	CString strExpressCompany;
	CString strExpressNo;
	vector<_BookStr> vecBookInfo;
	_ExpressStr()
	{
		strOrderNo.Empty();
		strAddr.Empty();
		strRecver.Empty();
		strPhone.Empty();
		strMemo.Empty();
		strPayTime.Empty();
		strExpressCompany.Empty();
		strExpressNo.Empty();
		vecBookInfo.clear();
	}	
}ExpressStr;
class CXlsCtrolClass
{
public:
	CXlsCtrolClass(void);
	~CXlsCtrolClass(void);
	CString GetExcelDriver();
	bool Export(CString strFile,vector<ExpressStr> &vecStr,bool bAppend);
	bool Import(CString strFile,vector<ExpressStr> &vecStr);
	bool Modify(CString strFile,ExpressStr &expressStr);
	bool ImportEx(CString strFile,vector<ExpressStr> &vecStr);
	bool GetTables(CString strFile,CStringArray &strTableArray);
	bool Add(CString strFile,vector<ExpressStr> &vecStr);
};
