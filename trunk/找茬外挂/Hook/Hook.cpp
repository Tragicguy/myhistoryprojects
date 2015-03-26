#include <windows.h>

#include "winuser.h"

#define	WM_GETDSTDIFFRENT	WM_USER+11					//获取不同区域消息

#define	WM_USER_CLOSEWINDOW	WM_USER+12	

typedef struct tagKBDLLHOOKSTRUCT {

    DWORD   vkCode;

    DWORD   scanCode;

    DWORD   flags;

    DWORD   time;

    DWORD   dwExtraInfo;

} KBDLLHOOKSTRUCT, FAR *LPKBDLLHOOKSTRUCT, *PKBDLLHOOKSTRUCT;


#pragma data_seg("MySec")
HWND g_hHelpWnd = NULL;
HHOOK hLowKeyBoardHook;
#pragma data_seg()


LRESULT CALLBACK LowLevelKeyboardProc(
  int nCode,     // hook code
  WPARAM wParam, // message identifier
  LPARAM lParam  // message data
)
{  
  if(nCode == HC_ACTION)
  {
	  
	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT) lParam;
    if(p->vkCode == VK_F1)
	{
			::PostMessage(g_hHelpWnd,WM_GETDSTDIFFRENT,0,0);
			return 1;
	}
		
	if(p->vkCode == VK_F4)
	{
			::PostMessage(g_hHelpWnd,WM_USER_CLOSEWINDOW,0,0);
			return 1;
	}
  }
  return  CallNextHookEx(hLowKeyBoardHook,nCode,wParam,lParam);
}

HHOOK SetHook(HWND m_hwndHelper)
{
	g_hHelpWnd = m_hwndHelper;
	hLowKeyBoardHook=SetWindowsHookEx(13,LowLevelKeyboardProc,GetModuleHandle("Hook"),0);
	if (hLowKeyBoardHook)
	{
			return hLowKeyBoardHook;
	}
	return NULL;
	
}