
// HookWininetDemoView.cpp : implementation of the CHookWininetDemoView class
//

#include "stdafx.h"
#include "HookWininetDemo.h"

#include "HookWininetDemoDoc.h"
#include "HookWininetDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHookWininetDemoView

IMPLEMENT_DYNCREATE(CHookWininetDemoView, CHtmlView)

BEGIN_MESSAGE_MAP(CHookWininetDemoView, CHtmlView)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CHookWininetDemoView construction/destruction

CHookWininetDemoView::CHookWininetDemoView()
{
	// TODO: add construction code here

}

CHookWininetDemoView::~CHookWininetDemoView()
{
}

BOOL CHookWininetDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CHtmlView::PreCreateWindow(cs);
}

void CHookWininetDemoView::OnInitialUpdate()
{
	CHtmlView::OnInitialUpdate();

	SetTimer( 1000, 1000, NULL);
}


// CHookWininetDemoView diagnostics

#ifdef _DEBUG
void CHookWininetDemoView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CHookWininetDemoView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}

CHookWininetDemoDoc* CHookWininetDemoView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHookWininetDemoDoc)));
	return (CHookWininetDemoDoc*)m_pDocument;
}
#endif //_DEBUG


// CHookWininetDemoView message handlers

void CHookWininetDemoView::OnTimer(UINT_PTR nIDEvent)
{
	Navigate2(_T("http://index.baidu.com/main/word.php?word=%BD%AB%B0%AE%C7%E9%BD%F8%D0%D0%B5%BD%B5%D7"),NULL,NULL);
	KillTimer(nIDEvent);
	CHtmlView::OnTimer(nIDEvent);
}
