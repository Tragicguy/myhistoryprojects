// WarHelper.h : main header file for the WARHELPER application
//

#if !defined(AFX_WARHELPER_H__58D2897D_B366_44BC_8838_85D8AA29B0C9__INCLUDED_)
#define AFX_WARHELPER_H__58D2897D_B366_44BC_8838_85D8AA29B0C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CWarHelperApp:
// See WarHelper.cpp for the implementation of this class
//

class CWarHelperApp : public CWinApp
{
public:
	CWarHelperApp();
	virtual BOOL ExitInstance(void);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWarHelperApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CWarHelperApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WARHELPER_H__58D2897D_B366_44BC_8838_85D8AA29B0C9__INCLUDED_)
