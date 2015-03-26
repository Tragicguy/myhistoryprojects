#include "StdAfx.h"
#include "UIHelper.h"


CUIHelper::CUIHelper(UINT nID)
{
	m_nID = nID;
	m_pPicture.Load(nID, _T("GIFMAP"));
}

CUIHelper::~CUIHelper(void)
{
	m_pPicture.Free();

	if (bmpMainUI.GetSafeHandle() != NULL)
	{
		bmpMainUI.DeleteObject();
	}
}

void CUIHelper::SetBackPicture(CDC* pDC, CRect rc)
{
	//Draw the background picture
	CDC dcMemory;

	dcMemory.CreateCompatibleDC(pDC);

	if (bmpMainUI.GetSafeHandle() != NULL)
	{
		bmpMainUI.DeleteObject();
	}

	bmpMainUI.CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());
	CBitmap *pOldBmp = dcMemory.SelectObject(&bmpMainUI);

	m_pPicture.Render(&dcMemory, rc);

	pDC->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &dcMemory, rc.left, rc.top, SRCCOPY);
	dcMemory.SelectObject(&pOldBmp);
	dcMemory.DeleteDC();

	return;
}
