// QQRobotDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "QQRobot.h"
#include "QQRobotDlg.h"
#include <atlconv.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "WebBase.h"

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


// CQQRobotDlg 对话框




CQQRobotDlg::CQQRobotDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQQRobotDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQQRobotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CQQRobotDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CQQRobotDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CQQRobotDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CQQRobotDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CQQRobotDlg 消息处理程序

BOOL CQQRobotDlg::OnInitDialog()
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

	g_HookHelp.m_OnNewReceived.BindCallback<void,
		CQQRobotDlg,
		BufferStr &>
		(this, &CQQRobotDlg::OnRecvContent);
	g_HookHelp.m_OnCloseHandle.BindCallback<void,
		CQQRobotDlg,
		HINTERNET>
		(this, &CQQRobotDlg::OnCloseHandle);

	g_HookHelp.m_OnInternetConnect.BindCallback<void,
		CQQRobotDlg,
		InternetConnectInfo &>
		(this, &CQQRobotDlg::OnInternetConnect);
	
	g_HookHelp.m_OnOpenRequest.BindCallback<void,
		CQQRobotDlg,
		RequestStr &>
		(this, &CQQRobotDlg::OnOpenRequesy);
	//
	if (m_wndHtmlView.CreateHtmlView(IDC_HTML_VIEW, this))
	{
		m_wndHtmlView.MoveWindow(0, 0, 600, 400);			
		
	}
	//m_wndHtmlView.SetParentWnd(m_pParentWnd);
	//m_wndHtmlView.Navigate(_T("http://1800cisvr30/web1800v2/status.asmx"));
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CQQRobotDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CQQRobotDlg::OnPaint()
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
HCURSOR CQQRobotDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CQQRobotDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		KillTimer(nIDEvent);
		//m_wndHtmlView.Navigate("www.baidu.com");
	}
	CDialog::OnTimer(nIDEvent);
}

