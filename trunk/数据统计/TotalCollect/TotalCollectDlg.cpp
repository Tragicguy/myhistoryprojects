// TotalCollectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TotalCollect.h"
#include "TotalCollectDlg.h"
#include ".\totalcollectdlg.h"
#include <io.h>
#include <atlbase.h>
#include "afxinet.h"
#include <io.h>
#include <odbcinst.h>
#include <afxdb.h>
#pragma comment(lib,"wininet.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef struct _DetailStruct
{
	CString strKey;
	CString strPosStr;
}DetailStruct;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
int g_iOptType = 0;	//0表示导入，1表示分析，2表示显示详情
CArray<CString, CString> g_strList;
CString g_strPath = "";
bool g_bExitFlag = true;
int g_iTotalNum = 0;
CString g_strTemplate = "900,800,700,600,009,008,007,006,090,080,070,060";
CString g_strSelectTail = "";
CArray<DetailStruct, DetailStruct> g_DetailStructList;
CArray<DetailStruct, DetailStruct>  g_UnionList;		//连期出现
map<CString, int> g_keyMap;
int g_iSelectPos = -1;
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


// CTotalCollectDlg 对话框



CTotalCollectDlg::CTotalCollectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTotalCollectDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTotalCollectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TOTAL, m_list_total);
	DDX_Control(pDX, IDC_LIST_DETAIL, m_list_detail);
	DDX_Control(pDX, IDC_LIST_DETAIL2, m_listResult);
}

BEGIN_MESSAGE_MAP(CTotalCollectDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_IMPORT, OnBnClickedBtnImport)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_ANANYZE, OnBnClickedBtnAnanyze)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_TOTAL, OnNMDblclkListTotal)
	ON_BN_CLICKED(IDC_BTN_SHOWUNION, OnBnClickedBtnShowunion)
	ON_BN_CLICKED(IDC_BTN_IMPORT2, OnBnClickedBtnImport2)
END_MESSAGE_MAP()


// CTotalCollectDlg 消息处理程序

