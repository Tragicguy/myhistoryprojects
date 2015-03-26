
// MainFrm.h : interface of the CMainFrame class
//

#pragma once

#include "DlgRequest.h"

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	static CDlgRequest & GetRequestDlg(void)
	{
		return s_dlgRequest;
	}

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar        m_wndStatusBar;
	static CDlgRequest		  s_dlgRequest;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

};


