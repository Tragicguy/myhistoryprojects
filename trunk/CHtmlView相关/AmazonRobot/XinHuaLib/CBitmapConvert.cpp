#include "StdAfx.h"
#include "CBitmapConvert.h"
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)
CCBitmapConvert::CCBitmapConvert(void)
{
	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);
}

CCBitmapConvert::~CCBitmapConvert(void)
{
	GdiplusShutdown(m_pGdiToken);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CCBitmapConvert::GetImageCLSID(const WCHAR* format, CLSID* pCLSID)
{
	UINT num = 0;
	UINT size = 0;
	ImageCodecInfo* pImageCodecInfo = NULL;
	GetImageEncodersSize(&num, &size);
	if(size == 0)
	{
		return FALSE;
	}
	pImageCodecInfo = (ImageCodecInfo *)malloc((size));
	if(pImageCodecInfo == NULL)
		return FALSE;
	GetImageEncoders(num, size, pImageCodecInfo);
	// Find for the support of format for image in the windows
	for(UINT i = 0; i < num; ++i)
	{
		if( wcscmp(pImageCodecInfo[i].MimeType, format) == 0)
		{
			*pCLSID = pImageCodecInfo[i].Clsid;
			free(pImageCodecInfo);
			return TRUE;
		}
	}
	free(pImageCodecInfo);
	return FALSE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/

BOOL CCBitmapConvert::TranstToBMP(string srcPath,string destPath)
{
	remove(destPath.c_str());
	WCHAR wCH[255] = {0};
	WCHAR wCH1[255] = {0};
	wstring wstrPath = s2ws(srcPath);
	wcscpy(wCH,wstrPath.c_str());
	Image img(wCH);//这里的图片可以是其它格式	
	CLSID pngClsid;
	GetImageCLSID(L"image/bmp", &pngClsid);//这里的图片可以是其它格式，此处转化为BMP格式
	wstrPath = s2ws(destPath);
	wcscpy(wCH1, wstrPath.c_str());
	img.Save(wCH1, &pngClsid, NULL);
	return TRUE;
}

std::wstring CCBitmapConvert::s2ws(const std::string& s) 
{ 
	int len; 
	int slength = (int)s.length() + 1; 
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);  
	wchar_t* buf = new wchar_t[len]; 
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len); 
	std::wstring r(buf); 
	delete[] buf; 
	return r; 
} 

string CCBitmapConvert::GenBmpPicPath(string strSrcPath)
{
	int iPos = strSrcPath.rfind('.');
	return strSrcPath.append(".bmp");
}

string CCBitmapConvert::GenBinaryPicPath(string strSrcPath)
{
	int iPos = strSrcPath.rfind('.');
	strSrcPath.insert(iPos,"binary");
	return strSrcPath;
}


/***************************************************************************
*   函数名称   
*   OSTUThreshold()   
*   参数   
*         LPSTR   lpDIBBits           -   指向源DIB图像指针   
*         LONG     lWidth                 -   源图像宽度(像素数)   
*         LONG     lHeight               -   源图像高度(像素数)   
*   返回值   
*         BOOL                                 -   运算成功   返回TRUE   ,   否则   返回FALSE。   
*   说明   
*         该函数采用大津法进行阈值分割   二值化   
***************************************************************************/   

