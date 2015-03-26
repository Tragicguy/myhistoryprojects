#pragma once
#include <string>
using namespace std;
#include <Windows.h>
#include <GdiPlus.h>
using namespace Gdiplus;
#pragma comment(lib,"gdiplus.lib")
class CCBitmapConvert
{
public:
	CCBitmapConvert(void);
public:
	~CCBitmapConvert(void);	
	BOOL TranstToBMP(string srcPath,string destPath);
	std::wstring s2ws(const std::string& s);
	string GenBmpPicPath(string strSrcPath);
	string GenBinaryPicPath(string strSrcPath);
private:
	GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_pGdiToken;
	BOOL GetImageCLSID(const WCHAR* format, CLSID* pCLSID);
};
