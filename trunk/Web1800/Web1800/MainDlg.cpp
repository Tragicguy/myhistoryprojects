#include "StdAfx.h"
#include "MainDlg.h"
#include <windows.h>
#if !defined(UNDER_CE)
#include <shellapi.h>
#endif

CMainDlg::CMainDlg(void)
{
}


CMainDlg::~CMainDlg(void)
{
}


void CMainDlg::OnClick(TNotifyUI& msg)
{
	CControlUI* pControl = msg.pSender;
	 if(pControl->IsEqual("btncustomer"))
	{
		::MessageBox(m_hWnd, "aaaaaaaaaa", NULL, 0);
		return;
	}
	 if (pControl->IsEqual("showleft"))
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
	 if (pControl->IsEqual("hideleft"))
	 {
		CControlUI* left_main_pannel = m_PaintManager.FindControl("servicepanel");
		 if ((left_main_pannel != NULL))
		 {
			 ::SetWindowPos(m_hWnd, NULL, 0, 0, 230, 680, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
			 left_main_pannel->SetVisible(false);
			 CControlUI* pControlHideLeft = m_PaintManager.FindControl("showleft");
			 if(pControlHideLeft) pControlHideLeft->SetVisible(true);
			 pControl->SetVisible(false);
		 }
	 }
	__super::OnClick(msg);
}

void CMainDlg::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, _T("itemclick")) == 0)
	{
		CFriendsUI* pFriendsList = static_cast<CFriendsUI*>(m_PaintManager.FindControl("friends"));
		if ((pFriendsList != NULL) &&  pFriendsList->GetItemIndex(msg.pSender) != -1)
		{
			if (_tcsicmp(msg.pSender->GetClass(), _T("ListContainerElementUI")) == 0)
			{
				Node* node = (Node*)msg.pSender->GetTag();

				if (pFriendsList->CanExpand(node))
				{
					pFriendsList->SetChildVisible(node, !node->data().child_visible_);
				}
			}
		}
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
		::SetWindowPos(m_hWnd, NULL, 0, 0, 230, 680, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
		CControlUI* pControl = m_PaintManager.FindControl("showleft");
		if(pControl) pControl->SetVisible(true);
		pControl = m_PaintManager.FindControl("hideleft");
		if(pControl) pControl->SetVisible(false);
	}
	UpdateFriendsList();
}
CControlUI* CMainDlg::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("FriendList")) == 0)
	{
		return new CFriendsUI(m_PaintManager);
	}
	return NULL;
}


void CMainDlg::UpdateFriendsList()
{
	CFriendsUI* pFriendsList = static_cast<CFriendsUI*>(m_PaintManager.FindControl("friends"));
	if (pFriendsList != NULL)
	{
		if (!friends_.empty())
			friends_.clear();
		if (pFriendsList->GetCount() > 0)
			pFriendsList->RemoveAll();

		FriendListItemInfo item;

		item.id = _T("0");
		item.folder = true;
		item.empty = false;
		item.nick_name = _T("售前咨询");

		Node* root_parent = pFriendsList->AddNode(item, NULL);
		friends_.push_back(item);

		item.id = _T("1");
		item.folder = false;		
		item.logo = _T("supporterlogo.png");
		item.nick_name = _T("李志宾");
		item.description = _T("zbl@web1800.com");

		myself_info_ = item;

		pFriendsList->AddNode(item, root_parent);
		friends_.push_back(item);

		item.id = _T("2");
		item.folder = false;
		item.logo = _T("default.png");
		item.nick_name = _T("陈伟");
		item.description = _T("weic@web1800.com");
		pFriendsList->AddNode(item, root_parent);
		friends_.push_back(item);
				
		item.id = _T("3");
		item.folder = true;
		item.empty = false;
		item.nick_name = _T("售后支持");
		Node* root_parent2 = pFriendsList->AddNode(item, NULL);
		friends_.push_back(item);

		item.id = _T("3");
		item.folder = false;
		item.logo = _T("default.png");
		item.nick_name = _T("张莉");
		item.description = _T("lz@web1800.com");
		pFriendsList->AddNode(item, root_parent2);

		item.id = _T("4");
		item.folder = true;
		item.empty = false;
		item.nick_name = _T("技术开发");
		Node* root_parent3 = pFriendsList->AddNode(item, NULL);
		friends_.push_back(item);

		item.id = _T("4");
		item.folder = false;
		item.logo = _T("supporterlogo.png");
		item.nick_name = _T("邵军");
		item.description = _T("zjs@web1800.com");
		pFriendsList->AddNode(item, root_parent3);

		item.id = _T("4");
		item.folder = false;
		item.logo = _T("default.png");
		item.nick_name = _T("游玉兵");
		item.description = _T("yby@web1800.com");
		pFriendsList->AddNode(item, root_parent3);

		item.id = _T("5");
		item.folder = true;
		item.empty = false;
		item.nick_name = _T("测试组");
		Node* root_parent4 = pFriendsList->AddNode(item, NULL);
		friends_.push_back(item);

		item.id = _T("4");
		item.folder = false;
		item.logo = _T("default.png");
		item.nick_name = _T("票飞");
		item.description = _T("fs@web1800.com");
		pFriendsList->AddNode(item, root_parent4);

		item.id = _T("5");
		item.folder = true;
		item.empty = false;
		item.nick_name = _T("市场部");
		Node* root_parent5 = pFriendsList->AddNode(item, NULL);
		friends_.push_back(item);

		item.id = _T("4");
		item.folder = false;
		item.logo = _T("supporterlogo.png");
		item.nick_name = _T("张弢");
		item.description = _T("taoz@web1800.com");
		pFriendsList->AddNode(item, root_parent5);
	}
}
