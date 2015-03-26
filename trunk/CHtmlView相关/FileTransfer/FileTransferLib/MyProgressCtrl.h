#pragma once

//add by pan peng 2008-12-19
class CMyMemDC : public CDC
{
	CRect m_rect;
	CBitmap m_bm;
	CBitmap *m_pbm;
	CDC *m_pSource;
public:
	CMyMemDC(CDC *pDC, CRect *pRect): CDC()
	{
		ASSERT(pDC);
		m_pSource = pDC;
		m_pbm = NULL;
		if (NULL == pRect) 
			pDC->GetClipBox(&m_rect);
		else 
			m_rect = *pRect;
		if(!pDC->IsPrinting())
		{
			CreateCompatibleDC(pDC);
			pDC->LPtoDP(&m_rect);

			m_bm.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
			m_pbm = SelectObject(&m_bm);

			SetMapMode(pDC->GetMapMode());

			SetWindowExt(pDC->GetWindowExt());
			SetViewportExt(pDC->GetViewportExt());

			pDC->DPtoLP(&m_rect);
			SetWindowOrg(m_rect.left, m_rect.top);
		}
		else
		{
			m_bPrinting = pDC->m_bPrinting;
			m_hDC       = pDC->m_hDC;
			m_hAttribDC = pDC->m_hAttribDC;
		}
		BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
			m_pSource, m_rect.left, m_rect.top, SRCCOPY);
		
	}
	~CMyMemDC()
	{
		if (m_pSource->IsPrinting()) 
		{
			m_hDC = m_hAttribDC = NULL;
			return;
		}
		m_pSource->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
				this, m_rect.left, m_rect.top, SRCCOPY);			
		SelectObject(m_pbm);		
	}

	CMyMemDC* operator->() { return this; }	
	operator CMyMemDC*() { return this;	}

};
// CMyProgressCtrl

class CMyProgressCtrl : public CProgressCtrl
{
	DECLARE_DYNAMIC(CMyProgressCtrl)

public:
	CMyProgressCtrl();
	virtual ~CMyProgressCtrl();
	int SetPos(int nPos);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


