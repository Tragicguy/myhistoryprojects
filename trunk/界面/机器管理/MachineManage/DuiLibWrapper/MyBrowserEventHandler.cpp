#include "stdafx.h"
#include "MyBrowserEventHandler.h"
#include <afxwin.h>
#include <Shlwapi.h>
CMyBrowserEventHandler::CMyBrowserEventHandler(void)
{
	m_bShowContextMenu = true;
}

CMyBrowserEventHandler::~CMyBrowserEventHandler(void)
{
}


HRESULT STDMETHODCALLTYPE CMyBrowserEventHandler::GetExternal(/* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDispatch, IDispatch* pDefaultDispatch)
{
	*ppDispatch = pDefaultDispatch;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CMyBrowserEventHandler::GetHostInfo(/* [out][in] */ DOCHOSTUIINFO __RPC_FAR *pInfo)
{
	if (pInfo != NULL)
	{
		pInfo->dwFlags |= DOCHOSTUIFLAG_NO3DBORDER | DOCHOSTUIFLAG_THEME;
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CMyBrowserEventHandler::GetIDsOfNames( __RPC__in REFIID riid, __RPC__in_ecount_full(cNames ) LPOLESTR *rgszNames, UINT cNames, LCID lcid, __RPC__out_ecount_full(cNames) DISPID *rgDispId)
{
	//网页调用window.external.CppCall时，会调用这个方法获取CppCall的ID
	map<wstring, int>::iterator it = m_mapInvokeFromWebMapString.find(rgszNames[0]);
	if (it != m_mapInvokeFromWebMapString.end())
	{
		*rgDispId = it->second;
		return S_OK;
	}
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CMyBrowserEventHandler::Invoke( DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr )
{
	map<int, wstring>::iterator it = m_mapInvokeFromWebMapInt.find((int)dispIdMember);
	if (it != m_mapInvokeFromWebMapInt.end())
	{
		if (m_OnInvokeFromWebCallBack.IsEnableCallback())
		{
			m_OnInvokeFromWebCallBack.call(it->second, dispIdMember, pDispParams, pVarResult, NULL);
		}
		return S_OK;
	}
	return DISP_E_MEMBERNOTFOUND;
}



HRESULT STDMETHODCALLTYPE CMyBrowserEventHandler::ShowContextMenu(/* [in] */ DWORD dwID,/* [in] */ POINT __RPC_FAR *ppt,/* [in] */ IUnknown __RPC_FAR *pcmdtReserved,/* [in] */ IDispatch __RPC_FAR *pdispReserved)
{
	//return E_NOTIMPL;
	//返回 E_NOTIMPL 正常弹出系统右键菜单	
	//返回S_OK 则可屏蔽系统右键菜单
	//if(dwID == CONTEXT_MENU_TEXTSELECT || dwID == CONTEXT_MENU_CONTROL)
	//	return E_NOTIMPL;
	return m_bShowContextMenu ? E_NOTIMPL : S_OK;
}