
// HookWininetDemoDoc.h : interface of the CHookWininetDemoDoc class
//


#pragma once


class CHookWininetDemoDoc : public CDocument
{
protected: // create from serialization only
	CHookWininetDemoDoc();
	DECLARE_DYNCREATE(CHookWininetDemoDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CHookWininetDemoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


