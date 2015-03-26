#pragma once
#include <afxmt.h>
#define  MWGATEINITFILE "mwgate.ini"
#define  DBSETINITFILE "dbset.ini"
#define  TITLESETINITFILE "title.ini"
#define  KEYWORDFILE "illegal.txt"
class CUserFile
{
private:
	static CUserFile * m_Instance;          //单例指针
	static CCriticalSection m_slockInstance;       //全局锁,double-check Lock用
public:
	static CUserFile * GetInstance();
	CUserFile(void);
	~CUserFile(void);
	int WriteInitInfo(CString strfilePath,CString strnodeName,CString strKeyName,CString strKeyValue,int iKeyValue,int iFlag);
	int ReadInitInfo(CString strfilePath,CString strnodeName,CString strKeyName,CString &strKeyValue,int &iKeyValue,int iFlag);
	int GetAppPath(CString &strAppPath);
	void CheckInitValue(int &iValue,int iMin,int iMax);
	CString StrEncode(CString srcStr);
	CString StrDecode(CString srcStr);
	void SplitMsg(CString srcStr,CString *strArray,CString strSplit,int iLen);
	CString GetFileVersion(CString strFileName);
	CString GetCurrentAppPath();
	CString GetModifyTime(CString appname);
};
