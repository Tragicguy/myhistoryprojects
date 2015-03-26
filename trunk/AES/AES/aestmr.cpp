/*
 ---------------------------------------------------------------------------
 Copyright (c) 2002, Dr Brian Gladman, Worcester, UK.   All rights reserved.

 LICENSE TERMS

 The free distribution and use of this software in both source and binary
 form is allowed (with or without changes) provided that:

   1. distributions of this source code include the above copyright
      notice, this list of conditions and the following disclaimer;

   2. distributions in binary form include the above copyright
      notice, this list of conditions and the following disclaimer
      in the documentation and/or other associated materials;

   3. the copyright holder's name is not used to endorse products
      built using this software without specific written permission.

 ALTERNATIVELY, provided that this notice is retained in full, this product
 may be distributed under the terms of the GNU General Public License (GPL),
 in which case the provisions of the GPL apply INSTEAD OF those given above.

 DISCLAIMER

 This software is provided 'as is' with no explicit or implied warranties
 in respect of its properties, including, but not limited to, correctness
 and/or fitness for purpose.
 ---------------------------------------------------------------------------
 Issue 28/01/2004
*/

/* Measure the Encryption, Decryption and Key Setup Times for AES using
   the Pentium Time Stamp Counter  */

#include <iostream>
#include <fstream>
#include <memory.h>
#include <cmath>

#if defined( AES_CPP )
#include "aescpp.h"
#else
#include "aes.h"
#endif
#include "aesaux.h"
#include "aestst.h"

// Use this define if testing aespp.c

//#define AESX

#if defined(AES_DLL)
fn_ptrs fn;
#endif

#define t_pro(hi,lo)                \
    __asm   {   __asm   cpuid       \
                __asm   rdtsc       \
                __asm   mov lo,eax  \
                __asm   mov hi,edx  \
            }

#define t_epi(hi,lo,cy)             \
    __asm   {   __asm   rdtsc       \
                __asm   sub eax,lo  \
                __asm   sbb edx,hi  \
                __asm   mov lo,eax  \
                __asm   mov hi,edx  \
            }                       \
    cy = ((unsigned long long)hi << 32) | lo

#define t_epif(hi,lo,cy)            \
    __asm   {   __asm   rdtsc       \
                __asm   sub eax,lo  \
                __asm   sbb edx,hi  \
                __asm   mov lo,eax  \
                __asm   mov hi,edx  \
            }                       \
    cy = (double)(((unsigned long long)hi << 32) | lo)

#define  PROCESSOR   "P4"  // Processor

const unsigned int loops = 100; // number of timing loops

word rand32(void)
{   static word   r4,r_cnt = -1,w = 521288629,z = 362436069;

    z = 36969 * (z & 65535) + (z >> 16);
    w = 18000 * (w & 65535) + (w >> 16);

    r_cnt = 0; r4 = (z << 16) + w; return r4;
}

byte rand8(void)
{   static word   r4,r_cnt = 4;

    if(r_cnt == 4)
    {
        r4 = rand32(); r_cnt = 0;
    }

    return (char)(r4 >> (8 * r_cnt++));
}

// fill a block with random charactrers

void block_rndfill(byte l[], word len)
{   word  i;

    for(i = 0; i < len; ++i)

        l[i] = rand8();
}

#define SAMPLE1  1000
#define SAMPLE2 10000

bool time_base(double& av, double& sig)
{   int                 i, tol, lcnt, sam_cnt;
    unsigned int        yl, yh;
    double              cy, av1, sig1;

    tol = 10; lcnt = sam_cnt = 0;
    while(!sam_cnt)
    {
        av1 = sig1 = 0.0;

        for(i = 0; i < SAMPLE1; ++i)
        {
            t_pro(yh, yl);
            t_epif(yh, yl, cy);

            av1 += cy;
            sig1 += cy * cy;
        }

        av1 /= SAMPLE1;
        sig1 = sqrt((sig1 - av1 * av1 * SAMPLE1) / SAMPLE1);
        sig1 = (sig1 < 0.05 * av1 ? 0.05 * av1 : sig1);

        av = sig = 0.0;
        for(i = 0; i < SAMPLE2; ++i)
        {
            t_pro(yh, yl);
            t_epif(yh, yl, cy);

            if(cy > av1 - sig1 && cy < av1 + sig1)
            {
                av += cy;
                sig += cy * cy;
                sam_cnt++;
            }
        }

        if(10 * sam_cnt > 9 * SAMPLE2)
        {
            av /= sam_cnt;
            sig = sqrt((sig - av * av * sam_cnt) / sam_cnt);
            if(sig > (tol / 100.0) * av)
                sam_cnt = 0;
        }
        else
        {
            if(lcnt++ == 10)
            {
                lcnt = 0; tol += 5;
                if(tol > 30)
                    return false;
            }
            sam_cnt = 0;
        }
    }

    return true;
}

