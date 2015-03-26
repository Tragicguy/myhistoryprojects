#include "StdAfx.h"
#include "LoginDlg.h"
#include <windows.h>
#include "InfoShowDlg.h"
#if !defined(UNDER_CE)
#include <shellapi.h>
#endif

#include "OtherWindow.h"
CLoginDlg::CLoginDlg(void) :MainFrame()
{
	//m_bShowMaxMizeBox = false;
	//m_strSkinFile = "aaaa";
}


CLoginDlg::~CLoginDlg(void)
{

}

void CLoginDlg::Notify(TNotifyUI& msg)
{
	__super::Notify(msg);
}

string CLoginDlg::GetMessageFromResult(int nRet)
{
	switch(nRet)
	{
	case -1:
		return "用户名或者密码错误";
	case -2:
		return "登录失败，网络错误";
	case -3:
		return "登录失败，出现异常";
	case -4:
		return "您已经在线，请勿重复登录";
	}
	return "未知代码";
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
		PostMessage(WM_CLOSE, 0, 0);
		return;
	}
	else if(pControl->IsEqual("reguser"))
	{
		CInfoShowDlg* pInfoShow = new CInfoShowDlg();
		if( pInfoShow == NULL ) { Close(); return; }
		pInfoShow->m_strTitle = "用户注册窗口";
		pInfoShow->m_strUrl = "http://www.qq.com";
		pInfoShow->Create(m_hWnd, _T("用户注册"), UI_WNDSTYLE_DIALOG, 0, 0, 0, 0, 0, NULL);
		pInfoShow->CenterWindow();
		pInfoShow->ShowModal();
		return;
	}
	else if (pControl->IsEqual("login1"))
	{
		CControlUI* pControl1 = m_PaintManager.FindControl("password");
		if(m_strFocusName == "1")
		{
			m_strFocusName = "0";
			pControl1 = m_PaintManager.FindControl("userid");
		}
		else
		{
			m_strFocusName = "1";
		}
		pControl1->SetFocus();
		
	}
	else if (pControl->IsEqual("deletesel"))
	{
		
	}
	__super::OnClick(msg);
}


LRESULT CLoginDlg::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (WM_MSG_SET_FOCUS == uMsg)
	{
		CWebBrowserUI* m_pbrowserSpList = static_cast<CWebBrowserUI*>(m_PaintManager.FindControl(_T("ie")));
		if (m_pbrowserSpList)
		{
			m_pbrowserSpList->Navigate2("http://www.baidu.com");
		}
	}
	return __super::HandleCustomMessage(uMsg, wParam, lParam, bHandled);
}

void CLoginDlg::LoadSkin()
{

	

	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_PaintManager.Init(m_hWnd);
	CDialogBuilder builder;
	//GetSkinFile().GetData()
	CControlUI* pRoot = builder.Create("mainwindow1.xml", (UINT)0,  NULL, &m_PaintManager);
	ASSERT(pRoot && "Failed to parse XML");
	m_PaintManager.AttachDialog(pRoot);
	m_PaintManager.AddNotifier(this);
	SetWindowText(m_hWnd, _T("1800小助手-登录窗口"));
	//Init();
}


void CLoginDlg::OnPrepare(TNotifyUI& msg)
{
	__super::OnPrepare(msg);
	CControlUI* pControl = m_PaintManager.FindControl("maxbtn");
	if(pControl) pControl->SetVisible(false);
	
	CCheckBoxUI* pCheckBox = (CCheckBoxUI*)m_PaintManager.FindControl("autologin");
	//pCheckBox->SetCheck(true);
	
	static CMyBrowserEventHandler eventhandle;
	CWebBrowserUI* m_pbrowserSpList = static_cast<CWebBrowserUI*>(m_PaintManager.FindControl(_T("ie")));
	if (m_pbrowserSpList)
	{
		
		m_pbrowserSpList->SetWebBrowserEventHandler(&eventhandle);
		m_pbrowserSpList->Navigate2("about:blank");
		PostMessage(WM_MSG_SET_FOCUS);
	//	m_pbrowserSpList->Navigate2("http://www.baidu.com");
	}
	CWebBrowserUI* m_pbrowserSpList1 = static_cast<CWebBrowserUI*>(m_PaintManager.FindControl(_T("ie2")));
	if (m_pbrowserSpList1)
	{

		m_pbrowserSpList1->SetWebBrowserEventHandler(&eventhandle);
		m_pbrowserSpList1->Navigate2("http://www.baidu.com");
	}

	CListUI *list = static_cast<CListUI*>(m_PaintManager.FindControl(_T("listex"))); 
	list->SetKeyboardEnabled(false);
	//初始化树的显示 
	CTreeViewUI* pTree = static_cast<CTreeViewUI*>(m_PaintManager.FindControl(_T("tree")));
	if (!pTree)
	{
		return;
	}
	pControl = m_PaintManager.FindControl(_T("drag"));
	pTree->m_pDragingCtrl = pControl;

	CTreeNodeUI* pNodeRoot = new CTreeNodeUI;
	pNodeRoot->SetAttribute(_T("folderattr"),_T("padding=\"0,1,0,0\" width=\"36\" height=\"16\" normalimage=\"file='treeview_b.png' source='0,0,36,16'\" hotimage=\"file='treeview_b.png' source='36,0,72,16'\" selectedimage=\"file='treeview_a.png' source='0,0,36,16' selectedhotimage=\"file='treeview_a.png' source='36,0,72,16'\""));
	pTree->Add(pNodeRoot);
	CDuiString strNode = "";
	strNode.Format("主机列表");
	pNodeRoot->SetItemText(strNode);
	pNodeRoot->SetExpand(true);
	//m_pNodeRoot = pNodeRoot;
	for (int i = 1; i < 17; i++)
	{
		int nn = pNodeRoot->GetCountChild();
		CTreeNodeUI* pNodeParent = new CTreeNodeUI;
		pNodeRoot->AddChildNode(pNodeParent);
		pNodeParent->SetName(strNode);
		pNodeParent->SetAttribute(_T("folderattr"),_T("padding=\"0,1,0,0\" width=\"36\" height=\"16\" normalimage=\"file='treeview_b.png' source='0,0,36,16'\" hotimage=\"file='treeview_b.png' source='36,0,72,16'\" selectedimage=\"file='treeview_a.png' source='0,0,36,16' selectedhotimage=\"file='treeview_a.png' source='36,0,72,16'\""));
		CDuiString strNode = "";
		strNode.Format("目录%d", i);
		pNodeParent->SetItemText(strNode);
		pNodeParent->SetName(strNode);
		if (i % 4 == 0)
		{
			pNodeParent->SetExpand(true);
		}
		for (int j = 1; j< 7; j++)
		{
			CTreeNodeUI* pNodeChild = new CTreeNodeUI(true);	//表示是叶子节点 
			CDuiString strNode = "";
			strNode.Format("%d_%d", i, j);
			pNodeChild->SetItemText(strNode);
			pNodeChild->SetName(strNode);
			pNodeParent->AddChildNode(pNodeChild);
		}
	}
}
