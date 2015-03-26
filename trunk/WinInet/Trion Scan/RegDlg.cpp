// RegDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RegDlg.h"
#include "51JobWebPost.h"

// CRegDlg 对话框

IMPLEMENT_DYNAMIC(CRegDlg, CDialog)

CRegDlg::CRegDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRegDlg::IDD, pParent)
{
	m_iRegSuc = 0;
}

CRegDlg::~CRegDlg()
{
}

void CRegDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRegDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CRegDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CRegDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CRegDlg 消息处理程序
BOOL CRegDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	GetAppPath(m_strAppPath);
	m_iRegSuc = 0;
	return TRUE;
}
void CRegDlg::OnBnClickedButton1()
{
	CString strUser = "";
	CString strPwd = "";
	GetDlgItemText(IDC_EDIT1,strUser);
	GetDlgItemText(IDC_EDIT2,strPwd);
	m_iRegSuc = 0;
	//验证
	if (strUser == "" || strPwd == "")
	{
		AfxMessageBox("用户名或密码不能为空");
		return;
	}
	m_iRegSuc = Reg(strUser,strPwd,gl_strMac.c_str());//进行验证
	if (m_iRegSuc == 1)
	{
		CString strPath = m_strAppPath + "set.ini";
		CString strNode = "set";
		::WritePrivateProfileString(strNode,"username", strUser, strPath);
		::WritePrivateProfileString(strNode,"password", strPwd, strPath);
	}
	else
	{
		AfxMessageBox("注册失败");
		return;
	}
	PostMessage(WM_CLOSE);
}

void CRegDlg::OnBnClickedButton3()
{
	PostMessage(WM_CLOSE);
}
int CRegDlg::GetAppPath(CString &strAppPath)
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