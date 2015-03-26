#include "StdAfx.h"
#include "CBitmapConvert.h"

CCBitmapConvert::CCBitmapConvert(void)
{
	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);
}

CCBitmapConvert::~CCBitmapConvert(void)
{
	GdiplusShutdown(m_pGdiToken);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CCBitmapConvert::GetImageCLSID(const WCHAR* format, CLSID* pCLSID)
{
	UINT num = 0;
	UINT size = 0;
	ImageCodecInfo* pImageCodecInfo = NULL;
	GetImageEncodersSize(&num, &size);
	if(size == 0)
	{
		return FALSE;
	}
	pImageCodecInfo = (ImageCodecInfo *)malloc((size));
	if(pImageCodecInfo == NULL)
		return FALSE;
	GetImageEncoders(num, size, pImageCodecInfo);
	// Find for the support of format for image in the windows
	for(UINT i = 0; i < num; ++i)
	{
		if( wcscmp(pImageCodecInfo[i].MimeType, format) == 0)
		{
			*pCLSID = pImageCodecInfo[i].Clsid;
			free(pImageCodecInfo);
			return TRUE;
		}
	}
	free(pImageCodecInfo);
	return FALSE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/

BOOL CCBitmapConvert::TranstToBMP(string srcPath,string destPath)
{
	remove(destPath.c_str());
	WCHAR wCH[255] = {0};
	WCHAR wCH1[255] = {0};
	wstring wstrPath = s2ws(srcPath);
	wcscpy(wCH,wstrPath.c_str());
	Image img(wCH);//这里的图片可以是其它格式	
	CLSID pngClsid;
	GetImageCLSID(L"image/bmp", &pngClsid);//这里的图片可以是其它格式，此处转化为BMP格式
	wstrPath = s2ws(destPath);
	wcscpy(wCH1, wstrPath.c_str());
	img.Save(wCH1, &pngClsid, NULL);
	return TRUE;
}

std::wstring CCBitmapConvert::s2ws(const std::string& s) 
{ 
	int len; 
	int slength = (int)s.length() + 1; 
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);  
	wchar_t* buf = new wchar_t[len]; 
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len); 
	std::wstring r(buf); 
	delete[] buf; 
	return r; 
} 

string CCBitmapConvert::GenBmpPicPath(string strSrcPath)
{
	int iPos = strSrcPath.rfind('.');
	return strSrcPath.append(".bmp");
}

string CCBitmapConvert::GenBinaryPicPath(string strSrcPath)
{
	int iPos = strSrcPath.rfind('.');
	strSrcPath.insert(iPos,"binary");
	return strSrcPath;
}