#pragma once

class CRichEditEx :
	public CRichEditCtrl
{
public:
	CRichEditEx(void);
	~CRichEditEx(void);
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void SetHwnd(HWND hwnd,UINT uCommandID){m_hParentWnd = hwnd; m_uCommandID = uCommandID;}
	HWND m_hParentWnd;
	UINT m_uCommandID;
};
