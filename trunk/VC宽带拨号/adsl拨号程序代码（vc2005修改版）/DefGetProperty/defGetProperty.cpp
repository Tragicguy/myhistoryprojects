//本程序由唐添麒编写，现将代码发布供学习交流之用
//本程序的发布遵守GPL协议，您可在此协议的规定下使用本程序
//如果您在使用本程序的途中有什么疑问或建议请访问http://digua254909662.blog.163.com/blog/static/74805072200922323951508/edit/
//或者联系作者：qq<254909662> 并说明来意，本人非常乐意与您交流

#include <windows.h>
#include <Ras.h>
#include <string>
#include <sstream>

typedef int (__stdcall* LPFNDLLFUNC1)(LPCSTR, LPCSTR, LPRASENTRYA, LPDWORD, LPBYTE, LPDWORD );
typedef DWORD (__stdcall* LPENUMENTRY)(LPCSTR, LPCSTR, LPRASENTRYNAMEA, LPDWORD,LPDWORD );
typedef DWORD (__stdcall* LPENUMCONNECT)(LPRASCONNA, LPDWORD, LPDWORD );
typedef DWORD (__stdcall* LPGETCONNECTSTATUS)(HRASCONN, LPRASCONNSTATUSA);
typedef DWORD (__stdcall* LPGETENTRYDIALPARAMS)(LPCSTR, LPRASDIALPARAMSA, LPBOOL );
typedef DWORD (__stdcall* LPSURROGATEDIALUP)(LPRASDIALEXTENSIONS, LPCSTR, LPRASDIALPARAMSA, DWORD,LPVOID, LPHRASCONN );
typedef DWORD (__stdcall* LPRASGETERRORSTRING)(UINT, LPSTR, DWORD );
typedef DWORD (__stdcall* LPRESETDIALCONNECT)(HRASCONN, LPRASCONNSTATUSA );
typedef DWORD (__stdcall* LPRASHANGUP)( HRASCONN );



typedef VOID (__stdcall* LPRASDIALFUNC)(UINT, RASCONNSTATE, DWORD);

struct EntryNameInfo
{
	CHAR  szEntryName[ RAS_MaxEntryName + 1 ];
	CHAR  szUserName[ UNLEN + 1 ];
	CHAR  szPassword[ PWLEN + 1 ];
};

//BOOL WINAPI DllEntryPoint(HINSTANCE hDLL,DWORD dwReason,LPVOID Reserved)
//{ 
//	switch(dwReason)
//	{
//	case DLL-PROCESS-ATTACH:
//		{
//			break; 
//		}
//	case DLL－PROCESS－DETACH:
//		{ 
//			break; 
//		} 
//	}
//	return TRUE;
//}

extern "C" int __stdcall GetEntryProperty( char *pNameType)
{
	if( NULL == pNameType)
	{
		return 0;
	}
	LPFNDLLFUNC1 lpGetEntryProperty;
	//MessageBox(0, pNameType, "entryName", 0);
	HINSTANCE hLib = LoadLibrary("RASAPI32.DLL");
	if (NULL == hLib) return -1; 
	 lpGetEntryProperty = (LPFNDLLFUNC1) GetProcAddress(hLib, "RasGetEntryPropertiesA");   
     if (NULL == lpGetEntryProperty)   
     {   
         ::FreeLibrary(hLib);   
         return -2;   
     }   
     RASENTRY RasEntry;   
     DWORD dw = sizeof(RASENTRY);   
     ZeroMemory(&RasEntry, dw);   
     RasEntry.dwSize = dw;   
     DWORD ret = lpGetEntryProperty(NULL, pNameType, &RasEntry, &dw, NULL, NULL); 
	 ::FreeLibrary(hLib);
	 hLib = NULL;
	 if (ret != 0)
	 {
		 return -3;
	 }
	 //MessageBox(0, RasEntry.szDeviceType, "deviceType", 0);
	 if(!_stricmp(RasEntry.szDeviceType, RASDT_Modem))
	 {
		 return 1;
	 }
	 if(!_stricmp(RasEntry.szDeviceType, RASDT_Vpn))
		 return 2;
	 if(!_stricmp(RasEntry.szDeviceType, RASDT_PPPoE))
		 return 3;
	 if(!_stricmp(RasEntry.szDeviceType, RASDT_Isdn))
		 return 4;
	 if(!_stricmp(RasEntry.szDeviceType, RASDT_X25))
		 return 5;
	 return 100;
}

