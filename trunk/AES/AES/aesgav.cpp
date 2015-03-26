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

// AES Algorithm Test - Generate local test files for KAT and Monte Carlo
// tests and optionally compare these files with a  reference set of test
// files
//
// usage: aes_gav /t:[knec] /b:[45678] /k:[45678] [/c]
//
// where:
//
// each of the symbols in square brakets can be used in any combination so
// that:
//
//         /t:[knec]  selects the tests to be used
//         /b:[45678] selects the block lengths to be used (in 32-bit words)
//         /k:[45678] selects the key lengths to be used (in 32-bit words)
//         /c         selects the comparison of output and reference file(s)
// and:
//         k: generate ECB Known Answer Test files
//         n: generate ECB Known Answer Test files (new)
//         e: generate ECB Monte Carlo Test files
//         c: generate CBC Monte Carlo Test files
//
// The reference files have a ".txt" extension while those generated use
// ".txt" if in a different directory or ".dat" otherwise. The directory
// paths for files are set below. Note that, when compared with the NIST
// test vector sequences, this code implements one additional (all zero)
// test vector as the first vector in each set (test 0).

#if defined( AES_CPP )
#include "aescpp.h"
#else
#include "aes.h"
#endif
#include "aesaux.h"
#include "aestst.h"

using   std::cout;
using   std::ofstream;
using   std::ifstream;
using   std::ios;

#if defined(AES_DLL)
fn_ptrs fn;
#endif

// Outputs a test vector file header

void header(ofstream& outf, const int type, const word blen, const word klen)
{   char    buf[32];
    byte    dummy;

    outf << "==============================================";
    outf << "\nAuthor:    Dr B R Gladman (brg@gladman.me.uk)";
    outf << "\nTest:      " << (type < 6 ? "ECB " : "CBC ");
    switch(type)
    {
        case  0:    outf << "Variable Key Known Answer Tests"; break;
        case  2:    outf << "Variable Key Known Answer Tests"; break;
        case  1:    outf << "Variable Text Known Answer Tests"; break;
        case  3:    outf << "Variable Text Known Answer Tests"; break;
        case  4:
        case  6:    outf << "Monte Carlo (Encryption) Tests"; break;
        case  5:
        case  7:    outf << "Monte Carlo (Decryption) Tests"; break;
    }

    outf << "\nAlgorithm: Extended Rijndael (an AES Superset)\nFilename:  "
         << file_name(buf, type, blen, klen)
         << "\n==============================================\n";

    block_out(block_len, &dummy, outf, 8 * blen);
    block_out(key_len, &dummy, outf, 8 * klen);
    outf << '\n';
}

// Test of Electronic Code Book (ECB) mode with Fixed Key and Variable Text

void ecb_vt(ofstream& outf, f_ectx alg[1], const word blen, const word klen)
{   word      j;
    byte      pt[32], ct[32], key[32], dummy;

    block_clear(key, klen);                         // all zero key
    block_out(key_val, key, outf, klen);            // output key value
    f_enc_key(alg, key, klen);                      // set key value

    for(j = 0; j <= 8 * blen; ++j)                  // test vectors include
    {                                               // an all zero one
        block_out(test_no, &dummy, outf, j);        // output test number
        block_clear(pt, blen);                      // set all zero plain text
        if(j)                                       // set bit (j-1) if j <> 0
            *(pt + (j - 1) / 8) = 0x80 >> (j - 1) % 8;
        block_out(pt_val, pt, outf, blen);          // output plaintext
        f_enc_blk(alg, pt , ct);                    // do encryption
        block_out(ct_val, ct, outf, blen);          // output ciphertext
    }
}

// Test of Electronic Code Book (ECB) mode with Fixed Text and Variable Key

