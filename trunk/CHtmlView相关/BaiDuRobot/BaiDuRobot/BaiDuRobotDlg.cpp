// BaiDuRobotDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BaiDuRobot.h"
#include "BaiDuRobotDlg.h"
#include "WindowFind.h"
#include <direct.h>
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma warning(disable:4996)
#pragma warning(disable:4267)
CWindowFind g_find;
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


// CBaiDuRobotDlg 对话框




CBaiDuRobotDlg::CBaiDuRobotDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBaiDuRobotDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBaiDuRobotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SHOW, m_list_show);
}

BEGIN_MESSAGE_MAP(CBaiDuRobotDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START, &CBaiDuRobotDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &CBaiDuRobotDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BUTTON1, &CBaiDuRobotDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CBaiDuRobotDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CBaiDuRobotDlg::OnBnClickedButton3)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CBaiDuRobotDlg 消息处理程序

BOOL CBaiDuRobotDlg::OnInitDialog()
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
	InitListCtrl();
	CoInitialize(NULL);
// 	SetDlgItemText(IDC_EDIT_KEY, "租车");
 	SetDlgItemText(IDC_EDIT_URL, "www.chesudi.com;\r\nwww.ystzc.com;");

	SetDlgItemText(IDC_EDIT_KEY, "租车");
	//SetDlgItemText(IDC_EDIT_URL, "www.blchina.com/;");
	GetAppPath(m_strAppPath);
	m_bExitFlag = true;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CBaiDuRobotDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBaiDuRobotDlg::OnPaint()
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
HCURSOR CBaiDuRobotDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CBaiDuRobotDlg::ThreadFunc(LPVOID lParam)
{
	CBaiDuRobotDlg *pParam = ((CBaiDuRobotDlg *)lParam);
	if ( pParam )
	{
		pParam->RunFunc();
	}	
}

void CBaiDuRobotDlg::RunFunc()
{
	CString strKey = "";
	CString strUrl = "";
	GetDlgItemText(IDC_EDIT_URL,strUrl);
	GetDlgItemText(IDC_EDIT_KEY,strKey);
	CoInitialize(NULL);
	AuToOpenPage(strKey, strUrl, &m_bExitFlag);
	CoUninitialize();
	GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
}

bool CBaiDuRobotDlg::StartExplorer(CString strUrl, STARTUPINFO &vStartupInfo, PROCESS_INFORMATION& vProcessInfo)
{

	memset(&vStartupInfo, 0, sizeof(vStartupInfo));
	vStartupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	vStartupInfo.wShowWindow = SW_SHOWMAXIMIZED; 
	vStartupInfo.cb = sizeof(STARTUPINFO);
	char commandBuf[512];
	sprintf_s(commandBuf, "\"c:\\Program Files\\Internet Explorer\\IEXPLORE.EXE\" %s", strUrl);
	int nRetry = 4;
	int nRetrunValue = -1;
	while(--nRetry > 0 && nRetrunValue != 0)
	{
		if (::CreateProcess(
			NULL,
			commandBuf,
			NULL,
			NULL,
			FALSE,
			0,
			NULL,
			NULL,
			&vStartupInfo,
			&vProcessInfo))
		{
			return true;
		}
		Sleep(200);
		DWORD dRet = GetLastError();
	}
	return false;
}

typedef struct _MainHwndStr
{
	HWND hwnd;
	DWORD dwProcessID;
}MainHwndStr;

BOOL CALLBACK EnumMailHwndProc(HWND  hwndTemp, LPARAM lParam)
{
	MainHwndStr* pMainStr = (MainHwndStr*)lParam;
	DWORD PID;
	GetWindowThreadProcessId(hwndTemp, &PID);
	if (PID != pMainStr->dwProcessID)
	{
		EnumChildWindows(hwndTemp,EnumMailHwndProc,lParam);
		return TRUE;
	}	
	CString strRet = "";
	TCHAR  szClassName[256]={0};	
	GetClassName(hwndTemp,(LPTSTR)&szClassName,256);
	strRet.Format("%s",szClassName);
	if (strRet.Find("IEFrame") < 0)
	{
		EnumChildWindows(hwndTemp,EnumMailHwndProc,lParam);
		return TRUE;
	}
	pMainStr->hwnd = hwndTemp;
	return FALSE;
}

