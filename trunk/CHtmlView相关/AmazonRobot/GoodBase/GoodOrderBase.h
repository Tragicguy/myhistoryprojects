#ifndef  _CGOODORDERBASE_DEFINE__
#define  _CGOODORDERBASE_DEFINE__
#include "DataDefine.h"
#include "../WebBase/WebBase.h"
#include "ThreadBase.h"
#include <afxmt.h>
class CGoodOrderBase : public CWebBase, public CThreadBase
{
public:
	CGoodOrderBase(void);
public:
	virtual ~CGoodOrderBase(void);

	virtual bool Login(CString strUser, CString strPwd, CString strCheckCode = "") = 0;
	virtual bool AddToCard(CString strGoodNo, int nGoodNum) = 0;
	virtual int  Order(GoodOrderStru* pGoodOrderStr, int nTimeOut = 3 * 60 * 1000);
	virtual int  DoOrder(GoodOrderStru* pGoodOrderStr) = 0;
	virtual CString GetRetString(int iCode) = 0;
	virtual void ThreadProc();
	bool GetAllHiddenValue(CString strSrc, CString& strRet);
private:
	GoodOrderStru* m_pGoodOrderStr;
	CEvent m_eQueryInfoEvent;
	int m_nRetCode;
};
#endif