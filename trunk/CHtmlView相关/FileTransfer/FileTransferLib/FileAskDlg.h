#pragma once

#include "Resource.h"
#include "UIHelper.h"
#include "PictureButton\PicButton.h"
#include "afxcmn.h"

extern CString strLanguageId;

// CFileAskDlg 对话框

class CFileAskDlg : public CDialog
{
	DECLARE_DYNAMIC(CFileAskDlg)

public:
	CFileAskDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFileAskDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_FILEASK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	
	CString		m_strFileName;
private:
	bool CreateTestFile(LPCTSTR szFileName);
	void SetDlgReign();
	void AddRichEditMsg(LPCTSTR strMsg);
private:
	CString		m_strMsg;
	CPicButton	m_btClose;
	CPicButton	m_btAccept;
	CPicButton	m_btSaveAs;
	CPicButton	m_btReject;
	CRichEditCtrl m_REMsg;
	HBITMAP m_hBmpScrollBar; //2009-1-9 for skin scrollbar
	HWND m_hRapperWnd;
public:
	CString m_strDistinationFolder;
	void SetMsgText(LPCTSTR szMsgText);
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	
	afx_msg void OnBnClickedCancel2();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonSaveas();
	afx_msg void OnEnSetfocusEditMsg();
	void SetRapperHwnd(HWND hwnd){m_hRapperWnd = hwnd;}
};
