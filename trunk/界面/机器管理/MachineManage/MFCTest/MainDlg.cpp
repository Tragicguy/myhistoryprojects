#include "StdAfx.h"
#include "MainDlg.h"
#include <windows.h>
#if !defined(UNDER_CE)
#include <shellapi.h>
#endif
#include "UIFriends.hpp"
#include "UIMenu.h"
#include "ClientMainDlg.h"
#include "InfoShowDlg.h"

#include<comutil.h> 
#pragma comment(lib,"comsuppw")

#include "FuncDefine.h"

CMainDlg::CMainDlg(void)
{
	m_pbrowserSpList = NULL;
	m_pbrowserViewer = NULL;
	m_pbrowserAdv = NULL;
	m_pMainDlg = NULL;
	m_bRealExit = false;
}

#define ID_TIMER_CHECKFORGROUND 1
#define TIMER_INTERVAL_CHECK 5*1000
#define CHANGE_INTERVAL 100*1000

CMainDlg::~CMainDlg(void)
{
}

string CMainDlg::GetDllExecResult(int nRet)
{
	switch(nRet)
	{
	case 0:
		return "打开网上认证失败";
	case -1:
		return "加载DLL失败";
	case -2:
		return "加载函数失败";
	case -3:
		return "调用DLL函数时发生未知错误";
	}
	return "错误代码值";
}

string CMainDlg::GetLocationUrl(CWebBrowserUI* pWeb)
{
	BSTR bstrStr;
	pWeb->GetLocationUrl(&bstrStr);
	string strRet = _com_util::ConvertBSTRToString(bstrStr);
	SysFreeString(bstrStr);
	return strRet;
}

