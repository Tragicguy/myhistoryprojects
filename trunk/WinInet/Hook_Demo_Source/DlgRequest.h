#pragma once
#include "afxwin.h"
#include "resource.h"
#include <WinInet.h>
#include <map>
// CDlgRequest dialog

typedef struct _tagInternetConnectInfo
{
	CString strHost;
	CString strPort;
} tagInternetConnectInfo, *LPInternetConnectInfo;


class CDlgRequest : public CDialog
{
	DECLARE_DYNAMIC(CDlgRequest)

public:
	CDlgRequest(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgRequest();
	
	void LogInternetConnect(HINTERNET hConnect, LPTSTR lpszHost, UINT uPort);
	void LogRequest(HINTERNET hConnect, HINTERNET hRequest, LPCTSTR lpszObjName, LPCTSTR lpszMethod);
	void LogHttpResponseHeader(HINTERNET hRequest);

// Dialog Data
	enum { IDD = IDD_DIALOG_REQUEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK() { };
	virtual void OnCancel() {};

	DECLARE_MESSAGE_MAP()
	afx_msg void OnLvnItemchangedListRequest(NMHDR *pNMHDR, LRESULT *pResult);

	CListCtrl m_ctlRequest;
	CString m_strHttpHeader;
	CString m_strResponseHttpHeader;
	CString m_strSessionID;

	std::map< HINTERNET, tagInternetConnectInfo>	m_mpConnection;
	std::map< HINTERNET, INT>						m_mpRequestItem;
};
