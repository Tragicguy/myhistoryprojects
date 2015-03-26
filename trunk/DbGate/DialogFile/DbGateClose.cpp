
#include "stdafx.h"
#include "DbGateClose.h"
#include ".\DbGateclose.h"
#include "../ControlFile/dataType.h"
HWND g_hwndClose = NULL;
IMPLEMENT_DYNAMIC(CDbGateClose, CDialog)
CDbGateClose::CDbGateClose(CWnd* pParent /*=NULL*/)
	: CDialog(CDbGateClose::IDD, pParent)
{
}

CDbGateClose::~CDbGateClose()
{
}

void CDbGateClose::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_THREAD, m_ProgressCtrl);
}


BEGIN_MESSAGE_MAP(CDbGateClose, CDialog)
	ON_WM_TIMER()
	ON_MESSAGE(USERWINDOWMSG_1,ThreadClose)		//线程关闭消息
END_MESSAGE_MAP()


/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CDbGateClose::OnInitDialog()
{
	CDialog::OnInitDialog();
	g_hwndClose = m_hWnd;
	m_Counter = 0;
	m_ProgressCtrl.SetRange(0,100);
	m_ProgressCtrl.SetPos(0);
	SetDlgItemText(IDC_INFO_SHOW,"程序正在退出,请稍后...");
	return TRUE;	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CDbGateClose::OnTimer(UINT nIDEvent)
{
	if ( nIDEvent == 1)
	{
		//OnOK();
	}
	CDialog::OnTimer(nIDEvent);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
LRESULT CDbGateClose::ThreadClose(WPARAM wParam,LPARAM lParam)
{
	m_Counter++;
	if (m_Counter <= m_ThreadNum)
	{
		int iOldCount = 100*(m_Counter-1)/m_ThreadNum;
		int i=0;
		for (i=iOldCount;i<100/m_ThreadNum+iOldCount;i++)
		{
			m_ProgressCtrl.SetPos(i);
			Sleep(10);
		}		
		if (m_Counter == m_ThreadNum)
		{
			Sleep(100);
			OnOK();
			return 0;
		}
	}
	else
	{
		Sleep(100);
		OnOK();
	}
	return 0;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CDbGateClose::PreTranslateMessage( MSG* pMsg )
{
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;      //屏蔽掉Esc键
	}
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		return TRUE;      //屏蔽掉Enter键
	}
	return CDialog::PreTranslateMessage(pMsg);
}

