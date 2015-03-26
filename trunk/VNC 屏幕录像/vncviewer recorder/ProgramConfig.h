//  Copyright (C) 2002 Shenzhen. All Rights Reserved.
//
//  This file is part of the RTS system, it is created by XPB.
//
//The functionality of this class is to manipulate the config file of RTS server.

#if !defined(AFX_PROGRAMCONFIG_H__4D38B431_090E_4226_9AF1_23D02A4B75D0__INCLUDED_)
#define AFX_PROGRAMCONFIG_H__4D38B431_090E_4226_9AF1_23D02A4B75D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Windows.h"

//Define for the Multiple Language
#define SECTION_UI_TEXT "UI_Text_"
#define SECTION_INFO_TEXT "Information_Text_"

// ProgramConfig.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// ProgramConfig window

//##ModelId=3C86CACC02EE
class ProgramConfig 
{
// Construction
public:
	ProgramConfig();

// Attributes
public:

// Operations
public:
	DWORD ReadUIString(
			LPCTSTR lpParaName,         // parameter name
			LPTSTR lpReturnedString,    // destination buffer
			LPCTSTR lpDefault);           //default value  

	DWORD ReadStringConfigParameter(
			LPCTSTR lpAppName,			// section name
			LPCTSTR lpParaName,         // parameter name
			LPTSTR lpReturnedString,    // destination buffer
			LPCTSTR lpDefault           //default value  
			);

//lpAppName 
	//[in] Pointer to a null-terminated string containing the name of the section 
	//to which the string will be copied. If the section does not exist, it is created.
	//The name of the section is case-independent; the string can be any combination of
	//uppercase and lowercase letters.

//lpParaName 
	//[in] Pointer to the null-terminated string specifying the name 
	//of the parameter whose associated string is to be retrieved. If this 
	//parameter is NULL, all parameter names in the section specified by the 
	//lpAppName parameter are copied to the buffer specified by the 
	//lpReturnedString parameter.

//lpReturnedString 
	//[out] Pointer to the buffer that receives the retrieved string. 

//lpDefault 
	//[in] Pointer to a null-terminated default string. If the lpParaName 
	//key cannot be found in the initialization file, function 
	//copies the default string to the lpReturnedString buffer, and write it 
	//into the config file.
	//This parameter cannot be NULL. 

//Return Values
	//The return value is the number of characters copied to the buffer,
	//not including the terminating null character.

	BOOL WriteStringConfigParameter(
			LPCTSTR lpAppName,			// section name
			LPCTSTR lpParaName, // parameter name
			LPCTSTR lpString    // string to add
			);
//lpAppName 
	//[in] Pointer to a null-terminated string containing the name of the section 
	//to which the string will be copied. If the section does not exist, it is created.
	//The name of the section is case-independent; the string can be any combination of
	//uppercase and lowercase letters.

//lpParaName 
	//[in] Pointer to the null-terminated string containing the name of
	//the parameter to be associated with a string. If the parameter does not exist 
	//in the specified section, it is created. If this parameter is NULL,
	//the entire section, including all entries within the section, is deleted.

//lpString 
	//[in] Pointer to a null-terminated string to be written to the file. 
	//If this parameter is NULL, the parameter pointed to by the lpKeyName parameter 
	//is deleted.

//Return Values
	//If the function successfully copies the string to the config file,
	//the return value is nonzero.

	int ReadIntConfigParameter(
		LPCTSTR lpAppName,			// section name
		LPCTSTR lpParaName, // parameter name
		int iDefault	    // return value if key name not found
		);
//lpAppName 
	//[in] Pointer to a null-terminated string containing the name of the section 
	//to which the string will be copied. If the section does not exist, it is created.
	//The name of the section is case-independent; the string can be any combination of
	//uppercase and lowercase letters.

//nDefault 
	//[in] If the key name cannot be found in the config file, the default value will be returned
	//and function will write it into the config file.

//Return Values
	//The return value is the integer equivalent of the string following 
	//the specified parameter name in the specified config file. If the parameter 
	//is not found, the return value is the specified default value.  


	BOOL WriteIntConfigParameter(
		LPCTSTR lpAppName,			// section name
		LPCTSTR lpParaName, // parameter name
		int iValue			// int value to add
		);
//lpAppName 
	//[in] Pointer to a null-terminated string containing the name of the section 
	//to which the string will be copied. If the section does not exist, it is created.
	//The name of the section is case-independent; the string can be any combination of
	//uppercase and lowercase letters.

//Return Values
	//If the function successfully copies the value to the config file,
	//the return value is nonzero.


	char* SetConfigFileName(
		const char *fileName //config file name. 
		);
//fileName
	//[in] it is best that in the fileName the absolute full path is included.
	//otherwise, if the fileName does not include any path, then the config file
	//will be placed in current path which the RTS is located; if it includes
	//the valid sub path of current path which the RTS is located, the config 
	//file will be placed in the given sub path.

//Return Value
	//This functions returns a pointer to a buffer containing 
	//the absolute path name. If there is an error (for example, 
	//if the value passed in fileName includes a drive letter that is not valid 
	//or cannot be found) the function returns NULL.

// Overrides

// Implementation
public:
	char * GetCurrentPath();

	virtual ~ProgramConfig();

	// char m_sInfoSectionName[64];

	char m_CurrentPath[128];  // Current path

	// Generated message map functions

private:
	char m_sUISectionName[64];

	char * GetLanguage();

	char m_configFileName[256]; //Attribute to keep the config file name, 
								//it include the full path.

protected:

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRAMCONFIG_H__4D38B431_090E_4226_9AF1_23D02A4B75D0__INCLUDED_)
