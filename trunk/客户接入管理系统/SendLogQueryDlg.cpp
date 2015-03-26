#include "stdafx.h"
#include "SendLogQuery.h"
#include "SendLogQueryDlg.h"
#include ".\sendlogquerydlg.h"
#include <io.h>
#include <odbcinst.h>
#include <afxdb.h>
#include "InfoShow.h"
#include "ServiceDetail.h"
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


// CSendLogQueryDlg 对话框
CSendLogQueryDlg::CSendLogQueryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSendLogQueryDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSendLogQueryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESSBAR, m_ProgressCtrl);
	DDX_Control(pDX, IDC_LIST_SHOW, m_resultList);
	DDX_Control(pDX, IDC_LIST_SHOW3, m_listtype);
	DDX_Control(pDX, IDC_COMBO_ACCTYPE, m_comboType);
	DDX_Control(pDX, IDC_COMBO_LANGUAGE, m_combo_language);
}

BEGIN_MESSAGE_MAP(CSendLogQueryDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_QUERY, OnBnClickedBtnQuery)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBnClickedBtnStop)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_OUTPORT, OnBnClickedBtnOutport)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBnClickedBtnAdd)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SHOW3, OnNMDblclkListShow3)
	ON_BN_CLICKED(IDC_BTN_FRESH, &CSendLogQueryDlg::OnBnClickedBtnFresh)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SHOW, &CSendLogQueryDlg::OnNMRclickListShow)
	ON_COMMAND(IDR_MENU_DELETE, &CSendLogQueryDlg::OnMenuDelete)
	ON_COMMAND(IDR_MENU_EDIT, &CSendLogQueryDlg::OnMenuEdit)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SHOW, &CSendLogQueryDlg::OnNMDblclkListShow)
	ON_CBN_SELCHANGE(IDC_COMBO_ACCTYPE, &CSendLogQueryDlg::OnCbnSelchangeComboAcctype)
END_MESSAGE_MAP()


// CSendLogQueryDlg 消息处理程序

