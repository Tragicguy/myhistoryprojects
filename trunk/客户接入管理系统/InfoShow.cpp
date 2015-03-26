// InfoShow.cpp : 实现文件
//

#include "stdafx.h"
#include "SendLogQuery.h"
#include "InfoShow.h"
#include "SendLogQueryDlg.h"
// CInfoShow 对话框

IMPLEMENT_DYNAMIC(CInfoShow, CDialog)

CInfoShow::CInfoShow(CWnd* pParent /*=NULL*/)
	: CDialog(CInfoShow::IDD, pParent)
{

	m_iFlag = 1;
	m_iCustomerID = -1;
}

CInfoShow::~CInfoShow()
{
}

void CInfoShow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboType);
	DDX_Control(pDX, IDC_COMBO2, m_combo_language);
	DDX_Control(pDX, IDC_DATE_TIMESTART, m_datePicker);
}


BEGIN_MESSAGE_MAP(CInfoShow, CDialog)
	ON_BN_CLICKED(IDC_BTN_DELETETYPE, &CInfoShow::OnBnClickedBtnDeletetype)
	ON_BN_CLICKED(IDC_BTN_CANCLE, &CInfoShow::OnBnClickedBtnCancle)
	ON_BN_CLICKED(IDC_BTN_ADDTYPE, &CInfoShow::OnBnClickedBtnAddtype)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CInfoShow::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_ADD, &CInfoShow::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_ADDLANG, &CInfoShow::OnBnClickedBtnAddlang)
	ON_BN_CLICKED(IDC_BTN_DELETELANG, &CInfoShow::OnBnClickedBtnDeletelang)
END_MESSAGE_MAP()

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CInfoShow::PreTranslateMessage( MSG* pMsg )
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
// CInfoShow 消息处理程序

void CInfoShow::OnBnClickedBtnDeletetype()
{
	CString strtypename = "";
	GetDlgItemText(IDC_COMBO1,strtypename);
	if (strtypename == "请选择类型")
	{
		return;
	}
	CString strSQL = "";
	strSQL.Format("delete from acctype where typename = '%s'",strtypename);
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
			MessageBox("删除类型失败"+adoConn.GetLastErrorText(),"错误信息");
			return;
		}
		//MessageBox("删除类型成功","提示");
	}
	catch(...)
	{
		MessageBox("删除类型失败","错误信息");
	}
	adoRst.Close();			
	adoConn.Close();
	QueType();	
}

BOOL CInfoShow::OnInitDialog()
{
	CDialog::OnInitDialog();
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	QueTypeAndLanguage();
	InitControls();	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CInfoShow::OnBnClickedBtnCancle()
{
	PostMessage(WM_CLOSE,0,0);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CInfoShow::QueTypeAndLanguage()
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
		//SetDlgItemText(IDC_INFO_SHOW,"执行出错,"+adoConn.GetLastErrorText());
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
		//SetDlgItemText(IDC_INFO_SHOW,"执行出错,"+adoConn.GetLastErrorText());
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
		//SetDlgItemText(IDC_INFO_SHOW,"执行出错,出现异常");
	}
	adoRst.Close();
	adoConn.Close();
}

void CInfoShow::SetParams(CString strdbName,CString strdbServer,CString strdbUser,CString strPwd,int iFlag,int iCustomerID)
{
	m_iFlag = iFlag;
	m_dbName = strdbName;
	m_dbServer = strdbServer;
	m_dbUser = strdbUser;
	m_dbPwd = strPwd;
	m_iCustomerID = iCustomerID;
}

