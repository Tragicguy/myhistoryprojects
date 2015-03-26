#pragma once
#include "Callback.h"
#include "WininetHook.h"
#include <string>
using namespace std;
typedef struct _BufferStr
{
	HINTERNET lHandle;
	LPVOID pBuffer;
	_BufferStr()
	{
		lHandle = 0;
		pBuffer = NULL;
	}
}BufferStr;
typedef struct _tagInternetConnectInfo
{
	HINTERNET lHandle;
	CString strHost;
	CString strPort;
} InternetConnectInfo, *LPInternetConnectInfo;


typedef struct _RequestStr
{
	HINTERNET hConnect;
	HINTERNET hRequest;
	CString strHost;
	CString strPort;
	CString strContent;	//返回的信息
	CString strType;
	CString strPostData;
	CString strObject;
	CString strHeader;
	CString strResponseHeader;
	CString strCookie;
}RequestStr;

class CWininetHookHelp
{
public:
	Callback<void,BufferStr &>	m_OnNewReceived;
	Callback<void,HINTERNET>	m_OnCloseHandle;
	Callback<void,InternetConnectInfo &>	m_OnInternetConnect;
	Callback<void,RequestStr &>	m_OnOpenRequest;
};

extern CWininetHookHelp g_HookHelp;