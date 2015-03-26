#include "StdAfx.h"
#include "MainDlg.h"
#include <windows.h>
#if !defined(UNDER_CE)
#include <shellapi.h>
#endif
#include "..\DuiLibWrapper\ListEx.h"

#include "AppListControl.h"
CMainDlg::CMainDlg(void)
{
}


CMainDlg::~CMainDlg(void)
{
}


void CMainDlg::OnClick(TNotifyUI& msg)
{
	CControlUI* pControl = msg.pSender;
	if(pControl->IsEqual("removeitem"))
	{
		m_appListUI->Remove(pControl->GetParent()->GetParent());
		int nnn = 0;
		return;
	}
	else if(pControl->IsEqual("iconshow"))
	{
		//应用程序的复选框区域外点击的选中 
		CCheckBoxUI* pCheck = (CCheckBoxUI*)m_PaintManager.FindSubControlByName(pControl->GetParent(), "isuse");
		if (pCheck)
		{
			pCheck->SetCheck(!pCheck->GetCheck());
		}
		return;
	}
	else if (pControl->IsEqual("savetreenode"))
	{
		//保存编辑节点名称或添加叶子节点
		EndEditOrAddTreeNode();
		return;
	}
	else if(pControl->IsEqual("beginadd") || pControl->IsEqual("beginedit"))
	{
		//显示编辑节点的输入框 
		BeginEditOrAddTreeNode(pControl);
		return;
	}
	else if (pControl->IsEqual("adddirectory"))
	{
		//添加目录
		CDuiString strDirectryName = GetDlgItemText("editdirectory");
		CTreeViewUI* pTree = static_cast<CTreeViewUI*>(m_PaintManager.FindControl(_T("tree")));
		
		CTreeNodeUI* pNodeAdd = new CTreeNodeUI;
		pNodeAdd->SetAttribute(_T("folderattr"),_T("padding=\"0,1,0,0\" width=\"36\" height=\"16\" normalimage=\"file='treeview_b.png' source='0,0,36,16'\" hotimage=\"file='treeview_b.png' source='36,0,72,16'\" selectedimage=\"file='treeview_a.png' source='0,0,36,16' selectedhotimage=\"file='treeview_a.png' source='36,0,72,16'\""));
		pNodeAdd->SetItemText(strDirectryName);
		pNodeAdd->SetExpand(true);
		m_pNodeRoot->AddChildNode(pNodeAdd);
		
	}
	else if (pControl->IsEqual("selall"))
	{
		int nCount = m_userList->GetCount();
		for (int i = nCount - 1; i >=0 ; i--)
		{
			CControlUI *pLine = m_userList->GetItemAt(i);
			CCheckBoxUI* pCheckLine = (CCheckBoxUI*)m_PaintManager.FindSubControlByName(pLine, "selme");
			if (pCheckLine)
			{
				//此处比较奇怪，要取全选的反才是正确的值 
				pCheckLine->SetCheck(!((CCheckBoxUI*)pControl)->GetCheck());
			}
		}
	}
	else if (pControl->IsEqual("deletesel"))
	{
		//遍历列表进行删除 
		int nCount = m_userList->GetCount();
		for (int i = nCount - 1; i >=0 ; i--)
		{
			CControlUI *pLine = m_userList->GetItemAt(i);
			CCheckBoxUI* pCheckLine = (CCheckBoxUI*)m_PaintManager.FindSubControlByName(pLine, "selme");
			if (pCheckLine && pCheckLine->GetCheck())
			{
				m_userList->RemoveAt(i);
			}
		}
	}
	else if (pControl->IsEqual("edititem"))
	{
		StartEditItem(pControl);
		return;
	}
	else if (pControl->IsEqual("deleteitem") || pControl->IsEqual("canceladd"))
	{
		CListContainerElementUI *pElement = (CListContainerElementUI *)GetLineContainer(pControl);
		if (pElement->GetUserData() == "fromedititem")
		{
			//是在编辑某一项，点击取消，则还原 
			SaveItemData(pControl, true);
			return;
		}
		int nIndex = m_userList->GetItemIndex(pElement);		
		m_userList->RemoveAt(nIndex);
		return;
	}
	else if (pControl->IsEqual("additem"))
	{
		StartAddUserInfo(pControl);
		return;
	}
	else if (pControl->IsEqual("saveitem"))
	{
		SaveItemData(pControl);
		return;
	}
	else if (pControl->IsEqual("hostmachine"))
	{
		::MessageBox(m_hWnd, "主机按钮被点击，需要处理相关事项。", "提示", 0);
		return;
	}
	else if (pControl->IsEqual("database"))
	{
		::MessageBox(m_hWnd, "数据库按钮被点击，需要处理相关事项。", "提示", 0);
		return;
	}
	else if (pControl->IsEqual("netmachine"))
	{
		::MessageBox(m_hWnd, "网络设备按钮被点击，需要处理相关事项。", "提示", 0);
		return;
	}
	__super::OnClick(msg);
}

