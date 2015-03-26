// AmazonRobotDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AmazonRobot.h"
#include "AmazonRobotDlg.h"
#include <WinInet.h>
#include <io.h>
#include <direct.h>
#include <mshtml.h>
#include <afxinet.h>
#pragma comment(lib,"wininet.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void WriteLog(CString srcStr,int iFlag);
int GetAppPath(CString &strAppPath);

int GetAppPath(CString &strAppPath)
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
/************************************************************************/
/*                                                                      */
/************************************************************************/
//写日志
void WriteLog(CString srcStr,int iFlag)
{
	CTime currTime = CTime::GetCurrentTime();
	try
	{
		CString filepath = "";
		CString destStr = "";
		CString folder = "";
		CString filename = "";
		CString strAppPath = "";
		if (iFlag == 0)			//注册成功列表
		{
			folder = "log";
			filename.Format("%s_success.txt",currTime.Format("%Y%m%d"));
		}
		else if(iFlag == 1)		//信息日志
		{
			folder = "log";
			filename.Format("%s_log.txt",currTime.Format("%Y%m%d"));
		}
		else if ( iFlag == 2 )	//缓冲日志
		{
			folder = "log";
			filename.Format("%s_fail.txt",currTime.Format("%Y%m%d"));
		}
		else if ( iFlag == 3 )	//缓冲日志
		{
			folder = "log";
			filename.Format("%s_insertErrorAcc.txt",currTime.Format("%Y%m%d"));
		}
		else			//异常日志 (3)
		{
			folder = "exception";
			filename.Format("%s_exception.txt",currTime.Format("%Y%m%d"));
		}
		GetAppPath(strAppPath);
		CString folderName = "";
		folderName.Format("%s%s",strAppPath,folder);
		if(_access(folderName,0)!=0)
		{
			if( mkdir(folderName) != 0 )//创建目录
			{
				return;
			}
		}			
		if (iFlag == 1)
		{
			destStr.Format("%s	%s",CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"),srcStr);
		}
		else
		{
			destStr.Format("%s",srcStr);
		}		
		filepath.Format("%s\\%s",folderName,filename);
		FILE *fp;
		char szfile[1000]={0};
		sprintf(szfile,filepath);
		fp=fopen(szfile,"a+");
		fprintf(fp,"%s\n",destStr);
		fclose(fp);
	}
	catch (...)
	{
	}
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
#define WM_CLICKMSG WM_USER + 10
#define WM_CLICKMSGEX WM_USER + 11
#define WM_RUNJAVASCRIPT WM_USER + 12
#define WM_GETHTMLCONTENT WM_USER + 13
#define WM_GETHTMLCONTENTEX WM_USER + 14
#define WM_SETELEMENTVALUE  WM_USER + 15
#define WM_SETSENDTYPEANDBILL		WM_USER + 16
#define WM_EXITCODEINFO WM_USER + 17
#define WM_GETELEMENTVALUE WM_USER + 18
#define WM_CHECKHASSPEEDSEND WM_USER + 19
int gl_iCompeleteFlag = 0;
CString gl_strContent = "";
CString gl_strSetValue = "";
int gl_iExitCode = -1;
AmazonOrderStru gl_AmazonStr;
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


// CAmazonRobotDlg 对话框

BEGIN_DHTML_EVENT_MAP(CAmazonRobotDlg)
END_DHTML_EVENT_MAP()

BEGIN_EVENTSINK_MAP(CAmazonRobotDlg, CDHtmlDialog)
	//ON_EVENT(CAmazonRobotDlg, AFX_IDC_BROWSER, 250 /* BeforeNavigate2 */, _OnBeforeNavigate2, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
END_EVENTSINK_MAP()

BEGIN_DISPATCH_MAP(CAmazonRobotDlg, CDHtmlDialog)
	DISP_FUNCTION(CAmazonRobotDlg, "SetValue", SetValue, VTS_NONE, VTS_BSTR)
END_DISPATCH_MAP()

CAmazonRobotDlg::CAmazonRobotDlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CAmazonRobotDlg::IDD, CAmazonRobotDlg::IDH, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	EnableAutomation();
}

void CAmazonRobotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAmazonRobotDlg, CDHtmlDialog)
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CAmazonRobotDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CAmazonRobotDlg::OnBnClickedButton2)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_RADIO1, &CAmazonRobotDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CAmazonRobotDlg::OnBnClickedRadio2)
END_MESSAGE_MAP()

void CAmazonRobotDlg::InitBillType()
{
	m_mapBillType["图书"] = "1";
	m_mapBillType["音像"] = "2";
	m_mapBillType["游戏"] = "3";
	m_mapBillType["软件"] = "4";
	m_mapBillType["资料"] = "5";
	m_mapBillType["办公用品"] = "6";
	m_mapBillType["洗涤用品"] = "7";
	m_mapBillType["化妆品"] = "8";
	m_mapBillType["体育用品"] = "9";
	m_mapBillType["劳保用品"] = "10";
	m_mapBillType["服装"] = "11";
	m_mapBillType["玩具"] = "12";
	m_mapBillType["饰品"] = "13";
	m_mapBillType["手机"] = "14";
	m_mapBillType["家电"] = "15";
	m_mapBillType["配件"] = "16";
	m_mapBillType["数码产品"] = "17";
	m_mapBillType["汽车用品"] = "18";
	m_mapBillType["厨具"] = "19";
	m_mapBillType["鞋靴"] = "20";
	m_mapBillType["钟表"] = "21";
	m_mapBillType["食品"] = "22";
	m_mapBillType["教材"] = "23";
	/*
	<select name="chinaInvoiceCategoryID" id="chinaInvoiceCategoryID" style="width: 100">
                      <option value=''>请选择</option>
<option value='1'>图书</option><option value='2'>音像</option><option value='3'>游戏</option>
<option value='4'>软件</option><option value='5'>资料</option><option value='6'>办公用品</option>
<option value='7' selected="true">洗涤用品</option><option value='8'>化妆品</option><option value='9'>体育用品</option><option value='10'>劳保用品</option>
<option value='11'>服装</option><option value='12'>玩具</option><option value='13'>饰品</option><option value='14'>手机</option>
<option value='15'>家电</option><option value='16'>配件</option><option value='17'>数码产品</option><option value='18'>汽车用品</option><option value='19'>厨具</option>
<option value='20'>鞋靴</option><option value='21'>钟表</option><option value='22'>食品</option><option value='23'>教材</option>                    </select>
	*/
}
// CAmazonRobotDlg 消息处理程序

BOOL CAmazonRobotDlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();

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
	
#ifdef _DEBUG
#else			//发布版要控制权限
	if (GetPower() != 0)
	{
		exit(0);
	}
