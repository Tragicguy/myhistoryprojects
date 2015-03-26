#pragma once
#include "afxwin.h"
#include "UIHelper.h"
#include "PictureButton\PicButton.h"

// CFolderNameBuilder 对话框

class CFolderNameBuilder : public CDialog
{
	DECLARE_DYNAMIC(CFolderNameBuilder)

public:
	CFolderNameBuilder(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFolderNameBuilder();

	CString GetFolderName();

// 对话框数据
	enum { IDD = IDD_DIALOG_NAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_FolderName;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	CEdit m_EditName;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	CPicButton m_btOK;
	CPicButton m_btCancel;
	CPicButton m_btCancel2;

	virtual BOOL OnInitDialog();
	void SetDlgReign();
	afx_msg void OnBnClickedCancel2();
	afx_msg LRESULT OnNcHitTest(CPoint point);
};
