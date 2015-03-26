#pragma once

#include <WinInet.h>  
#pragma comment( lib, "WinInet.lib")  
#include <Dbghelp.h>  
#include <DelayImp.h>  
#pragma comment( lib, "Dbghelp.lib")  
#include <Psapi.h>  
#pragma comment( lib, "Psapi.lib")  


#ifndef INTERNET_COOKIE_NON_SCRIPT
#define INTERNET_COOKIE_NON_SCRIPT      0x00001000
#endif

#ifndef INTERNET_COOKIE_HTTPONLY
#define INTERNET_COOKIE_HTTPONLY      0x00002000
#endif




typedef struct _STRING {  
	USHORT  Length;  
	USHORT  MaximumLength;  
	PCHAR   Buffer;  
} ANSI_STRING, *PANSI_STRING;  
typedef struct _LSA_UNICODE_STRING {  
	USHORT Length;  
	USHORT MaximumLength;  
	PWSTR  Buffer;  
}LSA_UNICODE_STRING, *PLSA_UNICODE_STRING, UNICODE_STRING, *PUNICODE_STRING;  
typedef LONG NTSTATUS;  

#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS ((NTSTATUS)0x00000000L) 
#endif

typedef NTSTATUS (WINAPI* PFN_LdrGetProcedureAddress)(IN HMODULE ModuleHandle, IN PANSI_STRING FunctionName OPTIONAL, IN WORD Oridinal OPTIONAL, OUT PVOID *FunctionAddress );  
typedef NTSTATUS (WINAPI* PFN_LdrLoadDll)(IN PWCHAR PathToFile OPTIONAL, IN ULONG Flags OPTIONAL, IN PUNICODE_STRING ModuleFileName, OUT PHANDLE ModuleHandle);  
typedef BOOL (WINAPI* PFN_HttpSendRequestA)(HINTERNET hRequest, LPCSTR lpszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength );  
typedef BOOL (WINAPI* PFN_HttpSendRequestW)(HINTERNET hRequest, LPCWSTR lpszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength );  
typedef BOOL (WINAPI* PFN_HttpSendRequestExA)( __in HINTERNET hRequest, __in_opt LPINTERNET_BUFFERSA lpBuffersIn, __out_opt LPINTERNET_BUFFERSA lpBuffersOut, __in DWORD dwFlags, __in_opt DWORD_PTR dwContext);  
typedef BOOL (WINAPI* PFN_HttpSendRequestExW)( __in HINTERNET hRequest, __in_opt LPINTERNET_BUFFERSW lpBuffersIn, __out_opt LPINTERNET_BUFFERSW lpBuffersOut, __in DWORD dwFlags, __in_opt DWORD_PTR dwContext);  
typedef BOOL (WINAPI* PFN_HttpEndRequestA)( __in HINTERNET hRequest, __out_opt LPINTERNET_BUFFERSA lpBuffersOut, __in DWORD dwFlags, __in_opt DWORD_PTR dwContext);  
typedef BOOL (WINAPI* PFN_HttpEndRequestW)( __in HINTERNET hRequest, __out_opt LPINTERNET_BUFFERSW lpBuffersOut, __in DWORD dwFlags, __in_opt DWORD_PTR dwContext);  
typedef HINTERNET (WINAPI* PFN_HttpOpenRequestA)(__in HINTERNET hConnect,__in_opt LPCSTR lpszVerb, __in_opt LPCSTR lpszObjectName, __in_opt LPCSTR lpszVersion, __in_opt LPCSTR lpszReferrer, __in_z_opt LPCSTR FAR * lplpszAcceptTypes, __in DWORD dwFlags, __in_opt DWORD_PTR dwContext);  
typedef HINTERNET (WINAPI* PFN_HttpOpenRequestW)(__in HINTERNET hConnect,__in_opt LPCWSTR lpszVerb,__in_opt LPCWSTR lpszObjectName,__in_opt LPCWSTR lpszVersion,__in_opt LPCWSTR lpszReferrer,__in_z_opt LPCWSTR FAR * lplpszAcceptTypes,__in DWORD dwFlags, __in_opt DWORD_PTR dwContext);  
typedef HINTERNET (WINAPI* PFN_InternetConnectA)(__in HINTERNET hInternet,__in LPCSTR lpszServerName,__in INTERNET_PORT nServerPort,__in_opt LPCSTR lpszUserName,__in_opt LPCSTR lpszPassword,__in DWORD dwService,__in DWORD dwFlags,__in_opt DWORD_PTR dwContext);  
typedef HINTERNET (WINAPI* PFN_InternetConnectW)(__in HINTERNET hInternet,__in LPCWSTR lpszServerName,__in INTERNET_PORT nServerPort,__in_opt LPCWSTR lpszUserName,__in_opt LPCWSTR lpszPassword,__in DWORD dwService,__in DWORD dwFlags,__in_opt DWORD_PTR dwContext);  
typedef BOOL (WINAPI* PFN_HttpAddRequestHeadersA)(__in HINTERNET hRequest,__in_ecount(dwHeadersLength) LPCSTR lpszHeaders,__in DWORD dwHeadersLength,__in DWORD dwModifiers);  
typedef BOOL (WINAPI* PFN_HttpAddRequestHeadersW)(__in HINTERNET hRequest,__in_ecount(dwHeadersLength) LPCWSTR lpszHeaders,__in DWORD dwHeadersLength,__in DWORD dwModifiers);  
typedef HINTERNET (WINAPI* PFN_InternetOpenUrlA)(HINTERNET hInternet,LPCSTR lpszUrl, LPCSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwFlags, DWORD_PTR dwContext);
typedef HINTERNET (WINAPI* PFN_InternetOpenUrlW)(HINTERNET hInternet,LPCWSTR lpszUrl,LPCWSTR lpszHeaders,DWORD dwHeadersLength,DWORD dwFlags,DWORD_PTR dwContext);
typedef HINTERNET (WINAPI* PFN_InternetOpenA)( LPCSTR lpszAgent, DWORD dwAccessType, LPCSTR lpszProxy, LPCSTR lpszProxyBypass, DWORD dwFlags );
typedef HINTERNET (WINAPI* PFN_InternetOpenW)( LPCWSTR lpszAgent, DWORD dwAccessType, LPCWSTR lpszProxy, LPCWSTR lpszProxyBypass, DWORD dwFlags );
typedef BOOL (WINAPI* PFN_InternetCloseHandle)(__in HINTERNET hInternet);
typedef BOOL (WINAPI* PFN_InternetSetCookieA)(LPCSTR lpszUrl, LPCSTR lpszCookieName, LPCSTR lpszCookieData );
typedef BOOL (WINAPI* PFN_InternetSetCookieW)( LPCWSTR lpszUrl, LPCWSTR lpszCookieName, LPCWSTR lpszCookieData);
typedef DWORD (WINAPI* PFN_InternetSetCookieExA)(LPCSTR lpszUrl, LPCSTR lpszCookieName, LPCSTR lpszCookieData, DWORD dwFlags, DWORD_PTR dwReserved);
typedef DWORD (WINAPI* PFN_InternetSetCookieExW)( LPCWSTR lpszUrl, LPCWSTR lpszCookieName, LPCWSTR lpszCookieData, DWORD dwFlags, DWORD_PTR dwReserved );

