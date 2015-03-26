// TESTEST.cpp : 实现文件
//

#include "stdafx.h"
#include "TESTEST.h"


// TESTEST 对话框

IMPLEMENT_DYNCREATE(TESTEST, CDHtmlDialog)

TESTEST::TESTEST(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(TESTEST::IDD, TESTEST::IDH, pParent)
{
}

TESTEST::~TESTEST()
{
}

void TESTEST::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL TESTEST::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BEGIN_MESSAGE_MAP(TESTEST, CDHtmlDialog)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(TESTEST)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// TESTEST 消息处理程序

HRESULT TESTEST::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;  // 除非将焦点设置到控件，否则返回 TRUE
}

HRESULT TESTEST::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;  // 除非将焦点设置到控件，否则返回 TRUE
}
