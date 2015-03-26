#pragma once

class CAmazonEncode
{
public:
	CAmazonEncode(void);
public:
	~CAmazonEncode(void);
	CString GenAuthKey();
private:
	int GetCeil(int iIntValue, int iFloatValue)
	{
		return iFloatValue == 0 ? iIntValue : (iIntValue + 1);
	}

	int GetFloor(int iIntValue, int iFloatValue)
	{
		return 1 + (iFloatValue == 0 ? iIntValue : (iIntValue - 1));
	}
	CString EncodeC(CString U);
	CString EncodeR(CString ab);
	CString Base64Encode(LPCTSTR strEncodeData, int nDataSize);
};