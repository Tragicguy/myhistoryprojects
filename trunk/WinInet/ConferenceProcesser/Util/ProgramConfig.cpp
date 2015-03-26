// ProgramConfig.cpp : implementation file
//


#include "stdafx.h"

#include "ProgramConfig.h"

#include "afxmt.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCriticalSection CLocker::m_locker;

#ifdef _DEBUG
long gl_index = 0;
#endif

#ifdef _DEBUG
CLocker::CLocker(int i)
#else
CLocker::CLocker()
#endif
{
	BOOL bLock = TRUE;
	bLock = m_locker.Lock();
#ifdef _DEBUG
	gl_index ++;
	m_index = gl_index;
	if (!bLock)
		TRACE("---------------------lock\n");
	CString str;
	str.Format("begin :%d, index:%d\n", m_index, i);
	//TRACE(str);
	ii = i;
#endif
}
CLocker::~CLocker()
{
	BOOL bUnlock = TRUE;
	bUnlock = m_locker.Unlock();
#ifdef _DEBUG
	if (!bUnlock)
		TRACE("---------------------unlock\n");
	CString str;
	str.Format("end :%d          %d\n\n", m_index, ii);
	//TRACE(str);
#endif
}

/////////////////////////////////////////////////////////////////////////////
// ProgramConfig

ProgramConfig::ProgramConfig()
{
	lstrcpy(m_configFileName, "");
	lstrcpy(m_CurrentPath, "");

	char strCurrentPath[_MAX_PATH];
	lstrcpy(strCurrentPath, GetCommandLine());
	int ch = '\\';
	int result = 0;
	char* pdest = strrchr(strCurrentPath, ch );

	if(pdest==NULL)
	{
		GetCurrentDirectory(_MAX_PATH,m_CurrentPath);
		lstrcat(m_CurrentPath,"\\");
	}
	else
	{
		result = pdest - strCurrentPath + 1;
		strCurrentPath[result] = '\0';
		// strncpy(m_CurrentPath, strCurrentPath + 1, sizeof(m_CurrentPath));
		lstrcpy(m_CurrentPath, &strCurrentPath[(strCurrentPath[0] == '\"') ? 1 : 0]);
	}
	//SetCurrentDirectory(m_CurrentPath);

}

ProgramConfig::~ProgramConfig()
{
}


/////////////////////////////////////////////////////////////////////////////
// ProgramConfig message handlers

void ProgramConfig::out()
{
#ifdef _DEBUG
	CString str;
	str.Format("----------->out :%d\n", m_i);
	//TRACE(str);
#endif
}


BOOL ProgramConfig::WriteStringConfigParameter(LPCTSTR lpAppName, LPCTSTR lpParaName, LPCTSTR lpString)
{
#ifdef _DEBUG
	CLocker locker(m_i);
#else
	CLocker locker;
#endif	
	return WritePrivateProfileString(lpAppName, lpParaName, lpString, m_configFileName);
}

CString ProgramConfig::ReadStringConfigParameter(LPCTSTR lpAppName, LPCTSTR lpParaName, LPCTSTR lpDefault)
{
	char lpReturnedString[513] = {0};
	int iCount = GetPrivateProfileString(lpAppName, lpParaName, "", 
		lpReturnedString, 512, m_configFileName);
	if(iCount == 0)
	{
		lstrcpy(lpReturnedString, lpDefault);
	}
	return lpReturnedString;
}

DWORD ProgramConfig::ReadStringConfigParameter(LPCTSTR lpAppName, LPCTSTR lpParaName, LPTSTR lpReturnedString, LPCTSTR lpDefault)
{
#ifdef _DEBUG
	CLocker locker(m_i);
#else
	CLocker locker;
#endif

	int iCount = GetPrivateProfileString(lpAppName, lpParaName, "", 
									lpReturnedString, 512, m_configFileName);
	if(iCount == 0)
	{
		//if(WriteStringConfigParameter(lpAppName, lpParaName, lpDefault))
		//	iCount = strlen(lpDefault);

		lstrcpy(lpReturnedString, lpDefault);
		TRACE("ReadStringConfigParameter faile\n");
	}
	out();
	return iCount;
}

int ProgramConfig::ReadIntConfigParameter(LPCTSTR lpAppName, LPCTSTR lpParaName, int iDefault)
{
#ifdef _DEBUG
	CLocker locker(m_i);
#else
	CLocker locker;
#endif

	char buf[32];

	int iFlag = GetPrivateProfileString(lpAppName, lpParaName, "",
										buf, 32, m_configFileName);
	if(iFlag > 0)
	{
		return atoi(buf);
	}
	else
	{
		//WriteIntConfigParameter(lpAppName, lpParaName, iDefault);
		TRACE("ReadIntConfigParameter faile\n");
		return iDefault;
	}

}