void ecb_vk(ofstream& outf, f_ectx alg[1], const word blen, const word klen)
{   word      j;
    byte      pt[32], ct[32], key[32], dummy;

    block_clear(pt, blen);                          // all zero plaintext
    block_out(pt_val, pt, outf, blen);              // output plaintext

    for(j = 0; j <= 8 * klen; ++j)                  // 129, 193 or 257 tests
    {
        block_out(test_no, &dummy, outf, j);        // output test number
        block_clear(key, klen);                     // set all zero key
        if(j)                                       // set bit (j-1) if j <> 0
            *(key + (j - 1) / 8) = 0x80 >> (j - 1) % 8;
        block_out(key_val, key, outf, klen);        // output key value
        f_enc_key(alg, key, klen);                  // set key value
        f_enc_blk(alg, pt , ct);                    // alg.encrypt
        block_out(ct_val, ct, outf, blen);          // output ciphertext
    }
}

// Test of Electronic Code Book (ECB) mode with Fixed Key and Variable Text

void ecb_vtn(ofstream& outf, f_ectx alg[1], const word blen, const word klen)
{   word      j;
    byte      pt[32], ct[32], key[32], *bp, dummy;

    block_clear(key, klen);                         // all zero key
    block_out(key_val, key, outf, klen);            // output key value
    f_enc_key(alg, key, klen);                      // set key value
    block_clear(pt, blen);                          // set all zero plain text

    for(j = 0; j < 16 * blen; ++j)
    {
        block_out(test_no, &dummy, outf, j);        // output test number
        block_out(pt_val, pt, outf, blen);          // output plaintext
        f_enc_blk(alg, pt , ct);                    // do encryption
        block_out(ct_val, ct, outf, blen);          // output ciphertext
        bp = pt + blen - 1 - j / 8;
        if(j < 8 * blen)
            *bp |= (*bp << 1) | 1;
        else
            *(bp + blen) = *(bp + blen) << 1;
    }
}

// Test of Electronic Code Book (ECB) mode with Fixed Text and Variable Key

void ecb_vkn(ofstream& outf, f_ectx alg[1], const word blen, const word klen)
{   word      j;
    byte      pt[32], ct[32], key[32], *bp, dummy;

    block_clear(pt, blen);                          // all zero plaintext
    block_out(pt_val, pt, outf, blen);              // output plaintext
    block_clear(key, klen);

    for(j = 0; j < 16 * klen; ++j)
    {
        block_out(test_no, &dummy, outf, j);        // output test number
        block_out(key_val, key, outf, klen);        // output key value
        f_enc_key(alg, key, klen);                  // set key value
        f_enc_blk(alg, pt , ct);                    // alg.encrypt
        block_out(ct_val, ct, outf, blen);          // output ciphertext
        bp = key + klen - 1 - j / 8;
        if(j < 8 * klen)
            *bp |= (*bp << 1) | 1;
        else
            *(bp + klen) = *(bp + klen) << 1;
    }
}

// Monte Carlo Encryption Test of Electronic Code Book (ECB) mode

void ecb_me(ofstream& outf, f_ectx alg[1], const word blen, const word klen)
{   word  j, k;
    byte  pt[32], ct[64], key[32], dummy;

    block_clear(pt, blen);                          // clear initial plaintext
    block_clear(key, klen);                         // and key blocks
    block_copy(ct + blen, pt, blen);                // put plaintext in upper half
                                                    // of double length buffer
    for(j = 0; j < 400; j++)                        // 400 Monte Carlo tests
    {
        block_out(test_no, &dummy, outf, j);        // output test number
        block_out(key_val, key, outf, klen);        // output key
        block_out(pt_val, pt, outf, blen);          // output plaintext
        f_enc_key(alg, key, klen);                  // set the key

        for(k = 0; k < 5000; ++k)                   // 10000 encryptions alternating
        {                                           // upper and lower blocks in ct
            f_enc_blk(alg, ct + blen, ct);
            f_enc_blk(alg, ct, ct + blen);
        }

        // compile next key as defined by NIST

        block_xor(key, ct + 2 * blen - klen, klen);
        block_out(ct_val, ct + blen, outf, blen);   // output ciphertext
        block_copy(pt, ct + blen, blen);            // copy cipertext as next plaintext
    }
}

