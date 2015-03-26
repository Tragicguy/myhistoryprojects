#include <windows.h>

#include "winuser.h"

#define	WM_SETTIMER3	WM_USER+2
#define	WM_SETTIMER4	WM_USER+3
#define	WM_KILLTIMER3	WM_USER+4
#define	WM_KILLTIMER4	WM_USER+5
#define	WM_KILLTHREAD	WM_USER+547
typedef struct tagKBDLLHOOKSTRUCT {

    DWORD   vkCode;

    DWORD   scanCode;

    DWORD   flags;

    DWORD   time;

    DWORD   dwExtraInfo;

} KBDLLHOOKSTRUCT, FAR *LPKBDLLHOOKSTRUCT, *PKBDLLHOOKSTRUCT;

struct HookStruct
{
  HHOOK hKeyBoardHook;
  HHOOK hLowKeyBoardHook;
};


#pragma data_seg("MySec")
HWND g_hWnd=NULL;
HWND g_hHelpWnd = NULL;
UINT m_vk7instead = 1;
UINT m_vk4instead = 1;
UINT m_vk1instead = 1;
UINT m_vk8instead = 1;
UINT m_vk5instead = 1;
UINT m_vk2instead = 1;
HookStruct KeyHook;

bool ShowMyFlag=false;//显示我方血条开启标志
bool ShowEnemyFlag=false;//显示敌方血条开启标志
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
    if(p->vkCode == VK_LWIN)
		return 1;
  }
  return  CallNextHookEx(KeyHook.hLowKeyBoardHook,nCode,wParam,lParam);
}

LRESULT CALLBACK KeyboardProc(
  int code,       // hook code
  WPARAM wParam,  // virtual-key code
  LPARAM lParam   // keystroke-message information
)
{
	if(wParam == VK_F11) //退出
	{
		if(GetAsyncKeyState(wParam) < 0)
		{
			::PostMessage(g_hHelpWnd,WM_CLOSE,0,0);		
		}
		return 1;
	}
	if(wParam == VK_F12) //停止
	{
		if(GetAsyncKeyState(wParam) < 0)
		{
			::PostMessage(g_hHelpWnd,WM_KILLTHREAD,0,0);
		}
		return 1;
	}
  return  CallNextHookEx(KeyHook.hKeyBoardHook,code,wParam,lParam);
}
HookStruct SetHook(HWND m_hwnd,HWND m_hwndHelper,UINT i,UINT j ,UINT k,UINT m,UINT n,UINT p,BOOL m_WinFlag)
{
    g_hWnd = m_hwnd;
	g_hHelpWnd = m_hwndHelper;
	m_vk7instead = MapVirtualKey(i,3);
	m_vk4instead = MapVirtualKey(j,3);
    m_vk1instead = MapVirtualKey(k,3);
    m_vk8instead = MapVirtualKey(m,3);
    m_vk5instead = MapVirtualKey(n,3);
    m_vk2instead = MapVirtualKey(p,3);
    LPDWORD lpdwProcessId=0;
	KeyHook.hKeyBoardHook=SetWindowsHookEx(WH_KEYBOARD,KeyboardProc,GetModuleHandle("Hook"),GetWindowThreadProcessId(g_hWnd,lpdwProcessId));
    if(m_WinFlag == TRUE)
	KeyHook.hLowKeyBoardHook=SetWindowsHookEx(13,LowLevelKeyboardProc,GetModuleHandle("Hook"),0);
	else KeyHook.hLowKeyBoardHook = NULL;
	return KeyHook;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
HookStruct SetHookEx(HWND m_hwndHelper)
{
	g_hHelpWnd = m_hwndHelper;
	LPDWORD lpdwProcessId=0;
	KeyHook.hKeyBoardHook=SetWindowsHookEx(WH_KEYBOARD,KeyboardProc,GetModuleHandle("Hook"),0);
	return KeyHook;
}