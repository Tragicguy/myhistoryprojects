////////////////////////////////////////////////////////////////
// MSDN Magazine -- October 2004
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Compiles with Visual Studio .NET 2003 on Windows XP. Tab size=3.
//
#include "stdafx.h"
#include "DragDrop.h"
#include <afxpriv.h> // for AfxLoadString

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT WM_DD_DRAGENTER = RegisterWindowMessage(_T("WM_DD_DRAGENTER"));
const UINT WM_DD_DRAGOVER  = RegisterWindowMessage(_T("WM_DD_DRAGOVER"));
const UINT WM_DD_DRAGDROP  = RegisterWindowMessage(_T("WM_DD_DRAGDROP"));
const UINT WM_DD_DRAGABORT = RegisterWindowMessage(_T("WM_DD_DRAGABORT"));

// Macro to get point from WM_ mouse messages
#define GETPOINT(lp) (CPoint(GET_X_LPARAM(lp), GET_Y_LPARAM(lp)))

CDragDropMgr::CDragDropMgr()
{
	m_pMainWnd = NULL;
	m_hwndTracking = NULL;
	m_hCursorSave = NULL;
	m_pDragImage = NULL;
	m_iState = 0;
	SetCursors(LoadCursor(NULL, IDC_ARROW), LoadCursor(NULL, IDC_NO));
	memset(&m_info,0,sizeof(m_info));
}

CDragDropMgr::~CDragDropMgr()
{
}

BOOL CDragDropMgr::Install(CWnd *pMainWnd, DRAGDROPWND* ddwnds)
{
	m_pMainWnd = pMainWnd;
	for (int i=0; ddwnds[i].type; i++) {
		HWND hwnd = ::GetDlgItem(pMainWnd->m_hWnd, ddwnds[i].id);
		ASSERT(hwnd && ::IsWindow(hwnd));
		m_mapHwnd[hwnd] = ddwnds[i].type;
	}
#ifdef _DEBUG
	HWNDMAP::iterator it;
	for (it=m_mapHwnd.begin(); it!=m_mapHwnd.end(); it++) {
		HWNDMAPENTRY pair = *it;
		TRACE("hwnd: %p = %04x\n", pair.first, pair.second);
	}
#endif
	return TRUE;
}

void CDragDropMgr::AddWindow(HWND hwnd, int type)
{
	m_mapHwnd[hwnd] = type;
}

void CDragDropMgr::RemoveWindow(HWND hwnd)
{
	HWNDMAP::iterator pos = m_mapHwnd.find(hwnd);
	if (pos != m_mapHwnd.end()) {
		m_mapHwnd.erase(pos);
	}
}

UINT CDragDropMgr::GetWindowType(HWND hwnd)
{
	HWNDMAP::const_iterator pos = m_mapHwnd.find(hwnd);
	return pos != m_mapHwnd.end() ? pos->second : 0;
}

//////////////////
// detail: Note that this works for input only
//
BOOL CDragDropMgr::PreTranslateMessage(MSG* pMsg)
{
	const MSG& msg = *pMsg;

	if (IsSourceWnd(msg.hwnd)) {
		if (msg.message==WM_LBUTTONUP) {
			return OnLButtonUp(msg);

		} 
		
		else if (msg.message==WM_LBUTTONDOWN) {
			return OnLButtonDown(msg);

		} 
		else if (msg.message==WM_MOUSEMOVE) {
			return OnMouseMove(msg);

		} 
		else if (msg.message==WM_KEYDOWN && msg.wParam==VK_ESCAPE) {
			if (m_iState) {
				m_pMainWnd->SendMessage(WM_DD_DRAGABORT, 0, NULL);
				SetState(NONE);
				delete m_info.data;
				m_info.data=NULL;
				return 1;
			}
		}
	}
	return FALSE;
}

//////////////////
// Handle button-down message: enter CAPTURED state.
//
BOOL CDragDropMgr::OnLButtonDown(const MSG& msg)
{
	::SendMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
	m_hwndTracking = msg.hwnd;
	m_ptOrg = GETPOINT(msg.lParam);
	SetState(CAPTURED);
	return TRUE;
}

//////////////////
// Handle mousemove: enter DRAGGING state.
//
BOOL CDragDropMgr::OnMouseMove(const MSG& msg)
{
	if (!m_iState)
		return FALSE;

	UINT nFlags = (UINT)msg.wParam;
	if (!(nFlags & MK_LBUTTON))
		return FALSE;

	CWnd* pWnd = CWnd::FromHandle(m_hwndTracking);
	CPoint pt = GETPOINT(msg.lParam);
	DRAGDROPINFO& dd = m_info;

	if (IsDragging()) {
		// already dragging: move drag image
		pWnd->ClientToScreen(&pt);			 // convert to screen coords
		m_pDragImage->DragMove(pt);		 // move drag image

		// get new target window if any and set cursor appropriately
		m_pMainWnd->ScreenToClient(&pt); // convert to main window coords
		dd.pt = pt;
		dd.hwndTarget = m_pMainWnd->ChildWindowFromPoint(pt)->GetSafeHwnd();
		m_pMainWnd->SendMessage(WM_DD_DRAGOVER, 0, (LPARAM)(void*)&dd);
		SetCursor(dd.hwndTarget && IsTargetWnd(dd.hwndTarget) ?
			m_hCursorDrop : m_hCursorNo);

	} else {
		// Not dragging yet: enter drag mode if mouse moves beyond threshhold.
		CPoint delta = pt - m_ptOrg;
		static CPoint jog(GetSystemMetrics(SM_CXDRAG),
			GetSystemMetrics(SM_CYDRAG));

		if (abs(delta.x)>=jog.x || abs(delta.y)>jog.y) {
			dd.hwndSource = m_hwndTracking;
			dd.pt = m_ptOrg;	// start from ORIGINAL point, not where now
			dd.hwndTarget = NULL;
			dd.data = NULL;

			// Send main window a message: enter drag mode. 
			BOOL bDrag = (BOOL)m_pMainWnd->SendMessage(WM_DD_DRAGENTER,
				::GetDlgCtrlID(m_hwndTracking), (LPARAM)(void*)&dd);

			if (bDrag && dd.data) {
				SetState(DRAGGING);			 // I am now dragging
				OnMouseMove(msg);
				pWnd->ClientToScreen(&pt);
				CRect rc;
				m_pDragImage = dd.data->CreateDragImage(pWnd, rc);
				m_pDragImage->BeginDrag(0, rc.BottomRight());
				m_pDragImage->DragEnter(NULL,pt);

			} else {
				SetState(NONE);
			}
		}
	}
	return TRUE;
}

