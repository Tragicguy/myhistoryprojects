#pragma once
#include "../GoodBase/DataDefine.h"
class CAmazonWraper
{
public:
	CAmazonWraper(void);
	~CAmazonWraper(void);
	int Order(GoodOrderStru& objStr);
	void WriteParams(CString strPath,GoodOrderStru &objStr);
	int GetAppPath(CString &strAppPath);
	int Query(GoodOrderStru& objStr);
	int ChangePayType(GoodOrderStru& objStr);
private:
	CString m_strAppPath;
	int Operate(GoodOrderStru& objStr);
};
