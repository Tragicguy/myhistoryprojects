// AESEncryption.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "AESWrapper.h"
#include "AESEncryption.h"
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

AESWrapper m_AES;

AESENCRYPTION_API int GenerateKey(const int iStrength, const unsigned char key[])
{
	return m_AES.GenerateKey(iStrength,key);
}

AESENCRYPTION_API int Encrypt(const unsigned char in[], unsigned char out[],int iLength)
{
	return m_AES.Encrypt(in,out,iLength);
}

AESENCRYPTION_API int RestoreKey(const int iStrength, const unsigned char key[])
{
	return m_AES.RestoreKey(iStrength,key);
}

AESENCRYPTION_API int Decrypt(const unsigned char in[], unsigned char out[],int iLength)
{
	return m_AES.Decrypt(in,out,iLength);
}
