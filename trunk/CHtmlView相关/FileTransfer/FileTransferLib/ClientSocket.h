#pragma once
#include "basesocket.h"

class CClientSocket : public CBaseSocket
{
public:
	CClientSocket(void);
	~CClientSocket(void);

public:
	bool	CreateClientSocket(char* pServerAddress,int nPort);
	void	StopClientSocket();
	SOCKET	GetWorkingSocket();

private:
	SOCKET	m_Socket;
};