#endif

	CString strPath = "c:\\test";
	if (_access(strPath,0) != 0)
	{
		mkdir(strPath);
	}
	SetHostFlags(DOCHOSTUIFLAG_NO3DBORDER|DOCHOSTUIFLAG_DIALOG);
	OnBnClickedRadio2();
	InitBillType();
	SetExternalDispatch(GetIDispatch(TRUE));
	
	if (gl_AmazonStr.iAutoRun == 1)
	{
		GetDlgItem(IDC_EDIT1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO1)->ShowWindow(SW_HIDE);
		AfxBeginThread((AFX_THREADPROC)ThreadFunc,(LPVOID)this);
	}
	else
	{
		Navigate("www.amazon.cn");
	}
	//通过此定时器来关闭JS错误窗口
	SetTimer(3,500,NULL);
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CAmazonRobotDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDHtmlDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CAmazonRobotDlg::OnPaint()
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
		CDHtmlDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CAmazonRobotDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CAmazonRobotDlg::GetKeyString(CString &srcStr,CString strBefore,CString strAfter)
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

void CAmazonRobotDlg::_OnBeforeNavigate2(LPDISPATCH pDisp, VARIANT FAR* URL, VARIANT FAR* Flags, VARIANT FAR* TargetFrameName, VARIANT FAR* PostData, VARIANT FAR* Headers, BOOL FAR* Cancel)
{
	//CString strUrl = (char*)(_bstr_t)(*URL);
	//WriteLog(strUrl,1);
	CDHtmlDialog::_OnBeforeNavigate2(pDisp,URL,Flags,TargetFrameName,PostData,Headers,Cancel);
}