typedef BOOL (WINAPI* PFN_InternetReadFile)(__in HINTERNET hFile,
											__out_bcount(dwNumberOfBytesToRead) LPVOID lpBuffer,
											__in DWORD dwNumberOfBytesToRead,
											__out LPDWORD lpdwNumberOfBytesRead);

typedef BOOL (WINAPI* PFN_InternetReadFileExW)(__in HINTERNET hRequest,__out LPINTERNET_BUFFERSW lpBuffersOut,__in DWORD dwFlags,__in_opt DWORD_PTR dwContent);
typedef BOOL (WINAPI* PFN_InternetReadFileExA)(__in HINTERNET hRequest,__out LPINTERNET_BUFFERSA lpBuffersOut,__in DWORD dwFlags,__in_opt DWORD_PTR dwContent);

/*
INTERNETAPI_(BOOL) InternetReadFileExW(
__in HINTERNET hFile,
__out LPINTERNET_BUFFERSW lpBuffersOut,
__in DWORD dwFlags,
__in_opt DWORD_PTR dwContext
);
*/


class CWininetHook
{
public:
	CWininetHook(void);
	~CWininetHook(void);

	
private:
	// variables to store the original function address
	static PFN_LdrLoadDll s_pfnLdrLoadDll;  
	static PFN_LdrGetProcedureAddress s_pfnLdrGetProcedureAddress;  
	static PFN_HttpSendRequestA s_pfnHttpSendRequestA;  
	static PFN_HttpSendRequestW s_pfnHttpSendRequestW;  
	static PFN_HttpSendRequestExA s_pfnHttpSendRequestExA;  
	static PFN_HttpSendRequestExW s_pfnHttpSendRequestExW;  
	static PFN_HttpEndRequestA s_pfnHttpEndRequestA;  
	static PFN_HttpEndRequestW s_pfnHttpEndRequestW;  
	static PFN_HttpOpenRequestA s_pfnHttpOpenRequestA;  
	static PFN_HttpOpenRequestW s_pfnHttpOpenRequestW;  
	static PFN_InternetConnectA s_pfnInternetConnectA;  
	static PFN_InternetConnectW s_pfnInternetConnectW;    
	static PFN_HttpAddRequestHeadersA s_pfnHttpAddRequestHeadersA;  
	static PFN_HttpAddRequestHeadersW s_pfnHttpAddRequestHeadersW;  
	static PFN_InternetOpenUrlA s_pfnInternetOpenUrlA;  
	static PFN_InternetOpenUrlW s_pfnInternetOpenUrlW;  
	static PFN_InternetOpenA s_pfnInternetOpenA;  
	static PFN_InternetOpenW s_pfnInternetOpenW;  
	static PFN_InternetCloseHandle s_pfnInternetCloseHandle;
	static PFN_InternetSetCookieA s_pfnInternetSetCookieA;
	static PFN_InternetSetCookieW s_pfnInternetSetCookieW;
	static PFN_InternetSetCookieExA s_pfnInternetSetCookieExA;
	static PFN_InternetSetCookieExW s_pfnInternetSetCookieExW;

