#pragma once
#include "main_frame.hpp"
class WindowImplBase;
class CLoginDlg : public MainFrame
{
public:
	CLoginDlg(void);
	~CLoginDlg(void);
protected:
	virtual void Notify(TNotifyUI& msg);
	virtual void OnClick(TNotifyUI& msg);
};

