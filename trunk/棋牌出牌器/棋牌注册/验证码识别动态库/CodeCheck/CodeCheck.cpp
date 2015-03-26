// CodeCheck.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include <afxdllx.h>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include "include/AspriseOCR.h"
#pragma comment(lib,"cxcore.lib")
#pragma comment(lib,"cv.lib")
#pragma comment(lib,"highgui.lib")
#pragma comment(lib,"lib\\AspriseOCR.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE CodeCheckDLL = { NULL, NULL };
int __stdcall MatchCheckCode(char *szPath, char *szCode,int iThreadShod);
extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// 如果使用 lpReserved，请将此移除
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("CodeCheck.DLL 正在初始化！\n");
		
		// 扩展 DLL 一次性初始化
		if (!AfxInitExtensionModule(CodeCheckDLL, hInstance))
			return 0;

		// 将此 DLL 插入到资源链中
		// 注意: 如果此扩展 DLL 由
		//  MFC 规则 DLL(如 ActiveX 控件)隐式链接到
		//  而不是由 MFC 应用程序链接到，则需要
		//  将此行从 DllMain 中移除并将其放置在一个
		//  从此扩展 DLL 导出的单独的函数中。使用此扩展 DLL 的
		//  规则 DLL 然后应显式
		//  调用该函数以初始化此扩展 DLL。否则，
		//  CDynLinkLibrary 对象不会附加到
		//  规则 DLL 的资源链，并将导致严重的
		//  问题。

		new CDynLinkLibrary(CodeCheckDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("CodeCheck.DLL 正在终止！\n");

		// 在调用析构函数之前终止该库
		AfxTermExtensionModule(CodeCheckDLL);
	}
	return 1;   // 确定
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString GetRetString(char *szSrc)
{
	char num = 0;
	CString strSrc = "";
	CString strRet = "";
	CString strTemp = "";
	strSrc.Format("%s",szSrc);
	for (int i= 0;i<strSrc.GetLength();i++)
	{
		num = strSrc.GetAt(i);
		if ((num >= 48 && num <=57) ||
			(num >= 65 && num <=90) ||
			(num >= 97 && num <=122))
		{
			strTemp.Format("%c",num);
			if (strTemp == "B")
			{
				strTemp = "8";
			}
			else if (strTemp == "b")
			{
				strTemp = "6";
			}
			else if (strTemp == "O" || strTemp == "o")
			{
				strTemp = "0";
			}
			else if (strTemp == "S" || strTemp == "s")
			{
				strTemp = "5";
			}
			else if (strTemp == "g")
			{
				strTemp = "9";
			}
			else if (strTemp == "l")
			{
				strTemp = "1";
			}
			strRet += strTemp;
		}
	}
	return strRet;	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int __stdcall MatchCheckCode(char *szPath, char *szCode,int iThreadShod)
{
	try
	{
		IplImage *m_Source_Pic = NULL;
		m_Source_Pic = cvLoadImage(szPath);
		if (!m_Source_Pic)
		{
			strcpy(szCode,"can not open the image");
			return -2;	//找不到
		}
		IplImage * RedChannel   = cvCreateImage( cvGetSize(m_Source_Pic), 8, 1);
		IplImage * GreenChannel  = cvCreateImage( cvGetSize(m_Source_Pic), 8, 1);
		//IplImage * BlueChannel = cvCreateImage( cvGetSize(m_Source_Pic), 8, 1);
		//cvSetImageCOI(m_Source_Pic,1); 
		//cvCopy(m_Source_Pic,BlueChannel); //提取蓝色
		cvSetImageCOI(m_Source_Pic,2);
		cvCopy(m_Source_Pic,GreenChannel);  //提取绿色
		cvSetImageCOI(m_Source_Pic,3);
		cvCopy(m_Source_Pic,RedChannel); //提取红色
		cvAdd(RedChannel,GreenChannel,RedChannel);
		cvThreshold(RedChannel,RedChannel,iThreadShod,255,CV_THRESH_BINARY);
		strcat(szPath,"2.bmp");
		cvSaveImage(szPath,RedChannel);
		char *szCodeDll = OCR(szPath,-1);
		CString strRet = GetRetString(szCodeDll);
		strcpy(szCode,strRet);
		cvReleaseImage(&m_Source_Pic);
		cvReleaseImage(&RedChannel);
		cvReleaseImage(&GreenChannel);
		//cvReleaseImage(&BlueChannel);
		return 0;
	}
	catch (...)
	{
		strcpy(szCode,"excption");
	}
	return -1;
}