CControlUI* CMainDlg::GetTreeNode(CControlUI* pControl)
{
	CControlUI *pNode = NULL;
	while (pControl != NULL && pNode == NULL)
	{
		pNode = (CControlUI *)pControl->GetInterface(DUI_CTR_TREENODE);
		if (pNode != NULL)
		{
			break;
		}
		pControl = pControl->GetParent();
	}
	return pNode;
}

void CMainDlg::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, _T("TreeNodeMove")) == 0 )
	{
		
		CTreeNodeUI* oldParent = (CTreeNodeUI*)msg.wParam;
		CTreeNodeUI* pNodeMoved =  (CTreeNodeUI*)msg.lParam;
		//通过pNodeMoved可以获取其新的父节点 
		CTreeNodeUI* pNewParent = pNodeMoved->GetParentNode();
		//此处根据pNodeMoved是否是叶子节点，决定如何处理数据的存储调整 
		if (pNodeMoved->IsLeaf())
		{
			CDuiString strShow = "";
			strShow.Format("叶子节点 %s 从 %s 移动到了 %s 下", pNodeMoved->GetItemText().GetData(), oldParent->GetItemText().GetData(), pNewParent->GetItemText().GetData());
			::MessageBox(NULL, strShow, "提示", 0);
		}
		
		//树节点移动完成 
		return;
	}
	//在此处响应列表的双击操作，此处树节点的双击与LIST的点击都在这里 
	if (_tcsicmp(msg.sType, _T("itemactivate")) == 0 )
	{
		CControlUI* pNode = GetTreeNode(msg.pSender);
		if (pNode != NULL)
		{
			CTreeNodeUI* pTreeNode = (CTreeNodeUI*)pNode;
			//在此处处理树节点的双击
			if (!pTreeNode->IsLeaf())	//如果不是叶子节点，则需要控制右侧的列表显示  
			{
				::MessageBox(NULL, pTreeNode->GetItemText().GetData(), "提示", 0);
			}
			return;
		}
		CControlUI* pEditContrl = m_PaintManager.FindSubControlByName(msg.pSender, "edituserid");
		if (pEditContrl)
		{
			return;
		}
		CControlUI* pLabel = m_PaintManager.FindSubControlByName(msg.pSender, "userid");
		if (!pLabel)
		{
			return;
		}
		CDuiString struserid = m_PaintManager.FindSubControlByName(msg.pSender, "userid")->GetText();
		CDuiString strpwd = m_PaintManager.FindSubControlByName(msg.pSender, "pwd")->GetText();
		CDuiString strprotocol = m_PaintManager.FindSubControlByName(msg.pSender, "protocol")->GetText();
		CDuiString strtype = m_PaintManager.FindSubControlByName(msg.pSender, "type")->GetText();
		CDuiString strShow = "";
			
		CDuiString strSelectedApp = " ";
		int nAppCount = m_appListUI->GetCount();
		for (int i=0; i< nAppCount; i++)
		{
		    CCheckBoxUI* pCheck = (CCheckBoxUI*)m_PaintManager.FindSubControlByName(m_appListUI->GetItemAt(i), "isuse");
			if (pCheck->GetCheck())
			{
				strSelectedApp += pCheck->GetText() + "\r\n ";
			}			
		}
		strShow.Format("选中的信息如下：\r\n账号：%s\r\n密码：%s\r\n协议：%s\r\n类型：%s\r\n选中的应用程序：\r\n%s", 
			struserid.GetData(), strpwd.GetData(), strprotocol.GetData(), strtype.GetData(), strSelectedApp.GetData());
		MessageBox(m_hWnd, strShow, "提示", 0);	
	}
	
	__super::Notify(msg);
}
void CMainDlg::OnPrepare(TNotifyUI& msg)
{
	__super::OnPrepare(msg);

	::DragAcceptFiles(this->m_hWnd, true);  

	//构造测试数据，添加到列表中 
	for (int i = 0; i < 8; i++)
	{
		UserInfo info;
		info.strip.Format("192.168.2.11%d", i);
		info.struserid.Format("userid%d", i);
		info.strpwd = "123";
		info.strprotocol = i%2==0 ? "HTTP" : "TCP";
		info.strtype = i%3==0 ? "企业应用" : "个人产品";
		m_vecUserList.push_back(info);
	}
	m_userList = static_cast<CListUIEx*>(m_PaintManager.FindControl(_T("listex")));
		
	for (int i = 0; i < m_vecUserList.size(); ++i)
	{
		CDialogBuilder builder;
		CListContainerElementUI* pLine = (CListContainerElementUI*)(builder.Create(_T("single_list_item.xml"), (UINT)0, this));

		if( pLine != NULL ) 
		{   
			SetListItemData(pLine, m_vecUserList[i].struserid, m_vecUserList[i].strpwd, m_vecUserList[i].strprotocol, m_vecUserList[i].strtype);
			m_userList->InsertItem(m_userList->GetCount(), 21, pLine);
		}
	}
	

	//测试代码，将自身程序的图标旋到程序中，显示出来
	char szPath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szPath, MAX_PATH);
	m_appListUI = (AppListUI*)m_PaintManager.FindControl(_T("applist"));
	m_appListUI->AddApplication(szPath);

	//初始化树的显示 
	CTreeViewUI* pTree = static_cast<CTreeViewUI*>(m_PaintManager.FindControl(_T("tree")));
	CControlUI* pControl = m_PaintManager.FindControl(_T("drag"));
	pTree->m_pDragingCtrl = pControl;

	CTreeNodeUI* pNodeRoot = new CTreeNodeUI;
	pNodeRoot->SetAttribute(_T("folderattr"),_T("padding=\"0,1,0,0\" width=\"36\" height=\"16\" normalimage=\"file='treeview_b.png' source='0,0,36,16'\" hotimage=\"file='treeview_b.png' source='36,0,72,16'\" selectedimage=\"file='treeview_a.png' source='0,0,36,16' selectedhotimage=\"file='treeview_a.png' source='36,0,72,16'\""));
	pTree->Add(pNodeRoot);
	CDuiString strNode = "";
	strNode.Format("主机列表");
	pNodeRoot->SetItemText(strNode);
	pNodeRoot->SetExpand(true);
	m_pNodeRoot = pNodeRoot;
	for (int i = 1; i < 7; i++)
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
CControlUI* CMainDlg::CreateControl(LPCTSTR pstrClass)
{
	if( _tcscmp(pstrClass, _T("AppList")) == 0 ) 
		return new AppListUI;
	return NULL;
}

