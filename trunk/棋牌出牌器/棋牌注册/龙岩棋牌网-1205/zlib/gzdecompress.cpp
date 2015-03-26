#include "stdafx.h"
#include "gzdecompress.h"
#include "GZipHelper.h"


//int gzuncompress(char* filename,char*&buf,int & len)
//输入解压前的内容， 与长度
//输出解压后的内容， 与长度
int gzuncompress( CString & strBuffer, int & nlen)
{
        if ( strBuffer.GetLength() <= 0 )
        {
                return -1;
        }

        CGZIP2A plain( (LPGZIP)(strBuffer.GetBuffer( nlen )), nlen );  // do decompressing here
        nlen = plain.Length;    // Length is length of unzipped data. 
        if( nlen <= 0 )
        {
                return 1;
        }
        if( plain.psz == NULL )
        {
                strBuffer.Empty() ;
        }
        else
        {
                strBuffer = CString( plain.psz, nlen );    // psz is plain data pointer
        }

	return 0;
}



