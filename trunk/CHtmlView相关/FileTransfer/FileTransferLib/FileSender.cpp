#include "StdAfx.h"
#include "FileSender.h"
#include "FileDefines.h"


CFileSender::CFileSender(void)
{
	m_pFile = NULL;
	m_Socket = INVALID_SOCKET;
	m_ParentHwnd = NULL;
	m_bRun = false;
	m_bStopbyServer = false;
}

CFileSender::~CFileSender(void)
{
}

void CFileSender::SetFileSenderInfo(CFile* pFile, SOCKET sock, HWND ParentHwnd)
{
	m_pFile = pFile;
	m_Socket = sock;
	m_ParentHwnd = ParentHwnd;
}

bool CFileSender::PackAndSendRequest(short nParam)
{
	FilePackHeader header;
	header.nType = FILE_REQUEST;
	header.nParam = nParam;

	if (!WriteExact(m_Socket, (char*)(&header), sizeof(header)))
	{
		return false;
	}
	return true;
}

bool CFileSender::WriteExact(SOCKET sock, char *buf, int bytes)
{
	if (sock == INVALID_SOCKET)
	{
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
			return false;
		}

		bytes -= nWrite;
		nBuffered += nWrite;
	}

	return true;
}

bool CFileSender::ReadExact(SOCKET sock, char *inbuf, int wanted)
{
	if (sock == INVALID_SOCKET)
	{
		return false;
	}

	int nBuffered = 0;
	int nRead;
	
	while (wanted > 0)
	{
		nRead = 0;

		nRead = recv(sock, &inbuf[nBuffered], wanted, 0);

		if (nRead <= 0)
		{
			return false;
		}

		wanted -= nRead;
		nBuffered += nRead;
	}

	return true;
}

bool CFileSender::QueryStatus()
{
	if (!PackAndSendRequest(CMD_PARAM_TRANSFILE_C2S_QUERYSTATUS))
	{
		return false;
	}
			
	int nDataToRead = sizeof(FilePackHeader);
	FilePackHeader header; 
	if (!ReadExact(m_Socket,(char *) &header, nDataToRead))
	{
		MessageBox(m_ParentHwnd, "发送本地文件数据失败！", "提示", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	FilePackHeader header1; 
	if (!ReadExact(m_Socket,(char *) &header1, nDataToRead))
	{
		MessageBox(m_ParentHwnd, "发送本地文件数据失败！", "提示", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	if(header1.nParam == CMD_PARAM_TRANSFILE_SSTOPRECEIVE && header1.nType != FILE_RESPONSE)
	{
		bool bStopByClient = false;
		SendMessage(m_ParentHwnd, WM_GETSTOPBYMYSEFLT, 0, (LPARAM)&bStopByClient);
		if(m_bStopbyServer == false && !bStopByClient)
		{
			m_bStopbyServer = true;
			PackAndSendRequest(CMD_PARAM_TRANSFILE_C2S_STOP); 
			PostMessage(this->m_ParentHwnd, WM_FILE_STOP, 0, 0);
			
		}
		return false;
	}
	

	if (header.nType != FILE_RESPONSE || header.nParam != CMD_PARAM_TRANSFILE_C2S_STATUSOK)
	{
		PackAndSendRequest(CMD_PARAM_TRANSFILE_C2S_STOP); 			
		PostMessage(m_ParentHwnd, WM_FILE_ERROR, 1, 0);
		return false;
	}


	return true; 
}

void CFileSender::DoSend()
{
	m_bRun = true;
	int nRead = 0;

	m_bStopbyServer = false;
	int nWriteCount = 100; 
	SendMessage(m_ParentHwnd, WM_SETSENDING, 1, 0);
	SendMessage(m_ParentHwnd, WM_SETSTOPBYMYSEFLT, 0, 0);
	while (nRead < m_pFile->GetLength())
	{
		if (m_bRun == false)
		{
			QueryStatus();
			if (!PackAndSendRequest(CMD_PARAM_TRANSFILE_C2S_STOP))
			{
				SendMessage(m_ParentHwnd, WM_SETSENDING, 0, 0);
				return;
			}
			break;
		}

		if (nWriteCount ++ >= 100)
		{
			nWriteCount = 0; 
			if (!QueryStatus()) 
			{
				SendMessage(m_ParentHwnd, WM_SETSENDING, 0, 0);
				return ; 
			}
		}

		if (!PackAndSendRequest(CMD_PARAM_TRANSFILE_C2S_DATA))
		{
			SendMessage(m_ParentHwnd, WM_SETSENDING, 0, 0);
			return;
		}

		FileData fileData;

		int nCount = m_pFile->Read(fileData.Buf, 4096);
		fileData.nLength = nCount;

		if (!WriteExact(m_Socket, (char*)(&fileData), sizeof(fileData)))
		{
			MessageBox(m_ParentHwnd, "发送本地文件数据失败！", "提示", MB_OK | MB_ICONEXCLAMATION);
			return;
		}

		nRead += nCount;

		
		
		SendMessage(m_ParentHwnd, WM_FILE_SENDFILETOSERVER_PROGRESS_MESSAGE, (WPARAM)(nRead), 0);
	}
}

void CFileSender::OnThreadStop()
{
	if (m_pFile != NULL)
	{
		SendMessage(m_ParentHwnd, WM_FILE_SENDFILETOSERVER_FINISH_MESSAGE, (WPARAM)(&(m_pFile->GetFileName())), 0);
		m_pFile->Close();
		delete m_pFile;
		m_pFile = NULL;
	}

	if (QueryStatus() && !m_bStopbyServer) {
		PackAndSendRequest(CMD_PARAM_TRANSFILE_C2S_FINISH);
	}
	//PackAndSendRequest(CMD_PARAM_TRANSFILE_C2S_STOP);
	SendMessage(m_ParentHwnd, WM_SETSENDING, 0, 0);
	SendMessage(m_ParentHwnd, WM_FILE_FILETOSERVERSTOP_MESSAGE, 0, 0);	
	SendMessage(m_ParentHwnd, WM_FILE_SENDFILETOSERVER_FINISHED_MESSAGE, 0, 0);//refresh list
}

void CFileSender::StopSending()
{
	m_bRun = false;
}
