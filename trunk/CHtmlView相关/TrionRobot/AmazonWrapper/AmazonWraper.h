#pragma once
#include "../AmazonRobot/DataDefine.h"
class CAmazonWraper
{
public:
	CAmazonWraper(void);
	~CAmazonWraper(void);
	int Order(AmazonOrderStru& objStr);
	void WriteParams(CString strPath,AmazonOrderStru &objStr);
	int GetAppPath(CString &strAppPath);
	int Query(AmazonOrderStru& objStr);
	int ChangePayType(AmazonOrderStru& objStr);
private:
	CString m_strAppPath;
	int Operate(AmazonOrderStru& objStr);
};
