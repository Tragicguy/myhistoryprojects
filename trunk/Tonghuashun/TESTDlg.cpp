// TESTDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TEST.h"
#include "TESTDlg.h"
#include ".\testdlg.h"
#include <io.h>
#include <direct.h>
#include <odbcinst.h>
#include <afxdb.h>
#include "afxinet.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int gl_iWidth = 0;

HWND gl_hwndDataWindow = NULL;
HWND gl_hwndMoveWindow = NULL;
HWND gl_hwwdCtrolWindow = NULL;


IplImage* gl_pImageSouce = NULL;
IplImage* gl_pImageSubMat[10] = {NULL};


CString gl_strLastTime = "";
int gl_iFileTotalCount = 0;
CString gl_strAppPath = "";
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


// CTESTDlg 对话框



CTESTDlg::CTESTDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTESTDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTESTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTESTDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BTN_START, &CTESTDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &CTESTDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_EXPORT, &CTESTDlg::OnBnClickedBtnExport)
	ON_BN_CLICKED(IDC_BTN_FIND, &CTESTDlg::OnBnClickedBtnFind)
	ON_BN_CLICKED(IDC_BUTTON3, &CTESTDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BTN_STOP2, &CTESTDlg::OnBnClickedBtnStop2)
END_MESSAGE_MAP()


// CTESTDlg 消息处理程序

BOOL CTESTDlg::OnInitDialog()
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
#ifdef _DEBUG
#else			//发布版要控制权限
	if (GetPower() != 0)
	{
		exit(0);
	}