extern "C" int __stdcall EnumEntries( RASENTRYNAMEA *pEntries, int *pNum)
{
	if( (NULL == pEntries)||(NULL == pNum) )
	{
		return 0;
	}
	LPENUMENTRY lpEnumEntry;
	HINSTANCE hLib = LoadLibrary("RASAPI32.DLL");
	if (NULL == hLib) return -1; 
	 lpEnumEntry = (LPENUMENTRY) GetProcAddress(hLib, "RasEnumEntriesA");   
     if (NULL == lpEnumEntry)   
     {   
         ::FreeLibrary(hLib);   
		 return -2;   
	 }   
	 DWORD cbBuf;
	 DWORD cEntry;
	 LPRASENTRYNAME lpRasEntry = new RASENTRYNAME; 
	 RtlZeroMemory(lpRasEntry, sizeof(RASENTRYNAME) );
	 lpRasEntry->dwSize = sizeof(RASENTRYNAME);   
	 cbBuf = sizeof(RASENTRYNAME);   

	 DWORD dwRet = lpEnumEntry(NULL, NULL, lpRasEntry, &cbBuf, &cEntry); //枚举连接，系统会把需要的缓冲区大小放在cuBuf中  
	 delete lpRasEntry;

	 LPBYTE lpBuffer = new BYTE[cbBuf];   //分配缓冲区
	 lpRasEntry = (LPRASENTRYNAME)lpBuffer;   
	 lpRasEntry->dwSize = sizeof(RASENTRYNAME);   //初始化dwSize参数 必须!!!!
	 dwRet = lpEnumEntry(NULL, NULL, lpRasEntry, &cbBuf, &cEntry); 
	 if(0 != dwRet)
	 {
		 ::FreeLibrary(hLib);
		 return -3;
	 }
	 if(*pNum == -1)
	 {
		 *pNum = (int)cEntry;
		 if( (cEntry > 0)&& (pEntries != NULL))
		 {
			  pEntries[0].dwSize = lpRasEntry[0].dwSize;
		 }
		 ::FreeLibrary(hLib);
		 return 1;
	 }
	 *pNum = (int)cEntry;
	 for(DWORD i = 0; i<cEntry; i++)
	 {
		 //MessageBox(0, lpRasEntry[i].szEntryName, "EnumEntries", 0);
		 memcpy(&(pEntries[i]), &(lpRasEntry[i]), sizeof(RASENTRYNAMEA));
	 }
	 ::FreeLibrary(hLib);
	 return 1;
}

