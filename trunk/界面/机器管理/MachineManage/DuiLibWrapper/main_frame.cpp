//
// main_frame.cpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2011 achellies (achellies at 163 dot com)
//
// This code may be used in compiled form in any way you desire. This
// source file may be redistributed by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
// Beware of bugs.
//

#include "stdafx.h"
#include <windows.h>
#if !defined(UNDER_CE)
#include <shellapi.h>
#endif
#include "main_frame.hpp"


#pragma warning(disable:4800)

const TCHAR* const kTitleControlName = _T("apptitle");
const TCHAR* const kCloseButtonControlName = _T("closebtn");
const TCHAR* const kMinButtonControlName = _T("minbtn");
const TCHAR* const kMaxButtonControlName = _T("maxbtn");
const TCHAR* const kRestoreButtonControlName = _T("restorebtn");
const TCHAR* const kBackgroundControlName = _T("bg");
const TCHAR* const kChangeBkSkinControlName = _T("bkskinbtn");
const TCHAR* const kChangeColorSkinControlName = _T("colorskinbtn");
#define		WM_MENU		WM_USER+1008	// 菜单选中消息
int MainFrame::m_nbkImageMaxSize = -1;
MainFrame::MainFrame() : m_bShowMaxMizeBox(true), m_bShowMiniMizeBox(true), m_bk_image_index_(0)
{
	m_strClassName = "YutoneGUI";
}

MainFrame::~MainFrame()
{
	//PostQuitMessage(0);
}

LPCTSTR MainFrame::GetWindowClassName() const
{
	return m_strClassName;
}

CControlUI* MainFrame::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void MainFrame::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	/*******************////////
	//与MFC配合使用时，不能delete this.
	//delete this;
	exit(0);
}

CDuiString MainFrame::GetSkinFile()
{
	return m_strSkinFile;	
}

CDuiString MainFrame::GetSkinFolder()
{
	if(!m_strSkinFolder.IsEmpty()) return m_strSkinFolder;
	return CPaintManagerUI::GetInstancePath();
}

LONG MainFrame::GetStyle()
{ 
		DWORD dwStyle = WindowImplBase::GetStyle();
		if(!m_bShowMaxMizeBox)
			dwStyle &=  ~WS_MAXIMIZEBOX;
		if(!m_bShowMiniMizeBox)
			dwStyle &=  ~WS_MINIMIZEBOX;
		return dwStyle;
	}

LRESULT MainFrame::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( wParam == SC_CLOSE ) {
		::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		bHandled = TRUE;
		return 0;
	}
	BOOL bZoomed = ::IsZoomed(m_hWnd);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	
	if (::IsZoomed(m_hWnd) != bZoomed)
	{
		CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(kMaxButtonControlName));
		if( pControl ) pControl->SetVisible(bZoomed);
		pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(kRestoreButtonControlName));
		if( pControl ) pControl->SetVisible(!bZoomed);
	}
	return 0;
}

LRESULT MainFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	switch(uMsg)
	{		
		case WM_MENU:
		{
			CControlUI * pControl = (CControlUI *)lParam;
			if (pControl != NULL)
				this->m_PaintManager.SendNotify(pControl, _T("menu_msg"), wParam, lParam);
			break;
		}
	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT MainFrame::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}

void MainFrame::OnTimer(TNotifyUI& msg)
{
}

void MainFrame::OnExit(TNotifyUI& msg)
{
	Close();
}

void MainFrame::Init()
{

	CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));
	if(pControl) pControl->SetVisible(m_bShowMaxMizeBox);

	pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("minbtn")));
	if(pControl) pControl->SetVisible(m_bShowMiniMizeBox);
}

void MainFrame::OnPrepare(TNotifyUI& msg)
{
	Init();
}

void MainFrame::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, kWindowInit) == 0)
	{
		OnPrepare(msg);
	}
	else if (_tcsicmp(msg.sType, kClick) == 0)
	{
		OnClick(msg);
	}
	else if (_tcsicmp(msg.sType, kTimer) == 0)
	{
		return OnTimer(msg);
	}
}