#endif
	// TODO: 在此添加额外的初始化代码
	GetAppPath(gl_strAppPath);
	CString strFolder = gl_strAppPath + "源图像";
	if (_access(strFolder,0) != 0)
	{
		mkdir(strFolder);
	}
	SetDlgItemText(IDC_EDIT_PATH,strFolder);
	SetWindowText("同花顺逐笔成交数据分析");
	MoveWindow(0,0,580,120);
	GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	//GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CTESTDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTESTDlg::OnPaint() 
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
HCURSOR CTESTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTESTDlg::OnBnClickedOk()
{
	//IplImage *img = cvLoadImage("e:\\checkcode.bmp",0);
	//IplImage *img1 = cvLoadImage("c:\\checkcode.bmp",0);
	IplImage *dstXorBinary;
	if((dstXorBinary = cvLoadImage("e:\\checkcode.bmp", 0)) == 0)
		return;
	cvNamedWindow("img",1);
	cvShowImage("img",dstXorBinary);
	cvThreshold(dstXorBinary,dstXorBinary,130,255,CV_THRESH_BINARY);
	//IplImage *img1  = cvCreateImage(cvSize(80,20),IPL_DEPTH_8U,3);
	//cvThreshold(img,img,1,127,CV_THRESH_BINARY);
	cvSaveImage("c:\\test.bmp",dstXorBinary);

	cvShowImage("img",dstXorBinary);

}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTESTDlg::OnBnClickedButton1()
{

	int xxx = 0xCA;

	return;
	CString strInfo = "";
	vector<InfoStruct>::iterator it;
	for (it=m_vecInfoList.begin();it != m_vecInfoList.end();it++)
	{
		strInfo += it->strTime + "," + it->strPrice +","+it->strNum + "\r\n";
	}

	CFile file;
	file.Open("c:\\test.csv",CFile::modeCreate | CFile::modeWrite);
	file.Write((void *)(LPCTSTR)strInfo,strInfo.GetLength());
	file.Close();
// 	IplImage *m_Source_Pic = NULL;
// 	m_Source_Pic = cvLoadImage("D:\\test.bmp");
// 	IplImage * RedChannel   = cvCreateImage( cvGetSize(m_Source_Pic), 8, 1);
// 	IplImage * GreenChannel  = cvCreateImage( cvGetSize(m_Source_Pic), 8, 1);
// 	IplImage * BlueChannel = cvCreateImage( cvGetSize(m_Source_Pic), 8, 1);
// 	IplImage * alphaChannel  = cvCreateImage( cvGetSize(m_Source_Pic), 8, 1);
// 	
// 	cvSetImageCOI(m_Source_Pic,1); 
// 	cvCopy(m_Source_Pic,BlueChannel); //提取蓝色
// 	cvSetImageCOI(m_Source_Pic,2);
// 	cvCopy(m_Source_Pic,GreenChannel);  //提取绿色
// 	cvSetImageCOI(m_Source_Pic,3);
// 	cvCopy(m_Source_Pic,RedChannel); //提取红色
// 	cvNamedWindow("0");
// 	cvNamedWindow("1");
// 	cvNamedWindow("2");
// 	cvNamedWindow("3");
// 	cvNamedWindow("4");
// 	cvNamedWindow("5");
// 	cvShowImage("0",m_Source_Pic);
// 	cvShowImage("1",RedChannel);
// 	cvShowImage("2",GreenChannel);
// 	cvShowImage("3",BlueChannel);
// 	cvAdd(RedChannel,GreenChannel,BlueChannel);
// 	cvShowImage("4",BlueChannel);
// 	cvThreshold(BlueChannel,BlueChannel,180,255,CV_THRESH_BINARY);
// 	//cvSmooth(BlueChannel,BlueChannel,CV_MEDIAN,1);
// 	cvShowImage("5",BlueChannel);
// 	cvSaveImage("c:\\test.bmp",BlueChannel);
	
}
BOOL CTESTDlg::PreTranslateMessage( MSG* pMsg )
{
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;      //屏蔽掉Esc键
	}
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		return TRUE;      //屏蔽掉Enter键
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CTESTDlg::OnBnClickedButton2()
{
	int iRet = MessageBox("执行此操作将清空之前的分析结果,继续吗?","提示",MB_ICONQUESTION|MB_YESNO);
	if (iRet != IDYES)
	{
		return;
	}
	CString strFilePath = "";
	char szFilter[]="All Files(*.*)|*.*|Png Files(*.png)|*.png|Jpeg Files(*.jpg)|*.jpg|Bmp Files(*.bmp)|*.bmp";
	CFileDialog dlg(1,NULL,NULL,NULL,szFilter); 
	if(dlg.DoModal()==IDOK) 
	{
		SetDlgItemText(IDC_EDIT_INFOSHOW,"");
		gl_iWidth = 0;
		m_mapInfoStruct.clear();
		m_vecInfoList.clear();
		CaculatePic(dlg.GetPathName());
		if (m_vecInfoList.size()>0)
		{
			SetDlgItemText(IDC_EDIT_INFOSHOW,"成功");
		}
		else
		{
			SetDlgItemText(IDC_EDIT_INFOSHOW,"失败");
		}
	}	
	
}