void CQQRobotDlg::OnBnClickedButton1()
{
	m_wndHtmlView.Navigate(_T("http://www.amazon.cn/?source=eqifa|140382|1|"));
}
void CQQRobotDlg::OnBnClickedButton2()
{
	SendData();
}
void CQQRobotDlg::OnBnClickedButton3()
{
	//获取网页控制权
	//IHtmlDocument2 *pDoc = NULL;
	//m_wndHtmlView.GetDocument();
	IHTMLDocument2* pDocument=(IHTMLDocument2*)m_wndHtmlView.GetHtmlDocument();
	IHTMLWindow2*   pWindow;
	pDocument->get_parentWindow(&pWindow);   
	VARIANT  ret;   
	CString strScript = _T("");	
	strScript= "document.getElementById('chat_area_30097710').value = 'aaaaaaaaaaaaaaa';document.getElementById('btn_sendMsg_30097710').click();";
	//strScript = "alert( Dec('20324124354620373c2d3630263334253735233a342030352437372035352d3630263334253735233a3420303563646e61722b7b73662d6276656471632d363d2735452037442237382132332436302037362d36352737312035352237382132332436302037362d36352737312035352237382132332436302037362d36352737312035352237382132332436302032442d34412735402035352237382132332436302037362d36352737312035352237382132332436302037362d36352737312035352237382132332436302037362d36352737312035352237382132332436302037362d36352737312035326e612d3637263245253735233a367170606f61466b6669717765712436322037452d34412735402035352237382132332436302037362d36352737312035352237382132332436302037362d36352737312035352237382132332436302037362d36352737312035352237382132332436302037362d3635273731203532706c6c7068263333253644233a34203037353c352035372d3643263140253544233a342030352437372035352d3630263334253735233a342030352437372035352d3630263334253735233a342030352437372035352d3630263334253735233a342030352437372035352d3630263334253735233a336d676c666d732035322d37412633342537373531322030372437442037412d3441263334253735233a342030352437372035352d3630263334253735233a342030352437372035352d3630263334253735233a342030352437372035352d3630263334253735233a342030352437372035352d3630263333766077233a33203144243737203537312a302d3121323720344b213546203144223737203a34253131213235203438213732203335223737203a34253131213235203438213732203335223737203a34253131213235203438213732203335223737203a34253131213235203438213732203335223730607a766f714c61737664616d213735203244223737203a36506f64657360203438606a756b6d6a66612237387068662436306b60716d7771273731636b64746d2d3630736d65796077282d36372737422037412235492132332436302037362d36352737312035352237382132332436302037362d36352737312035352237382132332436302037362d36352737312035352237382132332436302037362d363527373120353522373821323474766c2037312d3744273731203537292b2d3646656d65253743527a616b66446f646b7c74607a2a73746721323720344b213546203144223737203a34253131213235203438213732203335223737203a34253131213235203438213732203335223737203a34253131213235203438213732203335223737203a34253131213235203438213732203335223730676f476f6f6e76253732233b452030352437352035364e4246454742253737233a472032412435462035352d3630263334253735233a342030352437372035352d3630263334253735233a342030352437372035352d3630263334253735233a342030352437372035352d3630263334253735233a342030352437372035327f696f67642132322035492137322033377377666b7b746171646a742037342d36462735452037442237382132332436302037362d36352737312035352237382132332436302037362d36352737312035352237382132332436302037362d36352737312035352237382132332436302037362d363527373120353522373f72617172213232203549213546203144223737203a34253131213235203438213732203335223737203a34253131213235203438213732203335223737203a34253131213235203438213732203335223737203a34253131213235203438213732203335223737203a34253131213235203438213732203335223737203a34253131213235203438213240203141223546203a34253131213235203438213732203335223737203a34253131213235203438213732203335223737203a34253131213235203438213732203335223737203a34253131213235203438213732203335223737203a34253131213235203438213732203335223737203a34253131213235203438213732203335223737203a342531316f657c76233b452030352437352072306b3436267433323634237d3235613024703f6363672d71383b3567257030343b34207730643c322035372d3643263140253544233a342030352437372035352d3630263334253735233a342030352437372035352d3630263334253735233a342030352437372035352d3630263334253735233a342030352437372035352d3630263334253735233a342030352437372035352d3630263334253735233a342030352437372035352d3630263334253735233a342030356077626474203b4525313121323735233a362030462435432037442d3630263334253735233a342030352437372035352d3630263334253735233a342030352437372035352d3630263334253735233a342030352437372035352d3630263334253735233a342030352437372035352d3630263334253735233a342030352437372035352d3630263334253735233a342030352437372035352d36302633347060776f67607627364020353522304a21323133343134283639293732203646353536342534322e303c253737233d402030462435432037442d3630263334253735233a342030352437372035352d3630263334253735233a342030352437372035352d3630263334253735233a342030352437372035352d3630263334253735233a342030352437372035352d3630263334253735233a342030352437372035352d3630263334253735233a342030352437372035352d36302633347060776f67604b63686476223646203a342536432132376b7364682030372430432035462d3444263145253735233a342030352437372035352d3630263334253735233a342030352437372035352d3630263334253735233a342030352437372035352d3630263334253735233a342030352437372035352d3630263334253735233a342030352437372035352d3630263334253735233a342030352437372035352d3630263334253735233a346174203244223737342d3643263140253544233a342030352437372035352d3630263334253735233a342030352437372035352d3630263334253735233a342030352437372035352d3630263334253735233a342030352437372035352d3630263334253735233a342030352437372035352d3630263334253735233a342030352437372035352d3630263334253735233a342030356f66223646203a34302633472535412338452030352437372035352d3630263334253735233a342030352437372035352d3630263334253735233a342030352437372035352d3630263334253735233a342030352437372035352d3630263334253735233a342030352437372035352d3630263334253735233a342030352437372035352d3630263334253735233a342030352437376266716d73617a2437412037362d36376a717575223646203a422531476d6e61607e2666646b61742b646a6a203a426762756177647c28786c7527373320354622354c2130422436302037362d36352737312035352237382132332436302037362d36352737312035352237382132332436302037362d36352737312035352237382132332436302037362d36352737312035352237382132332436302037362d36352737312035352237382132332436302037362d36352737312035352237382132332436302037367b6a647276696a732034442d3630263336687171762d37442737472035436e6b6c61782d6365696170286b6b6827374768666c69203a42736b6e732e756d762d36372737422037412235492132332436302037362d36352737312035352237382132332436302037362d36352737312035352237382132332436302037362d36352737312035352237382132332436302037362d36352737312035352237382132332436302037362d36352737312035352237382132332436302037362d36352737312035352237382132332436306e607f2d3744273733323066363c313734323131383567366a30302737332037412235492132332436302037362d36352737312035352237382132332436302037362d36352737312035352237382132332436302037362d36352737312035352237382132332436302037362d36352737312035352237382132332436302037362d36352737312035352237382132332436302037362d363527373120304122354c2130422436302037362d36352737312035352237382132332436302037362d36352737312035352237382132332436302037362d36352737312035352237382132332436302037362d363527324520354622354c2130422436302037362d36352737312035352237382132332436302037362d36352737312035352237382132332436302037362d36352737312035352237382132332436302037362d3635273733636b64746d4b6b6e77646a742037342d363c2736435840314055684','4399') );";



	BSTR bstrStr = strScript.AllocSysString();
	pWindow->execScript(bstrStr,L"javascript",&ret);
	::SysFreeString(bstrStr);
	pWindow->Release();
	//uuidCode
	//CString bstr = GetItemValue("uuidCode");

	pDocument->Release();
	//m_wndHtmlView.
}

