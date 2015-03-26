// WarHelperDlg.h : header file
//

#if !defined(AFX_WARHELPERDLG_H__4BF615FA_B867_42D3_ABBA_66512F5E312E__INCLUDED_)
#define AFX_WARHELPERDLG_H__4BF615FA_B867_42D3_ABBA_66512F5E312E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "WarHelperSetDlg.h"
#include "lib/SkinPPWTL.h"
#define WM_SHOWTASK WM_USER + 1
/////////////////////////////////////////////////////////////////////////////
// CWarHelperDlg dialog
#define	WM_SETTIMER3	WM_USER+2
#define	WM_SETTIMER4	WM_USER+3
#define	WM_KILLTIMER3	WM_USER+4
#define	WM_KILLTIMER4	WM_USER+5

struct HookStruct
{
  HHOOK hKeyBoardHook;
  HHOOK hLowKeyBoardHook;
};
class CWarHelperDlg : public CDialog
{
// Construction
public:
	void OnCancel();
	void SetToTray();
	void GetSavedData(CString str);
	void Begin();
	void GetData(CString vk7,CString vk4,CString vk1,CString vk8,CString vk5,CString vk2,CString winstr);
	CWarHelperDlg(CWnd* pParent = NULL);	// standard constructor
    BOOL m_WinFlag;
    HWND m_hwndHack;
	HookStruct KeyHook;
// Dialog Data
	//{{AFX_DATA(CWarHelperDlg)
	enum { IDD = IDD_WARHELPER_DIALOG };
	
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWarHelperDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
    // Generated message map functions
	//{{AFX_MSG(CWarHelperDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSet();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnShowTask(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnSetTimer3(WPARAM wParam,LPARAM lParam);
    afx_msg LRESULT OnSetTimer4(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnKillTimer3(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnKillTimer4(WPARAM wParam,LPARAM lParam);
	afx_msg void OnClose();
	afx_msg void OnMenuSet();
	afx_msg void OnAbout();
	afx_msg void OnButtonExit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString m_vk7;
	CString m_vk4;
	CString m_vk1;
	CString m_vk8;
	CString m_vk5;
	CString m_vk2;
	char *stopstring;
	NOTIFYICONDATA nid;
	CString	m_show;
public:
	void UnloadHook(void);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WARHELPERDLG_H__4BF615FA_B867_42D3_ABBA_66512F5E312E__INCLUDED_)