void CInfoShow::InitControls()
{
	if (m_iFlag == 1)
	{
		GetDlgItem(IDC_BTN_ADD)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_SAVE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DATE_TIMESTART)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TIMESHOW)->ShowWindow(SW_HIDE);
		return;
	}
	else if (m_iFlag == 2)
	{
		GetDlgItem(IDC_BTN_ADD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_SAVE)->ShowWindow(SW_SHOW);
	}
	CString strSQL = "";
	strSQL.Format("select * from customer where customerid = %d",m_iCustomerID);
	CAdoConnection adoConn;
	CAdoRecordSet adoRst;
	try
	{	
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
		CString strTemp = "";
		int iTemp = 0;
		if( !adoRst.Open(strSQL,adCmdText))
		{
			adoRst.Close();			
			adoConn.Close();
			MessageBox("获取客户信息失败","错误信息");
			return;
		}
		if(!adoRst.IsEOF())		//如果记录不为空
		{
			//////////////////////////////////////////////////
			adoRst.GetCollect("comname",strTemp);
			SetDlgItemText(IDC_EDIT_COMNAME,strTemp);
			adoRst.GetCollect("linkman",strTemp);
			SetDlgItemText(IDC_EDIT_LINKMAN,strTemp);
			adoRst.GetCollect("linkphone",strTemp);
			SetDlgItemText(IDC_EDIT_LINKPHONE,strTemp);
			adoRst.GetCollect("linkqq",strTemp);
			SetDlgItemText(IDC_EDIT_LINKQQ,strTemp);
			adoRst.GetCollect("linkemail",strTemp);
			SetDlgItemText(IDC_EDIT_LINKEMAIL,strTemp);
			adoRst.GetCollect("num",iTemp);
			SetDlgItemInt(IDC_EDIT_SENDNUM,iTemp);
			//////////////////////////////////////////////////
			//初始化类型值
			adoRst.GetCollect("type",iTemp);
			if (iTemp != 0)
			{
				m_comboType.SetCurSel(GetComboIndex(iTemp,0));
			}
			adoRst.GetCollect("language",iTemp);
			if (iTemp != 0)
			{
				m_combo_language.SetCurSel(GetComboIndex(iTemp,1));
			}
			//////////////////////////////////////////////////
			adoRst.GetCollect("account",strTemp);
			SetDlgItemText(IDC_EDIT_ACCOUNT,strTemp);
			adoRst.GetCollect("password",strTemp);
			SetDlgItemText(IDC_EDIT_PWD,strTemp);
			//////////////////////////////////////////////////
			adoRst.GetCollect("saleman",strTemp);
			SetDlgItemText(IDC_EDIT_SALEMAN,strTemp);
			adoRst.GetCollect("salemanphone",strTemp);
			SetDlgItemText(IDC_EDIT_SALEPHONE,strTemp);
			adoRst.GetCollect("salemanqq",strTemp);
			SetDlgItemText(IDC_EDIT_SALEQQ,strTemp);
			adoRst.GetCollect("salemanemail",strTemp);
			SetDlgItemText(IDC_EDIT_SALEEMAIL,strTemp);
			//////////////////////////////////////////////////
			adoRst.GetCollect("serverip",strTemp);
			SetDlgItemText(IDC_EDIT_SERVERIP,strTemp);
			adoRst.GetCollect("serverport",strTemp);
			SetDlgItemText(IDC_EDIT_SERVERPORT,strTemp);
			adoRst.GetCollect("serveracc",strTemp);
			SetDlgItemText(IDC_EDIT_SERVERACC,strTemp);
			adoRst.GetCollect("serverpwd",strTemp);
			SetDlgItemText(IDC_EDIT_SERVERPWD,strTemp);
			COleDateTime tempTime;
			adoRst.GetCollect("firsttime",tempTime);
			m_datePicker.SetTime(tempTime);
			/////////////////////////////////////////////////////
			adoRst.GetCollect("reverse1",strTemp);
			SetDlgItemText(IDC_EDIT_REVERSE,strTemp);	
		}
	}
	catch(...)
	{
			MessageBox("获取客户信息异常","错误信息");
	}
	adoRst.Close();			
	adoConn.Close();	
}
void CInfoShow::OnBnClickedBtnAddtype()
{
	CString strtypename = "";
	GetDlgItemText(IDC_EDIT_TYPE,strtypename);
	strtypename.Trim();
	if (strtypename == "")
	{
		MessageBox("类型名不能为空!","错误");
		return;
	}
	CString strSQL = "";
	strSQL.Format("insert into acctype (typename)values('%s')",strtypename);
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
			MessageBox("添加类型失败"+adoConn.GetLastErrorText(),"错误信息");
			return;
		}
		//MessageBox("添加类型成功","提示");
	}
	catch(...)
	{
		MessageBox("添加类型失败","错误信息");
	}
	adoRst.Close();			
	adoConn.Close();
	SetDlgItemText(IDC_EDIT_TYPE,"");
	QueType();
}

