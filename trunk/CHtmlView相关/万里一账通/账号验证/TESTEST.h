#pragma once


// TESTEST 对话框

class TESTEST : public CDHtmlDialog
{
	DECLARE_DYNCREATE(TESTEST)

public:
	TESTEST(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~TESTEST();
// 重写
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// 对话框数据
	enum { IDD = IDD_DLG_HTMLDLG, IDH = IDR_HTML_TESTEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
};
