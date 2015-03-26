#include "StdAfx.h"
#include ".\seagleaeswrapper.h"

SeagleAESWrapper::SeagleAESWrapper(void)
{
	m_Seagle = NULL;
}

SeagleAESWrapper::~SeagleAESWrapper(void)
{
	if(m_Seagle != NULL)
	{
		delete m_Seagle;
		m_Seagle = NULL;
	}
}

bool SeagleAESWrapper::Initialize(unsigned char* key, short iStrenth)
{
	try
	{
		if(key == NULL) return false;

		if(iStrenth == 128 || iStrenth == 192 || iStrenth == 256)
		{
			m_Seagle = new SeagleAES(iStrenth,key);
			return (m_Seagle != NULL) ? true : false;
		}
		return false;
	}
	catch(...)
	{
		return false;
	}
}

int SeagleAESWrapper::Encrypt(unsigned char* in, unsigned char* out, int iLength)
{
	try
	{
		size_t iCount = iLength;
		
		if(iCount==0) return false;

		for(size_t i=0; i<iCount; i+=16)
		{
			m_Seagle->Cipher(in + i,out + i);
		}
		return 1;
	}
	catch(...)
	{
		return 0;
	}
}


int SeagleAESWrapper::Decrypt(unsigned char* in, unsigned char* out, int iLength)
{
	try
	{
		size_t iCount = iLength;

		if(iCount==0) return false;

		for(size_t i=0; i<iCount; i+=16)
		{
			m_Seagle->InvCipher(in + i,out + i);
		}
		return 1;
	}
	catch(...)
	{
		return 0;
	}
}



