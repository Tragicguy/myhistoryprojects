//取得号段
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//加密算法MD5和DES
/////////////////////////////DES加密与解密/////////////////////////////////////
enum {ENCRYPT,DECRYPT};
#define true 1
#define false 0
/* MD5 context. */
typedef unsigned long UINT4;
typedef unsigned char *POINTER;
typedef struct {
  UINT4 state[4];                                   /* state (ABCD) */
  UINT4 count[2];        /* number of bits, modulo 2^64 (lsb first) */
  unsigned char buffer[64];                         /* input buffer */
} MD5_CTX;

void MD5Init(MD5_CTX *);/* context */
void MD5Update(MD5_CTX *context,unsigned char * input, unsigned int inputLen);
void MD5Final(unsigned char digest[16], MD5_CTX *context);
static void MD5Transform(UINT4 state[4],unsigned char block[64]);
static void MD5_memcpy(POINTER output,POINTER input,unsigned int len);
static void MD5Transform(UINT4 state[4],unsigned char block[64]);
static void Encode(unsigned char *output,UINT4 * input,unsigned int len);
static void MD5_memset(POINTER output, int value,unsigned int len);
static void Decode(UINT4 *output,unsigned char * input,unsigned int len);
int __stdcall MD5(unsigned char *out,const char *in);
int __stdcall MD5String(char *out,const char *in);

