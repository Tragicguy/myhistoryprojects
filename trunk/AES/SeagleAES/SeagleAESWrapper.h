#pragma once
#include "../SeagleAESDLL/seagleaes.h"

class SeagleAESWrapper
{
public:
	SeagleAESWrapper(void);
	~SeagleAESWrapper(void);

	bool Initialize(unsigned char* key, short iStrenth);
	int Encrypt(unsigned char* in, unsigned char* out, int iLength);
	int Decrypt(unsigned char* in, unsigned char* out, int iLength);
private:
	SeagleAES* m_Seagle;
};