BOOL ProgramConfig::WriteIntConfigParameter(LPCTSTR lpAppName, LPCTSTR lpParaName, int iValue)
{
#ifdef _DEBUG
	CLocker locker(m_i);
#else
	CLocker locker;
#endif

	char buf[64];

	_itoa(iValue,buf,10);
	//sprintf(buf, "%d", iValue); 

	return WritePrivateProfileString(lpAppName, lpParaName, buf, m_configFileName);
}

char* ProgramConfig::SetConfigFileName(const char *fileName, LPCSTR strPath)
{
#ifdef _DEBUG
	CLocker locker(m_i);
#else
	CLocker locker;
#endif

//	return _fullpath(m_configFileName, fileName, 256);
	if (strPath == NULL) 
		strPath = m_CurrentPath; 
	lstrcpy(m_configFileName, strPath);
	lstrcat(m_configFileName, fileName);

	GetLanguage(); // Set the UI_Language section value

	return m_configFileName;
}

char * ProgramConfig::GetLanguage()
{
	char strLanguage[128];
	memset(strLanguage, 0, 128);

	ReadStringConfigParameter("RTS Options With String Type","LanguageName", strLanguage,"ENGLISH");

	/*
	switch(PRIMARYLANGID(GetUserDefaultLangID()))
	{

		case LANG_ENGLISH: 
			{
				strncpy(strLanguage,"ENGLISH", sizeof(strLanguage) - 1);
				break;
			}

		case LANG_DANISH:
			{
				strncpy(strLanguage,"DANISH", sizeof(strLanguage) - 1);
				break;
			}

		case LANG_CHINESE:
			{
				strncpy(strLanguage,"CHINESE", sizeof(strLanguage) - 1);
				break;
			}

		case LANG_FRENCH:
			{
				strncpy(strLanguage,"FRENCH", sizeof(strLanguage) - 1);
				break;
			}

		case LANG_GERMAN:
			{
				strncpy(strLanguage,"GERMAN", sizeof(strLanguage) - 1);
				break;
			}

		case LANG_SWEDISH:
			{
				strncpy(strLanguage,"SWEDISH", sizeof(strLanguage) - 1);
				break;
			}

		case LANG_ITALIAN:
			{
				strncpy(strLanguage,"ITALIAN", sizeof(strLanguage) - 1);
				break;
			}

		case LANG_DUTCH:
			{
				strncpy(strLanguage,"DUTCH", sizeof(strLanguage) - 1);
				break;
			}

		case LANG_FINNISH:
			{
				strncpy(strLanguage,"FINNISH", sizeof(strLanguage) - 1);
				break;
			}

		case LANG_SPANISH:
			{
				strncpy(strLanguage,"SPANISH", sizeof(strLanguage) - 1);
				break;
			}

		default:
			{
				MessageBox(NULL, "Can't support current language, use English to display the UI.", "Server", MB_TOPMOST | MB_OK | MB_ICONSTOP);
				strncpy(strLanguage,"ENGLISH", sizeof(strLanguage) - 1);
				break;
			}
	}
	*/
	lstrcpy(m_sUISectionName, SECTION_UI_TEXT);
	// strncpy(m_sInfoSectionName, SECTION_INFO_TEXT, sizeof(m_sInfoSectionName) - 1);

	lstrcat(m_sUISectionName, strLanguage);
	// strncat(m_sInfoSectionName, strLanguage, sizeof(strLanguage));

	return NULL;
}

char * ProgramConfig::GetCurrentPath()
{
	return m_CurrentPath;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//从配置文件中读取提示信息
DWORD ProgramConfig::ReadUIString(LPCTSTR lpParaName,				// parameter name
                                  LPTSTR lpReturnedString,			// destination buffer
                                  LPCTSTR lpDefault,				// default value  
								  LPCTSTR strLanguage/* = CHINESE*/)	// Language type
{
#ifdef _DEBUG
	CLocker locker(m_i);
#else
	CLocker locker;
#endif
	char sLanguageSection[MAX_PATH];
	lstrcpy(sLanguageSection, "UI_Text_CHINESE");
	if(strcmp(strLanguage, CHINESE) == 0)
	//if(strLanguage == CHINESE)
	{
		lstrcpy(sLanguageSection, "UI_Text_CHINESE");
	}
	//else if(strLanguage == ENGLISH)
	else if(strcmp(strLanguage, ENGLISH) == 0)
	{
		lstrcpy(sLanguageSection, "UI_Text_ENGLISH");
	}
	char strConfigFileName[MAX_PATH]; //Attribute to keep the config file name
	lstrcpy(strConfigFileName, m_CurrentPath);
	lstrcat(strConfigFileName, "RTSUI.Ini");

	DWORD dwLen = GetPrivateProfileString(sLanguageSection, lpParaName, lpDefault, 
		lpReturnedString, MAX_PATH, strConfigFileName);
	// Convert of \n
	char *pLeft = lpReturnedString; 
	while ((pLeft = strchr(pLeft, '\\')) != NULL)
	{
		if (pLeft[1] == 'n')
		{
			pLeft[0] = '\r';
			pLeft[1] = '\n';
		}
		pLeft ++;
	}
	return dwLen;
}


