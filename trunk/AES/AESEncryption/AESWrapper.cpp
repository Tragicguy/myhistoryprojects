#include "aeswrapper.h"

AESWrapper::AESWrapper(void)
{

}

AESWrapper::~AESWrapper(void)
{
	
}

int AESWrapper::InitKey(const int iStrength, const unsigned char key[])
{
	GenerateKey(iStrength,key);
	return RestoreKey(iStrength,key);
}
int AESWrapper::GenerateKey(const int iStrength, const unsigned char key[])
{
	switch(iStrength)
	{
	case 128:
		{
			return (int)m_AESencrypt.key128(key);
		}
	case 192:
		{
			return (int)m_AESencrypt.key192(key);
		}
	case 256:
		{
			return (int)m_AESencrypt.key256(key);
		}
	default:
		{
			return 0;
		}
	}
}

int AESWrapper::Encrypt(const unsigned char in[], unsigned char out[],int iLength)
{
	size_t iCount = iLength;
	
	if(iCount==0) return false;

	for(size_t i=0; i<iCount; i+=16)
	{
		if(m_AESencrypt.encrypt(in + i,out + i) != 0)
		{
			return 0;
		}
	}
	return 1;
}

int AESWrapper::RestoreKey(const int iStrength, const unsigned char key[])
{
	switch(iStrength)
	{
	case 128:
		{
			return (int)m_AESdncrypt.key128(key);
		}
	case 192:
		{
			return (int)m_AESdncrypt.key192(key);
		}
	case 256:
		{
			return (int)m_AESdncrypt.key256(key);
		}
	default:
		{
			return 0;
		}
	}
}

int AESWrapper::Decrypt(const unsigned char in[], unsigned char out[],int iLength)
{
	size_t iCount = iLength;

	if(iCount==0) return false;

	for(size_t i=0; i<iCount; i+=16)
	{
		if(m_AESdncrypt.decrypt(in + i,out + i) != 0)
		{
			return 0;
		}
	}
	return 1;
}





