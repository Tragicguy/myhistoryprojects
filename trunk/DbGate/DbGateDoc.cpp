// DbGateDoc.cpp : implementation of the CDbGateDoc class
//

#include "stdafx.h"
#include "DbGate.h"

#include "DbGateDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDbGateDoc

IMPLEMENT_DYNCREATE(CDbGateDoc, CDocument)

BEGIN_MESSAGE_MAP(CDbGateDoc, CDocument)
	//{{AFX_MSG_MAP(CDbGateDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDbGateDoc construction/destruction

CDbGateDoc::CDbGateDoc()
{
	// TODO: add one-time construction code here

}

CDbGateDoc::~CDbGateDoc()
{
}

BOOL CDbGateDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDbGateDoc serialization

void CDbGateDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDbGateDoc diagnostics

#ifdef _DEBUG
void CDbGateDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDbGateDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDbGateDoc commands
