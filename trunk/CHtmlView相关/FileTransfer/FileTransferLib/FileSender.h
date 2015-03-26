#pragma once
#include "threadbase.h"

class CFileSender
{
public:
	CFileSender(void);
	~CFileSender(void);

public:
	virtual void DoSend();
	virtual void OnThreadStop();
	void SetFileSenderInfo(CFile* pFile, SOCKET sock, HWND ParentHWND);
	void StopSending();

private:
	CFile* m_pFile;
	SOCKET m_Socket;
	HWND m_ParentHwnd;
	bool m_bRun;
	
	bool PackAndSendRequest(short nParam);
	bool WriteExact(SOCKET sock, char *buf, int bytes);
	bool ReadExact(SOCKET sock, char *inbuf, int wanted); 
	bool QueryStatus(); 

	bool m_bStopbyServer;
};