	static PFN_InternetReadFile  s_pfnInternetReadFile;
	static PFN_InternetReadFileExA  s_pfnInternetReadFileExA;
	static PFN_InternetReadFileExW  s_pfnInternetReadFileExW;

	// user-provided functions
	static BOOL WINAPI _HttpSendRequestA(HINTERNET hRequest, LPCSTR lpszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength );  
	static BOOL WINAPI _HttpSendRequestW(HINTERNET hRequest, LPCWSTR lpszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength );  
	static BOOL WINAPI _HttpSendRequestExA( __in HINTERNET hRequest, __in_opt LPINTERNET_BUFFERSA lpBuffersIn, __out_opt LPINTERNET_BUFFERSA lpBuffersOut, __in DWORD dwFlags, __in_opt DWORD_PTR dwContext);  
	static BOOL WINAPI _HttpSendRequestExW( __in HINTERNET hRequest, __in_opt LPINTERNET_BUFFERSW lpBuffersIn, __out_opt LPINTERNET_BUFFERSW lpBuffersOut, __in DWORD dwFlags, __in_opt DWORD_PTR dwContext);  
	static BOOL WINAPI _HttpEndRequestA( __in HINTERNET hRequest, __out_opt LPINTERNET_BUFFERSA lpBuffersOut, __in DWORD dwFlags, __in_opt DWORD_PTR dwContext);  
	static BOOL WINAPI _HttpEndRequestW( __in HINTERNET hRequest, __out_opt LPINTERNET_BUFFERSW lpBuffersOut, __in DWORD dwFlags, __in_opt DWORD_PTR dwContext);  
	static HINTERNET WINAPI _HttpOpenRequestA(__in HINTERNET hConnect,__in_opt LPCSTR lpszVerb, __in_opt LPCSTR lpszObjectName, __in_opt LPCSTR lpszVersion, __in_opt LPCSTR lpszReferrer, __in_z_opt LPCSTR FAR * lplpszAcceptTypes, __in DWORD dwFlags, __in_opt DWORD_PTR dwContext);  
	static HINTERNET WINAPI _HttpOpenRequestW(__in HINTERNET hConnect,__in_opt LPCWSTR lpszVerb,__in_opt LPCWSTR lpszObjectName,__in_opt LPCWSTR lpszVersion,__in_opt LPCWSTR lpszReferrer,__in_z_opt LPCWSTR FAR * lplpszAcceptTypes,__in DWORD dwFlags, __in_opt DWORD_PTR dwContext);  
	static HINTERNET WINAPI _InternetConnectA(__in HINTERNET hInternet,__in LPCSTR lpszServerName,__in INTERNET_PORT nServerPort,__in_opt LPCSTR lpszUserName,__in_opt LPCSTR lpszPassword,__in DWORD dwService,__in DWORD dwFlags,__in_opt DWORD_PTR dwContext);  
	static HINTERNET WINAPI _InternetConnectW(__in HINTERNET hInternet,__in LPCWSTR lpszServerName,__in INTERNET_PORT nServerPort,__in_opt LPCWSTR lpszUserName,__in_opt LPCWSTR lpszPassword,__in DWORD dwService,__in DWORD dwFlags,__in_opt DWORD_PTR dwContext);  
	static BOOL WINAPI _HttpAddRequestHeadersA(__in HINTERNET hRequest,__in_ecount(dwHeadersLength) LPCSTR lpszHeaders,__in DWORD dwHeadersLength,__in DWORD dwModifiers);  
	static BOOL WINAPI _HttpAddRequestHeadersW(__in HINTERNET hRequest,__in_ecount(dwHeadersLength) LPCWSTR lpszHeaders,__in DWORD dwHeadersLength,__in DWORD dwModifiers);  
	static HINTERNET WINAPI _InternetOpenUrlA(HINTERNET hInternet,LPCSTR lpszUrl, LPCSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwFlags, DWORD_PTR dwContext);
	static HINTERNET WINAPI _InternetOpenUrlW(HINTERNET hInternet,LPCWSTR lpszUrl,LPCWSTR lpszHeaders,DWORD dwHeadersLength,DWORD dwFlags,DWORD_PTR dwContext);
	static HINTERNET WINAPI _InternetOpenA( LPCSTR lpszAgent, DWORD dwAccessType, LPCSTR lpszProxy, LPCSTR lpszProxyBypass, DWORD dwFlags );
	static HINTERNET WINAPI _InternetOpenW( LPCWSTR lpszAgent, DWORD dwAccessType, LPCWSTR lpszProxy, LPCWSTR lpszProxyBypass, DWORD dwFlags );
	static BOOL WINAPI _InternetCloseHandle(__in HINTERNET hInternet);
	static BOOL WINAPI _InternetSetCookieA(LPCSTR lpszUrl, LPCSTR lpszCookieName, LPCSTR lpszCookieData );
	static BOOL WINAPI _InternetSetCookieW( LPCWSTR lpszUrl, LPCWSTR lpszCookieName, LPCWSTR lpszCookieData);
	static DWORD WINAPI _InternetSetCookieExA(LPCSTR lpszUrl, LPCSTR lpszCookieName, LPCSTR lpszCookieData, DWORD dwFlags, DWORD_PTR dwReserved);
	static DWORD WINAPI _InternetSetCookieExW( LPCWSTR lpszUrl, LPCWSTR lpszCookieName, LPCWSTR lpszCookieData, DWORD dwFlags, DWORD_PTR dwReserved );


