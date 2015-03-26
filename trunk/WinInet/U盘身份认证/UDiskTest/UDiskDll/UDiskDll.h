// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 UDISKDLL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// UDISKDLL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef UDISKDLL_EXPORTS
#define UDISKDLL_API __declspec(dllexport)
#else
#define UDISKDLL_API __declspec(dllimport)
#endif
#define GDI_VOLUMENAME 0
#define GDI_VOLUMESERIALNUMBER 1
#define GDI_VOLUMEFILESYSTEM 2
#define GDI_VOLUMETYPE 3
#define GDI_VOLUMESIZE 4
#define GDI_VOLUMEFREESIZE 5
// 此类是从 UDiskDll.dll 导出的
class UDISKDLL_API CUDiskDll {
public:
	CUDiskDll(void);
	// TODO: 在此添加您的方法。
};

extern UDISKDLL_API int nUDiskDll;

UDISKDLL_API int fnUDiskDll(void);
CString GetDriverInfomation(char* bAlpha);
extern "C" UDISKDLL_API bool GetDiskSerial(char* szDisk,char* szSerial);
extern "C" UDISKDLL_API bool GetDisks(char* szDisks);