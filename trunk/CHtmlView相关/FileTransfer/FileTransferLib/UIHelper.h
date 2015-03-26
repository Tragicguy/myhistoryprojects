#pragma once
#include "Picture.h"

class CUIHelper
{
public:
	CUIHelper(UINT nID);
	~CUIHelper(void);

public:
	void SetBackPicture(CDC* pDC,CRect rc);

private:
	CPicture m_pPicture;
	UINT	m_nID;
	CBitmap bmpMainUI;
};
