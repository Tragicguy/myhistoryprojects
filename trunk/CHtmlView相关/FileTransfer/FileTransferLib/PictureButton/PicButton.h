#if !defined(AFX_MYBUTTON_H__5BBF3FA8_E488_4AC1_A2B7_3EB39A14EC64__INCLUDED_)
#define AFX_MYBUTTON_H__5BBF3FA8_E488_4AC1_A2B7_3EB39A14EC64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PicButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPicButton window

class CPicButton : public CButton
{
// Construction
public:
	CPicButton();

// Attributes
protected:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPicButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetToolTipText(CString *spText, BOOL bActivate=TRUE);
	void SetToolTipText(int nId, BOOL bActivate=TRUE);
	void InitToolTip();
	BOOL SetBtnCursor(int nCursorId=-1);
	void SetIcon(int nIconInId, int nIconOutId=NULL);
	BOOL m_MouseOnButton;
	BOOL m_bTracking;
	CBitmap* m_hBmpOut;
	CBitmap* m_hBmpIn;
	HCURSOR m_hCursor;

	virtual ~CPicButton();

	// Generated message map functions
protected:
	CToolTipCtrl m_ToolTip;
	//{{AFX_MSG(CPicButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam) ;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	bool m_IconLoaded;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PicButton_H__5BBF3FA8_E488_4AC1_A2B7_3EB39A14EC64__INCLUDED_)
