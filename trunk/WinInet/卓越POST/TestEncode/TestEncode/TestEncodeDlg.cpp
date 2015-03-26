// TestEncodeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestEncode.h"
#include "TestEncodeDlg.h"
#include "AmazonEncode.h"
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


// CTestEncodeDlg 对话框


CString Base64Encode(LPCTSTR strEncodeData, int nDataSize); 

CTestEncodeDlg::CTestEncodeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestEncodeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestEncodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestEncodeDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CTestEncodeDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTestEncodeDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CTestEncodeDlg 消息处理程序

BOOL CTestEncodeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

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
time_t timer; time(&timer);
	long lTime = timer;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTestEncodeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestEncodeDlg::OnPaint()
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
//
HCURSOR CTestEncodeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTestEncodeDlg::OnBnClickedButton1()
{
	CString strSrc = "{\"version\":\"2.2.0\",\"start\":1311468074765,\"elapsed\":281,\"userAgent\":\"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.2; SV1; EmbeddedWB 14.52 from: http://www.bsalsa.com/ EmbeddedWB 14.52; .NET CLR 1.1.4322; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022; .NET CLR 2.0.50727; .NET4.0C)\",\"plugins\":\"ShockwaveFlash : 655363|AB 6,0,3790,3959|WDUN 6,0,3790,3959|DA 6,0,3,531|DAJC 6,0,1,223|DS 10,0,0,3997|DHDB 5,5000,3130,0|DHDBFJ 6,0,1,223|ICW 5,0,2918,1900|IE 6,0,3790,3959|WMP 10,0,0,3997|NN 4,4,0,4000|OBP 6,0,3790,3959|OE 6,0,3790,3959|TS 4,71,1968,1||1366-768-738-32-96-96-0\",\"flashVersion\":\"10.3\",\"timeZone\":-8}";
	CString strX = EncodeC(strSrc);
	CString strRSrc = strX + "#" + strSrc;
	CString strR = EncodeR(strRSrc);
	CString strRet = Base64Encode(strR, strR.GetLength());//base64
	AfxMessageBox(strRet);

}


CString CTestEncodeDlg::EncodeC(CString U)
{
	int iLen = U.GetLength();
	CString strShow = "";
	ULONG T = 3988292384;
	ULONG I[256] = {0};
	for (int X = 0; X < 256; X++)
	{
		ULONG W = X;
		for(int V = 0; V < 8; V++)
		{
			if ((W&1) == 1)
				W = (W>>1) ^ T;
			else
				W = (W >> 1);
		}
		I[X] = W;
	}
	ULONG Y=0;
	Y = Y^4294967295;
	for(int X=0;X<iLen;X++)
	{
		int S=(Y^U.GetAt(X))&255;
		Y=(Y>>8)^I[S];
	}
	Y=Y^4294967295;
	CString Z="0123456789ABCDEF";
	CString strRet = "";
	strRet.Format("%c%c%c%c%c%c%c%c",Z.GetAt((Y>>28)&15),Z.GetAt((Y>>24)&15),Z.GetAt((Y>>20)&15),Z.GetAt((Y>>16)&15),Z.GetAt((Y>>12)&15),Z.GetAt((Y>>8)&15),Z.GetAt((Y>>4)&15),Z.GetAt((Y)&15));
	return strRet;
}

int GetCeil(int iIntValue, int iFloatValue)
{
	return iFloatValue == 0 ? iIntValue : (iIntValue + 1);
}

int GetFloor(int iIntValue, int iFloatValue)
{
	return 1 + (iFloatValue == 0 ? iIntValue : (iIntValue - 1));
}