void CQQRobotDlg::OnCloseHandle(HINTERNET hHandle)
{
	map<HINTERNET,RequestStr>::iterator it;
	it = m_mapRequestList.find(hHandle);
	CString strContent = _T("");
	if (it != m_mapRequestList.end())
	{
		
		TCHAR tszTemp[10240] = {0};
		DWORD dwSize = sizeof(tszTemp);
	
		dwSize = sizeof(tszTemp);
		ZeroMemory( tszTemp, sizeof(tszTemp));
		if( HttpQueryInfo( hHandle, HTTP_QUERY_RAW_HEADERS_CRLF | HTTP_QUERY_FLAG_REQUEST_HEADERS , tszTemp, &dwSize, 0) )			
		{
			CString strHeader = tszTemp;
			//找到COOKIE
			m_strCookie = GetKeyString(strHeader,_T("Cookie:"),_T("\r"));
		}

		strContent = it->second.strContent;
	//	ConvertGBKToUtf8(strContent);
		//::MessageBox(NULL,strContent,NULL,NULL);
		//发送消息
		m_mapRequestList.erase(it);
	}
	map<HINTERNET,InternetConnectInfo>::iterator itConn;
	itConn = m_mpConnection.find(hHandle);
	if (itConn != m_mpConnection.end())
	{
		m_mpConnection.erase(itConn);
	}
}

CString CQQRobotDlg::GetKeyString(CString &srcStr,CString strBefore,CString strAfter)
{
	int iPosBefore = 0;
	int iPosAfter = 0;
	iPosBefore = srcStr.Find(strBefore) + strBefore.GetLength();
	iPosAfter = srcStr.Find(strAfter,iPosBefore);
	if (iPosBefore < 0 || iPosAfter < 0 || iPosAfter - iPosBefore < 0 )
	{
		return _T("");
	}
	else
	{
		return srcStr.Mid(iPosBefore,iPosAfter - iPosBefore);
	}
}
void CQQRobotDlg::OnRecvContent(BufferStr &contentStr)
{
	map<HINTERNET,RequestStr>::iterator it;
	it = m_mapRequestList.find(contentStr.lHandle);
	string strContent = "";
	if (it == m_mapRequestList.end())
	{
		return;
	}
	else
	{
 		it->second.strContent += CString((char*)contentStr.pBuffer);
	}
	//接收内容
}

