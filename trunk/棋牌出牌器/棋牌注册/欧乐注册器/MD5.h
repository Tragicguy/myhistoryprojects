#pragma once

// class MD5
// {
// public:
// 	MD5(void);
// 	~MD5(void);
// };
// 

//#pragma once
#include <string>

typedef unsigned char    UINT1;
typedef unsigned char *    POINTER;
typedef unsigned short    UINT2;
typedef unsigned long    UINT4;


/* MD5 context */
typedef struct {
	UINT4    state[4];        // state (ABCD)
	UINT4    count[2];        // number of bit, modulo 2 ^ 64
	UINT1    buffer[64];        // input buffer
} MD5_CTX;

void MD5Init(MD5_CTX*);
void MD5Update(MD5_CTX *, UINT1 *,    UINT4);
void MD5Final(UINT1 [16], MD5_CTX *);
std::string MD5Result(UINT1 [16]);
std::string MD5Encode(char* src);
