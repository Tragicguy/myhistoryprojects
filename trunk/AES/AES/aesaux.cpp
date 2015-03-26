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

#include "aesaux.h"

char *st[8] = { "vk", "vt", "nk", "nt", "me", "md", "me", "md" };

char *file_name(char* buf, const word type, const word blen, const word klen)
{
    std::strstream  sout(buf, 128);

    sout << (type < 6 ? "ecb" : "cbc") << st[type]
         << static_cast<char>('0' + blen / 4)
         << static_cast<char>('0' + klen / 4) << ".txt";

    buf[sout.pcount()] = '\0';

    return buf;
}

const char *pos(const char *s)
{
    while(*s++ != '=') ;
    while(*s++ == ' ') ;
    return s - 1;
}

// Strings to locate lines in test vector files

const char    *fstr[NO_LTYPES] =
        { BADL_STR, BLEN_STR, KLEN_STR, TEST_STR, IV_STR, KEY_STR, PT_STR, CT_STR };

char    *hxx = "0123456789abcdef";

// Hexadecimal conversion from hex character to a number in range 0 <= no <= 15

inline int to_hex(int ch)
{
    return (ch & 15) + (ch >= '0' && ch <= '9' ? 0 : 9);
}

bool get_line(std::ifstream& inf, char s[])
{
    if(inf.eof()) return false;

    inf.getline(s, 100);

    return true;
}

char *copy_str(char *s, const char *fstr)
{   const char *p = fstr;

    while(*p)

        *s++ = *p++;

    *s = '\0'; return s;
}

// Read from a string of hexadecimal digits into an array of 8-bit bytes
// Each pair of hexadecimal digits is used to compile one 8-bit byte and
// the first (last) digit forms the most (least) significant four bits
// of each byte. The hexadecimal digits are read from left (low) to right
// (high) and are placed in increasing index positions in the byte array

int block_in(byte l[], const char *p)
{   int i = 0;

    p = pos(p);

    while(*p && *(p + 1) && isxdigit(*p) && isxdigit(*(p + 1)))
    {
        l[i++] = (to_hex(*p) << 4) + to_hex(*(p + 1)); p += 2;
    }

    return i;
}

// clears an array of 8-bit bytes

void block_clear(byte l[], const word len)
{   byte *lp = l;

    while(lp < l + len)

        *lp++ = 0;
}

// reverses the order of an array of 8-bit bytes

void block_reverse(byte l[], const word len)
{    byte  *blo = l, *bhi = l + len - 1, by;

    while(blo < bhi)

        by = *blo, *blo++ = *bhi, *bhi-- = by;
}

// copies an array of 8-bit bytes (non overlapping)

void block_copy(byte l[], const byte r[], const word len)
{   byte          *lp = l;
    const byte    *rp = r;

    while(lp < l + len)

        *lp++ = *rp++;
}

// xors one array of 8-bit bytes with another

void block_xor(byte l[], const byte r[], const word len)
{   byte          *lp = l;
    const byte    *rp = r;

    while(lp < l + len)

        *lp++ ^= *rp++;
}

// compares two arrays of 8-bit bytes and return true if the same or
// false otherwise

bool block_cmp(const byte l[], const byte r[], const word len)
{   const byte    *lp = l, *rp = r;

    while(lp < l + len)

        if(*lp++ != *rp++)

            return false;

    return true;
}

word rand32(void)
{   static word   r4,r_cnt = (word)-1, w = 521288629, z = 362436069;

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

// Convert decimal number to a character string

void put_dec(char *s, word val)
{   char    *p = s, *q = s, ch;

    do
    {
        *p++ = (char)('0' + (val % 10)); val /= 10;
    }
    while
        (val);

    *p-- = '\0';

    while(p > q)
    {
        ch = *p, *p-- = *q, *q++ = ch;
    }
}

// Extract decimal number from a character string

word get_dec(const char *s)
{   const char  *p = pos(s);
    word        nbr = 0;

    while(*p && *p >= '0' && *p <= '9')
    {
        nbr = 10 * nbr + (*p - '0'); ++p;
    }

    return nbr;
}

// Compare two strings ignoring case

int cmp_nocase(const char *s1, const char *s2)
{   const char  *p1 = pos(s1), *p2 = pos(s2);

    while(*p1 && *p2)
    {
        if(toupper(*p1) != toupper(*p2))
        {
            return (toupper(*p1) < toupper(*p2) ? -1 : 1);
        }

        ++p1; ++p2;
    }

    while(*p1) p1++; while(*p2) p2++;

    return (p2 - s2) - (p1 - s1);
}

// Input command line arguments

bool test_args(int argc, char *argv[], char des_chr, char tst_chr)
{
    for(int i = 1; i < argc; ++i)
    {
        if(argv[i][0] != '-' && argv[i][0] != '/')

            continue;

        if(argv[i][1] != tolower(des_chr) && argv[i][1] != toupper(des_chr))

            continue;

        else if(!tst_chr)

            return true;

        if(argv[i][2] != ':')

            continue;

        for(word j = 3; argv[i][j]; ++j)

            if(argv[i][j] == tolower(tst_chr) || argv[i][j] == toupper(tst_chr))

                return true;
    }

    return false;
}

// Find a given string s2 in s1

int find_string(const char *s1, const char s2[])
{   const char  *p1 = s1, *q1, *q2;

    while(*p1)
    {
        q1 = p1; q2 = s2;

        while(*q1 && *q2 && *q1 == *q2)
        {
            q1++; q2++;
        }

        if(!*q2)

            return p1 - s1;

        p1++;
    }

    return -1;
}

// Find a line with a given header and return line type
// or -1 if end of file

line_type find_line(std::ifstream& inf, char str[])
{
    while(get_line(inf, str))               // input a line
    {
        for(word i = 1; i < NO_LTYPES; ++i)  // compare line header
        {
            if(find_string(str, fstr[i]) >= 0)
            {
                return static_cast<line_type>(i);
            }
        }
    }

    return bad_line;            // end of file
}

// Output an array of bytes to a string of hexadecimal digits using
// the conventions described for input above.  But if type (ty) = 1
// it outputs a test number in decimal notation

void block_out(const line_type ty, const byte b[], std::ofstream& outf, const word len)
{
    outf << '\n' << fstr[ty];

    if(ty <= test_no)
    {
        outf << len;
    }
    else

        for(word i = 0; i < len; ++i)
        {
            outf << hxx[(b[i] >> 4) & 15] << hxx[b[i] & 15];
        }
}
