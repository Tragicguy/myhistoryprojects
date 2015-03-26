#include "stdafx.h"
#include "Function.h"
CFunction::CFunction(void)
{
}

CFunction::~CFunction(void)
{
}

int CFunction::GetAppPath(CString &strAppPath)
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

	}
	return iRet;	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CFunction::GetKeyString(CString &srcStr,CString strBefore,CString strAfter)
{
	int iPosBefore = 0;
	int iPosAfter = 0;
	iPosBefore = srcStr.Find(strBefore) + strBefore.GetLength();
	if (iPosBefore < strBefore.GetLength())
	{
		return "";
	}
	iPosAfter = srcStr.Find(strAfter,iPosBefore);
	if (iPosBefore < 0 || iPosAfter < 0 || iPosAfter - iPosBefore < 0 )
	{
		return "";
	}
	else
	{
		return srcStr.Mid(iPosBefore,iPosAfter - iPosBefore);
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CFunction::ConvertUtf8ToGBK(CString& strUtf8) 
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
void CFunction::ConvertUtf8ToGBK(string& strUtf8) 
{
	int len = 0;
	unsigned short * wszGBK = NULL;
	char * szGBKt = NULL;
	char * szGBKs = NULL;

	len = MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8.c_str(), -1, NULL,0);
	wszGBK = new unsigned short[len+1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8.c_str(), -1, (LPWSTR)wszGBK, len);

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
void CFunction::ConvertGBKToUtf8(CString& strGBK) 
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
CString CFunction::URLEncode(CString sIn)
{
	CString sOut;
	const int nLen = sIn.GetLength() + 1;
	register LPBYTE pOutTmp = NULL;
	LPBYTE pOutBuf = NULL;
	register LPBYTE pInTmp = NULL;
	LPBYTE pInBuf =(LPBYTE)sIn.GetBuffer(nLen);
	//alloc out buffer
	pOutBuf = (LPBYTE)sOut.GetBuffer(nLen*3);

	if(pOutBuf)
	{
		pInTmp   = pInBuf;
		pOutTmp = pOutBuf;
		// do encoding
		while (*pInTmp)
		{
			if(isalnum(*pInTmp) || '-'==*pInTmp || '_'==*pInTmp || '.'==*pInTmp)
				*pOutTmp++ = *pInTmp;
			else if(isspace(*pInTmp))
				*pOutTmp++ = '+';
			else
			{
				*pOutTmp++ = '%';
				*pOutTmp++ = toHex(*pInTmp>>4);//高4位
				*pOutTmp++ = toHex(*pInTmp%16);//低4位
			}
			pInTmp++;
		}
		*pOutTmp = '\0';
		sOut.ReleaseBuffer();
	}
	sIn.ReleaseBuffer();

	return sOut;
}


CString CFunction::Base64Encode(LPCTSTR strEncodeData, int nDataSize)
{
	CString strRetData = _T("");
	try
	{
		int nEncodedSize = nDataSize * 4 / 3 + 5 + nDataSize * 2 / 57;
		char *strEncodedData = new char[nEncodedSize + 1];
		// Base64 encoding
		char code[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		unsigned char * strData = (unsigned char *) strEncodeData;
		int i;
		int j;
		for (i = 0, j = 0; i < (nDataSize - nDataSize % 3); i += 3)
		{	  
			strEncodedData[j++] = code[strData[i] >> 2];
			strEncodedData[j++] = code[(strData[i] & 0x3) << 4 | strData[i + 1] >> 4];
			strEncodedData[j++] = code[(strData[i + 1] & 0xf) << 2 | strData[i + 2] >> 6];
			strEncodedData[j++] = code[strData[i + 2] & 0x3f];

			if (i % 57 >= 54)
			{
				strEncodedData[j++] = '\r';
				strEncodedData[j++] = '\n';
			}
		}

		if (nDataSize % 3 == 2)
		{
			strEncodedData[j++] = code[strData[i] >> 2];
			strEncodedData[j++] = code[(strData[i] & 0x3) << 4 | strData[i + 1] >> 4];
			strEncodedData[j++] = code[(strData[i + 1] & 0xf) << 2];
			strEncodedData[j++] = '=';
		}
		else if (nDataSize % 3 == 1)
		{
			strEncodedData[j++] = code[strData[i] >> 2];
			strEncodedData[j++] = code[(strData[i] & 0x3) << 4];
			strEncodedData[j++] = '=';
		}

		strEncodedData[j] = '\0';
		strRetData = strEncodedData;

		delete [] strEncodedData;
	}
	catch (CException *e)
	{

	}
	catch (...)
	{

	}

	return strRetData;
}

void CFunction::ClearSiteCookie(CString strFlag)
{
	TCHAR szPath[MAX_PATH] = {0};
	if (SHGetSpecialFolderPath(NULL, szPath, CSIDL_COOKIES, FALSE))
	{//得到目录，并清空
		char szFind[MAX_PATH]={0};
		WIN32_FIND_DATA FindFileData;
		strcpy(szFind,szPath);
		strcat(szFind,"\\*.txt");
		HANDLE hFind=::FindFirstFile(szFind,&FindFileData);
		if(INVALID_HANDLE_VALUE == hFind)    return;
		while(TRUE)
		{
			if (1)
			{
				CString strPath = "";
				strPath.Format("%s\\%s",szPath,FindFileData.cFileName);
				if ( strPath.Find(strFlag) >=0 )
				{
					CFile::Remove(strPath);
				}
			}
			if(!FindNextFile(hFind,&FindFileData))
			{
				break;
			}
		}
		FindClose(hFind);
	}
}

void CFunction::InitInfo(CString strData, map<CString, CString>& mapList)
{
	mapList.clear();
	CString strInner = "";
	CString strNo = "";
	while(true)
	{
		CString mid  = CFunction::GetKeyString(strData,"<option","option>");
		strInner = CFunction::GetKeyString(mid,">","<");
		strNo = CFunction::GetKeyString(mid,"value='","'");
		if(strNo.IsEmpty())
		{
			break;
		}
		mapList[strInner] = strNo;
		strData.Replace("<option"+mid+"option>","");
	}
}

void CFunction::InitInfo(CString strData, vector<CString>& vecList)
{
	vecList.clear();
	CString strInner = "";
	CString strNo = "";
	while(true)
	{
		CString mid  = CFunction::GetKeyString(strData,"<option","option>");
		strInner = CFunction::GetKeyString(mid,">","<");
		strNo = CFunction::GetKeyString(mid,"value='","'");
		if(strNo.IsEmpty())
		{
			break;
		}
		vecList.push_back(strInner);
		strData.Replace("<option"+mid+"option>","");
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
string CFunction::GetAppPath()
{
	char szExePath[_MAX_PATH] = {0};
	::GetModuleFileName(NULL, szExePath, _MAX_PATH);	
	char *szFlag = strrchr(szExePath,'\\');
	if (szFlag)
	{
		*szFlag = '\0';
	}
	string strRet = string(szExePath);
	strRet.append("\\");
	return strRet;
}