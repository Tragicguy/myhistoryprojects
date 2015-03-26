// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__04B22B39_B172_44E8_966A_5C6A5448B362__INCLUDED_)
#define AFX_MAINFRM_H__04B22B39_B172_44E8_966A_5C6A5448B362__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ControlFile/SplitterWndEx.h"
#include "ControlFile/ADOFile/AdoRecordSet.h"
#include "ControlFile/MWMessageSendEx.h"
class CMainFrame : public CFrameWnd
{
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members	
	CToolBar    m_wndToolBar;

	CSplitterWndEx m_wndSplitter;
	bool m_bCreateSplit;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	BOOL OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext);
	void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnMenuRun();
	afx_msg void OnMenuSet();
	afx_msg void OnMenuCheckfee();
	afx_msg void OnMenuStop();
	afx_msg void OnUpdateMenuCmd(CCmdUI *pCmdUI);
	void OnClose();
	void SetToTray();
	
private:
	NOTIFYICONDATA m_nid;
	CMWMessageSendEx m_MWSendCtrl;
	BOOL m_ThreadRunFlag;
	bool m_bAutoStop;		//是否为自动停止标志
	CTime m_StartTime;		//系统启动时间
	CStatusBar  m_wndStatusBar;
public:
    afx_msg	LRESULT OnShowTask(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnAppExit(WPARAM wParam,LPARAM lParam);
	afx_msg void OnAppQuit();
	void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg LRESULT OnSendMessage(WPARAM wParam,LPARAM lParam);
	afx_msg void OnTimer(UINT nIDEvent);
	CString GetRunningTime();
	void ReSetParams();
	afx_msg LRESULT OnStatusMessage(WPARAM wParam,LPARAM lParam);
	afx_msg void OnMenuSinglesend();
	void RunHelpTool(CString strAppPath);
	void SendSingleMessage(CString strPhone,CString strSubport,CString strContent);
	BOOL PreTranslateMessage( MSG* pMsg );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__04B22B39_B172_44E8_966A_5C6A5448B362__INCLUDED_)
