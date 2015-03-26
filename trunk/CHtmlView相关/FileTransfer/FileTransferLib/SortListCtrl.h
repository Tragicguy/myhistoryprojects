#pragma once


// CSortListCtrl

class CSortListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CSortListCtrl)

public:
	CSortListCtrl();
	virtual ~CSortListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_fAsc;		//是否顺序排序
	int m_nSortedCol;	//当前排序的列
};


