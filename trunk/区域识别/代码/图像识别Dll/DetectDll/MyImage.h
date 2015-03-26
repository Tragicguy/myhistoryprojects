#pragma once

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>


class CMyImage
{

	struct myParam
	{
		IplImage* img;
		bool drawFlag;
		bool finishFlag;

	};
public:
	CMyImage(void); 
	~CMyImage(void);
	CPoint Detect_and_Draw(IplImage* img,CvHaarClassifierCascade* cascade,int do_pyramids);
	CPoint MatchPicture(CString imgPath,CString xmlPath);
	static void on_mouseProc( int event, int x, int y, int flags, void* param);
	void MackTemplate(CString srcPath);



	

};
