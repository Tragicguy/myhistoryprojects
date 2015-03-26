// TabSheet.cpp : 实现文件
//

#include "stdafx.h"
#include "TabSheet.h"
#include ".\tabsheet.h"


// CTabSheet

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabSheet

CTabSheet::CTabSheet()
{
        m_nNumOfPages = 0;
        m_nCurrentPage = 0;
        m_crSelColour     = RGB( 0, 0, 255 );
        m_crUnselColour   = RGB( 0, 0, 0 );
        //m_crUnselColour   = RGB( 160, 100, 40);//黄褐色
        //m_crUnselColour   = RGB( 21, 150, 32 );//深绿色

        //m_crUnselColour   = RGB(0,153,204); 
        //m_crUnselColour   = RGB(0,153,204);
}

CTabSheet::~CTabSheet()
{
}


BEGIN_MESSAGE_MAP(CTabSheet, CTabCtrl)
        //{{AFX_MSG_MAP(CTabSheet)
        ON_WM_LBUTTONDOWN()
        ON_WM_SIZE()
        //}}AFX_MSG_MAP
        ON_WM_CREATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabSheet message handlers
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CTabSheet::AddPage(LPCTSTR title, CDialog *pDialog,UINT ID)
{
        if( MAXPAGE == m_nNumOfPages )
                return FALSE;

        m_nNumOfPages++;

        m_pPages[m_nNumOfPages-1] = pDialog;
        m_IDD[m_nNumOfPages-1] = ID;
        m_Title[m_nNumOfPages-1] = title;

        return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTabSheet::SetRect()
{
        CRect tabRect, itemRect;
        int nX;
        int nY;
        int nXc;
        int nYc;

        if (m_nNumOfPages < 1 ) 
        {
                return ;
        }
        GetClientRect(&tabRect);
        GetItemRect(0, &itemRect);

        nX=itemRect.left;
        nY=itemRect.bottom+1;
        nXc=tabRect.right-itemRect.left-2;
        nYc=tabRect.bottom-nY-2;

        //m_pPages[0]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
        for( int nCount=0; nCount < m_nNumOfPages; nCount++ )
        {
                m_pPages[nCount]->SetWindowPos( &wndTop, nX, nY, nXc, nYc, 
                                                SWP_HIDEWINDOW );
        }
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTabSheet::Show()
{
	int i=0;
	for(i=0; i < m_nNumOfPages; i++)
	{
		m_pPages[i]->Create( m_IDD[i], this );
		InsertItem( i, m_Title[i] );
	}
	SetRect();
	m_pPages[0]->ShowWindow(SW_SHOW);
	for( i=1; i < m_nNumOfPages; i++)
	{
		m_pPages[i]->ShowWindow(SW_HIDE);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTabSheet::OnLButtonDown(UINT nFlags, CPoint point) 
{
        CTabCtrl::OnLButtonDown(nFlags, point);

        if(m_nCurrentPage != GetCurFocus())
        {
                m_pPages[m_nCurrentPage]->ShowWindow(SW_HIDE);
                m_nCurrentPage=GetCurFocus();
                m_pPages[m_nCurrentPage]->ShowWindow(SW_SHOW);
                //		m_pPages[m_nCurrentPage]->SetFocus();
        }
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CTabSheet::SetCurSel(int nItem)
{
        if( nItem < 0 || nItem >= m_nNumOfPages)
                return -1;

        int ret = m_nCurrentPage;

        if(m_nCurrentPage != nItem )
        {
                m_pPages[m_nCurrentPage]->ShowWindow(SW_HIDE);
                m_nCurrentPage = nItem;
                m_pPages[m_nCurrentPage]->ShowWindow(SW_SHOW);
                //		m_pPages[m_nCurrentPage]->SetFocus();
                CTabCtrl::SetCurSel(nItem);
        }

        return ret;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CTabSheet::GetCurSel()
{
        return CTabCtrl::GetCurSel();
}

/************************************************************************/
/*                                                                      */
/************************************************************************/

void CTabSheet::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{	
        CRect rect = lpDrawItemStruct->rcItem;
        int nTabIndex = lpDrawItemStruct->itemID;
        if (nTabIndex < 0) return;
        BOOL bSelected = (nTabIndex == GetCurSel());

        char label[64];
        TC_ITEM tci;
        tci.mask = TCIF_TEXT|TCIF_IMAGE;
        tci.pszText = label;     
        tci.cchTextMax = 63;    	
        if (!GetItem(nTabIndex, &tci )) return;

        CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
        if (!pDC)
        {
                return;
        }
        int nSavedDC = pDC->SaveDC();

        // For some bizarre reason the rcItem you get extends above the actual
        // drawing area. We have to workaround this "feature".
        rect.top += ::GetSystemMetrics(SM_CYEDGE);

        pDC->SetBkMode(TRANSPARENT);
        pDC->FillSolidRect(rect, ::GetSysColor(COLOR_BTNFACE));

        // Draw image
        /*
        CImageList* pImageList = GetImageList();
        if (pImageList && tci.iImage >= 0) 
        {
                rect.left += pDC->GetTextExtent(_T(" ")).cx;		// Margin
                // Get height of image so we 
                IMAGEINFO info;
                pImageList->GetImageInfo(tci.iImage, &info);
                CRect ImageRect(info.rcImage);
                int nYpos = rect.top;

                pImageList->Draw( pDC, tci.iImage, CPoint(rect.left, nYpos), 
                                  ILD_TRANSPARENT);
                rect.left += ImageRect.Width();
        }
        */

        if (bSelected) 
        {
                pDC->SetTextColor(m_crSelColour);
                //pDC->SelectObject(&m_SelFont);
                rect.top -= ::GetSystemMetrics(SM_CYEDGE);
                pDC->DrawText(label, rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
        } 
        else 
        {
                pDC->SetTextColor(m_crUnselColour);
                //pDC->SelectObject(&m_UnselFont);
                pDC->DrawText(label, rect, DT_SINGLELINE|DT_BOTTOM|DT_CENTER);
        }

        pDC->RestoreDC(nSavedDC);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTabSheet::OnSize(UINT nType, int cx, int cy) 
{
        CTabCtrl::OnSize(nType, cx, cy);
        int CurSel;
        int i;
        // TODO: Add your message handler code here
        SetRect();
        CurSel = GetCurSel();

        for( i=0; i < m_nNumOfPages; i++)
        {
                if( CurSel == i  ) 
                {
                        m_pPages[i]->ShowWindow(SW_SHOW);
                }
                else
                {
                        m_pPages[i]->ShowWindow(SW_HIDE);
                }
        }

        SetRedraw(TRUE );

}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CTabSheet::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
        if (CTabCtrl::OnCreate(lpCreateStruct) == -1)
        {                
                return -1;
        }
        ModifyStyle(0, TCS_OWNERDRAWFIXED);
        return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTabSheet::PreSubclassWindow()
{
        CTabCtrl::PreSubclassWindow();
        ModifyStyle(0, TCS_OWNERDRAWFIXED);
}