BOOL CAmazonRobotDlg::OnAmbientProperty(COleControlSite* pSite, DISPID dispid, VARIANT* pvar)
{ 
	// TODO: Add your specialized code here and/or call the base class 
// 	if (dispid == DISPID_AMBIENT_DLCONTROL) 
// 	{
// 		pvar->vt = VT_I4; 
// 		pvar->lVal = pvar->lVal & ~DLCTL_DLIMAGES; 
// 		return TRUE;
// 	} 
	return CDHtmlDialog::OnAmbientProperty(pSite, dispid, pvar); 
} 
void CAmazonRobotDlg::OnBnClickedButton1()
{

	gl_AmazonStr.strUserID = "wang22011211@126.com";
	gl_AmazonStr.strPwd = "A860829";

	gl_AmazonStr.strRecver = "张月月";
	gl_AmazonStr.strAddr = "开发区盛达街9号金融广场11层B区";
	gl_AmazonStr.strProvince = "广东";
	gl_AmazonStr.strCity = "广州市";
	gl_AmazonStr.strTown = "天河区";
	gl_AmazonStr.strPostCode = "32";
	gl_AmazonStr.strPhone = "022-66286223";
	gl_AmazonStr.bBill = true;
	gl_AmazonStr.strBillHeader = "随便咯";
	gl_AmazonStr.strBillContent = "1";
	gl_AmazonStr.eSendType = NightOrOther;
	gl_AmazonStr.strGoodNos = "B0011C1GJK";//B002YGSOS8//1594483078
	gl_AmazonStr.eQueryType = Last30;
	gl_AmazonStr.bCanNormalSend = FALSE;

	int iFlag = ((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck();
	if (iFlag)
	{
		gl_AmazonStr.iOpFlag = 2;
	}
	else
	{
		gl_AmazonStr.iOpFlag = 1;
	}
	AfxBeginThread((AFX_THREADPROC)ThreadFunc,(LPVOID)this);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CAmazonRobotDlg::GetItemValue(IHTMLElement *pElement)
{
	CString csValue = "";
	if (pElement != NULL)
	{
		CString itemtype = "value";
		VARIANT value;
		BSTR bstrType = itemtype.AllocSysString();
		pElement->getAttribute(bstrType,0,&value);
		::SysFreeString(bstrType);
		if (value.vt == VT_BSTR)
		{
			csValue.Format ("%s",(const char*)(_bstr_t)(_variant_t)value);
		}
	}
	return csValue;
}

void CAmazonRobotDlg::OnBnClickedButton2()
{
	gl_AmazonStr.strUserID = "58357601@qq.com";
	gl_AmazonStr.strPwd = "shizisen861225";
	gl_AmazonStr.strRecver = "陈军";
	gl_AmazonStr.strAddr = "开发区盛达街9号金融广场11层B区";
	gl_AmazonStr.strProvince = "天津";
	gl_AmazonStr.strCity = "天津市";
	gl_AmazonStr.strTown = "塘沽区";
	gl_AmazonStr.strPostCode = "300457";
	gl_AmazonStr.strPhone = "022-66286223";
	gl_AmazonStr.bBill = true;
	gl_AmazonStr.strBillHeader = "随便咯";
	gl_AmazonStr.strBillContent = "家电";
	gl_AmazonStr.eSendType = NightOrOther;
	gl_AmazonStr.strGoodNos = "0140623361";//B002YGSOS8//1594483078
	gl_AmazonStr.eQueryType = Last30;

	CString strJs = "";
	strJs.Format("document.getElementById(\"enterAddressStateOrRegion\").value = \"%s\";resetCity();resetDistrict();getCities();",gl_AmazonStr.strProvince);
	//AfxMessageBox(strJs);
	RunJavascript(strJs);
	Sleep(20);
	strJs.Format("document.getElementById(\"enterAddressCity\").value = \"%s\";resetDistrict();getDistricts();",gl_AmazonStr.strCity);
	RunJavascript(strJs);
	Sleep(20);
	strJs.Format("document.getElementById(\"enterAddressDistrictOrCounty\").value = \"%s\";",gl_AmazonStr.strTown);
	RunJavascript(strJs);
	SetItemValue("enterAddressFullName",gl_AmazonStr.strRecver);
	SetItemValue("enterAddressAddressLine1",gl_AmazonStr.strAddr);
	SetItemValue("enterAddressPostalCode",gl_AmazonStr.strPostCode);
	SetItemValue("enterAddressPhoneNumber",gl_AmazonStr.strPhone);	
	return;
	bool bRet = CheckHasSpeedSend();
	AfxMessageBox(bRet?"1":"0");
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	//needChinaInvoice
	//chinaInvoiceTitle
	//chinaInvoiceCategoryID   //需要计算序号
	int iType = GetDlgItemInt(IDC_EDIT1);
	/*
	IHTMLElement* pElement = NULL;
	IDispatch* pElemDisp = NULL;
	GetElement("",&pElement);
	IHTMLElementCollection* pElemColl = NULL;
	IHTMLElement* pElem=NULL;
	GetElementCollectionEx("A",&pElemColl,pElement);
	if(pElemColl!=NULL)
	{
		long length;
		pElemColl->get_length(&length);	//指定TAG集合长度
		int iCount = 0;
		for (long i=0;i<length;i++)
		{
			_variant_t varBase(0);
			_variant_t varIndex(i);
			HRESULT hr = pElemColl->item(varIndex, varBase, &pElemDisp);
			if (SUCCEEDED(hr) && pElemDisp!=NULL)
			{
				hr = pElemDisp->QueryInterface(IID_IHTMLElement,(void**)&pElem);
				if(SUCCEEDED(hr) && pElem!=NULL)
				{
					BSTR bstrType;
					//order.477:93160512976.deliveryTimePreference.weekdays_cn_71874
					pElem->get_id(&bstrType);
					CString strType(bstrType);		
					::SysFreeString(bstrType);
					strType.MakeLower();
					if(strType.Find("deliverytimepreference")>0)
					{
						iCount++;
						if ((int)type == iCount)
						{
							pElem->click();
						}																		
					}
					pElem->Release();
				}
				pElemDisp->Release();
			}
		}
		pElemColl->Release();
	}
*/
//	return;
	
	CString strJsEx = "";
	CString strIndex = "";
	//location-text
	
	//strIndex.Format("jQuery( '.tracking td' ).each( function(){alert(jQuery(this).text()); jQuery(this).find('.track').each(function(){this.click();}) })");
	GetDlgItemText(IDC_EDIT2,strIndex);
	//strIndex.Format("jQuery( '.item-action-button' ).each( function(){jQuery(this).next().find('.track').each( function(){this.click();})})");
	ExecJavascript(strIndex);
// 	GetElementValue("location-text-id");
// 	IHTMLElement* pElement = NULL;
// 	GetElement("location-text-id",&pElement);
// 	if (!pElement)
// 	{
// 		return;
// 	}
// 	GetContentEx(pElement);
// 	AfxMessageBox(gl_strContent);
	//window.external.AddFavorite(Url,Title)
	return;
	map<CString,CString>::iterator it = m_mapBillType.find("家电");
	if (it != m_mapBillType.end())
	{
		strJsEx.Format("setFormAction('SPCConfirmPurchaseForm', '/gp/checkout/confirm/select.html/ref=ox_spc_place_order_top'); return validateChinaInvoice();",it->second);
		ExecJavascript(strJsEx);
	}
	
	return;
	SetSendType((_SendTime)iType);
	return;
	//省 enterAddressStateOrRegion
	//市 enterAddressCity
	//区 enterAddressDistrictOrCounty
	//姓名 enterAddressFullName
	//地址 enterAddressAddressLine1
	//电话 enterAddressPostalCode
	//邮编 enterAddressPhoneNumber
	//配送 shipToThisAddress
	AmazonOrderStru objStr;
	objStr.strUserID = "58357601@qq.com";
	objStr.strPwd = "shizisen861225";
	objStr.strRecver = "金杉";
	objStr.strAddr = "开发区盛达街9号金融广场11层B区103";
	objStr.strProvince = "天津";
	objStr.strCity = "天津市";
	objStr.strTown = "塘沽区";
	objStr.strPostCode = "300457";
	objStr.strPhone = "022-66286223";
	//ConvertGBKToUtf8(objStr.strRecver);
	//resetCity).change(resetDistrict).change(getCities
	//resetDistrict
	strJs = "";
	strJs.Format("document.getElementById(\"enterAddressStateOrRegion\").value = \"%s\";resetCity();resetDistrict();getCities();",objStr.strProvince);
	ExecJavascript(strJs);
	Sleep(20);
	strJs.Format("document.getElementById(\"enterAddressCity\").value = \"%s\";resetDistrict();getDistricts();",objStr.strCity);
	ExecJavascript(strJs);
	Sleep(20);
	strJs.Format("document.getElementById(\"enterAddressDistrictOrCounty\").value = \"%s\";",objStr.strTown);
	ExecJavascript(strJs);
// 	IHTMLSelectElement* pElement = NULL;
// 	GetElement("enterAddressStateOrRegion",(IDispatch**)&pElement);
// 	if (!pElement)
// 	{
// 		return;
// 	}
// 	BSTR bstrStr = objStr.strProvince.AllocSysString();
// 	pElement->put_value(bstrStr);
// 	SetItemValue("enterAddressStateOrRegion",objStr.strProvince);
// 	Sleep(20);
// 	SetItemValue("enterAddressCity",objStr.strCity);
// 	Sleep(20);
// 	SetItemValue("enterAddressDistrictOrCounty",objStr.strTown);

	SetItemValue("enterAddressFullName",objStr.strRecver);
	SetItemValue("enterAddressAddressLine1",objStr.strAddr);
	SetItemValue("enterAddressPostalCode",objStr.strPostCode);
	SetItemValue("enterAddressPhoneNumber",objStr.strPhone);
	//CString strText = GetHtmlContent();
	//CString strJs = "alert('aa');document.getElementById('bb_atc_button').click();";
	//ExecJavascript(strJs);
	//AfxMessageBox("bb");
}
void CAmazonRobotDlg::ConvertGBKToUtf8(CString& strGBK) 
{
	int len=MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, NULL,0);
	unsigned short * wszUtf8 = new unsigned short[len+1];
	memset(wszUtf8, 0, len * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, (LPWSTR)wszUtf8, len);

	len = WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)wszUtf8, -1, NULL, 0, NULL, NULL); 
	char *szUtf8=new char[len + 1];
	memset(szUtf8, 0, len + 1);
	WideCharToMultiByte (CP_UTF8, 0, (LPWSTR)wszUtf8, -1, szUtf8, len, NULL,NULL);

	strGBK = szUtf8;
	delete[] szUtf8;
	delete[] wszUtf8;
	return ;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CAmazonRobotDlg::ConvertUtf8ToGBK(CString& strUtf8) 
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
/************************************************************************/
/*                                                                      */
/************************************************************************/
bool CAmazonRobotDlg::AddGoodsTocart(CString strGoodNo)
{
	CString strUrl = "";
	strUrl.Format("http://www.amazon.cn/mn/detailApp?asin=%s&%s",strGoodNo,FLAG_STRING);
	NavigateEx(strUrl);
	WaitForComplete();
	IHTMLElement* pElement = NULL;
	GetElement("bb_atc_button",&pElement);
	if (!pElement)
	{
		return false;
	}
	gl_iCompeleteFlag = 0;
	//ClickElement("bb_atc_button");
	ClickElementEx(pElement);
	pElement->Release();
	WaitForComplete();
	//商品添加到购物车
	pElement = NULL;
	GetElement("hl-confirm-text",&pElement);
	if (!pElement)
	{
		return false;
	}
	GetContentEx(pElement);
	pElement->Release();
	if ( gl_strContent.Find("商品添加到购物车") < 0 )
	{
		return false;
	}
	return true;
}


bool CAmazonRobotDlg::DoChange(AmazonOrderStru &objStr)
{
	CString strJs = "";
	IHTMLElement* pElement = NULL;
	CString strUrl = "";
	strUrl.Format("https://www.amazon.cn/gp/css/order/edit.html/ref=oss_pa?ie=UTF8&useCase=payment&orderID=%s",objStr.strOrderNo);
	NavigateEx(strUrl);
	WaitForComplete();
	SetItemValue("ap_email",objStr.strUserID);
	SetItemValue("ap_password",objStr.strPwd);
	GetElement("signInSubmit",&pElement);
	if (!pElement)
	{
		gl_iExitCode = LOGINPAGE_ERROR;
		return false;
	}
	gl_iCompeleteFlag = 0;
	ClickElementEx(pElement);
	pElement->Release();
	WaitForComplete();

	//查找标志，是否登录成功
	GetElement("current-instruments",&pElement);
	if (!pElement)
	{
		gl_iExitCode = GETCHANGE_ERROR;
		return false;
	}
	GetContentEx(pElement);
	pElement->Release();

	if (gl_strContent.Find("中国邮政")>0)//已经是中国邮政方式
	{
		return true;
	}
	GetElement("pm_domesticcards",&pElement);
	if (!pElement)
	{
		gl_iExitCode = GETCHANGE_ERROR;
		return false;
	}
	ClickElementEx(pElement);
	pElement->Release();

	//设置为中国邮政
	strJs.Format("document.getElementsByName(\"domesticCardsPaymentMethodId\")[0].value = \"43\";");
	RunJavascript(strJs);
	
	strJs.Format("document.getElementsByName(\"cpm.otherPaymentMethod\")[0].click();");
	gl_iCompeleteFlag = 0;
	RunJavascript(strJs);
	WaitForComplete();
	GetContent();
	if (gl_strContent.Find("您已成功变更了此订单") < 0)
	{
		//https://www.amazon.cn/gp/css/summary/edit.html?ie=UTF8&orderID=C02-1648374-3804857
		//跳转判断此订单是否已经准备发货
		gl_iExitCode = CHANGE_ERROR;
		return false;
	}
	return true;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
bool CAmazonRobotDlg::DoQuery(AmazonOrderStru &objStr)
{
	CString strJs = "";
	IHTMLElement* pElement = NULL;
	CString strUrl = "https://www.amazon.cn/ap/signin?_encoding=UTF8&openid.assoc_handle=cnflex&openid.return_to=https%3A%2F%2Fwww.amazon.cn%2Fgp%2Fcss%2Fhistory%2Forders%2Fview.html%3Fie%3DUTF8%26ref_%3Dya__6b&openid.mode=checkid_setup&openid.ns=http%3A%2F%2Fspecs.openid.net%2Fauth%2F2.0&openid.claimed_id=http%3A%2F%2Fspecs.openid.net%2Fauth%2F2.0%2Fidentifier_select&openid.pape.max_auth_age=900&openid.ns.pape=http%3A%2F%2Fspecs.openid.net%2Fextensions%2Fpape%2F1.0&openid.identity=http%3A%2F%2Fspecs.openid.net%2Fauth%2F2.0%2Fidentifier_select";
	NavigateEx(strUrl);
	WaitForComplete();
	SetItemValue("ap_email",objStr.strUserID);
	SetItemValue("ap_password",objStr.strPwd);
	GetElement("signInSubmit",&pElement);
	if (!pElement)
	{
		gl_iExitCode = LOGINPAGE_ERROR;
		return false;
	}
	gl_iCompeleteFlag = 0;
	ClickElementEx(pElement);
	pElement->Release();
	WaitForComplete();
	
	//查找标志，是否登录成功
	//orderFilter
	
	GetElement("order-dropdown-form",&pElement);
	if (!pElement)
	{
		gl_iExitCode = GETQUERY_ERROR;
		return false;
	}
	GetContentEx(pElement);
	pElement->Release();
	CString strFlag = "";	
	switch(gl_AmazonStr.eQueryType)
	{
	case Last30:
		strFlag = "last30";
		break;
	case Months6:
		strFlag = "months-6";
		break;
	case Year2011:
		strFlag = "year-2011";
		break;
	}
	//last30
	//months-6
	//year-2011
	//获取GROUPID
	CString strGroupID = GetKeyString(gl_strContent,"name=\"groupID\" value=\"","\"");
	int iIndex = 0;
	bool bHaveGetNo = false;
	int iTotalNum = 0;
	int iTotalPage = 35;
	int iCount = 0;

 	CString strGoodNo = "";
	CString strTemp = "";
	
	while(true)
	{
		strUrl.Format("https://www.amazon.cn/gp/css/history/orders/view.html/ref=oss_pagination?ie=UTF8&search=&startAtIndex=%d&orderFilter=%s&groupID=%s",iIndex,strFlag,strGroupID);
		NavigateEx(strUrl);
		WaitForComplete();
		GetElement("signInSubmit",&pElement);
		if (pElement)
		{
			SetItemValue("ap_email",objStr.strUserID);
			SetItemValue("ap_password",objStr.strPwd);
			gl_iCompeleteFlag = 0;
			ClickElementEx(pElement);
			pElement->Release();
			WaitForComplete();
		}
		iIndex += 10;
		Sleep(500);
		strJs.Format("var vIndex = 1;jQuery( '.item-action-button' ).each( function(){jQuery(this).next().find('.track').each(function(){ this.id='item-action-button-id-'+vIndex;vIndex++; this.click();})})");
		RunJavascript(strJs);
		GetElement("item-action-button-id-1",&pElement);
		if (pElement)
		{
			pElement->Release();
			Sleep(5000);
		}	
		strJs.Format("var vIndex = 1; jQuery( '.order-table' ).each( function(){this.id='order-table-id-'+vIndex;vIndex++;})");
		RunJavascript(strJs);
		Sleep(500);
		//循环读取保存
		int i = 0;
		CString strId = "";
		for (i=1;i<=10;i++)
		{
			strId.Format("order-table-id-%d",i);
			GetElement(strId,&pElement);
			if (!pElement)
			{
				return true;
			}
			GetContentEx(pElement);
			pElement->Release();
		
			CString strWrite = "";
			strTemp = GetKeyString(gl_strContent,"orderID=","\"");	//订单号
			strGoodNo = strTemp;
			strGoodNo.Trim();
			strWrite += strGoodNo +"\r\n";

			strTemp = gl_strContent.Mid(gl_strContent.Find("收件人")+22);
			strTemp = GetKeyString(strTemp,">","<");	//收件人
			strTemp.Trim();
			strTemp.Replace("&nbsp;","");
			strWrite += strTemp +"\r\n"+"\r\n";

			//item-title>Alice's Adventures in Wonderland </SPAN>
			strTemp = gl_strContent.Mid(gl_strContent.Find("item-title"));
			strTemp.Replace("</SPAN>","</span>");
			while(true)
			{
				
				CString strGoodName = GetKeyString(strTemp,"item-title>","</span>");
				if (strGoodName == "")
				{
					break;
				}
				strGoodName.Replace("<B>","");
				strGoodName.Replace("</B>","");
				strGoodName.Trim();
				strWrite += strGoodName +"\r\n";
				strTemp = strTemp.Mid(strTemp.Find(strGoodName)+strGoodName.GetLength());
				if (strTemp.Find("item-title") > 0)
				{
					strTemp = strTemp.Mid(strTemp.Find("item-title"));
				}
				else
				{
					break;
				}
			}
			
			strWrite += "\r\n";

			strTemp = gl_strContent.Mid(gl_strContent.Find("送货地址")+22);
			strTemp.MakeLower();
			strTemp = GetKeyString(strTemp,">","</span>");	//送货地址
			
			strTemp.Replace("<br>","\t");
			strTemp.Trim();
			strWrite += strTemp +"\r\n";
			
			strTemp = gl_strContent.Mid(gl_strContent.Find("送货方式")+22);
			strTemp = GetKeyString(strTemp,">","<");	//送货方式
			strTemp.Trim();
			strWrite += strTemp +"\r\n";

			strTemp = GetKeyString(gl_strContent,"current-state>","<");		//当前状态
			strTemp.Trim();
			strWrite += strTemp +"\r\n";


			strTemp = gl_strContent.Mid(gl_strContent.Find("订单总计")+22);
			strTemp = GetKeyString(strTemp,">","<");	//订单总计
			strTemp.Trim();
			strWrite += strTemp +"\r\n";
			strWrite += "/************************************************************************/\r\n";

			if (gl_strContent.Find("track-package-parent") > 0)
			{
				CString strTrack = gl_strContent.Mid(gl_strContent.Find("track-package-parent"));

				strTemp = strTrack.Mid(strTrack.Find("包裹状态"));
				strTemp = strTemp.Mid(strTemp.Find("info-data"));
				strTemp = GetKeyString(strTemp,">","<");	//包裹状态
				strTemp.Trim();
				strWrite += strTemp +"\r\n";

				strTemp = strTrack.Mid(strTrack.Find("配送公司"));
				strTemp = strTemp.Mid(strTemp.Find("info-data"));
				strTemp = GetKeyString(strTemp,">","<");	//配送公司
				strTemp.Trim();
				strWrite += strTemp +"\r\n";

				strTemp = strTrack.Mid(strTrack.Find("包裹单号"));
				strTemp = strTemp.Mid(strTemp.Find("info-data"));
				strTemp = GetKeyString(strTemp,">","<");	//包裹单号
				strTemp.Trim();
				strWrite += strTemp +"\r\n";

				strTemp = strTrack.Mid(strTrack.Find("最新活动"));
				strTemp = strTemp.Mid(strTemp.Find("info-data"));
				strTemp.MakeLower();
				//<SPAN class=alertgreen>广州总站020-34061875/34061859/34061776/34061514 </SPAN><BR>配送成功 - Mar 16, 2011&nbsp;6:00:00 PM</SPAN> </TD>
				strTemp = GetKeyString(strTemp,">","</td");	//最新活动
				strTemp = strTemp.Mid(strTemp.Find(">")+1);
				strTemp.Replace("<br>","\t");
				strTemp.Replace("</span>","\t");
				strTemp.Replace("&nbsp;"," ");
				strTemp.Trim();
				strWrite += strTemp +"\r\n";
			}
			
			strWrite.Replace("\t\t","\t");
			CString strFile = "";
			strFile.Format("C:\\test\\%s.txt",strGoodNo);
			
			CFile file;
			file.Open(strFile,CFile::modeCreate | CFile::modeWrite);
			file.Write(strWrite,strWrite.GetLength());
			file.Close();
			//解析,保存 
		}

	}
	return true;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
bool CAmazonRobotDlg::DoOrder(AmazonOrderStru &objStr)
{
	CString strJs = "";
	IHTMLElement* pElement = NULL;
	//进入首页，带标志
	CString strUrl = "";
// 	strUrl.Format("http://www.amazon.cn/?%s",FLAG_STRING);
// 	NavigateEx(strUrl);
// 	WaitForComplete();
	//此处改为点击进入，以保证REFER不丢失
	//进入购物车
	GetElement("hlb-next-steps",&pElement);
	if (!pElement)
	{
		gl_iExitCode = PROCEEDTOCHECKOUT_ERROR;
		return false;
	}
	pElement->Release();
	gl_iCompeleteFlag = 0;
	strJs.Format("document.getElementById(\"hlb-next-steps\").getElementsByTagName(\"A\")[1].click();");
	RunJavascript(strJs);
	WaitForComplete();
	//进入结算中心
	
// 	GetElement("proceedToCheckout",&pElement);
// 	if (!pElement)
// 	{
// 		gl_iExitCode = PROCEEDTOCHECKOUT_ERROR;
// 		return false;
// 	}
// 	gl_iCompeleteFlag = 0;
// 	ClickElementEx(pElement);
// 	pElement->Release();
// 	WaitForComplete();
	//登录
	SetItemValue("ap_email",objStr.strUserID);
	SetItemValue("ap_password",objStr.strPwd);
	
	GetElement("signInSubmit",&pElement);
	if (!pElement)
	{
		gl_iExitCode = LOGINPAGE_ERROR;
		return false;
	}
	gl_iCompeleteFlag = 0;
	ClickElementEx(pElement);
	pElement->Release();
	WaitForComplete();
	//跳转至自定义地址处
	GetElement("SPCConfirmPurchaseForm",&pElement);
	if (pElement)	//如果存在找地址
	{
		GetContentEx(pElement);
		pElement->Release();
		CString strTemp = gl_strContent.Mid(gl_strContent.Find("送货地址"));
		strTemp.Replace("INPUT","input");
		gl_iCompeleteFlag = 0;
		strTemp = GetKeyString(strTemp,"input",">");
		CString strID = "";
		if (strTemp.Find("name=\"") > 0)
		{
			strID = GetKeyString(strTemp,"name=\"","\"");
		}
		else
		{
			strID = strTemp.Mid(strTemp.Find("name=")+5);
		}
		strID.Trim();
		GetElement(strID,&pElement);
		if (!pElement)
		{
			gl_iExitCode = GETSNDADDRINPUT_ERROR;
			return false;
		}
		gl_iCompeleteFlag = 0;
		ClickElementEx(pElement);
		pElement->Release();
		WaitForComplete();
	}
	else
	{		
		GetElement("purchase-shipping-address",&pElement);
		if (!pElement)
		{
			GetElement("enterAddressPhoneNumber",&pElement);
			if (!pElement)
			{
				gl_iExitCode = LOGIN_FAILED;
				return false;
			}
			pElement->Release();
		}
		else
		{
			pElement->Release();
			gl_iCompeleteFlag = 0;
			strJs.Format("document.getElementById(\"purchase-shipping-address\").getElementsByTagName(\"A\")[0].click();");
			RunJavascript(strJs);
			WaitForComplete();
		}		
	}
	int iCount = 0;
	while (true)
	{
		iCount++;
		GetElement("shipToThisAddress",&pElement);
		if (!pElement)
		{
			if (iCount > 5)
			{
				gl_iExitCode = SETADDR_ERROR;
				return false;
			}	
			Sleep(1000);
			continue;
		}
		break;
	}	
	//填写地址，发送	
	strJs.Format("document.getElementById(\"enterAddressStateOrRegion\").value = \"%s\";resetCity();resetDistrict();getCities();",objStr.strProvince);
	RunJavascript(strJs);
	Sleep(20);
	strJs.Format("document.getElementById(\"enterAddressCity\").value = \"%s\";resetDistrict();getDistricts();",objStr.strCity);
	RunJavascript(strJs);
	Sleep(20);
	strJs.Format("document.getElementById(\"enterAddressDistrictOrCounty\").value = \"%s\";",objStr.strTown);
	RunJavascript(strJs);
	SetItemValue("enterAddressFullName",objStr.strRecver);
	SetItemValue("enterAddressAddressLine1",objStr.strAddr);
	SetItemValue("enterAddressPostalCode",objStr.strPostCode);
	SetItemValue("enterAddressPhoneNumber",objStr.strPhone);	
	gl_iCompeleteFlag = 0;
	ClickElementEx(pElement);
	pElement->Release();
	WaitForComplete();

	GetElement("needChinaInvoice",&pElement);
	if (!pElement)
	{
		gl_iExitCode = ORDER_ERROR;
		return false;
	}
	pElement->Release();
	
	if (gl_AmazonStr.bCanNormalSend == FALSE)
	{
		//判断是否有快递方式,如果没有,则退出
		bool bHasSpeed = false;
		//
		bHasSpeed = CheckHasSpeedSend();
		if (!bHasSpeed)
		{
			gl_iExitCode = NOSPEEDSEND;
			return false;
		}		
	}
	//填写发送方式，发票等信息，提交
	SetSendTypeAndBillInfo(objStr);	
	strJs.Format("document.getElementsByName(\"placeYourOrder\")[0].click();");	
	gl_iCompeleteFlag = 0;
	RunJavascript(strJs);
	WaitForComplete();

	//检测是否短时间内重复提交
	GetContent();
	if (gl_strContent.Find("这是重复订单") >= 0)
	{
		gl_iExitCode = ORDER_DUPLICATE;
		return false;
	}

	//已经提交订单操作，判断是否下订单成功
	GetElement("thank-you-header",&pElement);
	if (!pElement)
	{
		gl_iExitCode = ORDER_ERROR;
		return false;
	}
	GetContentEx(pElement);
	pElement->Release();
	if (gl_strContent.Find("感谢您提交订单") < 0)
	{
		gl_iExitCode = ORDER_ERROR;
		return false;
	}
	return true;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CAmazonRobotDlg::ExecJavascript(CString strJs)
{
	IHTMLDocument2*   pDocument;
	HRESULT   hr   =   GetDHtmlDocument(&pDocument);   
	IHTMLWindow2*   pWindow;
	hr   =   pDocument->get_parentWindow(&pWindow);
	VARIANT  ret;
	//BSTR bstrStr = strJs.AllocSysString();
	BSTR bstrStr=_com_util::ConvertStringToBSTR((LPCTSTR)strJs);
	pWindow->execScript(bstrStr,L"javascript",&ret);
	::SysFreeString(bstrStr);
	pWindow->Release();
	pDocument->Release();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CAmazonRobotDlg::NavigateEx(CString strUrl)
{	
	gl_iCompeleteFlag = 0;
	Navigate(strUrl);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CAmazonRobotDlg::WaitForComplete()
{
	int iCount = 0;
	//20秒超时
	while( iCount++ < 100 )
	{
		if (gl_iCompeleteFlag == 1)
		{
			Sleep(500);
			break;
		}
		Sleep(300);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CAmazonRobotDlg::OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl)
{
     LPDISPATCH lpWBDisp = NULL;
     HRESULT    hr = NULL;
     hr = m_pBrowserApp->QueryInterface(IID_IDispatch, (void**)&lpWBDisp);
     ASSERT(SUCCEEDED(hr));
     if (pDisp == lpWBDisp)
     {
		gl_iCompeleteFlag = 1;
     }
     lpWBDisp->Release();
	 CDHtmlDialog::OnDocumentComplete(pDisp,szUrl);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CAmazonRobotDlg::ThreadFunc(LPVOID lParam)
{
	CoInitialize(NULL);
	CAmazonRobotDlg *pParam = ((CAmazonRobotDlg *)lParam);
	if ( pParam )
	{
		pParam->RunFunc();
	}
	::CoUninitialize();
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CAmazonRobotDlg::PreTranslateMessage( MSG* pMsg )
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


/************************************************************************/
/*                                                                      */
/************************************************************************/
void CAmazonRobotDlg::RunFunc()
{
	if (gl_AmazonStr.iOpFlag == 2)	//下单
	{
		Order();
	}
	else if (gl_AmazonStr.iOpFlag == 3) // 修改支付方式
	{
		//https://www.amazon.cn/gp/css/history/orders/view.html/ref=ohsearch1_C?opt=ab&groupID=0&__mk_zh_CN=%E4%BA%9A%E9%A9%AC%E9%80%8A%E7%BD%91%E7%AB%99&search=C02-1648374-3804857&x=18&y=14
		ChangePayType();
	}
	else
	{
		Query();
	}
}

void CAmazonRobotDlg::ChangePayType()
{
	bool bRet = false;	
	bRet = DoChange(gl_AmazonStr);
	if (!bRet)
	{
		if (gl_AmazonStr.iDebug == 1)
		{
			CString strCode = "";
			strCode.Format("%d",gl_iExitCode);
			AfxMessageBox(strCode);
			return;
		}
		PostMessage(WM_EXITCODEINFO,0,gl_iExitCode);
		return;
	}
	gl_iExitCode = 0;
	if (gl_AmazonStr.iDebug == 1)
	{
		return;
	}	
	PostMessage(WM_EXITCODEINFO,0,0);
}

void CAmazonRobotDlg::Query()
{
	bool bRet = false;	
	bRet = DoQuery(gl_AmazonStr);
	if (!bRet)
	{
		if (gl_AmazonStr.iDebug == 1)
		{
			CString strCode = "";
			strCode.Format("%d",gl_iExitCode);
			AfxMessageBox(strCode);
			return;
		}
		PostMessage(WM_EXITCODEINFO,0,gl_iExitCode);
		return;
	}
	gl_iExitCode = 0;
	if (gl_AmazonStr.iDebug == 1)
	{
		return;
	}	
	PostMessage(WM_EXITCODEINFO,0,0);
}

void CAmazonRobotDlg::Order()
{
	bool bRet = false;
	CString strArray[100] = {""};
	//得到ID数组
	int iCount = StringSplit(gl_AmazonStr.strGoodNos+",",strArray,',');
	if (iCount < 1)
	{
		if (gl_AmazonStr.iDebug == 1)
		{
			return;
		}
		gl_iExitCode = PARAM_ERROR;
		PostMessage(WM_EXITCODEINFO,0,PARAM_ERROR);
		return;
	}
	for (int i=0;i<iCount;i++)
	{
		if (strArray[i] == "")
		{
			continue;
		}
		bRet = AddGoodsTocart(strArray[i]);
		if (!bRet)
		{
			if (gl_AmazonStr.iDebug == 1)
			{
				return;
			}
			gl_iExitCode = ADD_TOCARTERROR;
			PostMessage(WM_EXITCODEINFO,0,ADD_TOCARTERROR);
			return;
		}	
	}
	bRet = DoOrder(gl_AmazonStr);
	if (!bRet)
	{
		//AfxMessageBox("2");
		//exit(gl_iExitCode);	//返回错误码
		if (gl_AmazonStr.iDebug == 1)
		{
			CString strCode = "";
			strCode.Format("%d",gl_iExitCode);
			AfxMessageBox(strCode);
			return;
		}
		PostMessage(WM_EXITCODEINFO,0,gl_iExitCode);
		return;
	}
	gl_iExitCode = 0;
	if (gl_AmazonStr.iDebug == 1)
	{
		return;
	}	
	PostMessage(WM_EXITCODEINFO,0,0);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CAmazonRobotDlg::StringSplit(CString srcStr,CString strArray[],char splitChar)
{
	try
	{
		int idx1 = 0;
		int idx2 = 0;
		int iCount = 0;
		idx2 = srcStr.Find(splitChar, idx1);  //找第一个记录 
		while(idx2 != -1)						//如果找到
		{ 
			strArray[iCount] = srcStr.Mid(idx1,idx2 - idx1); //取到一个记录
			idx1 = idx2 + 1; 
			idx2 = srcStr.Find(splitChar, idx1);  //找下一个记录
			iCount++;
		} 
		return iCount;
	}
	catch (...)
	{
		WriteLog("StringSplit异常",3);
	}
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
LRESULT CAmazonRobotDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	try
	{
		if (message == WM_NCPAINT && gl_AmazonStr.iShowFlag == 0)
		{
			ShowWindow(SW_HIDE);
		}
		switch (message)
		{
		case WM_CLICKMSG:
			OnClickMessage(wParam,lParam);
			break;
		case WM_CLICKMSGEX:
			OnClickMessageEx(wParam,lParam);
			break;
		case WM_RUNJAVASCRIPT:
			OnRunJavascript(wParam,lParam);
			break;
		case WM_GETHTMLCONTENT:
			OnGetHtmlContent(wParam,lParam);
			break;
		case WM_GETHTMLCONTENTEX:
			OnGetHtmlContentEx(wParam,lParam);
			break;
		case WM_SETELEMENTVALUE:
			OnSetValue(wParam,lParam);
			break;
		case WM_SETSENDTYPEANDBILL:
			OnSetSendTypeAndBill(wParam,lParam);
			break;
		case WM_CHECKHASSPEEDSEND:
			OnCheckSpeedSend(wParam,lParam);
			break;
		case WM_EXITCODEINFO:
			Sleep(200);
			exit(lParam);
			break;
			
		}
	}
	catch(...)
	{

	}
	return CDHtmlDialog::DefWindowProc(message, wParam, lParam);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CAmazonRobotDlg::ClickElement(CString strID)
{
	SendMessage(WM_CLICKMSG,0,(LPARAM)&strID);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CAmazonRobotDlg::SetSendTypeAndBillInfo(AmazonOrderStru &objInfo)
{
	SendMessage(WM_SETSENDTYPEANDBILL,0,(LPARAM)&objInfo);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CAmazonRobotDlg::SetItemValue(CString strID,CString strValue)
{
	SendMessage(WM_SETELEMENTVALUE,(WPARAM)&strID,(LPARAM)&strValue);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CAmazonRobotDlg::ClickElementEx(IHTMLElement* pElement)
{
	SendMessage(WM_CLICKMSGEX,0,(LPARAM)pElement);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CAmazonRobotDlg::RunJavascript(CString strJS)
{
	SendMessage(WM_RUNJAVASCRIPT,0,(LPARAM)&strJS);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CAmazonRobotDlg::GetContent()
{
	SendMessage(WM_GETHTMLCONTENT,0,0);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CAmazonRobotDlg::GetElementValue(CString strId)
{
	SendMessage(WM_GETELEMENTVALUE,0,0);
}


void CAmazonRobotDlg::GetContentEx(IHTMLElement* pElement)
{
	SendMessage(WM_GETHTMLCONTENTEX,0,(LPARAM)pElement);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CAmazonRobotDlg::OnSetValue(WPARAM wParam, LPARAM lParam)
{
	CString strId = (*(CString*)wParam);
	CString strValue = (*(CString*)lParam);
	//BSTR bstrStr = strValue.AllocSysString();
	BSTR bstrStr=_com_util::ConvertStringToBSTR((LPCTSTR)strValue);
	SetElementText(strId,bstrStr);
	::SysFreeString(bstrStr);
}

void CAmazonRobotDlg::OnCheckSpeedSend(WPARAM wParam, LPARAM lParam)
{
	bool* pbRet = (bool*)lParam;
	*pbRet = false;
	//进行检查
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CAmazonRobotDlg::OnSetSendTypeAndBill(WPARAM wParam, LPARAM lParam)
{
	AmazonOrderStru* pStr = (AmazonOrderStru*)lParam;	
	try
	{		
		CString strJsEx = "";
		strJsEx.Format("document.getElementById(\"needChinaInvoice\").checked = %s;",pStr->bBill==true?"true":"false");
		ExecJavascript(strJsEx);
		if (pStr->bBill==true)
		{
			//填写发票信息
			//needChinaInvoice
			//chinaInvoiceTitle
			//chinaInvoiceCategoryID   //需要计算序号
			SetItemValue("chinaInvoiceTitle",pStr->strBillHeader);
			map<CString,CString>::iterator it = m_mapBillType.find(pStr->strBillContent);
			if (it != m_mapBillType.end())
			{
				strJsEx.Format("document.getElementById(\"chinaInvoiceCategoryID\").value = \"%s\";",it->second);
				ExecJavascript(strJsEx);
			}
			//采集下来保存
		}
		SetSendType(pStr->eSendType);
		
	}
	catch (...)
	{
	}
	//_SendType type = (_SendType)lParam;
	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CAmazonRobotDlg::OnRunJavascript(WPARAM wParam, LPARAM lParam)
{
	CString* pStrJs = (CString*)lParam;
	ExecJavascript((*pStrJs));
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CAmazonRobotDlg::OnGetHtmlContent(WPARAM wParam, LPARAM lParam)
{
	gl_strContent = GetHtmlContent();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CAmazonRobotDlg::OnGetHtmlContentEx(WPARAM wParam, LPARAM lParam)
{
	try
	{
		IHTMLElement* pElement = (IHTMLElement*)lParam;
		BSTR bstrStr;
		pElement->get_innerHTML(&bstrStr);
		gl_strContent = CString((char*)(_bstr_t)bstrStr);
		::SysFreeString(bstrStr);
	}
	catch (...)
	{
	}	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CAmazonRobotDlg::OnClickMessage(WPARAM wParam, LPARAM lParam)
{	
	CString* pStrId = (CString*)lParam;
	IHTMLElement* pElement = NULL;
	GetElement((*pStrId),&pElement);
	if (!pElement)
	{
		return;
	}
	pElement->click();
	pElement->Release();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CAmazonRobotDlg::OnClickMessageEx(WPARAM wParam, LPARAM lParam)
{	
	IHTMLElement* pElement = (IHTMLElement*)lParam;	
	pElement->click();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CAmazonRobotDlg::GetHtmlContent()
{
	IHTMLDocument2* pDoc = NULL;
	IHTMLElement* pelmBody = NULL;
	GetDHtmlDocument(&pDoc);
	if (!pDoc)
	{
		return "";
	}
	pDoc->get_body(&pelmBody);
	if (!pelmBody)
	{
		pDoc->Release();
		return "";
	}
	BSTR bstrStr;
	pelmBody->get_innerHTML(&bstrStr);
	CString str((char*)(_bstr_t)bstrStr);
	::SysFreeString(bstrStr);
	pDoc->Release();
	pelmBody->Release();
	return str;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CAmazonRobotDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 3)
	{
		HWND hwnd = ::FindWindow("#32770","Microsoft Internet Explorer");
		if (hwnd)
		{
			::PostMessage(hwnd,WM_CLOSE,0,0);
		}
		hwnd = ::FindWindow("Internet Explorer_TridentDlgFrame","Internet Explorer 脚本错误");
		if (hwnd)
		{
			::PostMessage(hwnd,WM_CLOSE,0,0);
		}
		hwnd = ::FindWindow("Internet Explorer_TridentDlgFrame","系统错误 -- 网页对话框");
		if (hwnd)
		{
			::PostMessage(hwnd,WM_CLOSE,0,0);
		}
		hwnd = ::FindWindow("#32770","错误");
		if (hwnd)
		{
			::PostMessage(hwnd,WM_CLOSE,0,0);
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CAmazonRobotDlg::SetSendType(_SendTime type)
{
	IDispatch* pElemDisp = NULL;
	IHTMLElement* pElem=NULL;
	IHTMLElementCollection* pElemColl = NULL;
	IHTMLDocument2* pDoc = NULL;
	GetDHtmlDocument(&pDoc);
	GetElementCollection("input",&pElemColl,pDoc);	//获取指定TAG集合
	if(pElemColl!=NULL)
	{
		long length;
		pElemColl->get_length(&length);	//指定TAG集合长度
		int iCount = 0;
		for (long i=0;i<length;i++)
		{
			_variant_t varBase(0);
			_variant_t varIndex(i);
			HRESULT hr = pElemColl->item(varIndex, varBase, &pElemDisp);
			if (SUCCEEDED(hr) && pElemDisp!=NULL)
			{
				hr = pElemDisp->QueryInterface(IID_IHTMLElement,(void**)&pElem);
				if(SUCCEEDED(hr) && pElem!=NULL)
				{
					BSTR bstrType;
					//order.477:93160512976.deliveryTimePreference.weekdays_cn_71874
					pElem->get_id(&bstrType);
					CString strType(bstrType);		
					::SysFreeString(bstrType);
					strType.MakeLower();
					if(strType.Find("deliverytimepreference")>0)
					{
						iCount++;
						if ((int)type == iCount)
						{
							pElem->click();
						}																		
					}
					pElem->Release();
				}
				pElemDisp->Release();
			}
		}
		pElemColl->Release();
	}
	pDoc->Release();
}

void CAmazonRobotDlg::GetElementCollection(CString tagName,IHTMLElementCollection** pSpecialColl,IHTMLDocument2* pDoc)
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
// 
// void CAmazonRobotDlg::GetElementCollectionEx(CString tagName,IHTMLElementCollection** pSpecialColl,IHTMLElement* pElement)
// {
// 	IHTMLElementCollection* pElemColl = NULL;
// 	IDispatch* pElemDisp = NULL;
// 	IHTMLElement* pElem=NULL;
// 	if (pElement != NULL)
// 	{
// 		pElement->get_all(&pElemColl);
// 		if (pElemColl != NULL)
// 		{
// 			_variant_t var(tagName); 
// 			pElemColl->tags(var,&pElemDisp);	//指定TAG集合
// 			if (pElemDisp != NULL)
// 			{
// 				*pSpecialColl = (IHTMLElementCollection*)pElemDisp;
// 				pElemColl->Release();
// 				return;
// 			}
// 			pElemColl->Release();
// 		}
// 
// 	}
// }

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CAmazonRobotDlg::GetPower()
{
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
				if (strTemp.Find("myownpoweamazon") !=-1)
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

void CAmazonRobotDlg::OnClose()
{
// 	CString strCode = "";
// 	strCode.Format("%d",gl_iExitCode);
// 	AfxMessageBox(strCode);
	exit(gl_iExitCode);
	CDialog::OnClose();
}
void CAmazonRobotDlg::OnBnClickedRadio1()
{
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);
}

void CAmazonRobotDlg::OnBnClickedRadio2()
{
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(TRUE);
}

void CAmazonRobotDlg::SetValue(LPCTSTR strValue)
{
	AfxMessageBox(strValue);
	gl_strSetValue = strValue;
}

bool CAmazonRobotDlg::CheckHasSpeedSend()
{
	bool bRet = false;
	GetContent();
	if (gl_strContent.Find("快递送货上门-免配送费") > 0)
	{
		return true;
	}
	return false;
	//SendMessage(WM_CHECKHASSPEEDSEND,0,&bRet);
}