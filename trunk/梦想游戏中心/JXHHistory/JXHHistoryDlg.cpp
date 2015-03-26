// JXHHistoryDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "JXHHistory.h"
#include "JXHHistoryDlg.h"
#include "ListCtrlColore.h"
#include ".\jxhhistorydlg.h"
#include "ADOFile\AdoRecordSet.h"
#include "UserFile.h"
#include <io.h>
#include "datatype.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <afxmt.h>
#include <afxtempl.h>
#include "afxinet.h"
using namespace std;


CListCtrl g_ShowResult;		//历史显示列表
CListCtrlColore g_ShowMsg;			//消息显示列表
BOOL g_bExitFlag = TRUE;			//线程运行控制
CCriticalSection g_ShowLock;		//消息显示锁
CString g_dbServer = "jxhrecord.mdb";//数据库
CString g_dbPwd = "";				//数据库密码
CString g_strCurrRecord = "";		//当前输赢结果
CString g_strCurrRecordList = "";	//当前历史记录链
int g_iLastRecordID = 0;			//最新的记录ID
CString g_strAppPath = "";
CString g_srcPicPath = "srcpic.bmp";
CString g_dstPicPath11 = "dstpic11.bmp";
CString g_dstPicPath12 = "dstpic12.bmp";
CString g_dstPicPath13 = "dstpic13.bmp";
CString g_cmpPicPath1 = "cmppic1.bmp";
CString g_srcPicPath2 = "dstpic2.bmp";
CString g_cmpPicPath2 = "olddstpic2.bmp";
CCriticalSection g_QueryFlagLock;		//消息显示锁
int g_iQueryFlag = 0;
int g_iAutoQueryFlag = 0;

int g_iQuerySleep = 1000;		//匹配线程暂停间隔
int g_iGotSleep = 6000;	//获取线程在获取到数据时暂停
int g_iGetWaitSleep = 3000;	//获取线程等待间隔

CString g_strClassName = "";
CString g_strWindowName = "";

int g_iMinNum = 0;
void ShowMessageEx(CString strMsg);
void ShowMessage(CString strMsg)
{
	g_ShowLock.Lock();
	ShowMessageEx(strMsg);
	g_ShowLock.Unlock();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void ShowMessageEx(CString strMsg)
{
	CString msgBuf = strMsg;
	int nCount = g_ShowMsg.GetItemCount();
	if ( nCount >= 100 )
	{
		for (int i=0;i<90;i++)
		{
			g_ShowMsg.DeleteItem(0);
		}
	}
	nCount = g_ShowMsg.GetItemCount();
	CString strShow  = "";
	if (nCount > 0)
	{
		strShow= g_ShowMsg.GetItemText(nCount-1,1);
	}	
	if (msgBuf == strShow)
	{
		g_ShowMsg.SetItemText( nCount-1 , 0, CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S") );
		::PostMessage(g_ShowMsg.m_hWnd,WM_VSCROLL,SB_BOTTOM,0);
		return;
	}
	g_ShowMsg.InsertItem( nCount, "");
	if ( nCount % 2 == 0 )
	{
		g_ShowMsg.SetRowSelectedBgColor(nCount, RGB(0,0,0) , false);
		g_ShowMsg.SetRowTxtColor(nCount , RGB(255,0,0) , false);
	}
	else
	{
		g_ShowMsg.SetRowTxtColor(nCount , RGB(0,0,255) , false);
	}
	g_ShowMsg.SetRowBgColor(nCount, RGB(230,190,220), false);
	g_ShowMsg.SetRowStyle(nCount,  LIS_TXTCOLOR | LIS_BGCOLOR);

	g_ShowMsg.SetItemText( nCount , 0, CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S") );
	g_ShowMsg.SetItemText( nCount , 1, msgBuf );
	::PostMessage(g_ShowMsg.m_hWnd,WM_VSCROLL,SB_BOTTOM,0);
	if (msgBuf != "等待中...")
	{
		//	WriteLog(msgBuf,2);
	}
}
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


// CJXHHistoryDlg 对话框



CJXHHistoryDlg::CJXHHistoryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJXHHistoryDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CJXHHistoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ShowResult, g_ShowResult);
	DDX_Control(pDX, IDC_LIST_MSG, g_ShowMsg);
}

BEGIN_MESSAGE_MAP(CJXHHistoryDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_BEGIN, OnBnClickedBtnBegin)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_QUERY, OnBnClickedBtnQuery)
	ON_BN_CLICKED(IDC_CHECK_AUTOQUERY, OnBnClickedCheckAutoquery)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_SHOWALL, OnBnClickedBtnShowall)
	ON_COMMAND(IDR_DELETETHIS, OnDeletethis)
	ON_COMMAND(IDR_DELETEALL, OnDeleteall)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_ShowResult, OnNMRclickListShowresult)