int gl_iHeight = 0;
// IplImage* gray = NULL;
// IplImage *dstPic = NULL;
// CvMat *dst;
void CTESTDlg::CaculatePic(CString strPath)
{
	IplImage *pImage = NULL;
	pImage = cvLoadImage(strPath);
	if (!pImage)
	{
		return;
	}
	int iY = 0;
	CArray<int,int> iPtXlist;
	GetLocation(pImage,iY,iPtXlist);

	if (iPtXlist.GetCount() < 2)
	{
		return;
	}
	gl_iWidth = 0;
	IplImage* gray = NULL;
	IplImage *dstPic = NULL;
	CvMat *dst = NULL;

	gl_iWidth = iPtXlist.GetAt(1) - iPtXlist.GetAt(0) - 1;
	gl_iHeight = pImage->height - iY;
	gray=cvCreateImage(cvSize(gl_iWidth,gl_iHeight),IPL_DEPTH_8U,1);
	dst=cvCreateMat(gl_iWidth,gl_iHeight,CV_8UC3);	
	dstPic=cvCreateImage(cvSize(gl_iWidth,gl_iHeight),IPL_DEPTH_8U,3);

	int width = gl_iWidth;
	int height = gl_iHeight;

	CvRect subRect;
	vector<CString> vecTypeList;
	CString strShow = "";
	for(int i=0;i<iPtXlist.GetCount();i++)
	{
		Sleep(9);
		subRect = cvRect(iPtXlist.GetAt(i)+1,iY,width,height);	
		cvGetSubRect(pImage,dst,subRect);	
		cvGetImage(dst,dstPic);

		//已经得到图像
		//消除虚线干扰
		CArray<CString,CString> strArrayType;
		//GetPutTypeAndClearNoise(dstPic,strArrayType);
		GetPutTypeAndClearNoiseEx(dstPic,strArrayType);
		cvCvtColor(dstPic,gray,CV_BGR2GRAY); 
		cvThreshold(gray,gray,100,255,CV_THRESH_BINARY_INV);
		CString strDst = "";
		strDst.Format("%stemp%d.bmp",gl_strAppPath,i);
		cvSaveImage(strDst,gray);
		
		
		//进行识别,得到识别结果
		char *result = NULL;
		result = OCR((LPSTR)(LPCTSTR)strDst,-1);		
		strShow.Format("%s",result);
		//将结果校正
		strShow = ModifyOcrResult(strShow);
		CArray<CString,CString> strArray;
		if (i == 9)
		{
			int iiii = 0;
		}
		StringSplit(strShow+"\n",strArray,'\n');
		InfoStruct infoStr;
		CString strKey = "";
		CString strTemp = "";
		int iCountType = strArrayType.GetCount();
		int iCountData = strArray.GetCount();
		if ( iCountType!=iCountData)
		{
			continue;
		}
		CString strTempEx = "";
		for (int j=0;j<strArray.GetCount();j++)
		{
			CArray<CString,CString> strArrayEx;
			strTemp = strArray.GetAt(j);
			
			strTemp.Replace(" ","");
			StringSplit(strTemp+",",strArrayEx,',');
			if (strArrayEx.GetCount() < 3)
			{
				continue;
			}			
			infoStr.strTime = strArrayEx.GetAt(0);
			infoStr.strPrice = strArrayEx.GetAt(1);
			infoStr.strNum = "";
			for (int k=2;k<strArrayEx.GetCount();k++)
			{
				infoStr.strNum += strArrayEx.GetAt(k);
			}
			
			infoStr.strTime.Trim();
			infoStr.strPrice.Trim();
			infoStr.strNum.Trim();
			infoStr.strType = strArrayType.GetAt(j);
			if ( infoStr.strTime.GetLength() > 4 )
			{
				strKey = infoStr.strTime + "1";
				gl_strLastTime = infoStr.strTime;
			}
			else
			{
				strKey = gl_strLastTime + infoStr.strTime;
			}
			AddToMap(strKey,infoStr);
		}
	}
	cvReleaseMat(&dst);
	cvReleaseImage(&gray);
	cvReleaseImage(&pImage);
}

int CTESTDlg::StringSplit(CString srcStr,CArray<CString,CString> &strArray,char splitChar)
{
	try
	{
		int idx1 = 0;
		int idx2 = 0;
		int iCount = 0;
		idx2 = srcStr.Find(splitChar, idx1);  //找第一个记录
		CString strTemp = "";
		while(idx2 != -1)						//如果找到
		{ 
			strTemp = srcStr.Mid(idx1,idx2 - idx1);
			if (strTemp != "")
			{
				strArray.InsertAt(strArray.GetSize(),strTemp); //取到一个记录
			}			
			idx1 = idx2 + 1; 
			idx2 = srcStr.Find(splitChar, idx1);  //找下一个记录
			iCount++;
		} 
		return iCount;
	}
	catch (...)
	{
		
	}
	return 0;
}
void CTESTDlg::AddToMap(CString strKey,InfoStruct &infoValue)
{
	if (m_mapInfoStruct.find(strKey) != m_mapInfoStruct.end())	//已经存在
	{
		return;
	}
	m_mapInfoStruct[strKey] = infoValue;
	m_vecInfoList.push_back(infoValue);
}
CString CTESTDlg::ModifyOcrResult(CString strResult)
{
	strResult.Trim();	//去掉两边的空格
	strResult.Replace("l","1");
	strResult.Replace("y",",");
	strResult.Replace("O","0");
	strResult.Replace("o","0");
	strResult.Replace("\r","");
	return strResult;
}

