#include "StdAfx.h"
#include ".\keyword.h"
#include <io.h>
CKeyWord::CKeyWord(void)
{
}

CKeyWord::~CKeyWord(void)
{
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//0成功,1失败,2关键词已经存在
int CKeyWord::AddKeyWord(CString strKeyWord)
{
// 	strKeyWord.MakeLower();
// 	if ( IsInKeyMap( strKeyWord ) )
// 	{
// 		return 2;	//已经存在
// 	}
// 	FILE *fp;
// 	char szfile[1000];
// 	sprintf(szfile,m_strFileName);
// 	fp=fopen(szfile,"a+");
// 	fprintf(fp,"%s\r\n",strKeyWord);
// 	fclose(fp);
// 	AddtoKeyMap(strKeyWord);
	return 0;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//1成功,0已经存在
int CKeyWord::AddtoKeyMap(CString strKeyWord)
{
	strKeyWord.MakeLower();
	try
	{
		if ( m_keyWordMap[strKeyWord] == 1)
		{
			return 0;
		}
		m_keyWordMap[strKeyWord] = 1;	//1为存在
		return 1;
	}	
	catch (...)
	{
		return 2;
	}	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//判断关键词是否已经在MAP中
bool CKeyWord::IsInKeyMap(CString strKeyWord)
{
	strKeyWord.MakeLower();
	bool existFlag = false;
	if (m_keyWordMap[strKeyWord] == 1)				//存在
	{
		existFlag = true;
	}
	else					//存在
	{
		existFlag = false;
	}
	return existFlag;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//0正常，-1异常,1没有非法关键词
int CKeyWord::KeyWordCheck(CString strSRC,CString &resultStr)
{
	resultStr = "";
	map<CString,int>::iterator it;   
	CString tempStr = "";
	int idx = 0;
	try
	{
		for(it=m_keyWordMap.begin();it!=m_keyWordMap.end();++it)
		{
			tempStr = it->first;
			idx = strSRC.Find(tempStr,0);  //找第一个记录
			if(idx != -1)//如果找到
			{
				resultStr = tempStr;
				return 0;
			}				
		}
		return 1;
	}
	catch (...)
	{
	}
	return -1;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//0成功,-1失败,-2连接数据库失败，-3异常,-4没有关键词
int CKeyWord::LoadAllKeyWords(int &iCount)
{
	iCount = 0;
	m_keyWordMap.clear();
	try
	{
		if(_access(m_strFileName,0)!=0)
		{
			return -1;
		}
		CStdioFile   file;
		CString   cstr ="";
		CFileException   fe;
		if(file.Open(m_strFileName,CFile::modeRead,&fe))   
		{ 
			file.SeekToBegin();
			while (file.ReadString(cstr))
			{
				cstr.Trim();
				if (cstr != "")
				{
					if ( AddtoKeyMap(cstr) == 1)
					{
						iCount ++;
					}
				}
			}
			file.Close();
		}
		if ( iCount > 0 )
		{
			return 0;
		}
		return -4;
	}
	catch(...)
	{
		return -3;
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//获取字符串长度
int CKeyWord::GetMsgULen(CString Msg)
{
	if (Msg.GetLength() > 10000)
	{
		return -1;
	}
	wchar_t wszTemp[10000] = {0};
	try
	{
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Msg, strlen(Msg), wszTemp, sizeof(wszTemp));
		int len = wcslen(wszTemp);
		if (len < 0)
		{
			return -3;
		}
		return len;
	}
	catch(...)
	{
	}
	return -9;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//获取与查询项匹配的关键词
int CKeyWord::GetQueryKeys(CString strKey,CListCtrl &listctrl,int &iCount,BOOL *iExit)
{
	try
	{
		iCount = 0;
		listctrl.DeleteAllItems();
		map<CString,int>::iterator it;   
		//打印出map中的key和value值
		CString strTempSRC = "";
		for(it=m_keyWordMap.begin();it!=m_keyWordMap.end();++it)   
		{   
			strTempSRC = strKey;
			CString tempStr = it->first;
			int iFlag = it->second;
			if (iFlag == 1)
			{
				if ( tempStr.Find(strTempSRC) >= 0 )
				{
					iCount++;
					int iCount = listctrl.GetItemCount();
					listctrl.InsertItem( iCount, "");
					listctrl.SetItemText( iCount , 0, tempStr );
					listctrl.PostMessage(WM_VSCROLL,SB_TOP,0);	
				}			
			}		
		}
	}
	catch(...)
	{
		
		return -3;
	}
	return 0;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//导出关键词
int CKeyWord::OutPortKeys(CString strFilePath)
{
	map<CString,int>::iterator it;
	int iCount = 0;
	CString strOut="";

	for(it=m_keyWordMap.begin();it!=m_keyWordMap.end();++it)   
	{   
		CString tempStr = it->first;
		int iFlag = it->second;
		if (iFlag == 1)
		{
			strOut += tempStr +"\r\n";
			iCount++;
		}		
	}
	if (strOut == "")
	{
		return 0;
	}

	try
	{
		CFile file;
		if (file.Open(strFilePath,CFile::modeCreate|CFile::modeWrite))
		{
			file.Write(strOut,strOut.GetLength());
			file.Close();
			return iCount;
		}
		else
		{
			return -1;
		}
	}
	catch (...)
	{
		return -1;
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//设置关键词路径
void CKeyWord::SetDBInfo(CString strPath)
{
	m_strFileName = strPath;
}