void CQQRobotDlg::OnInternetConnect(InternetConnectInfo &connInfo)
{	
	m_mpConnection[connInfo.lHandle] = connInfo;
}

void CQQRobotDlg::OnOpenRequesy(RequestStr &requestStr)
{
	map<HINTERNET,InternetConnectInfo>::iterator it;
	it = m_mpConnection.find(requestStr.hConnect);
	if (it == m_mpConnection.end())
	{
		return;
	}
	requestStr.strHost = it->second.strHost;
	requestStr.strPort = it->second.strPort;
	if (requestStr.strObject.Find(_T("/cgi/mgcgi.fcg"))>=0)
	{
		m_mapRequestList[requestStr.hRequest] = requestStr;
	}	
}

void CQQRobotDlg::SendData()
{
	USES_CONVERSION;
	CWebBase objWeb(50);
	objWeb.SetNoCookie(true);
	string strServer = "b3.qq.com";
	CString strHeader = _T("");
	strHeader += _T("User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022; .NET CLR 1.1.4322)");
	strHeader += _T("\r\nAccept: */*");
	strHeader += _T("\r\nConnection: Keep-Alive");
	strHeader += _T("\r\nReferer:http://b3.qq.com/home.htm#t2s1w1u1");
	strHeader += _T("\r\nContent-Type:text/plain; charset=utf-8");
	strHeader += _T("\r\nAccept-Encoding:gzip, deflate");
	strHeader += _T("\r\nCookie:");
	strHeader += m_strCookie;
	
	srand(time(NULL));
	int iRet  = -1;
	int iLoopCount = 0;
	CString strFailReason = _T("");
	CString strFormat =  _T("");
	CString strData = _T("kfguin=800018888&ext=1002&cid=30094613&uin=182534287&ty=1&msg=111111&idx=0&");
	strFormat.Format(_T("/cgi/d?t=20859352453472057"),GetTickCount());	

	time_t t = time(NULL);
	HINTERNET m_hInterSessin;
	m_hInterSessin = InternetOpen((LPCWSTR)"YuTone_Http_Post", 
		INTERNET_OPEN_TYPE_DIRECT, 
		NULL, 
		NULL, 
		INTERNET_FLAG_NO_CACHE_WRITE);
	//获取HTTP连接
	HINTERNET hConnect = InternetConnect(m_hInterSessin,
		//(LPCWSTR)lpszServer,
		_T("b3.qq.com"),
		80,
		NULL,
		NULL,
		INTERNET_SERVICE_HTTP, 
		INTERNET_FLAG_NO_CACHE_WRITE,
		NULL);
	if (hConnect == NULL)
	{
		//SetLastErrorInfo("InternetConnect Error:%d,TimeSpan:%d", GetLastError(),time(NULL)-t);
		return;
	}
	DWORD dwFlag = objWeb.GenFlags();
	t = time(NULL);
	//发送HTTP请求
	HINTERNET hRequest = HttpOpenRequest(hConnect,_T("post"),(LPCWSTR)strFormat,NULL,NULL,NULL,dwFlag,NULL);
	if (NULL == hRequest)
	{		
		//SetLastErrorInfo("HttpOpenRequest %s Error:%d,TimeSpan:%d",lpszVerb, GetLastError(),time(NULL)-t);
		InternetCloseHandle(hConnect);
		return ;
	}
	t = time(NULL);
	string strDataEx = "kfguin=800018888&ext=1002&cid=30094613&uin=182534287&ty=1&msg=111111&idx=0&";
	BOOL bRet =  HttpSendRequest(hRequest,(LPCWSTR)strHeader,strHeader.GetLength(),(void *)strDataEx.c_str(),strDataEx.length());
	if (!bRet)
	{
		DWORD ds = GetLastError();
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		return ;
	}
	t = time(NULL);
	DWORD dwStatus = 0;
	DWORD dwStatusSize = sizeof(dwStatus);
	bRet = HttpQueryInfo(hRequest,HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,&dwStatus,&dwStatusSize,NULL);
	if (!bRet)
	{
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		return ;
	}
	if ( !(dwStatus == HTTP_STATUS_OK ||dwStatus == HTTP_STATUS_MOVED ||
		dwStatus == HTTP_STATUS_REDIRECT ||	dwStatus == HTTP_STATUS_REDIRECT_METHOD) )
	{
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		return ;
	}
	char szHttpHeader[10240] = {0};
	DWORD dHeaderLen = sizeof(szHttpHeader);
	bRet = HttpQueryInfo(hRequest,HTTP_QUERY_RAW_HEADERS_CRLF,szHttpHeader,&dHeaderLen,NULL);
	if (!bRet)
	{
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		return ;
	}
	
	t = time(NULL);
	string m_strContent;
	m_strContent.clear();
	DWORD dwReadLen = 0;
	DWORD dwCurrLen = 0;
	char szBuf[1024] = {0};
	
	while (true)
	{
		
		ZeroMemory(szBuf,1024);
		bRet = InternetReadFile(hRequest,szBuf,1023,&dwCurrLen);
		if (!bRet)
		{
		//	SetLastErrorInfo("InternetReadFile Error:%d,TimeSpan:%d", GetLastError(),time(NULL)-t);
			InternetCloseHandle(hRequest);
			InternetCloseHandle(hConnect);
			return ;
		}
		if (dwCurrLen == 0)
		{
			break;
		}
		dwReadLen += dwCurrLen;
		m_strContent.append(szBuf);
	}	
	::MessageBoxA(NULL,m_strContent.c_str(),"提示",NULL);
// 	iRet = objWeb.PostPage((const char*)A2T((LPCSTR)(LPCTSTR)strFormat),(const char *)(LPCWSTR)strData,(const char *)(LPCTSTR)strHeader,strServer.c_str(),80,false,"");
// 	if (iRet < 0)
// 	{
// 		return;
// 	}
// 	CString strRet = CString(objWeb.GetContentInfo());
	
}

