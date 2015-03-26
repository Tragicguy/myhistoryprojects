//
// main.cpp
// ~~~~~~~~
//
// Copyright (c) 2011 achellies (achellies at 163 dot com)
//
// This code may be used in compiled form in any way you desire. This
// source file may be redistributed by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
// Beware of bugs.
//

#include "stdafx.h"
#include "LoginDlg.h"
#include "MainDlg.h"
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;
	CLoginDlg *pDlg = new CLoginDlg();
	pDlg->SetSkinFile("LoginDlg.xml");	
	pDlg->SetSubSkinFolder("res");
	pDlg->ShowMaxMizeBox(false);
	pDlg->Create(NULL, _T("µÇÂ¼´°¿Ú"), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 600, 800);
	pDlg->CenterWindow();
	::ShowWindow(*pDlg, SW_SHOW);
	CPaintManagerUI::MessageLoop();

	CMainDlg * pMainDlg = new CMainDlg();
	pMainDlg->SetSkinFile("MainWindow.xml");
	pMainDlg->SetSubSkinFolder("res");
	pMainDlg->ShowMaxMizeBox(false);
	pMainDlg->SetBkImageMaxSize(3);
	pMainDlg->Create(NULL, _T("Ö÷´°¿Ú"), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 600, 800);
	pMainDlg->CenterWindow();
	::ShowWindow(*pMainDlg, SW_SHOW);
	CPaintManagerUI::MessageLoop();
	::CoUninitialize();
	return 0;
}