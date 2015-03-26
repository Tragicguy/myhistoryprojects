// DbGateView.h : interface of the CDbGateView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DbGateVIEW_H__B3E2896B_A0D7_4699_9561_5B99CC41C360__INCLUDED_)
#define AFX_DbGateVIEW_H__B3E2896B_A0D7_4699_9561_5B99CC41C360__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDbGateView : public CView
{
protected: // create from serialization only
	CDbGateView();
	DECLARE_DYNCREATE(CDbGateView)

// Attributes
public:
	CDbGateDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDbGateView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDbGateView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDbGateView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DbGateView.cpp
inline CDbGateDoc* CDbGateView::GetDocument()
   { return (CDbGateDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DbGateVIEW_H__B3E2896B_A0D7_4699_9561_5B99CC41C360__INCLUDED_)