int CCBitmapConvert::OSTUThreshold(LPBYTE   lpDIBBits,   LONG   lWidth,   LONG   lHeight)   
{   
	//   指向源图像像素指针   
	LPSTR   lpSrc;   

	//   指向缓存图像指针   
	LPSTR   lpDst;   

	//   指向缓存图像像素指针   
	LPSTR   lpNewDIBBits;   
	HLOCAL   hNewDIBBits;   

	//   循环变量   
	int   i,   j,   t;   

	//   用于计算两个区域的中间变量   
	long   lP1,   lS1,   lP2,   lS2;   

	//   像素值   
	unsigned   char   pixel;   

	//   灰度直方图数组   
	long   lHistogram[256];   

	//   阈值,   最大灰度值和最小灰度值,   两个区域的平均灰度值   
	unsigned   char   iThreshold,   iNewThreshold,   iMaxGrayValue,   iMinGrayValue,   iMean1GrayValue,   iMean2GrayValue;   

	//     前景点数占图像比例,   背景点数占图像比例   
	double   w0,w1;   

	//   方差   
	double   G,   tempG=0;   

	//   图像每行占字节数   
	LONG   lLineBytes;   

	//   暂时分配内存,   以保存新图像   
	hNewDIBBits   =   LocalAlloc(LHND,   lWidth   *   lHeight);   

	if   (hNewDIBBits   ==   NULL)     
	{   
		//分配内存失败   
		return   FALSE;   
	}   

	//   锁定内存   
	lpNewDIBBits   =   (char   *)   LocalLock(hNewDIBBits);   

	//   初始化新分配的内存,   设定初始值为255   
	lpDst   =   (char   *) lpNewDIBBits;   
	memset(lpDst,   (BYTE)255,   lWidth   *   lHeight);   

	lLineBytes   =   WIDTHBYTES(lWidth   *   8);   

	for(i=   0;   i   <   256;   i++)
	{   
		lHistogram[i]   =   0;   
	}   

	//   获得灰度直方图，灰度最大值和灰度最小值   
	iMaxGrayValue   =   0;   
	iMinGrayValue   =   255;   
	for(i   =   0;   i   <   lWidth;   i++)   
	{   
		for(j   =   0;   j   <   lHeight;   j++)   
		{   
			lpSrc   =   (char   *)   lpDIBBits   +   lLineBytes   *   j   +   i;   
			pixel   =   (unsigned   char)   *lpSrc;   
			lHistogram[pixel]++;   

			//   修改最大灰度值和最小灰度值   
			if   (iMinGrayValue   >   pixel)   
			{   
				iMinGrayValue   =   pixel;   
			}   
			if   (iMaxGrayValue   <   pixel)   
			{   
				iMaxGrayValue   =   pixel;   
			}   
		}   
	}   

	//   遍历t,   选取最佳阈值   
	for(t   =   iMinGrayValue;   t   <   iMaxGrayValue   ;   t++)   
	{   
		iNewThreshold   =   t;   
		lP1   =   0;   
		lS1   =   0;   
		lP2   =   0;   
		lS2   =   0;   

		//     求前景,背景两个区域的平均灰度值,   点数所占比例   
		for(i   =   iMinGrayValue;   i   <=   iNewThreshold;   i++)   
		{   
			lP1   +=   lHistogram[i]   *   i;   
			lS1   +=   lHistogram[i];   
		}   
		iMean1GrayValue   =   (unsigned   char)   (lP1/lS1);   
		w0   =   (double)   (lS1)   /   (lWidth   *   lHeight);   
		for(i   =   iNewThreshold   +   1;   i   <=   iMaxGrayValue;   i++)   
		{   
			lP2   +=   lHistogram[i]   *   i;   
			lS2   +=   lHistogram[i];   
		}   
		iMean2GrayValue   =   (unsigned   char)   (lP2/lS2);   
		w1   =   1   -   w0;   

		//   计算类间方差   
		G   =   (double)   w0   *   w1     
			*   (iMean1GrayValue   -   iMean2GrayValue)   *   (iMean1GrayValue   -   iMean2GrayValue);   
		if   (G   >   tempG)     
		{   
			tempG   =   G;   
			iThreshold   =   iNewThreshold;   
		}   
	}   

	//   根据阈值将图像二值化   
	for(i   =   0;   i   <   lWidth;   i++)   
	{   
		for(j   =   0;   j   <   lHeight;   j++)   
		{   
			lpSrc   =   (char   *)   lpDIBBits   +   lLineBytes   *   j   +   i;   
			lpDst   =   (char   *)   lpNewDIBBits   +   lLineBytes   *   j   +   i;   
			pixel   =   (unsigned   char)   *lpSrc;   
			if   (pixel   <=   iThreshold)     
			{   
				*lpDst   =   (unsigned   char)0;   
			}   
			else   
			{   
				*lpDst   =   (unsigned   char)   255;   
			}   
		}   
	}   

	//   复制图像   
	memcpy(lpDIBBits,   lpNewDIBBits,   lWidth   *   lHeight);
	//delete lpDst;
	//lpDst = NULL;
	//   释放内存   
	LocalUnlock(hNewDIBBits);   
	LocalFree(hNewDIBBits);   

	//   返回   
	return   iThreshold;   
}



