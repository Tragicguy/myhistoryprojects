#pragma once
#include "stdafx.h"
class CWindowFind
{
public:
	CWindowFind(void);
	~CWindowFind(void);
	
   	HWND MyFindWindow(CString strClassName,CString strWindowName,HWND hParant=NULL);
	BOOL CopyStringToClipboard(HWND hWnd, LPCTSTR lpszText);
	void SetTestChildEditText(HWND hwnd,int iIndex = 0);
	HWND GetChildWindow(HWND hwnd,int iIndex);
	void SetEditText(HWND hwnd,CString strValue);
	void PostBtnClick(HWND hwnd);
	CString GetContrlClassName(HWND hwnd);
	CString GetContrlWindowText(HWND hwnd);
};
