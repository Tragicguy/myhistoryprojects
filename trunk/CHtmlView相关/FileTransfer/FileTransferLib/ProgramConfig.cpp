// ProgramConfig.cpp : implementation file
//


#include "stdafx.h"

#include "ProgramConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ProgramConfig

ProgramConfig::ProgramConfig()
{
	lstrcpy(m_configFileName, "");
	lstrcpy(m_CurrentPath, "");

	ZeroMemory(m_CurrentPath, MAX_PATH);
	DWORD dwCount = GetModuleFileName(NULL, m_CurrentPath, MAX_PATH);
	if (dwCount > 0)
		*(strrchr(m_CurrentPath, '\\') + 1) = '\0';
	else
	{
		GetCurrentDirectory(_MAX_PATH,m_CurrentPath);
		lstrcat(m_CurrentPath,"\\");
	}
}

ProgramConfig::~ProgramConfig()
{
}


/////////////////////////////////////////////////////////////////////////////
// ProgramConfig message handlers


BOOL ProgramConfig::WriteStringConfigParameter(LPCTSTR lpAppName, LPCTSTR lpParaName, LPCTSTR lpString)
{
	return WritePrivateProfileString(lpAppName, lpParaName, lpString, m_configFileName);
}

DWORD ProgramConfig::ReadStringConfigParameter(LPCTSTR lpAppName, LPCTSTR lpParaName, LPTSTR lpReturnedString, LPCTSTR lpDefault)
{
	int iCount = GetPrivateProfileString(lpAppName, lpParaName, "", 
									lpReturnedString, MAX_PATH, m_configFileName);
	if(iCount == 0)
	{
		if(WriteStringConfigParameter(lpAppName, lpParaName, lpDefault))
			iCount = strlen(lpDefault);

		lstrcpy(lpReturnedString, lpDefault);
	}
	return iCount;
}

int ProgramConfig::ReadIntConfigParameter(LPCTSTR lpAppName, LPCTSTR lpParaName, int iDefault)
{
	char buf[32];

	int iFlag = GetPrivateProfileString(lpAppName, lpParaName, "",
										buf, 32, m_configFileName);
	if(iFlag > 0)
	{
		return atoi(buf);
	}
	else
	{
		WriteIntConfigParameter(lpAppName, lpParaName, iDefault);
		return iDefault;
	}

}

BOOL ProgramConfig::WriteIntConfigParameter(LPCTSTR lpAppName, LPCTSTR lpParaName, int iValue)
{
	char buf[64];

	_itoa(iValue,buf,10);
	//sprintf(buf, "%d", iValue); 

	return WritePrivateProfileString(lpAppName, lpParaName, buf, m_configFileName);
}

char* ProgramConfig::SetConfigFileName(const char *fileName, LPCSTR strPath)
{
//	return _fullpath(m_configFileName, fileName, 256);
	if (strPath == NULL) 
		strPath = m_CurrentPath; 
	lstrcpy(m_configFileName, strPath);
	lstrcat(m_configFileName, fileName);

	//GetLanguage(); // Set the UI_Language section value

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

DWORD ProgramConfig::ReadUIString(LPCTSTR lpParaName,				// parameter name
                                  LPTSTR lpReturnedString,			// destination buffer
                                  LPCTSTR lpDefault,				// default value  
								  LPCTSTR strLanguage/* = CHINESE*/)	// Language type
{
	char sLanguageSection[MAX_PATH];
	lstrcpy(sLanguageSection, "UI_Text_CHINESE");
	if(StrCmp(strLanguage, CHINESE) == 0)
	//if(strLanguage == CHINESE)
	{
		lstrcpy(sLanguageSection, "UI_Text_CHINESE");
	}
	//else if(strLanguage == ENGLISH)
	else if(StrCmp(strLanguage, ENGLISH) == 0)
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


