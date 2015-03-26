#pragma once
#include "WebBase.h"
#include "ThreadManager.h"
#include "ASCIIConver.h"
#include "afxwin.h"
#include "RasDialMgr.h"
#include "datatype.h"
#include <mshtml.h>
//#include <afxhtml.h>
//#include <atlbase.h>
#include <oleacc.h>
//#include <MsHtmcid.h>
//#pragma comment(lib,"comsupp.lib")
//#pragma comment(lib,"comsuppw.lib")
//#pragma comment(lib,"kernel32.lib")
//51JOB的 WEB POST类
class C51JobWebPost:public CWebBase
{
public:
	C51JobWebPost();
	~C51JobWebPost();
	bool UserRegister();
	static void ThreadFunc(LPVOID lParam);
	CString GetKeyString(CString &srcStr,CString &strBefore,CString &strAfter);
	void RegisterFunc();
	void BeginRegister();
	void ShowMessage(CString strMsg);
	CString NativeToAscii(CString strSrc);
	static void ShowMessageEx(CString strMsg);
	int GetGateWayIP(CString &strIP);
	void SaveToExcel(RegisterPack rgPack);
	BOOL CreatExcelDb(CString dbpath,CString tableName,CString &strError);
	CString GetExcelDriver();
	int WriteToBase(RegisterPack rgPack,int iFlag);
	int GetHtmlCtrl(IHTMLDocument2 *pDoc2);
	IHTMLDocument2* GetDocInterface(HWND hWnd);
	CString GetElementValue(CComPtr<IHTMLElement> pElement);
	int ChangeMacAddr();
	int ReOpenRegWindow();
	CString MacAddrRand();
	int AddEmail(RegisterPack &rgPack);
	void SendAddMessage(int iFlag,CString strMsg);
	void RegisterFuncEx();
private:
	CThreadManager m_ThreadManager;
	bool m_LoginFlag;
	bool m_OpenFreshPageFlag;
	int m_ExceptionNum;
	int m_FreshTimes;		//已刷新次数
	CString m_strHeader;            //请求头
	CString m_ResumeIDStr;
	CString m_FreshPageURL;         //刷新页URL
	CString m_strUserName;
	CString m_strUserPwd;
	int m_TimeInterval;             //刷新间隔
	int m_iCount;
	int m_nErrorTime;		//错误次数
	int m_iCheckFlag;
	
	//中文简历信息
	CString m_ChHPNation;           //家庭国别号
	CString m_ChHPCity;             //家庭区号
	CString m_ChHPNumber;           //家用电话号码
	CString m_ChFPNation;           //公司国别号
	CString m_ChFPCity;             //公司区号
	CString m_ChFPNumber;           //公司总机
	CString m_ChFPExtension;        //分机号
	CString m_ChMPNation;           //移动电话国别号
	CString m_ChMobile;             //移动电话
	CString m_ChEmailAdd;           //Email
	CString m_ChResumeID;           //简历ID
	CString m_ChHelpID;             //辅助ID

	//英文简历信息
	CString m_EnHPNation;
	CString m_EnHPCity;
	CString m_EnHPNumber;
	CString m_EnFPNation;
	CString m_EnFPCity;
	CString m_EnFPNumber;
	CString m_EnFPExtension;
	CString m_EnMPNation;
	CString m_EnMobile;
	CString m_EnEmailAdd;
	CString m_EnResumeID;
	CASCIIConver m_Conver;	
	int m_iNeedChageIP;
	int m_iIpSuccNum;		//单IP注册成功数
	CString m_strOldIP;		//旧的网关IP
	int m_iCodeErrorNum;

	CString m_strVerify;
	CString m_strTrans;
	CString m_strMac;
	HWND m_hwnd;
	int m_iIsNeedChangeMac;
};