END_MESSAGE_MAP()


// CJXHHistoryDlg 消息处理程序

BOOL CJXHHistoryDlg::OnInitDialog()
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
	
#ifndef DEBUG
	if (GetPower() != 0)
	{
		exit(0);
	}
#endif
	::CoInitialize(NULL);
	//////////////////////////////////////////////////////////////////////////
	CUserFile *pFile = CUserFile::GetInstance();
	pFile->GetAppPath(g_strAppPath);
	InitParams();
	g_dbServer = g_strAppPath + g_dbServer;
	//源图像
	g_srcPicPath = g_strAppPath + g_srcPicPath;

	//三个输赢判断图像
	g_dstPicPath11 = g_strAppPath + g_dstPicPath11;
	g_dstPicPath12 = g_strAppPath + g_dstPicPath12;
	g_dstPicPath13 = g_strAppPath + g_dstPicPath13;
	
	//对比图像
	g_cmpPicPath1 = g_strAppPath + g_cmpPicPath1;
	
	//数字图像
	g_srcPicPath2 = g_strAppPath + g_srcPicPath2;	
	g_cmpPicPath2 = g_strAppPath + g_cmpPicPath2;

	SetTableStyle();
	InitListCtrl();
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	SetDlgItemInt(IDC_EDIT_GOTWAIT,g_iGotSleep);
	SetDlgItemInt(IDC_EDIT_GETWAIT,g_iGetWaitSleep);
	SetDlgItemInt(IDC_EDIT_MINNUM,g_iMinNum);
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CJXHHistoryDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CJXHHistoryDlg::OnPaint() 
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
HCURSOR CJXHHistoryDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CJXHHistoryDlg::SetTableStyle()
{
	//CListCtrl* conList = &g_ShowResult;
	DWORD dwSytle=::GetWindowLong(g_ShowResult.m_hWnd,GWL_STYLE);
	// 设置为报表形式
	SetWindowLong(g_ShowResult.m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=g_ShowResult.GetExtendedStyle();
	g_ShowResult.SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CImageList image;
	image.Create(1,16, ILC_COLOR, 0, 0);
	g_ShowResult.SetImageList(&image, LVSIL_STATE);
	g_ShowResult.MoveWindow(3,3,725,230);
	g_ShowResult.InsertColumn(0,"序号",LVCFMT_CENTER,40);
	g_ShowResult.InsertColumn(1,"将",LVCFMT_LEFT,30);
	g_ShowResult.InsertColumn(2,"相",LVCFMT_LEFT,30);
	g_ShowResult.InsertColumn(3,"和",LVCFMT_LEFT,30);
	//g_ShowResult.InsertColumn(2,"完整段",LVCFMT_LEFT,50);
	g_ShowResult.InsertColumn(4,"详情",LVCFMT_LEFT,550);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CJXHHistoryDlg::InitListCtrl()
{
	DWORD dwSytle=::GetWindowLong(g_ShowMsg.m_hWnd,GWL_STYLE);
	// 设置为报表形式
	SetWindowLong(g_ShowMsg.m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=g_ShowMsg.GetExtendedStyle();
	g_ShowMsg.SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	::SendMessage(g_ShowMsg.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	g_ShowMsg.MoveWindow(3,293,475,155);
	g_ShowMsg.InsertColumn(0,_T("时间"),LVCFMT_LEFT,125);
	g_ShowMsg.InsertColumn(1,_T("信息"),LVCFMT_LEFT,320);
}

/************************************************************************/
/*  获取图像线程                                                        */
/************************************************************************/
void CJXHHistoryDlg::ThreadFunc(LPVOID lParam)
{
	CJXHHistoryDlg *pParam = ((CJXHHistoryDlg *)lParam);
	if ( pParam )
	{
		ShowMessage("获取图像线程开始");
		::CoInitialize(NULL);
		pParam->GetFunc(); //执行线程函数体
		::CoUninitialize();
		
		ShowMessage("获取图像线程退出");
		pParam->GetDlgItem(IDC_BTN_BEGIN)->EnableWindow(TRUE);
		pParam->GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	}
}
/************************************************************************/
/*  获取图像线程函数                                                    */
/************************************************************************/
void CJXHHistoryDlg::GetFunc()
{
	while (!g_bExitFlag)
	{
		GetRecordPic();
		SplitPic();
		bool bRet = IsNeedToGetRecord();
		if (bRet)
		{
			SplitPicEx();
			g_strCurrRecord = "";
			g_strCurrRecord = GetCurrRecord();			
			SaveHistory();			
			Delay(g_iGotSleep,&g_bExitFlag);
		}
		else		//局数未变,不需要更新结果
		{
			ShowMessage("等待新一局的结果...");
			Delay(g_iGetWaitSleep,&g_bExitFlag);
		}		
	}
}

/************************************************************************/
/*  匹配历史线程                                                        */
/************************************************************************/
void CJXHHistoryDlg::ThreadQueryFunc(LPVOID lParam)
{
	CJXHHistoryDlg *pParam = ((CJXHHistoryDlg *)lParam);
	if ( pParam )
	{
		ShowMessage("匹配历史线程开始");
		::CoInitialize(NULL);
		pParam->QueryFunc(); //执行线程函数体
		::CoUninitialize();
		ShowMessage("匹配历史线程退出");
		pParam->GetDlgItem(IDC_BTN_BEGIN)->EnableWindow(TRUE);
		pParam->GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	}
}
/************************************************************************/
/*  匹配历史线程函数                                                        */
/************************************************************************/
void CJXHHistoryDlg::QueryFunc()
{
	while (!g_bExitFlag)
	{
		int iFlag = 0;
		g_QueryFlagLock.Lock();
		iFlag = g_iQueryFlag;
		g_iQueryFlag = 0;
		g_QueryFlagLock.Unlock();

		if (iFlag == 1)		//执行匹配操作
		{
			ShowMessage("自动匹配历史开始...");
			//执行匹配操作
			ShowRecords();
			ShowMessage("自动匹配历史结束");
		}
		Delay(500,&g_bExitFlag);
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CJXHHistoryDlg::OnBnClickedBtnBegin()
{
	AfxBeginThread((AFX_THREADPROC)ThreadFunc,(LPVOID)this);
	AfxBeginThread((AFX_THREADPROC)ThreadQueryFunc,(LPVOID)this);
	g_bExitFlag = false;
	GetDlgItem(IDC_BTN_BEGIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CJXHHistoryDlg::OnBnClickedBtnStop()
{
	g_bExitFlag = true;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CJXHHistoryDlg::OnBnClickedBtnQuery()
{
	ShowMessage("匹配历史开始...");
	//执行匹配操作
	g_iMinNum = GetDlgItemInt(IDC_EDIT_MINNUM);
	ShowRecords();
	ShowMessage("匹配历史结束");
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CJXHHistoryDlg::OnBnClickedCheckAutoquery()
{
	g_iAutoQueryFlag = ((CButton*)GetDlgItem(IDC_CHECK_AUTOQUERY))->GetCheck();
	GetDlgItem(IDC_BTN_QUERY)->EnableWindow(g_iAutoQueryFlag==1?FALSE:TRUE);
	ShowMessage(g_iAutoQueryFlag==1?"开启自动匹配":"取消自动匹配");
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//保存历史函数
bool CJXHHistoryDlg::SaveHistory()
{
	CAdoConnection adoConn;
	CAdoRecordSet adoRst;
	try
	{
		if(!adoConn.ConnectAccess(g_dbServer,g_dbPwd))
		{
			ShowMessage("连接数据库失败");
			return false;	
		}
		adoRst.SetAdoConnection(&adoConn);
		adoRst.SetCursorLocation(adUseClient);
		CString strSQL = "";
		if (g_strCurrRecord == "0")		//是和	0和2将1相
		{
			if (g_strCurrRecordList == "")
			{
				ShowMessage("开始统计");
			}
			ShowMessage("此局和局");
			strSQL.Format("update hisrecord set isfull=1,detail=detail+'%s' where [id] = %d",g_strCurrRecord,g_iLastRecordID);
		}
		else
		{
			if (g_strCurrRecord == "1")
			{
				ShowMessage("此局相赢");
			}
			else
			{
				ShowMessage("此局将赢");
			}
			if (g_strCurrRecordList == "")	//初始统计
			{
				ShowMessage("开始统计");
				strSQL.Format("insert into hisrecord (isfull,rectime,detail) values(0,'%s','%s')",CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"),g_strCurrRecord);
			}
			else		//统计中
			{
				strSQL.Format("update hisrecord set detail=detail+'%s' where [id] = %d",g_strCurrRecord,g_iLastRecordID);
			}			
		}
		//更新数据库
		if ( !adoRst.Open(strSQL,adCmdText) )
		{
			ShowMessage(adoConn.GetLastErrorText());
			ShowMessage("添加历史失败");
			adoRst.Close();
			adoConn.Close();
			return false;
		}
		adoRst.Close();
		if (g_strCurrRecordList == "")//新增记录情况下,获取最新的ID值
		{
			strSQL.Format("select top 1 [id] from hisrecord order by [id] desc");
			if ( !adoRst.Open(strSQL,adCmdText))
			{
				ShowMessage(adoConn.GetLastErrorText());
				ShowMessage("获取最新ID失败");
				adoRst.Close();
				adoConn.Close();
				return false;
			}
			//获取最新ID
			adoRst.GetCollect("ID",g_iLastRecordID);
			CString strShow = "";
			strShow.Format("当前ID号:%d",g_iLastRecordID);
			ShowMessage(strShow);
			adoRst.Close();
		}
		g_strCurrRecordList += g_strCurrRecord;
		SetDlgItemText(IDC_EDIT_CURRRECORD,g_strCurrRecordList);
		//g_strCurrRecordList += g_strCurrRecord;//记录累加
		if (g_strCurrRecord == "0")
		{
			g_strCurrRecordList = "";
			g_iLastRecordID = 0;
		}
		else
		{
			//通知查询线程可以进行匹配历史了
			g_QueryFlagLock.Lock();			
			if (g_iAutoQueryFlag == 1)
			{
				g_iQueryFlag = 1;
			}
			else
			{
				g_iQueryFlag = 0;
			}
			g_QueryFlagLock.Unlock();
		}
		SetDlgItemText(IDC_EDIT_QUERYSTRING,g_strCurrRecordList);
		adoConn.Close();
		return true;
	}
	catch(...)
	{
		ShowMessage("保存历史异常");
	}
	adoRst.Close();
	adoConn.Close();
	return false;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//获取图像函数
void CJXHHistoryDlg::GetRecordPic()
{
	//查找窗口
	HWND hwnd = ::FindWindow(g_strClassName,g_strWindowName);
	if (!hwnd)
	{
		ShowMessage("查找窗口失败");
		return;
	}	
	hwnd = m_myWind.GetChildWindow(hwnd,3);
	if (m_myWind.GetContrlClassName(hwnd) != "#32770")
	{
		ShowMessage("查找子窗口失败");
		return;
	}
	//获取图像
	DeleteFile(g_srcPicPath);
	m_myImg.GetSelectWindowPic(hwnd,g_srcPicPath);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//判断是否需要更新历史记录
bool CJXHHistoryDlg::IsNeedToGetRecord()
{
	IplImage *Img = NULL;
	Img = cvLoadImage(g_srcPicPath2,0);
	cvThreshold(Img,Img,90,255,CV_THRESH_BINARY);
	for (int i=2;i<Img->width-2;i++)
	{
		CvScalar tempColor = cvGet2D(Img,9,i);
		if (tempColor.val[0] != 255)
		{
			cvReleaseImage(&Img);
			return false;
		}
	}
	ShowMessage("开始获取结果...");
	cvReleaseImage(&Img);
	return true;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//获取当前的赢家
CString CJXHHistoryDlg::GetCurrRecord()
{
	if (IsSelectResult(1))//将
	{
		return "2";				
	}
	else if (IsSelectResult(2))//相
	{
		return "1";
	}
	else if (IsSelectResult(3))//和
	{
		return "0";
	}
	return "";
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//判断是不是指定结果
bool CJXHHistoryDlg::IsSelectResult(int iFlag)
{
	IplImage *Img = NULL;
	CString strPicPath = "";
	switch(iFlag)
	{
	case 1:strPicPath = g_dstPicPath11;break;
	case 2:strPicPath = g_dstPicPath12;break;
	case 3:strPicPath = g_dstPicPath13;break;
	default:return false;
	}
	Img = cvLoadImage(strPicPath,0);
	if (Img == NULL)
	{
		ShowMessage("拆分图像时加载源图像失败");
		return false;
	}
	cvThreshold(Img,Img,90,255,CV_THRESH_BINARY);
	for (int i=7;i<Img->width-7;i++)
	{
		for (int j=7;j<Img->height-7;j++)
		{
			CvScalar tempColor = cvGet2D(Img,j,i);
			if (tempColor.val[0] != 0 || tempColor.val[1] != 0 || tempColor.val[2] != 0)
			{
				cvReleaseImage(&Img);
				return true;
			}
		}
	}
	cvReleaseImage(&Img);
	return false;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CJXHHistoryDlg::DeleteFile(CString strPath)
{
	if (_access(strPath,0) == 0)
	{
		CFile::Remove(strPath);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//针对源图像进行拆分,拆分成两个历史记录图像
void CJXHHistoryDlg::SplitPic()
{
	IplImage *Img = NULL;	
	Img = cvLoadImage(g_srcPicPath);
	if (Img == NULL)
	{
		ShowMessage("拆分图像时加载源图像失败");
		return;
	}
	SaveSubRectPic(Img,g_srcPicPath2,161,562,10,10);//保存数据图像
	cvReleaseImage(&Img);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//针对源图像进行拆分,拆分成两个历史记录图像
void CJXHHistoryDlg::SplitPicEx()
{
	IplImage *Img = NULL;	
	Img = cvLoadImage(g_srcPicPath);
	if (Img == NULL)
	{
		ShowMessage("拆分图像时加载源图像失败");
		return;
	}
	SaveSubRectPic(Img,g_dstPicPath11,635,615,20,20);//保存图像1
	SaveSubRectPic(Img,g_dstPicPath12,635,645,20,20);//保存图像2
	SaveSubRectPic(Img,g_dstPicPath13,635,675,20,20);//保存图像3
	cvReleaseImage(&Img);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//获取子区域的图像保存
void CJXHHistoryDlg::SaveSubRectPic(IplImage *srcImg,CString strPath,int x,int y,int iWidth,int iHeight)
{
	CvMat *dst;
	CvRect subRect;
	IplImage *dstImg;
	int iii = CV_8UC3;
	dst=cvCreateMat(iWidth,iHeight,CV_8UC3);
	subRect =cvRect(x,y,iWidth,iHeight);
	cvGetSubRect(srcImg,dst,subRect);
	int ii = IPL_DEPTH_8U;
	dstImg=cvCreateImage(cvSize(iWidth,iHeight),srcImg->depth,srcImg->nChannels);
	cvGetImage(dst,dstImg);
	cvSaveImage(strPath,dstImg);
	cvReleaseMat(&dst);
	cvReleaseImage(&dstImg);

}
void CJXHHistoryDlg::OnClose()
{	
	if (g_bExitFlag == false)
	{
		MessageBox("请先停止线程","提示");
		return;
	}
	CUserFile *pFile = CUserFile::GetInstance();
	if (pFile)
	{
		delete pFile;
		pFile = NULL;
	}
	CDialog::OnClose();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CJXHHistoryDlg::OnBnClickedButton1()
{
	SplitPic();
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//线程中控制延时的函数
void CJXHHistoryDlg::Delay(int iMiniSecond,BOOL *bExit)
{
	for (int i=0;i<iMiniSecond;i+=500)
	{
		if (*bExit)
		{
			break;
		}
		Sleep( 500 );
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CJXHHistoryDlg::OnBnClickedBtnExit()
{
	PostMessage(WM_CLOSE,0,0);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CJXHHistoryDlg::OnBnClickedBtnSave()
{
	g_iGotSleep = GetDlgItemInt(IDC_EDIT_GOTWAIT);
	g_iGetWaitSleep = GetDlgItemInt(IDC_EDIT_GETWAIT);
	//g_iQuerySleep = GetDlgItemInt(IDC_EDIT_QUERYWAIT);
	g_iMinNum = GetDlgItemInt(IDC_EDIT_MINNUM);
	CString strTemp = "";
	int iTemp = 0;
	CUserFile *pFile = NULL;
	pFile = CUserFile::GetInstance();
	if (pFile)
	{	
		int iRet = -1;
		CString strAppPath = g_strAppPath;
		strAppPath += MWGATEINITFILE;
		pFile->CheckInitValue(g_iGetWaitSleep,1000,1000000);
		pFile->CheckInitValue(g_iGotSleep,5000,1000000);
		pFile->CheckInitValue(g_iQuerySleep,1000,1000000);
		pFile->WriteInitInfo(strAppPath,"Time","getwait",strTemp,g_iGetWaitSleep,1);
		pFile->WriteInitInfo(strAppPath,"Time","gotwait",strTemp,g_iGotSleep,1);
		pFile->WriteInitInfo(strAppPath,"Time","querywait",strTemp,g_iQuerySleep,1);
		pFile->WriteInitInfo(strAppPath,"QueryNum","min",strTemp,g_iMinNum,1);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CJXHHistoryDlg::InitParams()
{
	CString strTemp = "";
	int iTemp = 0;
	CUserFile *pFile = NULL;
	pFile = CUserFile::GetInstance();
	if (pFile)
	{	
		int iRet = -1;
		CString strAppPath = g_strAppPath;
		strAppPath += MWGATEINITFILE;
		//数据库信息
		iRet = pFile->ReadInitInfo(strAppPath,"database","dbserver",g_dbServer,iTemp,0);
		if (iRet < 0)
		{
			g_dbServer = "jxhrecord.mdb";
		}
		iRet = pFile->ReadInitInfo(strAppPath,"database","dbpwd",g_dbPwd,iTemp,0);
		if (iRet < 0)
		{
			g_dbPwd = "";
		}
		iRet = pFile->ReadInitInfo(strAppPath,"window","class",g_strClassName,iTemp,0);
		if (iRet < 0)
		{
			g_strClassName = "ClientFrame_CMainFrame";
		}
		iRet = pFile->ReadInitInfo(strAppPath,"window","name",g_strWindowName,iTemp,0);
		if (iRet < 0)
		{
			g_strWindowName = "梦想游戏--将相和";
		}
		pFile->ReadInitInfo(strAppPath,"Time","getwait",strTemp,g_iGetWaitSleep,1);
		pFile->ReadInitInfo(strAppPath,"Time","gotwait",strTemp,g_iGotSleep,1);		
		pFile->ReadInitInfo(strAppPath,"Time","querywait",strTemp,g_iQuerySleep,1);
		pFile->ReadInitInfo(strAppPath,"QueryNum","min",strTemp,g_iMinNum,1);
		pFile->CheckInitValue(g_iMinNum,1,1000000);
		pFile->CheckInitValue(g_iGetWaitSleep,1000,1000000);
		pFile->CheckInitValue(g_iGotSleep,5000,1000000);
		pFile->CheckInitValue(g_iQuerySleep,1000,1000000);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CJXHHistoryDlg::QueryHistoryRecord(CString strQuery,CStringList &strResultList)
{
	CAdoConnection adoConn;
	CAdoRecordSet adoRst;
	try
	{
		if(!adoConn.ConnectAccess(g_dbServer,g_dbPwd))
		{
			ShowMessage("连接数据库失败");
			return -1;	
		}
		adoRst.SetAdoConnection(&adoConn);
		adoRst.SetCursorLocation(adUseClient);
		CString strSQL = "";
		strSQL.Format("select detail from hisrecord where [id] <>%d and detail like '%%%s%%'",g_iLastRecordID,strQuery);
		//更新数据库
		if ( !adoRst.Open(strSQL,adCmdText) )
		{
			ShowMessage(adoConn.GetLastErrorText());
			ShowMessage("匹配历史失败");
			adoRst.Close();
			adoConn.Close();
			return -1;
		}
		int iRecordCount = 0;
		CString strTemp = "";
		while(!adoRst.IsEOF())
		{
			strTemp = "";
			adoRst.GetCollect("detail",strTemp);
			if (strTemp != "0")
			{
				strResultList.AddTail(strTemp);
				iRecordCount ++;
			}
			adoRst.MoveNext();
		}
		adoRst.Close();
		adoConn.Close();
		return iRecordCount;
	}
	catch(...)
	{
		ShowMessage("匹配历史异常");
	}
	adoRst.Close();
	adoConn.Close();
	return -1;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CJXHHistoryDlg::GetPower()
{
	CInternetSession session;
	CString strTemp = "";
	try
	{
		CStdioFile *pFile = session.OpenURL("http://blog.csdn.net/TragicGuy/archive/2010/01/08/5157814.aspx");
		if (pFile != NULL)
		{
			while(pFile->ReadString(strTemp))
			{
				strTemp.MakeLower();
				if (strTemp.Find("myownpower111") !=-1)
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
void CJXHHistoryDlg::ShowRecords()
{
	CString strQuery = "";
	GetDlgItemText(IDC_EDIT_QUERYSTRING,strQuery);
	g_ShowResult.DeleteAllItems();
	SetDlgItemText(IDC_EDIT_FIRST,"0");
	SetDlgItemText(IDC_EDIT_SECOND,"0");
	SetDlgItemText(IDC_EDIT_THIRD,"0");
	if (strQuery == "")
	{
		ShowMessage("待匹配数据非法");
		return;
	}
	CStringList strRecords;
	while (true)
	{
		if(strQuery.GetLength() < g_iMinNum)
		{
			ShowMessage("等待匹配串长度小于最小配置数,退出");
			break;
		}
		strRecords.RemoveAll();
		int iRet = QueryHistoryRecord(strQuery,strRecords);
		if (iRet < 0)
		{
			break;
		}
		else if (iRet == 0)
		{
			if (strQuery.GetLength() < 1)
			{
				break;
			}
			strQuery = strQuery.Mid(1);
			SetDlgItemText(IDC_EDIT_QUERYSTRING,strQuery);
			continue;
		}
		else	//匹配到结果
		{
			CString strFormat = "";
			strFormat.Format("应用 \"%s\" 匹配到%d条记录",strQuery,iRet);
			ShowMessage(strFormat);
			break;
		}
	}
	//将结果显示
	CString strNum = "";
	for (POSITION Pos_ = strRecords.GetHeadPosition(); Pos_ != NULL;)
	{
		CString strResult = strRecords.GetNext(Pos_);
		int nCount = g_ShowResult.GetItemCount();
		strNum.Format("%d",nCount);
		g_ShowResult.InsertItem( nCount, strNum);
		g_ShowResult.SetItemText(nCount,4,strResult);
	}
	ComputeResult(strQuery);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CJXHHistoryDlg::ComputeResult(CString strDstString)
{
	CString strRecord = "";	
	int iTotalFirst = 0;
	int iTotalSecond = 0;
	int iTotalThird = 0;
	//char charTemp = 0;
	CString strTemp = "";
	int iLen = strDstString.GetLength();
	int iRecordCount = g_ShowResult.GetItemCount();	
	for (int i=0;i<iRecordCount;i++)
	{
		int iFirst = 0;
		int iSecond = 0;
		int iThird = 0;
		strRecord = g_ShowResult.GetItemText(i,4);
		int idx = -1;
		while (true)
		{
			idx = strRecord.Find(strDstString);
			if (idx == -1)
			{
				break;
			}
			if (strRecord.GetLength() <= idx + iLen)
			{
				break;
			}
			strTemp = strRecord.Mid(idx + strDstString.GetLength(),1);
			strRecord = strRecord.Right(strRecord.GetLength()-idx-1);
			switch (atoi(strTemp))
			{
			case 2:
				iFirst ++;
				iTotalFirst++;
				break;
			case 1:
				iSecond++;
				iTotalSecond++;
				break;
			case 0:
				iThird++;
				iTotalThird++;
				break;
			default: break;
			}			
		}
		//显示统计结果
		strTemp.Format("%d",iFirst);
		g_ShowResult.SetItemText(i,1,strTemp);
		strTemp.Format("%d",iSecond);
		g_ShowResult.SetItemText(i,2,strTemp);
		strTemp.Format("%d",iThird);
		g_ShowResult.SetItemText(i,3,strTemp);
	}
	//显示赢率
	int iTotalCount = iTotalFirst + iTotalSecond + iTotalThird;
	if (iTotalCount == 0)
	{
		SetDlgItemText(IDC_EDIT_FIRST,"0");
		SetDlgItemText(IDC_EDIT_SECOND,"0");
		SetDlgItemText(IDC_EDIT_THIRD,"0");
		return;
	}
	float fFirst = 0.00;
	float fSecond = 0.00;
	float fThird = 0.00;
	fFirst = float(iTotalFirst * 100)/float(iTotalCount);
	fSecond = float(iTotalSecond * 100)/float(iTotalCount);
	fThird = float(iTotalThird * 100)/float(iTotalCount);
	strTemp.Format("%g %%",fFirst);
	SetDlgItemText(IDC_EDIT_FIRST,strTemp);
	strTemp.Format("%g %%",fSecond);
	SetDlgItemText(IDC_EDIT_SECOND,strTemp);
	strTemp.Format("%g %%",fThird);
	SetDlgItemText(IDC_EDIT_THIRD,strTemp);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CJXHHistoryDlg::OnBnClickedBtnShowall()
{
	g_ShowResult.DeleteAllItems();
	CAdoConnection adoConn;
	CAdoRecordSet adoRst;	
	try
	{
		if(!adoConn.ConnectAccess(g_dbServer,g_dbPwd))
		{
			ShowMessage("连接数据库失败");
			return;	
		}
		adoRst.SetAdoConnection(&adoConn);
		adoRst.SetCursorLocation(adUseClient);
		CString strSQL = "";
		strSQL.Format("select [id],detail from hisrecord where [id] <> %d",g_iLastRecordID);
		//更新数据库
		if ( !adoRst.Open(strSQL,adCmdText) )
		{
			ShowMessage("查询历史失败");
			ShowMessage(adoConn.GetLastErrorText());
			adoRst.Close();
			adoConn.Close();
			return;
		}
		int iRecordCount = 0;
		CString strTemp = "";
		CString strNum = "";
		while(!adoRst.IsEOF())
		{
			strTemp = "";
			int iRecordID = 0;
			adoRst.GetCollect("id",iRecordID);
			adoRst.GetCollect("detail",strTemp);			
			if (strTemp != "0")
			{
				int nCount = g_ShowResult.GetItemCount();				
				strNum.Format("%d",iRecordID);
				g_ShowResult.InsertItem( nCount, strNum);
				g_ShowResult.SetItemText(nCount,4,strTemp);
				iRecordCount++;
			}
			adoRst.MoveNext();
		}
		adoRst.Close();
		adoConn.Close();
		strTemp.Format("数据库中共有%d条历史记录",iRecordCount);
		ShowMessage(strTemp);
		return ;
	}
	catch(...)
	{
		ShowMessage("查询历史异常");
	}
	adoRst.Close();
	adoConn.Close();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CJXHHistoryDlg::OnDeletethis()
{
	if (g_bExitFlag == false)
	{
		ShowMessage("请在程序停止状态下进行此操作");
		return;
	}
	if(MessageBox("历史删除后将无法恢复,您确定要删除吗?","警告",MB_ICONEXCLAMATION|MB_YESNO)==IDNO)
	{
		return;
	}
	POSITION pos = g_ShowResult.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int nItem = g_ShowResult.GetNextSelectedItem(pos);
		CString strID = g_ShowResult.GetItemText(nItem,0);
		CString strSQL = "";
		strSQL.Format("delete from hisrecord where [ID] = %s",strID);
		ExcuteSQL(strSQL);
		OnBnClickedBtnShowall();
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CJXHHistoryDlg::OnDeleteall()
{
	if (g_bExitFlag == false)
	{
		ShowMessage("请在程序停止状态下进行此操作");
		return;
	}
	if(MessageBox("历史删除后将无法恢复,您确定要删除吗?","警告",MB_ICONEXCLAMATION|MB_YESNO)==IDNO)
	{
		return;
	}
	CString strSQL = "";
	strSQL.Format("delete from hisrecord");
	ExcuteSQL(strSQL);
	OnBnClickedBtnShowall();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CJXHHistoryDlg::OnNMRclickListShowresult(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	POSITION pos = g_ShowResult.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		//int nItem = g_ShowResult.GetNextSelectedItem(pos);		
		CMenu menu; 
		menu.LoadMenu(IDR_MENU1); //载入事先定义的选单 
		CMenu *pmenu = menu.GetSubMenu(0); 
		CPoint pos; 
		GetCursorPos(&pos);
		SetForegroundWindow();
		pmenu->TrackPopupMenu(TPM_LEFTALIGN,pos.x,pos.y,this);		
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CJXHHistoryDlg::ExcuteSQL(CString strSQL)
{
	CAdoConnection adoConn;
	CAdoRecordSet adoRst;
	try
	{
		if(!adoConn.ConnectAccess(g_dbServer,g_dbPwd))
		{
			ShowMessage("连接数据库失败");
			return;	
		}
		adoRst.SetAdoConnection(&adoConn);
		adoRst.SetCursorLocation(adUseClient);
		//更新数据库
		if ( !adoRst.Open(strSQL,adCmdText) )
		{
			ShowMessage("查询历史失败");
			ShowMessage(adoConn.GetLastErrorText());
			adoRst.Close();
			adoConn.Close();
			return;
		}		
		adoRst.Close();
		adoConn.Close();
		return ;
	}
	catch(...)
	{
		ShowMessage("查询历史异常");
	}
	adoRst.Close();
	adoConn.Close();
}