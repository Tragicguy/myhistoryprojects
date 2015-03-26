#include "StdAfx.h"
#include ".\serversocket.h"
#include "GlobalFun.h"

CServerSocket::CServerSocket(void)
{
	m_ListenSocket = INVALID_SOCKET;
	m_NewSocket = INVALID_SOCKET;
	m_bConnected = false;
}

CServerSocket::~CServerSocket(void)
{
	StopServerSocket();
}

bool CServerSocket::CreateServerSocket(int nPort)
{
	sockaddr_in serv;
	serv.sin_addr.s_addr = INADDR_ANY;  //inet_addr("127.0.0.1");
	serv.sin_family = AF_INET;
	serv.sin_port = htons(nPort);
	int addlen = sizeof(serv);

	m_ListenSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (m_ListenSocket == INVALID_SOCKET)
	{
		return false;
	}

	if (bind(m_ListenSocket, (sockaddr*)&serv, addlen))
	{
		closesocket(m_ListenSocket);
		m_ListenSocket = INVALID_SOCKET;
		return false;
	}
	else
	{
		if (0 == listen(m_ListenSocket, 5))
		{
			return true;
		}
		else
		{
			closesocket(m_ListenSocket);
			m_ListenSocket = INVALID_SOCKET;
			return false;
		}
	}
}

void CServerSocket::StopServerSocket()
{
	try
	{
		if (m_NewSocket != INVALID_SOCKET)
		{
			shutdown(m_NewSocket, 2);
			closesocket(m_NewSocket);
			m_NewSocket = INVALID_SOCKET;
		}

		if (m_ListenSocket != INVALID_SOCKET)
		{
			shutdown(m_ListenSocket, 2);
			closesocket(m_ListenSocket);
			m_ListenSocket = INVALID_SOCKET;
		}
		m_bConnected = false;
	}
	catch(...){}
}

SOCKET CServerSocket::GetWorkingSocket()
{
	return m_NewSocket;
}

//Accept a new socket
bool CServerSocket::WaitForConnection()
{
	if (gl_pLogger)
		gl_pLogger->log_info("CServerSocket::WaitForConnection()...");
	if (m_ListenSocket != INVALID_SOCKET)
	{
		// receive a connection
		m_NewSocket = accept(m_ListenSocket, 0, 0);
		if (m_NewSocket == INVALID_SOCKET)
		{
			if (gl_pLogger)
				gl_pLogger->log_info("CServerSocket::WaitForConnection accept  m_NewSocket == INVALID_SOCKET");
		}
		else
		{
			if (gl_pLogger)
				gl_pLogger->log_info("CServerSocket::WaitForConnection accept  m_NewSocket = %d", m_NewSocket);
		}
		m_bConnected = m_NewSocket != INVALID_SOCKET;
		return m_bConnected;
	}
	else
	{
		if (gl_pLogger)
			gl_pLogger->log_info("m_NewSocket == INVALID_SOCKET");
	}

	return false;
}