/***************************************************************************   
*   函数名称   
*   ReadBMPFile  
*   参数   
*        LPCTSTR strFileName         -   图像的路径   

*   返回值   
*         LPBYTE                     -返回图像二进制数据
*   说明   
*         该函数采用文件读取的方法读取图像二进制流   
***************************************************************************/ 
bool CCBitmapConvert::ReadBMPFile(LPCTSTR strFileName,LPBYTE &m_lpBMPFileData,long& fileLength,long &width,long &height)
{
	
	CFile BitmapFile;
	BOOL blOpen=BitmapFile.Open(strFileName, CFile::modeRead,NULL);
	if( !blOpen )
	{
		//CString str = "读文件失败！";
		m_lpBMPFileData = NULL;
		return false;
	}

	unsigned long FileLength=BitmapFile.GetLength();
	fileLength = FileLength;
	m_lpBMPFileData = new BYTE[FileLength];

	ASSERT( m_lpBMPFileData!=NULL );
	BitmapFile.Read(m_lpBMPFileData,FileLength);
	
	if ( ((LPBITMAPFILEHEADER)m_lpBMPFileData)->bfType != 0x4D42 )
	{
		//CString str = "不支持除BMP之外的文件！";
		delete m_lpBMPFileData;
        m_lpBMPFileData = NULL;
		return false;;
	}
	LPBITMAPINFOHEADER lpBMPInfoHeader=(LPBITMAPINFOHEADER) (m_lpBMPFileData + sizeof(BITMAPFILEHEADER));
	width = lpBMPInfoHeader->biWidth;
	height = lpBMPInfoHeader->biHeight;
	return true;
}	

