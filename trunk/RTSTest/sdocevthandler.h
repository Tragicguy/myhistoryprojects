#pragma once
#include <mshtml.h>

#include "Callback.h"

#pragma warning(disable:4192)
#pragma warning(disable:4278)

#import <mshtml.tlb>

// SDocEvtHandler ÃüÁîÄ¿±ê
class CWeb1800HtmlView;
class SDocEvtHandler : public CCmdTarget
{
	friend class CWeb1800HtmlView;

	DECLARE_DYNAMIC(SDocEvtHandler)

	Callback<BOOL, MSHTML::IHTMLEventObjPtr>*	m_pOnClickCallback;
public:
	SDocEvtHandler();
	virtual ~SDocEvtHandler();

	void OnClick(MSHTML::IHTMLEventObjPtr pEvtObj);

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};


