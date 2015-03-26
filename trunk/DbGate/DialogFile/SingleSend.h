#pragma once


// CSingleSend 对话框

class CSingleSend : public CDialog
{
	DECLARE_DYNAMIC(CSingleSend)

public:
	CSingleSend(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSingleSend();

// 对话框数据
	enum { IDD = IDD_DIALOG_SINGLESEND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedBtnSend();
public:
	CString m_Content;
public:
	CString m_strPhones;
public:
	CString m_strPhone;
};
