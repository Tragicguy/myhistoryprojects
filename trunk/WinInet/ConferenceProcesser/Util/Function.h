#ifndef  _CFUNCTION_DEFINE__
#define  _CFUNCTION_DEFINE__
#include <map>
#include <vector>
#include <string>
using std::map;
using std::vector;
using std::string;
#pragma warning(disable:4996)
class CFunction
{
public:
	CFunction(void);
	~CFunction(void);
private:
	static BYTE toHex(const BYTE &x){return x > 9 ? x + 55: x + 48;	}
public:
	static int GetAppPath(CString &strAppPath);
	static string GetAppPath();
	static CString GetKeyString(CString &srcStr,CString strBefore,CString strAfter);
	static void ConvertGBKToUtf8(CString& strGBK);
	static CString ConvertUtf8ToGBKEx(CString& strUtf8);
	static CString ConvertGBKToUtf8Ex(CString strGBK);
	static void ConvertUtf8ToGBK(CString& strUtf8);
	static void ConvertUtf8ToGBK(string& strUtf8);
	static CString URLEncode(CString sIn);
	static CString Base64Encode(LPCTSTR strEncodeData, int nDataSize);
	static void ClearSiteCookie(CString strFlag);
	static void InitInfo(CString strData, map<CString, CString>& mapList);
	static void InitInfo(CString strData, vector<CString>& vecList);	
};
#endif
