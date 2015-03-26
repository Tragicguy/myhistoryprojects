#pragma once

#include "FileDefines.h"

class CFileManagerBase
{
public:
	CFileManagerBase(void);
	~CFileManagerBase(void);
public:

/*
message: FileClientStatusMSG
wParam:
0   Init file component failed.
1   Connect/accept/test send/recv  failed.
2   normal(loop recv/send) recv/send return 0/-1
3	send one file finished, lParam is the file name
4	receive one file finished, lParam is the file name
5   thread exit. OnThreadStop
6   Start file proccess ok.
7	Initiactive close.
*/
	void			NotifyWrapperWindow(int nExitType, LPARAM lParam = 0);
	
public:
	HWND			m_MSGNotifyWnd;
	int				m_nQuitMessageID;
};
