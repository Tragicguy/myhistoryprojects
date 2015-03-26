#pragma once
#include "main_frame.hpp"
class WindowImplBase;
class CMessageBoxDlg : public MainFrame
{
public:
	CMessageBoxDlg(void){}
	~CMessageBoxDlg(void){}
protected:
	virtual void Notify(TNotifyUI& msg)
	{
		__super::Notify(msg);
	}
	virtual void OnClick(TNotifyUI& msg)
	{
		CControlUI* pControl = msg.pSender;
		if( pControl->IsEqual("ok"))
		{
			::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
			return;
		}
		__super::OnClick(msg);
	}
	virtual void OnPrepare(TNotifyUI& msg)
	{
		__super::OnPrepare(msg);
		CControlUI* pControl = m_PaintManager.FindControl("maxbtn");
		if(pControl) pControl->SetVisible(false);
		pControl = m_PaintManager.FindControl("minbtn");
		if(pControl) pControl->SetVisible(false);
		CLabelUI* pLabel = (CLabelUI*)m_PaintManager.FindControl("apptitle");

		((CRichEditUI*)m_PaintManager.FindControl("notice"))->SetText(m_strMessage.c_str());
		pLabel->SetText("消息提示");
		SetWindowText(m_hWnd, "消息提示");

	}

	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam)
	{
		if (wParam == VK_RETURN)
		{
			::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
			return FALSE;
		}
		else if (wParam == VK_ESCAPE)
		{
			::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
			return TRUE;
		}
		return FALSE;
	}
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
		styleValue &= ~WS_CAPTION;
		::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

		m_PaintManager.Init(m_hWnd);
		m_PaintManager.AddPreMessageFilter(this);
		CDialogBuilder builder;
		CControlUI* pRoot = builder.Create(_T("MessageBox.xml"), (UINT)0, NULL, &m_PaintManager);
		ASSERT(pRoot && "Failed to parse XML");
		m_PaintManager.AttachDialog(pRoot);
		m_PaintManager.AddNotifier(this);

		Init();
		return 0;
	}
	virtual void OnFinalMessage(HWND hWnd)
	{
		WindowImplBase::OnFinalMessage(hWnd);
		delete this;
	}
public:
	string m_strMessage;

	static void ShowNoticeMessage(string msg, HWND hwndParent)
	{
		CMessageBoxDlg* msgboxDlg = new CMessageBoxDlg();
		if( msgboxDlg == NULL ) { return; }
		msgboxDlg->m_strMessage = msg;
		msgboxDlg->Create(hwndParent, _T("提示窗口"), UI_WNDSTYLE_DIALOG, 0, 0, 0, 0, 0, NULL);
		msgboxDlg->CenterWindow();
		msgboxDlg->ShowModal();
	}
};

class CPasswordMofiyDlg : public MainFrame
{
public:
	CPasswordMofiyDlg(void){
		m_bModify = false;
	}
	~CPasswordMofiyDlg(void){}
protected:
	virtual void Notify(TNotifyUI& msg)
	{
		__super::Notify(msg);
	}
	virtual void OnClick(TNotifyUI& msg)
	{
		CControlUI* pControl = msg.pSender;
		if( pControl->IsEqual("ok"))
		{
			m_bModify = true;
			m_strPwd = GetDlgItemText("password");
			::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
			return;
		}
		__super::OnClick(msg);
	}
	virtual void OnPrepare(TNotifyUI& msg)
	{
		__super::OnPrepare(msg);
		CControlUI* pControl = m_PaintManager.FindControl("maxbtn");
		if(pControl) pControl->SetVisible(false);
		pControl = m_PaintManager.FindControl("minbtn");
		if(pControl) pControl->SetVisible(false);
		CLabelUI* pLabel = (CLabelUI*)m_PaintManager.FindControl("apptitle");
		pLabel->SetText("认证密码修改");
		SetWindowText(m_hWnd, "认证密码修改");

		((CLabelUI*)m_PaintManager.FindControl("company"))->SetText(m_strComName.c_str());
		((CLabelUI*)m_PaintManager.FindControl("nsrsbh"))->SetText(m_strNSRSBH.c_str());
		((CLabelUI*)m_PaintManager.FindControl("password"))->SetText(m_strPwd.c_str());
	}

	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam)
	{
		if (wParam == VK_RETURN)
		{
			::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
			return FALSE;
		}
		else if (wParam == VK_ESCAPE)
		{
			::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
			return TRUE;
		}
		return FALSE;
	}
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
		styleValue &= ~WS_CAPTION;
		::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

		m_PaintManager.Init(m_hWnd);
		m_PaintManager.AddPreMessageFilter(this);
		CDialogBuilder builder;
		CControlUI* pRoot = builder.Create(_T("PasswordModify.xml"), (UINT)0, NULL, &m_PaintManager);
		ASSERT(pRoot && "Failed to parse XML");
		m_PaintManager.AttachDialog(pRoot);
		m_PaintManager.AddNotifier(this);

		Init();
		return 0;
	}
	virtual void OnFinalMessage(HWND hWnd)
	{
		WindowImplBase::OnFinalMessage(hWnd);
		//delete this;
	}
public:
	bool m_bModify;
	string m_strPwd;
	string m_strComName;
	string m_strNSRSBH;
	static string ShowPwdModify(string strcompany, string strNSRSBH, string pwd, HWND hwndParent, bool& bModify)
	{
		CPasswordMofiyDlg* msgboxDlg = new CPasswordMofiyDlg();
		if( msgboxDlg == NULL ) { return ""; }
		msgboxDlg->m_strPwd = pwd;
		msgboxDlg->m_strNSRSBH = strNSRSBH;
		msgboxDlg->m_strComName = strcompany;
		msgboxDlg->Create(hwndParent, _T("认证密码修改窗口"), UI_WNDSTYLE_DIALOG, 0, 0, 0, 0, 0, NULL);
		msgboxDlg->CenterWindow();
		msgboxDlg->ShowModal();
		string strRet = msgboxDlg->m_strPwd;
		bModify = msgboxDlg->m_bModify;
		delete msgboxDlg;
		msgboxDlg = NULL;
		return strRet;

	}
};