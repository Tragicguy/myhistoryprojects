
// HookWininetDemoDoc.cpp : implementation of the CHookWininetDemoDoc class
//

#include "stdafx.h"
#include "HookWininetDemo.h"

#include "HookWininetDemoDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHookWininetDemoDoc

IMPLEMENT_DYNCREATE(CHookWininetDemoDoc, CDocument)

BEGIN_MESSAGE_MAP(CHookWininetDemoDoc, CDocument)
END_MESSAGE_MAP()


// CHookWininetDemoDoc construction/destruction

CHookWininetDemoDoc::CHookWininetDemoDoc()
{
	// TODO: add one-time construction code here

}

CHookWininetDemoDoc::~CHookWininetDemoDoc()
{
}

BOOL CHookWininetDemoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CHookWininetDemoDoc serialization

void CHookWininetDemoDoc::Serialize(CArchive& ar)
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


// CHookWininetDemoDoc diagnostics

#ifdef _DEBUG
void CHookWininetDemoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHookWininetDemoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CHookWininetDemoDoc commands