//////////////////
// Handle button-up: drop the data and return to home state (NONE).
//
BOOL CDragDropMgr::OnLButtonUp(const MSG& msg)
{
	if (!IsDragging()) {
		SetState(NONE); 
		return FALSE;
	}
	DRAGDROPINFO& dd = m_info;
	if (IsTargetWnd(dd.hwndTarget)) {
		CPoint pt = GETPOINT(msg.lParam);
		CWnd* pWndSource = CWnd::FromHandle(dd.hwndSource);
		CWnd* pWndTarget = CWnd::FromHandle(dd.hwndTarget);
		pWndSource->ClientToScreen(&pt);
		pWndTarget->ScreenToClient(&pt);
		dd.pt = pt;
		m_pMainWnd->SendMessage(WM_DD_DRAGDROP, 
			pWndTarget->GetDlgCtrlID(), (LPARAM)(void*)&dd);
	} else {
		m_pMainWnd->SendMessage(WM_DD_DRAGABORT, 0, 0);
	}
	delete m_info.data;
	m_info.data=NULL;
	SetState(NONE);
	return TRUE;
}

//////////////////
// Change state for finite-state-machine.
//
void CDragDropMgr::SetState(UINT iState)
{
	TRACE(_T("CDragDropMgr::SetState %d\n"),iState);
	if (iState!=m_iState) {
		if (iState==CAPTURED) {
			::SetCapture(m_hwndTracking);	 // capture mouse input

		} else if (iState==DRAGGING) {
			m_hCursorSave = GetCursor();	 // save current cursor
		
		} else if (iState==NONE) {
			::ReleaseCapture();				 // release capture and..
			SetCursor(m_hCursorSave);		 // ..restore cursor
			if (m_pDragImage) {
				m_pDragImage->EndDrag();	 // end drawing and..
				delete m_pDragImage;			 // ..destroy..
				m_pDragImage=NULL;			 // ..image list
			}
			m_hwndTracking = NULL;
		}
		m_iState = iState;
	}
}

//////////////////
// Create the drag image: create an image list and call virtual darw function
// to draw the data into the image list. Will then use this during dragging.
//
CImageList* CDragDropData::CreateDragImage(CWnd* pWnd, CRect& rc)
{
	const COLORREF BGCOLOR = GetSysColor(COLOR_3DLIGHT);

	// create memory dc compatible w/source window
	CWindowDC dcWin(pWnd);
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dcWin);

	// use same font as source window
	CFont* pFont = pWnd->GetFont();
	CFont* pOldFont = dcMem.SelectObject(pFont);

	// get size of drag image
	CSize sz = OnGetDragSize(dcMem); // call virtual fn to get size
	rc = CRect(CPoint(0,0), sz);

	// create image list: create bitmap and draw into it
	m_bitmap.CreateCompatibleBitmap(&dcWin, sz.cx, sz.cy);
	CBitmap* pOldBitmap = dcMem.SelectObject(&m_bitmap);
	CBrush brush;
	brush.CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));
	dcMem.FillRect(&rc,&brush);
	dcMem.SetBkMode(TRANSPARENT);
	dcMem.SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
	OnDrawData(dcMem, rc); // call virtual fn to draw
	dcMem.SelectObject(pOldFont);
	dcMem.SelectObject(pOldBitmap);

	// create image list and add bitmap to it
	CImageList *pil = new CImageList();
	pil->Create(sz.cx, sz.cy, ILC_COLOR24|ILC_MASK, 0, 1);
	pil->Add(&m_bitmap, BGCOLOR);
	return pil;
}

//////////////////
// Get draw size for text: use DrawText with DT_CALCRECT.
//
CSize CDragDropText::OnGetDragSize(CDC& dc)
{
	CRect rc(0,0,0,0);
	dc.DrawText(m_text, &rc, DT_CALCRECT);
	if (rc.right>MAXWIDTH)
		rc.right = MAXWIDTH;
	return rc.Size();
}

//////////////////
// Call MFC/Windows to draw text.
//
void CDragDropText::OnDrawData(CDC& dc, CRect& rc)
{
	dc.DrawText(m_text, &rc, DT_LEFT|DT_END_ELLIPSIS);
}
