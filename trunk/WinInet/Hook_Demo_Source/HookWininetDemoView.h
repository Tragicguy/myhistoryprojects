
// HookWininetDemoView.h : interface of the CHookWininetDemoView class
//


#pragma once


class CHookWininetDemoView : public CHtmlView
{
protected: // create from serialization only
	CHookWininetDemoView();
	DECLARE_DYNCREATE(CHookWininetDemoView)

// Attributes
public:
	CHookWininetDemoDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CHookWininetDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // debug version in HookWininetDemoView.cpp
inline CHookWininetDemoDoc* CHookWininetDemoView::GetDocument() const
   { return reinterpret_cast<CHookWininetDemoDoc*>(m_pDocument); }
#endif