void CMainDlg::SetListItemData( CControlUI* pLine, CDuiString struserid, CDuiString strpwd, CDuiString strprotocol, CDuiString strtype) 
{
	m_PaintManager.FindSubControlByName(pLine, "userid")->SetText(struserid);;
	m_PaintManager.FindSubControlByName(pLine, "pwd")->SetText(strpwd);;
	m_PaintManager.FindSubControlByName(pLine, "protocol")->SetText(strprotocol);;
	m_PaintManager.FindSubControlByName(pLine, "type")->SetText(strtype);
}

int CMainDlg::GetComboItemIndexFromString(CDuiString strItemText)
{
	if (strItemText == "HTTP")
	{
		return 0;
	}
	if (strItemText == "TCP")
	{
		return 1;
	}
	if (strItemText == "企业应用")
	{
		return 0;
	}
	if (strItemText == "个人产品")
	{
		return 1;
	}
}

void CMainDlg::SetEditListItemData( CControlUI* pLine, CDuiString struserid, CDuiString strpwd, CDuiString strprotocol, CDuiString strtype) 
{
	CControlUI * pControl = m_PaintManager.FindSubControlByName(pLine, "edituserid");
	pControl->SetText(struserid);
	pControl->SetUserData(struserid);

	pControl = m_PaintManager.FindSubControlByName(pLine, "editpwd");
	pControl->SetText(strpwd);
	pControl->SetUserData(strpwd);

    CComboUI* pCombo = (CComboUI*)m_PaintManager.FindSubControlByName(pLine, "editprotocol");
	pCombo->SelectItem(GetComboItemIndexFromString(strprotocol));
	pCombo->SetUserData(strprotocol);

	pCombo = (CComboUI*)m_PaintManager.FindSubControlByName(pLine, "edittype");
	pCombo->SelectItem(GetComboItemIndexFromString(strtype));
	pCombo->SetUserData(strtype);
}


