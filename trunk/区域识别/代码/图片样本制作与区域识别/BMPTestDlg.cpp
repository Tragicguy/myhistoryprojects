// BMPTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BMPTest.h"
#include "BMPTestDlg.h"
#include ".\bmptestdlg.h"
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <io.h>
#include   <iostream>   
#include   <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CBMPTestDlg 对话框



CBMPTestDlg::CBMPTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBMPTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBMPTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBMPTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP

	ON_BN_CLICKED(IDC_BTN_DETECT, OnBnClickedBtnDetect)
	ON_BN_CLICKED(IDC_BTN_CreatTemp, OnBnClickedBtnCreattemp)
	ON_BN_CLICKED(IDC_BTN_CBAT, OnBnClickedBtnCbat)
	ON_BN_CLICKED(IDC_BTN_CVEC, OnBnClickedBtnCvec)
	ON_BN_CLICKED(IDC_BTN_TRAIN, OnBnClickedBtnTrain)
	ON_BN_CLICKED(IDC_BTN_CXML, OnBnClickedBtnCxml)
END_MESSAGE_MAP()


// CBMPTestDlg 消息处理程序

BOOL CBMPTestDlg::OnInitDialog()
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

	// TODO: 在此添加额外的初始化代码
	SetDlgItemText(IDC_INFO,"1: 在C盘根目录下建一名为OpenCVTrain的文件夹\r\n2: 将bin文件夹COPY到此文件夹中\r\n3: 在OpenCVTrain中建立一个名为data的文件夹\r\n4: 进行样本的准备\r\n5: 进行训练脚本的生成\r\n6: 生成样本文件\r\n7: 进行样本的训练\r\n8: 生成XML模板文件\r\n\r\n所有操作必需按顺序执行.");
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CBMPTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBMPTestDlg::OnPaint() 
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
HCURSOR CBMPTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//区域检测
void CBMPTestDlg::OnBnClickedBtnDetect()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sFolderPath;
	BROWSEINFO bi;
	char Buffer[MAX_PATH]; //初始化入口参数bi开始
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer; //此参数如为NULL则不能显示对话框
	bi.lpszTitle = "请选择图片所在目录";
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.iImage = 0;   //初始化入口参数bi结束
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//调用显示选择对话框
	if(pIDList)
	{
		SHGetPathFromIDList(pIDList, Buffer);
		//取得文件夹路径到Buffer里
		sFolderPath = Buffer;//将路径保存在一个CString对象里
	}
	CFileFind finder;
	CString findName = sFolderPath + "\\*.jpg";
	BOOL findFlag = finder.FindFile(findName);
	while (findFlag)
	{
		findFlag = finder.FindNextFile();
		if (finder.IsDots())
			continue;

		CString sFileName = finder.GetFileName();
		CMyImage myImg;
		CPoint detectPoint = myImg.MatchPicture(sFolderPath+"\\"+sFileName,"c:\\OpenCVTrain\\data.xml");
		CString aa;
		aa.Format("%d,%d",detectPoint.x,detectPoint.y);
		MessageBox(aa);
	}
	finder.Close();
	
}

//生成样本图片，正负都有
void CBMPTestDlg::OnBnClickedBtnCreattemp()
{
	CString sFolderPath;
	BROWSEINFO bi;
	char Buffer[MAX_PATH]; //初始化入口参数bi开始
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer; //此参数如为NULL则不能显示对话框
	bi.lpszTitle = "请选择图片所在目录";
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.iImage = 0;   //初始化入口参数bi结束
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//调用显示选择对话框
	if(pIDList)
	{
		SHGetPathFromIDList(pIDList, Buffer);
		//取得文件夹路径到Buffer里
		sFolderPath = Buffer;//将路径保存在一个CString对象里
	}

	CFileFind finder;
	CString findName = sFolderPath + "\\*.jpg";
	BOOL findFlag = finder.FindFile(findName);
	while (findFlag)
	{
		
		findFlag = finder.FindNextFile();
		if (finder.IsDots())
		continue;
		
		CString sFileName = finder.GetFileName();
		CMyImage myImg;
		myImg.MackTemplate(sFolderPath+"\\"+sFileName);
		
	}
	finder.Close();
}

