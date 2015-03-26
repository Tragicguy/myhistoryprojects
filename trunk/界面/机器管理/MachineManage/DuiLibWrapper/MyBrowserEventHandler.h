#pragma once
#include "..\Duilib\Utils\WebBrowserEventHandler.h"
using namespace DuiLib;

#include "Callback.h"
#include <string>
#include <map>

using namespace std;
class CMyBrowserEventHandler : public CWebBrowserEventHandler
{
public:
	CMyBrowserEventHandler(void);
	~CMyBrowserEventHandler(void);

	/******************************************************************/
	virtual HRESULT STDMETHODCALLTYPE GetExternal(/* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDispatch, IDispatch* pDefaultDispatch);	
	virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames( __RPC__in REFIID riid, __RPC__in_ecount_full(cNames ) LPOLESTR *rgszNames, UINT cNames, LCID lcid, __RPC__out_ecount_full(cNames) DISPID *rgDispId);
	virtual HRESULT STDMETHODCALLTYPE Invoke( DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr);

	/********************************************************************/
	virtual HRESULT STDMETHODCALLTYPE GetHostInfo(/* [out][in] */ DOCHOSTUIINFO __RPC_FAR *pInfo);
	virtual HRESULT STDMETHODCALLTYPE ShowContextMenu(/* [in] */ DWORD dwID,/* [in] */ POINT __RPC_FAR *ppt,/* [in] */ IUnknown __RPC_FAR *pcmdtReserved,/* [in] */ IDispatch __RPC_FAR *pdispReserved);
	
	/********************************************************************/
	
public:
	void SetShowContextMenu(bool bShow){ m_bShowContextMenu = bShow; }
	void RegisterCallBackEvent(wstring funcname, int dispid)
	{
		m_mapInvokeFromWebMapString[funcname] = dispid;
		m_mapInvokeFromWebMapInt[dispid] = funcname;
	}
public:
    CallbackEx<BOOL, wstring, int, DISPPARAMS *, VARIANT *, LPVOID>		m_OnInvokeFromWebCallBack;
	
private:
	bool m_bShowContextMenu;
	map<int, wstring> m_mapInvokeFromWebMapInt;
	map<wstring, int> m_mapInvokeFromWebMapString;
};