void CMainDlg::OnClick(TNotifyUI& msg)
{
	 CControlUI* pControl = msg.pSender;
	 if( pControl->IsEqual("searchnsrsbh"))
	 {
		 OnSearchEnterprise();
		 return;
	 }
	 else if(pControl->IsEqual("addnsrsbh"))
	 {
		 CAddEnterPriseDlg* pAddEnterprise = new CAddEnterPriseDlg();
		 if( pAddEnterprise == NULL ) { return; }		
		 pAddEnterprise->Create(m_hWnd, _T("添加企业"), UI_WNDSTYLE_DIALOG, 0, 0, 0, 0, 0, NULL);
		 pAddEnterprise->CenterWindow();
		 pAddEnterprise->ShowModal();
		 string strNSRSBH = pAddEnterprise->m_strNSRSBH;
		 string strPwd = pAddEnterprise->m_strPwd;
		 bool bAdd = pAddEnterprise->m_bNeedAdd;
		 delete pAddEnterprise;
		 pAddEnterprise = NULL;
		 if (bAdd)
		 {
			 int nRet = m_pMainDlg->FuncFuncAddEnterprise(strNSRSBH, strPwd);
			 if (nRet < 1)
			 {
				 CMessageBoxDlg::ShowNoticeMessage(GetDllExecResult(nRet), m_hWnd);
			 }

		 }		 		 
	 }
	 else if (pControl->IsEqual("userinfo"))
	 {
		 CInfoShowDlg* pInfoShow = new CInfoShowDlg();
		 if( pInfoShow == NULL ) { return; }
		 pInfoShow->m_strTitle = "用户信息窗口";
		 pInfoShow->m_strUrl = m_pMainDlg->GetUserInfoUrl();
		 pInfoShow->Create(m_hWnd, _T("用户信息窗口"), UI_WNDSTYLE_DIALOG, 0, 0, 0, 0, 0, NULL);
		 pInfoShow->CenterWindow();
		 pInfoShow->ShowModal();
		 return;
	 }
	 else if (pControl->IsEqual("faxsubmit"))
	 {
		 CLabelUI *label_company = (CLabelUI*)m_PaintManager.FindSubControlByName(msg.pSender->GetParent()->GetParent(), _T("companyname"));
		 CLabelUI *label_nsrsbh = (CLabelUI*)m_PaintManager.FindSubControlByName(msg.pSender->GetParent()->GetParent(), _T("nsrsbh"));		
		 if (m_pMainDlg)
		 {
			 int nRet = m_pMainDlg->FuncRunExe(label_nsrsbh->GetText().GetData(), m_pMainDlg->GetPasswordByNSRSBH(label_nsrsbh->GetText().GetData()));
			 if (nRet < 1)
			 {
				 CMessageBoxDlg::ShowNoticeMessage(GetDllExecResult(nRet), m_hWnd);				 
			 }
		 }
	 }
	 else if (pControl->IsEqual("listmodifypwd"))
	 {
		 CLabelUI *label_company = (CLabelUI*)m_PaintManager.FindSubControlByName(msg.pSender->GetParent()->GetParent(), _T("companyname"));
		 CLabelUI *label_nsrsbh = (CLabelUI*)m_PaintManager.FindSubControlByName(msg.pSender->GetParent()->GetParent(), _T("nsrsbh"));		
		 if (m_pMainDlg)
		 {
			 string strNSRSBH = label_nsrsbh->GetText().GetData();
			 string strPwd = m_pMainDlg->GetPasswordByNSRSBH(strNSRSBH.c_str());
			 bool bModify = false;
			 string strNewPwd = CPasswordMofiyDlg::ShowPwdModify(label_company->GetText().GetData(), strNSRSBH, strPwd, m_hWnd, bModify);
			 if (bModify)
			 {
				 m_pMainDlg->SavePasswordByNSRSBH(strNSRSBH.c_str(), strNewPwd.c_str());
			 }
		 }
	 }
	 else if(pControl->IsEqual("btnmenu"))
	 {
		 ::CPoint point = msg.ptMouse;
		 ClientToScreen(m_hWnd, &point);
		 ShowMenu(point, pControl);
	 }
	 else if (pControl->IsEqual("showleft"))
	 {
		 CControlUI* left_main_pannel = m_PaintManager.FindControl("servicepanel");
		 if ((left_main_pannel != NULL))
		 {
			 ::SetWindowPos(m_hWnd, NULL, 0, 0, 900, 680, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
			 left_main_pannel->SetVisible(true);
			 CControlUI* pControlHideLeft = m_PaintManager.FindControl("hideleft");
			 if(pControlHideLeft) pControlHideLeft->SetVisible(true);
			 pControl->SetVisible(false);
		 }
	 }
	 else if (pControl->IsEqual("hideleft"))
	 {
		CControlUI* left_main_pannel = m_PaintManager.FindControl("servicepanel");
		 if ((left_main_pannel != NULL))
		 {
			 ::SetWindowPos(m_hWnd, NULL, 0, 0, 300, 680, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
			 left_main_pannel->SetVisible(false);
			 CControlUI* pControlHideLeft = m_PaintManager.FindControl("showleft");
			 if(pControlHideLeft) pControlHideLeft->SetVisible(true);
			 pControl->SetVisible(false);
		 }
	 }
	 else if (pControl->IsEqual("btnmodifypwd"))
	 {
		 ShowMainWindow();
		 m_pbrowserViewer->Navigate2(m_pMainDlg->GetPwdModifyUrl().c_str());	
	 }
	 else if (pControl->IsEqual("onlineclass"))
	 {
		 ShowMainWindow();
		 m_pbrowserViewer->Navigate2(m_pMainDlg->GetOnlineClassUrl().c_str());		 
	 }
	 else if (pControl->IsEqual("msghistory"))
	 {
		 ShowMainWindow();
		 m_pbrowserViewer->Navigate2(m_pMainDlg->GetHistoryUrl().c_str());		 
	 }
	 else if (pControl->IsEqual("downcenter"))
	 {
		 ShowMainWindow();
		 m_pbrowserViewer->Navigate2(m_pMainDlg->GetOnlineClassUrl().c_str());		 
	 }

	__super::OnClick(msg);
}

void CMainDlg::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("menu_msg"))
	{
		OnMenuItemClick(msg);
		return;		
	}
	if(msg.sType==_T("selectchanged"))
	{
		CDuiString name = msg.pSender->GetName();
		CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabs")));
		if(name==_T("splist"))
		{
			string strUrlOld = GetLocationUrl(m_pbrowserSpList);
			if (strUrlOld == "" || strUrlOld == "about:blank")
			{
				m_pbrowserSpList->Navigate2(m_pMainDlg->GetSpCenterUrl().c_str());
			}			
			pControl->SelectItem(0);
			m_lLastForGroundTime = ::GetTickCount();
		}
		else if(name==_T("faxs"))
			pControl->SelectItem(1);
	}
	__super::Notify(msg);
}