HWND CBaiDuRobotDlg::GetMainHwnd(DWORD dwProcessId)
{
	MainHwndStr mainStr;
	mainStr.hwnd = NULL;
	mainStr.dwProcessID = dwProcessId;
	EnumChildWindows(NULL,EnumMailHwndProc,(LPARAM)&mainStr);
	return mainStr.hwnd;
}

void CBaiDuRobotDlg::AuToOpenPage(CString strKey, CString strUrl, bool *pbExitFlag)
{
	//初始化URL数组
	strUrl.MakeLower();	
	map<CString,int> mapUrlList;	//URL List
	StringSplit(strUrl, mapUrlList, ';');
	map<CString,int>::iterator it;
	bool bRet = false;
	int iSize = mapUrlList.size();
	if (iSize < 1)
	{
		AddToList("无URL需要查找");
		return;
	}
	IHTMLDocument2 *pDoc = NULL;
	CString strBaidu = "www.baidu.com";
	strBaidu.Format("http://www.baidu.com/s?wd=%s",URLEncode(strKey));
	STARTUPINFO vStartupInfo;
	PROCESS_INFORMATION vProcessInfo;
	bRet = StartExplorer(strBaidu, vStartupInfo, vProcessInfo);
	if (!bRet)
	{
		AddToList("启动IE失败");
		return;
	}
	Sleep(1000);
	//得到对应的百度IE窗口句柄
	HWND hwndMain = GetMainHwnd(vProcessInfo.dwProcessId);
	if (hwndMain == NULL)
	{
		AddToList("查找IE主窗口句柄失败");
		return;
	}
	::SetWindowPos(hwndMain,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	Sleep(500);
	HWND hwnd = GetExplorerHWND(hwndMain);
	if (hwnd == NULL)
	{
		AddToList("查找IE内容页窗口失败");
	}
	Sleep(1000);
	int iCount =0;
	while (iCount++ < 25)
	{
		pDoc = GetDocInterface(hwnd);
		if (pDoc != NULL)
		{
			break;
		}
		Sleep(600);
	}
	
	if (!pDoc)
	{
		AddToList("获取HTML控制失败");
		return;
	}
	AddToList("已经获取HTML控制权");
	IHTMLElement *pElement = NULL;
	IHTMLElement *pElementParent = NULL;
	int iKeyFlag = 0;
	CString strKeyId = "";
	CString strId = "";
	CString strInnerText = "";
	CString strUrlFlag = "";
	while(!(*pbExitFlag))
	{
		switch (iKeyFlag)
		{
		case 0:
			strKeyId = "aw";	//左侧 “推广”
			break;
		case 1:
			strKeyId = "dfs";	//右侧广告
			break;
		case 2:
			break;
		default:
			return;
		}
		int iIndex = -1;		
		while(!(*pbExitFlag))
		{
			iIndex++;
			strId.Format("%s%d",strKeyId, iIndex);
			GetElement(strId,&pElement,pDoc);
			if (pElement == NULL)
			{
				break;
			}
			//找到父标签
			pElement->get_parentElement(&pElementParent);
			if (pElementParent == NULL)
			{
				pElement->Release();
				continue;;
			}
			strInnerText = GetInnerHtml(pElementParent);
			if (strInnerText == "")
			{
				pElement->Release();
				pElementParent->Release();
				continue;
			}
			strUrlFlag = GetInnerUrl(strInnerText, iKeyFlag);
			strUrlFlag.MakeLower();
			strUrlFlag.Trim();
			if(strUrlFlag != "")
			{
				
				it = mapUrlList.find(strUrlFlag);
				if (it != mapUrlList.end())
				{
					AddToList("找到 " + strUrlFlag);
					RECT rt;
					HTMLElementRect(pElement, rt, 1000);
					RECT rectTotal;
					::GetWindowRect(hwnd, &rectTotal);

					rt.top += rectTotal.top;
					rt.bottom += rectTotal.top;
					rt.left += rectTotal.left;
					rt.right += rectTotal.left;

					CRect rect(rt);
					int pX = rect.left+30;
					int pY = rect.top + 5;
					Sleep(100);
					//pY = 529;
					CPoint pt;
					GetCursorPos(&pt);
					::SetCursorPos(pX,pY);
					Sleep(100);
					mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
					Sleep(200);
					
					CString strInfo = "";
					strInfo.Format("%ld, %ld, %ld, %ld", rect.left, rect.top, rect.Width(), rect.Height());
					//AddToList(strInfo);
					AddToList("3秒后尝试点击下一个链接");
					Sleep(3500);
				}
			}
			else
			{
				pElement->Release();
			}			
			pElementParent->Release();
		}
		iKeyFlag++;
		if (iKeyFlag >= 3)
		{
			break;
		}
	}
	pDoc->Release();
	//::TerminateProcess(vProcessInfo.hProcess, 0);
	::PostMessage(hwndMain, WM_CLOSE, 0, 0);
	AddToList("关闭百度窗口");
	AddToList("线程退出");
}

CString CBaiDuRobotDlg::GetInnerUrl(CString strText, int iFlag)
{
	int iPos = strText.Find("#008000");
	if (iPos < 0)
	{
		return "";
	}
	strText = strText.Mid(iPos);
	CString strAfter = " ";
	if (iFlag == 1)
	{
		strAfter = "<";
	}
	CString strRet = GetKeyString(strText,">",strAfter);
	if (iFlag == 0 && strRet == "")
	{
		strRet = GetKeyString(strText,">","<");
	}
	return strRet;
}

CString CBaiDuRobotDlg::GetInnerText(IHTMLDocument2 *pDoc,CString strId)
{
	if (pDoc == NULL)
	{
		return "";
	}
	IHTMLElement *pElement = NULL;
	GetElement(strId,&pElement,pDoc);
	if (pElement == NULL)
	{
		return "";
	}

	BSTR bstrStr;	
	pElement->get_innerText(&bstrStr);	
	CString str((char*)(_bstr_t)bstrStr);
	::SysFreeString(bstrStr);
	pElement->Release();	
	return str;
}

CString CBaiDuRobotDlg::GetInnerText(IHTMLElement *pElement)
{
	if (pElement == NULL)
	{
		return "";
	}

	BSTR bstrStr;	
	pElement->get_innerText(&bstrStr);	
	CString str((char*)(_bstr_t)bstrStr);
	::SysFreeString(bstrStr);
	return str;
}

CString CBaiDuRobotDlg::GetInnerHtml(IHTMLElement *pElement)
{
	if (pElement == NULL)
	{
		return "";
	}

	BSTR bstrStr;	
	pElement->get_innerHTML(&bstrStr);
	CString str((char*)(_bstr_t)bstrStr);  
	::SysFreeString(bstrStr);
	return str;
}

bool CBaiDuRobotDlg::PostWebClick(IHTMLDocument2 *pDoc,CString strId)
{
	if (pDoc == NULL)
	{
		return false;
	}
	IHTMLElement *pElement = NULL;
	GetElement(strId,&pElement,pDoc);
	if (pElement == NULL)
	{
		return false;
	}	
	pElement->click();
	pElement->Release();
	return true;
}

void CBaiDuRobotDlg::AddToList(CString strInfo)
{
	int nCount = m_list_show.GetItemCount();
	if (nCount > 200)
	{
		m_list_show.DeleteAllItems();
		nCount = 0;
	}
	m_list_show.InsertItem( nCount, "");	
	m_list_show.SetItemText( nCount , 0, CTime::GetCurrentTime().Format("%H:%M:%S") );
	m_list_show.SetItemText( nCount , 1, strInfo );
	::PostMessage(m_list_show.m_hWnd,WM_VSCROLL,SB_BOTTOM,0);
	WriteLog(strInfo,1);
}


HRESULT CBaiDuRobotDlg::GetElement(LPCTSTR szElementId, IHTMLElement **pphtmlElement,IHTMLDocument2* pDoc)
{
	return GetElementInterface(szElementId, __uuidof(IHTMLElement), (void **) pphtmlElement,pDoc);
}

HRESULT CBaiDuRobotDlg::GetElementInterface(LPCTSTR szElementId, REFIID riid, void **ppvObj,IHTMLDocument2* pDoc)
{
	ENSURE_ARG(ppvObj!=NULL);
	HRESULT hr = E_NOINTERFACE;
	*ppvObj = NULL;
	CComPtr<IDispatch> spdispElem;
	BOOL bCollect = FALSE;
	hr = GetElement(szElementId, &spdispElem,&bCollect,pDoc);

	if (spdispElem)
		hr = spdispElem->QueryInterface(riid, ppvObj);
	return hr;
}

HRESULT CBaiDuRobotDlg::GetElement(LPCTSTR szElementId, IDispatch **ppdisp, BOOL *pbCollection /*= NULL*/,CComPtr<IHTMLDocument2> sphtmlDoc)
{
	ENSURE_ARG(ppdisp!=NULL);
	CComPtr<IHTMLElementCollection> sphtmlAll;
	CComPtr<IHTMLElementCollection> sphtmlColl;
	CComPtr<IDispatch> spdispElem;
	CComVariant varName;
	CComVariant varIndex;
	HRESULT hr = S_OK;
	//CComPtr<IHTMLDocument2> sphtmlDoc;

	*ppdisp = NULL;

	if(szElementId == NULL)
		return E_INVALIDARG;

	const CString strElementId(szElementId);
	varName.vt = VT_BSTR;
	varName.bstrVal = strElementId.AllocSysString();

#ifndef _UNICODE	
	if (!varName.bstrVal)
	{
		hr = E_OUTOFMEMORY;
		goto Error;
	}
#endif

	hr = sphtmlDoc->get_all(&sphtmlAll);
	if (sphtmlAll == NULL)
		goto Error;
	hr = sphtmlAll->item(varName, varIndex, &spdispElem);
	if (spdispElem == NULL)
	{
		hr = E_NOINTERFACE;
		goto Error;
	}

	spdispElem->QueryInterface(__uuidof(IHTMLElementCollection), (void **) &sphtmlColl);
	if (sphtmlColl)
	{
		if (pbCollection)
			*pbCollection = TRUE;
#ifdef _DEBUG
		else
		{
			TRACE(traceHtml, 0, "Warning: duplicate IDs or NAMEs.\n");
			ATLASSERT(FALSE);
		}
#endif

	}
Error:
	if (SUCCEEDED(hr))
	{
		*ppdisp = spdispElem;
		if (spdispElem)
			(*ppdisp)->AddRef();
	}
	return hr;
}

HWND CBaiDuRobotDlg::GetExplorerHWND()
{
	HWND hwnd = ::FindWindow("IEFrame","百度一下，你就知道 - Microsoft Internet Explorer");
	if (!hwnd)
	{
		AddToList("百度页面未找到");
		return NULL;
	}
	hwnd = g_find.GetChildWindow(hwnd,5);
	hwnd = g_find.GetChildWindow(hwnd,1);
	CString strShow = g_find.GetContrlClassName(hwnd);
	if (g_find.GetContrlClassName(hwnd) == "Internet Explorer_Server")
	{
		return hwnd;
	}
	return NULL;
}

//得到浏览器的对应窗口句柄
HWND CBaiDuRobotDlg::GetExplorerHWND(HWND hwnd)
{
	hwnd = g_find.GetChildWindow(hwnd,5);
	hwnd = g_find.GetChildWindow(hwnd,1);
	CString strShow = g_find.GetContrlClassName(hwnd);
	if (g_find.GetContrlClassName(hwnd) == "Internet Explorer_Server")
	{
		return hwnd;
	}
	return NULL;
}

void CBaiDuRobotDlg::GetElementCollection(CString tagName,IHTMLElementCollection** pSpecialColl,IHTMLDocument2* pDoc)
{
	IHTMLElementCollection* pElemColl = NULL;
	IDispatch* pElemDisp = NULL;
	IHTMLElement* pElem=NULL;
	if (pDoc != NULL)
	{
		pDoc->get_all(&pElemColl);
		if (pElemColl != NULL)
		{
			_variant_t var(tagName); 
			pElemColl->tags(var,&pElemDisp);	//指定TAG集合
			if (pElemDisp != NULL)
			{
				*pSpecialColl = (IHTMLElementCollection*)pElemDisp;
				pElemColl->Release();
				return;
			}
			pElemColl->Release();
		}
	}
}
void CBaiDuRobotDlg::OnBnClickedBtnStart()
{
	m_bExitFlag = false;
	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
	AfxBeginThread((AFX_THREADPROC)ThreadFunc,(LPVOID)this);
}

void CBaiDuRobotDlg::OnBnClickedBtnStop()
{
	m_bExitFlag = true;
}

void CBaiDuRobotDlg::InitListCtrl()
{
	DWORD dwSytle=::GetWindowLong(m_list_show.m_hWnd,GWL_STYLE);
	// 设置为报表形式
	SetWindowLong(m_list_show.m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle = m_list_show.GetExtendedStyle();
	m_list_show.SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	::SendMessage(m_list_show.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	m_list_show.MoveWindow(7,160,500,160);
	m_list_show.InsertColumn(0,_T("时间"),LVCFMT_LEFT,90);
	m_list_show.InsertColumn(1,_T("信息"),LVCFMT_LEFT,250);
}


void CBaiDuRobotDlg::WriteLog(CString srcStr,int iFlag)
{
	CTime currTime = CTime::GetCurrentTime();
	try
	{
		CString filepath = "";
		CString destStr = "";
		CString folder = "";
		CString filename = "";
		CString strAppPath = "";

		folder = "log";
		filename.Format("%s_log.txt",currTime.Format("%Y%m%d"));

		CString folderName = "";
		folderName.Format("%s%s",m_strAppPath,folder);
		if(_access(folderName,0)!=0)
		{
			if( mkdir(folderName) != 0 )//创建目录
			{
				return;
			}
		}					
		destStr.Format("%s	%s",currTime.Format("%Y-%m-%d %H:%M:%S"),srcStr);
		filepath.Format("%s\\%s",folderName,filename);
		FILE *fp;
		char szfile[1000]={0};
		sprintf(szfile,filepath);
		fp=fopen(szfile,"a+");
		fprintf(fp,"%s\r\n",destStr);
		fclose(fp);
	}
	catch (...)
	{
	}
}


int CBaiDuRobotDlg::GetAppPath(CString &strAppPath)
{
	int iRet = -3;
	try
	{
		TCHAR szBuff[_MAX_PATH];
		VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szBuff, _MAX_PATH));
		strAppPath.Format("%s",szBuff);
		int pos=strAppPath.ReverseFind('\\');
		strAppPath=strAppPath.Left(pos+1);		
		iRet = 0;
	}
	catch (...)
	{

	}
	return iRet;	
}
HWND gl_hwnd = NULL;
void CBaiDuRobotDlg::OnBnClickedButton1()
{
	IHTMLDocument2* pDoc = GetDocInterface(gl_hwnd);
	if (!pDoc)
	{
		AddToList("获取HTML控制失败");
		return;
	}
	AddToList("已经获取HTML控制权");
	IHTMLElement *pElement = NULL;
	
	GetElement("dfs7",&pElement,pDoc);
	if (pElement == NULL)
	{
		AddToList("找不到对应标签");
		return;
	}
	CString strInnerText = GetInnerText(pElement);
	AddToList(strInnerText);
	
	RECT rt;
	HTMLElementRect(pElement, rt, 1000);
	RECT rectTotal;
	::GetWindowRect(gl_hwnd, &rectTotal);
	
	rt.top += rectTotal.top;
	rt.bottom += rectTotal.top;
	rt.left += rectTotal.left;
	rt.right += rectTotal.left;
	
	CRect rect(rt);	

	int pX = rect.left+30;
	int pY = rect.top + 5;
	//Sleep(100);
	//pY = 529;
	CPoint pt;
	GetCursorPos(&pt);
	::SetCursorPos(pX,pY);
	//Sleep(100);
	//mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	//Sleep(200);
	
	CString strInfo = "";
	strInfo.Format("%ld, %ld, %ld, %ld", rect.left, rect.top, rect.Width(), rect.Height());
	AddToList(strInfo);
}

IHTMLDocument2* CBaiDuRobotDlg::GetDocInterface(HWND hWnd) 
{
	// 我们需要显示地装载OLEACC.DLL,这样我们才知道有没有安装MSAA

	HINSTANCE hInst = ::LoadLibrary( _T("OLEACC.DLL") );
	IHTMLDocument2* pDoc2=NULL;
	if ( hInst != NULL ){
		if ( hWnd != NULL ){
			CComPtr<IHTMLDocument> spDoc=NULL;
			LRESULT lRes;

			UINT nMsg = ::RegisterWindowMessage( _T("WM_HTML_GETOBJECT") );
			::SendMessageTimeout( hWnd, nMsg, 0L, 0L, SMTO_ABORTIFHUNG, 1000, (DWORD*)&lRes );

			LPFNOBJECTFROMLRESULT pfObjectFromLresult = (LPFNOBJECTFROMLRESULT)::GetProcAddress( hInst, ("ObjectFromLresult") );
			if ( pfObjectFromLresult != NULL ){
				HRESULT hr;
				hr=pfObjectFromLresult(lRes,IID_IHTMLDocument,0,(void**)&spDoc);
				if ( SUCCEEDED(hr) ){
					CComPtr<IDispatch> spDisp;
					CComQIPtr<IHTMLWindow2> spWin;
					spDoc->get_Script( &spDisp );
					spWin = spDisp;
					spWin->get_document( &pDoc2 );
				}
			}
		}
		::FreeLibrary(hInst);
	} 
	else{//如果没有安装MSAA
		AfxMessageBox(_T("请您安装Microsoft Active Accessibility"));
	}
	//CoUninitialize();
	return pDoc2;
}
void CBaiDuRobotDlg::OnBnClickedButton2()
{
	IHTMLDocument2* pDoc = GetDocInterface(gl_hwnd);
	if (!pDoc)
	{
		AddToList("获取HTML控制失败");
		return;
	}
	IHTMLElement2 *pElement = NULL;
//	pDoc->get_body(&pElement);
	IHTMLElement *pBody = NULL;
	pDoc->get_body(&pBody);
	pBody->QueryInterface(&pElement);
	VARIANT t;
	t.vt = VT_INT;
	t.iVal = 5;
	long lTop = 0;
	long lHeight = 0;
	pElement->get_scrollHeight(&lTop);
	pElement->get_clientHeight(&lHeight);
	CString strInfo = "";
	strInfo.Format("%ld, %ld", lTop, lHeight);
	AddToList(strInfo);
}

void CBaiDuRobotDlg::OnBnClickedButton3()
{
	CString strBaidu = "www.baidu.com";
	strBaidu.Format("http://www.baidu.com/s?wd=%s",URLEncode("租车"));
	STARTUPINFO vStartupInfo;
	PROCESS_INFORMATION vProcessInfo;
	
	bool bRet = StartExplorer(strBaidu, vStartupInfo, vProcessInfo);
	if (!bRet)
	{
		AddToList("启动IE失败");
		return;
	}
	Sleep(2500);
	HWND hwnd = GetMainHwnd(vProcessInfo.dwProcessId);
	gl_hwnd = GetExplorerHWND(hwnd);
	//ShellExecute(NULL, "open", "IEXPLORE", strBaidu, 0, SW_SHOWMAXIMIZED);
}

void CBaiDuRobotDlg::OnClose()
{
	if (gl_hwnd)
	{
		//::PostMessage(gl_hwnd, WM_CLOSE, 0, 0);
	}
	CDialog::OnClose();
}

inline BYTE toHex(const BYTE &x)
{
	return x > 9 ? x + 55: x + 48;
}
inline BYTE toByte(const BYTE &x)
{
	return x > 57? x - 55: x - 48;
}
CString CBaiDuRobotDlg::URLEncode(CString sIn)
{
	CString sOut;
	const int nLen = sIn.GetLength() + 1;
	register LPBYTE pOutTmp = NULL;
	LPBYTE pOutBuf = NULL;
	register LPBYTE pInTmp = NULL;
	LPBYTE pInBuf =(LPBYTE)sIn.GetBuffer(nLen);
	//alloc out buffer
	pOutBuf = (LPBYTE)sOut.GetBuffer(nLen*3);

	if(pOutBuf)
	{
		pInTmp   = pInBuf;
		pOutTmp = pOutBuf;
		// do encoding
		while (*pInTmp)
		{
			if(isalnum(*pInTmp) || '-'==*pInTmp || '_'==*pInTmp || '.'==*pInTmp)
				*pOutTmp++ = *pInTmp;
			else if(isspace(*pInTmp))
				*pOutTmp++ = '+';
			else
			{
				*pOutTmp++ = '%';
				*pOutTmp++ = toHex(*pInTmp>>4);//高4位
				*pOutTmp++ = toHex(*pInTmp%16);//低4位
			}
			pInTmp++;
		}
		*pOutTmp = '\0';
		sOut.ReleaseBuffer();
	}
	sIn.ReleaseBuffer();

	return sOut;
}

ELEMENT_TAG_ID GetElementTagID(IHTMLElement* pElement)
{
	ELEMENT_TAG_ID tagid = TAGID_NULL;

	if( pElement )
	{
		LPDISPATCH pDoc2 = NULL;
		IMarkupServices* pMarkup = NULL;
		if( SUCCEEDED(pElement->get_document(&pDoc2)) && pDoc2 )
		{
			if( SUCCEEDED(pDoc2->QueryInterface(&pMarkup)) && pMarkup )
			{
				pMarkup->GetElementTagId(pElement, &tagid);
				pMarkup->Release();
			}
			pDoc2->Release();
		}
	}

	return tagid;
}

bool rawframeElement(LPDISPATCH pHTMLWnd, IHTMLFrameBase** ppHTMLFrame)
{
	if( pHTMLWnd==NULL || ppHTMLFrame==NULL ) return false;
	*ppHTMLFrame=NULL;

	IHTMLWindow4* pWnd4 = NULL;
	pHTMLWnd->QueryInterface(&pWnd4);

	if( pWnd4==NULL ) return false;

	HRESULT hr = pWnd4->get_frameElement(ppHTMLFrame);
	if( SUCCEEDED(hr) && *ppHTMLFrame)
	{

		pWnd4->Release();
		return true;
	}
	else
	{
		pWnd4->Release();
		*ppHTMLFrame = NULL;
		return false;
	}

}
//得到对应标签的坐标信息
bool CBaiDuRobotDlg::HTMLElementRect(IHTMLElement* pElement, RECT& rect, int level)
{
	memset(&rect,0, sizeof(rect));
	if( pElement==NULL ) return false;

	LPDISPATCH lpDisp = NULL;
	IHTMLDocument2* pDoc2 = NULL;
	IHTMLWindow2* pWnd2 = NULL;
	IHTMLFrameBase* pHFB = NULL;
	IHTMLElement* pElement1=NULL, *pTmp=NULL, *pHtml=NULL;
	IHTMLElement2* pElement2=NULL, *pHtml2=NULL;

	pElement->get_offsetWidth(&rect.right);
	pElement->get_offsetHeight(&rect.bottom);

	long lx=0,ly=0;BSTR bsVal=NULL;bool bRet=false;
	for(int i=0; pElement && !bRet; i++)
	{
		if( level>0 && i>=level ) break;

		pElement->get_offsetLeft(&lx);
		pElement->get_offsetTop(&ly);
		rect.left += lx;rect.top+=ly;

		int specid = GetElementTagID(pElement);
		if(9 == specid || 10 == specid)
		{
			//for ie6 or later standards-compliant mode
			pElement->get_parentElement(&pTmp);
		}
		else
		{
			pElement->get_offsetParent(&pTmp);
		}

		if( level!=0 && pTmp==NULL )
		{
			pElement->get_document(&lpDisp);
			pElement->Release();pElement=NULL;
			if( lpDisp==NULL ) break;
			lpDisp->QueryInterface(&pDoc2);
			lpDisp->Release();lpDisp=NULL;
			pDoc2->get_parentWindow(&pWnd2);
			pDoc2->Release();pDoc2=NULL;
			if( pWnd2==NULL ) break;
			rawframeElement(pWnd2,&pHFB);
			if( pHFB==NULL ) break;
			pHFB->QueryInterface(&pElement);
			pHFB->Release();pHFB=NULL;
		}
		else
		{
			pElement->Release();pElement=pTmp;pTmp=NULL;
		}
		if( pElement==NULL ) break;
		pElement->QueryInterface(&pElement2);
		ATLASSERT(pElement2);
		pElement2->get_clientLeft(&lx);
		pElement2->get_clientTop(&ly);
		rect.left+=lx;rect.top+=ly;

		pElement2->get_scrollLeft(&lx);
		pElement2->get_scrollTop(&ly);
		pElement2->Release();pElement2=NULL;
		rect.left-=lx;rect.top-=ly;
	}
	if( pElement ) 
	{
		pElement->Release();pElement=NULL;
	}

	rect.right  += rect.left;
	rect.bottom += rect.top;

	return true;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CBaiDuRobotDlg::GetKeyString(CString &srcStr,CString strBefore,CString strAfter)
{
	int iPosBefore = 0;
	int iPosAfter = 0;
	iPosBefore = srcStr.Find(strBefore) + strBefore.GetLength();
	if (iPosBefore < strBefore.GetLength())
	{
		return "";
	}
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

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CBaiDuRobotDlg::StringSplit(CString srcStr,map<CString, int>& mapInfo,char splitChar)
{
	try
	{
		int idx1 = 0;
		int idx2 = 0;
		int iCount = 0;
		CString strKey  = "";
		idx2 = srcStr.Find(splitChar, idx1);  //找第一个记录 
		while(idx2 != -1)						//如果找到
		{ 
			strKey = srcStr.Mid(idx1,idx2 - idx1); //取到一个记录
			strKey.Trim();
			if (strKey != "")
			{
				mapInfo[strKey] = 1;
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