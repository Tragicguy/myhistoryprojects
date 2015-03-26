
#include "stdafx.h"
#include "SplitterWndEx.h" 
IMPLEMENT_DYNCREATE(CSplitterWndEx, CSplitterWnd) 


/************************************************************************/
/*                                                                      */
/************************************************************************/
CSplitterWndEx::CSplitterWndEx()
{

        m_cxSplitter = 4;    //must >=4 ,拖动时拖动条的宽度
        m_cySplitter = 4;    
        m_cxBorderShare = 0; //按下鼠标时拖动条的偏移量
        m_cyBorderShare = 0;  
        m_cxSplitterGap= 1;  //splitter拖动条的宽度   
        m_cySplitterGap= 2;  

        /*
        m_cxSplitter = 4;    //must >=4 ,拖动splitter时拖动条的宽度
        m_cySplitter = 4;   
        m_cxBorderShare = -4;   //按下鼠标时splitter拖动条的偏移量
        m_cyBorderShare = -4; 
        m_cxSplitterGap= 16;    
        m_cySplitterGap= 16;     //splitter拖动条的宽度 
        
        m_hbr = ::CreateSolidBrush(RGB(98,98,98));
        m_clsName = "";
        */
} 

/************************************************************************/
/*                                                                      */
/************************************************************************/
CSplitterWndEx::~CSplitterWndEx()
{

} 

/************************************************************************/
/*                                                                      */
/************************************************************************/
BEGIN_MESSAGE_MAP(CSplitterWndEx, CSplitterWnd)
//ON_WM_LBUTTONDOWN()
//ON_WM_MOUSEMOVE()
//ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP() 

/*
void CSplitterWndEx::OnLButtonDown(UINT nFlags, CPoint point)
{
        //拖动splitter拖动条，能够拖动窗口
        ::PostMessage(AfxGetMainWnd()->GetSafeHwnd(),
                      WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
        CWnd::OnLButtonDown(nFlags,point);
} 

void CSplitterWndEx::OnMouseMove(UINT nFlags, CPoint point)
{      
        //跳过调用CSplitterWnd::OnMouseMove,因为他会引起光标的改变
        CWnd::OnMouseMove(nFlags, point);
} 
*/

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CSplitterWndEx::PreCreateWindow(CREATESTRUCT& cs)
{ 
        //窗口类名必须在类的域类有效，否则::CreateWindowEx得到的是无效的类名。
        //将会导致创建文档失败的错误。
        //m_clsName = AfxRegisterWndClass(0, ::LoadCursor (NULL,
         //       IDC_ARROW), ::CreateSolidBrush(RGB(255,95,17)));
        
        //cs.lpszClass = (const char* )m_clsName;
        return CSplitterWnd::PreCreateWindow(cs);
}

