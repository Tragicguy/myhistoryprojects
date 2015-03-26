#include "StdAfx.h"
#include ".\myimage.h"
#include <direct.h>
#include <io.h>
#define WIDTHBYTES(i) ((i+31)/32*4)
CMyImage::CMyImage(void)
{

}

CMyImage::~CMyImage(void)
{

}

/***************************************************************************   
*   函数名称   
*   MatchPicture  
*   参数   
*       CString imgPath         -   图像的路径  
*       CString xmlPath         -   模板路径

*   返回值   
*         CPoint   -返回匹配区域的中心坐标，若检测不到则返回（0，0）
*   说明   
*         该函数进行图片区域识别 
***************************************************************************/ 
CPoint CMyImage::MatchPicture(CString imgPath,CString xmlPath)
{
	CPoint centerPoint;
	
	CvHaarClassifierCascade* cascade = 0;
	cascade = (CvHaarClassifierCascade*)cvLoad(xmlPath); //加载特征文件
	if( !cascade )
	{
		return CPoint(0,0);
	}

	IplImage* image = cvLoadImage( imgPath,1); //加载等待检测的图像
	if(image)
	{
		centerPoint = Detect_and_Draw(image,cascade,0);   //目标检测
		cvWaitKey(0);
		cvReleaseImage(&image);
	}
	else
	{
		return CPoint(0,0);
	}
	cvReleaseHaarClassifierCascade( &cascade);  //释放
	return centerPoint;
}

//检测图片，并返回中心点坐标
CPoint CMyImage::Detect_and_Draw(IplImage* img,CvHaarClassifierCascade* cascade,int do_pyramids)
{
	IplImage* small_image = img;
	CvMemStorage* storage = cvCreateMemStorage(0); //创建动态内存
	CvSeq* faces;
	int i, scale = 1;
	if( do_pyramids )
	{
		small_image = cvCreateImage( cvSize(img->width/2,img->height/2), IPL_DEPTH_8U, 3 );
		//函数 cvPyrDown 使用 Gaussian 金字塔分解对输入图像向下采样。
		//首先它对输入图像用指定滤波器进行卷积，然后通过拒绝偶数的行与列来下采样图像。
		cvPyrDown( img, small_image, CV_GAUSSIAN_5x5 );
		scale = 2;
	}
	
	CvPoint center;
	if( cascade )
	{
		//检测
		faces = cvHaarDetectObjects( small_image, cascade, storage, 1.2, 2, CV_HAAR_DO_CANNY_PRUNING );
		int radius = 200;
		if (faces->total > 0)
		{
				for( i = 0; i < (faces ? faces->total : 0); i++ )
				{
					CvRect* r = (CvRect*)cvGetSeqElem( faces, i );
					
					int tempRadiu = cvRound((r->width + r->height)*0.25*scale);
					if (tempRadiu < radius)  //取半径最小的为中心点
 					{
						radius = tempRadiu;
						center.x = cvRound((r->x + r->width*0.5)*scale);
						center.y = cvRound((r->y + r->height*0.5)*scale);
					}
					
				}
		}
		else
		{
			center.x = 0;
			center.y = 0;
		}
	}
	else
	{
		center.x = 0;
		center.y = 0;
	}
	if( small_image != img )
	cvReleaseImage( &small_image );

	return CPoint(center.x,center.y);
}

//样本生成
void CMyImage::MackTemplate(CString srcPath)
{

	IplImage* pImg; //声明IplImage指针
	if((pImg = cvLoadImage(srcPath, 1)) != 0 )
	{
		cvNamedWindow( "Image", 1 );//创建窗口
		cvShowImage("Image", pImg );//显示图像
		
		myParam *myparam = new myParam;
		myparam->img = pImg;
		myparam->drawFlag = false;
		myparam->finishFlag = false;
		cvSetMouseCallback("Image",on_mouseProc,(void*)myparam);  //建立窗口过程

		cvWaitKey(0); //等待按键

		cvDestroyWindow( "Image" );//销毁窗口
		cvReleaseImage( &pImg ); //释放图像
		delete myparam;
		myparam = NULL;
	}
}

//获取样本的鼠标操作的窗口过程,生成正负样本
void CMyImage::on_mouseProc( int event, int x, int y, int flags, void* param)
{
	
	myParam *myparam = (myParam*)param;
	//如果是左键点击
	if(myparam->drawFlag==false && myparam->finishFlag == false && event==CV_EVENT_LBUTTONDOWN)
	{
		myparam->drawFlag = true;
	}
	
	if(myparam->drawFlag==true && myparam->finishFlag == false && event==CV_EVENT_MOUSEMOVE)
	{
		IplImage *tempImage = 0;
		tempImage = cvCreateImage(cvGetSize(myparam->img), IPL_DEPTH_8U, 1);
		tempImage = cvCloneImage(myparam->img);

        cvRectangle(tempImage,cvPoint(x,y),cvPoint(x+40,y+20),cvScalar(255,255,255),1);
		cvShowImage("Image",tempImage);
	}

	//右键点击，则生成样本
	if(myparam->drawFlag==true && myparam->finishFlag == false && event==CV_EVENT_RBUTTONDOWN)
	{
		CFileStatus   rStatus;
		CString dirPath = "C:\\OpenCVTrain\\Template";
		if(!CFile::GetStatus(dirPath,rStatus)) //目录不存在
		{
			mkdir(dirPath);
		}

		CvMat *dst;
		dst=cvCreateMat(40,20,CV_8UC1);
		CvRect subRect;

		subRect=cvRect(x,y,40,20);
		cvGetSubRect(myparam->img,dst,subRect);
		IplImage *dst1;
		dst1=cvCreateImage(cvSize(40,20),IPL_DEPTH_8U,1);
		cvGetImage(dst,dst1);
		CString filePath;
		CString fileName;
		fileName.Format("%d%d%s.jpg",x,y,CTime::GetCurrentTime().Format("%Y%m%d%H%M%S"));//保存正样本
		filePath = dirPath + "\\" + fileName;
		cvSaveImage(filePath,dst1);

		// 将目标区域填充成白色，再将整幅图保存为背景图（负样本）
		CvPoint  curve1[]={x-2,y-2,  x+42,y-2,  x+42,y+22,  x-2,y+22}; 
		CvPoint* curveArr[1]={curve1};
		int  nCurvePts[1]={4};
		int  nCurves=1;

		cvFillPoly(myparam->img,curveArr,nCurvePts,nCurves,cvScalar(255,255,255));


		CString diroffPath = "C:\\OpenCVTrain\\offTemplate";
		if(!CFile::GetStatus(diroffPath,rStatus)) //目录不存在
		{
			mkdir(diroffPath);
		}

		CString bgFilePath;
		CString bgFileName;
		bgFileName.Format("%d%d%s.jpg",x,y,CTime::GetCurrentTime().Format("%Y%m%d%H%M%S"));//保存负样本
		bgFilePath = diroffPath + "\\" + fileName;
		cvSaveImage(bgFilePath,myparam->img);

		myparam->drawFlag = false;
		myparam->finishFlag = true;

	}


}
