#pragma once

#include "Resource.h"
#include "UIHelper.h"
#include "PictureButton\PicButton.h"
#include "afxcmn.h"

#include "afxmt.h"

#define WM_CLOSEPROMPTDLG WM_USER + 10000

extern bool gl_bPrompt;
extern CCriticalSection gl_PromptSection;

extern void SetPrompt(bool bShow,HWND hwnd);

extern bool GetPrompt(HWND hwnd);

// CPromptDlg 对话框

typedef enum { MessageDlg = 0, AcceptAskDlg = 1, OkCancelDlg = 2 } DlgType;

class CPromptDlg : public CDialog
{
	DECLARE_DYNAMIC(CPromptDlg)

public:
	CPromptDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPromptDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PROMPT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

private:
	void AddRichEditMsg(LPCTSTR strMsg);

	DlgType	m_eType;
	int m_iSpType;
private:
	CString		m_strMsg;
	CPicButton	m_btClose;
	CPicButton	m_btCancel;
	CPicButton	m_btOk;
	HWND m_hwndParant;
	CPicButton	m_btMessageOk;
	CRichEditCtrl m_REMsg;
	HBITMAP m_hBmpScrollBar; //2009-1-9 for skin scrollbar
public:
	void SetMsgText(LPCTSTR szMsgText);
	void SetType(DlgType eType);
	void Show();
	void SetHwnd(HWND hwnd){m_hwndParant = hwnd;}
	void SetSpType(int iType){ m_iSpType = iType; }
public:
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButtonOk();
	
private:
	void InitUI();
};
