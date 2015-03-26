
// HookWininetDemo.h : main header file for the HookWininetDemo application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CHookWininetDemoApp:
// See HookWininetDemo.cpp for the implementation of this class
//

class CHookWininetDemoApp : public CWinApp
{
public:
	CHookWininetDemoApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CHookWininetDemoApp theApp;
