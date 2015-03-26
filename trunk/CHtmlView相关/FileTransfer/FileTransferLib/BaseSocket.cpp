#include "StdAfx.h"
#include ".\basesocket.h"
#include "GlobalFun.h"

bool CBaseSocket::ReadExact(SOCKET sock, char *inbuf, int wanted)
{
	if (sock == INVALID_SOCKET)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CBaseSocket::ReadExact sock == INVALID_SOCKET");
		m_bConnected = false;
		return false;
	}

	int nBuffered = 0;
	int nRead;
	int n = 0;
	while (wanted > 0)
	{
		nRead = 0;

		nRead = recv(sock, &inbuf[nBuffered], wanted, 0);
		int nError = WSAGetLastError();
		if (nError != 0 && nError != WSAEWOULDBLOCK)
		{
			if (gl_pLogger)
				gl_pLogger->log_info("Socket Error, WSAGetLastError() = %d, Socket=%d", nError, sock);
		}
		if (nRead <= 0)
		{
			
			if (gl_pLogger)
				gl_pLogger->log_info("CBaseSocket::ReadExact nRead <= 0 nRead: %d, Socket=%d", nRead, sock);
			n ++;
			//if (n < 5)
			//	continue;
			m_bConnected = false;
			return false;
		}

		if (nRead == 0)
		{
			if (gl_pLogger)
				gl_pLogger->log_info("CBaseSocket::ReadExact nRead = 0, Socket=%d", sock);
		}

		wanted -= nRead;
		nBuffered += nRead;
	}

	return true;
}

int CBaseSocket::Read(SOCKET sock, char *inbuf, int wanted)
{
	if (sock == INVALID_SOCKET)
	{
		m_bConnected = false;
		return -1;
	}

	int nRead = recv(sock, inbuf, wanted, 0);
	m_bConnected = nRead > 0;
	return nRead;
}

bool CBaseSocket::WriteExact(SOCKET sock, char *buf, int bytes)
{
	if (sock == INVALID_SOCKET)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CBaseSocket::WriteExact sock == INVALID_SOCKET");
		m_bConnected = false;
		return false;
	}

	int nBuffered = 0;
	int nWrite;

	while (bytes > 0)
	{
		nWrite = 0;

		nWrite = send(sock, &buf[nBuffered], bytes, 0);

		if (nWrite <= 0)
		{
			if (gl_pLogger)
				gl_pLogger->log_info("CBaseSocket::WriteExact nWrite <= 0 nWrite: %d", nWrite);
			m_bConnected = false;
			return false;
		}

		bytes -= nWrite;
		nBuffered += nWrite;
	}

	return true;
}