void CTESTDlg::GetPutTypeAndClearNoiseEx(IplImage *pImage,CArray<CString,CString> &arrayTypeList)
{
	arrayTypeList.RemoveAll();
	//此处需要将箭头方向提取出来
	bool bFind = false;
	int iRight = 0;
	CString strTemp = "";
	for (int i=0;i< 21;i++)
	{
		CvScalar tempColor = cvGet2D(pImage,i,0);
		if (tempColor.val[2] > 100 && tempColor.val[0] < 100)
		{
			cvDrawLine(pImage,cvPoint(0,i),cvPoint(pImage->width,i),cvScalar(0,0,0));
		}
	}

	for (int i=pImage->width-1;i>pImage->width-20;i--)
	{
		if (bFind)
		{
			break;
		}
		for (int j=1;j<20;j++)
		{
			CvScalar tempColor = cvGet2D(pImage,j,i);
			//CvScalar tempColor1 = cvGet2D(pImage,j+1,i);
			//&& !(tempColor1.val[1] == 0x30 && tempColor1.val[0] == 0x40)
			if ((tempColor.val[0] > 135 || tempColor.val[1] > 135 || tempColor.val[2] > 135))
			{
				//如果两点颜色一致,则不为黑色即可
				iRight = i;
				bFind = true;
				break;
			}			
			//提取颜色
		}
	}
	if (!bFind)
	{
		return;
	}
	CvFont font;
	cvInitFont(&font, CV_FONT_VECTOR0,1, 1, 0, 1, 8);

	//先取得最右边的像素坐标
	for (int i=0;i<pImage->height-1;i++)
	{
		CvScalar tempColor = cvGet2D(pImage,i,0);
		if (tempColor.val[2] > 100 && tempColor.val[0] < 100)
		{
			cvDrawLine(pImage,cvPoint(0,i),cvPoint(pImage->width,i),cvScalar(0,0,0));
		}

		CvScalar tempColor1 = cvGet2D(pImage,i,iRight);
		if ( tempColor1.val[0] > 135 || tempColor1.val[1] > 135 || tempColor1.val[2] > 135 )
		{
			//如果两点颜色一致,则不为黑色即可
			if (tempColor1.val[0] > 135)
			{
				strTemp = "0";
			}
			else if (tempColor1.val[2] >  135)	//红色 
			{
				strTemp = "1";//↑↓-
			}
			else if (tempColor1.val[1] >  135)
			{
				strTemp = "2";
			}
			//在两个地方画逗号
			cvPutText(pImage, "," , cvPoint(55, i), &font, CV_RGB(255,255,255));
			cvPutText(pImage, "," , cvPoint(105, i), &font, CV_RGB(255,255,255));
			arrayTypeList.InsertAt(arrayTypeList.GetSize(),strTemp);
			i++;
		}
	}
	cvDrawLine(pImage,cvPoint(0,pImage->height-1),cvPoint(pImage->width,pImage->height-1),cvScalar(0,0,0));
}
//加载源图像并进行预处理
void CTESTDlg::LoadSoucePicAndPreCal(CString strPath)
{
	gl_pImageSouce = NULL;
	gl_pImageSouce = cvLoadImage(strPath);
	if (gl_pImageSouce == NULL)
	{
		return;
	}
	//图像加载后,去除红色的分界线

	//找到最左边的红色分隔线下的一个像素Y坐标
	//从此坐标向右走,找到第一个红色的坐标左边的

}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTESTDlg::GetLocation(IplImage* Img,int &ptY,CArray<int,int> &iPtXlist)
{
	int iWidth = Img->width;
	int iHeight = Img->height;
	int iTempY = 0;
	int tempY = 0;

	for (int iY=0;iY<iHeight;iY++)
	{
		CvScalar tempColor = cvGet2D(Img,iY,0);
		if (tempColor.val[2] > 100)
		{
			iTempY = iY;
			break;
		}		
	}
	if (iTempY == 0)
	{
		iTempY = 23;
	}
	int iY = 0;
	for (iY=iTempY+1;iY<iHeight;iY++)
	{
		CvScalar tempColor = cvGet2D(Img,iY,0);
		if (tempColor.val[2] < 100 || ( tempColor.val[2] > 100 && tempColor.val[0] < 100 ) )
		{
			break;
		}
	}
	ptY = iHeight -1;	
	int iX = 0;
	iPtXlist.Add(0);
	for (iX =1;iX<iWidth;iX++ )
	{
		CvScalar tempColor = cvGet2D(Img,ptY,iX);
		if (tempColor.val[2] > 100 && tempColor.val[0] < 100)
		{
			iPtXlist.Add(iX);
		}
	}
// 	for (iX=1;iX<iWidth;iX++)
// 	{
// 		CvScalar tempColor = cvGet2D(Img,ptY,iX);
// 		if (tempColor.val[2] > 100)
// 		{
// 			x2 = iX;
// 			break;
// 		}
// 	}
// 
// 	for (iX=x2+1;iX<iWidth;iX++)
// 	{
// 		CvScalar tempColor = cvGet2D(Img,ptY,iX);
// 		if (tempColor.val[2] > 100)
// 		{
// 			x3 = iX;
// 			break;
// 		}
// 	}
// 
// 	for (iX=x3 + 1;iX<iWidth;iX++)
// 	{
// 		CvScalar tempColor = cvGet2D(Img,ptY,iX);
// 		if (tempColor.val[2] > 100)
// 		{
// 			x4 = iX;
// 			break;
// 		}
// 	}
// 
// 	for (iX=x4 + 1;iX<iWidth;iX++)
// 	{
// 		CvScalar tempColor = cvGet2D(Img,ptY,iX);
// 		if (tempColor.val[2] > 100)
// 		{
// 			x5 = iX;
// 			break;
// 		}
// 	}
// 
// 	for (iX=x5 + 1;iX<iWidth;iX++)
// 	{
// 		CvScalar tempColor = cvGet2D(Img,ptY,iX);
// 		if (tempColor.val[2] > 100)
// 		{
// 			x6 = iX;
// 			break;
// 		}
// 	}
// 	int iiii = 0;
	ptY = iY;
}

