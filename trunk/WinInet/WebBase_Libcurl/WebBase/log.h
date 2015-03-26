#ifndef  _LOG_DEFINE__
#define  _LOG_DEFINE__
#include <string>
#include <fstream>
#include <time.h>
using namespace std;
#pragma warning(disable:4996)

class log
{
public:
	void loginfo(const char * szInfo)
	{
		setlocale(LC_ALL, "Chinese-simplified");		
		time_t t = time(NULL);
		char szBuf[50] = {0};
		struct tm *ptm = localtime(&t);
		strftime(szBuf, 49, "%Y-%m-%d %H:%M:%S", ptm);
		fstream fsLog(m_strFileName.c_str(), ios_base::out | ios_base::app);
		fsLog << szBuf << "\t " << szInfo<<endl;
		fsLog.flush();
		fsLog.close();
	}
	void SetLogPath(const char* lpszPath){m_strFileName = string(lpszPath);}
private:
	string m_strFileName;
};
#endif