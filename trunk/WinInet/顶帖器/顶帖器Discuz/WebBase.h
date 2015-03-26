#pragma once 
#include "afxinet.h"
#include "afxmt.h"
#include <math.h>
#pragma comment(lib,"wininet.lib")

//HTTP 基础类 GetPage

class CWebBase 
{

public:
        CWebBase();
        ~CWebBase();
public:

        //用户信息
        CString                 m_strServerURL;
        CString                 m_strCompany;
        int                     m_nElseWhereIP;

        //连接信息
        CInternetSession        m_Session;
        CHttpConnection  *      m_pConnection;

        CString                 m_strLanguage;
        int                     m_nNotDirect; //是否重定向

        CMutex                  m_lock;   //用于以后同步
        CEvent                  m_even;   //以后发通知用

        //服务器返回的不是内容,就是头
        //要的东西都可以在这里找到
        CString                 m_strWorkBuf;
        CString                 m_strResponHead;
        CString                 m_strRedirectUrl;
        CString                 m_strSetCookie;

		int						m_iHttpsFlag;	//是否为HTTPS协议

public:

        //因为登录,刷新,下注都要用到,所以放这里了
		int RequestGetPageExEx( CInternetSession & internetSession,
			CHttpConnection * & httpConncection,
			CString & strServer, CString & strObject, 
			CString & strHead,CString &strError,
			int iGetType=0,	//表示获取文件，1表示获取验证码
			int iIsProxy=0,	//代理类型
			int iProxyType=0,	//是否使用代理
			CString strProxyIP="",//代理IP
			int iProxyPort=80,//代理端口
			CString strProxyUser="",//代理用户名
			CString strProxyPwd=""//代理密码
			);
		/************************************************************************/
		/*获取页面或验证码图片                                                  */
		/************************************************************************/
		int RequestPostPageExEx( CInternetSession & internetSession,
			CHttpConnection * & httpConncection,
			CString & strServer, CString & strObject, 
			CString & strHead,CString & strPostData,
			CString &strError,
			int iGetType=0,	//表示获取文件，1表示获取验证码
			int iIsProxy=0,	//代理类型
			int iProxyType=0,	//是否使用代理
			CString strProxyIP="",//代理IP
			int iProxyPort=80,//代理端口
			CString strProxyUser="",//代理用户名
			CString strProxyPwd=""//代理密码
			);

		//因为登录,刷新,下注都要用到,所以放这里了
		int RequestGetPageExExEx(
			CString & strServer, CString & strObject, 
			CString & strHead,CString &strError,
			int iGetType=0,	//表示获取文件，1表示获取验证码
			int iIsProxy=0,	//代理类型
			int iProxyType=0,	//是否使用代理
			CString strProxyIP="",//代理IP
			int iProxyPort=80,//代理端口
			CString strProxyUser="",//代理用户名
			CString strProxyPwd=""//代理密码
			);
		/************************************************************************/
		/*获取页面或验证码图片                                                  */
		/************************************************************************/
		int RequestPostPageExExEx(
			CString & strServer, CString & strObject, 
			CString & strHead,CString & strPostData,
			CString &strError,
			int iGetType=0,	//表示获取文件，1表示获取验证码
			int iIsProxy=0,	//代理类型
			int iProxyType=0,	//是否使用代理
			CString strProxyIP="",//代理IP
			int iProxyPort=80,//代理端口
			CString strProxyUser="",//代理用户名
			CString strProxyPwd=""//代理密码
			);
        int SetTimeOut( CInternetSession & internetSession,
                int  nTimeOutSecond );
        int  UploadFile(LPCTSTR strURL, LPCTSTR strLocalFileName);
        //为了解析Big5,unicode的编码问题,加入这两个函数
        int TraditionTOSimple( CString & strHtml );
        int HtmlCodeToSimple( CString & strHtmlText );
        void ConvertGBKToUtf8(CString& strGBK) ;
        void ConvertUtf8ToGBK(CString& strUtf8) ;
        void URL_Encode(  CString  &  strSource );
        void URL_Decode(  CString  &  strSource );
		void SetHttpsType(int iType);
		void DeleteObjectEx(CHttpFile *pHttpFile);
		void DeleteObjectExEx();
		DWORD GetDwflag();
		CString URLEncode(CString sIn);
		CString URLDecode(CString sIn);
		int m_iStatusCode;
	private:
        int HtmlCodeToUniCodeByte( char *  pszHtmlcode, int iLen );
        int HtmlCodeToUniCodeByteReg( char *  pszHtmlcode, int iLen );		
		int SendRequest(CHttpFile *pHttpFile,CString &strError,CString strHeader,CString strPostData,int iPostFlag=0);
		int GetHttpConnection(CInternetSession & internetSession, CHttpConnection *&httpConncection, CString & strServer, CString & strObject, CString & strHead,CString &strError, int iIsProxy, /*是否使用代理 */ int iProxyType,/*代理类型 */ CString strProxyIP,/*代理IP */ int iProxyPort/*代理端口 */ );
		int ConvertData(CString strReponseHead,CString &strError,CString &strData);
		int GetHttpConnectionEx(CString & strServer, CString & strObject, CString & strHead,CString &strError, int iIsProxy, /*是否使用代理 */ int iProxyType,/*代理类型 */ CString strProxyIP,/*代理IP */ int iProxyPort/*代理端口 */ );
	
};

