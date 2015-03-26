#include "StdAfx.h"
#include "InfoShowDlg.h"
#include <windows.h>
#if !defined(UNDER_CE)
#include <shellapi.h>
#endif

CInfoShowDlg::CInfoShowDlg(void) :MainFrame()
{
	//m_bShowMaxMizeBox = false;
	//m_strSkinFile = "aaaa";
}


CInfoShowDlg::~CInfoShowDlg(void)
{

}

void CInfoShowDlg::Notify(TNotifyUI& msg)
{
	__super::Notify(msg);
}

void CInfoShowDlg::OnClick(TNotifyUI& msg)
{
	__super::OnClick(msg);
}

LRESULT CInfoShowDlg::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_PaintManager.Init(m_hWnd);
	m_PaintManager.AddPreMessageFilter(this);
	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(_T("InfoShowDlg.xml"), (UINT)0, NULL, &m_PaintManager);
	ASSERT(pRoot && "Failed to parse XML");
	m_PaintManager.AttachDialog(pRoot);
	m_PaintManager.AddNotifier(this);
	
	Init();
	return 0;
}

void CInfoShowDlg::LoadSkin()
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_PaintManager.Init(m_hWnd);
	CDialogBuilder builder;
	//GetSkinFile().GetData()
	CControlUI* pRoot = builder.Create("LoginDlg.xml", (UINT)0,  NULL, &m_PaintManager);
	ASSERT(pRoot && "Failed to parse XML");
	m_PaintManager.AttachDialog(pRoot);
	m_PaintManager.AddNotifier(this);
	//Init();
}

void CInfoShowDlg::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void CInfoShowDlg::OnPrepare(TNotifyUI& msg)
{
	__super::OnPrepare(msg);
	CControlUI* pControl = m_PaintManager.FindControl("maxbtn");
	if(pControl) pControl->SetVisible(false);
	pControl = m_PaintManager.FindControl("minbtn");
	if(pControl) pControl->SetVisible(false);
	CLabelUI* pLabel = (CLabelUI*)m_PaintManager.FindControl("apptitle");
	pLabel->SetText(m_strTitle.c_str());
	SetWindowText(m_hWnd, _T(m_strTitle.c_str()));
	CWebBrowserUI* pWeb = (CWebBrowserUI*)m_PaintManager.FindControl("ie");
	if (pWeb)
	{
		pWeb->SetWebBrowserEventHandler(&m_eventhandlerViewer);
		pWeb->Navigate2("about:blank");
		pWeb->Navigate2(m_strUrl.c_str());
	}
}