#define         CX_BORDER       1 
#define         CY_BORDER       1

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSplitterWndEx::OnDrawSplitter( CDC *pDC, ESplitType nType, 
                                     const CRect &rectArg)
{
        if(pDC==NULL) 
        { 
                RedrawWindow(rectArg,NULL,RDW_INVALIDATE|RDW_NOCHILDREN);
                return;
        } 
        ASSERT_VALID(pDC);
        CRect rc=rectArg;
        switch(nType) 
        { 
        case splitBorder:
                //重画分割窗口边界,使之为红色 
                pDC->Draw3dRect(rc,RGB(172,168,153),RGB(172,168,153));
                rc.InflateRect(-CX_BORDER,-CY_BORDER); 
                pDC->Draw3dRect(rc,RGB(172,168,153),RGB(172,168,153)); 
                return; 
        case splitBox:
                pDC->Draw3dRect(rc,RGB(0,0,0),RGB(0,0,0));
                rc.InflateRect(-CX_BORDER,-CY_BORDER); 
                pDC->Draw3dRect(rc,RGB(0,0,0),RGB(0,0,0));
                rc.InflateRect(-CX_BORDER,-CY_BORDER);
                pDC->FillSolidRect(rc,RGB(0,0,0)); 
                pDC->Draw3dRect(rc,RGB(0,0,0),RGB(0,0,0));
                return; 
        case splitBar: 
                //重画分割条，使之为绿色 
                pDC->FillSolidRect(rc,RGB(198,213,253));
                rc.InflateRect(-2,-2); 
                pDC->Draw3dRect(rc,RGB(172,168,153),RGB(172,168,153)); 
                return; 
        default: 
                ASSERT(FALSE); 
        } 
        pDC->FillSolidRect(rc,RGB(0,0,255));
} 

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSplitterWndEx::OnInvertTracker(CRect &rect) 
{ 
        ASSERT_VALID(this);
        ASSERT(!rect.IsRectEmpty()); 
        ASSERT((GetStyle()&WS_CLIPCHILDREN)==0);

        CRect rc=rect; 
        rc.InflateRect(1,1);
        CDC* pDC=GetDC(); 
        CBrush* pBrush=CDC::GetHalftoneBrush();
        HBRUSH hOldBrush=NULL;
   

        CRect   r;   
        GetClientRect(r);   
        if(pBrush   !=   NULL)   
        {
                hOldBrush = (HBRUSH)SelectObject(pDC->m_hDC, pBrush->m_hObject);   
        }
        if(rect.top-r.top < 100   &&   GetColumnCount()==1 )
        {   
                pDC->PatBlt( rect.left,   100,   rect.Width(),   
                             rect.Height(),   PATINVERT);   
        }
        else   if(r.bottom-rect.bottom<100   &&   GetColumnCount()==1)
        {   
                pDC->PatBlt( rect.left,   r.bottom-100,   
                             rect.Width(),   rect.Height(),   PATINVERT);   
        }
        else   if(rect.left-r.left<100   &&   GetRowCount()==1)
        {   
                pDC->PatBlt( 100,   r.top,   
                             rect.Width(),   rect.Height(),   PATINVERT);   
        }
        else   if( r.right-rect.right<100   &&   GetRowCount()==1 )
        {   
                pDC->PatBlt( r.right-100,   r.top,   
                               rect.Width(),   rect.Height(),   PATINVERT );   
        }
        else   
                pDC->PatBlt( rect.left,   rect.top,   
                              rect.Width(),   rect.Height(),   PATINVERT);   

        if   (hOldBrush   !=   NULL)   
                SelectObject(pDC->m_hDC,   hOldBrush);   
        ReleaseDC(pDC);   
}   
/************************************************************************/
/*                                                                      */
/************************************************************************/
void   CSplitterWndEx::TrackRowSize(int   y,   int   row)   
{   

        //CWnd * pWnd;
        CRect   clientRect;   
        CRect   rect;
        GetClientRect( clientRect );   

        if (  y < 100   &&   GetColumnCount()==1)
        {     
                y=100;   
        }
        else if( y >  clientRect.bottom - 100   &&   GetColumnCount()==1)
        {   
                y = clientRect.bottom-100;   
        }   
        CSplitterWnd::TrackRowSize( y, row); 

        //
        //nHeight = clientRect.Height() ;
        /*
        pWnd = GetPane( 0 , 0  );
        pWnd->GetClientRect( rect );
        nLeft = rect.left;
        nTop = rect.top;
        nWidth = rect.Width() ;
        nHeight = y ;     
        pWnd->MoveWindow(nLeft, nTop, nWidth, nHeight );
        pWnd->SetRedraw( );*/

        /*pWnd = GetPane( 1 , 0  );
        pWnd->GetClientRect( rect );
        nLeft = rect.left;
        nTop = rect.top;
        nWidth = rect.Width() ;
        nHeight = clientRect.bottom - y ;           
        pWnd->MoveWindow(nLeft, nTop, nWidth, nHeight);
        pWnd->SetRedraw( );*/
        return;



}   
/************************************************************************/
/*                                                                      */
/************************************************************************/
void   CSplitterWndEx::TrackColumnSize(int   x,   int   col)   
{   
        CRect   r;   
        GetClientRect(r);   

        if ( x < 100   &&   GetRowCount() == 1 )
        {     
                x=100;   
        }
        else if( x > r.right-100  && GetRowCount() == 1 )
        {   
                x=r.right-100;   
        }   
        CSplitterWnd::TrackColumnSize( x,  col );   
} 
/************************************************************************/
/*                                                                      */
/************************************************************************/
