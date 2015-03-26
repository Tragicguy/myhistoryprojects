#include "StdAfx.h"
#include ".\windowfind.h"
HWND g_hWnd = NULL;
CWindowFind::CWindowFind(void)
{
}

CWindowFind::~CWindowFind(void)
{
}
typedef struct _LPPARAMNAME
{
	char szClassName[255];
	char szWindowName[255];
	_LPPARAMNAME()
	{
		memset(szClassName,0,sizeof(szClassName));
		memset(szWindowName,0,sizeof(szWindowName));
	}
}LPPARAMNAME;
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CALLBACK EnumChildProcEx(HWND   hwnd, LPARAM lParam)
{   
	LPPARAMNAME *pParam = (LPPARAMNAME*)lParam;
	char strClassName[255];
	char strWindowName[255];
	memset(strClassName,0,sizeof(strClassName));   
	memset(strWindowName,0,sizeof(strWindowName));  
	if(GetClassName(hwnd,strClassName,sizeof(strClassName)) > 0)   
	{   
		if(0   ==   lstrcmp(strClassName,pParam->szClassName)) 
		{
			::GetWindowText(hwnd,strWindowName,sizeof(strWindowName));
			if (0   ==   lstrcmp(strWindowName,pParam->szWindowName))
			{
				g_hWnd  = hwnd;
				return FALSE;
			}
		}
		else
		{
			EnumChildWindows(hwnd,EnumChildProcEx,lParam);
		}
	}   
	return   TRUE;   
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
HWND CWindowFind::MyFindWindow(CString strClassName,CString strWindowName,HWND hParant)
{
	g_hWnd = NULL;
	LPPARAMNAME lParam;
	strcpy(lParam.szClassName,strClassName);
	strcpy(lParam.szWindowName,strWindowName);
	EnumChildWindows(hParant,EnumChildProcEx,(LPARAM)&lParam);
	return g_hWnd;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CWindowFind::SetEditText(HWND hwnd,CString strValue)
{
	TCHAR  szClassName[256]={0};
	GetClassName(hwnd,(LPTSTR)&szClassName,256);
	if ((CString)szClassName == "ThunderRT6TextBox")
	{			
		CopyStringToClipboard(NULL,strValue);
		CEdit *pCob = (CEdit *)CWnd::FromHandle(hwnd);
		if (pCob)
		{
			pCob->SetSel(0, -1);
			pCob->Clear();
			::SendMessage(hwnd,WM_PASTE,0,0);				
		}	
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CWindowFind::PostBtnClick(HWND hwnd)
{
	::PostMessage(hwnd,WM_LBUTTONDOWN,0,MAKELONG(1,1));
	Sleep(5);
	::PostMessage(hwnd,WM_LBUTTONUP,0,MAKELONG(1,1));
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CWindowFind::SetTestChildEditText(HWND hwnd,int iIndex)
{
	HWND hwndChind = NULL;
	CString strTemp = "";
	TCHAR  szClassName[256]={0};
	int i = 1;
	hwndChind = GetWindow(hwnd,GW_CHILD);
	CWnd *pWnd = CWnd::FromHandle(hwndChind);
	while (pWnd)
	{	
		memset(szClassName,0,sizeof(szClassName));
		hwndChind = pWnd->GetSafeHwnd();
		GetClassName(hwndChind,(LPTSTR)&szClassName,256);
		if ((CString)szClassName == "Edit")
		{
			if (iIndex == 0)
			{
				strTemp.Format("test%d",i);
			}
			else
			{
				if (i == iIndex)
				{
					strTemp.Format("te11111111st%d",i);
				}
			}
			CopyStringToClipboard(NULL,strTemp);
			CEdit *pCob = (CEdit *)CWnd::FromHandle(hwndChind);
			if (pCob)
			{
				//先删除原有号码后向号码输入框发送复制消息				
				if (iIndex == 0)
				{
					pCob->SetSel(0, -1);
					pCob->Clear();
					::PostMessage(hwndChind,WM_PASTE,0,0);
				}
				else
				{
					if (i == iIndex)
					{
						pCob->SetSel(0, -1);
						pCob->Clear();
						::PostMessage(hwndChind,WM_PASTE,0,0);
					}
				}				
			}	
		}		
		pWnd = pWnd->GetNextWindow(GW_HWNDNEXT);
		i++;
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
HWND CWindowFind::GetChildWindow(HWND hwnd,int iIndex)
{
	HWND hwndChind = NULL;
	CString strTemp = "";
	int i = 1;
	hwndChind = GetWindow(hwnd,GW_CHILD);
	CWnd *pWnd = CWnd::FromHandle(hwndChind);
	while (pWnd)
	{	
		hwndChind = pWnd->GetSafeHwnd();
		if (i == iIndex)
		{
			return hwndChind;
		}
		pWnd = pWnd->GetNextWindow(GW_HWNDNEXT);
		i++;
	}
	return hwndChind;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CWindowFind::CopyStringToClipboard(HWND hWnd, LPCTSTR lpszText)
{
	// 将szText中的内容复制到剪贴板
	EmptyClipboard();
	HGLOBAL hglbCopy;
	LPTSTR  lptstrCopy;
	int nlen = strlen(lpszText);
	if (nlen == 0)
	{
		return FALSE;
	}
	if (!::OpenClipboard(hWnd))
		return FALSE;

	hglbCopy = GlobalAlloc(GMEM_DDESHARE, 
		(nlen + 1) * sizeof(char)); 

	if (hglbCopy == NULL) 
	{ 
		CloseClipboard(); 
		return FALSE; 
	}
	lptstrCopy = (LPTSTR)GlobalLock(hglbCopy); 
	memcpy(lptstrCopy, lpszText, nlen);
	lptstrCopy[nlen] = (TCHAR)0;  //null character
	GlobalUnlock(lptstrCopy);
	SetClipboardData(CF_TEXT, hglbCopy);
	CloseClipboard();
	return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CWindowFind::GetContrlClassName(HWND hwnd)
{
	CString strRet = "";
	TCHAR  szClassName[256]={0};	
	GetClassName(hwnd,(LPTSTR)&szClassName,256);
	strRet.Format("%s",szClassName);
	return strRet;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CWindowFind::GetContrlWindowText(HWND hwnd)
{
	CString strRet = "";
	TCHAR  szWindName[256]={0};	
	::GetWindowText(hwnd,(LPTSTR)&szWindName,256);
	strRet.Format("%s",szWindName);
	return strRet;
}