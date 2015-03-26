// AESTesterDlg.h : header file
//

#pragma once
#include "../AESEncryption/AESEncryption.h"
#include "afxwin.h"

// CAESTesterDlg dialog
class CAESTesterDlg : public CDialog
{
// Construction
public:
	CAESTesterDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_AESTESTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	CEdit m_Source;
	CEdit m_Destination;
	afx_msg void OnBnClickedButton2();
	CComboBox m_Key;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButton3();
};
