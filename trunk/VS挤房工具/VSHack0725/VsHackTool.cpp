#include "StdAfx.h"
#include ".\vshacktool.h"

CVsHackTool::CVsHackTool(void)
{
}

CVsHackTool::~CVsHackTool(void)
{
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//关闭警告框
void CVsHackTool::CloseWarnWindow()
{
	HWND hwndWarn = NULL;
	hwndWarn = FindWindow(_T("#32770"), _T("VS竞技游戏平台")); //满员警告框
	if(hwndWarn != NULL)
	{
		//hwndWarn = pWnd->GetSafeHwnd();
		//将系统时间减少30秒,避开客户端验证
		SYSTEMTIME systime;
		GetLocalTime(&systime); 
		union 
		{
			FILETIME ft;
			ULONGLONG ut;
		}t;
		DWORD dwMiniSeconds=30;
		SystemTimeToFileTime(&systime, &t.ft);
		t.ut-=UInt32x32To64(dwMiniSeconds,10000000);//减30秒
		FileTimeToSystemTime(&t.ft, &systime);
		SetLocalTime(&systime);
		::SendMessage(hwndWarn,WM_CLOSE,0,0);

		Sleep(200);		//SLEEP 100 毫秒

		SystemTimeToFileTime(&systime, &t.ft);
		t.ut+=UInt32x32To64(dwMiniSeconds,10000000);//加30秒
		FileTimeToSystemTime(&t.ft, &systime);
		SetLocalTime(&systime);
		//将正确的系统时间还原
		dblhitFlag = true;
	}
	hwndWarn = NULL;
	hwndWarn = FindWindow(_T("#32770"), _T("VSClient"));//不能重复进入房间警告框
	if(hwndWarn != NULL)
	{
		//hwndWarn = pWnd->GetSafeHwnd();
		::SendMessage(hwndWarn,WM_CLOSE,0,0);

		Sleep(200);		//SLEEP 100 毫秒

		SYSTEMTIME systime;
		GetLocalTime(&systime);
		union 
		{
			FILETIME ft;
			ULONGLONG ut;
		}t;
		DWORD dwMiniSeconds=10;
		SystemTimeToFileTime(&systime, &t.ft);
		t.ut+=UInt32x32To64(dwMiniSeconds,10000000);//加10秒
		FileTimeToSystemTime(&t.ft, &systime);
		SetLocalTime(&systime);
		dblhitFlag = true;
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//挤房并获取房间名
void CVsHackTool::TryIntoRoom()
{
	CWnd *pHack = CWnd::FromHandle(hwnd);
	CListCtrl *listHack = (CListCtrl *)pHack;

	POSITION pos = listHack->GetFirstSelectedItemPosition();
	int iItem = listHack->GetNextSelectedItem(pos); //房间索引号

	if (iItem != oldItem)  //如果房间索引号变了，则重新获取新的房间名，并重新获取新的房间项区域
	{
		oldItem = iItem;   //重置原房间索引

		RECT *lpRect;
		LVITEM lvitem, *plvitem;
		char ItemBuf[512],*pItem;
		DWORD PID;
		HANDLE hProcess;

		GetWindowThreadProcessId(hwnd, &PID);
		hProcess=OpenProcess(PROCESS_ALL_ACCESS,false,PID);
		if (!hProcess)
		{
			//获取进程句柄操作失败
		}
		else
		{
			plvitem=(LVITEM*)VirtualAllocEx(hProcess, NULL, sizeof(LVITEM), MEM_COMMIT, PAGE_READWRITE);
			pItem=(char*)VirtualAllocEx(hProcess, NULL, 512, MEM_COMMIT, PAGE_READWRITE);

			if ((!plvitem)||(!pItem))
			{
				//无法分配内存
			}
			else    //获取房间名
			{
				lvitem.cchTextMax=512;
				lvitem.iSubItem=0; //ProcessName
				lvitem.pszText=pItem;
				WriteProcessMemory(hProcess, plvitem, &lvitem, sizeof(LVITEM), NULL);
				::SendMessage(hwnd, LVM_GETITEMTEXT, (WPARAM)iItem, (LPARAM)plvitem);
				ReadProcessMemory(hProcess, pItem, ItemBuf, 512, NULL);
			}
			CloseHandle(hProcess);
			VirtualFreeEx(hProcess, plvitem, 0, MEM_RELEASE);
			VirtualFreeEx(hProcess, pItem, 0, MEM_RELEASE);

			lpRect  =  (RECT*)VirtualAllocEx(hProcess,  NULL,  sizeof(RECT),  MEM_COMMIT,  PAGE_READWRITE);

			if (!lpRect)
			{
				//无法分配内存
			}
			else
			{
				//获取房间项区域
				ZeroMemory(&Rect,sizeof(Rect));
				WriteProcessMemory(hProcess,  lpRect,  &Rect,  sizeof(RECT),  NULL);
				::SendMessage(hwnd,  LVM_GETITEMRECT,  (WPARAM)iItem,  (LPARAM)lpRect);
				ReadProcessMemory(hProcess,  lpRect,  &Rect,  sizeof(RECT),  NULL);
				CloseHandle(hProcess);
				VirtualFreeEx(hProcess, lpRect, 0, MEM_RELEASE);
			}

		}
		SetDlgItemText(IDC_EDIT_ROOM,ItemBuf);//将房间名赋值到文本框
	}

	DblHitItem(); //双击，挤房
	if (hitFlag)
	{
		::SetTimer(m_hWnd,2,400,NULL);//重新开启计时器，循环挤房
	}
}

//发送双击消息
void CVsHackTool::DblHitItem()
{
	CWnd *pWnd1 = NULL;
	CWnd *pWnd2 = NULL;
	pWnd2 = FindWindow(_T("#32770"), _T("VSClient"));
	pWnd1 = FindWindow(_T("#32770"), _T("VS竞技游戏平台"));
	if(!pWnd1 && !pWnd2 && dblhitFlag)  //如果没有警告框且挤房标志为真
	{
		dblhitFlag = false;
		::SendMessage(hwnd, WM_LBUTTONDBLCLK, 0,MAKELONG(Rect.left,Rect.top));//发送双击消息
	}

}