#pragma once
#include "basesocket.h"

class CServerSocket : public CBaseSocket
{
public:
	CServerSocket(void);
	~CServerSocket(void);

public:
	bool		CreateServerSocket(int nPort);
	void		StopServerSocket();
	SOCKET		GetWorkingSocket();
	bool		WaitForConnection();

private:
	SOCKET		m_ListenSocket;
	SOCKET		m_NewSocket;
};