void CTESTDlg::OnBnClickedBtnStart()
{
	m_bExit = false;
	m_mapInfoStruct.clear();
	m_vecInfoList.clear();
	gl_iWidth = 0;
	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_EXPORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
	AfxBeginThread((AFX_THREADPROC)ThreadProc, (LPVOID)this);
}

void CTESTDlg::OnBnClickedBtnStop()
{
	m_bExit = true;
}
CString CTESTDlg::GetExcelDriver()
{
	char szBuf[2001];
	WORD cbBufMax = 2000;
	WORD cbBufOut;
	char *pszBuf = szBuf;
	CString sDriver;

	// 获取已安装驱动的名称(涵数在odbcinst.h里)
	if (!SQLGetInstalledDrivers(szBuf, cbBufMax, &cbBufOut))
		return "";

	// 检索已安装的驱动是否有Excel...
	do
	{
		if (strstr(pszBuf, "Excel") != 0)
		{
			//发现 !
			sDriver = CString(pszBuf);
			break;
		}
		pszBuf = strchr(pszBuf, '\0') + 1;
	}
	while (pszBuf[1] != '\0');

	return sDriver;
}


void CTESTDlg::OnBnClickedBtnExport()
{
	SaveData("");
}

void CTESTDlg::OnBnClickedBtnFind()
{
	CString strPath = "";
	BROWSEINFO bInfo;
	ZeroMemory(&bInfo, sizeof(bInfo));
	bInfo.hwndOwner = m_hWnd;
	bInfo.lpszTitle = _T("请选择路径: ");
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS;
	

	LPITEMIDLIST lpDlist; //用来保存返回信息的IDList
	lpDlist = SHBrowseForFolder(&bInfo) ; //显示选择对话框
	if(lpDlist != NULL)  //用户按了确定按钮
	{
		TCHAR *chPath=new TCHAR[300]; //用来存储路径的字符串
		SHGetPathFromIDList(lpDlist, chPath);//把项目标识列表转化成字符串
		strPath = chPath; //将TCHAR类型的字符串转换为CString类型的字符串
		delete[] chPath;
	}
	GetDlgItem(IDC_EDIT_PATH)->SetWindowText(strPath);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTESTDlg::ThreadProc(LPVOID lpParam)
{
	CTESTDlg *pWnd = (CTESTDlg *)lpParam;
	pWnd->GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
	pWnd->GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
	pWnd->GetDlgItem(IDC_BTN_EXPORT)->EnableWindow(FALSE);
	pWnd->GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	pWnd->GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
	pWnd->OperateProc();
	pWnd->GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
	pWnd->GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	pWnd->GetDlgItem(IDC_BTN_EXPORT)->EnableWindow(TRUE);
	pWnd->GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
	pWnd->GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
}

void CTESTDlg::OperateProc()
{
	CString strFolder = "";
	GetDlgItemText(IDC_EDIT_PATH,strFolder);
	CString strShow = "";
	CString strSavePath = gl_strAppPath + "导出路径";
	if (_access(strSavePath,0) != 0)
	{
		mkdir(strSavePath);
	}
	//遍历子文件，进行识别匹配
	while(!m_bExit)
	{
		int count = 0;
		CFileFind finder;
		BOOL working = finder.FindFile(strFolder + "\\*.*");
		CString strTemp = "";
		while (working)
		{
			working = finder.FindNextFile();
			if (!finder.IsDirectory())
				continue;
			if (finder.IsDots())
				continue;
			//找到期文件名中第一个数字
			
			CaculateFile(strFolder + "\\" + finder.GetFileName(),finder.GetFileName(),strSavePath);
			count++;
		}
		break;;
	}
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
//获得默认的文件名
BOOL CTESTDlg::GetDefaultXlsFileName(CString &sExcelFile)
{
	///默认文件名：yyyymmddhhmmss.xls
	sExcelFile = "";
	CString timeStr = "";
	timeStr = "DataDetail_" + CTime::GetCurrentTime().Format("%Y%m%d%H%M%S");
	sExcelFile =  timeStr + ".xls";
	// prompt the user (with all document templates)
	CFileDialog dlgFile(FALSE,".xls",sExcelFile);
	CString title;
	CString strFilter;

	title = "导出";
	strFilter = "Excel文件(*.xls)";
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.xls");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;
	dlgFile.m_ofn.nFilterIndex = 1;
	// append the "*.*" all files filter
	CString allFilter;
	VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.*");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;

	if (dlgFile.DoModal()==IDCANCEL)
	{
		return FALSE; // open cancelled
	}	
	sExcelFile.ReleaseBuffer();
	sExcelFile = dlgFile.GetPathName();
	sExcelFile.MakeLower();
	if (sExcelFile.Right(4) != ".xls")
	{
		sExcelFile+=".xls";
	}
	if (MakeSurePathExists(sExcelFile,true)) 
	{
		if(!DeleteFile(sExcelFile)) 
		{    // delete the file
			AfxMessageBox("覆盖文件时出错！");
			return FALSE;
		}
	}
	return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CTESTDlg::MakeSurePathExists(CString &Path,bool FilenameIncluded)
{
	int Pos=0;
	while((Pos=Path.Find('\\',Pos+1))!=-1)
	{
		CreateDirectory(Path.Left(Pos),NULL);
	}
	if(!FilenameIncluded)
	{
		CreateDirectory(Path,NULL);
	}
	return !_access(Path,0);
}

int CTESTDlg::CountDirectory(CString path,map<int,CString> &mapFileList)
{
	int count = 0;
	CFileFind finder;
	BOOL working = finder.FindFile(path + "\\*.png");
	CString strTemp = "";
	mapFileList.clear();
	while (working)
	{
		working = finder.FindNextFile();
		if (finder.IsDots())
			continue;
		if (finder.IsDirectory())
			continue;
		//找到期文件名中第一个数字
		strTemp = GetKeyString(finder.GetFileName(),"Frame",".");
		mapFileList[atoi(strTemp)] = finder.GetFileName();
		count++;
	}
	return count;
}

CString CTESTDlg::GetKeyString(CString srcStr,CString strBefore,CString strAfter)
{
	int iPosBefore = 0;
	int iPosAfter = 0;
	iPosBefore = srcStr.Find(strBefore) + strBefore.GetLength();
	iPosAfter = srcStr.Find(strAfter,iPosBefore);
	if (iPosBefore < 0 || iPosAfter < 0 || iPosAfter - iPosBefore < 0 )
	{
		return "";
	}
	else
	{
		return srcStr.Mid(iPosBefore,iPosAfter - iPosBefore);
	}
}

void CTESTDlg::CaculateFile(CString strFoldername,CString strSaveName,CString strParantFolder)
{
	m_mapInfoStruct.clear();
	m_vecInfoList.clear();
	gl_iWidth = 0;
	map<int,CString> mapFileList;
	gl_iFileTotalCount = CountDirectory(strFoldername,mapFileList);
	gl_iFileTotalCount = mapFileList.size();
	if (gl_iFileTotalCount < 1)
	{
		return;
	}
	map<int,CString>::iterator it;
	//CString strTest = "";
	int iCount = 0;
	CString strShow = "";
	for (it = mapFileList.begin();it != mapFileList.end();it++)
	{
		if (m_bExit)
		{
			break;
		}
		strShow.Format("%d / %d",iCount,gl_iFileTotalCount);
		SetDlgItemText(IDC_EDIT_INFOSHOW,strShow);
		//strTest += it->second + "\r\n";
		CaculatePic(strFoldername   +   "\\" + it->second);		
		iCount ++;
		strShow.Format("%d / %d",iCount,gl_iFileTotalCount);
		SetDlgItemText(IDC_EDIT_INFOSHOW,strShow);
		Sleep(10);
	}
	SaveData(strParantFolder + "\\" + strSaveName + ".xls");
// 	CFile file;
// 	file.Open("c:\\sssss.txt",CFile::modeCreate|CFile::modeWrite);
// 	file.Write((LPCTSTR)strTest,strTest.GetLength());
// 	file.Close();
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//返回应用程序所在目录，包含"\"
int CTESTDlg::GetAppPath(CString &strAppPath)
{
	int iRet = -3;
	try
	{
		TCHAR szBuff[_MAX_PATH];
		VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szBuff, _MAX_PATH));
		strAppPath=szBuff;
		int pos=strAppPath.ReverseFind('\\');
		strAppPath=strAppPath.Left(pos+1);		
		iRet = 0;
	}
	catch (...)
	{
	}
	return iRet;	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CTESTDlg::GetPower()
{
		return 0;
	CInternetSession session;
	CString strTemp = "";
	try
	{
		CStdioFile *pFile = session.OpenURL("http://blog.163.com/shaojun_hnist/blog/static/1387171242011115387766/");
		if (pFile != NULL)
		{
			while(pFile->ReadString(strTemp))
			{
				strTemp.MakeLower();
				if (strTemp.Find("myownpowertonghuashun") !=-1)
				{
					pFile->Close();
					delete pFile;
					pFile = NULL;
					return 0;
				}
			}

			if (pFile == NULL)
			{
				pFile->Close();
				delete pFile;
				pFile = NULL;
			}
		}
	}
	catch (...)
	{
	}
	return -1;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//获取游戏图片数据
void CTESTDlg::GetSelectWindowPic(HWND hwnd,CString strPath)
{
	if (_access(strPath,0) == 0)
	{
		CFile::Remove(strPath);
	}
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

	SaveNewPicture(strPath,pDst,bfh.bfOffBits+bmp.bmWidthBytes * bmp.bmHeight);
	delete []p;
	delete []pDst;
	memDC.SelectObject(oldmemBitmap);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//位图方式保存图片的部分区域
bool CTESTDlg::SaveNewPicture(CString filePath,LPBYTE m_newBmpData,long filelength)
{
	CFile file;
	if( !file.Open( filePath, CFile::modeWrite|CFile::modeCreate))
		return false;
	file.Write(m_newBmpData,filelength);
	return true;
}

void CTESTDlg::OnBnClickedButton3()
{
	CPoint pt(600,450);
	HWND hwnd = ::WindowFromPoint(pt);
	char szClass[256] ={0};
	::GetWindowText(hwnd,szClass,250);
	CString strTemp = CString(szClass);
	CString strPath = gl_strAppPath +"\\srcPic.bmp";
	if (strTemp.Find("逐笔成交--")>=0)
	{
		SetDlgItemText(IDC_EDIT_INFOSHOW,"");
		GetSelectWindowPic(hwnd,strPath);
		gl_iWidth = 0;
		m_mapInfoStruct.clear();
		m_vecInfoList.clear();
		CaculatePic(strPath);
		if(m_vecInfoList.size()>0)
		{
			SetDlgItemText(IDC_EDIT_INFOSHOW,"成功");
		}
	}
	else
	{
		AfxMessageBox("请打开同花顺逐笔明细,保证同花顺窗体没被其他窗体挡住.");
	}
}

void CTESTDlg::OnBnClickedBtnStop2()
{
	//清空旧数据
}

void CTESTDlg::SaveData(CString strXlsPath)
{
	if ( _access(strXlsPath,0) == 0 )
	{
		CFile::Remove(strXlsPath);
	}
	if (m_vecInfoList.size() > 0)
	{
		if (strXlsPath == "")
		{
			if (!GetDefaultXlsFileName(strXlsPath))
			{
				AfxMessageBox("覆盖文件时出错");
				return;
			}
		}		

		CDatabase database;
		CString sDriver;
		CString sExcelFile = strXlsPath; 
		CString sSql;
		CString tableName = "dddd";

		// 检索是否安装有Excel驱动 "Microsoft Excel Driver (*.xls)" 
		sDriver = GetExcelDriver();
		if (sDriver.IsEmpty())
		{
			// 没有发现Excel驱动
			AfxMessageBox("请先安装Excel软件才能使用导出功能");
			return;
		}

		// 创建进行存取的字符串
		sSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, sExcelFile);
		CString strDay = "";
		CString strSucCount = "";

		// 创建数据库 (既Excel表格文件)
		if( database.OpenEx(sSql,CDatabase::noOdbcDialog) )
		{
			CString strTemp;
			strTemp.Format("时间 TEXT,价格 DOUBLE,笔数 INT,类型 INT");
			// 创建表结构			
			sSql = "CREATE TABLE " + tableName + " ( " + strTemp +  " ) ";
			int i = 0;
			database.ExecuteSQL(sSql);		
		}
		else
		{
			AfxMessageBox("创建EXCEL文件出错,导出未完成");
			return;
		}
		CString strInfo = "";
		vector<InfoStruct>::iterator it;
		int iCount = 0;
		for (it=m_vecInfoList.begin();it != m_vecInfoList.end();it++)
		{
			sSql.Format("INSERT INTO %s (时间,价格,笔数,类型)values('%s',%s,%s,%s)",
				tableName,
				it->strTime,
				it->strPrice,
				it->strNum,
				it->strType
				);
			database.ExecuteSQL(sSql);
		}
	}
}