void CMainDlg::OnPrepare(TNotifyUI& msg)
{
	__super::OnPrepare(msg);
	CControlUI* left_main_pannel = m_PaintManager.FindControl("servicepanel");
	if ((left_main_pannel != NULL))
	{
		left_main_pannel->SetVisible(false);
		::SetWindowPos(m_hWnd, NULL, 0, 0, 300, 680, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
		CControlUI* pControl = m_PaintManager.FindControl("showleft");
		if(pControl) pControl->SetVisible(true);
		pControl = m_PaintManager.FindControl("hideleft");
		if(pControl) pControl->SetVisible(false);
		pControl = m_PaintManager.FindControl("maxbtn");
		if(pControl) pControl->SetVisible(false);
	}
	::SendMessage(m_hWnd, WM_SETICON, true, (LPARAM)m_pMainDlg->m_hIcon);
	CTabLayoutUI* pControlTab = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabs")));
	if (pControlTab)
	{
		pControlTab->SelectItem(0);
	}
	
	map<string, string> mapList;
	int nRet = m_pMainDlg->GetEnterpriseInfomation(mapList);
	UpdateEnterpriseList("", "", mapList);
	//m_PaintManager.FindControl("faxs")->SetVisible(false);
	

	CLabelUI* pLabelName = (CLabelUI*)m_PaintManager.FindControl("username");
	pLabelName->SetText((LPCTSTR)m_pMainDlg->m_strUserName);

	m_pbrowserSpList = static_cast<CWebBrowserUI*>(m_PaintManager.FindControl(_T("ie")));
	m_eventhandlerSpListOrAdv.SetShowContextMenu(false);	
	m_eventhandlerSpListOrAdv.m_OnInvokeFromWebCallBack.init();
	m_eventhandlerSpListOrAdv.m_OnInvokeFromWebCallBack.BindCallback(this, &CMainDlg::InvokeFromWebFunction);
	RegisterEventCallback(m_eventhandlerSpListOrAdv);
	

	m_eventhandlerViewer.m_OnInvokeFromWebCallBack.init();
	m_eventhandlerViewer.m_OnInvokeFromWebCallBack.BindCallback(this, &CMainDlg::InvokeFromWebFunction);
	m_eventhandlerViewer.RegisterCallBackEvent(L"CppCall", ID_TEST_INVOKE);
	
	if( m_pbrowserSpList ) {
		m_pbrowserSpList->SetWebBrowserEventHandler(&m_eventhandlerSpListOrAdv);		
		m_pbrowserSpList->Navigate2("about:blank");
		m_pbrowserSpList->Navigate2("about:blank");
	}
	m_pbrowserAdv= static_cast<CWebBrowserUI*>(m_PaintManager.FindControl(_T("adv")));
	if( m_pbrowserAdv ) {
		m_pbrowserAdv->SetWebBrowserEventHandler(&m_eventhandlerSpListOrAdv);
		m_pbrowserAdv->Navigate2("about:blank");
		m_pbrowserAdv->Navigate2(m_pMainDlg->GetAdvertisementUrl().c_str());
	}
	m_pbrowserViewer = static_cast<CWebBrowserUI*>(m_PaintManager.FindControl(_T("viewer")));
	if( m_pbrowserViewer ) 
	{
		m_pbrowserViewer->SetWebBrowserEventHandler(&m_eventhandlerViewer);		
		m_pbrowserViewer->Navigate2("about:blank");
		m_pbrowserViewer->Navigate2("about:blank");
	}

	m_pbrowserSpList->Navigate2(m_pMainDlg->GetSpCenterUrl().c_str());
	SetTimer(m_hWnd, ID_TIMER_CHECKFORGROUND, TIMER_INTERVAL_CHECK, NULL);
}
CControlUI* CMainDlg::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("CompanyList")) == 0)
	{
		return new CFriendsUI(m_PaintManager);
	}
	return NULL;
}


void CMainDlg::UpdateEnterpriseList(string strNSRSBH, string strCompanyName, map<string, string>& mapList)
{	
	CFriendsUI* pFriendsList = static_cast<CFriendsUI*>(m_PaintManager.FindControl("friends"));
	if (pFriendsList != NULL)
	{
		if (!friends_.empty())
			friends_.clear();
		if (pFriendsList->GetCount() > 0)
			pFriendsList->RemoveAll();

		if (strNSRSBH != "")
		{
			FriendListItemInfo item;
			item.id = _T("1");
			item.folder = false;		
			item.companyname = strCompanyName;
			item.nsrsbh = strNSRSBH;
			item.password = "888111";	
			pFriendsList->AddNode(item, NULL);
			friends_.push_back(item);
		}
		else
		{
			map<string, string>::iterator it;
			for (it = mapList.begin(); it != mapList.end(); ++it)
			{
				FriendListItemInfo item;
				item.id = _T("1");
				item.folder = false;		
				item.companyname = it->second;
				item.nsrsbh = it->first;
				item.password = "888111";	
				pFriendsList->AddNode(item, NULL);
				friends_.push_back(item);
			}
		}		
	}
	
}



