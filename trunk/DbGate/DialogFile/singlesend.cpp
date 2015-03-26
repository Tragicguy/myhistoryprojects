// CSingleSend 消息处理程序
// DialogFile/SingleSend.cpp : 实现文件
//
#include "stdafx.h"
#include "../DbGate.h"
#include "SingleSend.h"
#include ".\SingleSend.h"
#include "../ControlFile/UserFile.h"
#include "../MainFrm.h"
// CSingleSend 对话框

IMPLEMENT_DYNAMIC(CSingleSend, CDialog)

CSingleSend::CSingleSend(CWnd* pParent /*=NULL*/)
	: CDialog(CSingleSend::IDD, pParent)
	, m_Content(_T(""))
	, m_strPhone(_T(""))
{

}

CSingleSend::~CSingleSend()
{
}

void CSingleSend::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CONTENT, m_Content);
	DDV_MaxChars(pDX, m_Content, 300);
	DDX_Text(pDX, IDC_EDIT_PHONE, m_strPhone);
	DDV_MaxChars(pDX, m_strPhone, 15000);
}


BEGIN_MESSAGE_MAP(CSingleSend, CDialog)
	ON_BN_CLICKED(IDC_BTN_SEND, &CSingleSend::OnBnClickedBtnSend)
END_MESSAGE_MAP()

BOOL CSingleSend::OnInitDialog()
{
	CDialog::OnInitDialog();
	HICON hIcon;
	hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(hIcon, TRUE);			// 设置大图标
	SetIcon(hIcon, FALSE);		// 设置小图标
	//加载最后一次保存的发送内容
	CUserFile *pFile = NULL;
	pFile = CUserFile::GetInstance();
	if (pFile)
	{
		CString strPhone = "";
		CString strContent = "";
		CString strSubPort = "";
		CString strTemp = "";
		int iTemp = 0;
		CString strAppPath = "";
		pFile->GetAppPath(strAppPath);
		CString strIniFile = strAppPath + "mwgate.ini";
		pFile->ReadInitInfo(strIniFile,"singlesend","phone",strPhone,iTemp,0);
		pFile->ReadInitInfo(strIniFile,"singlesend","content",strContent,iTemp,0);
		pFile->ReadInitInfo(strIniFile,"singlesend","subport",strSubPort,iTemp,0);
		SetDlgItemText(IDC_EDIT_PHONE,strPhone);
		SetDlgItemText(IDC_EDIT_CONTENT,strContent);
		SetDlgItemText(IDC_EDIT_SUBPORT,strSubPort);
	}
	return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSingleSend::OnBnClickedBtnSend()
{
	CString strPhone = "";
	CString strContent = "";
	CString strSubPort = "";
	try
	{
		GetDlgItemText(IDC_EDIT_PHONE,strPhone);
		GetDlgItemText(IDC_EDIT_CONTENT,strContent);
		GetDlgItemText(IDC_EDIT_SUBPORT,strSubPort);
		strPhone.Trim();
		strContent.Trim();
		strSubPort.Trim();
		if (strPhone == "" || strContent == "")
		{
			MessageBox("手机号或发送内容不能为空!","警告");
			return;
		}
		if (strContent.GetLength() > 300)
		{
			MessageBox("信息内容长度不能超过300!","警告");
			return;
		}
		if (strSubPort == "")
		{
			strSubPort = "*";
		}

		((CMainFrame *)this->GetParent())->SendSingleMessage(strPhone,strSubPort,strContent);
		//////////////////////////////////
		CUserFile *pFile = NULL;
		pFile = CUserFile::GetInstance();
		if (pFile)
		{
			CString strTemp = "";
			int iTemp = 0;
			CString strAppPath = "";
			pFile->GetAppPath(strAppPath);
			CString strIniFile = strAppPath + "mwgate.ini";
			pFile->WriteInitInfo(strIniFile,"singlesend","phone",strPhone,iTemp,0);
			pFile->WriteInitInfo(strIniFile,"singlesend","content",strContent,iTemp,0);
			pFile->WriteInitInfo(strIniFile,"singlesend","subport",strSubPort,iTemp,0);
		}
	}
	catch(...)
	{
	}	
	OnOK();
}