// Monte Carlo Decryption Test of Electronic Code Book (ECB) mode

void ecb_md(ofstream& outf, f_dctx alg[1], const word blen, const word klen)
{   word  j, k;
    byte  pt[32], ct[64], key[32], dummy;

    block_clear(pt, blen);                          // clear initial plaintext and key
    block_clear(key, klen);
    block_copy(ct + blen, pt, blen);                // copy plaintext into upper half
                                                    // of double length ciphertext block
    for(j = 0; j < 400; j++)                        // 400 Monte Carlo tests
    {
        block_out(test_no, &dummy, outf, j);        // output test number
        block_out(key_val, key, outf, klen);        // output key
        block_out(ct_val, pt, outf, blen);          // output plaintext
        f_dec_key(alg, key, klen);                  // set key

        for(k = 0; k < 5000; ++k)                   // 10000 decryptions alternating
        {                                           // upper and lower blocks in ct
            f_dec_blk(alg, ct + blen, ct);
            f_dec_blk(alg, ct, ct + blen);
        }

        // compile next key as defined by NIST

        block_xor(key, ct + 2 * blen - klen, klen);
        block_out(pt_val, ct + blen, outf, blen);   // output ciphertext
        block_copy(pt, ct + blen, blen);            // set ciphertext as next plaintext
    }
}

// Monte Carlo Encryption Test of Cipher Block Chaining (CBC) mode

void cbc_me(ofstream& outf, f_ectx alg[1], const word blen, const word klen)
{   word  j, k;
    byte  ct[64], key[32], dummy;

    block_clear(key, klen);                         // clear key: KEY[0]
    block_clear(ct, 2 * blen);                      // clear ct:  PT[0], ct + blen: IV[0]

    for(j = 0; j < 400; j++)                        // 400 Monte Carlo tests
    {
        block_out(test_no, &dummy, outf, j);        // output test number
        block_out(key_val, key, outf, klen);        // output key
        block_out(iv_val, ct + blen, outf, blen);   // output initialisation vector
        block_out(pt_val, ct, outf, blen);          // output plaintext
        f_enc_key(alg, key, klen);                  // set key

        for(k = 0; k < 5000; ++k)                   // 10000 encryptions, two at a time
        {
            block_xor(ct, ct + blen, blen);         // do CBC chaining
            f_enc_blk(alg, ct, ct);                 // do block encryption
            block_xor(ct + blen, ct, blen);         // do CBC chaining
            f_enc_blk(alg, ct + blen, ct + blen);   // do block encryption

        }

        block_out(ct_val, ct + blen, outf, blen);   // output ciphertext
        // compile next key as defined by NIST
        block_xor(key, ct + 2 * blen - klen, klen);
    }
}

// Monte Carlo Encryption Test of Cipher Block Chaining (CBC) mode

void cbc_md(ofstream& outf, f_dctx alg[1], const word blen, const word klen)
{   word  j, k;
    byte  pt[32], ct[64], key[32], dummy;

    block_clear(key, klen);                         // clear key: KEY[0]
    block_clear(ct, 2 * blen);                      // clear ct: IV[0] ct + blen: CT[0]

    for(j = 0; j < 400; j++)                        // 400 Monte Carlo tests
    {
        block_out(test_no, &dummy, outf, j);        // output test number
        block_out(key_val, key, outf, klen);        // output key
        block_out(iv_val, ct, outf, blen);          // output initialisation vector
        block_out(ct_val, ct + blen, outf, blen);   // output ciphertext
        f_dec_key(alg, key, klen);                  // set key

        for(k = 0; k < 5000; ++k)                   // 10000 encryptions, two at a time
        {
            f_dec_blk(alg, ct + blen, pt);          // do block decryption

            block_xor(ct, pt, blen);                // do CBC chaining

            f_dec_blk(alg, ct, pt);                 // do block decryption

            block_xor(ct + blen, pt, blen);         // do CBC chaining
        }

        block_out(pt_val, ct + blen, outf, blen);   // output plaintext
        // compile next key as defined by NIST
        block_xor(key, ct + 2 * blen - klen, klen);
    }
}

