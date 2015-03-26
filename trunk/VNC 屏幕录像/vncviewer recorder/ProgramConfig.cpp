// ProgramConfig.cpp : implementation file
//


//#include "stdafx.h"

#include "ProgramConfig.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ProgramConfig

ProgramConfig::ProgramConfig()
{
	lstrcpy(m_configFileName, "");
	strcpy(m_CurrentPath,"");

	char strCurrentPath[_MAX_PATH];
	strcpy(strCurrentPath, GetCommandLine());
	int ch = '\\';
	int result = 0;
	char* pdest = strrchr(strCurrentPath, ch );

	if(pdest==NULL)
	{
		GetCurrentDirectory(_MAX_PATH,strCurrentPath);
	}
	else
	{
		result = pdest - strCurrentPath + 1;
		strCurrentPath[result] = '\0';
		// strcpy(m_CurrentPath, strCurrentPath + 1);
		strcpy(m_CurrentPath, &strCurrentPath[(strCurrentPath[0] == '\"') ? 1 : 0]);
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

	itoa(iValue,buf,10);
	//sprintf(buf, "%d", iValue); 

	return WritePrivateProfileString(lpAppName, lpParaName, buf, m_configFileName);
}

char* ProgramConfig::SetConfigFileName(const char *fileName)
{
//	return _fullpath(m_configFileName, fileName, 256);

	strcpy(m_configFileName,m_CurrentPath);
	strcat(m_configFileName, fileName);
	GetLanguage(); // Set the UI_Language section value

	return m_configFileName;
}

char * ProgramConfig::GetLanguage()
{
	char strLanguage[128];
	memset(strLanguage,0,128);

	ReadStringConfigParameter("RTS Options With String Type","LanguageName", strLanguage,"ENGLISH");

	/*
	switch(PRIMARYLANGID(GetUserDefaultLangID()))
	{

		case LANG_ENGLISH: 
			{
				strcpy(strLanguage,"ENGLISH");
				break;
			}

		case LANG_DANISH:
			{
				strcpy(strLanguage,"DANISH");
				break;
			}

		case LANG_CHINESE:
			{
				strcpy(strLanguage,"CHINESE");
				break;
			}

		case LANG_FRENCH:
			{
				strcpy(strLanguage,"FRENCH");
				break;
			}

		case LANG_GERMAN:
			{
				strcpy(strLanguage,"GERMAN");
				break;
			}

		case LANG_SWEDISH:
			{
				strcpy(strLanguage,"SWEDISH");
				break;
			}

		case LANG_ITALIAN:
			{
				strcpy(strLanguage,"ITALIAN");
				break;
			}

		case LANG_DUTCH:
			{
				strcpy(strLanguage,"DUTCH");
				break;
			}

		case LANG_FINNISH:
			{
				strcpy(strLanguage,"FINNISH");
				break;
			}

		case LANG_SPANISH:
			{
				strcpy(strLanguage,"SPANISH");
				break;
			}

		default:
			{
				MessageBox(NULL, "Can't support current language, use English to display the UI.", "Server", MB_TOPMOST | MB_OK | MB_ICONSTOP);
				strcpy(strLanguage,"ENGLISH");
				break;
			}
	}
	*/
	strcpy(m_sUISectionName, SECTION_UI_TEXT);
	// strcpy(m_sInfoSectionName, SECTION_INFO_TEXT);

	strcat(m_sUISectionName, strLanguage);
	// strcat(m_sInfoSectionName,strLanguage);

	return NULL;
}

char * ProgramConfig::GetCurrentPath()
{
	return m_CurrentPath;
}

DWORD ProgramConfig::ReadUIString(LPCTSTR lpParaName,         // parameter name
                                  LPTSTR lpReturnedString,    // destination buffer
                                  LPCTSTR lpDefault)           //default value  
{
	char strConfigFileName[MAX_PATH]; //Attribute to keep the config file name
	strcpy(strConfigFileName,m_CurrentPath);
	strcat(strConfigFileName, "RTSUI.ini");
    
	DWORD dwLen = GetPrivateProfileString(m_sUISectionName, lpParaName, lpDefault, 
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

