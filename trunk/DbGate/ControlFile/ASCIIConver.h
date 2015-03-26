// ASCIIConver.h: interface for the CASCIIConver class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASCIICONVER_H__F06817E7_D125_4C55_83BC_A8959DAB5AB7__INCLUDED_)
#define AFX_ASCIICONVER_H__F06817E7_D125_4C55_83BC_A8959DAB5AB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CASCIIConver  
{
public:
	CASCIIConver();
	virtual ~CASCIIConver();
	int CalcMobiNum(int Type, CString MobiList);
	int GetFull70Msg(CString Msg, char *Dest);
	int GetMsgULen(CString Msg);
	int SplitMsg(CString Msg, int nSplitLen, char *Msg1, char *Msg2);
	
	int ChinatelType(CString Mobile);
	int ASCIIstrToChar(CString Msg, unsigned char *Buf, int *Len);
	CString WebASCIItostrEx(CString WebASCII, int EnCode);
	CString WebASCIItostr(CString WebASCII);
	int MobileType(CString &Mobile);
	int US2ToAscii(unsigned char *InMsg, unsigned int InLen, unsigned char *Data, unsigned int *Len);
	CString US2ToAscii(unsigned char *InMsg, unsigned int InLen);

	int AsciiToUS2(CString Msg, unsigned char *Data, unsigned int *Len);
	CString AsciiToUS2(CString Msg);
	int fstr2int(CString strIn);
	CString str2ASCIIstr(CString strMsg);
	CString ASCIIstrtostr(CString strASCII);
	
	int SplitMsg(CString &srcMsg, int nSplitLen, CString &strOutMsg);
	int StringSplit(CString srcStr,CString strArray[],char splitChar);
	void SetNumbers(CString strNumbers[],int iFlag,int iLen);
	void InitNumbers(CString strMobile,CString strUnicom,CString strTelecom);	
protected:
	char m_PhonCom[101];
};

#endif // !defined(AFX_ASCIICONVER_H__F06817E7_D125_4C55_83BC_A8959DAB5AB7__INCLUDED_)