void CMainDlg::LoadSkin()
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_PaintManager.Init(m_hWnd);
	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create("MainWindow.xml", (UINT)0,  this, &m_PaintManager);
	ASSERT(pRoot && "Failed to parse XML");
	m_PaintManager.AttachDialog(pRoot);
	m_PaintManager.AddNotifier(this);
	SetWindowText(m_hWnd, _T("1800小助手"));
	//Init();
}

void CMainDlg::OnSearchEnterprise()
{
	CFriendsUI* pFriendsList = static_cast<CFriendsUI*>(m_PaintManager.FindControl("friends"));
	if (pFriendsList != NULL)
	{
		if (!friends_.empty())
			friends_.clear();
		if (pFriendsList->GetCount() > 0)
			pFriendsList->RemoveAll();
	}
	string strNSRSBH = "";
	CTextUI* pTextUI = (CTextUI*)m_PaintManager.FindControl("editnsrsbh");
	if (pTextUI)
	{
		strNSRSBH = pTextUI->GetText();
	}
	map<string, string> mapList;
	if(m_pMainDlg)
	{
		int nRet = m_pMainDlg->GetEnterpriseInfomation(mapList);
		if(nRet >= 0)
		{
			if (strNSRSBH != "")
			{
				map<string, string>::iterator it = mapList.find(strNSRSBH);
				if (it != mapList.end())
				{
					//清空后显示
					UpdateEnterpriseList(it->first, it->second, mapList);
				}
				else
				{
					//提示结果
				}
			}
			else
			{
				UpdateEnterpriseList("", "", mapList);
			}

		}
		else
		{
			CMessageBoxDlg::ShowNoticeMessage("获取企业信息失败", m_hWnd);		
			return;
		}
	}
}

void CMainDlg::OnMenuItemClick( TNotifyUI &msg )
{
	CControlUI * pControl = (CControlUI *)msg.lParam;
	string strMenuName = (TCHAR *)(LPCTSTR)pControl->GetUserData();
	if (strMenuName == _T("downcenter") ||strMenuName == _T("msghistory") || strMenuName == _T("onlineclass") || strMenuName == _T("btnmodifypwd"))
	{	
		IsNeedSyncSpStatus();
		CControlUI* pControlSend = m_PaintManager.FindControl(strMenuName.c_str());
		if(pControlSend == NULL)
		{
			return;
		}
		SendClickNotify(pControlSend);
	}
	else if (strMenuName == _T("menu_exit"))
	{
		OnExitFromTray();
	}
}

BOOL CMainDlg::InvokeFromWebFunction(wstring funcname, int dispIdMember, DISPPARAMS *pDispParams, VARIANT *pVarResult, LPVOID)
{
	//DISPPARAMS 参数顺序是反的
	switch (dispIdMember)
	{
	case ID_TEST_INVOKE:
		{
			VARIANT v1 = pDispParams->rgvarg[0];
			//vtFld.bstrVal = _com_util::ConvertStringToBSTR(str.c_str());
			string strOut = _com_util::ConvertBSTRToString(v1.bstrVal);
			AfxMessageBox(strOut.c_str());
		}
		break;
	case ID_SERVICE_REQUEST:	//服务窗口双击请求服务
		{
			VARIANT v1 = pDispParams->rgvarg[0];
			string strSpID = _com_util::ConvertBSTRToString(v1.bstrVal);
			RequestService(strSpID);
		}
		break;
	case ID_ISNEEDSYNCSPSTATUS:
		{
			//根据条件返回是否可以同步服务人员状态 
			bool bRet = IsNeedSyncSpStatus();
			pVarResult->vt = VT_I4;			
			pVarResult->intVal = bRet ? 1 : 0;
		}
		break;
	}
	return TRUE;
}

void CMainDlg::RequestService(string strSpIDOrObject)
{
	if (strSpIDOrObject == "")
	{
		return;
	}
	m_pMainDlg->m_ChatDlg.RequestChat(m_pMainDlg->m_strServiceUrl, strSpIDOrObject.c_str(), m_pMainDlg->m_strEuID);
}

