#pragma once
#include "CBitmapConvert.h"
#include <afxmt.h>
#define OPENCV_USE
#ifdef OPENCV_USE
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#pragma comment(lib,"cxcore.lib")
#pragma comment(lib,"cv.lib")
#pragma comment(lib,"highgui.lib")
#pragma comment(lib,"cvaux.lib")
#pragma comment(lib,"cvcam.lib")
#endif

#define ASPRISEOCR_LIB
#ifdef ASPRISEOCR_LIB
#include "AspriseOCR.h"
#pragma comment(lib,"./AspriseOCR.lib")
#endif

//#define TESS_OCR_CHECK
#ifdef TESS_OCR_CHECK
#include "TessOcr/TessOcr.h"
#pragma comment(lib,"CtrlFile/TessOcr/CheckCodeOcr.lib")
#endif
class CCodeCheck : public CCBitmapConvert
{
public:
	CCodeCheck();
public:
	~CCodeCheck(void);
	string AspriseOCRCheck(string strPicPath);
	string CheckCode(string strPicPath);
	string GetRetCodeString(string strSrcCode);
	string TessOCRCheck(string strPicPath);
	CCriticalSection m_CheckLock;
};