/************************************************************************* 
* 
* 函数名称： 
* ConvertToGrayScale() 
* 
* 参数: 
* LPSTR lpDIB - 指向源DIB图像指针 
* 
* 返回值: 
* bool - 成功返回true，否则返回false。 
* 
* 说明: 
* 该函数将24位真彩色图转换成256级灰度图 
* 从信息头开始
************************************************************************/ 
bool CCBitmapConvert::ConvertToGreyPicture( LPBYTE  pSource ) 
{ 

	LPSTR lpDIB = NULL;

	LPSTR   lpDIBBits = NULL; //指向DIB的象素的指针 
	LPSTR   lpNewDIBBits = NULL;
	LONG    lLineBytes = 0; 
	LONG    nWidth = 0; 
	LONG    nHeight = 0;
	LONG    nGreyLineBytes = 0; 
	int     nBitCount = 0; 
	LPBITMAPINFOHEADER pInforHead;

	unsigned char * lpSrc = NULL; //指向原图像象素点的指针 
	unsigned char * lpdest = NULL; //指向目标图像象素点的指针


	unsigned char  * ired = NULL;
	unsigned char  * igreen = NULL;
	unsigned char  * iblue = NULL; 
	long i = 0;
	long j =0; //循环变量
	RGBQUAD * lpRGBquad = NULL; 

	if( pSource == NULL )
	{
		return false;
	}
	
	lpDIB = (LPSTR)pSource + sizeof(BITMAPFILEHEADER);
	pInforHead = (BITMAPINFOHEADER *)(lpDIB);
	nBitCount = pInforHead->biBitCount;

	nWidth= pInforHead->biWidth; //DIB 宽度
	nHeight = pInforHead->biHeight; //DIB 高度

	if ( nBitCount == 8  )
	{
		//	lpDIBBits = (LPSTR)lpDIB + sizeof(BITMAPINFOHEADER) + 256 * 4;//指向DIB象素
		//不需要处理
		return true;
	}

	lpRGBquad = (RGBQUAD *)&lpDIB[sizeof(BITMAPINFOHEADER)]; //INFOHEADER后为调色板 

	if ( nBitCount == 24  )
	{
		lLineBytes = WIDTHBYTES( nWidth * 8 * 3 ); 
	}
	else
	{
		lLineBytes = WIDTHBYTES( nWidth * 8  ); 
	}

	nGreyLineBytes = WIDTHBYTES( nWidth * 8 );
	long m_nLineBytes = nGreyLineBytes;

	//灰度化等处理过后的位图数据
	lpdest= new BYTE[ nHeight * nGreyLineBytes];
	memset(lpdest , 0xff,  nHeight * nGreyLineBytes );

	lpDIBBits = (LPSTR)lpDIB + sizeof(BITMAPINFOHEADER);//指向DIB象素

	
	for( i = 0; i < nHeight; i++)
	{
		for(j = 0; j < nWidth * 3; j += 3 )
			{ 
				ired = (unsigned char*)lpDIBBits + lLineBytes * i + j + 2; 
				igreen = (unsigned char*)lpDIBBits + lLineBytes * i + j + 1; 
				iblue = (unsigned char*)lpDIBBits + lLineBytes * i + j ; 
				lpdest[ i * nGreyLineBytes +  j / 3] = (unsigned char)((*ired)*0.299 + (*igreen)*0.587 + (*iblue)*0.114); 
			}

	}
	

	//需要做三件事情：1、修改INFOHEADER 2、增加调色板 3、修改原图像灰度值 

	LPBITMAPINFOHEADER lpBMPInfoHeader;
	LPBITMAPFILEHEADER lpBMPFileHeader;
	lpBMPInfoHeader = (LPBITMAPINFOHEADER)lpDIB;
	lpBMPInfoHeader->biBitCount = 8; 
	lpBMPInfoHeader->biSizeImage = nGreyLineBytes *  nHeight; 
	lpBMPFileHeader = (LPBITMAPFILEHEADER)pSource; 
	lpBMPFileHeader->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256 * 4;
	lpBMPFileHeader->bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + lpBMPInfoHeader->biSizeImage ;
	
	//设置灰度调色板 
	for(i = 0;i < 256;i++) 
	{ 
		lpRGBquad[i].rgbRed = (unsigned char)i;
		lpRGBquad[i].rgbGreen = (unsigned char)i;
		lpRGBquad[i].rgbBlue = (unsigned char)i;
		lpRGBquad[i].rgbReserved = 0; 
	} 

	lpNewDIBBits = lpDIB + sizeof(BITMAPINFOHEADER) + 256 * 4;
	
	//修改灰度值 
	for(i = 0;i < nHeight; i++) 
	{
		for(j = 0;j < nGreyLineBytes; j++)
		{ 
			lpSrc = (unsigned char*)lpNewDIBBits + nGreyLineBytes * i+ j ; 
			*lpSrc = lpdest[ i * nGreyLineBytes + j] ; 
		} 
	}

	delete lpdest;
	lpdest = NULL;
	return true;
} 

//位图方式保存图片的部分区域
bool CCBitmapConvert::SaveNewPicture(CString filePath,LPBYTE m_newBmpData,long filelength)
{
	CFile file;
	if( !file.Open( filePath, CFile::modeWrite|CFile::modeCreate) )
		return false;
	file.Write(m_newBmpData,filelength);
		return true;
}