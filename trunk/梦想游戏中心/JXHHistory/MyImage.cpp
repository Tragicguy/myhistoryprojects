#include "StdAfx.h"
#include ".\myimage.h"
#include <direct.h>
#include <io.h>
#define WIDTHBYTES(i) ((i+31)/32*4)
CMyImage::CMyImage(void)
{
	    
	GdiplusStartup(&gdiplusToken,&gdiplusStartupInput, NULL);
}

CMyImage::~CMyImage(void)
{
	GdiplusShutdown(gdiplusToken);
}

//将图片信息转换成BMP格式保存
BOOL CMyImage::TranstToBMP(CString srcPath,CString destPath)
{
	
	
	WCHAR* wCH = new WCHAR[srcPath.GetLength()];
	wcscpy(wCH,srcPath.AllocSysString());

	Image img(wCH);//这里的图片可以是其它格式
	CLSID pngClsid;
	GetImageCLSID(L"image/bmp", &pngClsid);//这里的图片可以是其它格式，此处转化为BMP格式

	WCHAR* wCH1 = new WCHAR[destPath.GetLength()];
	wcscpy(wCH1,   destPath.AllocSysString());
	img.Save(wCH1, &pngClsid, NULL);
	
	delete []wCH;
	delete []wCH1;
	wCH = NULL;
	wCH1 = NULL;

	
	return TRUE;
}

/////////
BOOL CMyImage::GetImageCLSID(const WCHAR* format, CLSID* pCLSID)
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
		//MimeType: Depiction for the program image
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

//显示位图数据
BOOL CMyImage::ShowBmpFile(HDC hdc, int nXDest, int nYDest,LPBYTE lpDIB)
{
	if( lpDIB == NULL)
	{
		return FALSE;
	}
	LPBITMAPINFOHEADER lpBMPInfoHeader=(LPBITMAPINFOHEADER) (lpDIB + sizeof(BITMAPFILEHEADER));
	BITMAPINFO * lpBMPInfo =(BITMAPINFO *)(lpDIB + sizeof(BITMAPFILEHEADER));

	SetDIBitsToDevice(hdc,nXDest,nYDest,lpBMPInfoHeader->biWidth,lpBMPInfoHeader->biHeight,
			0,0,0,lpBMPInfoHeader->biHeight,lpDIB+((BITMAPFILEHEADER *)lpDIB)->bfOffBits,lpBMPInfo,DIB_RGB_COLORS);
		
	return TRUE;
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

int CMyImage::OSTUThreshold(LPBYTE   lpDIBBits,   LONG   lWidth,   LONG   lHeight)   
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
	delete lpDst;
	lpDst = NULL;
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
bool CMyImage::ReadBMPFile(LPCTSTR strFileName,LPBYTE &m_lpBMPFileData,long& fileLength)
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
	return true;
}	