void CBMPTestDlg::GenerateDataFile()
{

	CString infopath = "C:\\OpenCVTrain\\info.dat";     //正样本集合文件
	CFile file;
	if (_access(infopath,0)==0) //文件存在
	{
		file.Remove(infopath);		
	}
	file.Open(infopath,CFile::modeCreate|CFile::modeReadWrite);
	
	CString sFolderPath = "C:\\OpenCVTrain\\Template";
	CFileFind finder;
	CString findName = sFolderPath + "\\*.jpg";
	BOOL findFlag = finder.FindFile(findName);
	while (findFlag)
	{

		findFlag = finder.FindNextFile();
		if (finder.IsDots())
			continue;
		CString sFileName = finder.GetFileName();
		CString infoStr;
		infoStr.Format("Template\\%s 1 0 0 %d %d \r\n",sFileName,40,40);
		file.SeekToEnd();
		file.Write(infoStr,infoStr.GetLength());
	}
	file.Close();
	finder.Close();

	
	infopath = "C:\\OpenCVTrain\\bg.txt";     //正样本集合文件

	if (_access(infopath,0)==0) //文件存在
	{
		file.Remove(infopath);		
	}
	file.Open(infopath,CFile::modeCreate|CFile::modeReadWrite);

	sFolderPath = "C:\\OpenCVTrain\\offTemplate";
	findName = sFolderPath + "\\*.jpg";
	findFlag = finder.FindFile(findName);
	while (findFlag)
	{

		findFlag = finder.FindNextFile();
		if (finder.IsDots())
			continue;
		CString sFileName = finder.GetFileName();
		CString infoStr;
		infoStr.Format("offTemplate\\%s\r\n",sFileName);
		file.SeekToEnd();
		file.Write(infoStr,infoStr.GetLength());
	}
	file.Close();
	finder.Close();
}


//根据生成的样本图片生成训练脚本
void CBMPTestDlg::OnBnClickedBtnCbat()
{
	// TODO: 在此添加控件通知处理程序代码

	//生成run.bat

	GenerateDataFile();
	CFile file;
	CString fileName;
	CString infoStr;
	int infoRows = 0;
	int bgRows = 0;
	fileName = "C:\\OpenCVTrain\\info.dat";
	if (_access(fileName,0)!=0) //文件不存在
	{
		MessageBox("样本集合文件不存在");
		return;
	}
	
	infoRows = GetRows(fileName);

	fileName = "C:\\OpenCVTrain\\run.bat";     //正样本集合文件
	if (_access(fileName,0)==0) //文件存在
	{
		file.Remove(fileName);		
	}
	file.Open(fileName,CFile::modeCreate|CFile::modeReadWrite);
	file.SeekToEnd();
	infoStr.Format("c:\\OpenCVTrain\\bin\\CreateSamples.exe  -vec c:\\OpenCVTrain\\data.vec -info c:\\OpenCVTrain\\info.dat -num %d -w 40 -h 40",infoRows);
	file.Write(infoStr,infoStr.GetLength());
	file.Close();


	//生成train.bat
	fileName = "C:\\OpenCVTrain\\bg.txt"; 
	if (_access(fileName,0)!=0) //文件不存在
	{
		MessageBox("负样本集合文件不存在");
		return;
	}
	bgRows = GetRows(fileName);
	fileName = "C:\\OpenCVTrain\\train.bat";     //正样本集合文件
	if (_access(fileName,0)==0) //文件存在
	{
		file.Remove(fileName);		
	}
	file.Open(fileName,CFile::modeCreate|CFile::modeReadWrite);
	file.SeekToEnd();
	infoStr.Format("c:\\OpenCVTrain\\bin\\haartraining.exe -mem 1000 -data c:\\OpenCVTrain\\data -vec c:\\OpenCVTrain\\data.vec -bg c:\\OpenCVTrain\\bg.txt -npos %d -nneg %d -w 40 -h 40",infoRows,bgRows);
	file.Write(infoStr,infoStr.GetLength());
	file.Close();
	
	//生成saveXML.bat
	fileName = "C:\\OpenCVTrain\\saveXML.bat";     //正样本集合文件
	if (_access(fileName,0)==0) //文件存在
	{
		file.Remove(fileName);		
	}
	file.Open(fileName,CFile::modeCreate|CFile::modeReadWrite);
	file.SeekToEnd();
	infoStr.Format("c:\\OpenCVTrain\\bin\\haarconv.exe c:\\OpenCVTrain\\data c:\\OpenCVTrain\\data.xml 40 40");
	file.Write(infoStr,infoStr.GetLength());
	file.Close();
	MessageBox("生成脚本成功");

}


//从一个文本文件中获取行数
int CBMPTestDlg::GetRows(CString fileName)
{
	int   lines   =   0;   
	CStdioFile   file;
	CString   cstr;
	CFileException   fe;
	if(file.Open(fileName,CFile::modeRead,&fe))   
	{ 
		file.SeekToBegin();
		while (file.ReadString(cstr))
		{
			lines ++;
		}
		file.Close();
	}
	return lines;
}


//生成样本文件
void CBMPTestDlg::OnBnClickedBtnCvec()
{
	// TODO: 在此添加控件通知处理程序代码
	system("c:\\OpenCVTrain\\run.bat");
}	


//训练样本
void CBMPTestDlg::OnBnClickedBtnTrain()
{
	// TODO: 在此添加控件通知处理程序代码
	system("c:\\OpenCVTrain\\train.bat");
}


//生成XML模板数据
void CBMPTestDlg::OnBnClickedBtnCxml()
{
	// TODO: 在此添加控件通知处理程序代码
	system("c:\\OpenCVTrain\\saveXML.bat");
}
