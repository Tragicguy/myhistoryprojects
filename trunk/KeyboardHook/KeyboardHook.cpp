#include "stdafx.h"
#include "stdio.h"
#include "KeyboardHook.h"
#include "winuser.h"

#pragma comment(lib,"user32.lib")

#ifdef _MANAGED
#pragma managed(push, off)
#endif

#pragma data_seg(".SHARDAT")
static HWND gl_hWnd = NULL;
HHOOK gl_hHook = NULL; 
HHOOK gl_hHookEx = NULL;
HINSTANCE gl_hInstance = NULL;
char gl_szpath[255] = {0};
#pragma data_seg()

void WriteLog(char * szInfo)
{	
	//return;
	FILE *fp;
	char szfile[1000]={0};
	sprintf(szfile,"%skeyboardhook.log",gl_szpath);
	fp=fopen(szfile,"a+");
	fprintf(fp,"%s\r\n",szInfo);
	fclose(fp);		
}
long gl_lastPushTime = 0;
//const UINT uHookMessage = ::RegisterWindowMessage("Web1800HookMessage");
const UINT uHookMessage = ::RegisterWindowMessage("Web1800HookMessage1");
const UINT uHookMessageEx = ::RegisterWindowMessage("Web1800HookMessage2");
LRESULT __declspec(dllexport)__stdcall  CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) 
{
	if(nCode == HC_ACTION)
	{
		
		PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT) lParam;		
		if ((p->vkCode == VK_ESCAPE && (GetKeyState(VK_SHIFT)&0x100)) 
			|| (p->vkCode == VK_SHIFT && (GetKeyState(VK_ESCAPE)&0x100)) )
		{
			if (GetTickCount() - gl_lastPushTime < 500)
			{
				WriteLog("dumplicate");
				return 1;
			}
			gl_lastPushTime = GetTickCount();
			char szInfo[1000] = {0};
			sprintf(szInfo,"VK_ESCAPE and VK_SHIFT pressed,%d",uHookMessage);
			WriteLog(szInfo);
			::PostMessage(HWND_BROADCAST, uHookMessage, 0, 0);  
			return 1;
		}
	}
	return CallNextHookEx(gl_hHook, nCode, wParam, lParam );  
}

LRESULT __declspec(dllexport)__stdcall  CALLBACK KeyboardProcEx(int nCode, WPARAM wParam, LPARAM lParam) 
{
	if(wParam == VK_F12)
	{ 
	   ::PostMessage(HWND_BROADCAST, uHookMessageEx, 0, 0);
	   return 1;
	   
	}
	return CallNextHookEx(gl_hHookEx, nCode, wParam, lParam );  
}

KEYBOARDHOOK_API BOOL StartHook(HWND hWnd,char *szPath)
{
	strcpy(gl_szpath,szPath);
	gl_hWnd = hWnd;

	gl_hHookEx = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeyboardProcEx, gl_hInstance, 0L); 
	DWORD dRet = GetLastError();
	char szInfo[1000] = {0};
	if (gl_hHookEx == NULL)
	{		
		sprintf(szInfo,"StartHookEx failed,%d",dRet);		
	}
	else
	{
		sprintf(szInfo,"StartHookEx run,%d",gl_hHookEx);
	}
	WriteLog(szInfo);

	gl_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardProc, gl_hInstance, 0L); 	
	dRet = GetLastError();
	memset(szInfo,0,1000);	
	if (gl_hHook == NULL)
	{		
		sprintf(szInfo,"StartHook failed,%d",dRet);		
	}
	else
	{
		sprintf(szInfo,"StartHook run,%d",gl_hHook);
	}
	WriteLog(szInfo);

	
	return (gl_hHook != NULL);
}

KEYBOARDHOOK_API BOOL StopHook()
{
	BOOL bResult = FALSE; 
	if(gl_hHook) 
	{
		WriteLog("StopHook real run");
		bResult = UnhookWindowsHookEx(gl_hHook);
		if(bResult)
		{
			gl_hHook = NULL;
		}		
	}
	WriteLog("StopHook run");
	return bResult; 
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	gl_hInstance = hModule;
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif