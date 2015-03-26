#pragma once

class CSplitterWndEx : public CSplitterWnd
{
public:
        CSplitterWndEx();          
        DECLARE_DYNCREATE(CSplitterWndEx)
                
public:
        ~CSplitterWndEx();
protected:
        //afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
        //afx_msg void OnMouseMove(UINT,CPoint);
        void OnDrawSplitter(CDC *pDC, ESplitType nType, const CRect &rectArg);
        void OnInvertTracker(CRect &rect) ;
        BOOL PreCreateWindow(CREATESTRUCT& cs);

        void  TrackRowSize(int y,  int row) ;
        void  TrackColumnSize(int x, int   col); 
        
protected:
        HBRUSH m_hbr;   //背景刷子
        CString m_clsName;  //窗口类名
        DECLARE_MESSAGE_MAP()
};