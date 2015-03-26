#pragma once
#include "../WebBase/WebBase.h"
#include "../Util/ThreadBase.h"
#include <afxinet.h>
#include "DataType.h"

class CDataProcess : public CWebBase, public CThreadBase
{
public:
	CDataProcess();
public:
	~CDataProcess(void);
	virtual void ThreadProc();
	BOOL InitInfo(CString strQueryUrl, long lInterval, bool bUseZipCompress, bool bGetType);
	void SetShowHwnd(HWND hwnd, HWND hwndNotice){ m_hWndShow = hwnd; m_hWndNotice = hwndNotice; }
	void ReadXmlInfo(CString strPath);
	void ShowInfo(HWND hwnd);
	void ToXml(const char* szPath){m_ConferenceRecordInfo.ToXml(szPath);}
private:
	long m_lQueryInterval;
	CString m_strObject;
	bool m_bUseZipCompress;
	bool m_bDataGetType;
	string m_strHeader;
	HWND m_hWndShow;
	HWND m_hWndNotice;
	ConferenceInfo m_ConferenceRecordInfo;
	ConferenceSourceList m_ConferenceSourceList;
};
