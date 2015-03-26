/***********************************************************************
 *
 * AspriseOCR.h - header file of Asprise OCR library. 
 * 
 * Copyright (c) 1998-2004 LAB Asprise!. All rights reserved.
 *
 * 
 * Please visit http://asprise.com/product/ocr for more details. 
 * 
 ***********************************************************************/


#ifndef ASPRISE_OCR_H
#define ASPRISE_OCR_H

// Definition of image types. 

#define IMAGE_TYPE_BMP			0x0420	// Windows Bitmap
#define IMAGE_TYPE_CUT			0x0421	// Dr. Halo Cut File
#define IMAGE_TYPE_DOOM			0x0422	
#define IMAGE_TYPE_DOOM_FLAT	0x0423
#define IMAGE_TYPE_ICO			0x0424	// Icons
#define IMAGE_TYPE_JPG			0x0425	// Jpeg
#define IMAGE_TYPE_JFIF			0x0425	
#define IMAGE_TYPE_LBM			0x0426	// Interlaced Bitmap
#define IMAGE_TYPE_PCD			0x0427	// PhotoCD
#define IMAGE_TYPE_PCX			0x0428
#define IMAGE_TYPE_PIC			0x0429	// PIC
#define IMAGE_TYPE_PNG			0x042A	// Portable Network Graphics
#define IMAGE_TYPE_PNM			0x042B	// Pnm
#define IMAGE_TYPE_SGI			0x042C	// Silicon Graphics
#define IMAGE_TYPE_TGA			0x042D	// Targa
#define IMAGE_TYPE_TIF			0x042E	// TIF
#define IMAGE_TYPE_CHEAD		0x042F
#define IMAGE_TYPE_RAW			0x0430
#define IMAGE_TYPE_MDL			0x0431	// Half-Life Model
#define IMAGE_TYPE_WAL			0x0432	// Quake2 Texture
#define IMAGE_TYPE_LIF			0x0434	// Homeworld File
#define IMAGE_TYPE_MNG			0x0435	// Mng Animation
#define IMAGE_TYPE_JNG			0x0435
#define IMAGE_TYPE_GIF			0x0436	// GIF
#define IMAGE_TYPE_DDS			0x0437
#define IMAGE_TYPE_DCX			0x0438
#define IMAGE_TYPE_PSD			0x0439	// PhotoShop
#define IMAGE_TYPE_EXIF			0x043A
#define IMAGE_TYPE_PSP			0x043B
#define IMAGE_TYPE_PIX			0x043C
#define IMAGE_TYPE_PXR			0x043D
#define IMAGE_TYPE_XPM			0x043E

#define IMAGE_TYPE_AUTO_DETECT	-1		// Detects the image type automatically



#ifdef __cplusplus
extern "C" {
#endif

	__declspec(dllexport)  char* OCR(char* filePath, int fileType);

	__declspec(dllexport)  char* OCRpart( char* filePath, int fileType, int startX, int startY, int width, int height);

	__declspec(dllexport)  char* OCRBarCodes(char* filePath, int fileType);

	__declspec(dllexport)  char* OCRpartBarCodes( char* filePath, int fileType, int startX, int startY, int width, int height);


#ifdef __cplusplus
   } 
#endif

#endif
