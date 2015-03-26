#pragma once
#include "..\DuiLibWrapper\main_frame.hpp"
#include <vector>
class WindowImplBase;
#include "..\DuiLibWrapper\ListEx.h"
#include "AppListControl.h"

typedef struct UserInfo 
{
	CDuiString strip;
	CDuiString struserid;
	CDuiString strpwd;
	CDuiString strprotocol;
	CDuiString strtype;	
};

class CMainDlg : public MainFrame
{
public:	CMainDlg(void);
	~CMainDlg(void);
	virtual void OnClick(TNotifyUI& msg);

	void EndEditOrAddTreeNode();
	void BeginEditOrAddTreeNode( CControlUI* pControl );
	void StartEditItem( CControlUI* pControl );
	void SaveItemData( CControlUI* pControl, bool bCancelEdit = false );
	void StartAddUserInfo( CControlUI* pControl );
	virtual void OnPrepare(TNotifyUI& msg);
	void SetListItemData( CControlUI* pLine, CDuiString struserid, CDuiString strpwd, CDuiString strprotocol, CDuiString strtype);
	virtual void Notify(TNotifyUI& msg);
	CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	AppListUI* m_appListUI;
	CListUIEx* m_userList;
	CTreeNodeUI* m_pNodeRoot;
	vector<UserInfo> m_vecUserList;
	LRESULT OnDropFile(HDROP hDropInfo);  
	CControlUI* GetLineContainer(CControlUI* pControl);
	CControlUI* GetTreeNode(CControlUI* pControl);
	void SetEditListItemData( CControlUI* pLine, CDuiString struserid, CDuiString strpwd, CDuiString strprotocol, CDuiString strtype);
	int GetComboItemIndexFromString(CDuiString strItemText);
};

