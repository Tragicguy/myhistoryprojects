// MyProgressCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "FileTransferLib.h"
#include "MyProgressCtrl.h"


// CMyProgressCtrl

IMPLEMENT_DYNAMIC(CMyProgressCtrl, CProgressCtrl)

CMyProgressCtrl::CMyProgressCtrl()
{

}

CMyProgressCtrl::~CMyProgressCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyProgressCtrl, CProgressCtrl)
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CMyProgressCtrl 消息处理程序

void CMyProgressCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	//CProgressCtrl::OnPaint();
	
	int nPos = this->GetPos();
	if (nPos != 0)
	{
		int nLower = 0;
		int nUpper = 0;
		this->GetRange(nLower, nUpper);
		if (nUpper - nLower <= 0)
			return;

		CString strPrgressText = _T("");
		if (nPos > 0)
		{
			int nProgress = (nPos*100)/(nUpper - nLower);
			strPrgressText.Format(_T("%d"), nProgress);
			strPrgressText += _T("%");
		}

		CRect rect;
		this->GetClientRect(&rect);

		CDC *pOrgDC = this->GetDC();
		if (pOrgDC == NULL)
			return;

		CMyMemDC memDC(pOrgDC, &rect);
		CDC *pDC = &memDC;

		int nMode = pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->DrawText(strPrgressText, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		pDC->SetBkMode(nMode);
		ReleaseDC(pDC);

	}
}


int CMyProgressCtrl::SetPos(int nPos)
{
	int n = CProgressCtrl::SetPos(nPos);
	return n;
}