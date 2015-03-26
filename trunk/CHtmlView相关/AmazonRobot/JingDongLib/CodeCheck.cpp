#include "StdAfx.h"
#include "CodeCheck.h"

CCodeCheck::CCodeCheck()
{
}

CCodeCheck::~CCodeCheck(void)
{
}

string CCodeCheck::CheckCode(string strPicPath)
{
	string strBmpPath = GenBmpPicPath(strPicPath);
	string strBinaryPath = GenBinaryPicPath(strBmpPath);
	TranstToBMP(strPicPath,strBmpPath);
#ifdef OPENCV_USE
	IplImage *pImage;
	if((pImage = cvLoadImage(strBmpPath.c_str(), 0)) == 0)
	{
		return "";
	}	
	cvThreshold(pImage,pImage,140,255,CV_THRESH_BINARY);
	remove(strBinaryPath.c_str());
	cvSaveImage(strBinaryPath.c_str(),pImage);
	cvReleaseImage(&pImage);
#endif	
	#ifdef ASPRISEOCR_LIB
	return AspriseOCRCheck(strBinaryPath);
	#endif

#ifdef TESS_OCR_CHECK
	return TessOCRCheck(strBinaryPath);
#endif
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
string CCodeCheck::AspriseOCRCheck(string strPicPath)
{
#ifdef ASPRISEOCR_LIB
	string strRet = "";
	m_CheckLock.Lock();
	try
	{
		strRet = GetRetCodeString(string(OCR((char *)strPicPath.c_str(),-1)));
	}
	catch (...)
	{
	}	 
	m_CheckLock.Unlock();
	return strRet;

#endif
	return "";
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
string CCodeCheck::TessOCRCheck(string strPicPath)
{
#ifdef TESS_OCR_CHECK
	char szResult[256] = {0};
	m_CheckLock.Lock();
	try
	{
		DoOcr(strPicPath.c_str(),szResult);
	}
	catch (...)
	{
	}	 
	m_CheckLock.Unlock();	
	return GetRetCodeString(string(szResult));
#endif
	return "";
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
string CCodeCheck::GetRetCodeString(string strSrcCode)
{
	char num = 0;
	string strRet = "";
	char szCode[100] = {0};
	if (strSrcCode.length() > 99)
	{
		return "";
	}
	strcpy(szCode,strSrcCode.c_str());
	for (int i= 0;i<strSrcCode.length();i++)
	{
		num = szCode[i];
		if (num == 'o'||num == 'O')
		{
			num = '0';
		}
		if (num == 'z' || num == 'Z')
		{
			//num = '2';
		}
		if (num == 'g')
		{
			//num = '9';
		}
		if (num == '$')
		{
			num = '8';
		}
		if (num == 'l')
		{
			//num = '1';
		}
		if (num == 'b')
		{
		//	num  = '6';
		}
		if ((num >= 48 && num <=57) ||
			(num >= 65 && num <=90) ||
			(num >= 97 && num <=122))
		{
			strRet.append(1,num);
		}
	}
	return strRet;		
}