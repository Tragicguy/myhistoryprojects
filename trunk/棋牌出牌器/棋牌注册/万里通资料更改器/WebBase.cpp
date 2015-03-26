#include "stdafx.h"
#include "WebBase.h"
#include <atlrx.h>
#include "zlib\gzdecompress.h"
#include "datatype.h"
using namespace std;

//HTTP 基础类 GetPage
//负责HTTP协议操作，编码转换，解压
/************************************************************************/
/*                                                                      */
/************************************************************************/
CWebBase::CWebBase( )
{
	m_pConnection=NULL;
	m_nNotDirect = 0;
	m_nElseWhereIP = 0;
	m_iHttpsFlag = 0;
	m_iHttpsFlag = 1;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CWebBase::~CWebBase()
{
	DeleteObjectExEx();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//负责接收上传操作的页面的URL ,待上传的本地文件路径
int  CWebBase::UploadFile(LPCTSTR strURL, LPCTSTR strLocalFileName)
{
	ASSERT(strURL != NULL && strLocalFileName != NULL);

	BOOL bResult = FALSE;
	DWORD dwType = 0;
	CString strServer;
	CString strObject;
	INTERNET_PORT wPort = 0;
	DWORD dwFileLength = 0;
	char * pFileBuff = NULL;

	CHttpConnection * pHC = NULL;
	CHttpFile * pHF = NULL;
	CInternetSession cis;

	bResult =  AfxParseURL(strURL, dwType, strServer, strObject, wPort);
	if(!bResult)
	{
		return FALSE;
	}
	CFile file;
	try
	{
		if(!file.Open(strLocalFileName, CFile::shareDenyNone | CFile::modeRead))
			return FALSE;
		dwFileLength = file.GetLength();
		if(dwFileLength <= 0)
			return FALSE;
		pFileBuff = new char[dwFileLength];
		memset(pFileBuff, 0, sizeof(char) * dwFileLength);
		file.Read(pFileBuff, dwFileLength);

		const int nTimeOut = 5000;
		cis.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut); //联接超时设置
		cis.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);  //重试1次
		pHC = cis.GetHttpConnection(strServer, wPort);  //取得一个Http联接

		pHF = pHC->OpenRequest(CHttpConnection::HTTP_VERB_POST, strObject);
		if(!pHF->SendRequest(NULL, 0, pFileBuff, dwFileLength))
		{
			delete[]pFileBuff;
			pFileBuff = NULL;
			pHF->Close();
			pHC->Close();
			cis.Close();
			return FALSE;
		}
		DWORD dwStateCode = 0;
		pHF->QueryInfoStatusCode(dwStateCode);

		if(dwStateCode == HTTP_STATUS_OK)
			bResult = TRUE;
	}

	catch(CInternetException * pEx)
	{
		char sz[256] = "";
		pEx->GetErrorMessage(sz, 25);
		CString str;
		str.Format("InternetException occur!\r\n%s", sz);
		AfxMessageBox(str);
	}
	catch(...)
	{
		DWORD dwError = GetLastError();
		CString str;
		str.Format("Unknow Exception occur!\r\n%d", dwError);
		AfxMessageBox(str);
	}

	delete[]pFileBuff;
	pFileBuff = NULL;
	file.Close();
	pHF->Close();
	pHC->Close();
	cis.Close();
	return bResult;
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
int CWebBase::TraditionTOSimple( CString & strHtml )
{
	char *          pszBig5=NULL;          //Big5编码的字符        
	wchar_t *       wszUnicode=NULL;       //Unicode编码的字符 
	char *          pszGbt=NULL;           //Gb编码的繁体字符 
	char *          pszGbs=NULL;           //Gb编码的简体字符 
	CString         sGb;                   //返回的字符串 
	int             iLen=0;                //需要转换的字符数 

	//取得长度
	iLen = strHtml.GetLength();
	if ( iLen <= 0 ) 
	{
		return iLen;
	}
	pszBig5 = strHtml.GetBuffer( iLen);

	//计算转换的字符数 
	iLen=MultiByteToWideChar (950, 0, pszBig5, -1, NULL,0) ;
	if( iLen <= 0 ) 
	{
		return iLen;
	}
	//给wszUnicode分配内存 
	wszUnicode = new wchar_t[iLen+1]; 
	if( wszUnicode == NULL )
	{
		return -1;
	}
	//转换Big5码到Unicode码，使用了API函数MultiByteToWideChar 
	MultiByteToWideChar (950, 0, pszBig5, -1, wszUnicode,iLen);

	//计算转换的字符数 
	iLen=WideCharToMultiByte( 936, 0, (PWSTR) wszUnicode, -1, 
		NULL,0, NULL, NULL) ; 
	//给pszGbt分配内存 
	pszGbt=new char[iLen+1]; 
	if( pszGbt == NULL )
	{
		delete [] wszUnicode; 
		return -1;
	}
	//给pszGbs分配内存 
	pszGbs = new char[iLen+1]; 
	if( pszGbs == NULL )
	{
		delete [] wszUnicode; 
		delete [] pszGbt;
		return -1;
	}
	//转换Unicode码到Gb码繁体，使用API函数WideCharToMultiByte 
	WideCharToMultiByte( 936, 0, (PWSTR) wszUnicode, -1, 
		pszGbt,iLen, NULL, NULL) ; 

	//转换Gb码繁体到Gb码简体，使用API函数LCMapString 
	LCMapString( 0x0804, LCMAP_SIMPLIFIED_CHINESE, 
		pszGbt, -1, pszGbs, iLen); 
	//释放内存        
	delete [] wszUnicode; 
	delete [] pszGbt;         
	if( HtmlCodeToUniCodeByte( (char *) pszGbs,  iLen) != iLen )
	{
		delete [] pszGbs; 
		return 0;
	}
	strHtml = CString( pszGbs, iLen);
	if( strHtml.GetLength() != iLen )
	{
		delete [] pszGbs; 
		return 0;
	}
	//释放内存 
	delete [] pszGbs; 
	strHtml.Replace("&#", "");
	strHtml.Replace("^^^;", "");
	//strHtml.Replace("^", "");

	//返回Gb码简体字符 
	return strHtml.GetLength();
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CWebBase::HtmlCodeToUniCodeByte( char *  pszHtmlcode, int iLen )
{

	int i;
	char * ptemp;
	unsigned short usData;
	char cbuffGbt[8] = {0};
	char cBuffGbs[8] = {0}; 

	if( pszHtmlcode == NULL || iLen <= 0 ) 
	{
		return -1;
	}
	ptemp = pszHtmlcode;
	pszHtmlcode = strstr( pszHtmlcode, "&#") ;
	if( pszHtmlcode == NULL ) 
	{
		return iLen;
	}

	for ( i = 0; i < 4000; i++ )//&#
	{
		pszHtmlcode = strstr( pszHtmlcode, "&#") ;
		if( pszHtmlcode == NULL ) 
		{
			break;
		}
		pszHtmlcode += 2;
		usData = (unsigned short)atoi(pszHtmlcode);
		//转换Unicode码到Gb码繁体，使用API函数WideCharToMultiByte 
		WideCharToMultiByte ( 936, 0, (PWSTR)&usData, -1, 
			cbuffGbt,2, NULL, NULL) ; 
		//转换Gb码繁体到Gb码简体，使用API函数LCMapString 
		LCMapString( 0x0804,LCMAP_SIMPLIFIED_CHINESE, cbuffGbt, -1, 
			cBuffGbs , 2); 
		memcpy( pszHtmlcode , cBuffGbs, 2);
		memset( pszHtmlcode + 2, '^' , 3 );
		pszHtmlcode += 6;
	}
	return iLen;

}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//用正则表达式来转换
int CWebBase::HtmlCodeToUniCodeByteReg( char *  pszHtmlcode, int iLen )
{

	int     i;
	char  * pcTemp = NULL;
	char *  pcWord = NULL;

	REParseError status;
	CAtlRegExp<>            reurl;
	CAtlREMatchContext<>    mcurl;
	const CAtlREMatchContext<>::RECHAR * szStart = 0;
	const CAtlREMatchContext<>::RECHAR * szEnd = 0;

	unsigned short usData;
	char cbuffGbt[8] = {0};
	char cBuffGbs[8] = {0}; 


	pcTemp = strstr( pszHtmlcode, "&#") ;
	if( pcTemp == NULL ) 
	{
		return iLen;
	}
	status = reurl.Parse( "&#{.*?};");
	if( REPARSE_ERROR_OK != status)
	{
		return iLen;
	}
	for ( i = 0; i < 4000; i++ )
	{
		if (!reurl.Match( pcTemp , &mcurl ) )
		{
			break;
		}
		if( mcurl.m_uNumGroups <= 0 )
		{
			break;
		}
		szStart = NULL;
		szEnd = NULL;
		mcurl.GetMatch( 0, &szStart, &szEnd );
		pcTemp = (char*)szStart;
		if( pcTemp == NULL || szStart == NULL )
		{
			break;
		}
		pcWord = pcTemp;
		//nLength = szEnd - szStart;        
		usData = (unsigned short)atoi(pcWord);
		//转换Unicode码到Gb码繁体，使用API函数WideCharToMultiByte 
		WideCharToMultiByte ( 936, 0, (PWSTR)&usData, -1, 
			cbuffGbt,2, NULL, NULL) ; 
		//转换Gb码繁体到Gb码简体，使用API函数LCMapString 
		LCMapString( 0x0804,LCMAP_SIMPLIFIED_CHINESE, cbuffGbt, -1, 
			cBuffGbs , 2); 
		memcpy( pcWord, cBuffGbs, 2);
		memset( pcWord + 2, '^' , 3 );                        

	}
	return iLen;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//把Html编码转换为简体
int CWebBase::HtmlCodeToSimple( CString & strHtmlText )
{

	int             i = 0;
	char *          ptemp = NULL;
	char *          pEnd = NULL;
	unsigned short  usData = 0;
	char cbuffGbt[8] = {0};
	char cBuffGbs[8] = {0}; 

	if( strHtmlText.IsEmpty() ) 
	{
		return 0;
	}
	ptemp = strHtmlText.GetBuffer( 20 );
	ptemp = strstr( ptemp, "&#") ;
	if( ptemp == NULL ) 
	{
		return strHtmlText.GetLength();
	}
	for ( i = 0; i < 4000; i++ )//&#
	{
		ptemp = strstr( ptemp, "&#") ;
		if( ptemp == NULL ) 
		{
			break;
		}
		ptemp += 2;
		usData = (unsigned short)atoi(ptemp);
		if( usData > 255 )
		{                
			//转换Unicode码到Gb码繁体，使用API函数WideCharToMultiByte 
			WideCharToMultiByte ( 936, 0, (PWSTR)&usData, -1, 
				cbuffGbt,2, NULL, NULL) ; 
			//转换Gb码繁体到Gb码简体，使用API函数LCMapString 
			LCMapString( 0x0804,LCMAP_SIMPLIFIED_CHINESE, cbuffGbt, -1, 
				cBuffGbs , 2); 
			memcpy( ptemp, cBuffGbs, 2);
			memset( ptemp + 2, '^' , 3 );
			ptemp += 6;
		}
		else
		{
			//* ptemp = (char)usData ;
			pEnd = strstr( ptemp, ";") ;
			if( pEnd != NULL )
			{
				memset( ptemp , '^' , pEnd - ptemp );
				ptemp = pEnd;
			}
		}
	}
	strHtmlText.Replace("&#", "");
	strHtmlText.Replace("^^^;", "");
	//strHtmlText.Replace("^", "");
	return strHtmlText.GetLength();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CWebBase::ConvertGBKToUtf8(CString& strGBK) 
{
	int len=MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, NULL,0);
	unsigned short * wszUtf8 = new unsigned short[len+1];
	memset(wszUtf8, 0, len * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, (LPWSTR)wszUtf8, len);

	len = WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)wszUtf8, -1, NULL, 0, NULL, NULL); 
	char *szUtf8=new char[len + 1];
	memset(szUtf8, 0, len + 1);
	WideCharToMultiByte (CP_UTF8, 0, (LPWSTR)wszUtf8, -1, szUtf8, len, NULL,NULL);

	strGBK = szUtf8;
	delete[] szUtf8;
	delete[] wszUtf8;
	return ;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CWebBase::ConvertUtf8ToGBK(CString& strUtf8) 
{
	int len = 0;
	unsigned short * wszGBK = NULL;
	char * szGBKt = NULL;
	char * szGBKs = NULL;

	len = MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, NULL,0);
	wszGBK = new unsigned short[len+1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, (LPWSTR)wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL); 
	szGBKt  =new char[len + 1];
	memset(szGBKt, 0, len + 1);
	WideCharToMultiByte (CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBKt, len, NULL,NULL);

	szGBKs  =new char[len + 1];
	memset(szGBKs, 0, len + 1);
	//转换Gb码繁体到Gb码简体，使用API函数LCMapString 
	LCMapString( 0x0804, LCMAP_SIMPLIFIED_CHINESE, szGBKt, -1, szGBKs, len);
	strUtf8 = szGBKs;
	delete[] szGBKs;
	delete[] szGBKt;
	delete[] wszGBK;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CWebBase::SetTimeOut( CInternetSession & internetSession, int  nTimeOutSecond  )
{
	if( nTimeOutSecond < 1 )
	{
		return -1;
	}
	m_Session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 1000 * nTimeOutSecond );   //3秒的连接超时
	m_Session.SetOption(INTERNET_OPTION_SEND_TIMEOUT, 1000);                        // 1秒的发送超时
	m_Session.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 1500 * nTimeOutSecond );   // 7秒的接收超时
	m_Session.SetOption(INTERNET_OPTION_DATA_SEND_TIMEOUT, 1200);                   // 1秒的数据发送超时
	m_Session.SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, 1500 * nTimeOutSecond );  // 7秒的数据接收超时
	m_Session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);                            // 1次重试
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CWebBase::URL_Encode(  CString  &  strSource )
{
	int  i;
	int  index = 0;
	int  len = 0 ;
	char tmp_chr;
	char * str_output = NULL;

	len = strSource.GetLength() ;
	if( len <= 0 )
	{
		return ;
	}
	str_output  = new char[ len * 4 ];
	for(i = 0; i < len; i++)
	{
		if(     ('A' <= strSource[i] && strSource[i] <= 'Z') 
			||  ('a' <= strSource[i] && strSource[i] <= 'z')
			||  ('0' <= strSource[i] && strSource[i] <= '9') 
			|| strSource[i] == '-' ||  strSource[i] == '_' 
			|| strSource[i] ==  '.'  )
		{
			str_output[index++] = strSource[i];
		}
		else
		{
			str_output[index++] = '%';
			tmp_chr = strSource[i] / 16;
			tmp_chr += tmp_chr > 9 ? 'A' - 10 : '0';
			str_output[index++] = tmp_chr;

			tmp_chr = strSource[i] % 16;
			tmp_chr += tmp_chr > 9 ? 'A' - 10 : '0';
			str_output[index++] = tmp_chr;
		}
	}
	str_output[index++] = '\0';
	strSource = str_output;
	delete [] str_output;
	return  ;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CWebBase::URL_Decode( CString  &  strSource )
{
	int i = 0, index = 0;
	int  len = 0;
	int hb, lb;

	char * str_output = NULL;
	len = strSource.GetLength();
	if( len <= 1 )
	{
		return ;
	}
	str_output = new char[len];

	while(i < len)
	{
		if((strSource[i] == '%') && (i + 2 <= len) )
		{
			if('A' <= strSource[i + 1] && strSource[i + 1] <= 'F')
			{
				hb = strSource[i + 1] - 'A' + 10;
			}
			else if('a' <= strSource[i + 1] && strSource[i + 1] <= 'f')
			{
				hb = strSource[i + 1] - 'a' + 10;
			}
			else if('0' <= strSource[i + 1] && strSource[i + 1] <= '9')
			{
				hb = strSource[i + 1] - '0';
			}
			else
			{
				hb = 0;
			}

			if('A' <= strSource[i + 2] && strSource[i + 2] <= 'F')
			{
				lb = strSource[i + 2] - 'A' + 10;
			}
			else if('a' <= strSource[i + 2] && strSource[i + 2] <= 'f')
			{
				lb = strSource[i + 2] - 'a' + 10;
			}
			else if('0' <= strSource[i + 2] && strSource[i + 2] <= '9')
			{
				lb = strSource[i + 2] - '0';
			}
			else
			{
				lb = 0;
			}

			str_output[index] = hb * 16 + lb;
			i += 3;
		}
		else
		{
			str_output[index] = strSource[i];
			i++;
		}

		index++;
	}

	str_output[index] = 0;
	strSource =  str_output;       
	delete [] str_output;

	return;
}

/************************************************************************/
/*获取页面或验证码图片                                                  */
/************************************************************************/
int CWebBase::RequestGetPageExExEx(CString & strServer, CString & strObject, 
								 CString & strHead,CString &strError,
								 int iGetType,	//表示获取文件，1表示获取验证码
								 int iIsProxy,	//是否使用代理
								 int iProxyType,//代理类型
								 CString strProxyIP,//代理IP
								 int iProxyPort,//代理端口
								 CString strProxyUser,//代理用户名
								 CString strProxyPwd//代理密码
								 )
{
	CHttpFile * pHttpFile;
	char cBuffer[1024];
	unsigned long StatusCode;
	unsigned long  iReadLen = 0;
	int            nStringLen = 0;
	CString        strTemp = "";	
	WORD            wdPort;
	BOOL result;
	int iRet = -1;
	try
	{
		//获取到参数信息
		DWORD dwFlags = GetDwflag();
		pHttpFile = NULL;
		m_strWorkBuf.Empty();
		//设置超时
		SetTimeOut(m_Session,g_iTimeOut);
		//获取连接
		iRet = GetHttpConnectionEx(strServer,strObject,strHead,strError,iIsProxy,iProxyType,strProxyIP,iProxyPort);
		if (iRet < 0)
		{
			return iRet;
		}
		//打开请求对象
		pHttpFile = m_pConnection->OpenRequest( CHttpConnection::HTTP_VERB_GET,
			(LPCTSTR)strObject,NULL, 0,
			NULL, NULL,
			dwFlags );
		if( pHttpFile == NULL )
		{
			strError = "OpenRequest失败";
			return -1;
		}
		//如果是SOCKET5代理，设置账号密码
		// 		if   (!pHttpFile->SetOption(INTERNET_OPTION_PROXY_USERNAME,strProxyUser,strProxyUser.GetLength()))   
		// 		{   
		// 			return -99;
		// 		}
		// 		if   (!pHttpFile->SetOption(INTERNET_OPTION_PROXY_PASSWORD,strProxyPwd,strProxyPwd.GetLength()))   
		// 		{   
		// 			return -99;   
		// 		}
		//发送请求到服务器		
		iRet = SendRequest(pHttpFile,strError,strHead,"");/*发送数据*/
		if (iRet < 0)//发送HTTP头信息失败
		{
			DeleteObjectExEx();
			return iRet; 
		}
		//获取状态码及头信息
		pHttpFile->QueryInfoStatusCode(StatusCode);
		pHttpFile->QueryInfo(HTTP_QUERY_LOCATION,m_strRedirectUrl); 
		pHttpFile->QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF,m_strResponHead);
		iReadLen = (unsigned long )pHttpFile->GetLength();
		// 是否重定向
		if ( StatusCode == HTTP_STATUS_OK ||
			StatusCode == HTTP_STATUS_MOVED ||
			StatusCode == HTTP_STATUS_REDIRECT ||
			StatusCode == HTTP_STATUS_REDIRECT_METHOD)
		{
			//获取数据
			if (iGetType == 0)			//读取文本信息
			{
				while( iReadLen >= 0 )
				{
					iReadLen = pHttpFile->Read( cBuffer ,1023 );
					if ( iReadLen <= 0)
					{
						break;
					}
					m_strWorkBuf += CString( cBuffer ,iReadLen);
				}
			}
			else		//读取验证码
			{
				CStdioFile out;
				out.Open(g_strDirect+g_strFileName, CFile::modeCreate | CFile::modeWrite |CFile::typeBinary);
				while(true)
				{
					iReadLen = pHttpFile->Read( cBuffer ,1 );
					if ( iReadLen <= 0)
					{
						break;
					}
					out.Write(cBuffer,1); 
				}
				out.Close();
			}			
		}
		//删除HTTPFILE对象
		DeleteObjectEx(pHttpFile);	
		if(!m_strWorkBuf.IsEmpty()) //解压数据
		{
			return ConvertData(m_strResponHead,strError,m_strWorkBuf);	
		}
		return 0;
	}
	catch(CInternetException * e)
	{
		DeleteObjectEx(pHttpFile);	
		DeleteObjectExEx();
		e->Delete();
		return -6;
	}
	catch(...)
	{
		strError = "抓取网页信息异常";
		DeleteObjectEx(pHttpFile);	
		DeleteObjectExEx();
		return -7;
	}
	return 0;
}
/************************************************************************/
/*获取页面或验证码图片                                                  */
/************************************************************************/
int CWebBase::RequestPostPageExExEx(CString & strServer, CString & strObject, 
								  CString & strHead,CString & strPostData,
								  CString &strError,
								  int iGetType,	//表示获取文件，1表示获取验证码
								  int iIsProxy,	//是否使用代理
								  int iProxyType,//代理类型
								  CString strProxyIP,//代理IP
								  int iProxyPort,//代理端口
								  CString strProxyUser,//代理用户名
								  CString strProxyPwd//代理密码
								  )
{
	CHttpFile * pHttpFile;
	char cBuffer[1024] = {0};
	unsigned long StatusCode;
	unsigned long  iReadLen = 0;
	int            nStringLen = 0;
	CString        strTemp = "";	
	WORD            wdPort;
	BOOL result;
	int iRet = -1;
	try
	{
		//获取到参数信息
		DWORD dwFlags = GetDwflag();
		pHttpFile = NULL;
		m_strWorkBuf.Empty();
		//设置超时
		SetTimeOut(m_Session, g_iTimeOut);
		//获取连接
		iRet = GetHttpConnectionEx(strServer,strObject,strHead,strError,iIsProxy,iProxyType,strProxyIP,iProxyPort);
		if (iRet < 0)
		{
			return iRet;
		}
		//打开请求对象
		pHttpFile = m_pConnection->OpenRequest( CHttpConnection::HTTP_VERB_GET,
			(LPCTSTR)strObject,NULL, 0,
			NULL, NULL,
			dwFlags );
		if( pHttpFile == NULL )
		{
			strError = "OpenRequest失败";
			return -1;
		}
		//如果是SOCKET5代理，设置账号密码
		// 		if   (!pHttpFile->SetOption(INTERNET_OPTION_PROXY_USERNAME,strProxyUser,strProxyUser.GetLength()))   
		// 		{   
		// 			return -99;
		// 		}
		// 		if   (!pHttpFile->SetOption(INTERNET_OPTION_PROXY_PASSWORD,strProxyPwd,strProxyPwd.GetLength()))   
		// 		{   
		// 			return -99;   
		// 		}
		//发送请求到服务器		
		iRet = SendRequest(pHttpFile,strError,strHead,strPostData,1);/*发送数据*/
		if (iRet < 0)	//发送POST数据失败
		{
			//清除连接对象
			DeleteObjectExEx();
			return iRet; 
		}
		//获取状态码及头信息
		pHttpFile->QueryInfoStatusCode(StatusCode);
		pHttpFile->QueryInfo(HTTP_QUERY_LOCATION,m_strRedirectUrl); 
		pHttpFile->QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF,m_strResponHead);
		iReadLen = (unsigned long )pHttpFile->GetLength();
		// 是否重定向
		if ( StatusCode == HTTP_STATUS_OK ||
			StatusCode == HTTP_STATUS_MOVED ||
			StatusCode == HTTP_STATUS_REDIRECT ||
			StatusCode == HTTP_STATUS_REDIRECT_METHOD)
		{
			//获取数据
			if (iGetType == 0)			//读取文本信息
			{
				CString strTemp = "";
				while(iReadLen >= 0)
				{
					memset(cBuffer,0,sizeof(cBuffer));
					iReadLen = pHttpFile->Read( cBuffer ,1023 );
					if (iReadLen <= 0)
					{
						break;
					}
					strTemp.Format("%s",cBuffer);					
					m_strWorkBuf += strTemp;
				}
			}
			else		//读取验证码
			{
				CStdioFile out;
				out.Open(g_strDirect+g_strFileName, CFile::modeCreate | CFile::modeWrite |CFile::typeBinary);
				while(true)
				{
					iReadLen = pHttpFile->Read(cBuffer ,1);
					if ( iReadLen <= 0)
					{
						break;
					}
					out.Write(cBuffer,1); 
				}
				out.Close();
			}			
		}
		//删除HTTPFILE对象
		DeleteObjectEx(pHttpFile);	
		if(!m_strWorkBuf.IsEmpty()) //解压数据
		{
			return ConvertData(m_strResponHead,strError,m_strWorkBuf);	
		}
		return 0;			
	}
	catch(CInternetException * e)
	{
		strError = "抓取网页信息异常";
		DeleteObjectEx(pHttpFile);	
		DeleteObjectExEx();
		e->Delete();
		return -6;
	}
	catch(...)
	{
		strError = "抓取网页信息异常";
		DeleteObjectEx(pHttpFile);	
		DeleteObjectExEx();
		return -7;
	}
	return 0;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CWebBase::SendRequest(CHttpFile *pHttpFile,CString &strError,CString strHeader,CString strPostData,int iPostFlag)
{
	int iRet = 0;
	BOOL bRet = FALSE;
	try
	{
		if (iPostFlag == 0)
		{
			bRet = pHttpFile->SendRequest(strHeader);
		}
		else
		{
			char szPostData[2048] = {0};
			strncpy(szPostData,strPostData,2048);
			bRet = pHttpFile->SendRequest(strHeader,szPostData,strPostData.GetLength());
		}
		if( bRet == FALSE )
		{
			pHttpFile->Close();
			delete pHttpFile;
			strError = "SendRequest失败";
			return -2;
		}
		return 0;
	}
	catch (CInternetException * e )
	{
		if ( pHttpFile )
		{
			pHttpFile->Close();
			delete pHttpFile;
			pHttpFile = NULL;
		}
		strError = "SendRequest异常";
		e->Delete();
		return -4; 
	}
	catch (...)
	{
		if ( pHttpFile )
		{
			pHttpFile->Close();
			delete pHttpFile;
			pHttpFile = NULL;
		}
		strError = "SendRequest异常";
		return -5;
	}
}
/************************************************************************/
/*获取连接                                                            */
/************************************************************************/
int CWebBase::GetHttpConnectionEx(CString & strServer, CString & strObject, 
								CString & strHead,CString &strError,
								int iIsProxy,	//是否使用代理
								int iProxyType,//代理类型
								CString strProxyIP,//代理IP
								int iProxyPort//代理端口
								)
{
	CString strTemp = "";
	WORD    wdPort;
	if ( m_pConnection == NULL )
	{
		if (iIsProxy == 1)		//使用代理
		{
			INTERNET_PROXY_INFO   proxyinfo;
			try   
			{   
				strTemp.Format("%s:%d",strProxyIP,iProxyPort);
				proxyinfo.dwAccessType = INTERNET_OPEN_TYPE_PROXY;   
				proxyinfo.lpszProxy = strTemp;   
				proxyinfo.lpszProxyBypass = NULL;
				if(!m_Session.SetOption(INTERNET_OPTION_PROXY,(LPVOID)&proxyinfo,sizeof(INTERNET_PROXY_INFO)))   
				{   
					strError = "设置代理失败";
					return -99;
				}
			}
			catch(...)
			{
				strError = "设置代理异常";
				return -99;
			}
		}
		try
		{
			if (m_iHttpsFlag == 0)
			{
				wdPort = INTERNET_INVALID_PORT_NUMBER;
				int nStringLen = strServer.Find(":");
				if( nStringLen != -1 )
				{
					strTemp  =  strServer.Mid( nStringLen + 1 );
					wdPort = atoi((LPCTSTR)strTemp);
					strTemp = strServer.Mid( 0,  nStringLen  );
					m_pConnection =
						m_Session.GetHttpConnection( strTemp,
						INTERNET_FLAG_KEEP_CONNECTION,
						wdPort,
						NULL, 
						NULL );

				}
				else
				{
					m_pConnection =
						m_Session.GetHttpConnection( strServer,
						INTERNET_FLAG_KEEP_CONNECTION,
						INTERNET_INVALID_PORT_NUMBER,
						NULL, 
						NULL );
				}
			}
			else		//是HTTPS协议
			{
				m_pConnection =
					m_Session.GetHttpConnection((LPCTSTR)strServer,
					INTERNET_FLAG_NO_CACHE_WRITE,
					INTERNET_DEFAULT_HTTPS_PORT,
					NULL, 
					NULL);
			}
			if (m_pConnection == NULL)
			{
				strError = "获取连接失败";
				return -3;
			}
		}
		catch (...)
		{
			if (m_pConnection)
			{
				m_pConnection->Close();
				delete m_pConnection;
				m_pConnection = NULL;
			}
			strError = "获取连接异常";
			return -99;
		}	
	}
	return 0;
}