typedef unsigned long long (*aes_tmr)(void);

bool time_eks8(unsigned int k_len, double& av, double& sig)
{   int                 i, tol, lcnt, sam_cnt;
    unsigned int        yl, yh;
    double              cy, av1, sig1;
    unsigned char       key[8][2 * AES_BLOCK_SIZE];
    f_ectx              ec[1];

    block_rndfill(key[0], 16 * AES_BLOCK_SIZE);

    tol = 10; lcnt = sam_cnt = 0;
    while(!sam_cnt)
    {
        av1 = sig1 = 0.0;

        for(i = 0; i < SAMPLE1; ++i)
        {
            t_pro(yh, yl);
            f_enc_key(ec, key[0], k_len);
            f_enc_key(ec, key[1], k_len);
            f_enc_key(ec, key[2], k_len);
            f_enc_key(ec, key[3], k_len);
            f_enc_key(ec, key[4], k_len);
            f_enc_key(ec, key[5], k_len);
            f_enc_key(ec, key[6], k_len);
            f_enc_key(ec, key[7], k_len);
            t_epif(yh, yl, cy);

            av1 += cy;
            sig1 += cy * cy;
        }

        av1 /= SAMPLE1;
        sig1 = sqrt((sig1 - av1 * av1 * SAMPLE1) / SAMPLE1);
        sig1 = (sig1 < 0.05 * av1 ? 0.05 * av1 : sig1);

        f_enc_key(ec, key[0], k_len);
        av = sig = 0.0;
        for(i = 0; i < SAMPLE2; ++i)
        {
            t_pro(yh, yl);
            f_enc_key(ec, key[0], k_len);
            f_enc_key(ec, key[1], k_len);
            f_enc_key(ec, key[2], k_len);
            f_enc_key(ec, key[3], k_len);
            f_enc_key(ec, key[4], k_len);
            f_enc_key(ec, key[5], k_len);
            f_enc_key(ec, key[6], k_len);
            f_enc_key(ec, key[7], k_len);
            t_epif(yh, yl, cy);

            if(cy > av1 - sig1 && cy < av1 + sig1)
            {
                av += cy;
                sig += cy * cy;
                sam_cnt++;
            }
        }

        if(10 * sam_cnt > 9 * SAMPLE2)
        {
            av /= sam_cnt;
            sig = sqrt((sig - av * av * sam_cnt) / sam_cnt);
            if(sig > (tol / 100.0) * av)
                sam_cnt = 0;
        }
        else
        {
            if(lcnt++ == 10)
            {
                lcnt = 0; tol += 5;
                if(tol > 30)
                    return false;
            }
            sam_cnt = 0;
        }
    }

    return true;
}

