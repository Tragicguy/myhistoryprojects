#ifdef TESS_OCR_EXPORTS
#define TESS_OCR_API __declspec(dllexport)
#else
#define TESS_OCR_API __declspec(dllimport)
#endif
TESS_OCR_API int DoOcr(const char* szpicpath,char* szresult);