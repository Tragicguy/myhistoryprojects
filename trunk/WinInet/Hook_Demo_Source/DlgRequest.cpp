// DlgRequest.cpp : implementation file
//

#include "stdafx.h"
#include "HookWininetDemo.h"
#include "DlgRequest.h"
#include "WininetHook.h"


// CDlgRequest dialog


IMPLEMENT_DYNAMIC(CDlgRequest, CDialog)

CDlgRequest::CDlgRequest(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRequest::IDD, pParent)
	, m_strHttpHeader(_T(""))
	, m_strResponseHttpHeader(_T(""))
	, m_strSessionID(_T(""))
{

}

CDlgRequest::~CDlgRequest()
{
}

void CDlgRequest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_REQUEST, m_ctlRequest);
	DDX_Text(pDX, IDC_EDIT_REQUEST_HTTP_HEADERS, m_strHttpHeader);
	DDX_Text(pDX, IDC_EDIT_RESPONSE_HTTP_HEADERS2, m_strResponseHttpHeader);

	DDX_Text(pDX, IDC_EDIT_SESSION, m_strSessionID);
}


BEGIN_MESSAGE_MAP(CDlgRequest, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_REQUEST, &CDlgRequest::OnLvnItemchangedListRequest)
END_MESSAGE_MAP()


// CDlgRequest message handlers

BOOL CDlgRequest::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ctlRequest.InsertColumn( 0, _T("Time"), LVCFMT_CENTER, 60);
	m_ctlRequest.InsertColumn( 1, _T("Method"), LVCFMT_CENTER, 70);
	m_ctlRequest.InsertColumn( 2, _T("Url"), LVCFMT_LEFT, 500);	
	m_ctlRequest.InsertColumn( 3, _T(""), LVCFMT_LEFT, 0);
	m_ctlRequest.InsertColumn( 4, _T(""), LVCFMT_LEFT, 0);
	m_ctlRequest.InsertColumn( 5, _T(""), LVCFMT_LEFT, 0);
	m_ctlRequest.InsertColumn( 6, _T(""), LVCFMT_LEFT, 0);
 
	m_ctlRequest.SetExtendedStyle( m_ctlRequest.GetExtendedStyle() | LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgRequest::LogInternetConnect(HINTERNET hConnect, LPTSTR lpszHost, UINT uPort)
{	
	tagInternetConnectInfo stConnectInfo = {0};

	stConnectInfo.strHost = lpszHost;
	stConnectInfo.strPort.Format( _T("%d"), uPort);

	std::map< HINTERNET, tagInternetConnectInfo>::iterator iter;
	iter = m_mpConnection.find(hConnect);
	if( iter != m_mpConnection.end() )
		m_mpConnection.erase(iter);

	m_mpConnection.insert( std::pair< HINTERNET, tagInternetConnectInfo>( hConnect, stConnectInfo));
}

void CDlgRequest::LogRequest(HINTERNET hConnect, HINTERNET hRequest, LPCTSTR lpszObjName, LPCTSTR lpszMethod)
{
	if( IsWindow(m_ctlRequest.GetSafeHwnd()) )
	{
		std::map< HINTERNET, tagInternetConnectInfo>::iterator iter;
		iter = m_mpConnection.find(hConnect);
		if( iter == m_mpConnection.end() )
			return;

		COleDateTime dtNow = COleDateTime::GetCurrentTime();

		int nItem = m_ctlRequest.InsertItem( 0xFFFF, dtNow.Format(_T("%H:%M:%S")));

		CString strUrl;
		strUrl.Format( _T("http://%s"), iter->second.strHost);
		if( iter->second.strPort.Compare(_T("80")) != 0 )
			strUrl.AppendFormat(_T(":%s"), iter->second.strPort);
		strUrl.Append(lpszObjName);

		m_ctlRequest.SetItemText( nItem, 1, lpszMethod);
		m_ctlRequest.SetItemText( nItem, 2, strUrl);
		
		m_mpRequestItem[hRequest] = nItem;		
	}
}




void CDlgRequest::LogHttpResponseHeader(HINTERNET hRequest)
{
	std::map< HINTERNET, INT>::iterator iter;
	iter = m_mpRequestItem.find(hRequest);
	if( iter != m_mpRequestItem.end() )
	{
		USES_CONVERSION;
		TCHAR tszTemp[10240] = {0};
		DWORD dwSize = sizeof(tszTemp);
		if( HttpQueryInfo( hRequest, HTTP_QUERY_RAW_HEADERS_CRLF, tszTemp, &dwSize, 0) )
			m_ctlRequest.SetItemText( iter->second, 3, W2T(tszTemp));

		dwSize = sizeof(tszTemp);
		ZeroMemory( tszTemp, sizeof(tszTemp));
		if( HttpQueryInfo( hRequest, HTTP_QUERY_RAW_HEADERS_CRLF | HTTP_QUERY_FLAG_REQUEST_HEADERS , tszTemp, &dwSize, 0) )
			m_ctlRequest.SetItemText( iter->second, 4, W2T(tszTemp));


		dwSize = sizeof(tszTemp);
		ZeroMemory( tszTemp, sizeof(tszTemp));
		if( InternetGetCookieEx( m_ctlRequest.GetItemText(iter->second, 2)
			, _T("ASP.NET_SessionId")
			, tszTemp
			, &dwSize
			, INTERNET_COOKIE_HTTPONLY
			, NULL
			) )
		{
			m_ctlRequest.SetItemText( iter->second, 6, W2T(tszTemp));
		}

		m_ctlRequest.SetItemState( iter->second, LVIS_SELECTED, LVIS_SELECTED);   
		m_mpRequestItem.erase(iter);
	}
}

void CDlgRequest::OnLvnItemchangedListRequest(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	POSITION pos = m_ctlRequest.GetFirstSelectedItemPosition();
	if( pos )
	{
		int nItem = m_ctlRequest.GetNextSelectedItem(pos);
		m_strHttpHeader = m_ctlRequest.GetItemText( nItem, 4);

		m_strResponseHttpHeader = m_ctlRequest.GetItemText( nItem, 3);

		m_strSessionID = m_ctlRequest.GetItemText( nItem, 6);

		UpdateData(FALSE);
	}
}
