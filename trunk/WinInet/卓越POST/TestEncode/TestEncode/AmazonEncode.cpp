#include "StdAfx.h"
#include "AmazonEncode.h"

CAmazonEncode::CAmazonEncode(void)
{
	srand(time(NULL));
}

CAmazonEncode::~CAmazonEncode(void)
{
}

CString CAmazonEncode::GenAuthKey()
{
	time_t timer; time(&timer);
	long lStart = timer;
	int iTimeTail = rand() % 1000;
	int iElapsed = rand() % 300;
	CString strSrc = "";
	strSrc.Format("{\"version\":\"2.2.0\",\"start\":%ld%03d,\"elapsed\":%d,\"userAgent\":\"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.2; SV1; EmbeddedWB 14.52 from: http://www.bsalsa.com/ EmbeddedWB 14.52; .NET CLR 1.1.4322; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022; .NET CLR 2.0.50727; .NET4.0C)\",\"plugins\":\"ShockwaveFlash : 655363|AB 6,0,3790,3959|WDUN 6,0,3790,3959|DA 6,0,3,531|DAJC 6,0,1,223|DS 10,0,0,3997|DHDB 5,5000,3130,0|DHDBFJ 6,0,1,223|ICW 5,0,2918,1900|IE 6,0,3790,3959|WMP 10,0,0,3997|NN 4,4,0,4000|OBP 6,0,3790,3959|OE 6,0,3790,3959|TS 4,71,1968,1||1366-768-738-32-96-96-0\",\"flashVersion\":\"10.3\",\"timeZone\":-8}",
		lStart,
		iTimeTail,
		iElapsed
		);
	CString strX = EncodeC(strSrc);
	CString strRSrc = strX + "#" + strSrc;
	CString strR = EncodeR(strRSrc);
	return Base64Encode(strR, strR.GetLength());
}

CString CAmazonEncode::EncodeC(CString U)
{
	int iLen = U.GetLength();
	CString strShow = "";
	ULONG T = 3988292384;
	ULONG I[256] = {0};
	for (int X = 0; X < 256; X++)
	{
		ULONG W = X;
		for(int V = 0; V < 8; V++)
		{
			if ((W&1) == 1)
				W = (W>>1) ^ T;
			else
				W = (W >> 1);
		}
		I[X] = W;
	}
	ULONG Y=0;
	Y = Y^4294967295;
	for(int X=0;X<iLen;X++)
	{
		int S=(Y^U.GetAt(X))&255;
		Y=(Y>>8)^I[S];
	}
	Y=Y^4294967295;
	CString Z="0123456789ABCDEF";
	CString strRet = "";
	strRet.Format("%c%c%c%c%c%c%c%c",Z.GetAt((Y>>28)&15),Z.GetAt((Y>>24)&15),Z.GetAt((Y>>20)&15),Z.GetAt((Y>>16)&15),Z.GetAt((Y>>12)&15),Z.GetAt((Y>>8)&15),Z.GetAt((Y>>4)&15),Z.GetAt((Y)&15));
	return strRet;
}



CString CAmazonEncode::Base64Encode(LPCTSTR strEncodeData, int nDataSize)
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


CString CAmazonEncode::EncodeR(CString ab)
{
	int iLen = ab.GetLength();
	if(iLen==0){return"";}
	ULONG V[4] = {4169969034, 4087877101,1706678977, 3681020276};
	int U = GetCeil(iLen / 4, iLen % 4);
	ULONG *ad = new ULONG[U];
	for(int W=0;W<U;W++)
	{
		int iLenth = W*4;
		int a = iLenth > (iLen-1) ? 0 : ab.GetAt(iLenth)&255;
		int b = (iLenth+1) > (iLen-1) ? 0 : ((ab.GetAt(iLenth+1)&255)<<8);
		int c = (iLenth+2) > (iLen-1) ? 0 : ((ab.GetAt(iLenth+2)&255)<<16);
		int d = (iLenth+3) > (iLen-1) ? 0 : ((ab.GetAt(iLenth+3)&255)<<24);
		ad[W]=a + b + c + d;
	}
	ULONG ac=2654435769;
	int ae= GetFloor(52 / U, 52 % U) + 6;
	ULONG aa=ad[0];
	ULONG Z=ad[U-1];
	ULONG X=0;
	while(ae-->0){
		X+=ac;
		ULONG Y=(X>>2)&3;
		for(int S=0;S<U;S++){
			aa=ad[(S+1)%U];
			Z=ad[S]+=(((Z>>5)^(aa<<2))+((aa>>3)^(Z<<4)))^((X^aa)+(V[(S&3)^Y]^Z));
		}
	}
	CString strRet = "";
	CString strFormat = "";	
	for(int W=0;W<U;W++){
		strFormat.Format("%c%c%c%c", (ad[W])&255, (ad[W]>>8)&255, (ad[W]>>16)&255, (ad[W]>>24)&255);
		strRet += strFormat;
	}
	return strRet;
}



