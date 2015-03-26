// zhaochaDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "zhaocha.h"
#include "zhaochaDlg.h"
#include ".\zhaochadlg.h"
#define WIDTHBYTES(i) ((i+31)/32*4)
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define	WM_GETDSTDIFFRENT	WM_USER+11					//获取不同区域消息
#define	WM_USER_CLOSEWINDOW	WM_USER+12				
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
		CAboutDlg();

		// 对话框数据
		enum { IDD = IDD_ABOUTBOX };

protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

		// 实现
protected:
		DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
		CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()




_declspec(dllimport) HHOOK SetHook(HWND m_hwndHelper);

// CzhaochaDlg 对话框



CzhaochaDlg::CzhaochaDlg(CWnd* pParent /*=NULL*/)
: CDialog(CzhaochaDlg::IDD, pParent)
{
		m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CzhaochaDlg::DoDataExchange(CDataExchange* pDX)
{
		CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CzhaochaDlg, CDialog)
		ON_WM_SYSCOMMAND()
		ON_WM_PAINT()
		ON_WM_QUERYDRAGICON()
		//}}AFX_MSG_MAP
		ON_BN_CLICKED(IDC_BTN_FIND, OnBnClickedBtnFind)
		ON_MESSAGE(WM_GETDSTDIFFRENT,GoToGetDiffrent)
		ON_MESSAGE(WM_USER_CLOSEWINDOW,CloseFindWindow)
		ON_WM_CLOSE()
END_MESSAGE_MAP()


// CzhaochaDlg 消息处理程序

BOOL CzhaochaDlg::OnInitDialog()
{
		CDialog::OnInitDialog();

		// 将\“关于...\”菜单项添加到系统菜单中。

		// IDM_ABOUTBOX 必须在系统命令范围内。
		ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
		ASSERT(IDM_ABOUTBOX < 0xF000);

		CMenu* pSysMenu = GetSystemMenu(FALSE);
		if (pSysMenu != NULL)
		{
				CString strAboutMenu;
				strAboutMenu.LoadString(IDS_ABOUTBOX);
				if (!strAboutMenu.IsEmpty())
				{
						pSysMenu->AppendMenu(MF_SEPARATOR);
						pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
				}
		}

		// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
		//  执行此操作
		SetIcon(m_hIcon, TRUE);			// 设置大图标
		SetIcon(m_hIcon, FALSE);		// 设置小图标

		//设置HOOK
		m_lowHook = NULL;
		m_lowHook = SetHook(m_hWnd);

		m_btn.MoveWindow(50,50,50,50);
		m_btn.SubclassDlgItem(IDC_BTN_FIND,this);
		m_btn.SetIcon(IDR_MAINFRAME);
		m_btn.DrawBorder(FALSE);
		m_btn.DrawTransparent(TRUE);
		return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CzhaochaDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
		if ((nID & 0xFFF0) == IDM_ABOUTBOX)
		{
				CAboutDlg dlgAbout;
				dlgAbout.DoModal();
		}
		else
		{
				CDialog::OnSysCommand(nID, lParam);
		}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CzhaochaDlg::OnPaint() 
{
		if (IsIconic())
		{
				CPaintDC dc(this); // 用于绘制的设备上下文

				SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

				// 使图标在工作矩形中居中
				int cxIcon = GetSystemMetrics(SM_CXICON);
				int cyIcon = GetSystemMetrics(SM_CYICON);
				CRect rect;
				GetClientRect(&rect);
				int x = (rect.Width() - cxIcon + 1) / 2;
				int y = (rect.Height() - cyIcon + 1) / 2;

				// 绘制图标
				dc.DrawIcon(x, y, m_hIcon);
		}
		else
		{
				CDialog::OnPaint();
		}

}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CzhaochaDlg::OnQueryDragIcon()
{
		return static_cast<HCURSOR>(m_hIcon);
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CzhaochaDlg::PreTranslateMessage( MSG* pMsg)
{
		if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		{
				return TRUE;      //屏蔽掉Esc键
		}
		if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		{
				return TRUE;      //屏蔽掉Enter键
		}

		return CDialog::PreTranslateMessage( pMsg );
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CzhaochaDlg::on_mouseProc( int event, int x, int y, int flags, void* param)
{
		myParam *myparam = (myParam*)param;
		if(event==CV_EVENT_LBUTTONDOWN)
		{
				::PostMessage(myparam->hwnd,WM_LBUTTONDOWN,0,MAKELONG(x+myparam->x,y+myparam->y));
		}
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
void CzhaochaDlg::GetLocation(IplImage* Img,int &x1,int &x2,int &x3,int tempY)
{

		CvScalar backColor;				//中间的背景色

		for (int i=4;i< 600;i++)
		{
				if (x1 == 0) 
				{
						CvScalar tempColor = cvGet2D(Img,tempY,i);
						CvScalar tempColor1 = cvGet2D(Img,tempY,i-1);
						CvScalar tempColor2 = cvGet2D(Img,tempY,i-2);
						if (tempColor.val[0] < 100 &&					//找到图1的起始X坐标
								tempColor.val[1] < 100 && 
								tempColor.val[2] < 100 &&
								tempColor1.val[0] > 200 && 
								tempColor1.val[1] > 200 && 
								tempColor1.val[2] > 200 &&
								tempColor2.val[0] > 200 && 
								tempColor2.val[1] > 200 && 
								tempColor2.val[2] > 200 )
						{
								x1 = i;
								backColor = tempColor;			//给中间的背景色赋值
								continue;
						}
				}
				else 
				{
						if (x2 == 0)
						{
								CvScalar tempColor = cvGet2D(Img,tempY,i);
								CvScalar tempColor1 = cvGet2D(Img,tempY,i-1);
								if (tempColor.val[0] == backColor.val[0] &&				//找图1的右X坐标
										tempColor.val[1] == backColor.val[1] && 
										tempColor.val[2] == backColor.val[2] &&
										tempColor1.val[0] != backColor.val[0] && 
										tempColor1.val[1] != backColor.val[1] && 
										tempColor1.val[2] != backColor.val[2]	)
								{
										x2 = i;
										continue;
								}
						}
						else
						{
								if (x3 == 0)
								{
										CvScalar tempColor = cvGet2D(Img,tempY,i);
										CvScalar tempColor1 = cvGet2D(Img,tempY,i+1);
										CvScalar tempColor2 = cvGet2D(Img,tempY,i+2);
										if (tempColor.val[0] == backColor.val[0] && 
												tempColor.val[1] == backColor.val[1] && 
												tempColor.val[2] == backColor.val[2] &&
												tempColor1.val[0] == backColor.val[0] && 
												tempColor1.val[1] == backColor.val[1] && 
												tempColor1.val[2] == backColor.val[2] &&
												tempColor2.val[0] != backColor.val[0] && 
												tempColor2.val[1] != backColor.val[1] && 
												tempColor2.val[2] != backColor.val[2] )
										{
												x3 = i+2;
												break;
										}
								}
						}
				}
		}

}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//位图方式保存图片的部分区域
bool CzhaochaDlg::SaveNewPicture(CString filePath,LPBYTE m_newBmpData,long filelength)
{
		CFile file;
		if( !file.Open( filePath, CFile::modeWrite|CFile::modeCreate))
				return false;
		file.Write(m_newBmpData,filelength);
		return true;
}



/************************************************************************/
/*                                                                      */
/************************************************************************/
//获取游戏图片数据
void CzhaochaDlg::GetSelectWindowPic(HWND hwnd)
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

		SaveNewPicture(_T("srcPic.bmp"),pDst,bfh.bfOffBits+bmp.bmWidthBytes * bmp.bmHeight);
		delete []p;
		delete []pDst;
		memDC.SelectObject(oldmemBitmap);
}



/************************************************************************/
/*                                                                      */
/************************************************************************/
//找茬操作
void CzhaochaDlg::OnBnClickedBtnFind()
{
		
		IplImage *Img;

		HWND hackHwnd = FindWindow(_T("#32770"),_T("大家来找茬"))->GetSafeHwnd();		//获取找茬游戏的窗口句柄

		if (hackHwnd == NULL)					//如果没有找到目标窗口
		{
				return;
		}
		
		GetSelectWindowPic(hackHwnd);			//获取游戏图片


		if((Img = cvLoadImage("srcPic.bmp", 1)) == 0)	//如果没有找到图片
		{
				return;
		}
		int x1 = 0;				//第一幅图的左X坐标
		int x2 = 0;				//第一幅图的右X坐标
		int x3 = 0;				//第二幅图的左X坐标
		int width = 0;			//图宽度
		int y = 190;			//图的Y方向起始坐标

		int tempY = 200           ;		//..........................

		while((x1 == 0 || x2 == 0 || x3 == 0 || width <= 0) && tempY <270)				//获取两图像的起始和结束位置
		{
				x1=0;
				x2=0;
				x3 = 0;
				width = 0;
				GetLocation(Img,x1,x2,x3,tempY);
				width = x2 - x1;
				tempY += 5;
		}

		
		int height = 450;				//图像高度
		if (x1 == 0 || x2 == 0 || x3 == 0 || width <= 0)		//如果找茬失败
		{
				return;
		}

		//调整坐标，否则找不到
		x1 += 1;
		width -= 1;
		
		//调整最小矩形的参数
		int ijStart = 10;
		width -= 10;
		height -= 10;

		CvMat *dst;
		CvRect subRect;
		dst=cvCreateMat(width,height,CV_8UC3);
		subRect =cvRect(x1,y,width,height);
		cvGetSubRect(Img,dst,subRect);
		IplImage *dstLeft;
		dstLeft=cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,3);
		cvGetImage(dst,dstLeft);

		subRect =cvRect(x3,y,width,height);
		cvGetSubRect(Img,dst,subRect);
		IplImage *dstRight;
		dstRight=cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,3);
		cvGetImage(dst,dstRight);

		IplImage *dstXor;
		dstXor=cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,3);
		cvXor(dstLeft,dstRight,dstXor);

		cvSaveImage("dstPic.bmp",dstXor);
		cvReleaseImage(&Img);

		IplImage *dstXorBinary;

		if((dstXorBinary = cvLoadImage("dstPic.bmp", 0)) == 0)
				return;
		cvThreshold(dstXorBinary,dstXorBinary,1,255,CV_THRESH_BINARY);
		dstXorBinary->origin = 1;

		int xRight;		//最小矩形最右坐标
		int xLeft;		//最小矩形最左坐标
		int yTop;		//最小矩形最上坐标
		int yBottom;	//最小矩形最下坐标

/************************************************************************/
/*以下操作获取不同区域所构成的最小矩形（为了让不同区域窗口最小，方便控制）*/
/************************************************************************/
		for (int i=ijStart;i<width;i++)
				for (int j=ijStart;j<height;j++)
				{
						CvScalar tempColor = cvGet2D(dstXorBinary,j,i);
						if (tempColor.val[0] != 0)
						{
								xRight = i;
								break;
						}
				}

		for (int i=width-1;i>=ijStart ;i--)
			for (int j=ijStart;j<height;j++)
			{
					CvScalar tempColor = cvGet2D(dstXorBinary,j,i);
					if (tempColor.val[0] != 0)
					{
							xLeft = i;
							break;
					}
			}		

		for (int j=ijStart;j<height;j++)
			for (int i=ijStart;i<width;i++)
				{
						CvScalar tempColor = cvGet2D(dstXorBinary,j,i);
						if (tempColor.val[0] != 0)
						{
								yBottom = j;
								break;
						}
				}

		for (int j=height-1;j>=ijStart;j--)
			for (int i=ijStart;i<width;i++)
			{
				CvScalar tempColor = cvGet2D(dstXorBinary,j,i);
				if (tempColor.val[0] != 0)
				{
						yTop = j;
						break;
				}
			}
/************************************************************************/
/*以上操作获取不同区域所构成的最小矩形（为了让不同区域窗口最小，方便控制）*/
/************************************************************************/
			//如果矩形获取出错，退出
			if (xRight - xLeft< 0 || yBottom-yTop < 0)
			{
					return;
			}

			//得到最小矩形图以显示
			dst=cvCreateMat(xRight - xLeft,yBottom-yTop,CV_8UC1);
			subRect =cvRect(xLeft,yTop,xRight - xLeft,yBottom-yTop);
			cvGetSubRect(dstXorBinary,dst,subRect);

			IplImage *dstImage;
			dstImage=cvCreateImage(cvSize(xRight - xLeft,yBottom-yTop),IPL_DEPTH_8U,1);
			cvGetImage(dst,dstImage);

			cvNamedWindow("找茬成功");
			cvShowImage( "找茬成功", dstImage);

			myParam *myparam = new myParam;
			myparam->img = dstXor;
			myparam->x = x1+xLeft;
			myparam->y = y+yTop;
			myparam->hwnd = hackHwnd;
			cvSetMouseCallback("找茬成功",on_mouseProc,(void*)myparam);

			cvWaitKey(0); //等待按键

			cvDestroyWindow( "找茬成功" );//销毁窗口
			cvReleaseImage(&dstXorBinary);//释放图像
			delete myparam;
			myparam = NULL;

}



/************************************************************************/
/*                                                                      */
/************************************************************************/
//键盘消息处理
LRESULT CzhaochaDlg::GoToGetDiffrent(WPARAM wParam,LPARAM lParam)
{
		HWND hWind = NULL;
		hWind = FindWindow(_T("Main HighGUI class"),_T("找茬成功"))->GetSafeHwnd();	
		if ( !hWind )			//如果找茬成功按钮不存在
		{
				OnBnClickedBtnFind();
		}
		
		return 0L;
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
LRESULT CzhaochaDlg::CloseFindWindow(WPARAM wParam,LPARAM lParam)
{
		HWND hWind = NULL;
		hWind = FindWindow(_T("Main HighGUI class"),_T("找茬成功"))->GetSafeHwnd();		
		if ( hWind )			//如果找茬成功按钮不存在
		{
				::PostMessage(hWind,WM_CLOSE,0,0);
		}

		return 0L;
}



void CzhaochaDlg::OnClose()
{
		// TODO: 在此添加消息处理程序代码和/或调用默认值
		if ( m_lowHook )
		{		
				UnhookWindowsHookEx(m_lowHook);
		}
		CDialog::OnClose();
}


CString CzhaochaDlg::GetAppPath()
{
		CString strRet;
		TCHAR szBuff[_MAX_PATH];
		VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szBuff, _MAX_PATH));
		strRet=szBuff;
		int pos=strRet.ReverseFind('\\');
		strRet=strRet.Left(pos);
		return strRet;
}