void CMainDlg::RegisterEventCallback(CMyBrowserEventHandler& hander)
{
	hander.RegisterCallBackEvent(L"CppCall", ID_TEST_INVOKE);
	hander.RegisterCallBackEvent(L"RequestService", ID_SERVICE_REQUEST);
	hander.RegisterCallBackEvent(L"IsNeedSyncSpStatus", ID_ISNEEDSYNCSPSTATUS);
}

 void CMainDlg::HideMainWindow()
 {	
	CControlUI* pControlShow = m_PaintManager.FindControl("faxs");
	pControlShow->SetAttribute("selected", "true");
	ShowWindow(false, false);
	m_pbrowserSpList->Navigate2("about:blank");
 }

 LRESULT CMainDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
 {
	 if (!m_bRealExit)
	 {
		HideMainWindow();
		bHandled = TRUE;
		return 0;
	 }
	 return __super::OnClose(uMsg, wParam, lParam, bHandled);
 }

 void CMainDlg::OnExitFromTray()
 {
	 if(!m_pMainDlg->m_ChatDlg.CanExitNow())
	 {
		return;
	 }
	 m_bRealExit = true;
	 m_pMainDlg->m_ChatDlg.SetRealExit();
	 m_pMainDlg->m_ChatDlg.DoCloseEvent();
	 Close();
 }

 LRESULT CMainDlg::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
 {	
	 switch(uMsg)
	 {
	 case WM_TRACKGETPOPUPMENU:
		 {
			 CControlUI * pControl = (CControlUI *)m_PaintManager.FindControl("btnmenu");
			 ::CPoint point;
			 GetCursorPos(&point);
			 ShowMenu(point, pControl);		
		 }
		 break;
	 case WM_TRACKDBLMESSAGE:
		 {
			 ShowWindow();
			 SetForegroundWindow(m_hWnd);
		 }
		 break;
// 	 case WM_TIMER:
// 		 {
// 			 if (wParam == ID_TIMER_CHECKFORGROUND)	//定时切换的计时器到了 
// 			 {
// 				 if (::IsWindowVisible(m_hWnd))
// 				 {
// 					 HWND hwnd = ::GetForegroundWindow();
// 					 long tCurr = ::GetTickCount();
// 					 if (hwnd == m_hWnd)
// 					 {
// 						 m_lLastForGroundTime = tCurr;
// 					 }
// 					 if (tCurr - m_lLastForGroundTime > CHANGE_INTERVAL)
// 					 {
// 						 m_lLastForGroundTime = tCurr;
// 						 CTabLayoutUI* pControlTab = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabs")));
// 						 if (pControlTab->GetCurSel() != 1)
// 						 {
// 							 CControlUI* pControlShow = m_PaintManager.FindControl("faxs");
// 							 pControlShow->SetAttribute("selected", "true");
// 							 m_pbrowserSpList->Navigate2("about:blank");
// 						 }						 
// 					 }
// 				 }				 
// 			 }
// 		 }
// 		 break;
	 }
	 return __super::HandleMessage(uMsg, wParam, lParam);
 }

 void CMainDlg::ShowMenu( ::CPoint &point, CControlUI * pControl )
 {
	 CMenuWnd* pMenu = new CMenuWnd(m_hWnd);

	 point.y -= 150;
	 pMenu->Init(NULL, _T("menu.xml"), (UINT)0, point, pControl);
 }

 void CMainDlg::ShowLastPushMessage(LPCTSTR strTitle, LPCTSTR strMsg)
 {
	 CRichEditUI* pControl = (CRichEditUI *)m_PaintManager.FindControl("pushmsgtitle");
	 pControl->SetText(strTitle);

	 CRichEditUI* pControlEdit = (CRichEditUI *)m_PaintManager.FindControl("pushmsgcontent");
	 pControlEdit->SetText(strMsg);
 }

 bool CMainDlg::IsNeedSyncSpStatus()
 {
	 if (!::IsWindowVisible(m_hWnd))
	 {
		 return false;
	 }
	 if(::IsIconic(m_hWnd))
		 return false;
	 CTabLayoutUI* pControlTab = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabs")));
	 if (pControlTab)
	 {
		 if(pControlTab->GetCurSel() != 0)
		 {
			 return false;
		 }
	 }
	 return true;
 }

 void CMainDlg::ShowDetailMessage(LPCTSTR strUrl)
 {
	ShowMainWindow();
	m_pbrowserViewer->Navigate2(strUrl);
 }

 void CMainDlg::ShowMainWindow()
 {
	 ShowWindow();
	 SetForegroundWindow(m_hWnd);
	 CControlUI* pControlShow = m_PaintManager.FindControl("showleft");
	 SendClickNotify(pControlShow);
 }