bool time_dks8(unsigned int k_len, double& av, double& sig)
{   int                 i, tol, lcnt, sam_cnt;
    unsigned int        yl, yh;
    double              cy, av1, sig1;
    unsigned char       key[8][2 * AES_BLOCK_SIZE];
    f_dctx              dc[1];

    block_rndfill(key[0], 16 * AES_BLOCK_SIZE);

    tol = 10; lcnt = sam_cnt = 0;
    while(!sam_cnt)
    {
        av1 = sig1 = 0.0;

        for(i = 0; i < SAMPLE1; ++i)
        {
            t_pro(yh, yl);
            f_dec_key(dc, key[ 0], k_len);
            f_dec_key(dc, key[ 1], k_len);
            f_dec_key(dc, key[ 2], k_len);
            f_dec_key(dc, key[ 3], k_len);
            f_dec_key(dc, key[ 4], k_len);
            f_dec_key(dc, key[ 5], k_len);
            f_dec_key(dc, key[ 6], k_len);
            f_dec_key(dc, key[ 7], k_len);
            t_epif(yh, yl, cy);

            av1 += cy;
            sig1 += cy * cy;
        }

        av1 /= SAMPLE1;
        sig1 = sqrt((sig1 - av1 * av1 * SAMPLE1) / SAMPLE1);
        sig1 = (sig1 < 0.05 * av1 ? 0.05 * av1 : sig1);

        av = sig = 0.0;
        for(i = 0; i < SAMPLE2; ++i)
        {
            t_pro(yh, yl);
            f_dec_key(dc, key[ 0], k_len);
            f_dec_key(dc, key[ 1], k_len);
            f_dec_key(dc, key[ 2], k_len);
            f_dec_key(dc, key[ 3], k_len);
            f_dec_key(dc, key[ 4], k_len);
            f_dec_key(dc, key[ 5], k_len);
            f_dec_key(dc, key[ 6], k_len);
            f_dec_key(dc, key[ 7], k_len);
            t_epif(yh, yl, cy);

            if(cy > av1 - sig1 && cy < av1 + sig1)
            {
                av += cy;
                sig += cy * cy;
                sam_cnt++;
            }
        }

        if(10 * sam_cnt > 9 * SAMPLE2)
        {
            av /= sam_cnt;
            sig = sqrt((sig - av * av * sam_cnt) / sam_cnt);
            if(sig > (tol / 100.0) * av)
                sam_cnt = 0;
        }
        else
        {
            if(lcnt++ == 10)
            {
                lcnt = 0; tol += 5;
                if(tol > 30)
                    return false;
            }
            sam_cnt = 0;
        }
    }

    return true;
}

bool time_enc16(unsigned int k_len, double& av, double& sig)
{   int                 i, tol, lcnt, sam_cnt;
    unsigned int        yl, yh;
    double              cy, av1, sig1;
    unsigned char       pt[2 * AES_BLOCK_SIZE];
    f_ectx              ec[1];

    block_rndfill(pt, 2 * AES_BLOCK_SIZE);
    f_enc_key(ec, pt, k_len);
    block_rndfill(pt,  AES_BLOCK_SIZE);

    tol = 10; lcnt = sam_cnt = 0;
    while(!sam_cnt)
    {
        av1 = sig1 = 0.0;

        for(i = 0; i < SAMPLE1; ++i)
        {
            t_pro(yh, yl);
            f_enc_blk(ec, pt, pt);
            f_enc_blk(ec, pt, pt);
            f_enc_blk(ec, pt, pt);
            f_enc_blk(ec, pt, pt);
            f_enc_blk(ec, pt, pt);
            f_enc_blk(ec, pt, pt);
            f_enc_blk(ec, pt, pt);
            f_enc_blk(ec, pt, pt);
            f_enc_blk(ec, pt, pt);
            f_enc_blk(ec, pt, pt);
            f_enc_blk(ec, pt, pt);
            f_enc_blk(ec, pt, pt);
            f_enc_blk(ec, pt, pt);
            f_enc_blk(ec, pt, pt);
            f_enc_blk(ec, pt, pt);
            f_enc_blk(ec, pt, pt);
            t_epif(yh, yl, cy);

            av1 += cy;
            sig1 += cy * cy;
        }

        av1 /= SAMPLE1;
        sig1 = sqrt((sig1 - av1 * av1 * SAMPLE1) / SAMPLE1);
        sig1 = (sig1 < 0.05 * av1 ? 0.05 * av1 : sig1);

        av = sig = 0.0;
        for(i = 0; i < SAMPLE2; ++i)
        {
            t_pro(yh, yl);
            f_enc_blk(ec, pt, pt);
            f_enc_blk(ec, pt, pt);
            f_enc_blk(ec, pt, pt);
            f_enc_blk(ec, pt, pt);
            f_enc_blk(ec, pt, pt);
            f_enc_blk(ec, pt, pt);
            f_enc_blk(ec, pt, pt);
            f_enc_blk(ec, pt, pt);
            f_enc_blk(ec, pt, pt);
            f_enc_blk(ec, pt, pt);
            f_enc_blk(ec, pt, pt);
            f_enc_blk(ec, pt, pt);
            f_enc_blk(ec, pt, pt);
            f_enc_blk(ec, pt, pt);
            f_enc_blk(ec, pt, pt);
            f_enc_blk(ec, pt, pt);
            t_epif(yh, yl, cy);

            if(cy > av1 - sig1 && cy < av1 + sig1)
            {
                av += cy;
                sig += cy * cy;
                sam_cnt++;
            }
        }

        if(10 * sam_cnt > 9 * SAMPLE2)
        {
            av /= sam_cnt;
            sig = sqrt((sig - av * av * sam_cnt) / sam_cnt);
            if(sig > (tol / 100.0) * av)
                sam_cnt = 0;
        }
        else
        {
            if(lcnt++ == 10)
            {
                lcnt = 0; tol += 5;
                if(tol > 30)
                    return false;
            }
            sam_cnt = 0;
        }
    }

    return true;
}

