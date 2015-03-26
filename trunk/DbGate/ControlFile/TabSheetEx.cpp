// TabSheet.cpp : 实现文件
//

#include "stdafx.h"
#include "TabSheetEx.h"


// CTabSheetEx

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabSheetEx

CTabSheetEx::CTabSheetEx()
{
        m_crSelColour     = RGB( 0, 0, 255 );
        m_crUnselColour   = RGB( 0, 0, 0 );
        //m_crUnselColour   = RGB( 160, 100, 40);//黄褐色
        //m_crUnselColour   = RGB( 21, 150, 32 );//深绿色

        //m_crUnselColour   = RGB(0,153,204); 
        //m_crUnselColour   = RGB(0,153,204);
}

CTabSheetEx::~CTabSheetEx()
{
}


BEGIN_MESSAGE_MAP(CTabSheetEx, CTabCtrl)
END_MESSAGE_MAP()



/************************************************************************/
/*                                                                      */
/************************************************************************/

void CTabSheetEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
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
void CTabSheetEx::PreSubclassWindow()
{
        CTabCtrl::PreSubclassWindow();
        ModifyStyle(0, TCS_OWNERDRAWFIXED);
}
