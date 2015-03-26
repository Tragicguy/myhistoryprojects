#pragma once
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
class COpenCVCheck
{
public:
	COpenCVCheck(void);
	~COpenCVCheck(void);
	void COpenCVCheck::GetBinaryData(LPBYTE pSourse,LPBYTE &m_newBMP);
	BOOL ShowBmpFile(HDC hdc, int nXDest, int nYDest,LPBYTE lpDIB);
	bool ReadBMPFile(LPCTSTR strFileName,LPBYTE& m_lpBMPFileData);
	bool SaveNewPicture(CString filePath,LPBYTE m_newBmpData,long filelength);
	void OpenCVBinary(CString fileName);
	int GetPicLength(LPBYTE pSource);
	void ReverseColor(IplImage* pImg);
	void ClearNoise(LPBYTE pDIB);
	void ClearNoise(IplImage *pImg);
	int GetNearBlackNum(LPBYTE lpBits,int &x,int &y,int i,int j,long lLineBytes);
	int GetNearBlackNum(IplImage * pImg,int &x,int &y,int i,int j);
private:
	CString myPicPath;
};