BOOL CSendLogQueryDlg::OnInitDialog()
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

	m_Counter = 50;
	m_ProgressCtrl.SetRange(0,50);
	m_ProgressCtrl.SetPos(0);
	SetTimer(1,50,NULL);
	MoveWindow(0,0,1000,700);
	InitCtrList();
	GetDlgItem(IDC_PROGRESSBAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_OUTPORT)->EnableWindow(FALSE);
	//读取数据库配置
	CString strAppPath;
	GetAppPath(strAppPath);
	CString strFilePath = strAppPath + "set.ini";
	char szTemp[1000] = {0};
	::GetPrivateProfileString("database", "dbserver", "",(LPSTR)szTemp,1000, strFilePath);
	m_dbServer = szTemp;

	memset(szTemp,0,1000);
	::GetPrivateProfileString("database", "dbname", "",(LPSTR)szTemp,1000, strFilePath);
	m_dbName = szTemp;

	memset(szTemp,0,1000);
	::GetPrivateProfileString("database", "dbuser", "",(LPSTR)szTemp,1000, strFilePath);
	m_dbUser = szTemp;

	memset(szTemp,0,1000);
	::GetPrivateProfileString("database", "dbpwd", "",(LPSTR)szTemp,1000, strFilePath);
	m_dbPwd = szTemp;
	memset(szTemp,0,1000);
	//::GetPrivateProfileString("database", "dbshortpath", "",(LPSTR)szTemp,1000, strFilePath);
	//m_dbPath = szTemp;
	//m_dbPath = "customer.mdb";
	//m_dbPath = strAppPath + m_dbPath;
	OnBnClickedBtnFresh();
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CSendLogQueryDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSendLogQueryDlg::OnPaint() 
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
HCURSOR CSendLogQueryDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::OnTimer(UINT nIDEvent)
{
	m_Counter++;
	if ( m_Counter <= 50 )
	{
		m_ProgressCtrl.SetPos(m_Counter);	
	}
	else
	{
		m_Counter = 0;
	}	
	CDialog::OnTimer(nIDEvent);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::ThreadProc(LPVOID lpParam)
{
	CSendLogQueryDlg *pWnd = (CSendLogQueryDlg *)((ThreadParams *)lpParam)->pWnd;
	int iFlag = ((ThreadParams *)lpParam)->m_iNo;
	::CoInitialize(NULL);
	pWnd->OperateProc(iFlag);
	::CoUninitialize();	
	delete lpParam;
	lpParam = NULL;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::OperateProc(int iFlag)
{
	if (iFlag != 10)
	{
		SetDlgItemText(IDC_INFO_SHOW,"正在查询,请稍后...");
		GetDlgItem(IDC_PROGRESSBAR)->ShowWindow(SW_SHOW);
		QuerySendLog(m_strWhere,m_iSendFlag,&(m_ThreadContrl.ThreadExit),m_iCurrPage,m_IsShowDetail);
	}
	else		//导出日志
	{
		GetDlgItem(IDC_PROGRESSBAR)->ShowWindow(SW_HIDE);
		SetDlgItemText(IDC_INFO_SHOW,"");
		if (m_IsShowDetail)
		{
			ExportToExcel("发送日志记录",m_strWhere,m_iSendFlag,&(m_ThreadContrl.ThreadExit),m_iCurrPage,m_IsShowDetail);
		}
		else		//导出每日日志
		{
//			ExportToExcelEx("每日发送成功数统计");
		}
	}

	GetDlgItem(IDC_PROGRESSBAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_QUERY)->EnableWindow(TRUE);
	if ( !m_IsShowDetail )
	{
		GetDlgItem(IDC_BTN_OUTPORT)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_OUTPORT)->EnableWindow(TRUE);
	}	
	GetDlgItem(IDC_PROGRESSBAR)->ShowWindow(SW_HIDE);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::OnBnClickedBtnQuery()
{
	//验证查询条件
	
// 	GetDlgItemText(IDC_DATE_DATESTART,m_strStartDate);
// 	GetDlgItemText(IDC_DATE_DATEEND,m_strEndDate);
// 	CTime startTime = GetCTimeFromString(m_strStartDate + " 00:00:00");
// 	CTime endTime = GetCTimeFromString(m_strEndDate + "  23:59:59");
// 	CTimeSpan timeSpan;
// 	timeSpan = endTime - startTime;
// 	if (timeSpan < 0 )
// 	{
// 		SetDlgItemText(IDC_INFO_SHOW,"开始时间必需小于结束时间");
// 		return;
// 	}
	m_strWhere = " where 1=1";
	CString strTemp = "";
	CString strFormat = "";
	int iTemp = 0;
	GetDlgItemText(IDC_COMBO_ACCTYPE,strTemp);
	strTemp.Trim();
	iTemp = m_typeMap[strTemp];
	//strTemp.Format("%d",iTemp);
	//MessageBox(strTemp);
	if (iTemp != -1 && iTemp != 0)
	{
		strFormat.Format(" and [type] = %d",iTemp);
		m_strWhere += strFormat;
	}

	GetDlgItemText(IDC_COMBO_LANGUAGE,strTemp);
	strTemp.Trim();
	iTemp = m_languageMap[strTemp];

	if (iTemp != -1 && iTemp != 0)
	{
		strFormat.Format(" and [language] = %d",iTemp);
		m_strWhere += strFormat;
	}

	GetDlgItemText(IDC_EDIT_COMNAME,strTemp);
	if (strTemp != "")
	{
		strFormat.Format(" and comname like '%%%s%%'",strTemp);
		m_strWhere += strFormat;
	}	
	strTemp = "";
	GetDlgItemText(IDC_EDIT_LINKMAN,strTemp);
	if (strTemp != "")
	{
		strFormat.Format(" and linkman like '%%%s%%'",strTemp);
		m_strWhere += strFormat;
	}
	strTemp = "";
	GetDlgItemText(IDC_EDIT_LINKPHONE,strTemp);
	if (strTemp != "")
	{
		strFormat.Format(" and linkphone like '%%%s%%'",strTemp);
		m_strWhere += strFormat;
	}
	strTemp = "";
	GetDlgItemText(IDC_EDIT_QQ,strTemp);
	if (strTemp != "")
	{
		strFormat.Format(" and linkqq like '%%%s%%'",strTemp);
		m_strWhere += strFormat;
	}
	strTemp = "";
	GetDlgItemText(IDC_EDIT_ACCOUNT,strTemp);
	if (strTemp != "")
	{
		strFormat.Format(" and account like '%%%s%%'",strTemp);
		m_strWhere += strFormat;
	}

	strTemp = "";
	GetDlgItemText(IDC_EDIT_SALEMAN,strTemp);
	if (strTemp != "")
	{
		strFormat.Format(" and saleman like '%%%s%%'",strTemp);
		m_strWhere += strFormat;
	}
	strTemp = "";
	GetDlgItemText(IDC_EDIT_SENDNUM,strTemp);
	if (strTemp != "")
	{
		strFormat.Format(" and (num > %s or num =%s)",strTemp,strTemp);
		m_strWhere += strFormat;
	}

	/************************************************************************/
	//开始查询
	OperateProc(1);
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::InitCtrList()
{
	DWORD dwSytle=::GetWindowLong(m_resultList.m_hWnd,GWL_STYLE);
	//设置为报表形式
	SetWindowLong(m_resultList.m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=m_resultList.GetExtendedStyle();
	m_resultList.SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	::SendMessage(m_resultList.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	m_resultList.InsertColumn(0,_T("编号"),LVCFMT_LEFT,50);
	m_resultList.InsertColumn(1,_T("公司名"),LVCFMT_LEFT,110);
	m_resultList.InsertColumn(2,_T("接入类型"),LVCFMT_LEFT,110);
	m_resultList.InsertColumn(3,_T("使用语言"),LVCFMT_LEFT,80);
	m_resultList.InsertColumn(4,_T("联系人"),LVCFMT_LEFT,80);
	m_resultList.InsertColumn(5,_T("电话"),LVCFMT_LEFT,100);
	m_resultList.InsertColumn(6,_T("QQ[Email]"),LVCFMT_LEFT,120);
	m_resultList.InsertColumn(7,_T("账号"),LVCFMT_LEFT,60);
	m_resultList.InsertColumn(8,_T("密码"),LVCFMT_LEFT,60);
	m_resultList.InsertColumn(9,_T("业务员"),LVCFMT_LEFT,80);
	m_resultList.InsertColumn(10,_T("发送量"),LVCFMT_LEFT,90);
	m_resultList.InsertColumn(11,_T("接入日期"),LVCFMT_LEFT,100);
	m_resultList.InsertColumn(12,_T("备注"),LVCFMT_LEFT,200);
	m_resultList.MoveWindow(1,115,991,545);
	//设置为报表形式
	 dwSytle=::GetWindowLong(m_resultList.m_hWnd,GWL_STYLE);

	//设置为报表形式
	SetWindowLong(m_listtype.m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	ExStyle=m_listtype.GetExtendedStyle();
	m_listtype.SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	::SendMessage(m_listtype.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	m_listtype.InsertColumn(0,_T("接入类型"),LVCFMT_LEFT,150);
	m_listtype.InsertColumn(1,_T("客户数量"),LVCFMT_LEFT,100);
	//m_listtype.InsertColumn(2,_T("编号"),LVCFMT_LEFT,60);
	m_listtype.MoveWindow(655,10,336,98);
	GetDlgItem(IDC_PROGRESSBAR)->MoveWindow(1,110,991,3);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::QuerySendLog(CString strWhere,int iFlag,BOOL *bExit,int iCurrPage,BOOL bDetail)
{	
	iCurrPage = 1;
	m_resultList.DeleteAllItems();
	bDetail = TRUE;
	//m_showDay.DeleteAllItems();	
	if (bDetail==TRUE)		//如果要查询详细信息
	{
		//m_showDay.ShowWindow(SW_HIDE);
		m_resultList.ShowWindow(SW_SHOW);
	}
	else		//不查询详细信息
	{	
		//m_showDay.ShowWindow(SW_SHOW);
		m_resultList.ShowWindow(SW_HIDE);
	}
	try
	{
		if ( !m_adoConn.IsOpen() )		//如果未连接上服务器
		{
			if ( !m_adoConn.ConnectSQLServer(m_dbServer,m_dbName,m_dbUser,m_dbPwd))//.ConnectSQLServer( m_dbServer , m_dbName, m_dbUser,m_dbPwd) )
			{
				SetDlgItemText(IDC_INFO_SHOW,"连接数据库失败");
				return ; 
			}
			m_adoRecrd.SetAdoConnection(&m_adoConn);
			m_adoRecrd.SetCursorLocation(adUseClient);
		}
		CString strSQL;
		CString strTempWhere;
		CString strTemp;
		int iTemp = 0;
		bool bQuit = false;		//手动退出循环
		if ( bDetail != TRUE)		//不显示详细信息
		{			
//			GetDaySucCount(m_strStartDate,m_strEndDate,m_adoConn,m_adoRecrd);
		}
		else	//显示详细信息
		{
			//strSQL.Format("select * from Acctype left join (Customer left join langType on Customer.language = langType.id) on Customer.type = Acctype.id");
			strSQL.Format("select * from (select * from Customer left join Acctype on Customer.type = Acctype.typeid) as table1 left join langType on table1.[language] = langType.langid %s order by firsttime asc,customerid asc",m_strWhere);

			//strSQL.Format("select * from Customer left join Acctype on Customer.[language] = Acctype.id");

			if( !m_adoRecrd.Open(strSQL,adCmdText))
			{
				m_adoRecrd.Close();
				SetDlgItemText(IDC_INFO_SHOW,"执行出错,"+m_adoConn.GetLastErrorText());

				return;
			}
			
			COleDateTime tempTime;
			while( !m_adoRecrd.IsEOF())		//如果记录不为空
			{
				if ( *bExit == TRUE )
				{
					bQuit = true;
					break;
				}
				int iCount = m_resultList.GetItemCount();
				m_resultList.InsertItem( iCount, "");			
				m_adoRecrd.GetCollect("customerid",iTemp);
				strTemp.Format("%d",iTemp);
				m_resultList.SetItemText( iCount , 0,strTemp);
	
				m_adoRecrd.GetCollect("comname",strTemp);
				m_resultList.SetItemText( iCount , 1,strTemp);

				m_adoRecrd.GetCollect("typename",strTemp);
				m_resultList.SetItemText( iCount , 2,strTemp);

				m_adoRecrd.GetCollect("langName",strTemp);
				m_resultList.SetItemText( iCount , 3,strTemp);

				m_adoRecrd.GetCollect("linkman",strTemp);
				m_resultList.SetItemText( iCount , 4,strTemp);

				m_adoRecrd.GetCollect("linkphone",strTemp);
				m_resultList.SetItemText( iCount , 5,strTemp);

				CString strQQEmail = "";
				m_adoRecrd.GetCollect("linkqq",strTemp);
				strQQEmail = strTemp;
				m_adoRecrd.GetCollect("linkemail",strTemp);
				strTemp.Trim();
				if (strTemp != "")
				{
					strQQEmail += "["+strTemp+"]";
				}				
				m_resultList.SetItemText( iCount ,6,strQQEmail);

				m_adoRecrd.GetCollect("account",strTemp);
				m_resultList.SetItemText( iCount ,7,strTemp);

				m_adoRecrd.GetCollect("password",strTemp);
				m_resultList.SetItemText( iCount ,8,strTemp);

				m_adoRecrd.GetCollect("saleman",strTemp);
				m_resultList.SetItemText( iCount ,9,strTemp);

				m_adoRecrd.GetCollect("num",iTemp);	
				strTemp.Format("%d",iTemp);
				m_resultList.SetItemText( iCount ,10,strTemp);
				
				m_adoRecrd.GetCollect("firsttime",tempTime);			
				m_resultList.SetItemText( iCount ,11,tempTime.Format("%Y-%m-%d"));
				m_adoRecrd.GetCollect("reverse1",strTemp);	
				m_resultList.SetItemText( iCount ,12,strTemp);
				m_adoRecrd.MoveNext();
			}
		}
		if ( bQuit )
		{
			SetDlgItemText(IDC_INFO_SHOW,"手动停止,查询未完成");
		}
		else
		{
			SetDlgItemText(IDC_INFO_SHOW,"查询完成");
		}
		
	}
	catch(...)
	{
		SetDlgItemText(IDC_INFO_SHOW,"执行异常,查询未完成");
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::OnBnClickedBtnStop()
{
	m_ThreadContrl.ThreadExit = TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CTime CSendLogQueryDlg::GetCTimeFromString(CString strTime)
{
	int   nYear,   nMonth,   nDate,   nHour,   nMin,   nSec;   
	sscanf(strTime,"%d-%d-%d %d:%d:%d",&nYear,&nMonth,&nDate,&nHour,&nMin,&nSec);   
	return CTime(nYear,nMonth,nDate,nHour,nMin,nSec);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::OnClose()
{
	OnBnClickedBtnStop();
	Sleep(500);
	CDialog::OnClose();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//返回应用程序所在目录，包含"\"
int CSendLogQueryDlg::GetAppPath(CString &strAppPath)
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
void CSendLogQueryDlg::OnBnClickedBtnOutport()
{
	/************************************************************************/
	//开始查询
	ThreadParams *lpAddBufParam = new ThreadParams;
	lpAddBufParam->pWnd = this;
	lpAddBufParam->m_iNo = 10;		//1是查询 
	m_ThreadContrl.pThread = AfxBeginThread((AFX_THREADPROC)ThreadProc, (LPVOID)lpAddBufParam);
	m_ThreadContrl.ThreadExit = FALSE;
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::ExportToExcel(CString strTitle,CString strWhere,int iFlag,BOOL *bExit,int iCurrPage,BOOL bDetail)
{
	try
	{
		CDatabase database;
		CString sDriver;
		CString sExcelFile; 
		CString sSql;
		CString tableName = strTitle;

		// 检索是否安装有Excel驱动 "Microsoft Excel Driver (*.xls)" 
		sDriver = GetExcelDriver();
		if (sDriver.IsEmpty())
		{
			// 没有发现Excel驱动
			SetDlgItemText(IDC_INFO_SHOW,"请先安装Excel软件才能使用导出功能");
			return;
		}
		if ( !m_adoConn.IsOpen() )		//如果未连接上服务器
		{
			if ( !m_adoConn.ConnectSQLServer(m_dbServer,m_dbName,m_dbUser,m_dbPwd) )
			{
				SetDlgItemText(IDC_INFO_SHOW,"连接数据库失败");
				return ; 
			}
			m_adoRecrd.SetAdoConnection(&m_adoConn);
			m_adoRecrd.SetCursorLocation(adUseClient);
		}
		CString strSQL;		
		//strSQL.Format("select localID,PhoneNo,content,SendTime,FinishTime,ReadStatus,SendStatus,msglen,PhoneCount,FailReason,smstype from win22_SMSendLog_MW %s",strTempWhere);
		strSQL.Format("select * from (select * from Customer left join Acctype on Customer.type = Acctype.typeid) as table1 left join langType on table1.[language] = langType.langid %s order by firsttime asc,customerid asc",m_strWhere);
		if( !m_adoRecrd.Open(strSQL,adCmdText))
		{
			m_adoRecrd.Close();
			SetDlgItemText(IDC_INFO_SHOW,"执行出错,导出未完成");
			return;
		}

		if ( m_adoRecrd.IsEOF() )
		{
			SetDlgItemText(IDC_INFO_SHOW,"无信息可导出");
			m_adoRecrd.Close();
			return;
		}
		///默认文件名
		if (!GetDefaultXlsFileName(sExcelFile))
		{
			return;
		}
		SetDlgItemText(IDC_INFO_SHOW,"正在进行导出,请稍后...");
		GetDlgItem(IDC_PROGRESSBAR)->ShowWindow(SW_SHOW);
		// 创建进行存取的字符串
		sSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, sExcelFile);
		COleDateTime tempTime;
		bool bQuit = false;
		CString localID;
		CString comName;
		CString accType;
		CString language;
		CString linkman;
		CString linkphone;
		CString linkqq;
		CString linkemail;
		CString account;
		CString pwd;
		CString sendnum;
		CString accdate;
		CString serverip;
		CString serveraccount;
		CString serverpwd;
		CString strreverse;
		CString saleman;
		CString salemanphone;
		CString salemanqq;
		CString salemanEmail;
		CString strPort;
		int iTemp;
		// 创建数据库 (既Excel表格文件)
		if( database.OpenEx(sSql,CDatabase::noOdbcDialog) )
		{
			CString strTemp;
			strTemp.Format("编号 INT,公司名 TEXT,接入类型 TEXT,使用语言 TEXT,联系人 TEXT,联系电话 TEXT,QQ TEXT,Email TEXT,业务员 TEXT,业务员电话 TEXT,业务员QQ TEXT,业务员Email TEXT,账号 TEXT,密码 TEXT,发送量 INT,接入日期 TEXT,服务器IP TEXT,服务器端口 TEXT,服务器账号 TEXT,服务器密码 TEXT,备注 TEXT");
			// 创建表结构			
			sSql = "CREATE TABLE " + tableName + " ( " + strTemp +  " ) ";
			int i = 0;
			database.ExecuteSQL(sSql);			
		}
		else
		{
			SetDlgItemText(IDC_INFO_SHOW,"创建EXCEL文件出错,导出未完成");
			return;
		}
		while( !m_adoRecrd.IsEOF())		//如果记录不为空
		{
			if ( *bExit == TRUE )
			{
				bQuit = true;
				break;
			}
			m_adoRecrd.GetCollect("customerid",iTemp);
			localID.Format("%d",iTemp);
			m_adoRecrd.GetCollect("comname",comName);
			m_adoRecrd.GetCollect("typename",accType);
			m_adoRecrd.GetCollect("langname",language);
			m_adoRecrd.GetCollect("linkman",linkman);
			m_adoRecrd.GetCollect("linkphone",linkphone);
			m_adoRecrd.GetCollect("linkqq",linkqq);
			m_adoRecrd.GetCollect("linkemail",linkemail);
			m_adoRecrd.GetCollect("account",account);
			m_adoRecrd.GetCollect("password",pwd);
			m_adoRecrd.GetCollect("num",sendnum);
			m_adoRecrd.GetCollect("firsttime",tempTime);
			accdate	= tempTime.Format("%Y-%m-%d");
			m_adoRecrd.GetCollect("serverip",serverip);
			m_adoRecrd.GetCollect("serverport",strPort);
			m_adoRecrd.GetCollect("serveracc",serveraccount);
			m_adoRecrd.GetCollect("serverpwd",serverpwd);
			m_adoRecrd.GetCollect("reverse1",strreverse);
			
			m_adoRecrd.GetCollect("saleman",saleman);
			m_adoRecrd.GetCollect("salemanphone",salemanphone);
			m_adoRecrd.GetCollect("salemanqq",salemanqq);
			m_adoRecrd.GetCollect("salemanemail",salemanEmail);
			sSql.Format("INSERT INTO %s (编号,公司名,接入类型,使用语言,联系人,联系电话,QQ,Email,业务员,业务员电话,业务员QQ,业务员Email,账号,密码,发送量,接入日期,服务器IP,服务器端口,服务器账号,服务器密码,备注)values('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",
				tableName,
				localID,
				comName,
				accType,
				language,
				linkman,
				linkphone,
				linkqq,
				linkemail,
				saleman,
				salemanphone,
				salemanqq,
				salemanEmail,
				account,
				pwd,
				sendnum,
				accdate,
				serverip,
				strPort,
				serveraccount,
				serverpwd,
				strreverse);
			database.ExecuteSQL(sSql);
			Sleep( 5 );
			m_adoRecrd.MoveNext();
		}
		m_adoRecrd.Close();
		if ( bQuit )
		{
			SetDlgItemText(IDC_INFO_SHOW,"手动停止,导出未完成");
		}
		else
		{
			SetDlgItemText(IDC_INFO_SHOW,"导出完成");
		}	
	}
	catch(...)
	{
		SetDlgItemText(IDC_INFO_SHOW,"执行异常,导出未完成");
		m_adoRecrd.Close();
	}
}



/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CSendLogQueryDlg::GetExcelDriver()
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

/************************************************************************/
/*                                                                      */
/************************************************************************/
//获得默认的文件名
BOOL CSendLogQueryDlg::GetDefaultXlsFileName(CString& sExcelFile)
{
	///默认文件名：yyyymmddhhmmss.xls
	CString timeStr = "";
	timeStr = CTime::GetCurrentTime().Format("%Y%m%d%H%M%S");
	timeStr = "用户资料-导出时间["+timeStr+"]";

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
BOOL CSendLogQueryDlg::MakeSurePathExists( CString &Path,bool FilenameIncluded)
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
/************************************************************************/
/* 添加客户                                                             */
/************************************************************************/
void CSendLogQueryDlg::OnBnClickedBtnAdd()
{
	CInfoShow infoShow;
	infoShow.SetParams(m_dbName,m_dbServer,m_dbUser,m_dbPwd,1,0);
	infoShow.DoModal();
}

void CSendLogQueryDlg::OnNMDblclkListShow3(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	POSITION pos = m_listtype.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		return;
	}
	else
	{
		int nItem = m_listtype.GetNextSelectedItem(pos);
		CString strFlag = m_listtype.GetItemText(nItem,0);
		m_strWhere.Format(" where type = %d",m_typeMap[strFlag]);
	}
	//开始查询
	OperateProc(1);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::QueryTypeInfo(CString strWhere,int iFlag,BOOL *bExit,int iCurrPage,BOOL bDetail)
{	
	iCurrPage = 1;
	m_listtype.DeleteAllItems();
	bDetail = TRUE;
	try
	{
		CAdoConnection adoConn;
		CAdoRecordSet adoRst;
		if ( !adoConn.IsOpen() )		//如果未连接上服务器
		{
			if ( !adoConn.ConnectSQLServer(m_dbServer,m_dbName,m_dbUser,m_dbPwd))//.ConnectSQLServer( m_dbServer , m_dbName, m_dbUser,m_dbPwd) )
			{
				SetDlgItemText(IDC_INFO_SHOW,"连接数据库失败");
				return ; 
			}
			adoRst.SetAdoConnection(&adoConn);
			adoRst.SetCursorLocation(adUseClient);
		}
		CString strSQL;
		CString strTempWhere;
		CString strTemp;
		int iTemp = 0;
		bool bQuit = false;		//手动退出循环


		//加截各个用户在不同类型下的个数
		if ( bDetail != TRUE)		//不显示详细信息
		{			
			//			GetDaySucCount(m_strStartDate,m_strEndDate,m_adoConn,m_adoRecrd);
		}
		else	//显示详细信息
		{
			strSQL.Format("select Acctype.*,count(customer.customerid) as totalnum from Acctype left join customer on acctype.typeid=customer.type group by acctype.typeid,typename order by typeid asc");
			if( !adoRst.Open(strSQL,adCmdText))
			{
				adoRst.Close();
				SetDlgItemText(IDC_INFO_SHOW,"执行出错,"+adoConn.GetLastErrorText());
				return;
			}
			while( !adoRst.IsEOF())		//如果记录不为空
			{
				int iCount = m_listtype.GetItemCount();
				m_listtype.InsertItem( iCount, "");			
				adoRst.GetCollect("typeid",iTemp);
				strTemp.Format("%d",iTemp);
				//m_listtype.SetItemText( iCount , 2,strTemp);

				adoRst.GetCollect("typename",strTemp);
				m_listtype.SetItemText( iCount , 0,strTemp);

				adoRst.GetCollect("totalnum",iTemp);
				strTemp.Format("%d",iTemp);
				m_listtype.SetItemText( iCount , 1,strTemp);
				adoRst.MoveNext();
			}
			adoRst.Close();
			adoConn.Close();
		}
	}
	catch(...)
	{
		SetDlgItemText(IDC_INFO_SHOW,"加载信息出现异常");
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::OnBnClickedBtnFresh()
{
	BOOL tempBool= FALSE;
	QueTypeAndLanguage();
	QueryTypeInfo("",0,&tempBool,1,TRUE);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::QueTypeAndLanguage()
{
	m_languageMap.clear();
	m_typeMap.clear();
// 	m_comboType.Clear();
// 	m_combo_language.Clear();
	while (m_comboType.GetCount() > 0)
	{
		m_comboType.DeleteString(0);
	}
	while (m_combo_language.GetCount() > 0)
	{
		m_combo_language.DeleteString(0);
	}
	CAdoConnection adoConn;
	CAdoRecordSet adoRst;
	try
	{	if ( !adoConn.IsOpen() )		//如果未连接上服务器
		{
			if ( !adoConn.ConnectSQLServer(m_dbServer,m_dbName,m_dbUser,m_dbPwd))//.ConnectSQLServer( m_dbServer , m_dbName, m_dbUser,m_dbPwd) )
			{
				SetDlgItemText(IDC_INFO_SHOW,"连接数据库失败");
				return ; 
			}
			adoRst.SetAdoConnection(&adoConn);
			adoRst.SetCursorLocation(adUseClient);
		}
		CString strSQL;
		CString strTemp = "";
		int iTemp = 0;
		strSQL.Format("select * from Acctype order by typeid asc");
		if( !adoRst.Open(strSQL,adCmdText))
		{
			adoRst.Close();
			SetDlgItemText(IDC_INFO_SHOW,"执行出错,"+adoConn.GetLastErrorText());
			adoConn.Close();
			return;
		}
		m_comboType.AddString("请选择类型");
		m_typeMap["请选择类型"] = -1;
		while(!adoRst.IsEOF())		//如果记录不为空
		{
			adoRst.GetCollect("typeid",iTemp);
			adoRst.GetCollect("typename",strTemp);
			m_comboType.AddString(strTemp);
			m_typeMap[strTemp] = iTemp;
			adoRst.MoveNext();
		}
		adoRst.Close();

		strSQL.Format("select * from langType order by langid asc");
		if( !adoRst.Open(strSQL,adCmdText))
		{
			adoRst.Close();
			SetDlgItemText(IDC_INFO_SHOW,"执行出错,"+adoConn.GetLastErrorText());
			adoConn.Close();
			return;
		}
		m_combo_language.AddString("请选择语言");
		m_languageMap["请选择语言"] = -1;
		while( !adoRst.IsEOF())		//如果记录不为空
		{
			adoRst.GetCollect("langid",iTemp);
			adoRst.GetCollect("langname",strTemp);
			m_combo_language.AddString(strTemp);
			m_languageMap[strTemp] = iTemp;
			adoRst.MoveNext();
		}
		m_comboType.SetCurSel(0);
		m_combo_language.SetCurSel(0);
	}
	catch(...)
	{
		SetDlgItemText(IDC_INFO_SHOW,"执行出错,出现异常");
	}
	adoRst.Close();
	adoConn.Close();
}

BOOL CSendLogQueryDlg::PreTranslateMessage( MSG* pMsg )
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
void CSendLogQueryDlg::OnNMRclickListShow(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION pos = m_resultList.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		return;
	}
	else
	{
		int nItem = m_resultList.GetNextSelectedItem(pos);
		CMenu menu; 
		menu.LoadMenu(IDR_MENU1); //载入事先定义的选单 
		CMenu *pmenu = menu.GetSubMenu(0);
		CPoint pos; 
		GetCursorPos(&pos);
		SetForegroundWindow();
		pmenu->TrackPopupMenu(TPM_LEFTALIGN,pos.x,pos.y,this);
	}
	*pResult = 0;
}

void CSendLogQueryDlg::OnMenuDelete()
{
	POSITION pos = m_resultList.GetFirstSelectedItemPosition();
	int nItem = m_resultList.GetNextSelectedItem(pos);
	CString strID = m_resultList.GetItemText(nItem,0);
	int iRet = MessageBox("真的要删除该客户吗?","提示",MB_ICONQUESTION|MB_YESNO);
	if (iRet != IDYES)
	{
		return;
	}
	CString strSQL = "";
	strSQL.Format("delete from Customer where [customerid]=%s",strID);
	CAdoConnection adoConn;
	CAdoRecordSet adoRst;
	try
	{	
		if ( !adoConn.IsOpen() )		//如果未连接上服务器
		{
			if ( !adoConn.ConnectSQLServer(m_dbServer,m_dbName,m_dbUser,m_dbPwd))//.ConnectSQLServer( m_dbServer , m_dbName, m_dbUser,m_dbPwd) )
			{
				return ; 
			}
			adoRst.SetAdoConnection(&adoConn);
			adoRst.SetCursorLocation(adUseClient);
		}
		if(!adoRst.Open(strSQL,adCmdText))
		{
			adoRst.Close();			
			adoConn.Close();
			MessageBox("删除客户信息失败"+adoConn.GetLastErrorText(),"错误信息");
			return;
		}
		OperateProc(1);
	}
	catch(...)
	{
		MessageBox("删除客户信息失败","错误信息");
	}
	adoRst.Close();			
	adoConn.Close();
}

void CSendLogQueryDlg::OnMenuEdit()
{
	POSITION pos = m_resultList.GetFirstSelectedItemPosition();
	int nItem = m_resultList.GetNextSelectedItem(pos);
	CString strID = m_resultList.GetItemText(nItem,0);
	CInfoShow infoShow;
	infoShow.SetParams(m_dbName,m_dbServer,m_dbUser,m_dbPwd,2,atoi(strID));
	infoShow.DoModal();
}

void CSendLogQueryDlg::OnNMDblclkListShow(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	POSITION pos = m_resultList.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		return;
	}
	else
	{
		int nItem = m_resultList.GetNextSelectedItem(pos);
		CString strFlag = m_resultList.GetItemText(nItem,0);
		CServiceDetail dlgDetail;
		dlgDetail.m_strID = strFlag;
		dlgDetail.SetParams(m_dbName,m_dbServer,m_dbUser,m_dbPwd,strFlag);
		dlgDetail.DoModal();
	}
}

void CSendLogQueryDlg::OnCbnSelchangeComboAcctype()
{
	// TODO: 在此添加控件通知处理程序代码
}