// Synchronise two comparison files if they get out of step

int sync(int nbr, ifstream& inf, char str[], bool outp)
{   line_type   ty;
    int         nn;

    for(;;)
    {
        ty = find_line(inf, str);

        if(ty == bad_line) return -1;

        if(ty == test_no)
        {
            nn = get_dec(str + 2);

            if(nn >= nbr) return nn;
        }

        if(outp) cout << "\n  " << str;
    }
}

// Compare two test vector files

void comp_vecs(const char *fn1, const char *fn2)
{   char       str1[128], str2[128];
    line_type  ty1, ty2;
    int        no1, no2, err_cnt, np_cnt;
    bool       req;
    ifstream   if1;
    ifstream   if2;

    err_cnt = np_cnt = 0; req = true;

    if1.open(fn1, ios::in);

    if(!if1)   // open first file
    {
        cout << "\n*** 1st file (" << fn1 << ") not found ***"; return;
    }

    if2.open(fn2, ios::in);

    if(!if2)   // open second file
    {
        cout << "\n*** 2nd file (" << fn2 << ") not found ***"; return;
    }

    for(;;)         // while there is still input
    {
        if(req)     // if another line needs to be input
        {
            ty1 = find_line(if1, str1); ty2 = find_line(if2, str2);
        }

        if(ty1 == bad_line && ty2 == bad_line)      // if end of file on both files

            break;

        if(ty1 == bad_line || ty2 == bad_line)      // if end of file on one file
        {
            cout << '\n' << fn1 << (ty1 == bad_line ? " short" : " long")
                                                << "er than " << fn2;
            break;
        }

        if(ty1 == test_no)          // if 'test number' line in file 1

            no1 = get_dec(str1);

        if(ty2 == test_no)          // if 'test number' line in file 2

            no2 = get_dec(str2);

        if(cmp_nocase(str1, str2) == 0)
        {
            req = true; continue;   // if lines are the same continue
        }

        if(ty1 == test_no && ty2 == test_no)    // if not the same but both are at a
        {                                       // 'test number' line
            np_cnt += abs(no2 - no1); req = false;

            if(no2 < no1)   // extra tests in file 2
            {
                cout << "\nextra test(s) in " << fn2 << ":\n  " << str2;
                no2 = sync(no1, if2, str2, np_cnt < 10); // skip tests in file 2
            }

            if(no1 < no2)   // extra test in file 1
            {
                cout << "\nextra test(s) in " << fn1 << ":\n  " << str1;
                no1 = sync(no2, if1, str1, np_cnt < 10);// skip tests in file 1
            }
        }
        else if(ty1 != ty2) // cannot synchronise test vector files
        {
            cout << "\n*** synchronisation error tests " << no1
                    << " and " << no2 << " ***";

            cout.flush(); return;
        }
        else if(ty1 != bad_line)   // test vector mismatch
        {
            err_cnt++;

            cout << "\r*** mismatch error test " << no1 << " ***";
        }

        cout.flush();
    }

    if(np_cnt && !err_cnt)  // all tests present match

        cout << "\nother tests match\n";

    else
    {
        if(err_cnt)
        {
            cout << '\r' << fn1 << " doesn't match " << fn2
                                            << " (" << err_cnt << "errors)\n";
        }
        else
        {
            cout << '\r' << fn1 << " matches " << fn2 << '\n';
        }
    }

    std::cout.flush();
    if1.close(); if2.close();
}

// array of functions to call for each test

typedef void (*f_ep)(ofstream& outf, f_ectx alg[1], const word blen, const word klen);
typedef void (*f_dp)(ofstream& outf, f_dctx alg[1], const word blen, const word klen);

