#include "StdAfx.h"
#include "DataProcess.h"
#include "../Util/FileUtil.h"
#pragma warning(disable:4018)
bool ConferenceInfo::SetRecord(TiXmlElement* boxElement, list<ConferenceRecord>::iterator dataRecord)
{
	TiXmlElement *recordElement = TinyXmlFunction::NewElement("record", boxElement);
	if(!recordElement) return false;

	TinyXmlFunction::NewElement("roomid", recordElement, dataRecord->roomid);
	TinyXmlFunction::NewElement("roomname", recordElement, dataRecord->roomname, true);
	TinyXmlFunction::NewElement("starttime", recordElement, dataRecord->starttime);
	TinyXmlFunction::NewElement("endtime", recordElement, dataRecord->endtime);
	TinyXmlFunction::NewElement("topic", recordElement, dataRecord->topic, true);
	TinyXmlFunction::NewElement("chairman", recordElement, dataRecord->chairman);
	TinyXmlFunction::NewElement("bookpeople", recordElement, dataRecord->bookpeople);
	TiXmlElement *peoplesElement = TinyXmlFunction::NewElement("peoples", recordElement);
	if(!peoplesElement) return false;
	list<CString>::iterator it;
	for (it = dataRecord->peoples.begin(); it != dataRecord->peoples.end(); it++)
	{
		TinyXmlFunction::NewElement("people", peoplesElement, (const char*)(*it), true);
	}
	TinyXmlFunction::NewElement("currentfloor", recordElement, dataRecord->currentfloor ? "true" : "false");

	TiXmlElement *displayformatElement = TinyXmlFunction::NewElement("displayformat", recordElement);
	if(!displayformatElement) return false;
	TinyXmlFunction::NewElement("ishighlight", displayformatElement, dataRecord->displayformat.ishighlight ? "true" : "false");
	return true;
}

bool ConferenceInfo::ToXml(const char* szPath)
{
	TiXmlDocument xmlDocument;
	TiXmlDeclaration *pDeclaration = new TiXmlDeclaration(_T("1.0"),_T("utf-8"),_T(""));
	if (!pDeclaration)	return false;
	xmlDocument.LinkEndChild(pDeclaration);

	TiXmlElement *rootElement = TinyXmlFunction::DocNewElement("root", &xmlDocument);
	if (!rootElement)	return false;
	rootElement->SetAttribute("updatetime", updatetime);
	rootElement->SetAttribute("version", version);
	
	TiXmlElement *boxElement = TinyXmlFunction::NewElement("box", rootElement);
	if (!boxElement)	return false;
	boxElement->SetAttribute("boxnos", boxnos);

	list<ConferenceRecord>::iterator it;
	for (it = record.begin(); it != record.end(); it++)
	{
		SetRecord(boxElement, it);
	}
	xmlDocument.SaveFile(szPath);//保存到文件
	return true;
}

CDataProcess::CDataProcess(void)
{
	m_hWndShow = NULL;
	m_strHeader = "Content-Type:application/x-www-form-urlencoded";
	m_strHeader.append("\r\nUser-Agent:Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.2; SV1; .NET CLR 1.1.4322; .NET CLR 2.0.50727; .NET CLR 3.0.04506.30; .NET CLR 3.0.04506.648)");
	m_strHeader.append("\r\nAccept: */*");
	m_strHeader.append("\r\nConnection: Keep-Alive");	
	m_strHeader.append("\r\nAccept-Language:zh-cn");
	m_strHeader.append("\r\nConnection: Keep-Alive");	
}


CDataProcess::~CDataProcess(void)
{
}

BOOL CDataProcess::InitInfo(CString strQueryUrl, long lInterval, bool bUseZipCompress, bool bGetType)
{
	m_lQueryInterval = lInterval;
	m_bDataGetType = bGetType;
	m_bUseZipCompress = bUseZipCompress;
	DWORD dwServiceType = 0;
	CString strServer = "";
	CString strObject = "";
	INTERNET_PORT nPort = 80;
	BOOL bRet = AfxParseURL(strQueryUrl, dwServiceType, strServer, m_strObject, nPort);
	if (!bRet)
	{
		return FALSE;
	}
	if (nPort == 443)
	{
		SetHttpType(0);
	}
	SetDefaultServer(LPCTSTR(strServer));
	return TRUE;
}

