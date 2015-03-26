#pragma once
#include "main_frame.hpp"
#include <vector>
#include "UIFriends.hpp"
#include "MyBrowserEventHandler.h"
#include "OtherWindow.h"
#include <map>
using std::map;
class WindowImplBase;
class CClientMainDlg;
class CMainDlg : public MainFrame
{
public:
	CMainDlg(void);
	~CMainDlg(void);
	virtual void OnClick(TNotifyUI& msg);

	void OnSearchEnterprise();
	virtual void OnPrepare(TNotifyUI& msg);
	virtual void Notify(TNotifyUI& msg);

	void OnMenuItemClick( TNotifyUI &msg );
	CControlUI* CreateControl(LPCTSTR pstrClass);
	void UpdateEnterpriseList(string strNSRSBH, string strCompanyName, map<string, string>& mapList);
	void LoadSkin();
	BOOL InvokeFromWebFunction(wstring funcname, int dispIdMember, DISPPARAMS *pDispParams, VARIANT *pVarResult, LPVOID);
	void RequestService(string strSpID);
	void RegisterEventCallback(CMyBrowserEventHandler& hander);
	void OnExitFromTray();

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void ShowMenu( ::CPoint &point, CControlUI * pControl );
	void ShowLastPushMessage(LPCTSTR strTitle, LPCTSTR strMsg);
	bool IsNeedSyncSpStatus();
	void ShowMainWindow();
	void ShowDetailMessage(LPCTSTR strUrl);
	string GetDllExecResult(int nRet);
	string GetLocationUrl(CWebBrowserUI* pWeb);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void HideMainWindow();
public:
	std::vector<FriendListItemInfo> friends_;
	CMyBrowserEventHandler m_eventhandlerSpListOrAdv;
	CMyBrowserEventHandler m_eventhandlerViewer;
	CWebBrowserUI* m_pbrowserSpList;
	CWebBrowserUI* m_pbrowserViewer;
	CWebBrowserUI* m_pbrowserAdv;

	CClientMainDlg* m_pMainDlg;
	bool m_bRealExit;
	long m_lLastForGroundTime;
};

class CAddEnterPriseDlg : public MainFrame
{
public:
	CAddEnterPriseDlg(void){
		m_bNeedAdd = false;
	}
	~CAddEnterPriseDlg(void){}
protected:
	virtual void Notify(TNotifyUI& msg)
	{
		__super::Notify(msg);
	}
	virtual void OnClick(TNotifyUI& msg)
	{
		CControlUI* pControl = msg.pSender;
		if( pControl->IsEqual("addenterprise"))
		{
			m_strNSRSBH = GetDlgItemText("nsrsbh");
			m_strPwd = GetDlgItemText("password");
			if (m_strNSRSBH == "" || m_strPwd == "")
			{
				CMessageBoxDlg::ShowNoticeMessage("请正确输入纳税人识别号及密码。", m_hWnd);
				return;
			}
			m_bNeedAdd = true;
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
		CLabelUI* pLabel = (CLabelUI*)m_PaintManager.FindControl("apptitle");
		pLabel->SetText("添加企业");
		SetWindowText(m_hWnd, "添加企业");
	}

	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
		styleValue &= ~WS_CAPTION;
		::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

		m_PaintManager.Init(m_hWnd);
		m_PaintManager.AddPreMessageFilter(this);
		CDialogBuilder builder;
		CControlUI* pRoot = builder.Create(_T("AddEnterprise.xml"), (UINT)0, NULL, &m_PaintManager);
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
	string m_strNSRSBH;
	string m_strPwd;	
	bool m_bNeedAdd;	
};