bool time_dec16(unsigned int k_len, double& av, double& sig)
{   int                 i, tol, lcnt, sam_cnt;
    unsigned int        yl, yh;
    double              cy, av1, sig1;
    unsigned char       pt[2 * AES_BLOCK_SIZE];
    f_dctx              dc[1];

    block_rndfill(pt, 32);
    f_dec_key(dc, pt, k_len);
    block_rndfill(pt,  16);

    tol = 10; lcnt = sam_cnt = 0;
    while(!sam_cnt)
    {
        av1 = sig1 = 0.0;

        for(i = 0; i < SAMPLE1; ++i)
        {
            t_pro(yh, yl);
            f_dec_blk(dc, pt, pt);
            f_dec_blk(dc, pt, pt);
            f_dec_blk(dc, pt, pt);
            f_dec_blk(dc, pt, pt);
            f_dec_blk(dc, pt, pt);
            f_dec_blk(dc, pt, pt);
            f_dec_blk(dc, pt, pt);
            f_dec_blk(dc, pt, pt);
            f_dec_blk(dc, pt, pt);
            f_dec_blk(dc, pt, pt);
            f_dec_blk(dc, pt, pt);
            f_dec_blk(dc, pt, pt);
            f_dec_blk(dc, pt, pt);
            f_dec_blk(dc, pt, pt);
            f_dec_blk(dc, pt, pt);
            f_dec_blk(dc, pt, pt);
            t_epif(yh, yl, cy);

            av1 += cy;
            sig1 += cy * cy;
        }

        av1 /= SAMPLE1;
        sig1 = sqrt((sig1 - av1 * av1 * SAMPLE1) / SAMPLE1);
        sig1 = (sig1 < 0.05 * av1 ? 0.05 * av1 : sig1);

        av = sig = 0.0;
        for(i = 0; i < SAMPLE2; ++i)
        {
            t_pro(yh, yl);
            f_dec_blk(dc, pt, pt);
            f_dec_blk(dc, pt, pt);
            f_dec_blk(dc, pt, pt);
            f_dec_blk(dc, pt, pt);
            f_dec_blk(dc, pt, pt);
            f_dec_blk(dc, pt, pt);
            f_dec_blk(dc, pt, pt);
            f_dec_blk(dc, pt, pt);
            f_dec_blk(dc, pt, pt);
            f_dec_blk(dc, pt, pt);
            f_dec_blk(dc, pt, pt);
            f_dec_blk(dc, pt, pt);
            f_dec_blk(dc, pt, pt);
            f_dec_blk(dc, pt, pt);
            f_dec_blk(dc, pt, pt);
            f_dec_blk(dc, pt, pt);
            t_epif(yh, yl, cy);

            if(cy > av1 - sig1 && cy < av1 + sig1)
            {
                av += cy;
                sig += cy * cy;
                sam_cnt++;
            }
        }

        if(10 * sam_cnt > 9 * SAMPLE2)
        {
            av /= sam_cnt;
            sig = sqrt((sig - av * av * sam_cnt) / sam_cnt);
            if(sig > (tol / 100.0) * av)
                sam_cnt = 0;
        }
        else
        {
            if(lcnt++ == 10)
            {
                lcnt = 0; tol += 5;
                if(tol > 30)
                    return false;
            }
            sam_cnt = 0;
        }
    }

    return true;
}

// measure cycles for an encryption call

