#pragma once


// CTabSheet

/////////////////////////////////////////////////////////////////////////////
// CTabSheet window
#define MAXPAGE 16

class CTabSheetEx : public CTabCtrl
{
        // Construction
public:
        CTabSheetEx();

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
      //  int GetCurSel();
       // int SetCurSel(int nItem);
       // void Show();
     //   void SetRect();
      //  BOOL AddPage(LPCTSTR title, CDialog *pDialog, UINT ID);
        virtual ~CTabSheetEx();

        // Generated message map functions
protected:
      //  LPCTSTR m_Title[MAXPAGE];
      //  UINT m_IDD[MAXPAGE];
       // CDialog* m_pPages[MAXPAGE];
       // int m_nNumOfPages;
       // int m_nCurrentPage;
        //{{AFX_MSG(CTabSheet)

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


