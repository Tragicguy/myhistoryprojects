#include "StdAfx.h"
#include ".\loaddllfile.h"

LoadDllFile::LoadDllFile(void)
{
}

LoadDllFile::~LoadDllFile(void)
{
}
HINSTANCE LoadDllFile::GetDllInstance(CString dllPath)
{
	return LoadLibrary(dllPath);
}

FARPROC LoadDllFile::GetFuncProc(HINSTANCE dllInstance,CString funcName)
{	
	if (dllInstance == NULL)
	{
		return NULL;
	}
	else
	{
		return GetProcAddress(dllInstance,funcName);
	}
	
}