word e_cycles(const word klen, f_ectx alg[1])
{   byte  pt[16], ct[16], key[32];
    unsigned int yl, yh;
    unsigned long long cy1, cy2, c1 = -1, c2 = -1;

    // set up a random key of 256 bits

    block_rndfill(key, 32);

    // set up a random plain text

    block_rndfill(pt, 16);

    // do a set_key in case it is necessary

    f_enc_key(alg, key, klen); c1 = c2 = 0xffffffff;

    // do an encrypt to remove any 'first time through' effects

    f_enc_blk(alg, pt, ct);

    for(int i = 0; i < loops; ++i)
    {
        block_rndfill(pt, 16);

        // time 1 and 9 encryptions - subtract to get cost of 8
        t_pro(yh, yl);
        f_enc_blk(alg, ct, ct);
        t_epi(yh, yl, cy1);

        t_pro(yh, yl);
        f_enc_blk(alg, ct, ct);
        f_enc_blk(alg, ct, ct);
        f_enc_blk(alg, ct, ct);
        f_enc_blk(alg, ct, ct);
        f_enc_blk(alg, ct, ct);
        f_enc_blk(alg, ct, ct);
        f_enc_blk(alg, ct, ct);
        f_enc_blk(alg, ct, ct);
        f_enc_blk(alg, ct, ct);
        t_epi(yh, yl, cy2);

        if(i > loops / 10)
        {
            c1 = (c1 < cy1 ? c1 : cy1); // find minimum values over the loops
            c2 = (c2 < cy2 ? c2 : cy2);
        }
    }

    return static_cast<word>(((c2 - c1) + 4) >> 3);
}

// measure cycles for a decryption call

word d_cycles(const word klen, f_dctx alg[1])
{   byte  pt[16], ct[16], key[32];
    unsigned int yl, yh;
    unsigned long long cy1, cy2, c1 = -1, c2 = -1;

    // set up a random key of 256 bits

    block_rndfill(key, 32);

    // set up a random plain text

    block_rndfill(pt, 16);

    // do a set_key in case it is necessary

    f_dec_key(alg, key, klen); c1 = c2 = 0xffffffff;

    // do an decrypt to remove any 'first time through' effects

    f_dec_blk(alg, pt, ct);

    for(int i = 0; i < loops; ++i)
    {
        block_rndfill(pt, 16);

        // time 1 and 9 decryptions - subtract to get cost of 8
        t_pro(yh, yl);
        f_dec_blk(alg, ct, ct);
        t_epi(yh, yl, cy1);

        t_pro(yh, yl);
        f_dec_blk(alg, ct, ct);
        f_dec_blk(alg, ct, ct);
        f_dec_blk(alg, ct, ct);
        f_dec_blk(alg, ct, ct);
        f_dec_blk(alg, ct, ct);
        f_dec_blk(alg, ct, ct);
        f_dec_blk(alg, ct, ct);
        f_dec_blk(alg, ct, ct);
        f_dec_blk(alg, ct, ct);
        t_epi(yh, yl, cy2);

        if(i > loops / 10)
        {
            c1 = (c1 < cy1 ? c1 : cy1); // find minimum values over the loops
            c2 = (c2 < cy2 ? c2 : cy2);
        }
    }

    return static_cast<word>(((c2 - c1) + 4) >> 3);
}

// measure cycles for an encryption key setup

