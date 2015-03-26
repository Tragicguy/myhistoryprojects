#pragma once
#include <gdiplus.h>
using   namespace   Gdiplus;
#pragma comment(lib,"gdiplus.lib")
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#pragma comment(lib,"cxcore.lib")
#pragma comment(lib,"cv.lib")
#pragma comment(lib,"highgui.lib")
//#pragma comment(lib,"cvaux.lib")
//#pragma comment(lib,"cvcam.lib")
class CMyImage
{

public:
	CMyImage(void); //使用本类请载入LIB文件 gdiplus.lib
	~CMyImage(void);
	
	BOOL GetImageCLSID(const WCHAR* format, CLSID* pCLSID);
	BOOL TranstToBMP(CString srcPath,CString destPath);
	BOOL ShowBmpFile(HDC hdc, int nXDest, int nYDest,LPBYTE lpDIB);
	int OSTUThreshold(LPBYTE   lpDIBBits,   LONG   lWidth,   LONG   lHeight);
	bool ReadBMPFile(LPCTSTR strFileName,LPBYTE& m_lpBMPFileData,long& fileLength);
	WCHAR* ToWChar(char * str);
	bool ConvertToGreyPicture( LPBYTE  pSource );

	bool SaveNewPicture(CString filePath,LPBYTE m_newBmpData,long filelength);
	void ClearNoise(LPBYTE m_BmpData);
	void ShowToDC(IplImage* imgShow,HWND m_hwnd,int startX,int startY);
	void GetSelectWindowPic(HWND hwnd,CString strSavePath);
private:
	GdiplusStartupInput   gdiplusStartupInput;    
	ULONG_PTR   gdiplusToken;
};
