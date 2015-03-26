#pragma once
#include "main_frame.hpp"
class WindowImplBase;
#include "MyBrowserEventHandler.h"
class CInfoShowDlg : public MainFrame
{
public:
	CInfoShowDlg(void);
	~CInfoShowDlg(void);
	void LoadSkin();
protected:
	virtual void Notify(TNotifyUI& msg);
	virtual void OnClick(TNotifyUI& msg);
	virtual void OnPrepare(TNotifyUI& msg);
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void OnFinalMessage(HWND hWnd);
public:
	string m_strTitle;
	string m_strUrl;
	CMyBrowserEventHandler m_eventhandlerViewer;
};