void MainFrame::OnClick(TNotifyUI& msg)
{
	if (_tcsicmp(msg.pSender->GetName(), kCloseButtonControlName) == 0)
	{
		OnExit(msg);
	}
	else if (_tcsicmp(msg.pSender->GetName(), kMinButtonControlName) == 0)
	{
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
	}
	else if (_tcsicmp(msg.pSender->GetName(), kMaxButtonControlName) == 0)
	{
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
	}
	else if (_tcsicmp(msg.pSender->GetName(), kRestoreButtonControlName) == 0)
	{
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
	}
	else if(msg.pSender->IsEqual(kChangeBkSkinControlName))
	{
		OnChangeBkImage();
	}
	else if (_tcsicmp(msg.pSender->GetName(), kChangeColorSkinControlName) == 0)
	{
		CDuiRect rcWindow;
		GetWindowRect(m_hWnd, &rcWindow);
		rcWindow.top = rcWindow.top + msg.pSender->GetPos().bottom;
		//new ColorSkinWindow(this, rcWindow);
	}
}

LRESULT MainFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

CDuiString MainFrame::GetDlgItemText(LPCTSTR pstrName)
{
	CControlUI* pControl = FindControl(pstrName);
	if(pControl)
	return pControl->GetText(); 
	return _T("");
}


void MainFrame::SetDlgItemText(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	CControlUI* pControl = FindControl(pstrName);
	if(pControl)
		return pControl->SetText(pstrValue); 
}

DWORD MainFrame::GetBkColor()
{
	CControlUI* background = m_PaintManager.FindControl(kBackgroundControlName);
	if (background != NULL)
		return background->GetBkColor();

	return 0;
}


void MainFrame::SetOptionSelected(LPCTSTR strName, bool bSelected)
{
	COptionUI* pOption = (COptionUI*)m_PaintManager.FindControl(strName);
	if(!pOption) return;
	pOption->Selected(bSelected);
}

void MainFrame::SendClickNotify(LPCTSTR controlID, bool bAsync)
{
	CControlUI* pControl = m_PaintManager.FindControl(controlID);
	if (!pControl)
	{
		return;
	}
	SendClickNotify(pControl, bAsync);
}

void MainFrame::SendClickNotify(CControlUI* pControl, bool bAsync)
{
	TNotifyUI notice;
	notice.pSender = pControl;
	notice.sType = "click";
	m_PaintManager.SendNotify(notice, true);
}

void MainFrame::SetBkColor(DWORD dwBackColor)
{
	CControlUI* background = m_PaintManager.FindControl(kBackgroundControlName);
	if (background != NULL)
	{
		background->SetBkImage(_T(""));
		background->SetBkColor(dwBackColor);
		background->NeedUpdate();
// 
// 		SkinChangedParam param;
// 		param.bkcolor = background->GetBkColor();
// 		param.bgimage = background->GetBkImage();
// 		m_skin_changed_observer_.Broadcast(param);
	}
}

void MainFrame::OnChangeBkImage()
{
	if(m_nbkImageMaxSize < 0) return;
	CControlUI* background = m_PaintManager.FindControl(kBackgroundControlName);
	if (!background) return;

	TCHAR szBuf[MAX_PATH] = {0};
	++m_bk_image_index_;
	if (m_nbkImageMaxSize < m_bk_image_index_)
		m_bk_image_index_ = 0;

#if defined(UNDER_WINCE)
	_stprintf(szBuf, _T("file='bg%d.png' corner='600,200,1,1'"), m_bk_image_index_);
#else
	_stprintf_s(szBuf, MAX_PATH - 1, _T("file='bg%d.png' corner='600,200,1,1'"), m_bk_image_index_);
#endif
	//此处检测背景图是否存在，以保证界面运行良好
	background->SetBkImage(szBuf);
	background->SetBkColor(0x00000000);
// 	SkinChangedParam param;
// 	param.bkcolor = background->GetBkColor();
// 	if (_tcslen(background->GetBkImage()) > 0)
// 	{
// #if defined(UNDER_WINCE)
// 		_stprintf(szBuf, _T("bg%d.png"), bk_image_index_);
// #else
// 		_stprintf_s(szBuf, MAX_PATH - 1, _T("bg%d.png"), m_bk_image_index_);
// #endif
// 	}
// 
// 	param.bgimage = szBuf;
// 
// 	m_skin_changed_observer_.Broadcast(param);
}



bool MainFrame::TranslateAccelerator( MSG* pMsg )
{
	return m_PaintManager.TranslateAccelerator(pMsg);
}

bool MainFrame::TranslateMessage( MSG* pMsg )
{
	return m_PaintManager.TranslateMessage(pMsg);
}