//在GDI+中，有关字符的参数类型全部都是WCHAR类型     
//该函数是将传统字符串进行转换 
WCHAR*  CMyImage::ToWChar(char * str)     
{     
	static   WCHAR   buffer[1024];
	wcsset(buffer,0);
	MultiByteToWideChar(CP_ACP,0,str,strlen(str),buffer,1024);
	return   buffer;     
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
bool CMyImage::ConvertToGreyPicture( LPBYTE  pSource ) 
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
bool CMyImage::SaveNewPicture(CString filePath,LPBYTE m_newBmpData,long filelength)
{
	CFile file;
	if( !file.Open( filePath, CFile::modeWrite|CFile::modeCreate) )
		return false;
	file.Write(m_newBmpData,filelength);
		return true;
}

//中值滤波
void CMyImage::ClearNoise(LPBYTE m_BmpData)
{
	long lWidth = 0;
	long lHeight = 0;

	if (m_BmpData == NULL)
	{
		return ;
	}

	lWidth = ((LPBITMAPINFOHEADER)(m_BmpData +sizeof(BITMAPFILEHEADER)))->biWidth;
	lHeight = ((LPBITMAPINFOHEADER)(m_BmpData +sizeof(BITMAPFILEHEADER)))->biHeight;

	long lLineBytes = WIDTHBYTES(lWidth   *   8);
	long totalNum = lWidth*lHeight;
	LPBYTE lpBitmapData = new BYTE[totalNum];

	::memcpy(lpBitmapData,(LPBYTE)m_BmpData + ((BITMAPFILEHEADER *)m_BmpData)->bfOffBits,totalNum);

	BYTE byteArea[9] = {' '};
	BYTE tempByte = 0;
	 for (long i = 1;i < lWidth - 1; i++ )
		for (long j = 1; j < lHeight - 1; j++)
		{
			byteArea[0] = *(lpBitmapData + lLineBytes *(j - 1) + i - 1);
            byteArea[1] = *(lpBitmapData + lLineBytes *(j - 1) + i);
			byteArea[2] = *(lpBitmapData + lLineBytes *(j - 1) + i + 1);
			byteArea[3] = *(lpBitmapData + lLineBytes * j + i - 1);
			byteArea[4] = *(lpBitmapData + lLineBytes * j + i);
			byteArea[5] = *(lpBitmapData + lLineBytes * j + i + 1);
			byteArea[6] = *(lpBitmapData + lLineBytes *(j + 1) + i - 1);
			byteArea[7] = *(lpBitmapData + lLineBytes *(j - 1) + i);
			byteArea[8] = *(lpBitmapData + lLineBytes *(j + 1) + i + 1);
			int totalNum = 0;
			for (int k = 0; k<5; k++)
				for (int m = k+1;m< 9;m++)
				{
					if (byteArea[m] > byteArea[k])
					{
						tempByte  = byteArea[m];
						byteArea[m] = byteArea[k];
						byteArea[k] = tempByte;

					}
				}
			
			 tempByte = *(lpBitmapData + lLineBytes * j + i);	
			 if(tempByte == byteArea[0] || tempByte == byteArea[9])
			 {
				 *(lpBitmapData + lLineBytes * j + i) = byteArea[4];
			 }
			
		}

		::memcpy((LPBYTE)m_BmpData + ((BITMAPFILEHEADER *)m_BmpData)->bfOffBits,lpBitmapData,totalNum);
		delete lpBitmapData;
		lpBitmapData = NULL;
		return;	
}

//将IplImage*类型的图像数据显示到窗口指定位置处
void CMyImage::ShowToDC(IplImage* imgShow,HWND m_hwnd,int startX,int startY)
{
	CImage CImgShow;
	CImgShow.CopyOf(imgShow,-1);
	CImgShow.DrawToHDC(::GetDC(m_hwnd), &CRect(startX,startY,imgShow->width+startX,imgShow->height+startY));
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
//获取游戏图片数据
void CMyImage::GetSelectWindowPic(HWND hwnd,CString strSavePath)
{
	LPBYTE pDst = NULL;
	CDC *pDC;
	pDC = CDC::FromHandle(::GetDC(hwnd));
	CRect rc;
	CWnd::FromHandle(hwnd)->GetClientRect(&rc);

	int BitPerPixel = pDC->GetDeviceCaps(BITSPIXEL);//获得颜色模式

	int Width = rc.Width();//pDC->GetDeviceCaps(HORZRES);
	int Height = rc.Height();//pDC->GetDeviceCaps(VERTRES);

	CDC memDC;//内存DC
	memDC.CreateCompatibleDC(pDC);

	CBitmap memBitmap, *oldmemBitmap;//建立和屏幕兼容的bitmap
	memBitmap.CreateCompatibleBitmap(pDC, Width, Height);

	oldmemBitmap = memDC.SelectObject(&memBitmap);//将memBitmap选入内存DC
	memDC.BitBlt(0, 0, Width, Height, pDC, 0, 0, SRCCOPY);//复制屏幕图像到内存DC

	//以下代码保存memDC中的位图到文件
	BITMAP bmp;
	memBitmap.GetBitmap(&bmp);//获得位图信息

	BITMAPFILEHEADER bfh = {0};//位图文件头
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//到位图数据的偏移量
	bfh.bfSize = bfh.bfOffBits + bmp.bmWidthBytes * bmp.bmHeight;//文件总的大小
	bfh.bfType = (WORD)0x4d42;

	BITMAPINFOHEADER bih = {0};//位图信息头
	bih.biBitCount = bmp.bmBitsPixel;//每个像素字节大小
	bih.biCompression = BI_RGB;
	bih.biHeight = bmp.bmHeight;//高度
	bih.biPlanes = 1;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;//图像数据大小
	bih.biWidth = bmp.bmWidth;//宽度

	BYTE *p = new BYTE[bmp.bmWidthBytes * bmp.bmHeight];//申请内存保存位图数据
	GetDIBits(memDC.m_hDC, (HBITMAP) memBitmap.m_hObject, 0, Height, p,
		(LPBITMAPINFO) &bih, DIB_RGB_COLORS);//获取位图数据
	pDst = new BYTE[bfh.bfOffBits + bmp.bmWidthBytes * bmp.bmHeight];
	memcpy(pDst,&bfh,sizeof(BITMAPFILEHEADER));
	memcpy(pDst +sizeof(BITMAPFILEHEADER),&bih,sizeof(BITMAPINFOHEADER));
	memcpy(pDst+bfh.bfOffBits,p,bmp.bmWidthBytes * bmp.bmHeight);

	SaveNewPicture(strSavePath,pDst,bfh.bfOffBits+bmp.bmWidthBytes * bmp.bmHeight);
	delete []p;
	delete []pDst;
	memDC.SelectObject(oldmemBitmap);
}