word ke_cycles(const word klen, f_ectx alg[1])
{   byte  key[32];
    unsigned int yl, yh;
    unsigned long long cy1, cy2, c1 = -1, c2 = -1;

    // set up a random key of 256 bits

    block_rndfill(key, 32);

    // do an set_key to remove any 'first time through' effects

    f_enc_key(alg, key, klen); c1 = c2 = 0xffffffff;

    switch(klen)
    {
    case 16:
        for(int i = 0; i < loops; ++i)
        {
            // time 1 and 5 encryption key schedules - subtract to get cost of 4
            t_pro(yh, yl);
            f_enc_key128(alg, key + 10 * AES_BLOCK_SIZE);
            t_epi(yh, yl, cy1);

            t_pro(yh, yl);
            f_enc_key128(alg, key + 10 * AES_BLOCK_SIZE);
            f_enc_key128(alg, key + 10 * AES_BLOCK_SIZE);
            f_enc_key128(alg, key + 10 * AES_BLOCK_SIZE);
            f_enc_key128(alg, key + 10 * AES_BLOCK_SIZE);
            f_enc_key128(alg, key + 10 * AES_BLOCK_SIZE);
            t_epi(yh, yl, cy2);

            if(i > loops / 10)
            {
                c1 = (c1 < cy1 ? c1 : cy1); // find minimum values over the loops
                c2 = (c2 < cy2 ? c2 : cy2);
            }
        }
        break;
    case 24:
        for(int i = 0; i < loops; ++i)
        {
            // time 1 and 5 encryption key schedules - subtract to get cost of 4
            t_pro(yh, yl);
            f_enc_key192(alg, key + 23 * (AES_BLOCK_SIZE / 2));
            t_epi(yh, yl, cy1);

            t_pro(yh, yl);
            f_enc_key192(alg, key + 23 * (AES_BLOCK_SIZE / 2));
            f_enc_key192(alg, key + 23 * (AES_BLOCK_SIZE / 2));
            f_enc_key192(alg, key + 23 * (AES_BLOCK_SIZE / 2));
            f_enc_key192(alg, key + 23 * (AES_BLOCK_SIZE / 2));
            f_enc_key192(alg, key + 23 * (AES_BLOCK_SIZE / 2));
            t_epi(yh, yl, cy2);

            if(i > loops / 10)
            {
                c1 = (c1 < cy1 ? c1 : cy1); // find minimum values over the loops
                c2 = (c2 < cy2 ? c2 : cy2);
            }
        }
        break;
    case 32:
        for(int i = 0; i < loops; ++i)
        {
            // time 1 and 5 encryption key schedules - subtract to get cost of 4
            t_pro(yh, yl);
            f_enc_key256(alg, key + 13 * AES_BLOCK_SIZE);
            t_epi(yh, yl, cy1);

            t_pro(yh, yl);
            f_enc_key256(alg, key + 13 * AES_BLOCK_SIZE);
            f_enc_key256(alg, key + 13 * AES_BLOCK_SIZE);
            f_enc_key256(alg, key + 13 * AES_BLOCK_SIZE);
            f_enc_key256(alg, key + 13 * AES_BLOCK_SIZE);
            f_enc_key256(alg, key + 13 * AES_BLOCK_SIZE);
            t_epi(yh, yl, cy2);

            if(i > loops / 10)
            {
                c1 = (c1 < cy1 ? c1 : cy1); // find minimum values over the loops
                c2 = (c2 < cy2 ? c2 : cy2);
            }
        }
        break;
    }

    return static_cast<word>(((c2 - c1) + 2) >> 2);
}

// measure cycles for an encryption key setup

word kd_cycles(const word klen, f_dctx alg[1])
{   byte  key[32];
    unsigned int yl, yh;
    unsigned long long cy1, cy2, c1 = -1, c2 = -1;

    // set up a random key of 256 bits

    block_rndfill(key, 32);

    // do an set_key to remove any 'first time through' effects

    f_dec_key(alg, key, klen); c1 = c2 = 0xffffffff;

    switch(klen)
    {
    case 16:
        for(int i = 0; i < loops; ++i)
        {
            // time 1 and 5 decryption key schedules - subtract to get cost of 4
            t_pro(yh, yl);
            f_dec_key128(alg, key + 10 * AES_BLOCK_SIZE);
            t_epi(yh, yl, cy1);

            t_pro(yh, yl);
            f_dec_key128(alg, key + 10 * AES_BLOCK_SIZE);
            f_dec_key128(alg, key + 10 * AES_BLOCK_SIZE);
            f_dec_key128(alg, key + 10 * AES_BLOCK_SIZE);
            f_dec_key128(alg, key + 10 * AES_BLOCK_SIZE);
            f_dec_key128(alg, key + 10 * AES_BLOCK_SIZE);
            t_epi(yh, yl, cy2);

            if(i > loops / 10)
            {
                c1 = (c1 < cy1 ? c1 : cy1); // find minimum values over the loops
                c2 = (c2 < cy2 ? c2 : cy2);
            }
        }
        break;
    case 24:
        for(int i = 0; i < loops; ++i)
        {
            // time 1 and 5 decryption key schedules - subtract to get cost of 4
            t_pro(yh, yl);
            f_dec_key192(alg, key + 23 * (AES_BLOCK_SIZE / 2));
            t_epi(yh, yl, cy1);

            t_pro(yh, yl);
            f_dec_key192(alg, key + 23 * (AES_BLOCK_SIZE / 2));
            f_dec_key192(alg, key + 23 * (AES_BLOCK_SIZE / 2));
            f_dec_key192(alg, key + 23 * (AES_BLOCK_SIZE / 2));
            f_dec_key192(alg, key + 23 * (AES_BLOCK_SIZE / 2));
            f_dec_key192(alg, key + 23 * (AES_BLOCK_SIZE / 2));
            t_epi(yh, yl, cy2);

            if(i > loops / 10)
            {
                c1 = (c1 < cy1 ? c1 : cy1); // find minimum values over the loops
                c2 = (c2 < cy2 ? c2 : cy2);
            }
        }
        break;
    case 32:
        for(int i = 0; i < loops; ++i)
        {
            // time 1 and 5 decryption key schedules - subtract to get cost of 4
            t_pro(yh, yl);
            f_dec_key256(alg, key + 13 * AES_BLOCK_SIZE);
            t_epi(yh, yl, cy1);

            t_pro(yh, yl);
            f_dec_key256(alg, key + 13 * AES_BLOCK_SIZE);
            f_dec_key256(alg, key + 13 * AES_BLOCK_SIZE);
            f_dec_key256(alg, key + 13 * AES_BLOCK_SIZE);
            f_dec_key256(alg, key + 13 * AES_BLOCK_SIZE);
            f_dec_key256(alg, key + 13 * AES_BLOCK_SIZE);
            t_epi(yh, yl, cy2);

            if(i > loops / 10)
            {
                c1 = (c1 < cy1 ? c1 : cy1); // find minimum values over the loops
                c2 = (c2 < cy2 ? c2 : cy2);
            }
        }
        break;
    }

    return static_cast<word>(((c2 - c1) + 2) >> 2);
}

