#pragma once

#include "FileDefines.h"

class CDestFolderHelper
{
public:
	CDestFolderHelper();
	~CDestFolderHelper(void);

public:
	bool IsDestFolderExist();
	bool IsDestFileExist(CString DestFileName);
	void SetDestFolder(const char* pDestFolder);

	BasicFileInfoList* GetFileListInDestFolder(int* nFileCount);

private:
	CString m_DestFolder;
};
