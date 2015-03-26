#include "StdAfx.h"
#include "WininetHook.h"

#include "MainFrm.h"

// initialize the static variables  
PFN_LdrLoadDll CWininetHook::s_pfnLdrLoadDll = NULL;  
PFN_LdrGetProcedureAddress CWininetHook::s_pfnLdrGetProcedureAddress = NULL;  
PFN_HttpSendRequestA CWininetHook::s_pfnHttpSendRequestA = HttpSendRequestA;  
PFN_HttpSendRequestW CWininetHook::s_pfnHttpSendRequestW = HttpSendRequestW;  
PFN_HttpAddRequestHeadersA CWininetHook::s_pfnHttpAddRequestHeadersA = HttpAddRequestHeadersA;  
PFN_HttpAddRequestHeadersW CWininetHook::s_pfnHttpAddRequestHeadersW = HttpAddRequestHeadersW;  
PFN_HttpSendRequestExA CWininetHook::s_pfnHttpSendRequestExA = HttpSendRequestExA;  
PFN_HttpSendRequestExW CWininetHook::s_pfnHttpSendRequestExW = HttpSendRequestExW;  
PFN_HttpEndRequestA CWininetHook::s_pfnHttpEndRequestA = HttpEndRequestA;  
PFN_HttpEndRequestW CWininetHook::s_pfnHttpEndRequestW = HttpEndRequestW;  
PFN_HttpOpenRequestA CWininetHook::s_pfnHttpOpenRequestA = HttpOpenRequestA;  
PFN_HttpOpenRequestW CWininetHook::s_pfnHttpOpenRequestW = HttpOpenRequestW;  
PFN_InternetConnectA CWininetHook::s_pfnInternetConnectA = InternetConnectA;  
PFN_InternetConnectW CWininetHook::s_pfnInternetConnectW = InternetConnectW;  
PFN_InternetOpenUrlA CWininetHook::s_pfnInternetOpenUrlA = InternetOpenUrlA;  
PFN_InternetOpenUrlW CWininetHook::s_pfnInternetOpenUrlW = InternetOpenUrlW;  
PFN_InternetOpenA CWininetHook::s_pfnInternetOpenA = InternetOpenA;
PFN_InternetOpenW CWininetHook::s_pfnInternetOpenW = InternetOpenW;
PFN_InternetCloseHandle CWininetHook::s_pfnInternetCloseHandle = InternetCloseHandle;
PFN_InternetSetCookieA CWininetHook::s_pfnInternetSetCookieA = InternetSetCookieA;
PFN_InternetSetCookieW CWininetHook::s_pfnInternetSetCookieW = InternetSetCookieW;
PFN_InternetSetCookieExA CWininetHook::s_pfnInternetSetCookieExA = InternetSetCookieExA;
PFN_InternetSetCookieExW CWininetHook::s_pfnInternetSetCookieExW = InternetSetCookieExW;

