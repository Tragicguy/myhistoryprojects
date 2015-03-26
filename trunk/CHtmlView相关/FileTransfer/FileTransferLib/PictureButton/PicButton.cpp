// PicButton.cpp : implementation file
//

#include "stdafx.h"

#include "PicButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPicButton

CPicButton::CPicButton()
{
	m_MouseOnButton = FALSE;
	m_bTracking = FALSE;
	m_hBmpIn = new CBitmap;
	m_hBmpOut = new CBitmap;
	m_IconLoaded = false;
}

CPicButton::~CPicButton()
{
	if (m_hBmpIn != NULL) 
	{
		delete m_hBmpIn;
	}

	if (m_hBmpOut != NULL) 
	{
		delete m_hBmpOut;
	}
}


BEGIN_MESSAGE_MAP(CPicButton, CButton)
	//{{AFX_MSG_MAP(CPicButton)
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPicButton message handlers

void CPicButton::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	// TODO: Add your code to draw the specified item

	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	CRect iconRect = lpDIS->rcItem;
	pDC->Rectangle(iconRect);
	BOOL bIsDisabled = (lpDIS->itemState & ODS_DISABLED);
	pDC->DrawState(iconRect.TopLeft(),
				   iconRect.Size(), 
				   (m_MouseOnButton == TRUE) ? m_hBmpIn : m_hBmpOut, 
				   (bIsDisabled ? DSS_DISABLED : DSS_NORMAL), 
				   (CBrush*)NULL);
}

void CPicButton::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	SetButtonStyle(GetButtonStyle() | BS_OWNERDRAW);
	CButton::PreSubclassWindow();
}

BOOL CPicButton::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	InitToolTip();
	m_ToolTip.RelayEvent(pMsg);		
	return CButton::PreTranslateMessage(pMsg);
}

void CPicButton::SetIcon(int nIconInId, int nIconOutId)
{
	BITMAP bm;
	int btnH,btnW;
	m_hBmpIn->LoadBitmap(nIconInId);
	m_hBmpIn->GetBitmap(&bm);
	
	
	

	btnH=bm.bmHeight;
	btnW=bm.bmWidth;
	if(nIconOutId==NULL)
		m_hBmpOut=m_hBmpIn;
	else
	{
		m_hBmpOut->LoadBitmap(nIconOutId);
		m_hBmpOut->GetBitmap(&bm);
		if(bm.bmHeight<btnH) btnH=bm.bmHeight;		//按钮的高是两张图片中较矮的图片的高度
		if(bm.bmWidth<btnW) btnW=bm.bmWidth;
	}
	CRect rc;
	GetWindowRect(rc);
	MoveWindow(rc.left,rc.top,btnW,btnH);
	RedrawWindow();

	m_IconLoaded = true;
}

void CPicButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bTracking)
    {
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(tme);
        tme.hwndTrack = m_hWnd;
        tme.dwFlags = TME_LEAVE|TME_HOVER;
        tme.dwHoverTime = 1;
        m_bTracking = _TrackMouseEvent(&tme);
    }
	
	CButton::OnMouseMove(nFlags, point);
}


BOOL CPicButton::SetBtnCursor(int nCursorId)
{
	// Destroy any previous cursor
	if (m_hCursor != NULL) ::DestroyCursor(m_hCursor);
	m_hCursor = NULL;

	// If we want a cursor
	if (nCursorId != -1)
	{
		m_hCursor=AfxGetApp()->LoadCursor(nCursorId);
		if (m_hCursor == NULL) return FALSE;
	}
	return TRUE;
}

BOOL CPicButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	/*if (m_hCursor != NULL)
	{
		::SetCursor(m_hCursor);
		return TRUE;
	}*/
	return CButton::OnSetCursor(pWnd, nHitTest, message);
}

LRESULT CPicButton::OnMouseHover(WPARAM wparam, LPARAM lparam) 
{
	// TODO: Add your message handler code here and/or call default
	m_MouseOnButton=TRUE;
	Invalidate();
	return 0;
}


LRESULT CPicButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking = FALSE;
	m_MouseOnButton=FALSE;
	Invalidate();
	return 0;
}

void CPicButton::InitToolTip()
{
	if (m_ToolTip.m_hWnd == NULL)
	{
		// Create ToolTip control
		m_ToolTip.Create(this);
		// Create inactive
		m_ToolTip.Activate(FALSE);
	}
}

// Set the tooltip with a string resource
void CPicButton::SetToolTipText(int nId, BOOL bActivate)
{
	CString sText;

	// load string resource
	sText.LoadString(nId);
	// If string resource is not empty
	if (sText.IsEmpty() == FALSE) SetToolTipText(&sText, bActivate);

}

// Set the tooltip with a CString
void CPicButton::SetToolTipText(CString *spText, BOOL bActivate)
{
	// We cannot accept NULL pointer
	if (spText == NULL) return;

	// Initialize ToolTip
	InitToolTip();

	// If there is no tooltip defined then add it
	if (m_ToolTip.GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip.AddTool(this, (LPCTSTR)*spText, rectBtn, 1);
	}

	// Set text for tooltip
	m_ToolTip.UpdateTipText((LPCTSTR)*spText, this, 1);
	m_ToolTip.Activate(bActivate);
}

void CPicButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CButton::OnLButtonDown(nFlags, point);
}
