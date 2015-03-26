#pragma once
#include "..\AES\aescpp.h"




class AESENCRYPTION_CLASS AESWrapper
{
public:
	AESWrapper(void);
	~AESWrapper(void);
private:
	AESencrypt m_AESencrypt;
	AESdecrypt m_AESdncrypt;
public:
	int GenerateKey(const int iStrength, const unsigned char key[]);
	int RestoreKey(const int iStrength, const unsigned char key[]);
	int InitKey(const int iStrength, const unsigned char key[]);
	int Encrypt(const unsigned char in[], unsigned char out[],int iLength);
	int Decrypt(const unsigned char in[], unsigned char out[],int iLength);
};