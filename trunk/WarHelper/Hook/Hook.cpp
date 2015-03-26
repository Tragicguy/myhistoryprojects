#include <windows.h>

#include "winuser.h"

#define	WM_SETTIMER3	WM_USER+2
#define	WM_SETTIMER4	WM_USER+3
#define	WM_KILLTIMER3	WM_USER+4
#define	WM_KILLTIMER4	WM_USER+5
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

if(wParam == VK_F12)
  { 
	   if(KeyHook.hKeyBoardHook  != NULL)
		UnhookWindowsHookEx(KeyHook.hKeyBoardHook);
		if(KeyHook.hLowKeyBoardHook != NULL)
		UnhookWindowsHookEx(KeyHook.hLowKeyBoardHook);
		::SendMessage(g_hWnd,WM_CLOSE,0,0);
		return 1;

  }	   

	if(wParam == m_vk7instead )
	{
		if(GetAsyncKeyState(m_vk7instead) < 0)
		   {
		  keybd_event(VK_NUMPAD7,0,0,0);
		  keybd_event(VK_NUMPAD7,0,KEYEVENTF_KEYUP,0);
		  }
		  return 1;
    }
	
	if(wParam == m_vk4instead )
	{
		if(GetAsyncKeyState(m_vk4instead) < 0)
		   {
		  keybd_event(VK_NUMPAD4,0,0,0);
		  keybd_event(VK_NUMPAD4,0,KEYEVENTF_KEYUP,0);
		  }
		  return  1;
    }

	if(wParam == m_vk1instead )
	{
		if(GetAsyncKeyState(m_vk1instead) < 0)
		   {
		  keybd_event(VK_NUMPAD1,0,0,0);
		  keybd_event(VK_NUMPAD1,0,KEYEVENTF_KEYUP,0);
		  }
		  return  1;
    }
	
	if(wParam == m_vk8instead )
	{
		if(GetAsyncKeyState(m_vk8instead) < 0)
		   {
		  keybd_event(VK_NUMPAD8,0,0,0);
		  keybd_event(VK_NUMPAD8,0,KEYEVENTF_KEYUP,0);
		  }
		  return  1;
    }

	if(wParam == m_vk5instead )
	{
		if(GetAsyncKeyState(m_vk5instead) < 0)
		   {
		  keybd_event(VK_NUMPAD5,0,0,0);
		  keybd_event(VK_NUMPAD5,0,KEYEVENTF_KEYUP,0);
		  }
		  return 1;
    }

	if(wParam == m_vk2instead )
	{
		if(GetAsyncKeyState(m_vk2instead) < 0)
		   {
		  keybd_event(VK_NUMPAD2,0,0,0);
		  keybd_event(VK_NUMPAD2,0,KEYEVENTF_KEYUP,0);
		  }
		  return  1;
    }
	if(wParam == VK_OEM_4 || wParam == VK_OEM_6)
	{
		return 1;
	}

	if(wParam == VK_HOME) //显示自己方血条
	{
		if(GetAsyncKeyState(wParam) < 0)
		{
          
		if(!ShowMyFlag)
		{
			::SendMessage(g_hWnd,WM_KEYDOWN,VK_OEM_4,0);
			 ::PostMessage(g_hHelpWnd,WM_SETTIMER3,0,0);	
			 ShowMyFlag=true;
		}
		else
		{
          ::PostMessage(g_hHelpWnd,WM_KILLTIMER3,0,0);
		  ::SendMessage(g_hWnd,WM_KEYUP,VK_OEM_4,0);
		 ShowMyFlag=false;
		}
		}
		return 1;
	}

	if(wParam == VK_END) //关闭显示我方血条
	{
		if(GetAsyncKeyState(wParam) < 0)
		{
		 if(!ShowEnemyFlag)
		{
			::SendMessage(g_hWnd,WM_KEYDOWN,VK_OEM_6,0);
			 ::PostMessage(g_hHelpWnd,WM_SETTIMER4,0,0);	
			 ShowEnemyFlag=true;
		}
		 else
		 {
		::PostMessage(g_hHelpWnd,WM_KILLTIMER4,0,0);
		::SendMessage(g_hWnd,WM_KEYUP,VK_OEM_6,0);
		 ShowEnemyFlag=false;
		 }
		
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