LRESULT CMainDlg::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	switch(uMsg)
	{
		
		case WM_DROPFILES:
			OnDropFile((HDROP)wParam);
			break;

	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CMainDlg::OnDropFile(HDROP hDropInfo)
{
	TCHAR szFileName[MAX_PATH];  
	DragQueryFile(hDropInfo, 0, szFileName, MAX_PATH);  
	DragFinish(hDropInfo);  
	m_appListUI->AddApplication(szFileName);
	return 0L;
}

void CMainDlg::StartAddUserInfo( CControlUI* pControl )
{
	int i = m_userList->GetCount();
	CDialogBuilder builder;
	CListContainerElementUI* pLine = (CListContainerElementUI*)(builder.Create(_T("single_list_item_add.xml"),(UINT)0, this));
	if( pLine != NULL ) 
	{
		m_userList->InsertItem(i, 23, pLine);
	}
}

CControlUI* CMainDlg::GetLineContainer(CControlUI* pControl)
{
	CControlUI *pLineUI = NULL;
	while (pControl != NULL && pLineUI == NULL)
	{
		pLineUI = (CControlUI *)pControl->GetInterface(DUI_CTR_LISTCONTAINERELEMENT);
		if (pLineUI != NULL)
		{
			break;
		}
		pControl = pControl->GetParent();
	}
	return pLineUI;
}

void CMainDlg::SaveItemData( CControlUI* pControl, bool bCancelEdit )
{
	CControlUI *pParentCtrl = GetLineContainer(pControl);
	if (pParentCtrl == NULL)
	{
		return;
	}
	
	CDuiString struserid = "";
	CDuiString strpwd = "";
	CDuiString strprotocol = "";
	CDuiString strtype = "";
	if (bCancelEdit)
	{
		struserid = m_PaintManager.FindSubControlByName(pParentCtrl, "edituserid")->GetUserData();;
		strpwd = m_PaintManager.FindSubControlByName(pParentCtrl, "editpwd")->GetUserData();;
		strprotocol = m_PaintManager.FindSubControlByName(pParentCtrl, "editprotocol")->GetUserData();;
		strtype = m_PaintManager.FindSubControlByName(pParentCtrl, "edittype")->GetUserData();
	}
	else
	{
		//此处获取要添加的信息
		struserid = m_PaintManager.FindSubControlByName(pParentCtrl, "edituserid")->GetText();;
		strpwd = m_PaintManager.FindSubControlByName(pParentCtrl, "editpwd")->GetText();;
		strprotocol = m_PaintManager.FindSubControlByName(pParentCtrl, "editprotocol")->GetText();;
		strtype = m_PaintManager.FindSubControlByName(pParentCtrl, "edittype")->GetText();
		if (struserid == "" || strprotocol == "" || strpwd == "" || strtype == "")
		{
			::MessageBox(m_hWnd, "请完整填入相关信息", "警告", 0);
			return;
		}
	}
	
	//此处应该要做存储数据库等操作
	int nIndex = m_userList->GetItemIndex(pParentCtrl);	
	m_userList->RemoveAt(nIndex);
	CDialogBuilder builder;
	CListContainerElementUI* pLine = (CListContainerElementUI*)(builder.Create(_T("single_list_item.xml"), (UINT)0, this));
	if( pLine != NULL ) 
	{
		SetListItemData(pLine, struserid, strpwd, strprotocol, strtype);
		m_userList->InsertItem(nIndex, 21, pLine);
	}	
}

void CMainDlg::StartEditItem( CControlUI* pControl )
{
	CControlUI *pParentCtrl = GetLineContainer(pControl);
	if (pParentCtrl == NULL)
	{
		return;
	}

	CDuiString struserid = m_PaintManager.FindSubControlByName(pParentCtrl, "userid")->GetText();
	CDuiString strpwd = m_PaintManager.FindSubControlByName(pParentCtrl, "pwd")->GetText();
	CDuiString strprotocol = m_PaintManager.FindSubControlByName(pParentCtrl, "protocol")->GetText();
	CDuiString strtype = m_PaintManager.FindSubControlByName(pParentCtrl, "type")->GetText();
	//获取到相关的数据  
	int nIndex = m_userList->GetItemIndex(pParentCtrl);	
	m_userList->RemoveAt(nIndex);
	CDialogBuilder builder;
	CListContainerElementUI* pLine = (CListContainerElementUI*)(builder.Create(_T("single_list_item_add.xml"), (UINT)0, this));
	if( pLine != NULL ) 
	{
		pLine->SetUserData("fromedititem");
		SetEditListItemData(pLine, struserid, strpwd, strprotocol, strtype);
		m_userList->InsertItem(nIndex, 23, pLine);
	}
}

void CMainDlg::BeginEditOrAddTreeNode( CControlUI* pControl )
{
	CTreeViewUI* pTree = static_cast<CTreeViewUI*>(m_PaintManager.FindControl(_T("tree")));
	RECT rt = pTree->m_pNodeModifyOrAdd->GetPos();
	RECT rtItem = pTree->m_pNodeModifyOrAdd->GetItemButton()->GetPos();

	//计算输入框应该显示的位置，如果认为不合适，可以调整此处 
	RECT rtPos;
	rtPos.left = rtItem.left;
	rtPos.top = rt.top;
	rtPos.bottom = rt.bottom;
	rtPos.right = rtPos.left + 150;
	CControlUI* pControlModify = m_PaintManager.FindControl("modifyoradd");

	//通过UserData来记录是编辑还是添加事件，以便后续处理
	if (pControl->IsEqual("beginedit"))
	{
		pControlModify->SetUserData("edit");
		m_PaintManager.FindControl(_T("edittext"))->SetText(pTree->m_pNodeModifyOrAdd->GetItemText());
		m_PaintManager.FindControl("savetreenode")->SetText("保存");
	}
	else 
	{
		m_PaintManager.FindControl(_T("edittext"))->SetText("");
		pControlModify->SetUserData("add");
		m_PaintManager.FindControl("savetreenode")->SetText("添加");
	}
	pControlModify->SetVisible(true);
	pControlModify->SetPos(rtPos);

	pControlModify = m_PaintManager.FindControl("treemenu");
	pControlModify->SetVisible(false);
}

void CMainDlg::EndEditOrAddTreeNode()
{
	CTreeViewUI* pTree = static_cast<CTreeViewUI*>(m_PaintManager.FindControl(_T("tree")));

	CControlUI* pControlModify = m_PaintManager.FindControl("modifyoradd");
	//通过UserData来确定是编辑还是添加事件
	CDuiString strUserData = pControlModify->GetUserData();
	CDuiString strText = m_PaintManager.FindControl(_T("edittext"))->GetText();
	if (strUserData == "edit")	//保存名称修改
	{
		pTree->m_pNodeModifyOrAdd->SetItemText(strText);
	}
	else if (strUserData == "add")	//确认添加叶子节点 
	{
		CTreeNodeUI* pNodeAdd = new CTreeNodeUI(true);
		pNodeAdd->SetItemText(strText);
		pTree->m_pNodeModifyOrAdd->AddChildNode(pNodeAdd);
	}
	pControlModify->SetVisible(false);
}