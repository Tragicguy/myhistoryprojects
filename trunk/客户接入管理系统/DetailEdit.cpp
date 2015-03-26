// DetailEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "SendLogQuery.h"
#include "DetailEdit.h"

#include "ServiceDetail.h"
// CDetailEdit 对话框

IMPLEMENT_DYNAMIC(CDetailEdit, CDialog)

CDetailEdit::CDetailEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CDetailEdit::IDD, pParent)
{

}

CDetailEdit::~CDetailEdit()
{
}

void CDetailEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDetailEdit, CDialog)
	ON_BN_CLICKED(IDC_BTN_ADD, &CDetailEdit::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CDetailEdit::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CDetailEdit::OnBnClickedBtnExit)
END_MESSAGE_MAP()


// CDetailEdit 消息处理程序

void CDetailEdit::OnBnClickedBtnAdd()
{
	CString strTime;
	CString strContent;
	
	GetDlgItemText(IDC_EDIT_TIME,strTime);
	GetDlgItemText(IDC_EDIT_DETAIL,strContent);

	CString strSQL = "";
	strSQL.Format("insert into servicedetail (customerid,begintime,content) values(%s,'%s','%s')",
				m_strID,
				strTime,
				strContent);
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
			MessageBox("添加记录信息失败"+adoConn.GetLastErrorText(),"错误信息");
			return;
		}
		//MessageBox("添加记录信息成功","提示");
	}
	catch(...)
	{
		MessageBox("添加记录信息失败","错误信息");
		adoRst.Close();			
		adoConn.Close();
		return;
	}
	adoRst.Close();			
	adoConn.Close();
	((CServiceDetail*)GetParent())->InitData();
	PostMessage(WM_CLOSE,0,0);
}

void CDetailEdit::OnBnClickedBtnSave()
{
	CString strTime;
	CString strContent;

	GetDlgItemText(IDC_EDIT_TIME,strTime);
	GetDlgItemText(IDC_EDIT_DETAIL,strContent);

	CString strSQL = "";
	strSQL.Format("update servicedetail set begintime='%s',content='%s' where [id]=%s",		
		strTime,
		strContent,
		m_strID);
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
			MessageBox("修改记录信息失败"+adoConn.GetLastErrorText(),"错误信息");
			return;
		}
		//MessageBox("添加记录信息成功","提示");
	}
	catch(...)
	{
		MessageBox("修改记录信息失败","错误信息");
		adoRst.Close();			
		adoConn.Close();
		return;
	}
	adoRst.Close();			
	adoConn.Close();
	((CServiceDetail*)GetParent())->InitData();
	PostMessage(WM_CLOSE,0,0);
}

void CDetailEdit::OnBnClickedBtnExit()
{
	PostMessage(WM_CLOSE,0,0);
}

void CDetailEdit::SetParams(CString strdbName,CString strdbServer,CString strdbUser,CString strPwd,CString strID,int iFlag)
{
	m_dbName = strdbName;
	m_dbServer = strdbServer;
	m_dbUser = strdbUser;
	m_dbPwd = strPwd;
	m_strID = strID;
	m_iFlag = iFlag;
}

BOOL CDetailEdit::OnInitDialog()
{
	CDialog::OnInitDialog();
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	if (m_iFlag == 0)	//0添加
	{
		GetDlgItem(IDC_EDIT_TIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SAVE)->ShowWindow(SW_HIDE);
		SetDlgItemText(IDC_EDIT_TIME,CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"));
	}
	else
	{
		GetDlgItem(IDC_BTN_ADD)->ShowWindow(SW_HIDE);
		InitData();
	}
	SetWindowText("服务详情编辑");	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CDetailEdit::InitData()
{
	CString strSQL = "";
	strSQL.Format("select * from servicedetail where [id]=%s", m_strID);
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
		if( !adoRst.IsEOF())		//如果记录不为空
		{
			adoRst.GetCollect("begintime",tempTime);
			SetDlgItemText(IDC_EDIT_TIME,tempTime.Format("%Y-%m-%d %H:%M:%S"));

			adoRst.GetCollect("content",strTemp);
			SetDlgItemText(IDC_EDIT_DETAIL,strTemp);
		}
	}
	catch (...)
	{
	}	
	adoRst.Close();
	adoConn.Close();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CDetailEdit::PreTranslateMessage( MSG* pMsg )
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