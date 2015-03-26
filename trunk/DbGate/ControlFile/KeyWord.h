#pragma once
#include <afxmt.h>
#include <afxtempl.h>
#include <map>
using namespace std;
class CKeyWord
{
public:
	CKeyWord(void);
	~CKeyWord(void);
	int AddKeyWord(CString strKeyWord);
	bool IsInKeyMap(CString strKeyWord);
	int AddtoKeyMap(CString strKeyWord);
	int KeyWordCheck(CString strSRC,CString &resultStr);
	int ExecuteSQL(CString strSQL);
	int LoadAllKeyWords(int &iCount);
	int GetMsgULen(CString Msg);
	int GetQueryKeys(CString strKey,CListCtrl &listctrl,int &iCount,BOOL *iExit);
	int OutPortKeys(CString strFilePath);
	void SetDBInfo(CString strPath);
private:
	map<CString, int>  m_keyWordMap;			
	CString m_strDbPath;
	CString m_strDbPwd;
	CString m_strFileName;
};
