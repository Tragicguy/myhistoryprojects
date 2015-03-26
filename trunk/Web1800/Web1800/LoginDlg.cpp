#include "StdAfx.h"
#include "LoginDlg.h"
#include <windows.h>
#if !defined(UNDER_CE)
#include <shellapi.h>
#endif
#include "UIMenu.h"

CLoginDlg::CLoginDlg(void)
{
}


CLoginDlg::~CLoginDlg(void)
{

}

void CLoginDlg::Notify(TNotifyUI& msg)
{
	__super::Notify(msg);
}

void CLoginDlg::OnClick(TNotifyUI& msg)
{
	CControlUI* pControl = msg.pSender;
	if(pControl->IsEqual("exit"))
	{
		PostMessage(WM_CLOSE, 0, 0);
		return;
	}
	else if(pControl->IsEqual("login"))
	{
		string strServiceID = GetDlgItemText("serviceid");
		string strUserID = GetDlgItemText("userid");
		string strPwd = GetDlgItemText("password");
		::MessageBox(NULL, (strServiceID + "\r\n" + strUserID + "\r\n" + strPwd).c_str(), NULL, 0);
		return;
	}
	__super::OnClick(msg);
}