void CInfoShow::OnBnClickedBtnSave()
{
	CString strTemp;

	CString comName;
	int iaccType;
	int ilanguage;

	CString linkman;
	CString linkphone;
	CString linkqq;
	CString linkemail;

	CString account;
	CString pwd;

	CString sendnum;
	CString strreserve;

	CString serverip;
	CString serveraccount;
	CString serverpwd;
	CString serverport;	

	CString saleman;
	CString salemanphone;
	CString salemanqq;
	CString salemanEmail;
	
	CString strDate = "";
	GetDlgItemText(IDC_DATE_TIMESTART,strDate);
	GetDlgItemText(IDC_EDIT_COMNAME,comName);
	comName.Trim();
	if (comName == "")
	{
		MessageBox("公司名不能为空!","警告");
		return;
	}
	GetDlgItemText(IDC_COMBO1,strTemp);
	iaccType = m_typeMap[strTemp];
	if (iaccType == 0 || iaccType == -1)
	{
		MessageBox("请选择接入类型!","警告");
		return;
	}
	GetDlgItemText(IDC_COMBO2,strTemp);
	ilanguage = m_languageMap[strTemp];
	if (ilanguage == 0 || ilanguage == -1)
	{
		MessageBox("请选择语言!","警告");
		return;
	}
	GetDlgItemText(IDC_EDIT_LINKMAN,linkman);
	linkman.Trim();
	if (linkman == "")
	{
		MessageBox("联系人不能为空","警告");
		return;
	}
	GetDlgItemText(IDC_EDIT_LINKPHONE,linkphone);
	linkphone.Trim();
	if (linkphone == "")
	{
		MessageBox("联系电话不能为空","警告");
		return;
	}
	GetDlgItemText(IDC_EDIT_LINKQQ,linkqq);
	GetDlgItemText(IDC_EDIT_LINKEMAIL,linkemail);

	GetDlgItemText(IDC_EDIT_ACCOUNT,account);
	GetDlgItemText(IDC_EDIT_PWD,pwd);

	GetDlgItemText(IDC_EDIT_SENDNUM,sendnum);
	GetDlgItemText(IDC_EDIT_REVERSE,strreserve);

	GetDlgItemText(IDC_EDIT_SERVERIP,serverip);
	GetDlgItemText(IDC_EDIT_SERVERPORT,serverport);
	GetDlgItemText(IDC_EDIT_SERVERACC,serveraccount);
	GetDlgItemText(IDC_EDIT_SERVERPWD,serverpwd);

	GetDlgItemText(IDC_EDIT_SALEMAN,saleman);
	saleman.Trim();
	if (saleman == "")
	{
		MessageBox("业务员不能为空","警告");
		return;
	}
	GetDlgItemText(IDC_EDIT_SALEPHONE,salemanphone);
	GetDlgItemText(IDC_EDIT_SALEQQ,salemanqq);
	GetDlgItemText(IDC_EDIT_SALEEMAIL,salemanEmail);

	CString strSQL = "";
	strSQL.Format("update [Customer] set [comname]='%s',[type]=%d,[language]=%d,[account]='%s',[password]='%s',\
				  [linkman]='%s',[linkphone]='%s',[linkqq]='%s',[linkemail]='%s',[serverip]='%s',\
				  [serverport]='%s',[serveracc]='%s',[serverpwd]='%s',[firsttime]='%s',[num]=%d,[reverse1]='%s',\
				  [saleman]='%s',[salemanphone]='%s',[salemanqq]='%s',[salemanemail]='%s' where [customerid]=%d",
				  comName,iaccType,ilanguage,
				  account,pwd,
				  linkman,linkphone,linkqq,linkemail,
				  serverip,serverport,serveraccount,serverpwd,
				  strDate,
				  atoi(sendnum),
				  strreserve,
				  saleman,
				  salemanphone,
				  salemanqq,
				  salemanEmail,
				  m_iCustomerID
				  );
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
			MessageBox("修改客户信息失败"+adoConn.GetLastErrorText(),"错误信息");
			return;
		}
		//MessageBox("修改客户信息成功","提示");
	}
	catch(...)
	{
		MessageBox("修改客户信息失败","错误信息");
		adoRst.Close();			
		adoConn.Close();
		return;
	}
	adoRst.Close();			
	adoConn.Close();
	((CSendLogQueryDlg*)GetParent())->OperateProc(1);
	PostMessage(WM_CLOSE,0,0);
}

