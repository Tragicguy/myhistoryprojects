#include "StdAfx.h"
#include "DestFolderHelper.h"


CDestFolderHelper::CDestFolderHelper()
{
}

CDestFolderHelper::~CDestFolderHelper(void)
{
}

void CDestFolderHelper::SetDestFolder(const char* pDestFolder)
{
	m_DestFolder.Format("%s",pDestFolder);

	if (m_DestFolder.Right(1) != "\\")
	{
		//m_DestFolder += "\\";
	}
}

bool CDestFolderHelper::IsDestFolderExist()
{
	WIN32_FIND_DATA fd;

	HANDLE hdTemp = FindFirstFile(this->m_DestFolder, &fd);

	if (hdTemp !=INVALID_HANDLE_VALUE)
	{
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			FindClose(hdTemp);
			return true;
		}
		else
		{
			FindClose(hdTemp);
			return false;
		}
	}

	//***Pan peng 2008-12-16
	//if m_DestFolder is local dirver, return true. 
	//just for C: because FindFirstFile does not work for C:.
	if(m_DestFolder.GetLength() == 2 && m_DestFolder.GetAt(1) == ':')
		return true;
	//end***

	return false;
}

bool CDestFolderHelper::IsDestFileExist(CString DestFileName)
{
	WIN32_FIND_DATA fd;

	HANDLE hdTemp = FindFirstFile(DestFileName, &fd);

	if (hdTemp !=INVALID_HANDLE_VALUE)
	{
		FindClose(hdTemp);
		return true;
	}

	//***Pan peng 2008-11-28
	if(GetDriveType(DestFileName) != DRIVE_UNKNOWN)
	{
		FindClose(hdTemp);
		return true;
	}
	//end***

	return false;
}

BasicFileInfoList* CDestFolderHelper::GetFileListInDestFolder(int* nFileCount)
{
	m_DestFolder += ("\\*.*");

	CFileFind m_FileFind;
	int Count = 0;
	BasicFileInfoList* pFileList = NULL;
	BasicFileInfoList* pHead = NULL;

	BOOL bWorking = m_FileFind.FindFile(m_DestFolder);

	if (!bWorking)
	{
		return NULL;
	}
	else
	{
		while (bWorking)
		{
			bWorking = m_FileFind.FindNextFile();

			if (m_FileFind.IsDots())
				continue;

			if (!m_FileFind.IsDirectory())
			{
				if (Count == 0)
				{
					pFileList = new BasicFileInfoList();
					pHead = pFileList;
				}
				else
				{
					BasicFileInfoList* pCurrent = pFileList;
					pFileList = new BasicFileInfoList();
					pCurrent->pNext = pFileList;
				}
				m_FileFind.GetLastWriteTime(&(pFileList->fileInfo.CreationTime));
				char* pFileName = new char[MAX_PATH];
				memset(pFileName, 0, MAX_PATH);
				memset(&(pFileList->fileInfo.FileName), 0, MAX_PATH);
				lstrcpy(pFileName, m_FileFind.GetFileName());

				CopyMemory(&(pFileList->fileInfo.FileName), pFileName, m_FileFind.GetFileName().GetLength());
				pFileList->fileInfo.Size = m_FileFind.GetLength();
				delete [] pFileName;

				Count++;
				pFileList->pNext = NULL;
			}
			//***Add by Pan peng 2008-11-27
			else
			{
				if (Count == 0)
				{
					pFileList = new BasicFileInfoList();
					pHead = pFileList;
				}
				else
				{
					BasicFileInfoList* pCurrent = pFileList;
					pFileList = new BasicFileInfoList();
					pCurrent->pNext = pFileList;
				}
				m_FileFind.GetLastWriteTime(&(pFileList->fileInfo.CreationTime));
				char* pFileName = new char[MAX_PATH];
				memset(pFileName, 0, MAX_PATH);
				memset(&(pFileList->fileInfo.FileName), 0, MAX_PATH);
				CString strFileName = _T("(");
				strFileName = strFileName + m_FileFind.GetFileName();
				strFileName = strFileName + _T(")");
				lstrcpy(pFileName, strFileName);

				CopyMemory(&(pFileList->fileInfo.FileName), pFileName, strFileName.GetLength());
				pFileList->fileInfo.Size = -1;
				delete [] pFileName;

				Count++;
				pFileList->pNext = NULL;
			}

			//end***
		}

		m_FileFind.Close();

		*nFileCount = Count;
		return pHead;
	}
}
