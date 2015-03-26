#if !defined(AFX_WARHELPERSETDLG_H__90AA0262_F3EF_4E36_98A6_7AFEB3FFE489__INCLUDED_)
#define AFX_WARHELPERSETDLG_H__90AA0262_F3EF_4E36_98A6_7AFEB3FFE489__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WarHelperSetDlg.h : header file
//
#include "WarHelperDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CWarHelperSetDlg dialog

class CWarHelperSetDlg : public CDialog
{
// Construction
public:
	BOOL OnInitDialog();
	BOOL PreTranslateMessage(MSG*   pMsg);
	void InitData(CString str);
	CWarHelperSetDlg(CWnd* pParent = NULL);   // standard constructor
    void OnCancel();
// Dialog Data
	//{{AFX_DATA(CWarHelperSetDlg)
	enum { IDD = IDD_DIALOG_SET };
	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWarHelperSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWarHelperSetDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CButton	m_winflag;
	CString	m_vk1;
	CString	m_vk2;
	CString	m_vk4;
	CString	m_vk5;
	CString	m_vk7;
	CString	m_vk8;
	CString wintemp;
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WARHELPERSETDLG_H__90AA0262_F3EF_4E36_98A6_7AFEB3FFE489__INCLUDED_)