CString Base64Encode(LPCTSTR strEncodeData, int nDataSize)
{
	CString strRetData = _T("");
	try
	{
		int nEncodedSize = nDataSize * 4 / 3 + 5 + nDataSize * 2 / 57;
		char *strEncodedData = new char[nEncodedSize + 1];
		// Base64 encoding
		char code[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		unsigned char * strData = (unsigned char *) strEncodeData;
		int i;
		int j;
		for (i = 0, j = 0; i < (nDataSize - nDataSize % 3); i += 3)
		{	  
			strEncodedData[j++] = code[strData[i] >> 2];
			strEncodedData[j++] = code[(strData[i] & 0x3) << 4 | strData[i + 1] >> 4];
			strEncodedData[j++] = code[(strData[i + 1] & 0xf) << 2 | strData[i + 2] >> 6];
			strEncodedData[j++] = code[strData[i + 2] & 0x3f];

			if (i % 57 >= 54)
			{
				strEncodedData[j++] = '\r';
				strEncodedData[j++] = '\n';
			}
		}

		if (nDataSize % 3 == 2)
		{
			strEncodedData[j++] = code[strData[i] >> 2];
			strEncodedData[j++] = code[(strData[i] & 0x3) << 4 | strData[i + 1] >> 4];
			strEncodedData[j++] = code[(strData[i + 1] & 0xf) << 2];
			strEncodedData[j++] = '=';
		}
		else if (nDataSize % 3 == 1)
		{
			strEncodedData[j++] = code[strData[i] >> 2];
			strEncodedData[j++] = code[(strData[i] & 0x3) << 4];
			strEncodedData[j++] = '=';
		}

		strEncodedData[j] = '\0';
		strRetData = strEncodedData;

		delete [] strEncodedData;
	}
	catch (CException *e)
	{
		
	}
	catch (...)
	{
		
	}

	return strRetData;
}

void Show(int i)
{
	CString strShow = "";
	strShow.Format("%d",i);
	AfxMessageBox(strShow);
}


CString CTestEncodeDlg::EncodeR(CString ab)
{
	int iLen = ab.GetLength();
	if(iLen==0){return"";}
	ULONG V[4] = {4169969034, 4087877101,1706678977, 3681020276};
	int U = GetCeil(iLen / 4, iLen % 4);
	ULONG *ad = new ULONG[U];
	for(int W=0;W<U;W++)
	{
		int iLenth = W*4;
		int a = iLenth > (iLen-1) ? 0 : ab.GetAt(iLenth)&255;
		int b = (iLenth+1) > (iLen-1) ? 0 : ((ab.GetAt(iLenth+1)&255)<<8);
		int c = (iLenth+2) > (iLen-1) ? 0 : ((ab.GetAt(iLenth+2)&255)<<16);
		int d = (iLenth+3) > (iLen-1) ? 0 : ((ab.GetAt(iLenth+3)&255)<<24);
		ad[W]=a + b + c + d;
	}
	ULONG ac=2654435769;
	int ae= GetFloor(52 / U, 52 % U) + 6;
	ULONG aa=ad[0];
	ULONG Z=ad[U-1];
	ULONG X=0;
	while(ae-->0){
		X+=ac;
		ULONG Y=(X>>2)&3;
		for(int S=0;S<U;S++){
			aa=ad[(S+1)%U];
			Z=ad[S]+=(((Z>>5)^(aa<<2))+((aa>>3)^(Z<<4)))^((X^aa)+(V[(S&3)^Y]^Z));
		}
	}
	CString strRet = "";
	CString strFormat = "";	
	for(int W=0;W<U;W++){
		strFormat.Format("%c%c%c%c", (ad[W])&255, (ad[W]>>8)&255, (ad[W]>>16)&255, (ad[W]>>24)&255);
		strRet += strFormat;
	}
	return strRet;
}



void CTestEncodeDlg::OnBnClickedButton2()
{
	CString strKey = "";
	CAmazonEncode objEncode;
	strKey = objEncode.GenAuthKey();
	strKey.Replace("\r","");
	strKey.Replace("\n","");
	SetDlgItemText(IDC_EDIT1, strKey);
	//AfxMessageBox(strKey);
	//GenAuthKey
}