BOOL CTotalCollectDlg::OnInitDialog()
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
// 	if (GetPower() != 0)
// 	{
// 		//exit(0);
// 	}
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	SetTableStyle();
	SetTableStyleEx();
	SetTableStyleExEx();
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CTotalCollectDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTotalCollectDlg::OnPaint() 
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
HCURSOR CTotalCollectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalCollectDlg::SetTableStyle()
{
	CListCtrl* conList = &m_list_total;
	DWORD dwSytle=::GetWindowLong(conList->m_hWnd,GWL_STYLE);
	// 设置为报表形式
	SetWindowLong(conList->m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=conList->GetExtendedStyle();
	conList->SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CImageList image;
	image.Create(1,16, ILC_COLOR, 0, 0);
	conList->SetImageList(&image, LVSIL_STATE);
	conList->MoveWindow(7,35,200,440);
	conList->InsertColumn(1,"序号",LVCFMT_LEFT,80);
	conList->InsertColumn(2,"地名",LVCFMT_LEFT,90);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalCollectDlg::SetTableStyleExEx()
{
	CListCtrl* conList = &m_listResult;
	DWORD dwSytle=::GetWindowLong(conList->m_hWnd,GWL_STYLE);
	// 设置为报表形式
	SetWindowLong(conList->m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=conList->GetExtendedStyle();
	conList->SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CImageList image;
	image.Create(1,16, ILC_COLOR, 0, 0);
	conList->SetImageList(&image, LVSIL_STATE);
	conList->MoveWindow(427,35,260,440);
	conList->InsertColumn(1,"序号",LVCFMT_LEFT,70);
	conList->InsertColumn(2,"地名",LVCFMT_LEFT,90);
	conList->InsertColumn(3,"职业",LVCFMT_LEFT,80);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalCollectDlg::SetTableStyleEx()
{
	DWORD dwSytle=::GetWindowLong(m_list_detail.m_hWnd,GWL_STYLE);
	// 设置为报表形式
	SetWindowLong(m_list_detail.m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=m_list_detail.GetExtendedStyle();
	m_list_detail.SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CImageList image;
	image.Create(1,16,ILC_COLOR,0,0);
	m_list_detail.SetImageList(&image, LVSIL_STATE);
	m_list_detail.MoveWindow(217,35,200,440);
	m_list_detail.InsertColumn(1,"序号",LVCFMT_LEFT,70);
	m_list_detail.InsertColumn(2,"职业",LVCFMT_LEFT,90);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalCollectDlg::OnBnClickedBtnImport()
{
	g_strPath = "";
	char szFilter[255] = {0};
	CString strPath = "";
	strcpy(szFilter,"Text files(*.txt)|*.txt");
	CFileDialog dlg(1,NULL,NULL,NULL,szFilter); 
	if(dlg.DoModal()==IDOK) 
	{
		strPath = dlg.GetPathName();
	}
	strPath.Trim();
	if (strPath != "")
	{
		g_strPath = strPath;
		//开启线程
		g_iOptType = 0;
		g_bExitFlag = false;
		AfxBeginThread((AFX_THREADPROC)ThreadProc, (LPVOID)this);		
	}	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalCollectDlg::OnBnClickedBtnExit()
{
	if (g_bExitFlag = false)
	{
		g_bExitFlag = true;
		Sleep(500);
	}
	PostMessage(WM_CLOSE,0,0);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalCollectDlg::ThreadProc(LPVOID lpParam)
{
	CTotalCollectDlg *pWnd = (CTotalCollectDlg *)lpParam;
	pWnd->GetDlgItem(IDC_BTN_IMPORT)->EnableWindow(FALSE);
	pWnd->GetDlgItem(IDC_BTN_ANANYZE)->EnableWindow(FALSE);
	pWnd->GetDlgItem(IDC_BTN_SHOWUNION)->EnableWindow(FALSE);
	pWnd->OperateProc();
	pWnd->GetDlgItem(IDC_BTN_IMPORT)->EnableWindow(TRUE);
	pWnd->GetDlgItem(IDC_BTN_ANANYZE)->EnableWindow(TRUE);
	pWnd->GetDlgItem(IDC_BTN_SHOWUNION)->EnableWindow(TRUE);
	g_bExitFlag = true;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalCollectDlg::OperateProc()
{
	CString strFormat = "";
	switch(g_iOptType)
	{
	case 0:	//导入
		{	
			//m_list_total.DeleteAllItems();
			//m_list_detail.DeleteAllItems();
			CStdioFile   file;
			CString   cstr = "";
			CFileException   fe;
			int iNum = 0;
			if(file.Open(g_strPath,CFile::modeRead,&fe))   
			{
				file.SeekToBegin();
				SetDlgItemText(IDC_EDIT_NOTICE,"正在导入,请稍后...");
				g_strList.RemoveAll();
				while (file.ReadString(cstr))
				{
					if (g_bExitFlag)
					{
						break;
					}
					cstr.Trim();
					if (cstr != "")
					{
						//g_strList.Add(cstr);
						int nCount = m_list_total.GetItemCount();
						bool bExist = false;
						for (int i=0;i<nCount;i++)
						{
							if (m_list_total.GetItemText(i,1)==cstr)
							{
								bExist = true;
							}
						}
						if (bExist)
						{
							continue;
						}
						strFormat.Format("%d",nCount);
						m_list_total.InsertItem( nCount,strFormat);
						m_list_total.SetItemText(nCount,1,cstr);
						iNum++;
					}				
				}
				g_iTotalNum = iNum;
				if (g_bExitFlag)
				{
					strFormat.Format("手动终止导入,共导入%d条记录信息",iNum);
				}
				else
				{
					strFormat.Format("导入完成,共导入%d条记录信息",iNum);
				}
				SetDlgItemText(IDC_EDIT_NOTICE,strFormat);
			}
		}
		break;
	case 5:
		{
			CStdioFile   file;
			CString   cstr = "";
			CFileException   fe;			
			int iNum = 0;
			if(file.Open(g_strPath,CFile::modeRead,&fe))   
			{
				file.SeekToBegin();
				SetDlgItemText(IDC_EDIT_NOTICE,"正在导入,请稍后...");
				g_strList.RemoveAll();
				while (file.ReadString(cstr))
				{
					if (g_bExitFlag)
					{
						break;
					}
					cstr.Trim();
					if (cstr != "")
					{
						//g_strList.Add(cstr);
						int nCount = m_list_detail.GetItemCount();
						bool bExist = false;
						for (int i=0;i<nCount;i++)
						{
							if (m_list_detail.GetItemText(i,1)==cstr)
							{
								bExist = true;
							}
						}
						if (bExist)
						{
							continue;
						}
						strFormat.Format("%d",nCount);
						m_list_detail.InsertItem( nCount,strFormat);
						m_list_detail.SetItemText(nCount,1,cstr);
						iNum++;
					}				
				}
				g_iTotalNum = iNum;
				if (g_bExitFlag)
				{
					strFormat.Format("手动终止导入,共导入%d条记录信息",iNum);
				}
				else
				{
					strFormat.Format("导入完成,共导入%d条记录信息",iNum);
				}
				SetDlgItemText(IDC_EDIT_NOTICE,strFormat);
			}
		}
		break;
	case 1://分析
		{
			g_keyMap.clear();
			CString strAddr = "";
			CString strJob = "";
			int iaddrNum = m_list_total.GetItemCount();
			int iJobNum = m_list_detail.GetItemCount();
			int iNum = 0;
			for (int i=0;i<iaddrNum;i++)
			{
				if (g_bExitFlag)
				{
					break;
				}
				strAddr = m_list_total.GetItemText(i,1);
				for (int j=0;j<iJobNum;j++)
				{
					if (g_bExitFlag)
					{
						break;
					}
					strJob = m_list_detail.GetItemText(j,1);
					if (g_keyMap[strAddr+strJob] == 1)
					{
						continue;
					}
					else
					{
						g_keyMap[strAddr+strJob] = 1;
					}
					int nCount = m_listResult.GetItemCount();
					strFormat.Format("%d",nCount);
					m_listResult.InsertItem( nCount,strFormat);
					m_listResult.SetItemText(nCount,1,strAddr);
					m_listResult.SetItemText(nCount,2,strJob);
					iNum++;
				}
			}
			if (g_bExitFlag)
			{
				strFormat.Format("手动终止合并,共合并%d条记录信息",iNum);
			}
			else
			{
				strFormat.Format("共合完成,共合并%d条记录信息",iNum);
			}
			SetDlgItemText(IDC_EDIT_NOTICE,strFormat);
		}
		break;
	case 2://详情
		{
			m_list_detail.DeleteAllItems();
			SetDlgItemText(IDC_EDIT_NOTICE,"正在显示详情,请稍后...");
			CString strPosList = g_DetailStructList.GetAt(g_iSelectPos).strPosStr;
			CString strLeft = "";
			CString strRight = "";
			CString strPos = "";
			CString strResult = "";
			int idx1 = 0;
			int idx2 = -1;
			while (true)
			{
				if (g_bExitFlag)
				{
					break;
				}
				idx2 = strPosList.Find(',',idx1);
				if (idx2 == -1)
				{
					break;
				}
				strPos = strPosList.Mid(idx1,idx2-idx1);
				strResult = g_strList.GetAt(atoi(strPos));
				int nCount = m_list_detail.GetItemCount();
				strFormat.Format("%d",nCount);
				m_list_detail.InsertItem( nCount,strFormat);
				m_list_detail.SetItemText(nCount,1,strResult.Left(strResult.GetLength() - 5));
				m_list_detail.SetItemText(nCount,2,strResult.Right(5));
				m_list_detail.SetItemText(nCount,3,strPos);
				idx1 = idx2 + 1;
			}
			if (g_bExitFlag)
			{
				strFormat.Format("手动终止显示详情");
			}
			else
			{
				strFormat.Format("显示详情结束");	
			}
			SetDlgItemText(IDC_EDIT_NOTICE,strFormat);
		}
		break;
	case 3:			//连期显示
		{
			//导出结果
			try
			{
				CDatabase database;
				CString sDriver;
				CString sExcelFile; 
				CString sSql;
				CString tableName = "导出结果";
				// 检索是否安装有Excel驱动 "Microsoft Excel Driver (*.xls)" 
				sDriver = GetExcelDriver();
				if (sDriver.IsEmpty())
				{
					// 没有发现Excel驱动
					SetDlgItemText(IDC_EDIT_NOTICE,"请先安装Excel软件才能使用导出功能");
					return;
				}
				CString strSQL;				
				///默认文件名
				if (!GetDefaultXlsFileName(sExcelFile))
				{
					return;
				}
				SetDlgItemText(IDC_EDIT_NOTICE,"正在进行导出,请稍后...");
				// 创建进行存取的字符串
				sSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, sExcelFile);
				// 创建数据库 (既Excel表格文件)
				if( database.OpenEx(sSql,CDatabase::noOdbcDialog) )
				{
					CString strTemp;
					strTemp.Format("地名 TEXT,职业 TEXT");
					// 创建表结构			
					sSql = "CREATE TABLE " + tableName + " ( " + strTemp +  " ) ";
					int i = 0;
					database.ExecuteSQL(sSql);			
				}
				else
				{
					SetDlgItemText(IDC_EDIT_NOTICE,"创建EXCEL文件出错,导出未完成");
					return;
				}
				int iCount = m_listResult.GetItemCount();
				for (int i=0;i<iCount;i++)
				{
					sSql.Format("INSERT INTO %s (地名,职业)values('%s','%s')",tableName,m_listResult.GetItemText(i,1),m_listResult.GetItemText(i,2));
					database.ExecuteSQL(sSql);
				}
				database.Close();
				SetDlgItemText(IDC_EDIT_NOTICE,"导出完成");		
			}
			catch(...)
			{
				SetDlgItemText(IDC_EDIT_NOTICE,"执行异常,导出未完成");
			}
		}
		break;
	default:
		break;
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalCollectDlg::OnBnClickedBtnStop()
{
	g_bExitFlag = true;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
bool CTotalCollectDlg::IsRightData(CString strData)
{
	if (g_strTemplate.Find(strData) != -1)	//特殊字符
	{
		return true;
	}
	CString strLeft = strData.Left(1);
	CString strMid = strData.Mid(1,1);
	CString strRight = strData.Right(1);
	int iResult = atoi(strLeft) + atoi(strMid) + atoi(strRight);
	if (iResult < 6 || iResult > 21)
	{
		return true;
	}
	return false;

}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalCollectDlg::OnBnClickedBtnAnanyze()
{
	if (g_bExitFlag == false)
	{
		SetDlgItemText(IDC_EDIT_NOTICE,"线程正在执行中,请稍后再进行分析...");
		return;
	}
	g_iOptType = 1;
	g_bExitFlag = false;
	AfxBeginThread((AFX_THREADPROC)ThreadProc, (LPVOID)this);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalCollectDlg::OnNMDblclkListTotal(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	g_strSelectTail = "";
	if (g_bExitFlag == false)
	{
		SetDlgItemText(IDC_EDIT_NOTICE,"线程正在执行中,请稍后再进行分析...");
		return;
	}
	POSITION pos = m_list_total.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int nItem = m_list_total.GetNextSelectedItem(pos);		
		g_strSelectTail = m_list_total.GetItemText(nItem,0);
		g_iSelectPos = nItem;
		g_iOptType = 2;
		g_bExitFlag = false;
		AfxBeginThread((AFX_THREADPROC)ThreadProc, (LPVOID)this);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalCollectDlg::AddToResultList(CString strTail,CString strSrcPos)
{
	int iPos = g_keyMap[strTail];
	if (iPos == 0)
	{
		iPos = m_list_total.GetItemCount();
		g_keyMap[strTail] = iPos + 1;
		//添加
		m_list_total.InsertItem(iPos,"");
		m_list_total.SetItemText(iPos,0,strTail);
		m_list_total.SetItemText(iPos,1,"1");
		DetailStruct detailStr;
		detailStr.strKey = strTail;
		detailStr.strPosStr = strSrcPos + ",";
		g_DetailStructList.Add(detailStr);
	}
	else
	{
		iPos = iPos - 1;
		//更新
		CString strCount = m_list_total.GetItemText(iPos,1);
		strCount.Format("%d",atoi(strCount) + 1);
		m_list_total.SetItemText(iPos,1,strCount);
		g_DetailStructList.GetAt(iPos).strPosStr += strSrcPos + ",";
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CTotalCollectDlg::GetPower()
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
				if (strTemp.Find("myownpower111myownpower") !=-1)
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
void CTotalCollectDlg::OnBnClickedBtnShowunion()
{
	if (g_bExitFlag == false)
	{
		SetDlgItemText(IDC_EDIT_NOTICE,"线程正在执行中,请稍后再进行分析...");
		return;
	}
	g_iOptType = 3;
	g_bExitFlag = false;
	AfxBeginThread((AFX_THREADPROC)ThreadProc, (LPVOID)this);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CTotalCollectDlg::GetCountTotal(CString strSrc)
{
	int iCount = 0;
	int iLen = strSrc.GetLength();
	for (int i=0;i<iLen;i++)
	{
		if (strSrc.GetAt(i)==',')
		{
			iCount++;
		}
	}
	return iCount;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalCollectDlg::OnBnClickedBtnImport2()
{
	g_strPath = "";
	char szFilter[255] = {0};
	CString strPath = "";
	strcpy(szFilter,"Text files(*.txt)|*.txt");
	CFileDialog dlg(1,NULL,NULL,NULL,szFilter); 
	if(dlg.DoModal()==IDOK) 
	{
		strPath = dlg.GetPathName();
	}
	strPath.Trim();
	if (strPath != "")
	{
		g_strPath = strPath;
		//开启线程
		g_iOptType = 5;		//导入数据2
		g_bExitFlag = false;
		AfxBeginThread((AFX_THREADPROC)ThreadProc, (LPVOID)this);		
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//获得默认的文件名
BOOL CTotalCollectDlg::GetDefaultXlsFileName(CString& sExcelFile)
{
	///默认文件名：yyyymmddhhmmss.xls
	CString timeStr = "";
	timeStr = CTime::GetCurrentTime().Format("%Y%m%d%H%M%S");
	timeStr = "组合数据-导出时间["+timeStr+"]";

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
BOOL CTotalCollectDlg::MakeSurePathExists( CString &Path,bool FilenameIncluded)
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
/*                                                                      */
/************************************************************************/
CString CTotalCollectDlg::GetExcelDriver()
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