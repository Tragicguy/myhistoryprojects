// CheckCodeInput.cpp : 实现文件
//

#include "stdafx.h"
#include "DangdangWindow.h"
#include "CheckCodeInput.h"


// CCheckCodeInput 对话框

IMPLEMENT_DYNAMIC(CCheckCodeInput, CDialog)

CCheckCodeInput::CCheckCodeInput(CWnd* pParent /*=NULL*/)
	: CDialog(CCheckCodeInput::IDD, pParent)
{

}

CCheckCodeInput::~CCheckCodeInput()
{
}

void CCheckCodeInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCheckCodeInput, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CCheckCodeInput::OnBnClickedButton1)
END_MESSAGE_MAP()


// CCheckCodeInput 消息处理程序

void CCheckCodeInput::OnBnClickedButton1()
{
	CString strCode = "";
	GetDlgItemText(IDC_EDIT1, strCode);
	strCode.Trim();
	if(strCode == "") 
	{
		AfxMessageBox("请输入验证码答案");
		return;
	}
	OnOK();
}
