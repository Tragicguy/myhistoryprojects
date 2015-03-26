#pragma once


// CTabSheet

/////////////////////////////////////////////////////////////////////////////
// CTabSheet window
#define MAXPAGE 16

class CTabSheet : public CTabCtrl
{
        // Construction
public:
        CTabSheet();

        // Attributes
public:

        // Operations
public:

        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CTabSheet)
        //}}AFX_VIRTUAL

        // Implementation
public:
        int GetCurSel();
        int SetCurSel(int nItem);
        void Show();
        void SetRect();
        BOOL AddPage(LPCTSTR title, CDialog *pDialog, UINT ID);
        virtual ~CTabSheet();

        // Generated message map functions
protected:
        LPCTSTR m_Title[MAXPAGE];
        UINT m_IDD[MAXPAGE];
        CDialog* m_pPages[MAXPAGE];
        int m_nNumOfPages;
        int m_nCurrentPage;
        //{{AFX_MSG(CTabSheet)
        afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnSize(UINT nType, int cx, int cy);
        afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
public:
        COLORREF m_crSelColour;
        COLORREF m_crUnselColour;

        virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
public:

        
protected:
        virtual void PreSubclassWindow();
};