void CInfoShow::OnBnClickedBtnAdd()
{
	CString strTemp;

	CString comName;
	int iaccType;
	int ilanguage;

	CString linkman;
	CString linkphone;
	CString linkqq;
	CString linkemail;

	CString account;
	CString pwd;

	CString sendnum;
	CString strreserve;

	CString serverip;
	CString serveraccount;
	CString serverpwd;
	CString serverport;	

	CString saleman;
	CString salemanphone;
	CString salemanqq;
	CString salemanEmail;

	GetDlgItemText(IDC_EDIT_COMNAME,comName);
	comName.Trim();
	if (comName == "")
	{
		MessageBox("公司名不能为空!","警告");
		return;
	}
	GetDlgItemText(IDC_COMBO1,strTemp);
	iaccType = m_typeMap[strTemp];
	if (iaccType == 0 || iaccType == -1)
	{
		MessageBox("请选择接入类型!","警告");
		return;
	}
	GetDlgItemText(IDC_COMBO2,strTemp);
	ilanguage = m_languageMap[strTemp];
	if (ilanguage == 0 || ilanguage == -1)
	{
		MessageBox("请选择语言!","警告");
		return;
	}
	GetDlgItemText(IDC_EDIT_LINKMAN,linkman);
	linkman.Trim();
	if (linkman == "")
	{
		MessageBox("联系人不能为空","警告");
		return;
	}
	GetDlgItemText(IDC_EDIT_LINKPHONE,linkphone);
	linkphone.Trim();
	if (linkphone == "")
	{
		MessageBox("联系电话不能为空","警告");
		return;
	}
	GetDlgItemText(IDC_EDIT_LINKQQ,linkqq);
	GetDlgItemText(IDC_EDIT_LINKEMAIL,linkemail);

	GetDlgItemText(IDC_EDIT_ACCOUNT,account);
	GetDlgItemText(IDC_EDIT_PWD,pwd);

	GetDlgItemText(IDC_EDIT_SENDNUM,sendnum);
	GetDlgItemText(IDC_EDIT_REVERSE,strreserve);

	GetDlgItemText(IDC_EDIT_SERVERIP,serverip);
	GetDlgItemText(IDC_EDIT_SERVERPORT,serverport);
	GetDlgItemText(IDC_EDIT_SERVERACC,serveraccount);
	GetDlgItemText(IDC_EDIT_SERVERPWD,serverpwd);

	GetDlgItemText(IDC_EDIT_SALEMAN,saleman);
	saleman.Trim();
	if (saleman == "")
	{
		MessageBox("业务员不能为空","警告");
		return;
	}
	GetDlgItemText(IDC_EDIT_SALEPHONE,salemanphone);
	GetDlgItemText(IDC_EDIT_SALEQQ,salemanqq);
	GetDlgItemText(IDC_EDIT_SALEEMAIL,salemanEmail);

	CString strSQL = "";
	strSQL.Format("INSERT INTO [Customer]([comname],[type],[language],[account],[password],[linkman],\
				  [linkphone],[linkqq],[linkemail],[serverip],[serverport],[serveracc],[serverpwd],[num]\
	,[reverse1],[saleman],[salemanphone],[salemanqq],[salemanemail])values('%s',%d,%d,'%s','%s','%s','%s',\
	'%s','%s','%s','%s','%s','%s',%d,'%s','%s','%s','%s','%s')",
		comName,iaccType,ilanguage,
		account,pwd,
		linkman,linkphone,linkqq,linkemail,
		serverip,serverport,serveraccount,serverpwd,
		atoi(sendnum),
		strreserve,
		saleman,
		salemanphone,
		salemanqq,
		salemanEmail
		);
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
			MessageBox("添加客户信息失败"+adoConn.GetLastErrorText(),"错误信息");
			return;
		}
		//MessageBox("添加客户信息成功","提示");
	}
	catch(...)
	{
		MessageBox("添加客户信息失败","错误信息");
		adoRst.Close();			
		adoConn.Close();
		return;
	}
	adoRst.Close();			
	adoConn.Close();
	((CSendLogQueryDlg*)GetParent())->OperateProc(1);
	PostMessage(WM_CLOSE,0,0);
}

