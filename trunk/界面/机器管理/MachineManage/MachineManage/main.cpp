
#include "stdafx.h"
#include "MainDlg.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{	
	CPaintManagerUI::SetInstance(hInstance);
	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;
	
	CMainDlg * pMainDlg = new CMainDlg();
	pMainDlg->SetSkinFile("MainWindow.xml");
	pMainDlg->SetSubSkinFolder("res_mm");
	pMainDlg->ShowMaxMizeBox(false);
	pMainDlg->SetBkImageMaxSize(3);
	/*
	UI_WNDSTYLE_DIALOG
		UI_WNDSTYLE_FRAME
		*/
	pMainDlg->Create(NULL, _T("Ö÷´°¿Ú"), UI_WNDSTYLE_DIALOG, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 600, 800);
	pMainDlg->CenterWindow();
	::ShowWindow(*pMainDlg, SW_SHOW);
	CPaintManagerUI::MessageLoop();
	::CoUninitialize();
	
    return 0;
}
