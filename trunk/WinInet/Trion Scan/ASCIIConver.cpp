// ASCIIConver.cpp: implementation of the CASCIIConver class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ASCIIConver.h"
#include "math.h"//pow
#include "dataType.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/************************************************************************/
/*                                                                      */
/************************************************************************/
CASCIIConver::CASCIIConver()
{
	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CASCIIConver::~CASCIIConver()
{

}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CASCIIConver::ASCIIstrtostr(CString strASCII)
{
	int i;
	CString strMsg = strASCII;

	char sUserData[512] = {0};
	int iLoop = (strMsg.GetLength() + 1) / 2;
	CString strInData="";
	for (i = 0; i < iLoop; i++)
	{
		if (i >= iLoop)
		{
			strInData = strMsg;
			if (strInData.GetLength() > 2)
				strInData.Left(2);
		}
		else
		{			
			strInData = strMsg.Left(2);
			strMsg = strMsg.Right(strMsg.GetLength() - 2);
		}
		sUserData[i] = fstr2int(strInData);
	}
	int UDLen=iLoop;
	if(UDLen>140)
	{
		UDLen=140;
	}
	return CString(sUserData);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CASCIIConver::str2ASCIIstr(CString strMsg)
{
	CString str = strMsg;
	CString strTemp="";
	CString strASCII = "";
	char chTmp[512] = {0};

	for (int i = 0; i < str.GetLength(); i++)
	{
		chTmp[i] = str.GetAt(i);
	}

	for (int i = 0; i < (int)strlen(chTmp); i++)
	{
		strTemp.Format("%02X", chTmp[i]);
		strTemp = strTemp.Right(2);
		strASCII += strTemp;
	}
	return strASCII;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CASCIIConver::fstr2int(CString strIn)
{
	int cNow;
	int iRet;
	iRet=0;
	for (long i=1; i >= 0; i--)
	{
		cNow = strIn.GetAt(1 - i);
		int  iDec = 0;
		switch (cNow){
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			iDec = cNow - '0';
			break;
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
			iDec = cNow - 'A' + 10;
			break;
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
			iDec = cNow - 'a' + 10;
			break;
		default:
			iDec = 0;
			break;
		};
		//iRet += (int)pow(long(16), long(i)) * iDec;		
	}
	return iRet;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CASCIIConver::AsciiToUS2(CString Msg)
{
	CString strMsg="";
	CString strTemp="";
	CString strUMsg="";
	wchar_t temp[1024]={0};
	char szUMsg[1024]={0};
	try
	{
		MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,Msg, strlen(Msg), temp, sizeof(temp));
		int len = wcslen(temp);
		for (int i=0;i<len;i++)
		{
			int iFirst = 1;
			for(int j=0;j<2;j++)
			{
				if(1 == iFirst)
				{
					szUMsg[2*i] = (temp[i]>>8) & 0xff;
					iFirst = 0;
				}
				else
				{
					szUMsg[2*i+1] = temp[i]&0xff;
					iFirst = 1;
				}
			}
		}
		strUMsg="";
		for (int i=0;i<2*len;i++)
		{
			strUMsg += "%";
			strTemp.Format("%02x", szUMsg[i]&0xff);
			strUMsg += strTemp;
		}
		return strUMsg;
	}
	catch(...)
	{
	};
	return "";
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CASCIIConver::US2ToAscii(unsigned char *InMsg, unsigned int InLen)
{
	CString strMsg="";
	CString strTemp="";
	wchar_t uMsg[1024]={0};
	unsigned int  uLen = 0;
	char temp[1024]={0};

	try
	{
		uLen = InLen;
		memset(uMsg, '\0', sizeof(uMsg));

		for (unsigned int i=0;i<uLen/2;i++)
		{
			uMsg[i] = InMsg[2*i]&0xff;
			uMsg[i] = uMsg[i] << 8;
			uMsg[i] = uMsg[i] + (InMsg[2*i+1]&0xff);
		}
		WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, uMsg, uLen, temp, sizeof(temp), NULL, NULL);

		return CString(temp);
	}
	catch(...)
	{
	}
	return "-99";
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CASCIIConver::AsciiToUS2(CString Msg, unsigned char *Data, unsigned int *Len)
{
	CString strMsg="";
	CString strTemp="";
	wchar_t temp[1024]={0};
	char szUMsg[1024]={0};
	try
	{
		MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,Msg, strlen(Msg), temp, sizeof(temp));
		int len = wcslen(temp);
		for (int i=0; i<len; i++){
			int iFirst = 1;
			for (int j=0; j<2; j++){
				if(1 == iFirst){
					szUMsg[2*i] = (temp[i]>>8) & 0xff;
					iFirst = 0;
				}
				else{
					szUMsg[2*i+1] = temp[i]&0xff;
					iFirst = 1;
				}
			}
		}
		*Len = 2*len;
		memcpy(Data, szUMsg, 2*len);
		return 0;
	}
	catch(...){
	}
	return -99;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CASCIIConver::US2ToAscii(unsigned char *InMsg, unsigned int InLen, unsigned char *Data, unsigned int *Len)
{
	CString strMsg="";
	CString strTemp="";
	wchar_t uMsg[1024]={0};
	unsigned int  uLen = 0;
	char temp[1024]={0};

	try
	{
		uLen = InLen;
		memset(uMsg, '\0', sizeof(uMsg));

		for (unsigned int i=0;i<uLen/2;i++)
		{
			uMsg[i] = InMsg[2*i]&0xff;
			uMsg[i] = uMsg[i] << 8;
			uMsg[i] = uMsg[i] + (InMsg[2*i+1]&0xff);
		}
		WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, uMsg, uLen, temp, sizeof(temp), NULL, NULL);

		*Len = uLen;
		memcpy(Data, temp, uLen);
		return 0;
	}
	catch(...)
	{
	};
	return -99;
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
//返回号码类型 -1错误，0移动，1联通，2小灵通(电信)
int CASCIIConver::MobileType(CString &Mobile)
{
	//负数错误。
	try
	{
		CString strMobile="";
		CString strFrst="";
		CString strHead="";

		int iMobiType = -1;
		int iNumLen = 0;
		if ("118" == Mobile.Left(3) || "116" == Mobile.Left(3))
		{
			strMobile = Mobile.Right(Mobile.GetLength() - 3);
		}
		else
		{
			strMobile = Mobile;
		}
		
		iNumLen = strMobile.GetLength();	
		if (iNumLen < 11 || iNumLen > 12)
		{
			return -1;
		}

		for (int i = 0; i < iNumLen; i++)
		{
			if (strMobile.GetAt(i) < 0x30 || strMobile.GetAt(i) > 0x39)
			{
				return -1;
			}
		}

		strFrst = strMobile.Left(1);	
		if ("0" == strFrst)//小灵通
		{
			if (0x30 == strMobile.GetAt(1)) //第二位也是0
			{
				return -1;
			}
			else if (0x31 == strMobile.GetAt(1) && 0x30 != strMobile.GetAt(2)) //不是010
			{//首位加了0的手机号码
				//return -1;
				strMobile = strMobile.Right(strMobile.GetLength() -1);
				strFrst = "1";
				iNumLen-- ;
			}
			else
			{
				iMobiType = ChinatelType(strMobile);
				return iMobiType;
			}
		}

		if ("1" == strFrst && 11 == iNumLen)	//手机号
		{
			strHead = strMobile.Mid(1, 2);
			iMobiType = m_PhonCom[atoi(strHead)];
		}
		else
		{
			iMobiType = -1;
		}
		Mobile = strMobile;
		return iMobiType;
	}
	catch (...)
	{
	}
	return -1;
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CASCIIConver::WebASCIItostr(CString WebASCII)
{
	CString strMsg = WebASCII;
	char sUserData[512] = {0};
	CString strInData="";
	
	int iLoop = 0;
	int iDataLen = 0;
	while (iLoop < strMsg.GetLength())
	{
		if (strMsg.GetAt(iLoop) == '%')
		{
			strInData = strMsg.Mid(iLoop+1, 2);
			sUserData[iDataLen] = fstr2int(strInData);
			iDataLen++;
			iLoop += 3;
		}
		else
		{
			sUserData[iDataLen] = strMsg.GetAt(iLoop);
			iDataLen++;
			iLoop++;
		}
	}
	return CString(sUserData);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CASCIIConver::WebASCIItostrEx(CString WebASCII, int EnCode)
{
	CString strMsg = WebASCII;
	char sUserData[512] = {0};
	CString strInData="";
	CString strTemp="";
	
	int iLoop = 0;
	int iDataLen = 0;
	while (iLoop < strMsg.GetLength())
	{
		if (strMsg.GetAt(iLoop) == '%')
		{
			strInData = strMsg.Mid(iLoop+1, 4);
			strTemp = strInData;
			strTemp.MakeUpper();
			if (strTemp == "FFFF")
			{
				strInData = strMsg.Mid(iLoop+5, 2);
				sUserData[iDataLen] = fstr2int(strInData);
				iDataLen++;
				strInData = strMsg.Mid(iLoop+7, 2);
				sUserData[iDataLen] = fstr2int(strInData);
				iDataLen++;
				iLoop += 9;
			}
			else
			{
				strInData = strMsg.Mid(iLoop+1, 2);
				sUserData[iDataLen] = fstr2int(strInData);
				iDataLen++;
				iLoop += 3;
			}
		}
		else
		{
			sUserData[iDataLen] = strMsg.GetAt(iLoop);
			iDataLen++;
			iLoop++;
		}
	}
	return (CString)sUserData;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CASCIIConver::ASCIIstrToChar(CString Msg, unsigned char *Buf, int *Len)
{
	try
	{
		int i = 0;
		CString strMsg = Msg;
		
		char sUserData[512] = {0};
		int iLoop = (strMsg.GetLength() + 1) / 2;
		CString strInData="";
		for (i = 0; i < iLoop; i++)
		{
			if (i >= iLoop)
			{
				strInData = strMsg;
				if (strInData.GetLength() > 2)
					strInData.Left(2);
			}
			else
			{			
				strInData = strMsg.Left(2);
				strMsg = strMsg.Right(strMsg.GetLength() - 2);
			}
			sUserData[i] = fstr2int(strInData);
		}
		
		*Len = iLoop;
		memcpy(Buf, sUserData, iLoop);
		return 0;
	}
	catch (...)
	{
	}
	return -99;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CASCIIConver::ChinatelType(CString Mobile)
{
	CString strMobile="";
	CString strHead="";
	int iMobiType = -1;	
	try
	{
		strMobile = Mobile;		
		if (strMobile.GetLength() < 10 || strMobile.GetLength() > 12)
		{
			iMobiType = -1;//错误
			return iMobiType;
		}
		
		strHead = strMobile.Left(2);
		if ("07" == strHead || "08" == strHead || "09" == strHead)
		{
			iMobiType = 2;//中国电信
		}
		else
		{
			strHead = strMobile.Left(3);
			if ("020" == strHead || 
				"021" == strHead || 
				"023" == strHead ||
				"027" == strHead ||
				"028" == strHead ||
				"029" == strHead ||
				"055" == strHead ||
				"056" == strHead ||
				"057" == strHead ||
				"058" == strHead ||
				"059" == strHead ||
				"066" == strHead ||
				"069" == strHead)
			{
				iMobiType = 2;//中国电信
			}
			else
			{
				iMobiType = 2;//北方网通
			}
		}
		return iMobiType;
	}
	catch (...)
	{
	}
	return -1;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CASCIIConver::SplitMsg(CString Msg, int nSplitLen, char *Msg1, char *Msg2)
{
	if (Msg.GetLength() <= 0)
	{
		return -1;
	}
	else if (Msg.GetLength() > 7200)
	{
		return -2;
	}
	
	CString strTemp="";
	wchar_t temp[7201] = {0}; //70*6
	wchar_t aa[7201] = {0};
	memset(Msg1, '\0', sizeof(Msg1));
	memset(Msg2, '\0', sizeof(Msg2));
	try
	{
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Msg, strlen(Msg), temp, sizeof(temp));
		int len = wcslen(temp);
		if (len > 0 && len <= nSplitLen){//只有一条信息
			wcsncpy(aa, temp, nSplitLen);
			strTemp = CString(aa);
			strcpy(Msg1, strTemp);
			return 0;
		}
		else if (len > nSplitLen)//超过一条信息
		{
			wcsncpy(aa, temp, nSplitLen);
			strTemp = CString(aa);
			strcpy(Msg1, strTemp);
			return 1;
		}
	}
	catch(...)
	{
	}
	return -9;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CASCIIConver::GetMsgULen(CString Msg)
{
	if (Msg.GetLength() > 7200 || Msg.GetLength() < 0)
	{
		return -1;
	}	
	wchar_t wszTemp[7201] = {0};
	
	try
	{
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Msg, strlen(Msg), wszTemp, sizeof(wszTemp));
		int len = wcslen(wszTemp);
		if (len < 0)
		{
			return -3;
		}
		return len;
	}
	catch(...)
	{
		WriteLog("GetMsgULen异常",3);
	}
	return -9;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CASCIIConver::GetFull70Msg(CString Msg, char *Dest)
{
	if (Msg.GetLength() > 140 || Msg.GetLength() <= 0)
	{
		return -1;
	}
	
	CString strRetMsg="";
	CString strTemp="";
	wchar_t temp[721] = {0}; //70*6
	wchar_t aa[721] = {0};
	try
	{
		strRetMsg = Msg;
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Msg, strlen(Msg), temp, sizeof(temp));
		int len = wcslen(temp);
		if (len < 70)
		{
			for (int i=0; i<69-len;i++)
			{
				strRetMsg += " ";
			}
			strRetMsg += ".";
		}
		else if (len > 70)
		{
			wcsncpy(aa, temp, 70);
			strRetMsg = CString(aa);
		}

		memcpy(Dest, strRetMsg, strRetMsg.GetLength());
		return 0;
	}
	catch(...)
	{
	}
	return -9;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CASCIIConver::CalcMobiNum(int Type, CString MobiList)
{
	CString strMobiList="";
	CString strMobile="";
	int iMobiNum = 0;
	int iType = 0;
	int iPos = -1;
	int iRetNum = 0;
	
	strMobiList = MobiList;
	iType = Type;
	
	try
	{
		if (strMobiList.GetLength() < 10)
		{
			return -1;//号码列表错误
		}
		
		iMobiNum = 0;
		while (1)
		{
			iPos = strMobiList.Find(',');
			if (-1 == iPos)
			{
				iMobiNum++;
				strMobile = strMobiList;
				strMobiList = "";
				
				//验证手机号码正确
				int iMobiType = MobileType(strMobile);
				if (iMobiType < 0)
				{
					return -1;//号码列表错误
				}
				break;
			}
			else
			{
				iMobiNum++;
				strMobile = strMobiList.Left(iPos);
				strMobiList = strMobiList.Mid( iPos + 1 );
				//验证手机号码正确
				int iMobiType = MobileType(strMobile);
				if (iMobiType < 0)
				{
					return -1;//号码列表错误
				}
			}
			if (iMobiNum > 1000)
			{
				return -1;
			}
		}
		return iMobiNum;
	}
	catch (...)
	{
		return -2;
	}
	return -3;
}

/************************************************************************/
/* shaojun add  2009/05/05                                              */
/************************************************************************/
int CASCIIConver::SplitMsg(CString &srcMsg, int nSplitLen, CString &strOutMsg)
{
	if (srcMsg.GetLength() <= 0)	//空信息
	{
		return -1;
	}
	wchar_t temp[7201] = {0}; //70*6
	wchar_t tempRet[7201] = {0};
	try
	{
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, srcMsg, strlen(srcMsg), temp, sizeof(temp));
		int len = wcslen(temp);
		if (len > 0 && len <= nSplitLen)	//只有一条信息
		{
			wcsncpy(tempRet, temp, nSplitLen);
			strOutMsg = CString(tempRet);
			return 0;
		}
		else if (len > nSplitLen)	//超过一条信息
		{
			wcsncpy(tempRet, temp, nSplitLen);
			strOutMsg = CString(tempRet);
			memset(tempRet,0,sizeof(tempRet));
			wcsncpy(tempRet,temp + nSplitLen,len - nSplitLen);
			srcMsg = CString(tempRet);// = (CString)aa;
			return 1;
		}
	}
	catch(...)
	{
		WriteLog("SplitMsg异常",3);
	}
	return -9;
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
void CASCIIConver::InitNumbers(CString strMobile,CString strUnicom,CString strTelecom)
{
	try
	{
		memset(m_PhonCom, -1, sizeof(m_PhonCom));
		CString arrayMobile[100];
		CString arrayUnicom[100];
		CString arrayTelecom[100];
		int iRet = 0;
		iRet = StringSplit(strMobile+",",arrayMobile,',');
		SetNumbers(arrayMobile,0,iRet);
		iRet = StringSplit(strUnicom+",",arrayUnicom,',');
		SetNumbers(arrayUnicom,1,iRet);
		iRet = StringSplit(strTelecom+",",arrayTelecom,',');
		SetNumbers(arrayTelecom,3,iRet);
	}
	catch (...)
	{
		WriteLog("InitNumbers异常",3);
	}
	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CASCIIConver::SetNumbers(CString strNumbers[],int iFlag,int iLen)
{
	try
	{
		for ( int i=0;i<iLen;i++)
		{
			int iIndex = -1;
			try
			{
				iIndex = atoi(strNumbers[i].Right(2));
				//AfxMessageBox(strNumbers[i]);
				if (iIndex < 0 )
				{
					iIndex = 0;
				}
			}
			catch (...)
			{
				iIndex = 100;
			}
			m_PhonCom[iIndex] = iFlag;
		}
	}
	catch (...)
	{
		WriteLog("SetNumbers异常",3);
	}
	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CASCIIConver::StringSplit(CString srcStr,CString strArray[],char splitChar)
{
	try
	{
		int idx1 = 0;
		int idx2 = 0;
		int iCount = 0;
		idx2 = srcStr.Find(splitChar, idx1);  //找第一个记录 
		while(idx2 != -1)						//如果找到
		{ 
			strArray[iCount] = srcStr.Mid(idx1,idx2 - idx1); //取到一个记录
			idx1 = idx2 + 1; 
			idx2 = srcStr.Find(splitChar, idx1);  //找下一个记录
			iCount++;
		} 
		return iCount;
	}
	catch (...)
	{
		WriteLog("StringSplit异常",3);
	}
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CASCIIConver::NativeToAscii(CString strSrc)
{
	wchar_t wszSrc[255] = {0};
	CString strResult= "";
	CString strTemp = "";
    try
	{
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, strSrc, strlen(strSrc), wszSrc, sizeof(wszSrc));
		int len = wcslen(wszSrc);
		for (int i=0;i<len;i++)
		{
			if (wszSrc[i] > 255)
			{
				strTemp.Format("%%u%02X",wszSrc[i]);
				//strTemp = "%u" + strTemp;
			}
			else
			{
				strTemp.Format("%c",wszSrc[i]);
			}
			strResult += strTemp;
		}
		return strResult;
	}
	catch(...)
	{
	}
	return "";
}