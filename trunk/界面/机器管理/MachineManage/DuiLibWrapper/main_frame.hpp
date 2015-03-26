 //
// main_frame.hpp
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

#ifndef MAINFRAME_HPP
#define MAINFRAME_HPP
#include "../DUIlib/UIlib.h"
#include "../DuiLib/Utils/WinImplBase.h"
#include "../DuiLibWrapper/UICrack.h"
using namespace DuiLib;

#ifdef Release_DuilibStatic

#else

#if defined(UILIB_EXPORTS)
#if defined(_MSC_VER)
#define UILIB_API __declspec(dllexport)
#else
#define UILIB_API 
#endif
#else
#if defined(_MSC_VER)
#define UILIB_API __declspec(dllimport)
#else
#define UILIB_API 
#endif
#endif

#endif



class MainFrame : public WindowImplBase
{
public:
	MainFrame();
	~MainFrame();

public:

	LPCTSTR GetWindowClassName() const;

	virtual void OnFinalMessage(HWND hWnd);

	virtual void Init();

	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

	virtual CDuiString GetSkinFile();

	virtual CDuiString GetSkinFolder();

	virtual CControlUI* CreateControl(LPCTSTR pstrClass);

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LONG GetStyle();
	void SetClassName(CDuiString className){ m_strClassName = className; }
	void SetSkinFile(int nID){
		TCHAR szBuf[MAX_PATH] = {0};
		_stprintf_s(szBuf, MAX_PATH - 1, _T("%d"), nID);
		m_strSkinFile = szBuf;
	}
	void SetSkinFile(CDuiString skinFile){ m_strSkinFile = skinFile; }
	void SetSkinFolder(CDuiString skinFolder){ m_strSkinFolder = skinFolder; }
	void SetSubSkinFolder(CDuiString skinSubFolder){ 
		m_strSkinFolder = skinSubFolder;
	}
	void ShowMaxMizeBox(bool bShow = true){ m_bShowMaxMizeBox = bShow; }
	void ShowMiniMizeBox(bool bShow = true){ m_bShowMiniMizeBox = bShow; }
	void SetBkImageMaxSize(int nCount){ m_nbkImageMaxSize = nCount; }
	DWORD GetBkColor();
	void SetBkColor(DWORD dwBackColor);
	void SendClickNotify(CControlUI* pControl, bool bAsync = true);
	void SendClickNotify(LPCTSTR controlID, bool bAsync = true);
	bool TranslateAccelerator( MSG* pMsg );
	bool TranslateMessage( MSG* pMsg );
protected:	

	virtual void Notify(TNotifyUI& msg);
	virtual void OnPrepare(TNotifyUI& msg);
	virtual void OnExit(TNotifyUI& msg);
	virtual void OnTimer(TNotifyUI& msg);
	virtual void OnClick(TNotifyUI& msg);
	CControlUI* FindControl(LPCTSTR pstrName){ return static_cast<CControlUI*>(m_PaintManager.FindControl(pstrName));}
	CDuiString GetDlgItemText(LPCTSTR pstrName);
	
	void OnChangeBkImage();
	void SetDlgItemText(LPCTSTR pstrName, LPCTSTR pstrValue);
	void SetOptionSelected(LPCTSTR strName, bool bSelected = true);

private:
	CDuiString m_strClassName;
	CDuiString m_strSkinFolder;
	
	
	bool m_bShowMiniMizeBox;
	int m_bk_image_index_;
	static int m_nbkImageMaxSize;
public:
	bool m_bShowMaxMizeBox;
	CDuiString m_strSkinFile;
};

#endif // MAINFRAME_HPP