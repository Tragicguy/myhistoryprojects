#pragma once
#include "../DuiLibWrapper/main_frame.hpp"
class WindowImplBase;
#define WM_MSG_SET_FOCUS WM_USER + 12
class CLoginDlg : public MainFrame
{
public:
	CLoginDlg(void);
	~CLoginDlg(void);
	void LoadSkin();
	void TestSet(bool bValue)
	{
		LRESULT lr;
		CControlUI* pControl1 = m_PaintManager.FindControl("login1");
		CControlUI* pControl = m_PaintManager.GetFocus();
		//pControl1->SetFocus();
		//m_strFocusName = pControl->GetName();
		//pControl1->SetUserData(pControl->GetName());		
		SendClickNotify("login1");
	}
	bool IsWebBrowserWnd(HWND hwnd)
	{
		return false;
		CWebBrowserUI* m_pbrowserSpList = static_cast<CWebBrowserUI*>(m_PaintManager.FindControl(_T("ie")));
		return m_pbrowserSpList->GetHostWindow() == hwnd;
	}

protected:
	virtual void Notify(TNotifyUI& msg);
	virtual void OnClick(TNotifyUI& msg);
	virtual void OnPrepare(TNotifyUI& msg);
	string GetMessageFromResult(int nRet);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	string m_strFocusName;
};