void CDataProcess::ThreadProc()
{
	long lLastQueryTime = 0;
	string strHeader = m_strHeader;
	int iRet = -1;
	CString strRet = "";
	CString strPath = CFunction::GetAppPath().c_str();
	strPath += "xmlLog";
	CString strFilePath = "";
	if (!CFileUtil::IsDirectoryExist(strPath))
	{
		CFileUtil::CreateDirectory(strPath);
	}
	while (IsRunning())
	{
		if (::GetTickCount() - lLastQueryTime < m_lQueryInterval)
		{
			Sleep(500);
			continue;
		}
		strHeader = m_strHeader;
		if (m_bUseZipCompress)
			strHeader.append("\r\nAccept-Encoding:gzip, deflate");
		strFilePath.Format("%s\\%s.xml", strPath, CTime::GetCurrentTime().Format("%Y%m%d%H%M%S"));

		int iRet = GetPage(m_strObject, strHeader.c_str(), NULL, 0, true, strFilePath);
		if(iRet < 0)
		{
			Sleep(500);
			continue;
		}
		lLastQueryTime = ::GetTickCount();
		ReadXmlInfo(strFilePath);
		if(m_hWndShow)	ShowInfo(m_hWndShow);
		CString strShow = "最后抓取时间: ";
		if(m_hWndNotice) ::SetWindowText(m_hWndNotice,strShow + CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"));
	}
}

void CDataProcess::ReadXmlInfo(CString strPath)
{
	try
	{
		m_ConferenceSourceList.clear();
		ConferenceSource conferenceSource;
		TiXmlDocument xmlDoc(strPath);
		xmlDoc.LoadFile() ;
		TiXmlElement* xmlRootElement = 0;
		xmlRootElement = xmlDoc.RootElement();
		if (xmlRootElement == NULL)
			return;
		TiXmlElement* xmlConfElement = xmlRootElement->FirstChildElement();
		while (xmlConfElement)
		{
			conferenceSource.ConfId = xmlConfElement->FirstChildElementText("ConfId");
			conferenceSource.ConfName = xmlConfElement->FirstChildElementText("ConfName");
			conferenceSource.StartTime = xmlConfElement->FirstChildElementText("StartTime");
			conferenceSource.EndTime = xmlConfElement->FirstChildElementText("EndTime");
			conferenceSource.Department = xmlConfElement->FirstChildElementText("Department");
			TiXmlElement* xmlRoomElement = xmlConfElement->FirstChildElement("Room");
			if (xmlRoomElement)
			{
				conferenceSource.Room.RoomName = xmlRoomElement->FirstChildElementText("RoomName");
			}		
			conferenceSource.ParticipantNumber = xmlConfElement->FirstChildElementText("ParticipantNumber");
			conferenceSource.ConfState = xmlConfElement->FirstChildElementText("ConfState");
			conferenceSource.ConfType = xmlConfElement->FirstChildElementText("ConfType");
			m_ConferenceSourceList.push_back(conferenceSource);
			xmlConfElement = xmlConfElement->NextSiblingElement();
		}
	}
	catch (CException* e)
	{
	}
	catch(...){}
}

void CDataProcess::ShowInfo(HWND hwnd)
{
	CListCtrl* pList = (CListCtrl*)CWnd::FromHandle(hwnd);
	if(!pList) return;
	pList->DeleteAllItems();
	ConferenceSourceList::iterator it;
	for (it = m_ConferenceSourceList.begin(); it != m_ConferenceSourceList.end(); it++)
	{
		int nCount = pList->GetItemCount();
		pList->InsertItem(nCount, "");
		pList->SetItemText(nCount, 0, it->ConfId);
		pList->SetItemText(nCount, 1, CFunction::ConvertUtf8ToGBKEx(it->ConfName));
		pList->SetItemText(nCount, 2, it->StartTime);
		pList->SetItemText(nCount, 3, it->EndTime);
		pList->SetItemText(nCount, 4, CFunction::ConvertUtf8ToGBKEx(it->Department));
		pList->SetItemText(nCount, 5, CFunction::ConvertUtf8ToGBKEx(it->Room.RoomName));
		pList->SetItemText(nCount, 6, it->ParticipantNumber);
		pList->SetItemText(nCount, 7, CFunction::ConvertUtf8ToGBKEx(it->ConfState));
		pList->SetItemText(nCount, 8, CFunction::ConvertUtf8ToGBKEx(it->ConfType));
	}

}