#include "StdAfx.h"
#include ".\FileManagerBase.h"
#include "GlobalFun.h"

CFileManagerBase::CFileManagerBase(void)
{
	m_MSGNotifyWnd = NULL;
	m_nQuitMessageID = 0;
}

CFileManagerBase::~CFileManagerBase(void)
{

}

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
void CFileManagerBase::NotifyWrapperWindow(int nExitType, LPARAM lParam/*= 0*/)
{
	if (IsWindow(m_MSGNotifyWnd) && m_nQuitMessageID != 0)
	{
		PostMessage(m_MSGNotifyWnd, m_nQuitMessageID, nExitType, lParam);
		if (gl_pLogger)
			gl_pLogger->log_info("NotifyWrapperWindow Message: %d, ExitType: %d, lParam: %d.", m_nQuitMessageID, nExitType, lParam);
	}	
}