static word kl[3] = { 16, 24, 32 };
static word ekt[3], dkt[3], et[3], dt[3];

void output(std::ofstream& outf, const word inx, const word bits)
{   word  t;
    byte  c0, c1, c2;

    outf << "\n// " << 8 * kl[inx] << " Bit:";
    outf << "   Key Setup: " << ekt[inx] << '/' << dkt[inx] << " cycles";
    t = (1000 * bits + et[inx] / 2) / et[inx];
    c0 = (byte)('0' + t / 100); c1 = (byte)('0' + (t / 10) % 10); c2 = (byte)('0' + t % 10);
    outf << "\n// Encrypt:   " << et[inx] << " cycles = 0."
         << c0 << c1 << c2 << " bits/cycle";
    t = (1000 * bits + dt[inx] / 2) / dt[inx];
    c0 = (byte)('0' + t / 100); c1 = (byte)('0' + (t / 10) % 10); c2 = (byte)('0' + t % 10);
    outf << "\n// Decrypt:   " << dt[inx] << " cycles = 0."
         << c0 << c1 << c2 << " bits/cycle";
}

#if defined( AES_DLL )

#include "windows.h"

HINSTANCE init_dll(fn_ptrs& fn)
{   HINSTANCE   h_dll;
    bool        ok = true;

    if(!(h_dll = LoadLibrary(dll_path)))
    {
        std::cout << "\n\nDynamic link Library AES_DLL not found\n\n"; return 0;
    }

    fn.fn_enc_key128 = (g_enc_key*)GetProcAddress(h_dll, ek_name128);
    fn.fn_enc_key192 = (g_enc_key*)GetProcAddress(h_dll, ek_name192);
    fn.fn_enc_key256 = (g_enc_key*)GetProcAddress(h_dll, ek_name256);
    fn.fn_enc_key = (g_enc_keyv*)GetProcAddress(h_dll, ek_name);
    fn.fn_enc_blk = (g_enc_blk*)GetProcAddress(h_dll, ec_name);
    fn.fn_dec_key128 = (g_dec_key*)GetProcAddress(h_dll, dk_name128);
    fn.fn_dec_key192 = (g_dec_key*)GetProcAddress(h_dll, dk_name192);
    fn.fn_dec_key256 = (g_dec_key*)GetProcAddress(h_dll, dk_name256);
    fn.fn_dec_key = (g_dec_keyv*)GetProcAddress(h_dll, dk_name);
    fn.fn_dec_blk = (g_dec_blk*)GetProcAddress(h_dll, dc_name);

#if defined( AES_128 )
    if(!fn.fn_enc_key128 || !fn.fn_dec_key128)
        ok = false;
#endif

#if defined( AES_192 )
    if(!fn.fn_enc_key192 || !fn.fn_dec_key192)
        ok = false;
#endif

#if defined( AES_256 )
    if(!fn.fn_enc_key128 || !fn.fn_dec_key128)
        ok = false;
#endif

#if defined( AES_VAR )
    if(!fn.fn_enc_key || !fn.fn_dec_key)
        ok = false;
#endif

    if(!fn.fn_enc_blk || !fn.fn_dec_blk)
        ok = false;

    if(!ok)
    {
        std::cout << "\n\nRequired DLL Entry Point(s) not found\n\n";
        FreeLibrary(h_dll);
        return 0;
    }

    return h_dll;
}