CWininetHook::CWininetHook(void)
{
	s_pfnLdrLoadDll = (PFN_LdrLoadDll)::GetProcAddress( ::GetModuleHandle(_T("NTDLL.DLL")), "LdrLoadDll");  
	s_pfnLdrGetProcedureAddress = (PFN_LdrGetProcedureAddress)::GetProcAddress( ::GetModuleHandle(_T("NTDLL.DLL")), "LdrGetProcedureAddress");  
	if( !s_pfnLdrLoadDll || !s_pfnLdrGetProcedureAddress )  
		return;  

	ReplaceIATEntryForAll( "NTDLL.DLL", s_pfnLdrLoadDll, &CWininetHook::_LdrLoadDll);  
	ReplaceIATEntryForAll( "NTDLL.DLL", s_pfnLdrGetProcedureAddress, &CWininetHook::_LdrGetProcedureAddress);  
	ReplaceIATEntryForAll( "WININET.DLL", s_pfnHttpSendRequestA, &CWininetHook::_HttpSendRequestA);  
	ReplaceIATEntryForAll( "WININET.DLL", s_pfnHttpSendRequestW, &CWininetHook::_HttpSendRequestW);  
	ReplaceIATEntryForAll( "WININET.DLL", s_pfnHttpAddRequestHeadersA, &CWininetHook::_HttpAddRequestHeadersA);  
	ReplaceIATEntryForAll( "WININET.DLL", s_pfnHttpAddRequestHeadersW, &CWininetHook::_HttpAddRequestHeadersW);  
	ReplaceIATEntryForAll( "WININET.DLL", s_pfnHttpSendRequestExA, &CWininetHook::_HttpSendRequestExA);  
	ReplaceIATEntryForAll( "WININET.DLL", s_pfnHttpSendRequestExW, &CWininetHook::_HttpSendRequestExW);  
	ReplaceIATEntryForAll( "WININET.DLL", s_pfnHttpEndRequestA, &CWininetHook::_HttpEndRequestA);  
	ReplaceIATEntryForAll( "WININET.DLL", s_pfnHttpSendRequestExW, &CWininetHook::_HttpSendRequestExW);  
	ReplaceIATEntryForAll( "WININET.DLL", s_pfnHttpEndRequestA, &CWininetHook::_HttpEndRequestA);  
	ReplaceIATEntryForAll( "WININET.DLL", s_pfnHttpEndRequestW, &CWininetHook::_HttpEndRequestW);  
	ReplaceIATEntryForAll( "WININET.DLL", s_pfnHttpOpenRequestA, &CWininetHook::_HttpOpenRequestA);  
	ReplaceIATEntryForAll( "WININET.DLL", s_pfnHttpOpenRequestW, &CWininetHook::_HttpOpenRequestW);  
	ReplaceIATEntryForAll( "WININET.DLL", s_pfnInternetConnectA, &CWininetHook::_InternetConnectA);  
	ReplaceIATEntryForAll( "WININET.DLL", s_pfnInternetConnectW, &CWininetHook::_InternetConnectW); 
	ReplaceIATEntryForAll( "WININET.DLL", s_pfnInternetOpenUrlA, &CWininetHook::_InternetOpenUrlA);  
	ReplaceIATEntryForAll( "WININET.DLL", s_pfnInternetOpenUrlW, &CWininetHook::_InternetOpenUrlW); 
	ReplaceIATEntryForAll( "WININET.DLL", s_pfnInternetOpenA, &CWininetHook::_InternetOpenA);  
	ReplaceIATEntryForAll( "WININET.DLL", s_pfnInternetOpenW, &CWininetHook::_InternetOpenW); 
	ReplaceIATEntryForAll( "WININET.DLL", s_pfnInternetCloseHandle, &CWininetHook::_InternetCloseHandle); 
	ReplaceIATEntryForAll( "WININET.DLL", s_pfnInternetSetCookieA, &CWininetHook::_InternetSetCookieA); 
	ReplaceIATEntryForAll( "WININET.DLL", s_pfnInternetSetCookieW, &CWininetHook::_InternetSetCookieW); 
	ReplaceIATEntryForAll( "WININET.DLL", s_pfnInternetSetCookieExA, &CWininetHook::_InternetSetCookieExA); 
	ReplaceIATEntryForAll( "WININET.DLL", s_pfnInternetSetCookieExW, &CWininetHook::_InternetSetCookieExW); 
}

CWininetHook::~CWininetHook(void)
{
}

static CWininetHook g_oHook;  


