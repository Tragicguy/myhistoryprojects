#ifdef DETECTDLL_API
#else
#define DETECTDLL_API extern "C" _declspec(dllimport)
#endif