/************************************************************************/
/*//解压数据                                                            */
/************************************************************************/
int CWebBase::ConvertData(CString strReponseHead,CString &strError,CString &strData)
{
	int nStringLen = -1;
	strReponseHead.MakeLower();
	//是被压缩过的， 要解压
	if(strReponseHead.Find("gzip" ) != -1) 
	{                        
		nStringLen = strData.GetLength();
		if( gzuncompress(strData, nStringLen ) != 0 )
		{
			strError = "解压失败";
			return -5;
		}
	}
	if( strReponseHead.Find("utf-8") != -1 && strReponseHead.Find("jpeg" ) == -1 )
	{                        
		ConvertUtf8ToGBK( strData );
	}  
	return 0;
}
/************************************************************************/
/*设置HTTPS协议类型                                                     */
/************************************************************************/
void CWebBase::SetHttpsType(int iType)
{
	if (iType >= 1)
	{
		m_iHttpsFlag = 1;
	}
	else
	{
		m_iHttpsFlag = 0;
	}
}
/************************************************************************/
/* 删除连接对象，释放资源												*/
/************************************************************************/
void CWebBase::DeleteObjectEx(CHttpFile *pHttpFile)
{
	if ( pHttpFile )
	{
		pHttpFile->Close();
		delete pHttpFile;
		pHttpFile = NULL;
	}
}
/************************************************************************/
/* 删除连接对象，释放资源												*/
/************************************************************************/
void CWebBase::DeleteObjectExEx()
{
	if (m_pConnection)
	{
		m_pConnection->Close();
		delete m_pConnection;
		m_pConnection = NULL;
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
DWORD CWebBase::GetDwflag()
{
	DWORD dwFlags = 0;
	if (m_iHttpsFlag == 1)			//如果是HTTPS协议
	{
		dwFlags = INTERNET_FLAG_NO_CACHE_WRITE |
			INTERNET_FLAG_SECURE |
			INTERNET_FLAG_IGNORE_CERT_CN_INVALID |
			INTERNET_FLAG_IGNORE_CERT_DATE_INVALID ;
	}
	else
	{
		dwFlags = INTERNET_FLAG_KEEP_CONNECTION;
	}
	if(m_nNotDirect)
	{
		dwFlags = dwFlags | INTERNET_FLAG_NO_AUTO_REDIRECT;
	}
	return dwFlags;
}