#endif  // AES_DLL

int main(int argc, char *argv[])
{   std::ofstream   outf;
    f_ectx   alge[1];
    f_dctx   algd[1];
    double   a0, av, sig;

    gen_tabs();
#if defined(AES_DLL)
    HINSTANCE   h_dll;

    if(!(h_dll = init_dll(fn))) return -1;
#endif

    memset(&alge, 0, sizeof(aes_encrypt_ctx));
    memset(&algd, 0, sizeof(aes_decrypt_ctx));

    outf.open(argc == 2 ? argv[1] : "CON", std::ios_base::out);

    outf << "\n// AES"
#if defined(AES_DLL)
        " (DLL)"
#elif defined(AES_CPP)
        " (CPP)"
#endif
    " Timing For The " << PROCESSOR << " Processor";

    for(word ki = 0; ki < 3; ++ki)
    {
        ekt[ki] = ke_cycles(kl[ki], alge);
        dkt[ki] = kd_cycles(kl[ki], algd);
        et[ki]  =  e_cycles(kl[ki], alge);
        dt[ki]  =  d_cycles(kl[ki], algd);
    }

    outf << "\n// KeySize     EKS(cycles)  ENC(cycles/byte)    DKS(cycles)  DEC(cycles/byte)";

    for(int i = 0; i < 3; ++i)
    {   int w, f;

        outf << "\n// min(" << std::setw(3) << 128 + 64 * i << ") ";
        outf << std::setw(14) << ekt[i];
        av = et[i] / 16.0; w = (int)av; f = (int)(10 * (av - w) + 0.5);
        outf << std::setw(16) << w << '.' << std::setw(1) << f;
        outf << std::setw(15) << dkt[i];
        av = dt[i] / 16.0; w = (int)av; f = (int)(10 * (av - w) + 0.5);
        outf << std::setw(16) << w << '.' << std::setw(1) << f;
    }

    time_base(a0, sig);
    outf.setf(std::ios::fixed);
    for(int i = 128; i <= 256; i += 64)
    {
        outf << "\n// avg(" << std::setw(3) << i << ") ";

        time_eks8(i, av, sig);
        sig *= 100.0 / av;
        av = ((av - a0 + 4.0) / 8.0);
        outf.precision(0);
        outf << std::setw(7) << std::noshowpoint << av;
        outf.precision(1);
        outf << "(" << std::setw(4) << std::showpoint << sig << "%)";

        time_enc16(i, av, sig);
        sig *= 100.0 / av;
        av = (int)(10.0 * (av - a0) / 256.0) / 10.0;
        sig = (int)(10 * sig) / 10.0;
        outf << std::setw(11) << std::noshowpoint << av;
        outf << "(" << std::setw(4) << std::showpoint << sig << "%)";

        time_dks8(i, av, sig);
        sig *= 100.0 / av;
        av = (int)((av - a0 + 4.0) / 8.0);
        outf.precision(0);
        outf << std::setw(8) << std::noshowpoint << av;
        outf.precision(1);
        outf << "(" << std::setw(4) << std::showpoint << sig << "%)";

        outf.precision(1);
        time_dec16(i, av, sig);
        sig *= 100.0 / av;
        av = (int)(10.0 * (av - a0) / 256.0) / 10.0;
        sig = (int)(10 * sig) / 10.0;
        outf << std::setw(11) << std::noshowpoint << av;
        outf << "(" << std::setw(4) << std::showpoint << sig << "%)";
    }

#if defined(AES_DLL)
    if(h_dll) FreeLibrary(h_dll);
#endif

    outf << "\n\n";
    return 0;
}
