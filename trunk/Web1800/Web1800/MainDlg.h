#pragma once
#include "main_frame.hpp"
#include <vector>
#include "UIFriends.hpp"
class WindowImplBase;

class CMainDlg : public MainFrame
{
public:
	CMainDlg(void);
	~CMainDlg(void);
	virtual void OnClick(TNotifyUI& msg);
	virtual void OnPrepare(TNotifyUI& msg);
	virtual void Notify(TNotifyUI& msg);
	CControlUI* CreateControl(LPCTSTR pstrClass);
	void UpdateFriendsList();
	
	FriendListItemInfo myself_info_;
	std::vector<FriendListItemInfo> friends_;
};