//////////////////////////////////////////////////////////////////////////
// Enumerate all the loaded modules in the current process, and search for the IAT to be processed
void CWininetHook::ReplaceIATEntryForAll(LPCSTR lpszDllName, LPVOID pfnCurrent, LPVOID pfnNew)  
{  
	HMODULE hMods[1024] = {0};  
	DWORD cbNeeded;  
	HANDLE hProcess = ::GetCurrentProcess();  
	if( ::EnumProcessModules( hProcess, hMods, sizeof(hMods), &cbNeeded))  
	{  
		for ( UINT i = 0; i < (cbNeeded / sizeof(HMODULE)); i++ )  
		{  
			ReplaceIATEntryInImageImportTable( hMods[i]  
			, lpszDllName  
				, pfnCurrent  
				, pfnNew  
				);  
		}  
	}  
}  
// search in IMAGE_IMPORT_DESCRIPTOR for the special module
BOOL CWininetHook::ReplaceIATEntryInImageImportTable( HANDLE hBaseAddress  
													 , LPCSTR lpszDllName  
													 , LPVOID pfnCurrent  
													 , LPVOID pfnNew  
													 )  
{  
	ASSERT(hBaseAddress && lpszDllName && pfnCurrent && pfnNew );  
	// retrieve IMAGE_IMPORT_DESCRIPTOR  
	DWORD dwSize = 0;  
	PIMAGE_SECTION_HEADER pFoundHeader = NULL;  
	PIMAGE_IMPORT_DESCRIPTOR pImgImportDescriptor  
		= (PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToDataEx( hBaseAddress  
		, TRUE  
		, IMAGE_DIRECTORY_ENTRY_IMPORT  
		, &dwSize  
		, &pFoundHeader  
		);  
	if( pImgImportDescriptor == NULL ){ return FALSE; }  
	while (pImgImportDescriptor->Name)  
	{  
		if ( _strcmpi((CHAR*)((PBYTE)hBaseAddress+pImgImportDescriptor->Name), lpszDllName) == 0 )  
		{  
			break; // found  
		}  
		++pImgImportDescriptor;  
	}  
	// NOTE:
	// If the special module can not be found in IMAGE_DIRECTORY_ENTRY_IMPORT
	// Then should try to search it in IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT
	if( !pImgImportDescriptor->Name )  
		return ReplaceIATEntryInDelayImageImportTable( hBaseAddress, lpszDllName, pfnCurrent, pfnNew);  

	// retrieve IAT  
	PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)(((LPBYTE)hBaseAddress) + pImgImportDescriptor->FirstThunk);  

	// enumerate functions in the IAT
	while(pThunk->u1.Function)    
	{    
		PDWORD lpAddr = (PDWORD)&(pThunk->u1.Function);    
		if(*lpAddr == (DWORD)pfnCurrent)    
		{  
			// modify the address
			::WriteProcessMemory(::GetCurrentProcess()  
				, lpAddr  
				, &pfnNew  
				, sizeof(DWORD)  
				, NULL  
				);  
			return TRUE;  
		}     
		pThunk++;    
	}  
	return FALSE;  
}  

// search in IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT for the special module
BOOL CWininetHook::ReplaceIATEntryInDelayImageImportTable( HANDLE hBaseAddress  
														  , LPCSTR lpszDllName  
														  , LPVOID pfnCurrent  
														  , LPVOID pfnNew  
														  )  
{  
	ASSERT(hBaseAddress && lpszDllName && pfnCurrent && pfnNew );  
	// retrieve IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT  
	DWORD dwSize = 0;  
	PIMAGE_SECTION_HEADER pFoundHeader = NULL;  
	PImgDelayDescr pImgDelayDescr  
		= (PImgDelayDescr)ImageDirectoryEntryToDataEx( hBaseAddress  
		, TRUE  
		, IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT  
		, &dwSize  
		, &pFoundHeader  
		);  
	if( pImgDelayDescr == NULL ){ return FALSE; }  
	while (pImgDelayDescr->rvaDLLName)  
	{  
		if ( _strcmpi((CHAR*)((PBYTE)hBaseAddress+pImgDelayDescr->rvaDLLName), lpszDllName) == 0 )  
		{  
			break;  
		}  
		++pImgDelayDescr;  
	}  
	// Not found in IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT
	if( !pImgDelayDescr->rvaDLLName )  
		return FALSE;  

	// retrieve IAT  
	PIMAGE_THUNK_DATA pThunk = NULL;  
	if( (pImgDelayDescr->grAttrs & dlattrRva) == 0 )  
		return FALSE;  
	pThunk = (PIMAGE_THUNK_DATA)(((LPBYTE)hBaseAddress) + pImgDelayDescr->rvaIAT); 

	// enumerate functions in the IAT
	while(pThunk->u1.Function)    
	{    
		PDWORD lpAddr = (PDWORD)&(pThunk->u1.Function);    
		if(*lpAddr == (DWORD)pfnCurrent)    
		{  
			// modify the address
			::WriteProcessMemory(::GetCurrentProcess()  
				, lpAddr  
				, &pfnNew  
				, sizeof(DWORD)  
				, NULL  
				);  
			return TRUE;
		}     
		pThunk++;    
	}

	return FALSE;  
}  

