#include "StdAfx.h"
#include "RichEditEx.h"

CRichEditEx::CRichEditEx(void)
{
}

CRichEditEx::~CRichEditEx(void)
{
}
LRESULT CRichEditEx::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_KEYDOWN && wParam == VK_RETURN)
	{
		AfxMessageBox("222222");
		::PostMessage(m_hParentWnd,WM_COMMAND,m_uCommandID,0);
		return 0L;
	}
	return CRichEditCtrl::WindowProc(message, wParam, lParam);
}
