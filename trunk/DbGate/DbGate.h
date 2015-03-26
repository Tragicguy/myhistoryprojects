// DbGate.h : main header file for the DbGate application
//

#if !defined(AFX_DbGate_H__BBEE846D_DBC6_4D3F_A387_16F241EDBFA6__INCLUDED_)
#define AFX_DbGate_H__BBEE846D_DBC6_4D3F_A387_16F241EDBFA6__INCLUDED_
 
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDbGateApp:
// See DbGate.cpp for the implementation of this class
//

class CDbGateApp : public CWinApp
{
public:
	CDbGateApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDbGateApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CDbGateApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	BOOL ExitInstance(void);
	HANDLE m_hMutex;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DbGate_H__BBEE846D_DBC6_4D3F_A387_16F241EDBFA6__INCLUDED_)