	static BOOL WINAPI _InternetReadFile(__in HINTERNET hFile,
		__out_bcount(dwNumberOfBytesToRead) LPVOID lpBuffer,
		__in DWORD dwNumberOfBytesToRead,
		__out LPDWORD lpdwNumberOfBytesRead);
	static BOOL WINAPI _InternetReadFileExA(__in HINTERNET hRequest,__out LPINTERNET_BUFFERSA lpBuffersOut,__in DWORD dwFlags,__in_opt DWORD_PTR dwContent);
	static BOOL WINAPI _InternetReadFileExW(__in HINTERNET hRequest,__out LPINTERNET_BUFFERSW lpBuffersOut,__in DWORD dwFlags,__in_opt DWORD_PTR dwContent);
	

	// process the IAT
	static void ReplaceIATEntryForAll(LPCSTR lpszDllName, LPVOID pfnCurrent, LPVOID pfnNew);
	static BOOL ReplaceIATEntryInDelayImageImportTable( HANDLE hBaseAddress  
		, LPCSTR lpszDllName  
		, LPVOID pfnCurrent  
		, LPVOID pfnNew  
		);
	static BOOL ReplaceIATEntryInImageImportTable( HANDLE hBaseAddress  
		, LPCSTR lpszDllName  
		, LPVOID pfnCurrent  
		, LPVOID pfnNew  
		);

	static NTSTATUS WINAPI _LdrLoadDll(IN PWCHAR PathToFile OPTIONAL, IN ULONG Flags OPTIONAL, IN PUNICODE_STRING ModuleFileName, OUT PHANDLE ModuleHandle);  
	static NTSTATUS WINAPI _LdrGetProcedureAddress(IN HMODULE ModuleHandle, IN PANSI_STRING FunctionName OPTIONAL, IN WORD Oridinal OPTIONAL, OUT PVOID *FunctionAddress );   
};
