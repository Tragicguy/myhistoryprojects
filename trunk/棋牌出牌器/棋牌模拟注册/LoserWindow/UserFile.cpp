#include "StdAfx.h"
#include ".\userfile.h"
#pragma comment(lib, "VERSION.LIB")
CUserFile *CUserFile:: m_Instance=NULL;
CCriticalSection CUserFile::m_slockInstance;
/************************************************************************/
/*                                                                      */
/************************************************************************/
CUserFile::CUserFile(void)
{
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CUserFile::~CUserFile(void)
{
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CUserFile * CUserFile::GetInstance()
{
	if(m_Instance==NULL)
	{
		m_slockInstance.Lock();
		if(m_Instance==NULL)
		{
			m_Instance=new CUserFile;
		}
		m_slockInstance.Unlock();
	}
	return m_Instance;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CUserFile::WriteInitInfo(CString strfilePath,CString strnodeName,CString strKeyName,CString strKeyValue,int iKeyValue,int iFlag)
{
	//数据库配置
	CString strTemp = "";
	try
	{
		if ( iFlag == 0 )	//字符串
		{
			strTemp.Format("%s",strKeyValue);
		}
		else
		{
			strTemp.Format("%d",iKeyValue);
		}
		::WritePrivateProfileString(strnodeName,strKeyName, strTemp, strfilePath);
		return 0;
	}
	catch (...)
	{
		//WriteLog("WriteInitInfo异常",3);
	}
	return -99;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CUserFile::ReadInitInfo(CString strfilePath,CString strnodeName,CString strKeyName,CString &strKeyValue,int &iKeyValue,int iFlag)
{
	char szTemp[2001] = {0};
	try
	{
		if (iFlag == 0)	//字符串
		{
			::GetPrivateProfileString(strnodeName,strKeyName, "",(LPSTR)szTemp,2001, strfilePath);
			strKeyValue.Format("%s",szTemp);
			if (strKeyValue == "")
			{
				return -1;
			}
		}
		else	//数字
		{
			iKeyValue = ::GetPrivateProfileInt(strnodeName,strKeyName, -99, strfilePath);
			if (iKeyValue == -99)
			{
				return -1;
			}
		}
		return iFlag;
	}
	catch (...)
	{
		//WriteLog("ReadInitInfo异常",3);
	}	
	return -99;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//返回应用程序所在目录，包含"\"
int CUserFile::GetAppPath(CString &strAppPath)
{
	int iRet = -3;
	try
	{
		TCHAR szBuff[_MAX_PATH];
		VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szBuff, _MAX_PATH));
		strAppPath.Format("%s",szBuff);
		int pos=strAppPath.ReverseFind('\\');
		strAppPath=strAppPath.Left(pos+1);		
		iRet = 0;
	}
	catch (...)
	{
		//WriteLog("GetAppPath异常",3);
	}
	return iRet;	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CUserFile::CheckInitValue(int &iValue,int iMin,int iMax)
{
	if (iValue < iMin)
	{
		iValue = iMin;
	}
	else if (iValue > iMax)
	{
		iValue = iMax;
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//加密成16进制字符串
CString CUserFile::StrEncode(CString srcStr)
{
	int             i ;
	int             nLen = 0;
	CString         newString = "";
	CString         tempStr = "";
	unsigned char   ucTemp = 0;
	nLen =  srcStr.GetLength( );
	//加密后转成16进制
	for( i = 0; i < nLen; i++ )
	{
		ucTemp = ( unsigned char )srcStr.GetAt(i);
		tempStr.Format("%02X", ucTemp^(i%255) );
		newString +=tempStr;
	}
	return newString;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//解密16进制字符串
CString CUserFile::StrDecode(CString srcStr)
{
	CString         newString = "";
	CString         tempStr = "";
	//解密16进制字符串
	int i = 0;
	while( srcStr.GetLength() > 0 ) 
	{ 
		CString temp1 = "0x" + srcStr.Left(2); //取到一个16进制字串
		int ldec = strtol(temp1, NULL, 16); //将16进制转换为10进制
		CString temp = "";
		temp.Format("%c",(unsigned char )ldec^(i%255));
		newString += temp;
		srcStr = srcStr.Right( srcStr.GetLength() - 2);
		i ++;
	} 
	return newString;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CUserFile::SplitMsg(CString srcStr,CString *strArray,CString strSplit,int iLen)
{
	int i = 0;
	int idx = 0;
	srcStr.Replace(strSplit," ");
	srcStr.Trim();
	if ( srcStr == "" )
	{
		return;
	}
	srcStr += " ";	
	while(true)						//如果找到
	{
		idx = srcStr.Find(' ',0);  //找第一个记录
		if ( idx != -1 )
		{
			strArray[i] = srcStr.Left( idx );
			srcStr = srcStr.Mid(idx + 1);
			i++;
			if ( i >= iLen)
			{
				break;
			}
		}
		else
		{
			break;
		}
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CUserFile::GetFileVersion(CString strFileName)   
{      
	int iVerInfoSize;   
	char *pBuf;   
	CString asVer="";   
	VS_FIXEDFILEINFO *pVsInfo;   
	unsigned int iFileInfoSize = sizeof(VS_FIXEDFILEINFO);
	iVerInfoSize   =   GetFileVersionInfoSize(strFileName,NULL);
	if(iVerInfoSize!= 0)   
	{      
		pBuf = new char[iVerInfoSize];   
		if(GetFileVersionInfo(strFileName,0,iVerInfoSize,pBuf))
		{      
			if(VerQueryValue(pBuf,"\\",(void**)&pVsInfo,&iFileInfoSize))      
			{      
				asVer.Format("%d.%d.%d.%d",HIWORD(pVsInfo->dwFileVersionMS),LOWORD(pVsInfo->dwFileVersionMS),HIWORD(pVsInfo->dwFileVersionLS),LOWORD(pVsInfo->dwFileVersionLS));   
			}      
		}      
		delete[] pBuf;
	}
	return "v"+asVer;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CUserFile::GetCurrentAppPath()   
{   
	char szFile[MAX_PATH];   
	GetModuleFileName(NULL,szFile,MAX_PATH);	
	return CString(szFile);   
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CUserFile::GetModifyTime(CString appname)   
{   
	WIN32_FIND_DATA ffd ;   
	HANDLE hFind = FindFirstFile(appname,&ffd);   
	SYSTEMTIME stUTC, stLocal;   
	FileTimeToSystemTime(&(ffd.ftLastWriteTime), &stUTC);   
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);   
	CString strTmp = "";   
	strTmp.Format("%d-%d-%d,%d:%d", stLocal.wYear,stLocal.wMonth,stLocal.wDay,stLocal.wHour,stLocal.wMinute);   
	return strTmp;   
}