extern "C" int __stdcall IsConnected( char *pEntryName)
{
	if( NULL == pEntryName)
	{
		return 0;
	}
	LPENUMCONNECT lpEnumConnect;
	HINSTANCE hLib = LoadLibrary("RASAPI32.DLL");
	if (NULL == hLib) return -1; 
	lpEnumConnect = (LPENUMCONNECT) GetProcAddress(hLib, "RasEnumConnectionsA");   
	if (NULL == lpEnumConnect)   
	{   
		::FreeLibrary(hLib);   
		return -2;   
	}   
	RASCONN rcArray[100];   
	RASCONNSTATUS rStatus;   
	DWORD dwRet;   
	DWORD dw1 = sizeof(rcArray);   
	DWORD dw2;   
	ZeroMemory(rcArray, dw1);   
	rcArray[0].dwSize = sizeof(RASCONN);   
	dwRet = lpEnumConnect(rcArray, & dw1, & dw2);  
	if(dwRet != ERROR_SUCCESS)
	{
		::FreeLibrary(hLib);
		return -3;
	}
	//  ERROR_BUFFER_TOO_SMALL    
	ZeroMemory(&rStatus, sizeof(RASCONNSTATUS));   
	rStatus.dwSize = sizeof(RASCONNSTATUS); 
	bool isExist = false;
	for (dw1 = 0; dw1 < dw2; dw1++)   
	{   
		if( !strcmp(pEntryName, rcArray[dw1].szEntryName) )
		{
			isExist = true;
			LPGETCONNECTSTATUS lpGetConnectStatus = (LPGETCONNECTSTATUS)GetProcAddress(hLib, "RasGetConnectStatusA");
			if(NULL == lpGetConnectStatus)
			{
				::FreeLibrary(hLib);
				return -4;
			}
			dwRet = lpGetConnectStatus(rcArray[dw1].hrasconn, &rStatus); 
			if( dwRet != ERROR_SUCCESS)
			{
				::FreeLibrary(hLib);
				return -5;
			}
			if (RASCS_Connected == rStatus.rasconnstate)   
			{   
				::FreeLibrary(hLib);
				return 1;
			} 
		}
	} 
	::FreeLibrary(hLib);
	return (isExist?99:100);
}

extern "C" int __stdcall GetEntryDialParams( RASDIALPARAMS *pEntryName)
{
	if( NULL == pEntryName )
	{
		return 0;
	}
	LPGETENTRYDIALPARAMS lpGetEntryDialParams;
	HINSTANCE hLib = LoadLibrary("RASAPI32.DLL");
	if (NULL == hLib) return -1; 
	lpGetEntryDialParams = (LPGETENTRYDIALPARAMS) GetProcAddress(hLib, "RasGetEntryDialParamsA");   
	if (NULL == lpGetEntryDialParams)   
	{   
		::FreeLibrary(hLib);   
		return -2;   
	}  
	RASDIALPARAMS rp = {0};//声明获取拨号参数的结构体
	rp.dwSize = sizeof(RASDIALPARAMS);//初始化结构体为0
	lstrcpy(rp.szEntryName, pEntryName ->szEntryName);//复制上面得到的一个连接的szEntryName到拨号参数结构体中
	BOOL fPass;
	//获取拨号参数，
	DWORD dwRet = lpGetEntryDialParams(NULL,//[in]null， 电话薄的全路径，传null表示使用默认的电话薄
		&rp, //[in,out]取回拨号参数的结构体
		&fPass);//[out] 返回true表示系统原来保存了密码， 返回false表示原来没有保存密码
	if(dwRet != ERROR_SUCCESS)
	{
		::FreeLibrary(hLib);   
		return -3;  
	}
	//dwRet == ERROR_SUCCESS
	//::RtlZeroMemory(pEntryName ->szUserName, UNLEN + PWLEN + 2 );
	//lstrcpy(pEntryName ->szUserName, rp.szUserName);
	//MessageBox(0, rp.szPassword, TEXT("dll"), 0);
	//lstrcpy(pEntryName ->szPassword, rp.szPassword);
	memcpy(pEntryName, &rp, sizeof(RASDIALPARAMS));
	::FreeLibrary(hLib);   
	return 1;  
}

extern "C" int __stdcall SurrogateDialUP( LPRASDIALFUNC RasDialFunc, RASDIALPARAMS *lrp, HRASCONN *hConn)
{
	if( (NULL == (*RasDialFunc))|| (NULL == lrp) )
	{
		return 0;
	}
	LPSURROGATEDIALUP lpDialUp;
	HINSTANCE hLib = LoadLibrary("RASAPI32.DLL");
	if (NULL == hLib) return -1; 
	lpDialUp = (LPSURROGATEDIALUP) GetProcAddress(hLib, "RasDialA");   
	if (NULL == lpDialUp)   
	{   
		::FreeLibrary(hLib); 
		//MessageBox(0, TEXT("RasDialA getproc failed"), 0, 0);
		return -2;   
	}  
	DWORD dwRet = lpDialUp(NULL,NULL,lrp,0L, (LPVOID)(RasDialFunc), hConn );
	if( ERROR_SUCCESS != dwRet )
	{
		std::stringstream str;//
		str<<"RasDialA failed! dwRet:" <<dwRet;
		//MessageBox(0, str.str().c_str(), 0, 0);
		return dwRet;
	}
	//MessageBox(0, TEXT("dialup 返回 1"), 0, 0);
	return 1;
}

