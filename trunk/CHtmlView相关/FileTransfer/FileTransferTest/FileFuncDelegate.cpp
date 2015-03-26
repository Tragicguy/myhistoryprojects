#include "StdAfx.h"
#include "FileFuncDelegate.h"

CFileFuncDelegate::CFileFuncDelegate(void)
{
	m_hInst = NULL;

	m_funcCreateFileServer = NULL;
	m_funcStopFileServer = NULL;

	m_funcCreateFileClient = NULL;
	m_funcStopFileClient = NULL;
}

CFileFuncDelegate::~CFileFuncDelegate(void)
{
	ReleaseFileDLL();
}

bool CFileFuncDelegate::InitFileDLLFunction()
{
	ReleaseFileDLL();

	m_hInst = LoadLibrary(FileDllName);
	
	if( m_hInst != NULL )
	{
		m_funcCreateFileServer = 
			(CreateFileServer) GetProcAddress(m_hInst, 
				CreateServerFuncName); 
		
		if(m_funcCreateFileServer == NULL) 
		{
			ReleaseFileDLL();		
			return false;
		}

		m_funcStopFileServer = 
			(StopFileServer) GetProcAddress(m_hInst, 
				StopServerFuncName); 

		if(m_funcStopFileServer == NULL)
		{
			ReleaseFileDLL();		
			return false;
		}
		m_funcCreateFileClient = 
			(CreateFileClient) GetProcAddress(m_hInst, 
				CreateClientFuncName); 

		if(m_funcCreateFileClient == NULL)
		{
			ReleaseFileDLL();		
			return false;
		}

		m_funcStopFileClient = 
			(StopFileClient) GetProcAddress(m_hInst, 
				StopClientFuncName); 

		if(m_funcStopFileClient == NULL)
		{
			ReleaseFileDLL();		
			return false;
		}

		return true;
	}
	

	return false;
}

void CFileFuncDelegate::ReleaseFileDLL()
{
	if(m_hInst != NULL)
	{
		m_funcCreateFileServer = NULL;
		m_funcStopFileServer = NULL;

		m_funcCreateFileClient = NULL;
		m_funcStopFileClient = NULL;

		FreeLibrary(m_hInst);
		m_hInst = NULL;
	}
}