// Process those modules loaded dynamically, 
NTSTATUS WINAPI CWininetHook::_LdrLoadDll(IN PWCHAR PathToFile OPTIONAL, IN ULONG Flags OPTIONAL, IN PUNICODE_STRING ModuleFileName, OUT PHANDLE ModuleHandle)  
{     
	NTSTATUS ntStatus = s_pfnLdrLoadDll( PathToFile, Flags, ModuleFileName, ModuleHandle);  
	if( ntStatus == STATUS_SUCCESS && (Flags & LOAD_LIBRARY_AS_DATAFILE) == 0 )  
	{
#ifdef _DEBUG
		if( ModuleFileName->Length > 0 )
		{
			WCHAR wszPath[MAX_PATH] = {0};  
			memcpy_s( wszPath, MAX_PATH*sizeof(WCHAR), ModuleFileName->Buffer, MAX_PATH*sizeof(WCHAR));
			TRACE( L"LdrLoadDll %s\n", wszPath);
		}		
#endif				

		HANDLE hDll = *ModuleHandle;  
		ReplaceIATEntryInImageImportTable( hDll, "NTDLL.DLL", s_pfnLdrLoadDll, &CWininetHook::_LdrLoadDll);  
		ReplaceIATEntryInImageImportTable( hDll, "NTDLL.DLL", s_pfnLdrGetProcedureAddress, &CWininetHook::_LdrGetProcedureAddress);  
		ReplaceIATEntryInImageImportTable( hDll, "WININET.DLL", s_pfnHttpSendRequestA, &CWininetHook::_HttpSendRequestA);  
		ReplaceIATEntryInImageImportTable( hDll, "WININET.DLL", s_pfnHttpSendRequestW, &CWininetHook::_HttpSendRequestW);  
		ReplaceIATEntryInImageImportTable( hDll, "WININET.DLL", s_pfnHttpAddRequestHeadersA, &CWininetHook::_HttpAddRequestHeadersA);  
		ReplaceIATEntryInImageImportTable( hDll, "WININET.DLL", s_pfnHttpAddRequestHeadersW, &CWininetHook::_HttpAddRequestHeadersW);  
		ReplaceIATEntryInImageImportTable( hDll, "WININET.DLL", s_pfnHttpSendRequestExA, &CWininetHook::_HttpSendRequestExA);  
		ReplaceIATEntryInImageImportTable( hDll, "WININET.DLL", s_pfnHttpSendRequestExW, &CWininetHook::_HttpSendRequestExW);  
		ReplaceIATEntryInImageImportTable( hDll, "WININET.DLL", s_pfnHttpEndRequestA, &CWininetHook::_HttpEndRequestA);  
		ReplaceIATEntryInImageImportTable( hDll, "WININET.DLL", s_pfnHttpSendRequestExW, &CWininetHook::_HttpSendRequestExW);  
		ReplaceIATEntryInImageImportTable( hDll, "WININET.DLL", s_pfnHttpEndRequestA, &CWininetHook::_HttpEndRequestA);  
		ReplaceIATEntryInImageImportTable( hDll, "WININET.DLL", s_pfnHttpEndRequestW, &CWininetHook::_HttpEndRequestW);  
		ReplaceIATEntryInImageImportTable( hDll, "WININET.DLL", s_pfnHttpOpenRequestA, &CWininetHook::_HttpOpenRequestA);  
		ReplaceIATEntryInImageImportTable( hDll, "WININET.DLL", s_pfnHttpOpenRequestW, &CWininetHook::_HttpOpenRequestW);  
		ReplaceIATEntryInImageImportTable( hDll, "WININET.DLL", s_pfnInternetConnectA, &CWininetHook::_InternetConnectA);  
		ReplaceIATEntryInImageImportTable( hDll, "WININET.DLL", s_pfnInternetConnectW, &CWininetHook::_InternetConnectW);  
		ReplaceIATEntryInImageImportTable( hDll, "WININET.DLL", s_pfnInternetOpenUrlA, &CWininetHook::_InternetOpenUrlA);  
		ReplaceIATEntryInImageImportTable( hDll, "WININET.DLL", s_pfnInternetOpenUrlW, &CWininetHook::_InternetOpenUrlW); 
		ReplaceIATEntryInImageImportTable( hDll, "WININET.DLL", s_pfnInternetOpenA, &CWininetHook::_InternetOpenA);  
		ReplaceIATEntryInImageImportTable( hDll, "WININET.DLL", s_pfnInternetOpenW, &CWininetHook::_InternetOpenW); 
		ReplaceIATEntryInImageImportTable( hDll, "WININET.DLL", s_pfnInternetCloseHandle, &CWininetHook::_InternetCloseHandle); 
		ReplaceIATEntryInImageImportTable( hDll, "WININET.DLL", s_pfnInternetSetCookieA, &CWininetHook::_InternetSetCookieA); 
		ReplaceIATEntryInImageImportTable( hDll, "WININET.DLL", s_pfnInternetSetCookieW, &CWininetHook::_InternetSetCookieW); 
		ReplaceIATEntryInImageImportTable( hDll, "WININET.DLL", s_pfnInternetSetCookieExA, &CWininetHook::_InternetSetCookieExA); 
		ReplaceIATEntryInImageImportTable( hDll, "WININET.DLL", s_pfnInternetSetCookieExW, &CWininetHook::_InternetSetCookieExW); 
	}  
	return ntStatus;  
}  


