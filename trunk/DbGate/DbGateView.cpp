// DbGateView.cpp : implementation of the CDbGateView class
//

#include "stdafx.h"
#include "DbGate.h"

#include "DbGateDoc.h"
#include "DbGateView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDbGateView

IMPLEMENT_DYNCREATE(CDbGateView, CView)

BEGIN_MESSAGE_MAP(CDbGateView, CView)
	//{{AFX_MSG_MAP(CDbGateView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDbGateView construction/destruction

CDbGateView::CDbGateView()
{
	// TODO: add construction code here

}

CDbGateView::~CDbGateView()
{
}

BOOL CDbGateView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDbGateView drawing

void CDbGateView::OnDraw(CDC* pDC)
{
	CDbGateDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CDbGateView printing

BOOL CDbGateView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDbGateView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDbGateView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDbGateView diagnostics

#ifdef _DEBUG
void CDbGateView::AssertValid() const
{
	CView::AssertValid();
}

void CDbGateView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDbGateDoc* CDbGateView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDbGateDoc)));
	return (CDbGateDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDbGateView message handlers
