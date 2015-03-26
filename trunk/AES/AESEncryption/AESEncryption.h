#ifndef AES_ENCRYPTION_H

#ifdef AESENCRYPTION_EXPORTS
#define AESENCRYPTION_API __declspec(dllexport)
#else
#define AESENCRYPTION_API __declspec(dllimport)
#endif

AESENCRYPTION_API int GenerateKey(const int iStrength, const unsigned char key[]);
AESENCRYPTION_API int Encrypt(const unsigned char in[], unsigned char out[],int iLength);
AESENCRYPTION_API int RestoreKey(const int iStrength, const unsigned char key[]);
AESENCRYPTION_API int Decrypt(const unsigned char in[], unsigned char out[],int iLength);

#endif