#define TRY_GET_ADDRESS(name, fun1, fun2, type) \
	if( strcmp( szFunName, name) == 0 ) { if( !fun1 ) { fun1 = (type)(*FunctionAddress); } *FunctionAddress = fun2; return ntStatus; }

// Return the user-provided function address instead of the read one
NTSTATUS WINAPI CWininetHook::_LdrGetProcedureAddress(IN HMODULE ModuleHandle, IN PANSI_STRING FunctionName OPTIONAL, IN WORD Oridinal OPTIONAL, OUT PVOID *FunctionAddress )  
{  
	NTSTATUS ntStatus = s_pfnLdrGetProcedureAddress( ModuleHandle, FunctionName, Oridinal, FunctionAddress);  
	if( ntStatus == STATUS_SUCCESS )  
	{  
		TCHAR tszPath[MAX_PATH] = {0};  
		if( GetModuleFileName( ModuleHandle, tszPath, MAX_PATH) )  
		{  
			CString strFile(tszPath);  
			int nFind = strFile.ReverseFind('\\');  
			if( nFind > 0 )  
				strFile = strFile.Mid(nFind+1);  
			if( strFile.CompareNoCase(_T("WININET.dll")) == 0 )  
			{  
				CHAR szFunName[1024] = {0};  
				memcpy( szFunName, FunctionName->Buffer, FunctionName->Length );  
#ifdef _DEBUG
				USES_CONVERSION;
				TRACE( _T("LdrGetProcedureAddress %s\n"), A2T(szFunName));
#endif				

				TRY_GET_ADDRESS( "HttpSendRequestA", s_pfnHttpSendRequestA, _HttpSendRequestA,  PFN_HttpSendRequestA);
				TRY_GET_ADDRESS( "HttpSendRequestW", s_pfnHttpSendRequestW, _HttpSendRequestW, PFN_HttpSendRequestW);
				TRY_GET_ADDRESS( "HttpSendRequestExA", s_pfnHttpSendRequestExA, _HttpSendRequestExA, PFN_HttpSendRequestExA);
				TRY_GET_ADDRESS( "HttpSendRequestExW", s_pfnHttpSendRequestExW, _HttpSendRequestExW, PFN_HttpSendRequestExW);
				TRY_GET_ADDRESS( "HttpEndRequestA", s_pfnHttpEndRequestA, _HttpEndRequestA, PFN_HttpEndRequestA);
				TRY_GET_ADDRESS( "HttpEndRequestW", s_pfnHttpEndRequestW, _HttpEndRequestW, PFN_HttpEndRequestW);
				TRY_GET_ADDRESS( "HttpOpenRequestA", s_pfnHttpOpenRequestA, _HttpOpenRequestA, PFN_HttpOpenRequestA);
				TRY_GET_ADDRESS( "HttpOpenRequestW", s_pfnHttpOpenRequestW, _HttpOpenRequestW, PFN_HttpOpenRequestW);
				TRY_GET_ADDRESS( "InternetConnectA", s_pfnInternetConnectA, _InternetConnectA, PFN_InternetConnectA);
				TRY_GET_ADDRESS( "InternetConnectW", s_pfnInternetConnectW, _InternetConnectW, PFN_InternetConnectW);
				TRY_GET_ADDRESS( "HttpAddRequestHeadersA", s_pfnHttpAddRequestHeadersA, _HttpAddRequestHeadersA, PFN_HttpAddRequestHeadersA);
				TRY_GET_ADDRESS( "HttpAddRequestHeadersW", s_pfnHttpAddRequestHeadersW, _HttpAddRequestHeadersW, PFN_HttpAddRequestHeadersW);
				TRY_GET_ADDRESS( "InternetOpenUrlA", s_pfnInternetOpenUrlA, _InternetOpenUrlA, PFN_InternetOpenUrlA);
				TRY_GET_ADDRESS( "InternetOpenUrlW", s_pfnInternetOpenUrlW, _InternetOpenUrlW, PFN_InternetOpenUrlW);
				TRY_GET_ADDRESS( "InternetOpenA", s_pfnInternetOpenA, _InternetOpenA, PFN_InternetOpenA);
				TRY_GET_ADDRESS( "InternetOpenW", s_pfnInternetOpenW, _InternetOpenW, PFN_InternetOpenW);
				TRY_GET_ADDRESS( "InternetCloseHandle", s_pfnInternetCloseHandle, _InternetCloseHandle, PFN_InternetCloseHandle);
				TRY_GET_ADDRESS( "InternetSetCookieA", s_pfnInternetSetCookieA, _InternetSetCookieA, PFN_InternetSetCookieA);
				TRY_GET_ADDRESS( "InternetSetCookieW", s_pfnInternetSetCookieW, _InternetSetCookieW, PFN_InternetSetCookieW);
				TRY_GET_ADDRESS( "InternetSetCookieExA", s_pfnInternetSetCookieExA, _InternetSetCookieExA, PFN_InternetSetCookieExA);
				TRY_GET_ADDRESS( "InternetSetCookieExW", s_pfnInternetSetCookieExW, _InternetSetCookieExW, PFN_InternetSetCookieExW);
			}  
		}  
	}  


	return ntStatus;  
}  
#include <afxinet.h>
//////////////////////////////////////////////////////////////////////////
BOOL WINAPI CWininetHook::_HttpSendRequestA(HINTERNET hRequest, LPCSTR lpszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength )  
{  
	::MessageBoxA(NULL,(char*)lpOptional,"",NULL);
	return s_pfnHttpSendRequestA( hRequest, lpszHeaders, dwHeadersLength, lpOptional, dwOptionalLength);  
}  
BOOL WINAPI CWininetHook::_HttpSendRequestW(HINTERNET hRequest, LPCWSTR lpszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength )  
{  
	if (dwOptionalLength > 0)
	{
		::MessageBoxW(NULL,(LPCWSTR)lpOptional,NULL,NULL);
	}
	
	return s_pfnHttpSendRequestW( hRequest, lpszHeaders, dwHeadersLength, lpOptional, dwOptionalLength);  
}  
BOOL WINAPI CWininetHook::_HttpAddRequestHeadersA(__in HINTERNET hRequest,__in_ecount(dwHeadersLength) LPCSTR lpszHeaders,__in DWORD dwHeadersLength,__in DWORD dwModifiers)  
{
	return s_pfnHttpAddRequestHeadersA( hRequest, lpszHeaders, dwHeadersLength, dwModifiers);  
}
BOOL WINAPI CWininetHook::_HttpAddRequestHeadersW(__in HINTERNET hRequest,__in_ecount(dwHeadersLength) LPCWSTR lpszHeaders,__in DWORD dwHeadersLength,__in DWORD dwModifiers)  
{
	return s_pfnHttpAddRequestHeadersW( hRequest, lpszHeaders, dwHeadersLength, dwModifiers);  
}  
BOOL WINAPI CWininetHook::_HttpSendRequestExA( __in HINTERNET hRequest, __in_opt LPINTERNET_BUFFERSA lpBuffersIn, __out_opt LPINTERNET_BUFFERSA lpBuffersOut, __in DWORD dwFlags, __in_opt DWORD_PTR dwContext)  
{  
	return s_pfnHttpSendRequestExA( hRequest, lpBuffersIn, lpBuffersOut, dwFlags, dwContext);  
}  
BOOL WINAPI CWininetHook::_HttpSendRequestExW( __in HINTERNET hRequest, __in_opt LPINTERNET_BUFFERSW lpBuffersIn, __out_opt LPINTERNET_BUFFERSW lpBuffersOut, __in DWORD dwFlags, __in_opt DWORD_PTR dwContext)  
{  
	return s_pfnHttpSendRequestExW( hRequest, lpBuffersIn, lpBuffersOut, dwFlags, dwContext);  
}  
BOOL WINAPI CWininetHook::_HttpEndRequestA( __in HINTERNET hRequest, __out_opt LPINTERNET_BUFFERSA lpBuffersOut, __in DWORD dwFlags, __in_opt DWORD_PTR dwContext)  
{  
	return s_pfnHttpEndRequestA( hRequest, lpBuffersOut, dwFlags, dwContext);  
}  
BOOL WINAPI CWininetHook::_HttpEndRequestW( __in HINTERNET hRequest, __out_opt LPINTERNET_BUFFERSW lpBuffersOut, __in DWORD dwFlags, __in_opt DWORD_PTR dwContext)  
{  
	return s_pfnHttpEndRequestW( hRequest, lpBuffersOut, dwFlags, dwContext);  
}  
HINTERNET WINAPI CWininetHook::_HttpOpenRequestA(__in HINTERNET hConnect,__in_opt LPCSTR lpszVerb, __in_opt LPCSTR lpszObjectName, __in_opt LPCSTR lpszVersion, __in_opt LPCSTR lpszReferrer, __in_z_opt LPCSTR FAR * lplpszAcceptTypes, __in DWORD dwFlags, __in_opt DWORD_PTR dwContext)  
{  
	HINTERNET hRequest = s_pfnHttpOpenRequestA( hConnect, lpszVerb, lpszObjectName, lpszVersion, lpszReferrer, lplpszAcceptTypes, dwFlags, dwContext);  
	USES_CONVERSION;
	CMainFrame::GetRequestDlg().LogRequest( hConnect, hRequest, A2T(lpszObjectName), A2T(lpszVerb));
	return hRequest;
}  
HINTERNET WINAPI CWininetHook::_HttpOpenRequestW(__in HINTERNET hConnect,__in_opt LPCWSTR lpszVerb,__in_opt LPCWSTR lpszObjectName,__in_opt LPCWSTR lpszVersion,__in_opt LPCWSTR lpszReferrer,__in_z_opt LPCWSTR FAR * lplpszAcceptTypes,__in DWORD dwFlags, __in_opt DWORD_PTR dwContext)  
{  
	HINTERNET hRequest = s_pfnHttpOpenRequestW( hConnect, lpszVerb, lpszObjectName, lpszVersion, lpszReferrer, lplpszAcceptTypes, dwFlags, dwContext);  
	USES_CONVERSION;
	CMainFrame::GetRequestDlg().LogRequest( hConnect, hRequest, W2T((LPWSTR)lpszObjectName), W2T((LPWSTR)lpszVerb));
	return hRequest;
}  
HINTERNET WINAPI CWininetHook::_InternetConnectA(__in HINTERNET hInternet,__in LPCSTR lpszServerName,__in INTERNET_PORT nServerPort,__in_opt LPCSTR lpszUserName,__in_opt LPCSTR lpszPassword,__in DWORD dwService,__in DWORD dwFlags,__in_opt DWORD_PTR dwContext)  
{  
	HINTERNET hConnect = s_pfnInternetConnectA( hInternet, lpszServerName, nServerPort, lpszUserName, lpszPassword, dwService, dwFlags, dwContext);  
	if( hConnect )
	{
		USES_CONVERSION;
		CMainFrame::GetRequestDlg().LogInternetConnect( hConnect, A2T(lpszServerName), nServerPort);
	}
	return hConnect;
}  
HINTERNET WINAPI CWininetHook::_InternetConnectW(__in HINTERNET hInternet,__in LPCWSTR lpszServerName,__in INTERNET_PORT nServerPort,__in_opt LPCWSTR lpszUserName,__in_opt LPCWSTR lpszPassword,__in DWORD dwService,__in DWORD dwFlags,__in_opt DWORD_PTR dwContext)  
{  
	HINTERNET hConnect = s_pfnInternetConnectW( hInternet, lpszServerName, nServerPort, lpszUserName, lpszPassword, dwService, dwFlags, dwContext);  
	if( hConnect )
	{
		USES_CONVERSION;
		CMainFrame::GetRequestDlg().LogInternetConnect( hConnect, W2T((LPWSTR)lpszServerName), nServerPort);
	}
	return hConnect;
}
HINTERNET WINAPI CWininetHook::_InternetOpenUrlA(HINTERNET hInternet,LPCSTR lpszUrl, LPCSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwFlags, DWORD_PTR dwContext)
{
	return s_pfnInternetOpenUrlA( hInternet, lpszUrl, lpszHeaders, dwHeadersLength, dwFlags, dwContext);
}
HINTERNET WINAPI CWininetHook::_InternetOpenUrlW(HINTERNET hInternet,LPCWSTR lpszUrl,LPCWSTR lpszHeaders,DWORD dwHeadersLength,DWORD dwFlags,DWORD_PTR dwContext)
{
	return s_pfnInternetOpenUrlW( hInternet, lpszUrl, lpszHeaders, dwHeadersLength, dwFlags, dwContext);
}
HINTERNET WINAPI CWininetHook::_InternetOpenA( LPCSTR lpszAgent, DWORD dwAccessType, LPCSTR lpszProxy, LPCSTR lpszProxyBypass, DWORD dwFlags )
{
	return s_pfnInternetOpenA( lpszAgent, dwAccessType, lpszProxy, lpszProxyBypass, dwFlags);
}
HINTERNET WINAPI CWininetHook::_InternetOpenW( LPCWSTR lpszAgent, DWORD dwAccessType, LPCWSTR lpszProxy, LPCWSTR lpszProxyBypass, DWORD dwFlags )
{
	return s_pfnInternetOpenW( lpszAgent, dwAccessType, lpszProxy, lpszProxyBypass, dwFlags);
}
BOOL WINAPI CWininetHook::_InternetCloseHandle(__in HINTERNET hInternet)
{
	CMainFrame::GetRequestDlg().LogHttpResponseHeader(hInternet);
	return s_pfnInternetCloseHandle(hInternet);
}
BOOL WINAPI CWininetHook::_InternetSetCookieA(LPCSTR lpszUrl, LPCSTR lpszCookieName, LPCSTR lpszCookieData )
{
	return s_pfnInternetSetCookieA( lpszUrl, lpszCookieName, lpszCookieData);
}
BOOL WINAPI CWininetHook::_InternetSetCookieW( LPCWSTR lpszUrl, LPCWSTR lpszCookieName, LPCWSTR lpszCookieData)
{
	return s_pfnInternetSetCookieW( lpszUrl, lpszCookieName, lpszCookieData);
}
DWORD WINAPI CWininetHook::_InternetSetCookieExA(LPCSTR lpszUrl, LPCSTR lpszCookieName, LPCSTR lpszCookieData, DWORD dwFlags, DWORD_PTR dwReserved)
{
	return s_pfnInternetSetCookieExA( lpszUrl, lpszCookieName, lpszCookieData, dwFlags, dwReserved);
}
DWORD WINAPI CWininetHook::_InternetSetCookieExW( LPCWSTR lpszUrl, LPCWSTR lpszCookieName, LPCWSTR lpszCookieData, DWORD dwFlags, DWORD_PTR dwReserved )
{
	return s_pfnInternetSetCookieExW( lpszUrl, lpszCookieName, lpszCookieData, dwFlags, dwReserved);
}