f_ep f_ptr[8] = { ecb_vk, ecb_vt, ecb_vkn, ecb_vtn, ecb_me, (f_ep)ecb_md, cbc_me, (f_ep)cbc_md };

// do the tests for each algorithm

void do_tests(bool do_cmp, bool ttype[3], f_ectx alg[1], const word blen, const word klen)
{   char       name1[128], name2[128], *sp1, *sp2;
    int        i;
    ofstream   outf;

    cout << "\nGenerate" << (do_cmp ? " and verify" : "")
         << " tests for aes (AES_BLOCK_SIZE = " << 8 * blen
         << ", key size = " << 8 * klen << ")\n";

    for(i = 0; i < 8; ++i)  // for each type of test /k /x /e /c (2 tests each)
        if(ttype[i / 2])    // if this test required
        {
            // name of file for output of generated test vectors

            sp1 = copy_str(name1, out_path);
            sp2 = copy_str(name2, ref_path);
            file_name(sp1, i, blen, klen);
            copy_str(sp2, sp1);

            outf.open(name1, ios::out);

            if(outf)      // if output file is open write it
            {
                header(outf, i, blen, klen);
                f_ptr[i](outf, alg, blen, klen);
                outf << '\n'; outf.close();

                if(do_cmp)  // compare it with reference if required
                    comp_vecs(name2, name1);
            }
       }
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
{   bool    do_cmp, tyf[4], kf[3];
    f_ectx   alg[1];

    gen_tabs();
#if defined(AES_DLL)
    HINSTANCE   h_dll;

    if(!(h_dll = init_dll(fn))) return -1;
#endif

    if(argc == 1)
    {
        cout << "\nusage: aes_gav /t:[knec] /b:[45678] /k:[45678] [/c]";
        cout << "\n";
        cout << "\nwhere the symbols in square brackets can be used in";
        cout << "\nany combination (without the brackets) and have the";
        cout << "\nfollowing meanings:";
        cout << "\n";
        cout << "\n        /t:[knec]   selects which tests are used";
        cout << "\n        /k:[468]    selects the key lengths used";
        cout << "\n        /c          compares output with reference";
        cout << "\nwhere:";
        cout << "\n        k: generate ECB Known Answer Test files";
        cout << "\n        n: generate ECB Known Answer Test files (new)";
        cout << "\n        e: generate ECB Monte Carlo Test files";
        cout << "\n        c: generate CBC Monte Carlo Test files";
        cout << "\n";
        cout << "\nand the characters giving block and key lengths are";
        cout << "\ndigits representing the lengths in 32-bit units.\n\n";
        exit(0);
    }

    cout << "\nRun tests for the AES algorithm"

#if defined(AES_DLL)
    " (DLL Version)\n";
#elif defined(AES_CPP)
    " (CPP Version)\n";
#else
    "\n";
#endif

    do_cmp = test_args(argc, argv, 'c', '\0');

    tyf[0] = test_args(argc, argv, 't', 'k');
    tyf[1] = test_args(argc, argv, 't', 'n');
    tyf[2] = test_args(argc, argv, 't', 'e');
    tyf[3] = test_args(argc, argv, 't', 'c');

    kf[0] = test_args(argc, argv, 'k', '4');
    kf[1] = test_args(argc, argv, 'k', '6');
    kf[2] = test_args(argc, argv, 'k', '8');

    if(!(kf[0] || kf[1] || kf[2]))
    {
        kf[0] = kf[1] = kf[2] = true;   // AES key sizes if not specified
    }

    for(int ki = 0; ki < 3; ++ki) if(kf[ki])
    {
        do_tests(do_cmp, tyf, alg, 16, 16 + 8 * ki);
    }

#if defined(AES_DLL)
    if(h_dll) FreeLibrary(h_dll);
#endif

    cout << "\n\n";
    return 0;
}
