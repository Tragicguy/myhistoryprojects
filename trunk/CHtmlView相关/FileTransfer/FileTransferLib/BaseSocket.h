#pragma once

class CBaseSocket
{
public:
	CBaseSocket(){ m_bConnected = false; }
	bool	ReadExact(SOCKET sock, char *inbuf, int wanted);
	bool	WriteExact(SOCKET sock,char *buf, int bytes);
	int		Read(SOCKET sock, char *inbuf, int wanted);
	bool	IsConnected(){ return m_bConnected; }
protected:
	bool m_bConnected;
};

