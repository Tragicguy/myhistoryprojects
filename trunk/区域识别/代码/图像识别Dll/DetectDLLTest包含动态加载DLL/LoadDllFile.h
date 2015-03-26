#pragma once
typedef CPoint (*MYPROC)(CString,CString);   //定义函数指针
class LoadDllFile
{
public:
	LoadDllFile(void);
	~LoadDllFile(void);
	HINSTANCE GetDllInstance(CString dllPath);
	FARPROC GetFuncProc(HINSTANCE dllInstance,CString funcName);
};
