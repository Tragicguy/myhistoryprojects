#pragma once
#include "afxcmn.h"

// CMainBottomDialog 窗体视图
class CMainBottomDialog : public CFormView
{
	DECLARE_DYNCREATE(CMainBottomDialog)

protected:
	CMainBottomDialog();           // 动态创建所使用的受保护的构造函数
	virtual ~CMainBottomDialog();

public:
	enum { IDD = IDD_DLG_MAINBOTTOM };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void InitDispCtrList();
	void OnInitialUpdate();
	
};