void CInfoShow::OnBnClickedBtnAddlang()
{
	CString strlangname = "";
	GetDlgItemText(IDC_EDIT_LANG,strlangname);
	strlangname.Trim();
	if (strlangname == "")
	{
		MessageBox("语言名不能为空!","错误");
		return;
	}
	CString strSQL = "";
	strSQL.Format("insert into langtype (langname)values('%s')",strlangname);
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
			MessageBox("添加语言失败"+adoConn.GetLastErrorText(),"错误信息");
			return;
		}
		//MessageBox("添加语言成功","提示");		
	}
	catch(...)
	{
		MessageBox("添加语言失败","错误信息");
	}
	adoRst.Close();			
	adoConn.Close();
	SetDlgItemText(IDC_EDIT_LANG,"");
	QueLanguage();
}

void CInfoShow::OnBnClickedBtnDeletelang()
{
	CString strlangname = "";
	GetDlgItemText(IDC_COMBO2,strlangname);
	if (strlangname == "请选择语言")
	{
		return;
	}
	CString strSQL = "";
	strSQL.Format("delete from langtype where langname = '%s'",strlangname);
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
			MessageBox("删除语言失败"+adoConn.GetLastErrorText(),"错误信息");
			return;
		}
		//MessageBox("删除语言成功","提示");
	}
	catch(...)
	{
		MessageBox("删除语言失败","错误信息");
	}
	adoRst.Close();			
	adoConn.Close();
	QueLanguage();	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CInfoShow::GetComboIndex(int iValue,int iFlag)
{
	try
	{
		CString strValue = "";
		int iRet = 0;
		if (iFlag == 0)	//类型
		{
			map<CString,int>::iterator it;   
			for(it=m_typeMap.begin();it!=m_typeMap.end();++it)   
			{   
				int iFlag = it->second;
				if (iFlag == iValue)
				{
					strValue =  it->first;
					break;
				}		
			}
			//已经得到字符串
// 			for (int i=0;i<)
// 			{
// 			}
			iRet = m_comboType.FindString(0,strValue);
		}
		else	//语言
		{
			map<CString,int>::iterator it;   
			for(it=m_languageMap.begin();it!=m_languageMap.end();++it)   
			{   
				int iFlag = it->second;
				if (iFlag == iValue)
				{
					strValue =  it->first;
					break;
				}		
			}
			iRet = m_combo_language.FindString(0,strValue);
		}
		return iRet;
	}
	catch(...)
	{

	}
	return 0;
}

void CInfoShow::QueType()
{
	m_typeMap.clear();
	while (m_comboType.GetCount() > 0)
	{
		m_comboType.DeleteString(0);
	}
	CAdoConnection adoConn;
	CAdoRecordSet adoRst;
	try
	{	
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
		CString strTemp = "";
		int iTemp = 0;
		strSQL.Format("select * from Acctype order by typeid asc");
		if( !adoRst.Open(strSQL,adCmdText))
		{
			adoRst.Close();
			//SetDlgItemText(IDC_INFO_SHOW,"执行出错,"+adoConn.GetLastErrorText());
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
		m_comboType.SetCurSel(0);
	}
	catch(...)
	{
		//SetDlgItemText(IDC_INFO_SHOW,"执行出错,出现异常");
	}
	adoRst.Close();
	adoConn.Close();
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CInfoShow::QueLanguage()
{
	m_languageMap.clear();
	while (m_combo_language.GetCount() > 0)
	{
		m_combo_language.DeleteString(0);
	}
	CAdoConnection adoConn;
	CAdoRecordSet adoRst;
	try
	{	
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
		CString strTemp = "";
		int iTemp = 0;
		strSQL.Format("select * from langType order by langid asc");
		if( !adoRst.Open(strSQL,adCmdText))
		{
			adoRst.Close();
			//SetDlgItemText(IDC_INFO_SHOW,"执行出错,"+adoConn.GetLastErrorText());
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
		m_combo_language.SetCurSel(0);
	}
	catch(...)
	{
		//SetDlgItemText(IDC_INFO_SHOW,"执行出错,出现异常");
	}
	adoRst.Close();
	adoConn.Close();
}