void CQQRobotDlg::ConvertUtf8ToGBK(CString& strUtf8) 
{
	int len = 0;
	unsigned short * wszGBK = NULL;
	char * szGBKt = NULL;
	char * szGBKs = NULL;

	len = MultiByteToWideChar(CP_UTF8, 0, (LPSTR)(LPCTSTR)strUtf8, -1, NULL,0);
	wszGBK = new unsigned short[len+1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPSTR)(LPCTSTR)strUtf8, -1, (LPWSTR)wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL); 
	szGBKt  =new char[len + 1];
	memset(szGBKt, 0, len + 1);
	WideCharToMultiByte (CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBKt, len, NULL,NULL);

	szGBKs  =new char[len + 1];
	memset(szGBKs, 0, len + 1);
	//转换Gb码繁体到Gb码简体，使用API函数LCMapString 
	LCMapString( 0x0804, LCMAP_SIMPLIFIED_CHINESE, (LPCWSTR)szGBKt, -1, (LPWSTR)szGBKs, len);
	strUtf8 = szGBKs;
	delete[] szGBKs;
	delete[] szGBKt;
	delete[] wszGBK;
}


void CQQRobotDlg::ConvertGBKToUtf8(CString& strGBK) {

	int len=MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(LPCTSTR)strGBK, -1,NULL,0);

	unsigned short * wszUtf8 = new unsigned short[len+1];

	memset(wszUtf8, 0, len * 2 + 2);

	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(LPCTSTR)strGBK, -1, (LPWSTR)wszUtf8,len);

	len = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)wszUtf8, -1, NULL, 0,NULL, NULL);

	char *szUtf8=new char[len + 1];

	memset(szUtf8, 0, len + 1);

	WideCharToMultiByte (CP_UTF8, 0, (LPCWSTR)wszUtf8, -1, szUtf8, len,NULL,NULL);

	strGBK = szUtf8;

	delete[] szUtf8;

	delete[] wszUtf8;

}