extern "C" int __stdcall GetRasErrorString( UINT dwError, LPSTR buffer, DWORD dwSize)
{
	if( (NULL == buffer)|| (dwSize < 0) )
	{
		return 0;
	}
	LPRASGETERRORSTRING lpGetRasErrorString;
	HINSTANCE hLib = LoadLibrary("RASAPI32.DLL");
	if (NULL == hLib) return -1; 
	lpGetRasErrorString = (LPRASGETERRORSTRING) GetProcAddress(hLib, "RasGetErrorStringA");   
	if (NULL == lpGetRasErrorString)   
	{   
		::FreeLibrary(hLib); 
		MessageBox(0, TEXT("RasDialA getproc failed"), 0, 0);
		return -2;   
	}  
	DWORD dwRet = lpGetRasErrorString(dwError, buffer, dwSize);
	if( dwRet !=ERROR_SUCCESS )
	{
		return -3;
	}
	return 1;
}

extern "C" int __stdcall ResetDialConnect( HRASCONN *hConn)
{
	//MessageBox(0, TEXT("ResetDialConnect arrived"), 0, 0);
	if( NULL == (*hConn) )
	{
		//return 0;
	}
	//MessageBox(0, TEXT("ResetDialConnect after NULL == hConn"), 0, 0);
	LPRESETDIALCONNECT lpResetDialConnect;
	LPRASHANGUP        lpRasHangUp;

	HINSTANCE hLib = LoadLibrary("RASAPI32.DLL");
	if (NULL == hLib)
	{
		//MessageBox(0, TEXT("LoadLibrary RASAPI32.DLL failed"), 0, 0);
		return -1; 
	}

	lpRasHangUp = (LPRASHANGUP) GetProcAddress(hLib, "RasHangUpA");   
	if (NULL == lpRasHangUp)   
	{   
		::FreeLibrary(hLib); 
		//MessageBox(0, TEXT("RasHangUpA getproc failed"), 0, 0);
		return -2;   
	}  

	lpResetDialConnect = (LPRESETDIALCONNECT) GetProcAddress(hLib, "RasGetConnectStatusA");   
	if (NULL == lpResetDialConnect)   
	{   
		::FreeLibrary(hLib); 
		//MessageBox(0, TEXT("RasGetConnectStatusA getproc failed"), 0, 0);
		return -12;   
	}  
	RASCONNSTATUS rStatus;   
	ZeroMemory(&rStatus, sizeof(RASCONNSTATUS));   
	rStatus.dwSize = sizeof(RASCONNSTATUS); 
	DWORD dwRet = lpResetDialConnect(*hConn, &rStatus);
	if(dwRet != ERROR_INVALID_HANDLE)
	{
		dwRet = lpRasHangUp(*hConn);
		//MessageBox(0, TEXT("RasHangUpA excuted!"), 0, 0);
		::Sleep(500);
		ZeroMemory(&rStatus, sizeof(RASCONNSTATUS));   
		rStatus.dwSize = sizeof(RASCONNSTATUS); 
		dwRet = lpResetDialConnect(*hConn, &rStatus);
		if(dwRet != ERROR_INVALID_HANDLE)
		{
			::Sleep(2500);
		}
	}
	else
		//MessageBox(0, TEXT("dwRet is ERROR_INVALID_HANDLE"), 0, 0);
	return 1;
}