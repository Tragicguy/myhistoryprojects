// ServiceDetail.cpp : 实现文件
//

#include "stdafx.h"
#include "SendLogQuery.h"
#include "ServiceDetail.h"
#include "DetailEdit.h"

// CServiceDetail 对话框

IMPLEMENT_DYNAMIC(CServiceDetail, CDialog)

CServiceDetail::CServiceDetail(CWnd* pParent /*=NULL*/)
	: CDialog(CServiceDetail::IDD, pParent)
{

}

CServiceDetail::~CServiceDetail()
{
}

void CServiceDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SHOW, m_resultList);
}


BEGIN_MESSAGE_MAP(CServiceDetail, CDialog)
	ON_COMMAND(IDR_DETAIL_ADD, &CServiceDetail::OnDetailAdd)
	ON_COMMAND(IDR_DETAIL_EDIT, &CServiceDetail::OnDetailEdit)
	ON_COMMAND(IDR_DETAIL_DELETE, &CServiceDetail::OnDetailDelete)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SHOW, &CServiceDetail::OnNMRclickListShow)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SHOW, &CServiceDetail::OnNMDblclkListShow)
END_MESSAGE_MAP()


// CServiceDetail 消息处理程序
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CServiceDetail::InitCtrList()
{
	DWORD dwSytle=::GetWindowLong(m_resultList.m_hWnd,GWL_STYLE);
	//设置为报表形式
	SetWindowLong(m_resultList.m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=m_resultList.GetExtendedStyle();
	m_resultList.SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	::SendMessage(m_resultList.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	m_resultList.InsertColumn(1,_T("编号"),LVCFMT_LEFT,50);
	m_resultList.InsertColumn(1,_T("时间"),LVCFMT_LEFT,120);
	m_resultList.InsertColumn(2,_T("处理细节"),LVCFMT_LEFT,750);
	m_resultList.MoveWindow(10,10,730,525);
}
BOOL CServiceDetail::OnInitDialog()
{
	CDialog::OnInitDialog();
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	SetWindowText("服务详情显示");
	InitCtrList();
	InitData();
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CServiceDetail::InitData()
{
	m_resultList.DeleteAllItems();
	CString strSQL = "";
	strSQL.Format("select * from servicedetail where customerid=%s order by begintime desc", m_strID);
	CAdoConnection adoConn;
	CAdoRecordSet adoRst;
	try
	{
		
		if ( !adoConn.IsOpen() )		//如果未连接上服务器
		{
			if ( !adoConn.ConnectSQLServer(m_dbServer,m_dbName,m_dbUser,m_dbPwd))//.ConnectSQLServer( m_dbServer , m_dbName, m_dbUser,m_dbPwd) )
			{
				//SetDlgItemText(IDC_INFO_SHOW,"连接数据库失败");
				return ; 
			}
			adoRst.SetAdoConnection(&adoConn);
			adoRst.SetCursorLocation(adUseClient);
		}
		if( !adoRst.Open(strSQL,adCmdText))
		{
			adoRst.Close();			
			return;
		}

		COleDateTime tempTime;
		CString strTemp;
		int iTemp;
		while( !adoRst.IsEOF())		//如果记录不为空
		{
			int iCount = m_resultList.GetItemCount();
			m_resultList.InsertItem( iCount, "");	
			adoRst.GetCollect("id",iTemp);
			strTemp.Format("%d",iTemp);
			m_resultList.SetItemText( iCount , 0,strTemp);

			adoRst.GetCollect("begintime",tempTime);
			m_resultList.SetItemText( iCount , 1,tempTime.Format("%Y-%m-%d %H:%M:%S"));

			adoRst.GetCollect("content",strTemp);
			m_resultList.SetItemText( iCount , 2,strTemp);
			adoRst.MoveNext();
		}
	}
	catch (...)
	{
	}	
	adoRst.Close();
	adoConn.Close();
}

void CServiceDetail::SetParams(CString strdbName,CString strdbServer,CString strdbUser,CString strPwd,CString strID)
{
	m_dbName = strdbName;
	m_dbServer = strdbServer;
	m_dbUser = strdbUser;
	m_dbPwd = strPwd;
	m_strID = strID;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CServiceDetail::PreTranslateMessage(MSG* pMsg)
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
void CServiceDetail::OnDetailAdd()
{
	CDetailEdit infoEdit;
	infoEdit.SetParams(m_dbName,m_dbServer,m_dbUser,m_dbPwd,m_strID,0);
	infoEdit.DoModal();	
}

void CServiceDetail::OnDetailEdit()
{
	POSITION pos = m_resultList.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		return;
	}
	int nItem = m_resultList.GetNextSelectedItem(pos);
	CString strID = m_resultList.GetItemText(nItem,0);
	CDetailEdit infoEdit;
	infoEdit.SetParams(m_dbName,m_dbServer,m_dbUser,m_dbPwd,strID,1);
	infoEdit.DoModal();
}

void CServiceDetail::OnDetailDelete()
{	
	POSITION pos = m_resultList.GetFirstSelectedItemPosition();
	int nItem = m_resultList.GetNextSelectedItem(pos);
	CString strID = m_resultList.GetItemText(nItem,0);
	int iRet = MessageBox("真的要删除该记录吗?","提示",MB_ICONQUESTION|MB_YESNO);
	if (iRet != IDYES)
	{
		return;
	}
	CString strSQL = "";
	strSQL.Format("delete from servicedetail where [id]=%s",strID);
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
			MessageBox("删除记录信息失败"+adoConn.GetLastErrorText(),"错误信息");
			return;
		}
		InitData();
	}
	catch(...)
	{
		MessageBox("删除记录信息失败","错误信息");
	}
	adoRst.Close();			
	adoConn.Close();
}

void CServiceDetail::OnNMRclickListShow(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION pos = m_resultList.GetFirstSelectedItemPosition();
	int iFlag = 0;
	if(pos == NULL)
	{
		iFlag = 0;		
	}
	else
	{
		iFlag = 1;
	}
	int nItem = m_resultList.GetNextSelectedItem(pos);
	CMenu menu; 
	menu.LoadMenu(IDR_MENU2); //载入事先定义的选单 
	CMenu *pmenu = menu.GetSubMenu(0);
	if (iFlag == 0)
	{
		pmenu->EnableMenuItem(IDR_DETAIL_EDIT,MF_GRAYED);
		pmenu->EnableMenuItem(IDR_DETAIL_DELETE,MF_GRAYED);
	}
	else
	{
		pmenu->EnableMenuItem(IDR_DETAIL_EDIT,MF_ENABLED);
		pmenu->EnableMenuItem(IDR_DETAIL_DELETE,MF_ENABLED);
	}
	CPoint pt; 
	GetCursorPos(&pt);
	SetForegroundWindow();
	pmenu->TrackPopupMenu(TPM_LEFTALIGN,pt.x,pt.y,this);
	*pResult = 0;
}

void CServiceDetail::OnNMDblclkListShow(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	POSITION pos = m_resultList.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		return;
	}
	int nItem = m_resultList.GetNextSelectedItem(pos);
	CString strID = m_resultList.GetItemText(nItem,0);
	CDetailEdit infoEdit;
	infoEdit.SetParams(m_dbName,m_dbServer,m_dbUser,m_dbPwd,strID,1);
	infoEdit.DoModal();	
}
