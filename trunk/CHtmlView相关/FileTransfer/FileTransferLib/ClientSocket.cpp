#include "StdAfx.h"
#include ".\clientsocket.h"
#include "GlobalFun.h"

CClientSocket::CClientSocket(void)
{
	m_Socket = INVALID_SOCKET;
}

CClientSocket::~CClientSocket(void)
{
	StopClientSocket();
}

bool CClientSocket::CreateClientSocket(char* pServerAddress,int nPort)
{
	if (gl_pLogger)
		gl_pLogger->log_info("CClientSocket::CreateClientSocket ip:%s ,Port:%d", pServerAddress, nPort);

	sockaddr_in serv;
	serv.sin_addr.s_addr = inet_addr(pServerAddress);
	serv.sin_family = AF_INET;
	serv.sin_port = htons(nPort);
	int addlen = sizeof(serv);

	m_Socket = socket(AF_INET, SOCK_STREAM, 0);

	if (m_Socket == INVALID_SOCKET)
	{
		return false;
	}

	if (connect(m_Socket, (struct sockaddr *)&serv, sizeof(serv)) == 0)
	{
		m_bConnected = true;
		return true;
	}
	if (gl_pLogger)	gl_pLogger->log_info("CClientSocket::CreateClientSocket failed: %d", GetLastError());
	return false;
}

void CClientSocket::StopClientSocket()
{
	if (m_Socket != INVALID_SOCKET)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CClientSocket::StopClientSocket().");
		shutdown(m_Socket, 2);
		closesocket(m_Socket);
		m_Socket = INVALID_SOCKET;
	}
	m_bConnected = false;
}

SOCKET CClientSocket::GetWorkingSocket()
{
	return m_Socket;
}
